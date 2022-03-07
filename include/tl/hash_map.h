#pragma once
#include "list.h"
#include "linked_list.h"
#include "array.h"
#include "optional.h"
#include "pointer.h"
#include "hash.h"

namespace tl {

template <class Key, class Value, umm _capacity, class Hasher = DefaultHasher<Key>>
struct StaticBucketHashMap {
	inline static constexpr umm capacity = _capacity;

	struct Entry {
		Key key;
		Value value;
	};

	using Block = LinkedList<Entry>;

	Array<Block, capacity> blocks = {};

	Value &get_or_insert(Key const &key) {
		umm hash = Hasher::get_hash(key);
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
		umm hash = Hasher::get_hash(key);
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

template <class Key, class Value, umm capacity, class Hasher>
void free(StaticBucketHashMap<Key, Value, capacity, Hasher> &map) {
	for (auto &block : map.blocks) {
		free(block);
	}
}

template <class Key, class Value, class Hasher = DefaultHasher<Key>>
struct BucketHashMap {
	using ValueType = Value;

	struct KeyValue {
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

		umm hash = Hasher::get_hash(key);
		auto bucket = &buckets[hash & (buckets.count - 1)];
		for (auto &it : *bucket) {
			if (it.key == key) {
				return it.value;
			}
		}

		if (total_value_count == buckets.count) {
			rehash(buckets.count * 2);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++total_value_count;
		auto &it = bucket->add(TL_LAC);
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

		umm hash = Hasher::get_hash(key);
		auto bucket = &buckets[hash & (buckets.count - 1)];
		for (auto &it : *bucket) {
			if (it.key == key) {
				callback(it.key, it.value);
				return false;
			}
		}

		if (total_value_count == buckets.count) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++total_value_count;
		auto &it = bucket->add(TL_LAC);
		it.key = key;
		it.value = value;
		return true;
	}
	bool try_insert(Key const &key, Value value, Value **existing = 0, Key **existing_key = 0 TL_LP) {
		scoped_allocator(allocator);

		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = Hasher::get_hash(key);
		auto bucket = &buckets[hash & (buckets.count - 1)];
		for (auto &it : *bucket) {
			if (it.key == key) {
				if (existing)
					*existing = &it.value;
				if (existing_key)
					*existing_key = &it.key;
				return false;
			}
		}

		if (total_value_count == buckets.count) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++total_value_count;
		auto &it = bucket->add(TL_LAC);
		it.key = key;
		it.value = value;
		return true;
	}

	Pointer<Value> find(Key const &key) {
		if (buckets.count == 0)
			return 0;

		umm hash = Hasher::get_hash(key);
		auto &bucket = buckets[hash & (buckets.count - 1)];
		for (auto &it : bucket) {
			if (it.key == key) {
				return &it.value;
			}
		}
		return 0;
	}
	Optional<Value> erase(Key const &key) {
		if (buckets.count == 0)
			return {};

		umm hash = Hasher::get_hash(key);
		auto &bucket = buckets[hash & (buckets.count - 1)];
		for (auto &it : bucket) {
			if (it.key == key) {
				auto result = it.value;
				tl::erase(bucket, &it);
				return result;
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
			for (KeyValue &key_value : old_buckets[bucket_index]) {
				auto hash = Hasher::get_hash(key_value.key);
				auto &new_bucket = buckets[hash & (new_buckets_count - 1)];
				new_bucket.add(key_value TL_LA);
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

template <ForEachFlags flags, class Key, class Value, class Hasher, class Fn>
void for_each(BucketHashMap<Key, Value, Hasher> map, Fn &&fn) {
	static_assert(flags == 0, "Only default flags supported");

	for (u32 i = 0; i < map.buckets.count; ++i) {
		for (auto &it : map.buckets[i]) {
			fn(it.key, it.value);
		}
	}
}

template <class Key, class Value, class Hasher>
BucketHashMap<Key, Value, Hasher> copy(BucketHashMap<Key, Value, Hasher> const &source TL_LP) {
	BucketHashMap<Key, Value, Hasher> result;
	for_each(source, [&](Key const &key, Value const &value) {
		result.get_or_insert(key TL_LA) = value;
	});
	return result;
}

template <class Key, class Value, class Hasher>
void set(BucketHashMap<Key, Value, Hasher> &destination, BucketHashMap<Key, Value, Hasher> const &source TL_LP) {
	destination.clear();
	for_each(source, [&](Key const &key, Value const &value) {
		destination.get_or_insert(key TL_LA) = value;
	});
}

template <class Key, class Value, class Hasher>
umm count_of(BucketHashMap<Key, Value, Hasher> &map) {
	umm result = 0;
	for (auto &bucket : map.buckets) {
		result += bucket.size();
	}
	return result;
}

template <class Key, class Value, class Hasher, class Fn>
umm count(BucketHashMap<Key, Value, Hasher> map, Fn &&fn) {
	umm result = 0;
	for_each(map, [&](Key &key, Value &value) {
		if (fn(key, value)) {
			result += 1;
		}
	});
	return result;
}

template <class Key, class Value, class Hasher>
bool is_empty(BucketHashMap<Key, Value, Hasher> map) {
	bool result = true;
	for_each(map, [&](Key &key, Value &value) { (void)key; (void)value; result = false; for_each_break; });
	return result;
}



inline static constexpr u64 map_cell_state_empty     = 0;
inline static constexpr u64 map_cell_state_full      = 1;
inline static constexpr u64 map_cell_state_tombstone = 2;

template <class Key, class Value, class Hasher = DefaultHasher<Key>>
struct ContiguousHashMap {
    struct Cell {
        u64 state : 2;
        u64 hash : 62;
        Key key;
        Value value;
    };

    Allocator allocator = current_allocator;
    Span<Cell> cells;
    umm count = 0;


    Value &get_or_insert(Key key) {
        reserve(count + 1);
        count += 1;
        return insert_into(cells, key, {});
    }

    bool reserve(umm desired) {
        if (desired <= cells.count)
            return false;

        Span<Cell> new_cells;
        new_cells.count = max(1, cells.count * 2);
        new_cells.data = allocator.allocate_uninitialized<Cell>(new_cells.count);
        for (auto &new_cell : new_cells) {
            new_cell.state = map_cell_state_empty;
        }

        for (auto &old_cell : cells) {
            insert_into(new_cells, old_cell.key, old_cell.value);
        }

        allocator.free_t(cells.data, cells.count);
        cells = new_cells;

		return true;
    }

    void step(umm *index) {
        *index = (*index + 1) % cells.count;
    }

    Value &insert_into(Span<Cell> cells, Key key, Value value) {
        auto hash = get_the_hash(key);
        auto index = hash % cells.count;

        while (cells.data[index].state == map_cell_state_full) {
            step(&index);
        }

        auto &cell = cells.data[index];

        cell.state = map_cell_state_full;
        cell.hash = hash;
        cell.key = key;
        return cell.value = value;
    }

    Value *find(Key key) {
        auto hash = get_the_hash(key);
        auto index = hash % cells.count;
        auto steps = cells.count; // TODO: i don't know if there is a good way to handle absence of a value
        while (steps--) {
            auto &cell = cells.data[index];
            if (cell.state == map_cell_state_full) {
                if (cell.hash == hash) {
                    if (cell.key == key) {
                        return &cell.value;
                    }
                }
            }
            step(&index);
        }

        return 0;
    }

    u64 get_the_hash(Key key) {
        return Hasher::get_hash(key) & 0x3FFF'FFFF'FFFF'FFFF; // get rid of 2 bits reserved for `state`
    }

	void clear() {
		for (auto &cell : cells) {
			cell.state = map_cell_state_empty;
		}
		count = 0;
	}
};


template <class Key, class Value>
umm count_of(ContiguousHashMap<Key, Value> map) {
    return map.count;
}


template <class Key, class Value, class Fn>
void for_each(ContiguousHashMap<Key, Value> map, Fn &&fn) {
    for (auto &cell : map.cells) {
        if (cell.state == map_cell_state_full) {
            fn(cell.key, cell.value);
        }
    }
}

#ifndef TL_DEFAULT_HASH_MAP
#define TL_DEFAULT_HASH_MAP(Key, Value, Hasher) ContiguousHashMap<Key, Value, Hasher>
#endif

template <class Key, class Value, class Hasher = DefaultHasher<Key>>
using HashMap = TL_DEFAULT_HASH_MAP(Key, Value, Hasher);

}
