#pragma once

#include "common.h"

namespace TL {

template <class T, umm _values_per_block = 256>
struct BlockListMasked {
	static constexpr u32 values_per_block = _values_per_block;

	using Mask = umm;

	struct Block {
		Block *next;
		Mask mask[values_per_block / (sizeof(Mask) * 8)];
		union {
			T values[values_per_block];
		};
	};

	Block *first = 0;
	Block *last = 0;

	T &add() {
		auto block = last;

		u32 block_index = 0;

		while (1) {
			if (!block) {
				block = (Block *)ALLOCATE(u8, default_allocator, sizeof(Block) + value_size * values_per_block);
				if (last) {
					last->next = block;
				} else {
					first = block;
				}
				last = block;
			}

			// Search for free space in current block
			for (u32 mask_index = 0; mask_index < count_of(block->mask); ++mask_index) {
				auto &mask = block->mask[mask_index];

				if (mask == ~0)
					continue;

				auto bit_index = find_lowest_zero_bit(mask);
				auto value_index = (mask_index * (sizeof(mask) * 8)) + bit_index;

				mask |= (Mask)1 << bit_index;

				return block->values[value_index] = {};
			}

			// No space here, go to next block
			block = block->next;
			++block_index;
		}
	}
};

template <class T, umm values_per_block>
void free(BlockListMasked<T, values_per_block> &list) {

}

}
