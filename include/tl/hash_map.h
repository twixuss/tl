#pragma once
#include "linked_list.h"
#include "array.h"

namespace TL {

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

}
