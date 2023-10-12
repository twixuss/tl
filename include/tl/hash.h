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
inline constexpr tl::u64 get_hash(T *const &value) { return (tl::u64)value / alignof(T); }

template <> inline constexpr tl::u64 get_hash(void *const &value) { return (tl::u64)value / sizeof(void *); }

template <> inline constexpr tl::u64 get_hash(tl::u8    const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::u16   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::u32   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::u64   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s8    const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s16   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s32   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::s64   const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::ascii const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::utf8  const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::utf16 const &value) { return value; }
template <> inline constexpr tl::u64 get_hash(tl::utf32 const &value) { return value; }

template <class T, class Size>
constexpr tl::u64 get_hash(tl::Span<T, Size> const &span) {
	tl::u64 result = 0xdeadc0debabeface;
	for (auto const &it : span) {
		if (std::is_constant_evaluated()) {
			result = tl::ce::rotate_left(result, 23) ^ get_hash(it);
		} else {
			result = tl::rotate_left(result, 23) ^ get_hash(it);
		}
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
