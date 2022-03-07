#pragma once
#include "common.h"

template <class T>
inline tl::u64 get_hash(T value);

template <class T>
inline tl::u64 get_hash(T *value) { return (tl::u64)value / alignof(T); }

inline tl::u64 get_hash(void *value) { return (tl::u64)value / sizeof(void *); }

template <> inline tl::u64 get_hash(tl::u8    value) { return value; }
template <> inline tl::u64 get_hash(tl::u16   value) { return value; }
template <> inline tl::u64 get_hash(tl::u32   value) { return value; }
template <> inline tl::u64 get_hash(tl::u64   value) { return value; }
template <> inline tl::u64 get_hash(tl::s8    value) { return value; }
template <> inline tl::u64 get_hash(tl::s16   value) { return value; }
template <> inline tl::u64 get_hash(tl::s32   value) { return value; }
template <> inline tl::u64 get_hash(tl::s64   value) { return value; }
template <> inline tl::u64 get_hash(tl::ascii value) { return value; }
template <> inline tl::u64 get_hash(tl::utf8  value) { return value; }
template <> inline tl::u64 get_hash(tl::utf16 value) { return value; }
template <> inline tl::u64 get_hash(tl::utf32 value) { return value; }

template <class T>
tl::u64 get_hash(tl::Span<T> value) {
	tl::u64 result = 0xdeadc0debabeface;
	for (auto const &it : value) {
		result = tl::rotate_left(result, 1) ^ get_hash(it);
	}
	return result;
}

namespace tl {

template <class T> struct DefaultHasher {
	static u64 get_hash(T value) {
		return ::get_hash(value);
	}
};

}
