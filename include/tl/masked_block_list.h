#pragma once

#include "common.h"

namespace tl {

template <class T, umm _values_per_block>
struct MaskedBlockList {
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

	T &operator[](umm index) {
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
};

template <class T, umm values_per_block>
void free(MaskedBlockList<T, values_per_block> &list) {

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

}
