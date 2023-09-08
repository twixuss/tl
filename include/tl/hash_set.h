#pragma once
#include "linked_list.h"
#include "array.h"
#include "hash.h"
#include "pointer.h"

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

template <class Value, class Traits = DefaultHashTraits<Value>>
struct HashSet {
	using ValueType = Value;

	using Bucket = LinkedList<Value>;

	Allocator allocator = current_allocator;
	Span<Bucket> buckets;
	umm count = 0;

	Value &insert(Value const &value TL_LP) {
		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = Traits::get_hash(value);
		auto bucket = &buckets[hash & (buckets.count - 1)];
		for (auto &it : *bucket) {
			if (it == value) {
				return it;
			}
		}

		if (count == buckets.count) {
			rehash(buckets.count * 2);
			bucket = &buckets[hash & (buckets.count - 1)];
		}
		++count;
		auto &it = bucket->add(TL_LAC);
		return it = value;
	}

	Optional<Value> erase(Value const &value) {
		if (buckets.count == 0)
			return {};

		umm hash = Traits::get_hash(value);
		auto &bucket = buckets[hash & (buckets.count - 1)];
		for (auto &it : bucket) {
			if (it == value) {
				auto result = it;
				::tl::erase(bucket, &it);
				count -= 1;
				return result;
			}
		}

		return {};
	}

	void rehash(umm new_buckets_count TL_LP) {
		scoped(allocator);

		auto old_buckets = buckets;

		buckets.data = allocator.allocate<Bucket>(new_buckets_count TL_LA);
		buckets.count = new_buckets_count;

		for (umm bucket_index = 0; bucket_index < old_buckets.count; ++bucket_index) {
			auto next_node = old_buckets[bucket_index].head;
			while (next_node) {
				auto node = next_node;
				next_node = next_node->next;
				node->next = 0;

				auto hash = Traits::get_hash(node->value);
				auto &new_bucket = buckets[hash & (new_buckets_count - 1)];
				new_bucket.add_steal(node);
			}
		}

		if (old_buckets.data) {
			allocator.free_t(old_buckets.data, old_buckets.count);
		}
	}

	void clear() {
		count = 0;
		for (umm bucket_index = 0; bucket_index < buckets.count; ++bucket_index) {
			buckets[bucket_index].clear();
		}
	}
};

template <ForEachFlags flags=0, class Value, class Traits, class Fn>
void for_each(HashSet<Value, Traits> set, Fn &&fn) {
	static_assert(flags == 0, "Only default flags supported");

	for (u32 i = 0; i < set.buckets.count; ++i) {
		for (auto &it : set.buckets[i]) {
			fn(it);
		}
	}
}

template <class Value, class Traits>
HashSet<Value, Traits> copy(HashSet<Value, Traits> const &source TL_LP) {
	HashSet<Value, Traits> result;
	for_each(source, [&](Value const &value) {
		result.insert(value TL_LA);
	});
	return result;
}

template <class Value, class Traits>
void set(HashSet<Value, Traits> &destination, HashSet<Value, Traits> const &source TL_LP) {
	destination.clear();
	for_each(source, [&](Value const &value) {
		destination.insert(value TL_LA);
	});
}

template <class Value, class Traits>
umm count_of(HashSet<Value, Traits> &set) {
	umm result = 0;
	for (auto &bucket : set.buckets) {
		result += bucket.size();
	}
	return result;
}

template <class Value, class Traits, class Fn>
umm count(HashSet<Value, Traits> set, Fn &&fn) {
	umm result = 0;
	for_each(set, [&](Value &value) {
		if (fn(value)) {
			result += 1;
		}
	});
	return result;
}

template <class Value, class Traits>
bool is_empty(HashSet<Value, Traits> set) {
	bool result = true;
	for_each(set, [&](Value &value) { result = false; return ForEach_break; });
	return result;
}

template <class Value, class Traits>
Value *find(HashSet<Value, Traits> set, Value const &value) {
	if (set.buckets.count == 0)
		return 0;
	umm hash = Traits::get_hash(value);
	auto &bucket = set.buckets[hash & (set.buckets.count - 1)];
	for (auto &it : bucket)
		if (it == value)
			return &it;
	return 0;
}

}
