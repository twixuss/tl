#pragma once

// TODO: why tf find returns KeyValue? Make it return only Value.

#include "list.h"
#include "linked_list.h"
#include "array.h"
#include "optional.h"
#include "pointer.h"
#include "hash.h"

namespace tl {

template <class Key, bool use_high_bits = true>
struct DefaultHashTraits {
	inline static constexpr u32 rehash_percentage = 70;
	inline static constexpr void on_collision(Key a, Key b) {}

	// These are templates because of situation where == operator is not defined, but are_equal is defined in derived class.
	// In that case these functions will not be instantiated.
	inline constexpr bool are_equal(this auto &&self, Key const &a, Key const &b) { return all(a == b); }
	inline constexpr u64 get_hash(this auto &&self, Key const &key) { return ::get_hash(key); }

	inline constexpr umm get_index_from_hash(this auto &&self, u64 hash, u64 capacity) {
		if constexpr (use_high_bits) {
			hash ^= hash >> 32;
			hash ^= hash >> 16;
			hash ^= hash >> 8;
		}

		assert(is_power_of_2(capacity));
		return hash & (capacity - 1);
	}
	inline constexpr umm get_index_from_key(this auto &&self, Key const &key, u64 capacity) {
		return self.get_index_from_hash(self.get_hash(key), capacity);
	}
};

#define TL_USE_HASH_TRAITS_MEMBERS \
	using Traits::rehash_percentage; \
	using Traits::on_collision; \
	using Traits::are_equal; \
	using Traits::get_hash; \
	using Traits::get_index_from_hash; \
	using Traits::get_index_from_key; \


template <class Key, class Value, umm _capacity, class Traits = DefaultHashTraits<Key>>
struct StaticBucketHashMap : Traits {
	TL_USE_HASH_TRAITS_MEMBERS;

	inline static constexpr umm capacity = _capacity;

	static_assert(is_power_of_2(capacity));

	struct Entry {
		Key key;
		Value value;
	};

	using Block = LinkedList<Entry>;

	Array<Block, capacity> blocks = {};

	[[no_unique_address]] Traits traits = {};
	
	inline constexpr umm get_index_from_key(Key const &key) { return get_index_from_key(key, capacity); }

	Value &get_or_insert(Key const &key) {
		auto &block = blocks[get_index_from_key(key)];
		for (auto &it : block) {
			if (it.key == key) {
				return it.value;
			}
		}
		auto &it = block.add();
		it.key = key;
		return it.value;
	}
	void insert(Key const &key, Value value) {
		get_or_insert(key) = value;
	}
	Value *find(Key const &key) {
		auto &block = blocks[get_index_from_key(key)];
		for (auto &it : block) {
			if (it.key == key) {
				return &it.value;
			}
		}
		return 0;
	}

	struct Iterator {
		StaticBucketHashMap *map;
		Block *block;
		typename Block::Iterator value;

		Iterator &operator++() {
			++value;
			if (value == block->end()) {
				do {
					++block;
					if (block == map->blocks.end()) {
						value = {};
						return *this;
					} else {
						value = block->begin();
					}
				} while (!value);
			}
			return *this;
		}
		Iterator operator++(int) {
			Iterator copy = *this;
			++*this;
			return copy;
		}
		bool operator==(Iterator const &that) { return value == that.value; }
		bool operator!=(Iterator const &that) { return value != that.value; }
		Entry &operator*() { return *value; }
		Entry *operator->() { return &*value; }
	};

	Iterator begin() {
		Iterator result;
		result.map = this;
		result.block = blocks.data;
		result.value = result.block->begin();
		return result;
	}
	Iterator end() {
		return {};
	}
};

template <class Key, class Value, umm capacity, class Traits>
void free(StaticBucketHashMap<Key, Value, capacity, Traits> &map) {
	for (auto &block : map.blocks) {
		free(block);
	}
}

template <class Key_, class Value_>
struct KeyValue {
	using Key = Key_;
	using Value = Value_;
	Key key;
	Value value;
};

template <class Key_, class Value_, class Traits = DefaultHashTraits<Key_>, class Allocator = Allocator>
struct BucketHashMap : Traits {
	TL_USE_HASH_TRAITS_MEMBERS;

	using Key = Key_;
	using Value = Value_;
	using KeyValue = KeyValue<Key, Value>;
	using ValueType = KeyValue;

