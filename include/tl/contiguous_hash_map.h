#pragma once

#include "list.h"
#include "hash_map_base.h"

namespace tl {

enum class ContiguousHashMapCellState : u8 {
	empty    = 0,
	occupied = 1,
	removed  = 2,
};

#ifndef TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY
#define TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY 8
#endif

static_assert(is_power_of_2(TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY));

template <class Key, class Value, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
struct ContiguousHashMap : Traits {
	TL_USE_HASH_TRAITS_MEMBERS;

	using CellState = ContiguousHashMapCellState;
	using KeyValue = KeyValue<Key, Value>;
	using KeyValueRef = KeyValueRef<Key, Value>;

	using Element = KeyValue;

	// TODO: SOA, store hash
	struct Cell {
		CellState state;
		Key key;
		Value value;
	};

	Span<Cell> cells;
	umm count = 0;

	[[no_unique_address]] Allocator allocator = Allocator::current();
	
	Value &get_or_insert(Key key, Value default_value = {} TL_LP) {
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
						return cell.value;
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
			// NOTE: TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY will be used inside `reserve`
			reserve(1 TL_LA);
			index = get_index_from_hash(hash, cells.count);
		}
		count += 1;

		while (cells.data[index].state == CellState::occupied) {
			on_collision(cells.data[index].key, key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::occupied;
		cell.key = key;
		cell.value = default_value;
		return cell.value;
	}
	void insert(Key const &key, Value value) {
		get_or_insert(key) = value;
	}
	void insert(KeyValue const &kv) {
		get_or_insert(kv.key) = kv.value;
	}

	bool reserve(umm desired TL_LP) {
		if (desired <= cells.count)
			return false;

		Span<Cell> new_cells;
		new_cells.count = max((umm)TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY, cells.count);
		while (new_cells.count < desired) {
			new_cells.count *= 2;
		}

		new_cells.data = allocator.template allocate_uninitialized<Cell>(new_cells.count TL_LA);
		for (auto &new_cell : new_cells) {
			new_cell.state = CellState::empty;
		}

		for (auto &old_cell : cells) {
			if (old_cell.state == CellState::occupied) {
				insert_into(new_cells, old_cell.key, old_cell.value);
			}
		}

		allocator.free_t(cells.data, cells.count);
		cells = new_cells;

		return true;
	}

	umm step(umm index, umm cells_count) const {
		return (index + 1) & (cells_count - 1);
	}

	Value &insert_into(Span<Cell> cells, Key key, Value value) {
		auto hash = get_hash(key);
		auto index = get_index_from_hash(hash, cells.count);

		while (cells.data[index].state == CellState::occupied) {
			on_collision(cells.data[index].key, key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::occupied;
		cell.key = key;
		return cell.value = value;
	}

	KeyValueRef find(Key key) const {
		if (cells.count == 0)
			return {};

		auto hash = get_hash(key);
		auto index = get_index_from_hash(hash, cells.count);
		auto steps = cells.count;
		while (steps--) {
			auto &cell = cells.data[index];
			if (cell.state == CellState::empty) {
				return {};
			}
			if (cell.state == CellState::occupied) {
				if (are_equal(cell.key, key)) {
					return {&cell.key, &cell.value};
				}
			}
			index = step(index, cells.count);
		}

		return {};
	}

	void clear() {
		for (auto &cell : cells) {
			cell.state = CellState::empty;
		}
		count = 0;
	}
	KeyValue erase(KeyValueRef kv) {
		bounds_check(assert(cells.count));
		auto cell = (Cell *)((u8 *)kv.key - offsetof(Cell, key));
		assert(cell->state == CellState::occupied);
		cell->state = CellState::removed;
		--count;
		return {cell->key, cell->value};
	}
	Optional<Value> erase(Key key) {
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
					return cell.value;
				}
			}
			index = step(index, cells.count);
		}
		return {};
	}
	
	void free() {
		allocator.free(cells.data);
		cells = {};
	}
	
	bool is_empty() const {
		return count == 0;
	}

	bool for_each(HashMapIterFn<Key, Value> auto &&in_fn) {
		auto fn = wrap_foreach_fn<KeyValueRef>(in_fn);
		for (auto &cell : cells) {
			if (cell.state == ContiguousHashMapCellState::occupied) {
				auto d = fn(KeyValueRef{&cell.key, &cell.value});
				switch (d & ForEach_erase_mask) {
					case ForEach_erase: cell.state = ContiguousHashMapCellState::removed; break;
					case ForEach_erase_unordered: cell.state = ContiguousHashMapCellState::removed; break;
				}
				if (d & ForEach_break)
					return true;
			}
		}
		return false;
	}

	template <class DstAllocator = Allocator>
	auto map(HashMapIterFn<Key, Value> auto &&fn TL_LP) {
		using U = decltype(fn(std::declval<KeyValueRef>()));
		List<U, DstAllocator> result;
		result.reserve(count TL_LA);
		for_each([&](auto kv) { result.add(fn(kv) TL_LA); });
		return result;
	}

	template <class DstAllocator = Allocator>
	auto to_list(TL_LPC) {
		return map<DstAllocator>([](auto kv) { return kv.load(); });
	}
	
	template <class DstAllocator, class DstSize>
	void set_to(List<KeyValue, DstAllocator, DstSize> &list TL_LP) {
		list.clear();
		list.reserve(count TL_LA);
		for_each([&](auto kv) { list.add(kv.load() TL_LA); });
	}

	template <class DstAllocator, class DstSize>
	void set_to(List<Value, DstAllocator, DstSize> &list TL_LP) {
		list.clear();
		list.reserve(count TL_LA);
		for_each([&](auto kv) { list.add(*kv.value TL_LA); });
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	{
		ContiguousHashMap<int, int> map;
		map.insert(42, 1);
		map.insert(69, 2);
		map.insert(1337, 3);
		map.insert(12345, 4);
	
		assert(map.count == 4);
		assert(*map.find(42).key == 42);
		assert(*map.find(69).key == 69);
		assert(*map.find(1337).key == 1337);
		assert(*map.find(12345).key == 12345);
		assert(*map.find(42).value == 1);
		assert(*map.find(69).value == 2);
		assert(*map.find(1337).value == 3);
		assert(*map.find(12345).value == 4);

		assert(map.erase(69).value() == 2);
		assert(!map.find(69));
		assert(map.count == 3);

		auto list = map.to_list();
		assert(find(list, KeyValue{42, 1}));
		assert(find(list, KeyValue{1337, 3}));
		assert(find(list, KeyValue{12345, 4}));
	}
	{
		struct CollideTraits : DefaultHashTraits<int> {
			u64 get_hash(int key) const { return 0; }
		};

		ContiguousHashMap<int, int, CollideTraits> map;
		map.insert(42, 1);
		map.insert(69, 2);
		map.insert(1337, 3);
		map.insert(12345, 4);
	
		assert(map.count == 4);
		assert(*map.find(42).key == 42);
		assert(*map.find(69).key == 69);
		assert(*map.find(1337).key == 1337);
		assert(*map.find(12345).key == 12345);
		assert(*map.find(42).value == 1);
		assert(*map.find(69).value == 2);
		assert(*map.find(1337).value == 3);
		assert(*map.find(12345).value == 4);
		
		assert(map.erase(69).value() == 2);
		assert(!map.find(69));
		assert(map.count == 3);
		
		auto list = map.to_list();
		assert(find(list, KeyValue{42, 1}));
		assert(find(list, KeyValue{1337, 3}));
		assert(find(list, KeyValue{12345, 4}));
	}
};

#endif
