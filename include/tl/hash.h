#pragma once
#include "common.h"

// Why there is no get_hash with default implementation for any T?
// Well, there was, and it was broken. It was just getting the hash of value's raw bytes.
// The problem was in padding in structs: this implementation could return different hashes
// for values that are identical member-wise.
// So to solve this problem we would have to iterate over members and combine their hashes, but
// there is no way to do that automatically. You have to write it yourself.

template <class T>
inline constexpr tl::u64 get_hash(T const &value);

template <class T>
inline constexpr tl::u64 get_hash(T *const &value) { 
	if constexpr (requires { alignof(T); }) {
		return (tl::u64)value / alignof(T);
	} else {
		return (tl::u64)value;
	}
}

template <> inline constexpr tl::u64 get_hash(void *const &value) { return (tl::u64)value / sizeof(void *); }

template <> inline constexpr tl::u64 get_hash(tl::u8    const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::u16   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::u32   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::u64   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s8    const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s16   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s32   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s64   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::f32   const &value) { return std::bit_cast<tl::u32>(value); }
template <> inline constexpr tl::u64 get_hash(tl::f64   const &value) { return std::bit_cast<tl::u64>(value); }
template <> inline constexpr tl::u64 get_hash(tl::ascii const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::utf8  const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::utf16 const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::utf32 const &value) { return value; }

template <class T, class Size>
constexpr tl::u64 get_hash(tl::Span<T, Size> const &span) {
	tl::u64 result = 0xdeadc0debabeface;
	for (auto const &it : span) {
		result = tl::rotate_left(result, 23) ^ get_hash(it);
	}
	return result;
}

template <class T, class Allocator, class Size>
constexpr tl::u64 get_hash(tl::List<T, Allocator, Size> const &list) {
	return get_hash(as_span(list));
}

template <tl::AnEnum T>
inline constexpr tl::u64 get_hash(T const &value) {
	return tl::to_underlying(value);
}

namespace tl {

template <class Key, bool use_high_bits = true>
struct DefaultHashTraits {
	inline static constexpr u32 rehash_percentage = 70;
	inline static constexpr void on_collision(Key a, Key b) {}

	// These are templates because of situation where == operator is not defined, but are_equal is defined in derived class.
	// In that case these functions will not be instantiated.
	inline constexpr bool are_equal(this auto &&self, Key const &a, Key const &b) { return all(a == b); }
	inline constexpr u64 get_hash(this auto &&self, Key const &key) { return ::get_hash(key); }

	inline constexpr umm get_index_from_hash(this auto &&self, u64 hash, u64 capacity) {
		if constexpr (use_high_bits) {
			hash ^= hash >> 32;
			hash ^= hash >> 16;
			hash ^= hash >> 8;
		}

		assert(is_power_of_2(capacity));
		return hash & (capacity - 1);
	}
	inline constexpr umm get_index_from_key(this auto &&self, Key const &key, u64 capacity) {
		return self.get_index_from_hash(self.get_hash(key), capacity);
	}
};

#define TL_USE_HASH_TRAITS_MEMBERS \
	using Traits::rehash_percentage; \
	using Traits::on_collision; \
	using Traits::are_equal; \
	using Traits::get_hash; \
	using Traits::get_index_from_hash; \
	using Traits::get_index_from_key; \

}