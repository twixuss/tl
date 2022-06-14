#pragma once
#include "list.h"
#include "linked_list.h"
#include "array.h"
#include "optional.h"
#include "pointer.h"
#include "hash.h"

namespace tl {

template <class Key>
struct DefaultHashTraits {
	inline static constexpr u32 rehash_percentage = 70;
	inline static constexpr void on_collision(Key a, Key b) {}

	// template is here because of situation where == operator is not defined, but
	// are_equal is overriden in a base class.
	template <class=void> inline static constexpr bool are_equal(Key a, Key b) { return a == b; }
	template <class=void> inline static constexpr u64 get_hash(Key key) { return ::get_hash(key); }
};

template <class Key, class Value, umm _capacity, class Traits = DefaultHashTraits<Key>>
struct StaticBucketHashMap {
	inline static constexpr umm capacity = _capacity;

	struct Entry {
		Key key;
		Value value;
	};

	using Block = LinkedList<Entry>;

	Array<Block, capacity> blocks = {};

	Value &get_or_insert(Key const &key) {
		umm hash = Traits::get_hash(key);
		auto &block = blocks[hash % blocks.size];
		for (auto &it : block) {
			if (it.key == key) {
				return it.value;
			}
		}
		auto &it = block.add();
		it.key = key;
		return it.value;
	}
	Value *find(Key const &key) {
		umm hash = Traits::get_hash(key);
		auto &block = blocks[hash % blocks.size];
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

template <class Key_, class Value_, class Traits = DefaultHashTraits<Key_>>
struct BucketHashMap {
	using Key = Key_;
	using Value = Value_;
	using ValueType = Value;

	struct KeyValue {
		u64 hash;
		Key key;
		Value value;
	};
	using Bucket = LinkedList<KeyValue>;

	Allocator allocator = current_allocator;
	Span<Bucket> buckets;
	umm total_value_count = 0;

	Value &get_or_insert(Key const &key TL_LP) {
		scoped_allocator(allocator);

		if (!buckets.count) {
			rehash(4 TL_LA);
		}

		umm hash = Traits::get_hash(key);
		auto bucket = &buckets[hash & (buckets.count - 1)];

		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (Traits::are_equal(it.key, key)) {
					return it.value;
				}
			}
		}

		if (total_value_count >= buckets.count * Traits::rehash_percentage / 100) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++total_value_count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.key = key;
		return it.value;
	}

	//
	// If value is not present returns true
	//
	// If value is already present calls a callback with stored key and value and returns false
	//
	template <class Callback>
	bool insert_or(Key const &key, Value value, Callback &&callback TL_LP) {
		scoped_allocator(allocator);

		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = Traits::get_hash(key);
		auto bucket = &buckets[hash & (buckets.count - 1)];
		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (Traits::are_equal(it.key, key)) {
					callback(it.key, it.value);
					return false;
				}
			}
		}

		if (total_value_count == buckets.count) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++total_value_count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.key = key;
		it.value = value;
		return true;
	}
	bool try_insert(Key const &key, Value value, Value **existing = 0, Key **existing_key = 0 TL_LP) {
		scoped_allocator(allocator);

		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = Traits::get_hash(key);
		auto bucket = &buckets[hash & (buckets.count - 1)];
		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (Traits::are_equal(it.key, key)) {
					if (existing)
						*existing = &it.value;
					if (existing_key)
						*existing_key = &it.key;
					return false;
				}
			}
		}

		if (total_value_count == buckets.count) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++total_value_count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.key = key;
		it.value = value;
		return true;
	}

	KeyValue *find(Key const &key) {
		if (buckets.count == 0)
			return 0;

		umm hash = Traits::get_hash(key);
		auto &bucket = buckets[hash & (buckets.count - 1)];
		for (auto &it : bucket) {
			if (it.hash == hash) {
				if (Traits::are_equal(it.key, key)) {
					return &it;
				}
			}
		}
		return 0;
	}
	Optional<Value> erase(Key const &key) {
		if (buckets.count == 0)
			return {};

		umm hash = Traits::get_hash(key);
		auto &bucket = buckets[hash & (buckets.count - 1)];
		for (auto &it : bucket) {
			if (it.hash == hash) {
				if (Traits::are_equal(it.key, key)) {
					auto result = it.value;
					tl::erase(bucket, &it);
					return result;
				}
			}
		}

		return {};
	}

	void rehash(umm new_buckets_count TL_LP) {
		scoped_allocator(allocator);

		auto old_buckets = buckets;

		buckets.data = allocator.allocate<Bucket>(new_buckets_count TL_LA);
		buckets.count = new_buckets_count;

		for (umm bucket_index = 0; bucket_index < old_buckets.count; ++bucket_index) {
			auto next_node = old_buckets[bucket_index].head;
			while (next_node) {
				auto node = next_node;
				next_node = next_node->next;
				node->next = 0;

				auto hash = Traits::get_hash(node->value.key);
				auto &new_bucket = buckets[hash & (new_buckets_count - 1)];
				new_bucket.add_steal(node);
			}
		}

		if (old_buckets.data) {
			allocator.free_t(old_buckets.data, old_buckets.count);
		}
	}

	void clear() {
		for (umm bucket_index = 0; bucket_index < buckets.count; ++bucket_index) {
			buckets[bucket_index].clear();
		}
	}
};

