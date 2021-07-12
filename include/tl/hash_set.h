#pragma once
#include "linked_list.h"
#include "array.h"

namespace tl {

template <class Value, umm _capacity>
struct StaticHashSet {
	inline static constexpr umm capacity = _capacity;

	using Block = LinkedList<Value>;

	Array<Block, capacity> blocks;

	Value &insert(Value value) {
		umm hash = get_hash(value);
		auto &block = blocks[hash % blocks.size];
		for (auto &it : block) {
			if (it == value) {
				return it = value;
			}
		}
		return block.add(value);
	}
	void clear() {
		for (auto &block : blocks) {
			block.clear();
		}
	}

	struct Iterator {
		StaticHashSet *set;
		Block *block;
		typename Block::Iterator value;

		Iterator &operator++() {
			++value;
			if (value == block->end()) {
				do {
					++block;
					if (block == set->blocks.end()) {
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
		Value &operator*() { return *value; }
		Value *operator->() { return &*value; }
	};

	Iterator begin() {
		Iterator result;
		result.set = this;
		result.block = blocks.data - 1;
		do {
			++result.block;
			if (result.block == blocks.end()) {
				result.value = {};
				break;
			} else {
				result.value = result.block->begin();
			}
		} while (!result.value);
		return result;
	}
	Iterator end() {
		return {};
	}
};

}
