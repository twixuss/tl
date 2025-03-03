#pragma once

#include "list.h"
#include "linked_list.h"
#include "hash_map_base.h"

namespace tl {

template <class Key_, class Value_, class Traits = DefaultHashTraits<Key_>, class Allocator = Allocator>
struct BucketHashMap : Traits {
	TL_USE_HASH_TRAITS_MEMBERS;

	using Key = Key_;
	using Value = Value_;
	using KeyValue = KeyValue<Key, Value>;
	using KeyValueRef = KeyValueRef<Key, Value>;

	using Element = KeyValue;

	struct HashKeyValue {
		u64 hash;
		Key key;
		Value value;
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
				if (are_equal(it.key, key)) {
					return it.value;
				}
			}
		}

		if (count >= buckets.count * rehash_percentage / 100) {
			rehash(buckets.count * 2 TL_LA);
			bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		}
		++count;
		auto &it = bucket->add(TL_LAC);
		it.hash = hash;
		it.key = key;

		return it.value;
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
				if (are_equal(it.key, key)) {
					callback(it.key, it.value);
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
		it.key = key;
		it.value = value;
		return true;
	}
	bool try_insert(Key const &key, Value value, Value **existing = 0, Key const **existing_key = 0 TL_LP) {
		if (!buckets.count) {
			rehash(16 TL_LA);
		}

		umm hash = get_hash(key);
		auto bucket = &buckets[get_index_from_hash(hash, buckets.count)];
		for (auto &it : *bucket) {
			if (it.hash == hash) {
				if (are_equal(it.key, key)) {
					if (existing)
						*existing = &it.value;
					if (existing_key)
						*existing_key = &it.key;
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
		it.key = key;
		it.value = value;
		return true;
	}

	KeyValueRef find(Key const &key) const {
		if (buckets.count == 0)
			return {};

		umm hash = get_hash(key);
		auto &bucket = buckets[get_index_from_hash(hash, buckets.count)];

		for (auto &it : bucket) {
			if (it.hash == hash) {
				if (are_equal(it.key, key)) {
					return {&it.key, &it.value};
				}
			}
		}
		return {};
	}
	Value erase(KeyValueRef kv) {
		if (buckets.count == 0)
			return {};

		--count;
		umm hash = get_hash(*kv.key);
		auto &bucket = buckets[get_index_from_hash(hash, buckets.count)];
		auto result = *kv.value;
		tl::erase(bucket, (HashKeyValue *)((u8 *)kv.key - offsetof(HashKeyValue, key)));
		return result;
	}
	Optional<Value> erase(Key const &key) {
		if (buckets.count == 0)
			return {};

		umm hash = get_hash(key);
		auto &bucket = buckets[get_index_from_hash(hash, buckets.count)];
		for (auto &it : bucket) {
			if (it.hash == hash) {
				if (are_equal(it.key, key)) {
					--count;
					auto result = it.value;
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

		buckets.data = allocator.template allocate<Bucket>(new_buckets_count TL_LA);
		buckets.count = new_buckets_count;

		for (auto old_bucket : old_buckets) {
			auto node = old_bucket.head;
			while (node) {
				auto next_node = node->next;

				auto hash = get_hash(node->value.key);
				auto &new_bucket = buckets[get_index_from_hash(hash, buckets.count)];

				old_bucket.unlink(node);
				new_bucket.add(node);

				node = next_node;
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

	
	void free() {
		for (auto &bucket : buckets) {
			bucket.free();
		}
		allocator.free(buckets.data);
		buckets = {};
	}

	template <ForEachFlags flags = 0>
	bool for_each(std::invocable<KeyValueRef> auto &&in_fn) {
		static_assert(flags == 0, "Only default flags supported");

		auto fn = wrap_foreach_fn<KeyValueRef>(in_fn);

		for (u32 i = 0; i < buckets.count; ++i) {
			for (auto &it : buckets[i]) {
				auto d = fn(KeyValueRef{&it.key, &it.value});

				switch (d & ForEach_erase_mask) {
					case ForEach_erase:
					case ForEach_erase_unordered:
						not_implemented();
				}

				if (d & ForEach_break)
					return true;
			}
		}
		return false;
	}

	template <class Allocator = tl::Allocator>
	auto map(std::invocable<KeyValueRef> auto &&fn TL_LP) {
		using U = decltype(fn(std::declval<KeyValueRef>()));
		List<U, Allocator> result;
		result.reserve(count TL_LA);
		for_each([&](auto kv) { result.add(fn(kv) TL_LA); });
		return result;
	}
	

	template <class DstAllocator = Allocator>
	auto to_list(TL_LPC) {
		return map<DstAllocator>([](auto kv) { return kv.load(); });
	}

	// TODO: make all bucket hash map functions use allocator.

	template <class Traits, class Allocator>
	void set(BucketHashMap<Key, Value, Traits, Allocator> const &source TL_LP) {
		clear();
		source.for_each([&](auto kv) {
			get_or_insert(*kv.key TL_LA) = *kv.value;
		});
	}
	
	template <class Allocator = tl::Allocator>
	auto copy(TL_LPC) {
		BucketHashMap<Key, Value, Traits, Allocator> result;
		result.set(*this);
		return result;
	}

	umm count_if(std::invocable<KeyValue> auto &&fn) {
		umm result = 0;
		for_each([&](auto &kv) {
			if (fn(kv.load())) {
				result += 1;
			}
		});
		return result;
	}

	bool is_empty() {
		bool result = true;
		for_each([&](auto kv) { (void)kv; result = false; return ForEach_break; });
		return result;
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	{
		BucketHashMap<int, int> map;
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

		BucketHashMap<int, int, CollideTraits> map;
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