template <ForEachFlags flags=0, class Key, class Value, class Traits, class Fn>
void for_each(BucketHashMap<Key, Value, Traits> map, Fn &&fn) requires
	requires { fn(*(Key *)0, *(Value *)0); }
{
	static_assert(flags == 0, "Only default flags supported");

	for (u32 i = 0; i < map.buckets.count; ++i) {
		for (auto &it : map.buckets[i]) {
			fn(it.key, it.value);
		}
	}
}

template <class Key, class Value, class Traits>
BucketHashMap<Key, Value, Traits> copy(BucketHashMap<Key, Value, Traits> const &source TL_LP) {
	BucketHashMap<Key, Value, Traits> result;
	for_each(source, [&](Key const &key, Value const &value) {
		result.get_or_insert(key TL_LA) = value;
	});
	return result;
}

template <class Key, class Value, class Traits>
void set(BucketHashMap<Key, Value, Traits> &destination, BucketHashMap<Key, Value, Traits> const &source TL_LP) {
	destination.clear();
	for_each(source, [&](Key const &key, Value const &value) {
		destination.get_or_insert(key TL_LA) = value;
	});
}

template <class Key, class Value, class Traits>
umm count_of(BucketHashMap<Key, Value, Traits> &map) {
	umm result = 0;
	for (auto &bucket : map.buckets) {
		result += bucket.size();
	}
	return result;
}

template <class Key, class Value, class Traits, class Fn>
umm count(BucketHashMap<Key, Value, Traits> map, Fn &&fn) {
	umm result = 0;
	for_each(map, [&](Key &key, Value &value) {
		if (fn(key, value)) {
			result += 1;
		}
	});
	return result;
}

template <class Key, class Value, class Traits>
bool is_empty(BucketHashMap<Key, Value, Traits> map) {
	bool result = true;
	for_each(map, [&](Key &key, Value &value) { (void)key; (void)value; result = false; for_each_break; });
	return result;
}


enum class ContiguousHashMapCellState : u64 {
	empty     = 0,
	full      = 1,
	tombstone = 2,
};

template <class Key, class Value, class Traits = DefaultHashTraits<Key>>
struct ContiguousHashMap {
	using CellState = ContiguousHashMapCellState;

    struct Cell {
        CellState state : 2;
        u64 hash : 62;
        Key key;
        Value value;
    };

    Allocator allocator = current_allocator;
    Span<Cell> cells;
    umm count = 0;