	struct HashKeyValue {
		u64 hash;
		KeyValue kv;
	};
	using Bucket = LinkedList<HashKeyValue>;

	[[no_unique_address]] Allocator allocator = Allocator::current();
	Span<Bucket> buckets;
	umm count = 0;
	
	Value &get_or_insert(Key const &key TL_LP) {
		if (!buckets.count) {
			rehash(4 TL_LA);
		}

		umm hash = get_hash(key);
		auto bucket = &buckets[get_index_from_hash(hash, buckets.count)];

		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (are_equal(it.kv.key, key)) {
					return it.kv.value;
				}
			}
		}

		// NOTE: Optimizer is able to get rid of this division.
		if (count >= buckets.count * rehash_percentage / 100) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		}
		++count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.kv.key = key;

		return it.kv.value;
	}
	void insert(Key const &key, Value value) {
		get_or_insert(key) = value;
	}

	//
	// If value is not present returns true
	//
	// If value is already present calls a callback with stored key and value and returns false
	//
	template <class Callback>
	bool insert_or(Key const &key, Value value, Callback &&callback TL_LP) {
		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = get_hash(key);
		auto bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (are_equal(it.kv.key, key)) {
					callback(it.kv.key, it.kv.value);
					return false;
				}
			}
		}

		if (count == buckets.count) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		}
		++count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.kv.key = key;
		it.kv.value = value;
		return true;
	}
	bool try_insert(Key const &key, Value value, Value **existing = 0, Key **existing_key = 0 TL_LP) {
		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = get_hash(key);
		auto bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (are_equal(it.kv.key, key)) {
					if (existing)
						*existing = &it.kv.value;
					if (existing_key)
						*existing_key = &it.kv.key;
					return false;
				}
			}
		}

		if (count == buckets.count) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		}
		++count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.kv.key = key;
		it.kv.value = value;
		return true;
	}

	KeyValue *find(Key const &key) const {
		if (buckets.count == 0)
			return 0;

		umm hash = get_hash(key);
		auto &bucket = buckets[get_index_from_hash(hash, buckets.count)];

		for (auto &it : bucket) {
			if (it.hash == hash) {
				if (are_equal(it.kv.key, key)) {
					return &it.kv;
				}
			}
		}
		return 0;
	}
	Value erase(KeyValue *kv) {
		if (buckets.count == 0)
			return {};

		--count;
		umm hash = get_hash(kv->key);
		auto &bucket = buckets[get_index_from_hash(hash, buckets.count)];
		auto result = kv->value;
		tl::erase(bucket, (HashKeyValue *)((u8 *)kv - offsetof(HashKeyValue, kv)));
		return result;
	}
	Optional<Value> erase(Key const &key) {
		if (buckets.count == 0)
			return {};

		umm hash = get_hash(key);
		auto &bucket = buckets[get_index_from_hash(hash, buckets.count)];
		for (auto &it : bucket) {
			if (it.hash == hash) {
				if (are_equal(it.kv.key, key)) {
					--count;
					auto result = it.kv.value;
					tl::erase(bucket, &it);
					return result;
				}
			}
		}

		return {};
	}

	void rehash(umm new_buckets_count TL_LP) {
		// Set current allocator so new buckets inherit it.
		scoped(allocator);

		auto old_buckets = buckets;

		buckets.data = allocator.allocate<Bucket>(new_buckets_count TL_LA);
		buckets.count = new_buckets_count;

		for (auto old_bucket : old_buckets) {
			auto next_node = old_bucket.head;
			while (next_node) {
				auto node = next_node;
				next_node = next_node->next;
				node->next = 0;

				auto hash = get_hash(node->value.kv.key);
				auto &new_bucket = buckets[get_index_from_hash(hash, buckets.count)];
				new_bucket.add_steal(node);
			}
		}

		if (old_buckets.data) {
			allocator.free_t(old_buckets.data, old_buckets.count);
		}
	}

	void clear() {
		count = 0;
		for (auto &bucket : buckets) {
			bucket.clear();
		}
	}


	struct Iterator {
		BucketHashMap *map;
		Bucket *bucket;
		typename Bucket::Iterator bucket_it;

		Iterator &operator++() {
			++bucket_it;
			if (bucket_it == bucket->end()) {
				do {
					++bucket;
					if (bucket == map->buckets.end()) {
						bucket_it = {};
						return *this;
					} else {
						bucket_it = bucket->begin();
					}
				} while (!bucket_it);
			}
			return *this;
		}
		Iterator operator++(int) {
			Iterator copy = *this;
			++*this;
			return copy;
		}
		bool operator==(Iterator const &that) { return bucket_it == that.bucket_it; }
		bool operator!=(Iterator const &that) { return bucket_it != that.bucket_it; }
		auto &operator*() { return bucket_it->kv; }
		auto *operator->() { return &bucket_it->kv; }
	};

	Iterator begin() {
		if (!buckets.count)
			return {};

		Iterator result;
		result.map = this;
		for (auto &bucket : buckets) {
			result.bucket_it = bucket.begin();
			if (result.bucket_it) {
				result.bucket = &bucket;
				break;
			}
		}
		return result;
	}
	Iterator end() {
		return {};
	}
};

