// VirtualMaskedArray allows you to reserve a huge contiguous range of virtual memory.
// It will automatically commit the memory on addition and decommit on deletion.

#pragma once
#include "common.h"
#include "memory.h"

namespace tl {

extern umm const vma_page_size;

// If page size is 4096, one page contains 32768 bits

struct VMAControl {
	u32 count = 0;
};

// [elem]...64 total...[elem][elem]...64 total...[elem]..........................[elem]...64 total...[elem]
// [          mask          ][          mask          ]..........................[          mask          ]
// [                          1x control per `page_size * 8` bits worth of masks                          ]


// [ ][ ][ ][ ]


template <class T>
struct VirtualMaskedArray {
	T *data = 0;
	u64 *masks = 0;
	VMAControl *controls = 0;
	umm capacity = 0;

	void init(umm capacity) {
		umm bits_in_one_page = vma_page_size * 8;

		capacity = ceil(capacity, bits_in_one_page);

		umm control_count = capacity / bits_in_one_page;

		this->capacity = capacity;
		data = (T *)reserve_memory(capacity * sizeof(T));
		masks = (u64 *)reserve_memory(capacity / 8); // NOTE: capacity is already ceiled to page size, which is definitely bigger than 64, so no need to worry about allocating half of a mask.
		controls = DefaultAllocator{}.allocate<VMAControl>(control_count);
	}

	T &add(T const &value = {}) {
		umm const bits_in_one_page = vma_page_size * 8;
		umm const masks_in_one_page = bits_in_one_page / 64;

		for (umm control_index = 0; control_index < control_count(); ++control_index) {
			auto &control = controls[control_index];

			if (control.count == bits_in_one_page)
				continue;
		
			umm mask_index_start = control_index * masks_in_one_page;
			umm mask_index_end = mask_index_start + masks_in_one_page;

			if (control.count == 0) {
				commit_memory(masks + mask_index_start, vma_page_size);
			}

			control.count += 1;

			for (umm mask_index = mask_index_start; mask_index < mask_index_end; ++mask_index) {
				auto &mask = masks[mask_index];

				if (mask == ~0)
					continue;

				auto bit_index = find_lowest_zero_bit(mask);

				mask |= 1ull << bit_index;

				return *new(&data[mask_index * 64 + bit_index]) T(value);
			}
		}

		bounds_check(invalid_code_path("Not enough space in VirtualMaskedArray"));
	}

	bool owns(T *value) {
		return (umm)(value - data) < capacity;
	}

	bool for_each(auto &&in_fn) {
		auto fn = wrap_foreach_fn<T>(in_fn);
		
		umm const bits_in_one_page = vma_page_size * 8;
		umm const masks_in_one_page = bits_in_one_page / 64;

		for (umm control_index = 0; control_index < control_count(); ++control_index) {
			auto &control = controls[control_index];

			if (control.count == 0)
				continue;
		
			umm mask_index_start = control_index * masks_in_one_page;
			umm mask_index_end = mask_index_start + masks_in_one_page;

			for (umm mask_index = mask_index_start; mask_index < mask_index_end; ++mask_index) {
				auto &mask = masks[mask_index];
				auto remaining_mask = mask;

				while (remaining_mask) {
					auto bit_index = find_lowest_one_bit(remaining_mask);
					remaining_mask &= ~(1ull << bit_index);
					
					auto directive = fn(data[mask_index * 64 + bit_index]);

					if (directive & (ForEach_erase | ForEach_erase_unordered))
						mask &= ~(1ull << bit_index);

					if (directive & ForEach_break)
						return true;
				}
			}
		}

		return false;
	}

	void clear() {
		if constexpr (std::is_trivially_destructible_v<T>) {
			#error not implemented
		} else {
			not_implemented();
		}
	}


	umm mask_count() const { return capacity / 64; }
	umm control_count() const { return capacity / (vma_page_size * 8); }
};

}

#ifdef TL_IMPL

namespace tl {

umm const vma_page_size = get_page_size();

}

#endif
