#pragma once

#include "common.h"
#include "optional.h"

namespace tl {

template <class T, umm _values_per_block>
struct MaskedBlockList {
	using ValueType = T;

	static constexpr u32 values_per_block = _values_per_block;

	using Mask = umm;
	static constexpr u32 bits_in_mask = sizeof(Mask) * 8;

	struct Block {
		Block *next = 0;
		u32 free_mask_count = count_of(masks);
		Mask masks[values_per_block / bits_in_mask] = {};
		union {
			T values[values_per_block];
		};
		Block() {}
	};

	Allocator allocator = current_allocator;
	Block first;
	Block *last = &first;

	MaskedBlockList &operator=(MaskedBlockList const &that) = delete;
	MaskedBlockList &operator=(MaskedBlockList &&that) = delete;

	T &add(T that) {
		auto block = last;

		while (1) {
			if (!block) {
				block = allocator.allocate<Block>();
				last->next = block;
				last = block;
			}

			if (block->free_mask_count != 0) {
				// Search for free space in current block
				for (u32 mask_index = 0; mask_index < count_of(block->masks); mask_index += 1) {
					auto mask = block->masks[mask_index];

					if (mask == ~0)
						continue;

					auto bit_index = find_lowest_zero_bit(mask);
					auto value_index = (mask_index * bits_in_mask) + bit_index;

					mask |= (Mask)1 << bit_index;
					block->masks[mask_index] = mask;

					if (mask == ~0)
						block->free_mask_count -= 1;

					return block->values[value_index] = that;
				}
			}

			// No space here, go to next block
			block = block->next;
		}
	}

	T &add() {
		return add(T{});
	}

	void remove(T *value) {
		auto block = &first;
		while (block) {
			if (block->values <= value && value < block->values + values_per_block) {
				u32 value_index = value - block->values;
				u32 mask_index = value_index / bits_in_mask;
				u32 bit_index  = value_index % bits_in_mask;

				block->masks[mask_index] &= ~((Mask)1 << bit_index);

				return;
			}
			block = block->next;
		}
		bounds_check(false, "attempt to remove non-existant value from MaskedBlockList");
	}

	T &at(umm index) {
		umm block_index = index / values_per_block;
		umm value_index = index % values_per_block;

		auto block = &first;
		while (block_index) {
			block = block->next;
			--block_index;
		}

		bounds_check(block->masks[value_index / bits_in_mask] & (1 << (value_index % bits_in_mask)), "attempt to index uninitialized value in MaskedBlockList");

		return block->values[value_index];
	}
	T &operator[](umm index) {
		return at(index);
	}
};

template <class T, umm values_per_block>
void free(MaskedBlockList<T, values_per_block> &list) {
	auto block = list.first.next;
	while (block) {
		list.allocator.free(block);
		block = block->next;
	}
}

template <class T, umm values_per_block>
Optional<umm> index_of(MaskedBlockList<T, values_per_block> const &list, T const *value) {
	auto block = &list.first;
	while (block) {
		if (block->values <= value && value < block->values + values_per_block) {
			return value - block->values;
		}
		block = block->next;
	}
	return {};
}


template <class T, umm values_per_block, class Fn>
void for_each(MaskedBlockList<T, values_per_block> &list, Fn &&fn) {
	using MaskedBlockList = MaskedBlockList<T, values_per_block>;
	using Mask = MaskedBlockList::Mask;
	constexpr u32 bits_in_mask = MaskedBlockList::bits_in_mask;
	auto block = &list.first;
	while (block) {
		for (u32 value_index = 0; value_index < values_per_block; ++value_index) {
			u32 mask_index = value_index / bits_in_mask;
			u32 bit_index  = value_index % bits_in_mask;
			if (block->masks[mask_index] & ((Mask)1 << bit_index)) {
				fn(block->values[value_index]);
			}
		}
		block = block->next;
	}
}

}
