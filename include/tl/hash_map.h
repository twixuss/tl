#pragma once
#include "linked_list.h"
#include "array.h"
#include "optional.h"
#include "pointer.h"
#include "hash.h"

namespace tl {

template <class Key, class Value, umm _capacity, class Hasher = DefaultHasher<Key>>
struct StaticHashMap {
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
		StaticHashMap *map;
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
void free(StaticHashMap<Key, Value, capacity, Hasher> &map) {
	for (auto &block : map.blocks) {
		free(block);
	}
}

template <class Key, class Value, class Hasher = DefaultHasher<Key>>
struct HashMap {
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
void for_each(HashMap<Key, Value, Hasher> map, Fn &&fn) {
	static_assert(flags == 0, "Only default flags supported");

	for (u32 i = 0; i < map.buckets.count; ++i) {
		for (auto &it : map.buckets[i]) {
			fn(it.key, it.value);
		}
	}
}

template <class Key, class Value, class Hasher>
HashMap<Key, Value, Hasher> copy(HashMap<Key, Value, Hasher> const &source TL_LP) {
	HashMap<Key, Value, Hasher> result;
	for_each(source, [&](Key const &key, Value const &value) {
		result.get_or_insert(key TL_LA) = value;
	});
	return result;
}

template <class Key, class Value, class Hasher>
void set(HashMap<Key, Value, Hasher> &destination, HashMap<Key, Value, Hasher> const &source TL_LP) {
	destination.clear();
	for_each(source, [&](Key const &key, Value const &value) {
		destination.get_or_insert(key TL_LA) = value;
	});
}

template <class Key, class Value, class Hasher>
umm count_of(HashMap<Key, Value, Hasher> &map) {
	umm result = 0;
	for (auto &bucket : map.buckets) {
		result += bucket.size();
	}
	return result;
}

template <class Key, class Value, class Hasher, class Fn>
umm count(HashMap<Key, Value, Hasher> map, Fn &&fn) {
	umm result = 0;
	for_each(map, [&](Key &key, Value &value) {
		if (fn(key, value)) {
			result += 1;
		}
	});
	return result;
}

template <class Key, class Value, class Hasher>
bool is_empty(HashMap<Key, Value, Hasher> map) {
	bool result = true;
	for_each(map, [&](Key &key, Value &value) { (void)key; (void)value; result = false; for_each_break; });
	return result;
}



}
