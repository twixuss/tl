#pragma once
#include "common.h"
#include "hash.h"

namespace tl {

enum class ContiguousHashSetCellState : u8 {
	empty    = 0,
	occupied = 1,
	removed  = 2,
};

#ifndef TL_INITIAL_CONTIGUOUS_HASH_SET_CAPACITY
#define TL_INITIAL_CONTIGUOUS_HASH_SET_CAPACITY 8
#endif

static_assert(is_power_of_2(TL_INITIAL_CONTIGUOUS_HASH_SET_CAPACITY));

template <class Key, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
struct ContiguousHashSet : Traits {
	TL_USE_HASH_TRAITS_MEMBERS;

	using CellState = ContiguousHashSetCellState;
	using Element = Key;

	// TODO: SOA, store hash
	struct Cell {
		CellState state;
		Key key;
	};

	Span<Cell> cells;
	umm count = 0;

	[[no_unique_address]] Allocator allocator = Allocator::current();
	
	Key const &get_or_insert(Key key TL_LP) {
		auto hash = get_hash(key);
		u64 index = 0;
		if (cells.count) {
			index = get_index_from_hash(hash, cells.count);
			auto steps = cells.count;
			while (steps--) {
				auto &cell = cells.data[index];
				if (cell.state == CellState::empty) {
					break;
				}
				if (cell.state == CellState::occupied) {
					if (are_equal(cell.key, key)) {
						return cell.key;
					}
				}
				index = step(index, cells.count);
			}

			if (count >= cells.count * rehash_percentage / 100) {
				if (reserve(cells.count * 2 TL_LA)) {
					hash = get_hash(key);
					index = get_index_from_hash(hash, cells.count);
				}
			}
		} else {
			// NOTE: TL_INITIAL_CONTIGUOUS_HASH_SET_CAPACITY will be used inside `reserve`
			reserve(1 TL_LA);
			index = get_index_from_hash(hash, cells.count);
		}
		count += 1;
		defer { assert(find(key)); };

		while (cells.data[index].state == CellState::occupied) {
			on_collision(cells.data[index].key, key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::occupied;
		cell.key = key;
		return cell.key;
	}
	void insert(Key const &key) {
		get_or_insert(key);
	}

	bool reserve(umm desired TL_LP) {
		if (desired <= cells.count)
			return false;

		Span<Cell> new_cells;
		new_cells.count = max((umm)TL_INITIAL_CONTIGUOUS_HASH_SET_CAPACITY, cells.count);
		while (new_cells.count < desired) {
			new_cells.count *= 2;
		}
		assert(is_power_of_2(new_cells.count));

		new_cells.data = allocator.template allocate_uninitialized<Cell>(new_cells.count TL_LA);
		for (auto &new_cell : new_cells) {
			new_cell.state = CellState::empty;
		}

		for (auto &old_cell : cells) {
			if (old_cell.state == CellState::occupied) {
				insert_into(new_cells, old_cell.key);
			}
		}

		allocator.free_t(cells.data, cells.count);
		cells = new_cells;

		return true;
	}

	umm step(umm index, umm cells_count) const {
		return (index + 1) & (cells_count - 1);
	}

	Key const &insert_into(Span<Cell> cells, Key key) {
		auto hash = get_hash(key);
		auto index = get_index_from_hash(hash, cells.count);

		while (cells.data[index].state == CellState::occupied) {
			on_collision(cells.data[index].key, key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::occupied;
		return cell.key = key;
	}

	Key const *find(Key key) const {
		if (cells.count == 0)
			return 0;

		auto hash = get_hash(key);
		auto index = get_index_from_hash(hash, cells.count);
		auto steps = cells.count;
		while (steps--) {
			auto &cell = cells.data[index];
			if (cell.state == CellState::empty) {
				return 0;
			}
			if (cell.state == CellState::occupied) {
				if (are_equal(cell.key, key)) {
					return &cell.key;
				}
			}
			index = step(index, cells.count);
		}

		return 0;
	}

	void clear() {
		for (auto &cell : cells) {
			cell.state = CellState::empty;
		}
		count = 0;
	}
	void erase(Key key) {
		bounds_check(assert(cells.count));

		auto hash = get_hash(key);
		auto index = get_index_from_hash(hash, cells.count);

		auto cell = cells.data + index;

		auto steps = cells.count;
		while (steps--) {
			auto &cell = cells.data[index];
			if (cell.state == CellState::occupied) {
				if (are_equal(cell.key, key)) {
					cell.state = CellState::removed;
					--count;
				}
			}
			index = step(index, cells.count);
		}
	}

	
	struct Iterator {
		ContiguousHashSet *set = 0;
		Cell *cell = 0;

		Iterator &operator++() {
			do {
				++cell; 
				if (cell == set->cells.end()) {
					cell = 0;
					return *this;
				}
			} while (cell->state != CellState::occupied);
			return *this;
		}
		Iterator operator++(int) {
			Iterator copy = *this;
			++*this;
			return copy;
		}
		bool operator==(Iterator const &that) { return cell == that.cell; }
		bool operator!=(Iterator const &that) { return cell != that.cell; }
		auto &operator*() { return cell->key; }
		auto *operator->() { return &cell->key; }
	};

	struct IteratorConst {
		const ContiguousHashSet *set = 0;
		const Cell *cell = 0;

		Iterator &operator++() {
			do {
				++cell; 
				if (cell == set->cells.end()) {
					cell = 0;
					return *this;
				}
			} while (cell->state != CellState::occupied);
			return *this;
		}
		Iterator operator++(int) {
			Iterator copy = *this;
			++*this;
			return copy;
		}
		bool operator==(Iterator const &that) { return cell == that.cell; }
		bool operator!=(Iterator const &that) { return cell != that.cell; }
		auto &operator*() { return cell->key; }
		auto *operator->() { return &cell->key; }
	};

	
	Iterator begin() { 
		if (cells.count == 0)
			return {this, 0};

		Iterator it = {this, cells.begin()};
		if (cells[0].state != CellState::occupied)
			++it;

		return it;
	}
	IteratorConst begin() const { 
		if (cells.count == 0)
			return {this, 0};

		IteratorConst it = {this, cells.begin()};
		if (cells[0].state != CellState::occupied)
			++it;

		return it;
	}
	Iterator end() { return {this, 0}; }
	IteratorConst end() const { return {this, 0}; }

	
	void erase(Iterator it) {
		bounds_check(assert(cells.count));
		assert(it.cell.state == CellState::occupied);
		it.cell->state = CellState::removed;
		--count;
	}
	
	void free() {
		allocator.free(cells.data);
		cells = {};
	}

	bool is_empty() {
		return count == 0;
	}

	template <class Fn>
	bool for_each(Fn &&in_fn) {

		auto fn = wrap_foreach_fn<Key>(in_fn);

		for (auto &cell : cells) {
			if (cell.state == ContiguousHashSetCellState::occupied) {
				auto d = fn(cell.key);
				switch (d & ForEach_erase_mask) {
					case ForEach_erase: cell.state = CellState::removed; break;
					case ForEach_erase_unordered: cell.state = CellState::removed; break;
				}
				if (d & ForEach_break)
					return true;
			}
		}
		return false;
	}

	template <class ResultAllocator = Allocator, class Fn>
	auto set(Fn &&fn TL_LP) {
		using U = decltype(fn(*(Key *)0));
		List<U, ResultAllocator> result;
		result.reserve(count TL_LA);
		for_each([&](auto &kv) { result.add(fn(kv)); });
		return result;
	}

	template <class DstAllocator, class DstSize>
	void copy_to(List<Key, DstAllocator, DstSize> &list) {
		list.clear();
		list.reserve(count);
		for_each([&](Key const &key) { list.add(key); });
	}
	
	template <class ResultAllocator = Allocator>
	auto to_list(TL_LPC) {
		List<Key, ResultAllocator> result;
		copy_to(result);
		return result;
	}

};

}