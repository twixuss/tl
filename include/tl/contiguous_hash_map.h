#pragma once

#include "hash_map_base.h"
#include "multi_array.h"

namespace tl {

#ifndef TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY
#define TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY 8
#endif

static_assert(is_power_of_2(TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY));

template <class Key, class Value, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
struct ContiguousHashMap : Traits {
	TL_USE_HASH_TRAITS_MEMBERS;

	using KeyValue = KeyValue<Key, Value>;
	using KeyValuePointers = KeyValuePointers<Key, Value>;

	using Element = KeyValue;
	using FullHash = u64;
	//  0 - empty
	//  1 - removed
	// >1 - hash
	using HashAndState = u8;

	inline static constexpr HashAndState state_empty = 0;
	inline static constexpr HashAndState state_removed = 1;

	MultiArray<Allocator, true, HashAndState, Key, Value> buffer;
	umm count = 0;
	
	static HashAndState compact(FullHash full_hash) {
		return (u8)full_hash | 2;
	}
	
	struct InsertResult {
		KeyValuePointers kv;
		bool inserted;
	};

	InsertResult find_or_insert(Key const &key, Value const &default_value = {} TL_LP) {
		FullHash hash = get_hash(key);
		umm index = 0;
		if (buffer.capacity) {
			index = get_index_from_hash(hash, buffer.capacity);

			auto found = find_index_of_key_or_empty_slot(key, hash, index);

			if (found.occupied) {
				return {
					&key_at(found.index),
					&value_at(found.index),
					false,
				};
			}

			index = found.index;

			if (count >= buffer.capacity * rehash_percentage / 100) {
				if (reserve(buffer.capacity * 2 TL_LA)) {
					hash = get_hash(key);
					index = get_index_from_hash(hash, buffer.capacity);
				}
			}
		} else {
			// NOTE: TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY will be used inside `reserve`
			reserve(1 TL_LA);
			index = get_index_from_hash(hash, buffer.capacity);
		}
		count += 1;

		while (hash_and_state_at(index) > 1) {
			on_collision(key_at(index), key);
			index = step(index, buffer.capacity);
		}

		hash_and_state_at(index) = compact(hash);
		
		auto pkey = &key_at(index);
		auto pvalue = &value_at(index);
		*pkey = key;
		*pvalue = default_value;
		return {
			pkey,
			pvalue,
			true,
		};
	}
	
	Value &get_or_insert(Key const &key, Value const &default_value = {} TL_LP) {
		return *find_or_insert(key, default_value TL_LA).kv.value;
	}

	bool reserve(umm desired TL_LP) {
		if (desired <= buffer.capacity)
			return false;

		auto old_buffer = buffer;

		buffer.init(ceil_to_power_of_2(max((umm)TL_INITIAL_CONTIGUOUS_HASH_MAP_CAPACITY, desired)));

		HashAndState *old_hns    = old_buffer.template base_of<0>();
		Key          *old_keys   = old_buffer.template base_of<1>();
		Value        *old_values = old_buffer.template base_of<2>();

		HashAndState *new_hns    = buffer.template base_of<0>();
		Key          *new_keys   = buffer.template base_of<1>();
		Value        *new_values = buffer.template base_of<2>();
		
		for (umm i = 0; i < old_buffer.capacity; ++i) {
			if (old_hns[i] > 1) {
				auto &key = old_keys[i];
				auto &value = old_values[i];

				FullHash hash = get_hash(key);
				umm index = get_index_from_hash(hash, buffer.capacity);

				while (new_hns[index] > 1) {
					on_collision(new_keys[index], key);
					index = step(index, buffer.capacity);
				}

				new_hns   [index] = compact(hash);
				new_keys  [index] = key;
				new_values[index] = value;
			}
		}

		old_buffer.free();

		return true;
	}

	umm step(umm index, umm cells_count) const {
		return (index + 1) & (cells_count - 1);
	}

	struct IndexAndOccupancy {
		umm index = 0;
		bool occupied = false;
	};