TL_DECLARE_CONCEPT(BucketHashMap);

void free(CBucketHashMap auto &map) {
	for (auto &bucket : map.buckets) {
		free(bucket);
	}
	map.allocator.free(map.buckets.data);
	map.buckets = {};
}

umm count_of(CBucketHashMap auto map) {
	umm result = 0;
	for (auto &bucket : map.buckets) {
		result += bucket.size();
	}
	return result;
}

template <ForEachFlags flags = 0, CBucketHashMap Map, class Fn>
void for_each(Map &&map, Fn &&fn) requires
	requires { fn(*map.begin()); }
{
	static_assert(flags == 0, "Only default flags supported");

	for (u32 i = 0; i < map.buckets.count; ++i) {
		for (auto &it : map.buckets[i]) {
			static_assert(std::is_same_v<decltype(fn(it.kv)), void>, "ForEachDirective not implemented");
			fn(it.kv);
		}
	}
}

template <class Fn>
auto map(CBucketHashMap auto map, Fn &&fn TL_LP) requires requires { fn(*(typename decltype(map)::KeyValue *)0); } {
	using U = decltype(fn(*(typename decltype(map)::KeyValue *)0));
	List<U, Allocator> result;
	result.reserve(count_of(map) TL_LA);
	for_each(map, [&](auto &kv) { result.add(fn(kv)); });
	return result;
}

// TODO: make all bucket hash map functions use allocator.

template <class Key, class Value, class Traits>
BucketHashMap<Key, Value, Traits> copy(BucketHashMap<Key, Value, Traits> const &source TL_LP) {
	BucketHashMap<Key, Value, Traits> result;
	for_each(source, [&](auto &kv) {
		result.get_or_insert(kv.key TL_LA) = kv.value;
	});
	return result;
}

template <class Key, class Value, class Traits>
void set(BucketHashMap<Key, Value, Traits> &destination, BucketHashMap<Key, Value, Traits> const &source TL_LP) {
	destination.clear();
	for_each(source, [&](auto &kv) {
		destination.get_or_insert(kv.key TL_LA) = kv.value;
	});
}

template <class Key, class Value, class Traits, class Fn>
umm count(BucketHashMap<Key, Value, Traits> map, Fn &&fn) {
	umm result = 0;
	for_each(map, [&](auto &kv) {
		if (fn(kv.key, kv.value)) {
			result += 1;
		}
	});
	return result;
}

