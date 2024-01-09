#pragma once

#include "common.h"
#include "optional.h"

namespace tl {

template <class T, umm _values_per_block, class Allocator = Allocator>
struct StaticMaskedBlockList {
	using ValueType = T;

	static constexpr umm values_per_block = _values_per_block;

	using Mask = umm;
	static constexpr umm bits_in_mask = sizeof(Mask) * 8;

	struct Block {
		Block *next = 0;
		umm available_mask_count = count_of(masks);
		Mask masks[values_per_block / bits_in_mask] = {};
		union {
			T values[values_per_block];
		};
		Block() {}

		bool get_mask(umm index) const {
			assert(index < values_per_block);
			return masks[index / bits_in_mask] & ((Mask)1 << (index % bits_in_mask));
		}

		bool owns(T *value) const {
			umm index = value - values;
			if (index >= values_per_block)
				return false;

			return get_mask(index);
		}
		void clear() {
			memset(masks, 0, sizeof(masks));
			available_mask_count = count_of(masks);
		}
	};

	struct Iterator {
		StaticMaskedBlockList *list = 0;
		Block *block = 0;
		umm index = 0;

		T &operator*() { return block->values[index]; }
		T *operator->() { return &block->values[index]; }

		Iterator &operator++() {
			do {
				index++;
				if (index == values_per_block) {
					if (block == list->last) {
						break;
					} else {
						block = block->next;
						index = 0;
					}
				}
			} while (!block->get_mask(index));
			return *this;
		}

		Iterator operator++(int) { auto it = *this; ++*this; return it; }

		bool operator==(Iterator const &that) const { return list == that.list && block == that.block && index == that.index; }
		bool operator!=(Iterator const &that) const { return !(*this == that); }
	};

	struct Added {
		T *pointer;
		umm index;
	};

	Block first;
	Block *last = &first;
	umm count = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	Iterator begin() {
		Iterator result = {this, &first, 0};
		if (!first.get_mask(0))
			++result;
		return result;
	}
	Iterator end() { return {this, last, values_per_block}; }
	StaticMaskedBlockList &operator=(StaticMaskedBlockList const &that) = delete;
	StaticMaskedBlockList &operator=(StaticMaskedBlockList &&that) = delete;

	Added add(T const &that = {}) {
		auto block = &first;

		umm block_index = 0;

		while (1) {
			if (!block) {
				block = allocator.allocate<Block>();
				last->next = block;
				last = block;
			}

			if (block->available_mask_count != 0) {
				// Search for free space in current block
				for (umm mask_index = 0; mask_index < count_of(block->masks); mask_index += 1) {
					auto mask = block->masks[mask_index];

					if (mask == ~0)
						continue;

					auto bit_index = find_lowest_zero_bit(mask);
					auto value_index = (mask_index * bits_in_mask) + bit_index;

					mask |= (Mask)1 << bit_index;
					block->masks[mask_index] = mask;

					if (mask == ~0)
						block->available_mask_count -= 1;

					block->values[value_index] = that;

					++count;

					return {
						.pointer = &block->values[value_index],
						.index = block_index * values_per_block + value_index,
					};
				}
			}

			// No space here, go to next block
			block = block->next;
			block_index += 1;
		}
		return {};
	}

	void erase_at(umm index) {
		auto block = &first;
		while (block) {
			if (index < values_per_block) {
				u32 mask_index = index / bits_in_mask;
				u32 bit_index  = index % bits_in_mask;

				bounds_check(block->masks[mask_index] & ((Mask)1 << bit_index), "attempt to remove already deleted value from StaticMaskedBlockList");

				if (block->masks[mask_index] == ~0)
					block->available_mask_count += 1;

				block->masks[mask_index] &= ~((Mask)1 << bit_index);

				--count;

				return;
			}
			block = block->next;
			index -= values_per_block;
		}
		bounds_check(false, "attempt to remove non-existant value from StaticMaskedBlockList");
	}
	void erase(T *value) {
		auto block = &first;
		while (block) {
			if (block->values <= value && value < block->values + values_per_block) {
				umm value_index = value - block->values;
				umm mask_index = value_index / bits_in_mask;
				umm bit_index  = value_index % bits_in_mask;

				bounds_check(block->masks[mask_index] & ((Mask)1 << bit_index), "attempt to remove already deleted value from StaticMaskedBlockList");

				block->masks[mask_index] &= ~((Mask)1 << bit_index);
				
				--count;

				return;
			}
			block = block->next;
		}
		bounds_check(false, "attempt to remove non-existant value from StaticMaskedBlockList");
	}
	void erase(Iterator it) {
		umm mask_index = it.index / bits_in_mask;
		umm bit_index  = it.index % bits_in_mask;
		it.block->masks[mask_index] &= ~((Mask)1 << bit_index);
		--count;
	}

