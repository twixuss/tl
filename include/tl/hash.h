#pragma once
#include "common.h"

template <class T>
inline tl::u64 get_hash(T const &value) {
	if constexpr(std::is_enum_v<T>) {
		return (std::underlying_type_t<T>)value;
	} else {
		static_assert(false, "get_hash was not specialized");
	}
}

template <class T>
inline tl::u64 get_hash(T *const &value) { return (tl::u64)value / alignof(T); }

inline tl::u64 get_hash(void *const &value) { return (tl::u64)value / sizeof(void *); }

template <> inline tl::u64 get_hash(tl::u8    const &value) { return value; }
template <> inline tl::u64 get_hash(tl::u16   const &value) { return value; }
template <> inline tl::u64 get_hash(tl::u32   const &value) { return value; }
template <> inline tl::u64 get_hash(tl::u64   const &value) { return value; }
template <> inline tl::u64 get_hash(tl::s8    const &value) { return value; }
template <> inline tl::u64 get_hash(tl::s16   const &value) { return value; }
template <> inline tl::u64 get_hash(tl::s32   const &value) { return value; }
template <> inline tl::u64 get_hash(tl::s64   const &value) { return value; }
template <> inline tl::u64 get_hash(tl::ascii const &value) { return value; }
template <> inline tl::u64 get_hash(tl::utf8  const &value) { return value; }
template <> inline tl::u64 get_hash(tl::utf16 const &value) { return value; }
template <> inline tl::u64 get_hash(tl::utf32 const &value) { return value; }

template <class T, class Size>
tl::u64 get_hash(tl::Span<T, Size> const &value) {
	tl::u64 result = 0xdeadc0debabeface;
	for (auto const &it : value) {
		result = tl::rotate_left(result, 1) ^ get_hash(it);
	}
	return result;
}