    Value &get_or_insert(Key key) {
		auto hash = get_the_hash(key);
		u64 index = 0;
		if (cells.count) {
			index = hash % cells.count;
			auto steps = cells.count;
			while (steps--) {
				auto &cell = cells.data[index];
				if (cell.state == CellState::empty) {
					break;
				}
				if (cell.state == CellState::full) {
					if (cell.hash == hash) {
						if (Traits::are_equal(cell.key, key)) {
							return cell.value;
						}
					}
				}
				index = step(index, cells.count);
			}

			bool recalc = false;
			if (count >= cells.count * Traits::rehash_percentage / 100) {
				if (reserve(cells.count * 2)) {
					hash = get_the_hash(key);
					index = hash % cells.count;
				}
			}
        } else {
			reserve(7);
			index = hash % cells.count;
		}
		count += 1;
		defer { assert(find(key)); };

		while (cells.data[index].state == CellState::full) {
			Traits::on_collision(cells.data[index].key, key);
			index = step(index, cells.count);
		}

		auto &cell = cells.data[index];

		cell.state = CellState::full;
		cell.hash = hash;
		cell.key = key;
		return cell.value = {};
    }

    bool reserve(umm desired) {
        if (desired <= cells.count)
            return false;

        Span<Cell> new_cells;
        new_cells.count = max(1, cells.count);
		while (new_cells.count < desired) {
			new_cells.count *= 2;
			new_cells.count += 1;
		}
		assert(is_power_of_2(new_cells.count + 1));

        new_cells.data = allocator.allocate_uninitialized<Cell>(new_cells.count);
        for (auto &new_cell : new_cells) {
            new_cell.state = CellState::empty;
        }

        for (auto &old_cell : cells) {
			if (old_cell.state == CellState::full) {
				insert_into(new_cells, old_cell.hash, old_cell.key, old_cell.value);
			}
        }

        allocator.free_t(cells.data, cells.count);
        cells = new_cells;

		return true;
    }

    umm step(umm index, umm cells_count) {
        return (index + 2) % cells_count;
    }

    Value &insert_into(Span<Cell> cells, Key key, Value value) {
        auto hash = get_the_hash(key);
        auto index = hash % cells.count;

        while (cells.data[index].state == CellState::full) {
			Traits::on_collision(cells.data[index].key, key);
            index = step(index, cells.count);
        }

        auto &cell = cells.data[index];

        cell.state = CellState::full;
        cell.hash = hash;
        cell.key = key;
        return cell.value = value;
    }
    Value &insert_into(Span<Cell> cells, u64 hash, Key key, Value value) {
        auto index = hash % cells.count;

        while (cells.data[index].state == CellState::full) {
			Traits::on_collision(cells.data[index].key, key);
            index = step(index, cells.count);
        }

        auto &cell = cells.data[index];

        cell.state = CellState::full;
        cell.hash = hash;
        cell.key = key;
        return cell.value = value;
    }

    Value *find(Key key) {
		if (cells.count == 0)
			return 0;

        auto hash = get_the_hash(key);
        auto index = hash % cells.count;
        auto steps = cells.count;
        while (steps--) {
            auto &cell = cells.data[index];
            if (cell.state == CellState::empty) {
				return 0;
			}
            if (cell.state == CellState::full) {
                if (cell.hash == hash) {
                    if (Traits::are_equal(cell.key, key)) {
                        return &cell.value;
                    }
                }
            }
            index = step(index, cells.count);
        }

        return 0;
    }

    u64 get_the_hash(Key key) {
        return Traits::get_hash(key) & 0x3FFF'FFFF'FFFF'FFFF; // get rid of 2 bits reserved for `state`
    }

	void clear() {
		for (auto &cell : cells) {
			cell.state = CellState::empty;
		}
		count = 0;
	}
};

template <class Key, class Value, class Traits>
bool is_empty(ContiguousHashMap<Key, Value, Traits> map) {
	return map.count == 0;
}


template <class Key, class Value, class Traits>
umm count_of(ContiguousHashMap<Key, Value, Traits> map) {
    return map.count;
}


template <class Key, class Value, class Traits, class Fn>
void for_each(ContiguousHashMap<Key, Value, Traits> map, Fn &&fn) {
    for (auto &cell : map.cells) {
        if (cell.state == ContiguousHashMapCellState::full) {
            fn(cell.key, cell.value);
        }
    }
}

#ifndef TL_DEFAULT_HASH_MAP
#define TL_DEFAULT_HASH_MAP(Key, Value, Hasher) BucketHashMap<Key, Value, Hasher>
#endif

template <class Key, class Value, class Traits = DefaultHashTraits<Key>>
using HashMap = TL_DEFAULT_HASH_MAP(Key, Value, Traits);

}