	void clear() {
		for (auto block = &first; block; block = block->next) {
			block->clear();
		}
		count = 0;
	}

	T &at(umm index) {
		umm block_index = index / values_per_block;
		umm value_index = index % values_per_block;

		auto block = &first;
		while (block_index) {
			block = block->next;
			--block_index;
		}

		bounds_check(block->masks[value_index / bits_in_mask] & ((Mask)1 << (value_index % bits_in_mask)), "attempt to index uninitialized value in StaticMaskedBlockList");

		return block->values[value_index];
	}
	T &operator[](umm index) {
		return at(index);
	}

	bool owns(T *value) const {
		for (auto block = &first; block; block = block->next) {
			if (block->owns(value)) {
				return true;
			}
		}
		return false;
	}
};

TL_DECLARE_CONCEPT(StaticMaskedBlockList);

void free(CStaticMaskedBlockList auto &list) {
	auto block = list.first.next;
	while (block) {
		list.allocator.free(block);
		block = block->next;
	}
}

template <CStaticMaskedBlockList StaticMaskedBlockList>
Optional<umm> index_of(StaticMaskedBlockList const &list, typename StaticMaskedBlockList::ValueType const *value) {
	auto block = &list.first;
	while (block) {
		if (block->values <= value && value < block->values + StaticMaskedBlockList::values_per_block) {
			return value - block->values;
		}
		block = block->next;
	}
	return {};
}


//template <CStaticMaskedBlockList StaticMaskedBlockList>
//void for_each(StaticMaskedBlockList &list, auto &&fn) requires requires { fn(*(typename StaticMaskedBlockList::ValueType *)0); } {
//	using Mask = typename StaticMaskedBlockList::Mask;
//	constexpr umm bits_in_mask = StaticMaskedBlockList::bits_in_mask;
//	auto block = &list.first;
//	while (block) {
//		for (umm value_index = 0; value_index < StaticMaskedBlockList::values_per_block; ++value_index) {
//			umm mask_index = value_index / bits_in_mask;
//			umm bit_index  = value_index % bits_in_mask;
//			if (block->masks[mask_index] & ((Mask)1 << bit_index)) {
//				fn(block->values[value_index]);
//			}
//		}
//		block = block->next;
//	}
//}

template <class T, umm values_per_block, class Allocator>
bool for_each(StaticMaskedBlockList<T, values_per_block, Allocator> &list, auto &&fn) requires requires { fn(*(T *)0); } {
	using Mask = typename StaticMaskedBlockList<T, values_per_block, Allocator>::Mask;
	constexpr umm bits_in_mask = list.bits_in_mask;
	auto block = &list.first;
	while (block) {
		for (umm value_index = 0; value_index < values_per_block; ++value_index) {
			umm mask_index = value_index / bits_in_mask;
			umm bit_index  = value_index % bits_in_mask;
			if (block->masks[mask_index] & ((Mask)1 << bit_index)) {
				using Ret = decltype(fn(*(T *)0));
				if constexpr (std::is_same_v<Ret, void>) {
					fn(block->values[value_index]);
				} else if constexpr (std::is_same_v<Ret, ForEachDirective>) {
					switch (fn(block->values[value_index])) {
						case ForEach_continue: break;
						case ForEach_break: return true;
						case ForEach_erase: 
						case ForEach_erase_unordered: 
							block->masks[mask_index] &= ~((Mask)1 << bit_index);
							--list.count;
							break;
						default: invalid_code_path("Bad for_each directive");
					}

				} else {
					static_error_v(fn, "Invalid return type");
				}
			}
		}
		block = block->next;
	}

	return false;
}

}
