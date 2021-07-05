#pragma once

#include "common.h"

namespace TL {

template <class T, umm _values_per_block>
struct MaskedBlockList {
	static constexpr u32 values_per_block = _values_per_block;

	using Mask = umm;
	static constexpr u32 bits_in_mask = sizeof(Mask) * 8;

	struct Block {
		Block *next = 0;
		u32 free_mask_count = count_of(mask);
		Mask mask[values_per_block / bits_in_mask] = {};
		union {
			T values[values_per_block];
		};
	};

	Block first;
	Block *last = &first;

	T &add(T that) {
		auto block = last;

		while (1) {
			if (!block) {
				block = default_allocator.allocate<Block>();
				last->next = block;
				last = block;
			}

			if (block->free_mask_count != 0) {
				// Search for free space in current block
				for (u32 mask_index = 0; mask_index < count_of(block->mask); mask_index += 1) {
					auto &mask = block->mask[mask_index];

					if (mask == ~0)
						continue;

					auto bit_index = find_lowest_zero_bit(mask);
					auto value_index = (mask_index * bits_in_mask) + bit_index;

					mask |= (Mask)1 << bit_index;

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
};

template <class T, umm values_per_block>
void free(MaskedBlockList<T, values_per_block> &list) {

}

}
