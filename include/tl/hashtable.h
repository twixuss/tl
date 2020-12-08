#pragma once
#include "system.h"
#include "list.h"

namespace TL {

template <class T>
struct Hash {
	static umm get(T const &value) {
		umm hash = (umm)0x0123456789ABCDEF;
		u8 *ptr = (u8 *)std::addressof(value);
		umm remainingSize = sizeof(value);
		if constexpr (sizeof(umm) >= 8) {
			while (remainingSize >= 8) {
				remainingSize -= 8;
				hash ^= *(u64 *)ptr;
				ptr += 8;
			}
		}
		while (remainingSize >= 4) { remainingSize -= 4; hash ^= *(u32 *)ptr; ptr += 4; }
		while (remainingSize >= 2) { remainingSize -= 2; hash ^= *(u16 *)ptr; ptr += 2; }
		while (remainingSize >= 1) { remainingSize -= 1; hash ^= *(u8  *)ptr; ptr += 1; }
		return hash;
	}
};

template <class T>
struct Hash<Span<T>> {
	static umm get(Span<T> value) {
		umm hash = (umm)0x192837465AFBECD;
		u32 index = 0;
		for (auto &v : value) {
			hash ^= Hash<T>::get(v) << (index++ & (sizeof(umm) * 8 - 1));
		}
		return hash;
	}
};

template <class First, class Second>
struct Pair {
	First first;
	Second second;
};

template <class Key, class Value, class Hasher = Hash<Key>, umm _capacity = 256, class Bucket = LinkedList<Pair<Key, Value>>>
struct HashTable {
	using Pair = typename Bucket::ValueType;
	static constexpr umm capacity = _capacity;
	HashTable() {
		buckets.resize(capacity);
	}
	Value &operator[](Key const &key) {
		umm index = getIndex(key);
		Bucket &bucket = buckets[index];
		auto it = find(bucket.begin(), bucket.end(), key, [](Pair const &a, Key const &b) {
			return equals(a.first, b);
		});
		if (it == bucket.end()) {
			bucket.push_front({key});
			it = bucket.begin();
		}
		return it->second;
	}
	List<Bucket> buckets;
	umm getIndex(Key const &key) {
		umm hash = Hasher::get(key);
		umm index = 0;
		if constexpr(capacity <= 0x100) {
			for (u32 i = 0; i < sizeof(umm); i += 1) {
				index ^= hash >> (i * 8);
			}
		} else if constexpr(capacity <= 0x10000) {
			for (u32 i = 0; i < sizeof(umm); i += 2) {
				index ^= hash >> (i * 8);
			}
		} else if constexpr(capacity <= 0x100000000) {
			for (u32 i = 0; i < sizeof(umm); i += 4) {
				index ^= hash >> (i * 8);
			}
		} else {
			index = hash;
		}
		return index % capacity;
	}
};

}