template <class Key, class Value, class Traits>
bool is_empty(BucketHashMap<Key, Value, Traits> map) {
	bool result = true;
	for_each(map, [&](auto &kv) { (void)kv; result = false; return ForEach_break; });
	return result;
}


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
	using ValueType = KeyValue;

	// TODO: SOA, store hash
	struct Cell {
		CellState state;
		KeyValue key_value;
		Key &key() { return key_value.key; }
		Value &value() { return key_value.value; }
	};

	struct Iterator {
		ContiguousHashMap *map = 0;
		Cell *cell = 0;

		Iterator &operator++() {
			do {
				++cell; 
				if (cell == map->cells.end()) {
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
		auto &operator*() { return cell->key_value; }
		auto *operator->() { return &cell->key_value; }
	};

	struct IteratorConst {
		const ContiguousHashMap *map = 0;
		const Cell *cell = 0;

		Iterator &operator++() {
			do {
				++cell; 
				if (cell == map->cells.end()) {
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
		auto &operator*() { return cell->key_value; }
		auto *operator->() { return &cell->key_value; }
	};

	Span<Cell> cells;
	umm count = 0;

	[[no_unique_address]] Allocator allocator = Allocator::current();
	
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
					if (are_equal(cell.key(), key)) {
						return cell.value();
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
		defer { assert(find(key)); };

		while (cells.data[index].state == CellState::occupied) {
			on_collision(cells.data[index].key(), key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::occupied;
		cell.key() = key;
		cell.value() = default_value;
		return cell.value();
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
		assert(is_power_of_2(new_cells.count));

		new_cells.data = allocator.allocate_uninitialized<Cell>(new_cells.count TL_LA);
		for (auto &new_cell : new_cells) {
			new_cell.state = CellState::empty;
		}

		for (auto &old_cell : cells) {
			if (old_cell.state == CellState::occupied) {
				insert_into(new_cells, old_cell.key(), old_cell.value());
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
			on_collision(cells.data[index].key(), key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::occupied;
		cell.key() = key;
		return cell.value() = value;
	}

	KeyValue *find(Key key) const {
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
				if (are_equal(cell.key(), key)) {
					return &cell.key_value;
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
	void erase(Iterator it) {
		bounds_check(assert(cells.count));
		it.cell->state = CellState::removed;
	}
	void erase(KeyValue *kv) {
		bounds_check(assert(cells.count));
		auto cell = (Cell *)((u8 *)kv - offsetof(Cell, key_value));
		cell->state = CellState::removed;
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
				if (are_equal(cell.key(), key)) {
					cell.state = CellState::removed;
				}
			}
			index = step(index, cells.count);
		}
	}
};

TL_DECLARE_CONCEPT(ContiguousHashMap);

void free(CContiguousHashMap auto &map) {
	map.allocator.free(map.cells.data);
	map.cells = {};
}

bool is_empty(CContiguousHashMap auto map) {
	return map.count == 0;
}

umm count_of(CContiguousHashMap auto map) {
	return map.count;
}

template <CContiguousHashMap Map, class Fn>
bool for_each(Map &&map, Fn &&fn) 
	requires requires { fn(*map.begin()); }
{
	for (auto &cell : map.cells) {
		if (cell.state == ContiguousHashMapCellState::occupied) {
			using FnRet = decltype(fn(cell.key_value));
			if constexpr (std::is_same_v<FnRet, void>) {
				fn(cell.key_value);
			} else if constexpr (std::is_same_v<FnRet, ForEachDirective>) {
				auto d = fn(cell.key_value);
				if (d & ForEach_erase) cell.state = Map::CellState::removed;
				if (d & ForEach_erase_unordered) cell.state = Map::CellState::removed;
				if (d & ForEach_break)
					return true;
			} else {
				static_error_t(Map, "Invalid return type of for_each function");
			}
		}
	}
	return false;
}

template <class Allocator = Allocator, CContiguousHashMap Map, class Fn>
auto map(Map map, Fn &&fn TL_LP) requires requires (Map &&map, Fn &&fn) { fn(map.cells[0].key_value); } {
	using U = decltype(fn(*(typename decltype(map)::KeyValue *)0));
	List<U, Allocator> result;
	result.reserve(map.count TL_LA);
	for_each(map, [&](auto &kv) { result.add(fn(kv)); });
	return result;
}

template <class Allocator = Allocator, CContiguousHashMap Map>
auto to_list(Map map TL_LP) {
	List<typename Map::KeyValue, Allocator> result;
	result.reserve(map.count TL_LA);
	for_each(map, [&](auto &kv) { result.add(kv); });
	return result;
}

template <class Key, class Value, class DstAllocator, class DstSize, class Traits, class SrcAllocator>
void set(List<KeyValue<Key, Value>, DstAllocator, DstSize> &list, ContiguousHashMap<Key, Value, Traits, SrcAllocator> const &map) {
	list.clear();
	list.reserve(map.count);
	for_each(map, [&](KeyValue<Key, Value> const &kv) {
		list.data[list.count++] = kv;
	});
}

#ifndef TL_DEFAULT_HASH_MAP
#define TL_DEFAULT_HASH_MAP BucketHashMap
#endif

template <class Key, class Value, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
using HashMap = TL_DEFAULT_HASH_MAP<Key, Value, Traits, Allocator>;

}
