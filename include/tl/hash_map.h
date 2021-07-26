#pragma once
#include "linked_list.h"
#include "array.h"

namespace tl {

template <class Key, class Value, umm _capacity>
struct StaticHashMap {
	inline static constexpr umm capacity = _capacity;

	struct Entry {
		Key key;
		Value value;
	};

	using Block = LinkedList<Entry>;

	Array<Block, capacity> blocks;

	Value &get_or_insert(Key const &key) {
		umm hash = get_hash(key);
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
		umm hash = get_hash(key);
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

template <class Key, class Value, umm capacity>
void free(StaticHashMap<Key, Value, capacity> &map) {
	for (auto &block : map.blocks) {
		free(block);
	}
}

template <class Key, class Value>
struct HashMap {
	struct KeyValue {
		Key key;
		Value value;
	};
	using Bucket = LinkedList<KeyValue>;

	Allocator allocator = current_allocator;
	Bucket *buckets;
	umm bucket_count;
	umm total_value_count;
	
	Value &get_or_insert(Key const &key) {
		if (!bucket_count) {
			rehash(256);
		}

		umm hash = get_hash(key);
		auto &bucket = buckets[hash & (bucket_count - 1)];
		for (auto &it : bucket) {
			if (it.key == key) {
				return it.value;
			}
		}

		if (total_value_count == bucket_count) {
			rehash(bucket_count * 2);
		}
		++total_value_count;
		auto &it = bucket.add();
		it.key = key;
		return it.value;
	}
	Value *find(Key const &key) {
		umm hash = get_hash(key);
		auto &bucket = buckets[hash & (bucket_count - 1)];
		for (auto &it : bucket) {
			if (it.key == key) {
				return &it.value;
			}
		}
		return 0;
	}

	void rehash(umm new_bucket_count) {
		Bucket *old_buckets = buckets;

		buckets = allocator.allocate<Bucket>(new_bucket_count);

		for (umm bucket_index = 0; bucket_index < bucket_count; ++bucket_index) {
			for (KeyValue &key_value : old_buckets[bucket_index]) {
				auto hash = get_hash(key_value.key);
				auto &new_bucket = buckets[hash & (new_bucket_count - 1)];
				new_bucket.add(key_value);
			}
		}

		if (old_buckets) {
			allocator.free(old_buckets);
		}
		bucket_count = new_bucket_count;
	}
};

}