	IndexAndOccupancy find_index_of_key_or_empty_slot(Key key, FullHash hash, umm index) const {
		assert(buffer.capacity);
		
		HashAndState *hnss   = buffer.template base_of<0>();
		Key          *keys   = buffer.template base_of<1>();
		
		HashAndState compact_hash = compact(hash);

		#if 1
		// Scalar version
		umm steps = buffer.capacity;
		while (steps--) {
			auto hns = hnss[index];
			if (hns == state_empty) {
				return {index, false};
			}
			if (hns == compact_hash) {
				if (are_equal(keys[index], key)) {
					return {index, true};
				}
			}
			index = step(index, buffer.capacity);
		}
		#else
		// SIMD version.
		// It was a lot slower. ~3x than scalar.
		// Simpler version that always checks 16 slots
		// was faster than this but still slower than scalar (~1.5x).
		// 
		__m128i compact_hash_m = _mm_set1_epi8(compact_hash);
		__m128i zero_m = _mm_setzero_si128();
		smm steps = buffer.capacity;
		while (steps > 0) {
			if (buffer.capacity - index >= 16) {
				// can load 16 elements.
				// Find bounds and check them
				auto hns_m = *(__m128i *)&hnss[index];
				u16 empty_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(hns_m, zero_m));
				u16 equal_hashes_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(hns_m, compact_hash_m));
			
				// ~0 if not found
				auto first_empty = find_lowest_one_bit(empty_mask);
				auto first_equal_hash = find_lowest_one_bit(equal_hashes_mask);
				auto last_equal_hash = find_highest_one_bit(equal_hashes_mask);

				if (first_empty < first_equal_hash) {
					return {index + first_empty, false};
				}

				if (equal_hashes_mask != 0) {
					// at least one hash matches
				
					//assert(first_empty > first_equal_hash);
					for (umm i = first_equal_hash; i < min(first_empty, last_equal_hash + 1); ++i) {
						umm element_index = index + i;
						if (empty_mask & (1 << i)) {
							return {element_index, false};
						}
						if (equal_hashes_mask & (1 << i)) {
							if (are_equal(keys[element_index], key)) {
								return {element_index, true};
							}
						}
					}
					if (first_empty != ~0) {
						return {index + first_empty, false};
					}
				}
				index = (index + 16) & (buffer.capacity - 1);
				steps -= 16;
			} else {
				// some of 16 elements would be out of bounds, check one by one.

				auto hns = hnss[index];
				if (hns == state_empty) {
					return {index, false};
				}
				if (hns == (compact_hash)) {
					if (are_equal(keys[index], key)) {
						return {index, true};
					}
				}
				index = step(index, buffer.capacity);
				--steps;
			}
		}
		#endif

		invalid_code_path("unreachable. table is 100% full?");
	}
	IndexAndOccupancy find_index_of_key_or_empty_slot(Key key) const {
		FullHash hash = get_hash(key);
		umm index = get_index_from_hash(hash, buffer.capacity);
		return find_index_of_key_or_empty_slot(key, hash, index);
	}

	KeyValuePointers find(Key key) const {
		if (count == 0)
			return {};

		auto found = find_index_of_key_or_empty_slot(key);

		if (found.occupied) {
			return {&key_at(found.index), &value_at(found.index)};
		}

		return {};
	}

	void clear() {
		for (umm i = 0; i < buffer.capacity; ++i) {
			hash_and_state_at(i) = state_empty;
		}
		count = 0;
	}
	KeyValue erase(KeyValuePointers kv) {
		bounds_check(assert(count));
		umm index = kv.key - &key_at(0);
		assert_equal(index, kv.value - &value_at(0));
		return mark_erased(index);
	}
	Optional<Value> erase(Key key) {
		if (auto found = find(key)) {
			return erase(found).value;
		}
		return {};
	}
	
	void free() {
		count = 0;
		buffer.free();
	}
	
	bool is_empty() const {
		return count == 0;
	}

	
	template <bool is_const>
	struct Iter {
		ContiguousHashMap *map = 0;
		umm it = 0;

		explicit operator bool() {
			return it < map->buffer.capacity;
		}

		void next() {
			while (1) {
				++it;
				if (it >= map->buffer.capacity)
					break;
				if (map->hash_and_state_at(it) > 1)
					break;
			}
		}

		Key const &key() { return map->key_at(it); }
		Value &value() { return map->value_at(it); }


		void erase() requires(!is_const) {
			map->mark_erased(it);
		}


		KeyValueReferences<Key, Value> key_value() { return {key(), value()}; }
	};

	using IterOptions = Empty;

	auto iter(this auto &&self, IterOptions options = {}) {
		auto iter = Iter<tl_self_const>{
			(ContiguousHashMap *)&self, 0
		};
		if (self.buffer.capacity && self.hash_and_state_at(0) < 2) {
			iter.next();
		}
		return iter;
	}
private:
	HashAndState &hash_and_state_at(umm index) const {
		return buffer.template at<0>(index);
	}
	Key &key_at(umm index) const {
		return buffer.template at<1>(index);
	}
	Value &value_at(umm index) const {
		return buffer.template at<2>(index);
	}
	KeyValue mark_erased(umm index) {
		assert(hash_and_state_at(index) > 1);
		hash_and_state_at(index) = state_removed;
		--count;
		return {key_at(index), value_at(index)};
	}
};


}

#ifdef TL_ENABLE_TESTS

#include "list.h"
#include "random.h"
#include "bucket_hash_map.h"
#include "time.h"

TL_TEST(ContiguousHashMap) {
	using namespace tl;

	static_assert(Collection<ContiguousHashMap<int, int>>);
	static_assert(AnIter<ContiguousHashMap<int, int>::Iter<false>>);


	auto test = []<class Map>(){
		Map map;
		assert(map.find_or_insert(42, 1).inserted);
		assert(map.find_or_insert(69, 2).inserted);
		assert(map.find_or_insert(1337, 3).inserted);
		assert(map.find_or_insert(12345, 4).inserted);
	
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

		auto list = to_list_with_keys(map);
		assert(find(list, KeyValue{42, 1}));
		assert(find(list, KeyValue{1337, 3}));
		assert(find(list, KeyValue{12345, 4}));

		
		foreach(it, map) {
			it.erase();
		}

		assert(map.count == 0);
	};

	struct CollideTraits : DefaultHashTraits<int> {
		u64 get_hash(int key) const { return 0; }
	};

	test.operator()<ContiguousHashMap<int, int>>();
	test.operator()<ContiguousHashMap<int, int, CollideTraits>>();
};

#endif
