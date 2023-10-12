#pragma once

#include "common.h"
#include "optional.h"

namespace tl {

template <class T, umm _values_per_block>
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

		bool get_mask(umm index) {
			assert(index < values_per_block);
			return masks[index / bits_in_mask] & ((Mask)1 << (index % bits_in_mask));
		}
	};

	Allocator allocator = current_allocator;
	Block first;
	Block *last = &first;

	StaticMaskedBlockList &operator=(StaticMaskedBlockList const &that) = delete;
	StaticMaskedBlockList &operator=(StaticMaskedBlockList &&that) = delete;

	struct Added {
		T *pointer;
		umm index;
	};

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
				u32 value_index = value - block->values;
				u32 mask_index = value_index / bits_in_mask;
				u32 bit_index  = value_index % bits_in_mask;

				bounds_check(block->masks[mask_index] & ((Mask)1 << bit_index), "attempt to remove already deleted value from StaticMaskedBlockList");

				block->masks[mask_index] &= ~((Mask)1 << bit_index);

				return;
			}
			block = block->next;
		}
		bounds_check(false, "attempt to remove non-existant value from StaticMaskedBlockList");
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

		bool operator==(Iterator const &that) { return list == that.list && block == that.block && index == that.index; }
		bool operator!=(Iterator const &that) { return !(*this == that); }
	};

	Iterator begin() {
		Iterator result = {this, &first, 0};
		if (!first.get_mask(0))
			++result;
		return result;
	}
	Iterator end() { return {this, last, values_per_block}; }
};

template <class T, umm values_per_block>
void free(StaticMaskedBlockList<T, values_per_block> &list) {
	auto block = list.first.next;
	while (block) {
		list.allocator.free(block);
		block = block->next;
	}
}

template <class T, umm values_per_block>
Optional<umm> index_of(StaticMaskedBlockList<T, values_per_block> const &list, T const *value) {
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
void for_each(StaticMaskedBlockList<T, values_per_block> &list, Fn &&fn) {
	using StaticMaskedBlockList = StaticMaskedBlockList<T, values_per_block>;
	using Mask = StaticMaskedBlockList::Mask;
	constexpr u32 bits_in_mask = StaticMaskedBlockList::bits_in_mask;
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
