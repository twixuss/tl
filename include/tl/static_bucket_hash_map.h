#pragma once

#include "linked_list.h"
#include "array.h"
#include "hash_map_base.h"

namespace tl {


template <class Key, class Value, umm _capacity, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
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

}

#ifdef TL_ENABLE_TESTS
#if 0
TL_TEST {
	using namespace tl;
	{
		StaticBucketHashMap<int, int, 16> map;
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

		
		foreach(it, map) {
			it.erase();
		}

		assert(map.count == 0);
	}
	{
		struct CollideTraits : DefaultHashTraits<int> {
			u64 get_hash(int key) const { return 0; }
		};

		StaticBucketHashMap<int, int, 16, CollideTraits> map;
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

		
		foreach(it, map) {
			it.erase();
		}

		assert(map.count == 0);
	}
};

#endif
#endif
