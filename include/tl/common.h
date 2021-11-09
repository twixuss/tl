#pragma once
#include "system.h"

#if COMPILER_MSVC
	#pragma warning(push, 0)
#endif

#if COMPILER_MSVC
	#include <intrin.h>
	#include <vcruntime_new.h>
#elif COMPILER_GCC
	#include <malloc.h>
#endif

#include <utility>
#include <type_traits>
#include <tuple>
#include <source_location>

#if COMPILER_MSVC
	#pragma warning(pop)
#endif

#ifndef ASSERTION_FAILURE
#define ASSERTION_FAILURE(cause_string, expression, ...) debug_break()
#endif

#define assert_always(x, ...) (void)((bool)(x) || ((ASSERTION_FAILURE("assert", #x, __VA_ARGS__)), false))
#define assert(x, ...) assert_always(x, __VA_ARGS__)

#define invalid_code_path(...) (ASSERTION_FAILURE("invalid_code_path", "", __VA_ARGS__), __assume(0))

#ifndef bounds_check
#define bounds_check(x, ...) (void)((bool)(x) || ((ASSERTION_FAILURE("bounds check", #x, __VA_ARGS__)), false))
#endif

#define TL_DISABLED_WARNINGS \
5026 /* implicitly deleted move constructor	*/ \
5027 /* implicitly deleted move assigmnent  */

#if COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: TL_DISABLED_WARNINGS 4820 4455)
#endif

#define is_statically_overridden(function, derived, base) (&base::function != &derived::function)

#if COMPILER_MSVC
#define tl_base __super
#endif

namespace tl {

inline constexpr umm string_char_count(ascii const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_char_count(ascii *str) { return string_char_count((ascii const *)str); }

inline constexpr umm string_unit_count(ascii const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_unit_count(utf8  const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_unit_count(utf16 const *str) { umm result = 0; while (*str++) ++result; return result;}
inline constexpr umm string_unit_count(utf32 const *str) { umm result = 0; while (*str++) ++result; return result;}
inline constexpr umm string_unit_count(wchar const *str) { umm result = 0; while (*str++) ++result; return result;}
inline constexpr umm string_unit_count(ascii *str) { return string_unit_count((ascii const *)str); }
inline constexpr umm string_unit_count(utf8  *str) { return string_unit_count((utf8  const *)str); }
inline constexpr umm string_unit_count(utf16 *str) { return string_unit_count((utf16 const *)str); }
inline constexpr umm string_unit_count(utf32 *str) { return string_unit_count((utf32 const *)str); }
inline constexpr umm string_unit_count(wchar *str) { return string_unit_count((wchar const *)str); }

inline constexpr umm string_byte_count(ascii const *str) { return string_unit_count(str) * sizeof(ascii); }
inline constexpr umm string_byte_count(utf8  const *str) { return string_unit_count(str) * sizeof(utf8 ); }
inline constexpr umm string_byte_count(utf16 const *str) { return string_unit_count(str) * sizeof(utf16);}
inline constexpr umm string_byte_count(utf32 const *str) { return string_unit_count(str) * sizeof(utf32);}
inline constexpr umm string_byte_count(wchar const *str) { return string_unit_count(str) * sizeof(wchar);}
inline constexpr umm string_byte_count(ascii *str) { return string_byte_count((ascii const *)str); }
inline constexpr umm string_byte_count(utf8  *str) { return string_byte_count((utf8  const *)str); }
inline constexpr umm string_byte_count(utf16 *str) { return string_byte_count((utf16 const *)str); }
inline constexpr umm string_byte_count(utf32 *str) { return string_byte_count((utf32 const *)str); }
inline constexpr umm string_byte_count(wchar *str) { return string_byte_count((wchar const *)str); }

#define TL_HANDLE_IMPL_NAME(name) CONCAT(name, Impl)
#define TL_DECLARE_HANDLE(name) typedef struct TL_HANDLE_IMPL_NAME(name) *name;
#define TL_DEFINE_HANDLE(name) struct TL_HANDLE_IMPL_NAME(name)


template <class T, class U> inline constexpr bool is_same = false;
template <class T> inline constexpr bool is_same<T, T> = true;

template <class T> inline constexpr bool is_integer = false;
template <> inline constexpr bool is_integer<u8 > = true;
template <> inline constexpr bool is_integer<u16> = true;
template <> inline constexpr bool is_integer<u32> = true;
template <> inline constexpr bool is_integer<u64> = true;
template <> inline constexpr bool is_integer<s8 > = true;
template <> inline constexpr bool is_integer<s16> = true;
template <> inline constexpr bool is_integer<s32> = true;
template <> inline constexpr bool is_integer<s64> = true;
template <> inline constexpr bool is_integer<slong> = true;
template <> inline constexpr bool is_integer<ulong> = true;

template <class T> inline constexpr bool is_integer_like = is_integer<T>;

template <class T> inline constexpr bool is_signed = false;
template <> inline constexpr bool is_signed<s8   > = true;
template <> inline constexpr bool is_signed<s16  > = true;
template <> inline constexpr bool is_signed<s32  > = true;
template <> inline constexpr bool is_signed<s64  > = true;
template <> inline constexpr bool is_signed<slong> = true;

template <class T> inline constexpr bool is_unsigned = false;
template <> inline constexpr bool is_unsigned<u8   > = true;
template <> inline constexpr bool is_unsigned<u16  > = true;
template <> inline constexpr bool is_unsigned<u32  > = true;
template <> inline constexpr bool is_unsigned<u64  > = true;
template <> inline constexpr bool is_unsigned<ulong> = true;

template <class T> inline constexpr bool is_float = false;
template <> inline constexpr bool is_float<f32> = true;
template <> inline constexpr bool is_float<f64> = true;

template <class ...Args> inline constexpr bool is_invocable = std::is_invocable_v<Args...>;

template <class T> struct RemoveReferenceT       { using Type = T; };
template <class T> struct RemoveReferenceT<T &>  { using Type = T; };
template <class T> struct RemoveReferenceT<T &&> { using Type = T; };
template <class T> using RemoveReference = typename RemoveReferenceT<T>::Type;

template <class T> struct RemoveConstT          { using Type = T; };
template <class T> struct RemoveConstT<T const> { using Type = T; };
template <class T> using RemoveConst = typename RemoveConstT<T>::Type;

template <class T> struct RemoveVolatileT             { using Type = T; };
template <class T> struct RemoveVolatileT<T volatile> { using Type = T; };
template <class T> using RemoveVolatile = typename RemoveVolatileT<T>::Type;

template <class T> using RemoveCVRef = RemoveConst<RemoveVolatile<RemoveReference<T>>>;

template <bool v, class T = void> struct EnableIfT {};
template <class T> struct EnableIfT<true, T> { using Type = T; };
template <bool v, class T = void> using EnableIf = typename EnableIfT<v, T>::Type;

template <class ToFind, class First, class ...Rest>
inline constexpr u32 type_index(u32 start) {
	if constexpr (is_same<ToFind, First>) {
		return start;
	} else {
		return type_index<ToFind, Rest...>(start + 1);
	}
}

template <class ...Rest>
inline constexpr u32 type_count() {
	return sizeof...(Rest);
}

struct EmptyStruct {};

template <class T, class ...Args>
constexpr T *construct(T *val, Args &&...args) {
	return new(val) T(std::forward<Args>(args)...);
}

template <class T, class ...Args>
constexpr T &construct(T &val, Args &&...args) {
	return *new(std::addressof(val)) T(std::forward<Args>(args)...);
}

//template <class T>
//void swap(T &a, T &b) {
//	T temp = move(a);
//	a = move(b);
//	b = move(temp);
//}

#pragma warning(push)
#pragma warning(disable : 4309)

template <class T> inline static constexpr T min_value = {};
template <class T> inline static constexpr T max_value = {};

template <class T> inline static constexpr T min_value<T &> = min_value<T>;
template <class T> inline static constexpr T max_value<T &> = max_value<T>;

template<> inline static constexpr u8  min_value<u8 > = 0;
template<> inline static constexpr u8  max_value<u8 > = 0xFF;

template<> inline static constexpr u16 min_value<u16> = 0;
template<> inline static constexpr u16 max_value<u16> = 0xFFFF;

template<> inline static constexpr u32 min_value<u32> = 0;
template<> inline static constexpr u32 max_value<u32> = 0xFFFFFFFF;

template<> inline static constexpr u64 min_value<u64> = 0;
template<> inline static constexpr u64 max_value<u64> = 0xFFFFFFFFFFFFFFFF;

template<> inline static constexpr s8  min_value<s8 > = 0x80;
template<> inline static constexpr s8  max_value<s8 > = 0x7F;

template<> inline static constexpr s16 min_value<s16> = 0x8000;
template<> inline static constexpr s16 max_value<s16> = 0x7FFF;

template<> inline static constexpr s32 min_value<s32> = 0x80000000;
template<> inline static constexpr s32 max_value<s32> = 0x7FFFFFFF;

template<> inline static constexpr s64 min_value<s64> = 0x8000000000000000;
template<> inline static constexpr s64 max_value<s64> = 0x7FFFFFFFFFFFFFFF;

template<> inline static constexpr ulong min_value<ulong> = (ulong)min_value<ulong_s>;
template<> inline static constexpr ulong max_value<ulong> = (ulong)max_value<ulong_s>;

template<> inline static constexpr slong min_value<slong> = (slong)min_value<slong_s>;
template<> inline static constexpr slong max_value<slong> = (slong)max_value<slong_s>;

template<> inline static constexpr f32 min_value<f32> = -3.402823466e+38f;
template<> inline static constexpr f32 max_value<f32> = +3.402823466e+38f;

template<> inline static constexpr f64 min_value<f64> = -1.7976931348623158e+308;
template<> inline static constexpr f64 max_value<f64> = +1.7976931348623158e+308;

template <class T> inline static constexpr T epsilon = {};
template<> inline static constexpr f32 epsilon<f32> = 1.175494351e-38f;
template<> inline static constexpr f64 epsilon<f64> = 2.2250738585072014e-308;

template <class T> inline static constexpr T infinity = {};
template<> inline static constexpr f32 infinity<f32> = 1e300 * 1e300;
template<> inline static constexpr f64 infinity<f64> = 1e300 * 1e300;

template <class T> inline static constexpr T nan = infinity<T> * 0;

#pragma warning(pop)

#define tl_enum_flags(t) \
forceinline constexpr t operator~(t a){using u=std::underlying_type_t<t>;return (t)~(u)a;} \
forceinline constexpr t operator|(t a,t b){using u=std::underlying_type_t<t>;return (t)((u)a|(u)b);} \
forceinline constexpr t operator&(t a,t b){using u=std::underlying_type_t<t>;return (t)((u)a&(u)b);} \

template <class To, class From>
forceinline constexpr To convert(From from) {
	return (To)from;
}
namespace ce {
template <class To, class From>
constexpr To convert(From from) {
	return (To)from;
}
} // namespace ce


forceinline void add_carry(u8  a, u8  b, u8  *result, bool *carry_out) { *carry_out = _addcarry_u8 (0, a, b, result); }
forceinline void add_carry(u16 a, u16 b, u16 *result, bool *carry_out) { *carry_out = _addcarry_u16(0, a, b, result); }
forceinline void add_carry(u32 a, u32 b, u32 *result, bool *carry_out) { *carry_out = _addcarry_u32(0, a, b, result); }
#if ARCH_X64
forceinline void add_carry(u64 a, u64 b, u64 *result, bool *carry_out) { *carry_out = _addcarry_u64(0, a, b, result); }
#endif

forceinline void add_carry(u8  a, u8  b, bool carry_in, u8  *result, bool *carry_out) { *carry_out = _addcarry_u8 (carry_in, a, b, result); }
forceinline void add_carry(u16 a, u16 b, bool carry_in, u16 *result, bool *carry_out) { *carry_out = _addcarry_u16(carry_in, a, b, result); }
forceinline void add_carry(u32 a, u32 b, bool carry_in, u32 *result, bool *carry_out) { *carry_out = _addcarry_u32(carry_in, a, b, result); }
#if ARCH_X64
forceinline void add_carry(u64 a, u64 b, bool carry_in, u64 *result, bool *carry_out) { *carry_out = _addcarry_u64(carry_in, a, b, result); }
#endif

forceinline constexpr bool is_nan(f32 v) {
	union {
		u32 u;
		f32 f;
	};
	f = v;
	return ((u & 0x7f800000) == 0x7f800000) && (u & 0x007fffff);
}

forceinline constexpr bool is_nan(f64 v) {
	union {
		u64 u;
		f64 f;
	};
	f = v;

	return ((u & 0x7ff0000000000000) == 0x7ff0000000000000) && (u & 0x000fffffffffffff);
}

#if COMPILER_GCC
forceinline u32 find_lowest_one_bit(u32 val) { val ? __builtin_ffs(val) : ~0; }
forceinline u32 find_lowest_one_bit(u64 val) { val ? __builtin_ffsll(val) : ~0; }
forceinline u32 find_highest_one_bit(u32 val) { val ? 32 - __builtin_clz(val) : ~0; }
forceinline u32 find_highest_one_bit(u64 val) { val ? 64 - __builtin_clzll(val) : ~0; }
#elif COMPILER_MSVC
forceinline u32 find_lowest_one_bit(u32 val) { unsigned long result; return _BitScanForward(&result, (ulong)val) ? (u32)result : ~0; }
forceinline u32 find_highest_one_bit(u32 val) { unsigned long result; return _BitScanReverse(&result, (ulong)val) ? (u32)result : ~0; }
forceinline u32 find_lowest_zero_bit (u32 val) { return find_lowest_one_bit (~val); }
forceinline u32 find_highest_zero_bit(u32 val) { return find_highest_one_bit(~val); }
#if ARCH_X64
forceinline u32 find_lowest_one_bit(u64 val) { unsigned long result; return _BitScanForward64(&result, val) ? (u32)result : ~0; }
forceinline u32 find_highest_one_bit(u64 val) { unsigned long result; return _BitScanReverse64(&result, val) ? (u32)result : ~0; }
forceinline u32 find_lowest_zero_bit (u64 val) { return find_lowest_one_bit (~val); }
forceinline u32 find_highest_zero_bit(u64 val) { return find_highest_one_bit(~val); }
#else
#endif
#endif

namespace ce {

constexpr u32 count_bits(auto v) {
	u32 s = 0;
	for (u32 i = 0; i < (sizeof(v) * 8); ++i) {
		s += v & 1;
		v >>= 1;
	}
	return s;
}

}

forceinline constexpr u32 count_bits(u8  v) { return std::is_constant_evaluated() ? ce::count_bits(v) : (u32)_mm_popcnt_u32(v); }
forceinline constexpr u32 count_bits(u16 v) { return std::is_constant_evaluated() ? ce::count_bits(v) : (u32)_mm_popcnt_u32(v); }
forceinline constexpr u32 count_bits(u32 v) { return std::is_constant_evaluated() ? ce::count_bits(v) : (u32)_mm_popcnt_u32(v); }

#if ARCH_X64
forceinline constexpr u32 count_bits(u64 v) { return std::is_constant_evaluated() ? ce::count_bits(v) : (u32)_mm_popcnt_u64(v); }
#else
forceinline constexpr u32 count_bits(u64 v) { return std::is_constant_evaluated() ? ce::count_bits(v) : (count_bits((u32)v) + count_bits((u32)(v >> 32))); }
#endif

forceinline constexpr u32 count_bits(s8  v) { return count_bits((u8 )v); }
forceinline constexpr u32 count_bits(s16 v) { return count_bits((u16)v); }
forceinline constexpr u32 count_bits(s32 v) { return count_bits((u32)v); }
forceinline constexpr u32 count_bits(s64 v) { return count_bits((u64)v); }

forceinline constexpr bool is_power_of_2(u8  v) { return count_bits(v) == 1; }
forceinline constexpr bool is_power_of_2(u16 v) { return count_bits(v) == 1; }
forceinline constexpr bool is_power_of_2(u32 v) { return count_bits(v) == 1; }
forceinline constexpr bool is_power_of_2(u64 v) { return count_bits(v) == 1; }

template <class T> forceinline constexpr T select(bool mask, T a, T b) { return mask ? a : b; }
template <class T, class U> forceinline constexpr auto min(T a, U b) { return a < b ? a : b; }
template <class T, class U> forceinline constexpr auto max(T a, U b) { return a > b ? a : b; }
template <class T, class U, class... Rest> forceinline constexpr auto min(T a, U b, Rest... rest) { return min(min(a, b), rest...); }
template <class T, class U, class... Rest> forceinline constexpr auto max(T a, U b, Rest... rest) { return max(max(a, b), rest...); }
template <class T, class U, class V, class W> forceinline constexpr void minmax(T a, U b, V& mn, W& mx) { mn = min(a, b); mx = max(a, b); }
template <class T, class U> forceinline constexpr void minmax(T& mn, U &mx) { minmax(mn, mx, mn, mx); }

template <class T, umm count>
forceinline constexpr auto min(T (&array)[count]) {
	T result = array[0];
	for (umm i = 1; i < count; ++i) {
		result = min(result, array[i]);
	}
	return result;
}

template <class T, umm count>
forceinline constexpr auto max(T (&array)[count]) {
	T result = array[0];
	for (umm i = 1; i < count; ++i) {
		result = max(result, array[i]);
	}
	return result;
}


template <class T>
forceinline constexpr T floor(T v, T s) {
	if constexpr (is_signed<T>) {
		return ((v < 0) ? ((v + 1) / s - 1) : (v / s)) * s;
	} else if constexpr (is_unsigned<T>) {
		return v / s * s;
	} else {
		static_assert(false, "floor(T, T) can be used with integers only");
	}
}

template <class T>
forceinline constexpr T ceil(T v, T s) {
	return floor(v + s - 1, s);
}

forceinline constexpr void *floor(void *v, umm s) { return (void *)floor((umm)v, s); }
forceinline constexpr void *ceil(void *v, umm s) { return floor((u8 *)v + s - 1, s); }

template <class T>
forceinline constexpr T frac(T v, T s) {
	if constexpr(is_signed<T>) {
		return (v < 0) ? ((v + 1) % s + s - 1) : (v % s);
	} else {
		return v % s;
	}
}

forceinline f32 log(f32 x, f32 base) {
	return ::logf(x) / ::logf(base);
}

template <class Base, class Exponent, class = EnableIf<is_integer_like<Base> && is_integer_like<Exponent>>>
forceinline constexpr Base pow(Base base, Exponent exp) {
	Base res = convert<Base>((u8)1);
	while (exp) {
		if (exp & 1)
			res *= base;
		exp >>= 1;
		base *= base;
	}
	return res;
}

namespace ce {
constexpr u32 count_leading_zeros(u8 v) {
	v |= v >> (u8)(1 << 0);
	v |= v >> (u8)(1 << 1);
	v |= v >> (u8)(1 << 2);
	return ce::count_bits((u8)~v);
}
constexpr u32 count_leading_zeros(u16 v) {
	v |= v >> (u16)(1 << 0);
	v |= v >> (u16)(1 << 1);
	v |= v >> (u16)(1 << 2);
	v |= v >> (u16)(1 << 3);
	return ce::count_bits((u16)~v);
}
constexpr u32 count_leading_zeros(u32 v) {
	v |= v >> (u32)(1 << 0);
	v |= v >> (u32)(1 << 1);
	v |= v >> (u32)(1 << 2);
	v |= v >> (u32)(1 << 3);
	v |= v >> (u32)(1 << 4);
	return ce::count_bits((u32)~v);
}
constexpr u32 count_leading_zeros(u64 v) {
	v |= v >> (u64)(1 << 0);
	v |= v >> (u64)(1 << 1);
	v |= v >> (u64)(1 << 2);
	v |= v >> (u64)(1 << 3);
	v |= v >> (u64)(1 << 4);
	v |= v >> (u64)(1 << 5);
	return ce::count_bits((u64)~v);
}
}

#if COMPILER_MSVC
#if ARCH_LZCNT
forceinline constexpr u32 count_leading_zeros(u8  val) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt16(val) - 8; }
forceinline constexpr u32 count_leading_zeros(u16 val) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt16(val); }
forceinline constexpr u32 count_leading_zeros(u32 val) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt32(val); }
forceinline constexpr u32 count_leading_zeros(u64 val) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : (u32)__lzcnt64(val); }
#else
forceinline constexpr u32 count_leading_zeros(u8  v) { ulong r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 8  : (_BitScanReverse(&r, v),  7 - r))); }
forceinline constexpr u32 count_leading_zeros(u16 v) { ulong r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 16 : (_BitScanReverse(&r, v), 15 - r))); }
forceinline constexpr u32 count_leading_zeros(u32 v) { ulong r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 32 : (_BitScanReverse(&r, v), 31 - r))); }
#if ARCH_X64
forceinline constexpr u32 count_leading_zeros(u64 v) { ulong r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 64 : (_BitScanReverse64(&r, v), 63 - r))); }
#endif
#endif
#endif

forceinline constexpr u32 count_leading_zeros(s8  val) { return count_leading_zeros((u8 )val); }
forceinline constexpr u32 count_leading_zeros(s16 val) { return count_leading_zeros((u16)val); }
forceinline constexpr u32 count_leading_zeros(s32 val) { return count_leading_zeros((u32)val); }
#if ARCH_X64
forceinline constexpr u32 count_leading_zeros(s64 val) { return count_leading_zeros((u64)val); }
#endif


forceinline constexpr u32 count_leading_ones(u8  val) { return count_leading_zeros((u8 )~val); }
forceinline constexpr u32 count_leading_ones(u16 val) { return count_leading_zeros((u16)~val); }
forceinline constexpr u32 count_leading_ones(u32 val) { return count_leading_zeros((u32)~val); }
#if ARCH_X64
forceinline constexpr u32 count_leading_ones(u64 val) { return count_leading_zeros((u64)~val); }
#endif


namespace ce {
// https://stackoverflow.com/a/24748637
#define S(k) if (n >= ((decltype(n))1 << k)) { i += k; n >>= k; }
constexpr s32 log2(u8 n)  { s32 i = -(n == 0); S(4); S(2); S(1); return i; }
constexpr s32 log2(u16 n) { s32 i = -(n == 0); S(8); S(4); S(2); S(1); return i; }
constexpr s32 log2(u32 n) { s32 i = -(n == 0); S(16); S(8); S(4); S(2); S(1); return i; }
constexpr s32 log2(u64 n) { s32 i = -(n == 0); S(32); S(16); S(8); S(4); S(2); S(1); return i; }
#undef S
}
forceinline constexpr u32 log2(u8  v) { ulong r; return (v == 0) ? -1 : (std::is_constant_evaluated() ? (7  - ce::count_leading_zeros(v)) : (_BitScanReverse(&r, v), r)); }
forceinline constexpr u32 log2(u16 v) { ulong r; return (v == 0) ? -1 : (std::is_constant_evaluated() ? (15 - ce::count_leading_zeros(v)) : (_BitScanReverse(&r, v), r)); }
forceinline constexpr u32 log2(u32 v) { ulong r; return (v == 0) ? -1 : (std::is_constant_evaluated() ? (31 - ce::count_leading_zeros(v)) : (_BitScanReverse(&r, v), r)); }
#if ARCH_X64
forceinline constexpr u32 log2(u64 v) { ulong r; return (v == 0) ? -1 : (std::is_constant_evaluated() ? (63 - ce::count_leading_zeros(v)) : (_BitScanReverse64(&r, v), r)); }
#endif

forceinline constexpr u32 log2(s8  v) { return log2((u8 )v); }
forceinline constexpr u32 log2(s16 v) { return log2((u16)v); }
forceinline constexpr u32 log2(s32 v) { return log2((u32)v); }
#if ARCH_X64
forceinline constexpr u32 log2(s64 v) { return log2((u64)v); }
#endif


forceinline constexpr u8  floor_to_power_of_2(u8  v) { return v == 0 ? (u8 )0 : (u8 )1 << (u8 )log2(v); }
forceinline constexpr u16 floor_to_power_of_2(u16 v) { return v == 0 ? (u16)0 : (u16)1 << (u16)log2(v); }
forceinline constexpr u32 floor_to_power_of_2(u32 v) { return v == 0 ? (u32)0 : (u32)1 << (u32)log2(v); }
#if ARCH_X64
forceinline constexpr u64 floor_to_power_of_2(u64 v) { return v == 0 ? (u64)0 : (u64)1 << (u64)log2(v); }
#endif

namespace ce {
inline constexpr u8 floor_to_power_of_2(u8 x) {
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    return x - (x >> 1);
}
inline constexpr u16 floor_to_power_of_2(u16 x) {
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    return x - (x >> 1);
}
inline constexpr u32 floor_to_power_of_2(u32 x) {
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}
inline constexpr u64 floor_to_power_of_2(u64 x) {
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    x = x | (x >> 32);
    return x - (x >> 1);
}
}

forceinline constexpr u32 ceil_to_power_of_2(u32 v) { return v == 0 ? 0 : (u32)1 << (u32)(32 - count_leading_zeros(v - 1)); }

#define S(k, m) if (n >= (decltype(n))m) { i += k; n /= (decltype(n))m; }
forceinline constexpr s32 log10(u8  n) { s32 i = -(n == 0); S(2,100)S(1,10)return i; }
forceinline constexpr s32 log10(u16 n) { s32 i = -(n == 0); S(4,10000)S(2,100)S(1,10)return i; }
forceinline constexpr s32 log10(u32 n) { s32 i = -(n == 0); S(8,100000000)S(4,10000)S(2,100)S(1,10)return i; }
#if ARCH_X64
forceinline constexpr s32 log10(u64 n) { s32 i = -(n == 0); S(16,10000000000000000)S(8,100000000)S(4,10000)S(2,100)S(1,10)return i; }
#endif
#undef S

forceinline constexpr s32 log(u32 n, u32 base) {
	struct Log {
		u32 value;
		u32 power;
	};
	Log logs[5] = {};
	logs[0].value = base;
	logs[0].power = 1;
	u32 log_count = 1;
	for (; log_count < 5; ++log_count) {
		Log &log = logs[log_count];
		Log &previous_log = logs[log_count - 1];
		if (max_value<u32> / previous_log.value < previous_log.value)
			break;
		log.value = previous_log.value * previous_log.value;
		log.power = previous_log.power * 2;
	}

	s32 i = -(n == 0);
	for (s32 log_index = (s32)log_count - 1; log_index >= 0; log_index--) {
		Log &log = logs[log_index];
		if (n >= log.value) {
			i += log.power;
			n /= log.value;
		}
	}
	return i;
}

forceinline constexpr f32 floor(f32 v) { return std::is_constant_evaluated() ? (v >= 0 ? (f32)(s32)v : ((f32)(s32)v - 1)) : ::floorf(v); }
forceinline constexpr f64 floor(f64 v) { return std::is_constant_evaluated() ? (v >= 0 ? (f64)(s64)v : ((f64)(s64)v - 1)) : ::floor (v); }

forceinline f32 ceil(f32 v) { return ::ceilf(v); }
forceinline f64 ceil(f64 v) { return ::ceil(v); }
forceinline s32 ceil_to_int(f32 v) { return (s32)ceil(v); }
forceinline s64 ceil_to_int(f64 v) { return (s64)ceil(v); }

forceinline f32 frac(f32 v) { return v - floor(v); }
forceinline f64 frac(f64 v) { return v - floor(v); }

forceinline constexpr bool is_negative(f32 v) { return *(u32 *)&v & 0x80000000; }
forceinline constexpr bool is_negative(f64 v) { return *(u64 *)&v & 0x8000000000000000; }

forceinline constexpr bool is_positive(f32 v) { return !(*(u32 *)&v & 0x80000000); }
forceinline constexpr bool is_positive(f64 v) { return !(*(u64 *)&v & 0x8000000000000000); }

#if COMPILER_GCC
forceinline u8  rotate_left (u8  v, s32 shift = 1) { return (v << shift) | (v >> ( 8 - shift)); }
forceinline u16 rotate_left (u16 v, s32 shift = 1) { return (v << shift) | (v >> (16 - shift)); }
forceinline u32 rotate_left (u32 v, s32 shift = 1) { return (v << shift) | (v >> (32 - shift)); }
forceinline u64 rotate_left (u64 v, s32 shift = 1) { return (v << shift) | (v >> (64 - shift)); }
forceinline u8  rotate_right(u8  v, s32 shift = 1) { return (v >> shift) | (v << ( 8 - shift)); }
forceinline u16 rotate_right(u16 v, s32 shift = 1) { return (v >> shift) | (v << (16 - shift)); }
forceinline u32 rotate_right(u32 v, s32 shift = 1) { return (v >> shift) | (v << (32 - shift)); }
forceinline u64 rotate_right(u64 v, s32 shift = 1) { return (v >> shift) | (v << (64 - shift)); }
#else
forceinline u8  rotate_left (u8  v, s32 shift = 1) { return _rotl8(v, (u8)shift); }
forceinline u16 rotate_left (u16 v, s32 shift = 1) { return _rotl16(v, (u8)shift); }
forceinline u32 rotate_left (u32 v, s32 shift = 1) { return _rotl(v, shift); }
forceinline u64 rotate_left (u64 v, s32 shift = 1) { return _rotl64(v, shift); }
forceinline u8  rotate_right(u8  v, s32 shift = 1) { return _rotr8(v, (u8)shift); }
forceinline u16 rotate_right(u16 v, s32 shift = 1) { return _rotr16(v, (u8)shift); }
forceinline u32 rotate_right(u32 v, s32 shift = 1) { return _rotr(v, shift); }
forceinline u64 rotate_right(u64 v, s32 shift = 1) { return _rotr64(v, shift); }
#endif

template <class T>
void swap(T &a, T &b) {
	T t = a;
	a = b;
	b = t;
}

template <class T>
constexpr T midpoint(T a, T b) {
	minmax(a, b, a, b);
	return a + (b - a) / 2;
}

template <class T>
constexpr T *midpoint(T *a, T *b) {
	minmax(a, b, a, b);
	return a + ((umm)(b - a) >> 1);
}

template <class... Callables>
struct Combine : public Callables... {
	inline constexpr Combine(Callables &&... c) : Callables(std::move(c))... {}
	using Callables::operator()...;
};

template <class Enum, class = EnableIf<std::is_enum_v<Enum>>>
inline constexpr auto toInt(Enum e) {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class T, umm count>
constexpr umm count_of(T const (&arr)[count]) { (void)arr; return count; }

template <class T, umm count>
constexpr T &back(T (&arr)[count]) { return arr[count - 1]; }
template <class T, umm count>
constexpr T const &back(T const (&arr)[count]) { return arr[count - 1]; }

template <class T, umm count>
constexpr T &front(T (&arr)[count]) { return arr[0]; }
template <class T, umm count>
constexpr T const &front(T const (&arr)[count]) { return arr[0]; }

inline void copyMemory(void *_dst, void const *_src, umm count) {
	u8 *dst = (u8 *)_dst;
	u8 const *src = (u8 const *)_src;
	while (count--) {
		*dst++ = *src++;
	}
}

template <class T>
inline void populate(void *_dst, T const &src, umm count) {
	T *dst = (T *)_dst;
	while (count--) {
		copyMemory(dst++, std::addressof(src), sizeof(T));
	}
}

template <class T>
inline bool memory_equals(T const *a, T const *b) {
	return memcmp(a, b, sizeof(T)) == 0;
}

template <class T, class U>
inline bool memory_equals(T const *a, U const *b) {
	static_assert(sizeof(T) == sizeof(U), "memory_equals: got different sizes");
	return memcmp(a, b, sizeof(T)) == 0;
}

inline bool all_true(bool value) { return value; }
inline bool any_true(bool value) { return value; }
inline bool all_false(bool value) { return !value; }
inline bool any_false(bool value) { return !value; }

enum ForEachDirective {
	ForEach_continue,
	ForEach_break,
	ForEach_erase,
};

#define for_each_break    return ForEach_break
#define for_each_continue return ForEach_continue

using ForEachFlags = u8;
enum : ForEachFlags {
	ForEach_reverse = 0x1,
};

template <class Container, class Fn>
constexpr void for_each(Container &container, Fn &&fn) {
	return for_each<(ForEachFlags)0>(container, fn);
}

template <ForEachFlags flags, class T, umm count, class Fn>
constexpr void for_each(T (&array)[count], Fn &&fn) {
	using FnRet = decltype(fn(*(T*)0));

	T *start;
	T *end;
	umm step;
	if constexpr (flags & ForEach_reverse) {
		start = array + count - 1;
		end = array - 1;
		step = -1;
	} else {
		start = array;
		end = array + count;
		step = 1;
	}
	for (auto it = start; it != end; it += step) {
		if constexpr (is_same<FnRet, void>) {
			fn(*it);
		} else if constexpr (is_same<FnRet, ForEachDirective>) {
			switch (fn(*it)) {
				case ForEach_break: return;
			}
		} else {
			static_assert(false, "Invalid return type of for_each function");
		}
	}
}

template <class Container>
struct ValueTypeOfT {
	using Type = typename Container::ValueType;
};

template <class T, umm count>
struct ValueTypeOfT<T[count]> {
	using Type = T;
};

template <class T>
using ValueTypeOf = ValueTypeOfT<T>::Type;

template <class Container, class Predicate>
constexpr auto find_if(Container &container, Predicate &&predicate) {
	ValueTypeOf<Container> *result = 0;
	for_each(container, [&] (auto &it) {
		if (predicate(it)) {
			result = &it;
			for_each_break;
		}
		for_each_continue;
	});
	return result;
}

template <class Predicate, class Iterator>
constexpr Iterator find_if(Iterator begin, Iterator end, Predicate &&predicate) {
	for (Iterator it = begin; it != end; ++it) {
		if (predicate(*it)) {
			return it;
		}
	}
	return 0;
}
template <class T, class Iterator>
constexpr Iterator find(Iterator begin, Iterator end, T const &value) {
	for (Iterator it = begin; it != end; ++it) {
		if (all_true(*it == value)) {
			return it;
		}
	}
	return 0;
}
template <class T, class Iterator, class Compare>
constexpr Iterator find(Iterator begin, Iterator end, T const &value, Compare &&compare) {
	for (Iterator it = begin; it != end; ++it) {
		if (compare(*it, value)) {
			return it;
		}
	}
	return 0;
}
template <class Iterator, class CmpIterator>
constexpr Iterator find(Iterator src_begin, Iterator src_end, CmpIterator cmp_begin, CmpIterator cmp_end) {
	umm src_count = (umm)(src_end - src_begin);
	umm cmp_count = (umm)(cmp_end - cmp_begin);
	for (umm i = 0; i < src_count - cmp_count + 1; ++i) {
		for (umm j = 0; j < cmp_count; ++j) {
			if (cmp_begin[j] != src_begin[i + j]) {
				goto continue_first;
			}
		}
		return src_begin + i;
	continue_first:;
	}
	return 0;
}

template <class Fn>
struct Deferrer {
	inline Deferrer(Fn &&fn) : fn(std::move(fn)) {}
	inline ~Deferrer() { fn(); }

private:
	Fn fn;
#if COMPILER_MSVC
#pragma warning(suppress: 4626)
};
#else
};
#endif

#define defer ::tl::Deferrer CONCAT(_deferrer, __LINE__) = [&]

template <class T>
auto reverse(T &x) {
	using Iter = decltype(x.rbegin());
	struct Range {
		Iter begin() { return {_begin}; }
		Iter end() { return {_end}; }
		Iter _begin, _end;
	};
	Range r = {
		x.rbegin(),
		x.rend(),
	};
	return r;
}

template <class Iterator>
struct ReverseIterator {
	using ValueType = decltype(*Iterator{});

	Iterator it;
	ReverseIterator(Iterator it) : it(it) {}
	ReverseIterator &operator++() { return --it, *this; }
	ReverseIterator operator++(int) { auto temp = *this; return --it, temp; }
	bool operator==(ReverseIterator that) const { return it == that.it; }
	bool operator!=(ReverseIterator that) const { return it != that.it; }
	ValueType &operator*() { return *it; }
	Iterator operator->() { return it; }
};

template <class T>
struct Span {
	using ValueType = T;
	using ReverseIterator = ReverseIterator<T *>;

	constexpr Span(std::initializer_list<ValueType> list) : data((ValueType *)list.begin()), count(list.size()) {}
	constexpr Span() = default;
	constexpr Span(ValueType &value) : data(std::addressof(value)), count(1) {}
	template <umm count>
	constexpr Span(ValueType (&array)[count]) : data(array), count(count) {}
	constexpr Span(ValueType *begin, ValueType *end) : data(begin), count(end - begin) {}
	constexpr Span(ValueType const *begin, ValueType const *end) : data((ValueType *)begin), count(end - begin) {}
	constexpr Span(ValueType *begin, umm count) : data(begin), count(count) {}
	constexpr ValueType *begin() const { return data; }
	constexpr ValueType *end() const { return data + count; }

	constexpr ReverseIterator rbegin() const { return data + count - 1; }
	constexpr ReverseIterator rend() const { return data - 1; }

	constexpr ValueType &front() const {
		bounds_check(count);
		return *data;
	}
	constexpr ValueType &back() const {
		bounds_check(count);
		return data[count - 1];
	}
	constexpr ValueType &operator[](umm i) const {
		bounds_check(i < count);
		return data[i];
	}
	constexpr ValueType &at(umm i) const {
		bounds_check(i < count);
		return data[i];
	}
	constexpr bool empty() const { return count == 0; }

	template <class U>
	constexpr explicit operator Span<U>() const {
		static_assert(sizeof(U) == sizeof(T));
		return {(U *)data, count};
	}

	//constexpr operator Span<utf8>() const { // Really this cast should be defined only in Span<char>, but this is impossible to do in this language without copypasta
	//	static_assert(is_same<T, ascii>);
	//	return {(utf8 *)data, count * sizeof(ValueType)};
	//}

	constexpr bool operator==(Span<ValueType> that) const {
		if (count != that.count)
			return false;
		for (umm i = 0; i < count; ++i) {
			if (data[i] != that.data[i])
				return false;
		}
		return true;
	}
	constexpr bool operator!=(Span<ValueType> that) const { return !(*this == that); }

	constexpr Span<T> subspan(umm subspan_start, umm subspan_count) const {
		return Span(data + subspan_start, subspan_count);
	}

	ValueType *data = 0;
	umm count = 0;
};

template <ForEachFlags flags, class T, class Fn>
constexpr void for_each(Span<T> span, Fn &&fn) {
	using FnRet = decltype(fn(*(T*)0));

	T *start = 0;
	T *end = 0;
	umm step = 0;
	if constexpr (flags & ForEach_reverse) {
		start = span.data + span.count - 1;
		end = span.data - 1;
		step = -1;
	} else {
		start = span.data;
		end = span.data + span.count;
		step = 1;
	}

	for (auto it = start; it != end; it += step) {
		if constexpr (is_same<FnRet, void>) {
			fn(*it);
		} else if constexpr (is_same<FnRet, ForEachDirective>) {
			if (fn(*it) == ForEach_break) {
				break;
			}
		} else {
			static_assert(false, "Invalid return type of for_each function");
		}
	}
}

forceinline constexpr Span<char > operator""s(char  const *string, umm count) { return Span((char  *)string, count); }
forceinline constexpr Span<utf8 > operator""s(utf8  const *string, umm count) { return Span((utf8  *)string, count); }
forceinline constexpr Span<utf16> operator""s(utf16 const *string, umm count) { return Span((utf16 *)string, count); }
forceinline constexpr Span<wchar> operator""s(wchar const *string, umm count) { return Span((wchar *)string, count); }
forceinline constexpr Span<char > operator""ts(char  const *string, umm count) { return Span((char  *)string, count + 1); }
forceinline constexpr Span<utf8 > operator""ts(utf8  const *string, umm count) { return Span((utf8  *)string, count + 1); }
forceinline constexpr Span<utf16> operator""ts(utf16 const *string, umm count) { return Span((utf16 *)string, count + 1); }
forceinline constexpr Span<wchar> operator""ts(wchar const *string, umm count) { return Span((wchar *)string, count + 1); }
forceinline Span<u8> operator""b(char const *string, umm count) { return Span((u8 *)string, count); }

template <class T, umm count>
inline constexpr Span<T> array_as_span(T const (&arr)[count]) { return Span((T *)arr, count); }

template <class T>
inline constexpr Span<T> as_span(std::initializer_list<T> list) { return Span((T *)list.begin(), list.size()); }

inline constexpr Span<char > as_span(char  const *str) { return Span((char  *)str, string_unit_count(str)); }
inline constexpr Span<wchar> as_span(wchar const *str) { return Span((wchar *)str, string_unit_count(str)); }
inline constexpr Span<utf8 > as_span(utf8  const *str) { return Span((utf8  *)str, string_unit_count(str)); }
inline constexpr Span<utf16> as_span(utf16 const *str) { return Span((utf16 *)str, string_unit_count(str)); }
inline constexpr Span<utf32> as_span(utf32 const *str) { return Span((utf32 *)str, string_unit_count(str)); }

template <class T>
inline constexpr Span<T> as_span(Span<T> span) {
	return span;
}

template <class T>
constexpr Span<utf8> as_utf8(Span<T> span) {
	return {(utf8 *)span.begin(), span.count * sizeof(T)};
}
template <class T>
constexpr Span<utf8> as_utf8(T span_like) {
	return as_utf8(as_span(span_like));
}


template <class T>
constexpr Span<u8> as_bytes(T span_like) {
	return as_bytes(as_span(span_like));
}

template <class T>
constexpr Span<u8> as_bytes(Span<T> span) {
	return {(u8 *)span.begin(), span.count * sizeof(T)};
}

template <class T>
constexpr Span<char> as_chars(Span<T> span) {
	return {(char *)span.begin(), span.count * sizeof(T)};
}

template <class T>
constexpr Span<u8> value_as_bytes(T const &value) {
	return {(u8 *)&value, sizeof(T)};
}

template <class T> constexpr umm count_of(Span<T> span) { return span.count; }

template <class T>
constexpr void replace(Span<T> destination, Span<T> source, umm start_index = 0) {
	for (umm i = 0; i < source.count; ++i) {
		destination[start_index + i] = source[i];
	}
}

template <class T, class U>
inline constexpr bool starts_with(Span<T> str, Span<U> sub_str) {
	if (sub_str.count > str.count)
		return false;
	for (umm i = 0; i < sub_str.count; ++i) {
		if (str.data[i] != sub_str.data[i]) {
			return false;
		}
	}
	return true;
}

template <class T, class U>
inline constexpr bool ends_with(Span<T> str, Span<U> sub_str) {
	if (sub_str.count > str.count)
		return false;
	umm base_offset = str.count - sub_str.count;
	for (umm i = 0; i < sub_str.count; ++i) {
		if (str.data[i + base_offset] != sub_str.data[i]) {
			return false;
		}
	}
	return true;
}

template <class T, umm count> constexpr T *find(T (&arr)[count], T const &value) { return find(arr, arr + count, value); }
template <class T> constexpr T *find(Span<T> span, T const &value) { return find(span.begin(), span.end(), value); }
template <class T> constexpr T *find(Span<T> span, Span<T> cmp) { return find(span.begin(), span.end(), cmp.begin(), cmp.end()); }

template <class T> constexpr T *find(Span<T> where, Span<Span<T>> whats) {
	while (where.count) {
		for (auto what : whats) {
			if (starts_with(where, what)) {
				return where.data;
			}
		}
		where.data += 1;
		where.count -= 1;
	}
	return 0;
}

template <class T>
constexpr T *find_last(Span<T> span, T const &value) {
	if (span.count == 0)
		return 0;

	for (auto it = span.end() - 1; it >= span.begin(); --it) {
		if (*it == value)
			return it;
	}
	return 0;
}

template <class T>
constexpr Span<T> find_last(Span<T> span, Span<T> sub) {
	Span<T> dest = {span.end() - sub.count, sub.count};
	while (dest.data >= span.data) {
		if (dest == sub)
			return dest;

		dest.data--;
	}
	return {};
}

template <class T, class Predicate>
constexpr T *find_if(Span<T> span, Predicate &&predicate) {
	for (auto &v : span) {
		if (predicate(v)) {
			return std::addressof(v);
		}
	}
	return 0;
}

template <class T>
constexpr T *find_any(Span<T> where, Span<T> what) {
	for (auto &a : where) {
		for (auto &b : what) {
			if (a == b) {
				return &a;
			}
		}
	}
	return 0;
}

template <class T>
constexpr T *find_last_any(Span<T> where, Span<T> what) {
	T *result = 0;
	for_each<ForEach_reverse>(where, [&](T &a) {
		for (auto &b : what) {
			if (a == b) {
				result = &a;
				for_each_break;
			}
		}
		for_each_continue;
	});
	return result;
}

template <class Collection, class T>
umm find_index_of(Collection &collection, T value) {
	return index_of(collection, find(collection, value));
}

inline constexpr bool is_whitespace(ascii c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

inline constexpr bool is_whitespace(utf32 c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

inline constexpr bool is_alpha(ascii c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline constexpr bool is_alpha(utf32 c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline constexpr bool is_digit(ascii c) {
	return c >= '0' && c <= '9';
}
inline constexpr bool is_digit(utf32 c) {
	return c >= '0' && c <= '9';
}

inline constexpr bool is_hex_digit(utf32 c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

inline constexpr ascii to_lower(ascii c) {
	if (c >= 'A' && c <= 'Z')
		return (char)(c + ('a' - 'A'));
	return c;
}
inline constexpr utf32 to_lower(utf32 c) {
	if (c >= 'A' && c <= 'Z')
		return c + ('a' - 'A');
	return c;
}

inline constexpr ascii to_upper(ascii c) {
	if (c >= 'a' && c <= 'z')
		return (ascii)(c - ('a' - 'A'));
	return c;
}
inline constexpr utf32 to_upper(utf32 c) {
	if (c >= 'a' && c <= 'z')
		return c - ('a' - 'A');
	return c;
}

template <class T, class U>
inline constexpr bool equals(Span<T> a, Span<U> b) {
	if (a.count != b.count)
		return false;
	auto ap = a.begin();
	for (auto bp = b.begin(); ap != a.end(); ++ap, ++bp)
		if (*ap != *bp)
			return false;
	return true;
}

inline bool equals_case_insensitive(ascii a, ascii b) { return to_lower(a) == to_lower(b); }
inline bool equals_case_insensitive(utf32 a, utf32 b) { return to_lower(a) == to_lower(b); }

template <class T, class Predicate, class = EnableIf<is_invocable<Predicate, T, T>>>
inline constexpr bool ends_with(Span<T> str, Span<T> sub_str, Predicate &&predicate) {
	if (sub_str.count > str.count)
		return false;
	umm base_offset = str.count - sub_str.count;
	for (umm i = 0; i < sub_str.count; ++i) {
		if (!predicate(str.data[i + base_offset], sub_str.data[i])) {
			return false;
		}
	}
	return true;
}

inline constexpr void set_to_lower_case(Span<ascii> span) {
	for (auto &c : span) {
		c = to_lower(c);
	}
}
inline constexpr void set_to_lower_case(Span<utf32> span) {
	for (auto &c : span) {
		c = to_lower(c);
	}
}

template <class T, class Compare>
inline constexpr bool is_any_of(T &value, Span<T> span, Compare &&compare) {
	for (auto &c : span) {
		if (compare(value, c)) {
			return true;
		}
	}
	return false;
}
template <class T>
inline constexpr bool is_any_of(T &value, Span<T> span) {
	return is_any_of(value, span, [](T &a, T &b) { return a == b; });
}

inline constexpr Span<char> skip_chars(Span<char> span, Span<char> chars_to_skip) {
	if (span.count == 0) {
		return span;
	}
	while (is_any_of(span.front(), chars_to_skip)) {
		++span.data;
		--span.count;
		if (span.count == 0) {
			break;
		}
	}
	return span;
}

template <class T>
struct Storage_Trivial {
	union {
		T value;
	};
};

template <class T>
struct Storage_NonTrivial {
	union {
		T value;
	};
	forceinline constexpr Storage_NonTrivial() {}
	forceinline ~Storage_NonTrivial() {}
};

template <class T>
struct Storage : Conditional<std::is_trivial_v<T>, Storage_Trivial<T>, Storage_NonTrivial<T>> {};

template <class T, umm _capacity>
struct StaticList {
	using ValueType = T;
	using Storage = Storage<T>;

	inline static constexpr umm capacity = _capacity;

	forceinline constexpr StaticList() {}

	template <umm that_capacity>
	constexpr StaticList(StaticList<T, that_capacity> const &that) {
		count = that.count;
		memcpy(data, that.data, count * sizeof(T));
	}

	template <umm that_capacity>
	constexpr StaticList(StaticList<T, that_capacity> &&that) = delete;

	constexpr StaticList(std::initializer_list<T> that) {
		count = that.size();
		memcpy(data, that.begin(), count * sizeof(T));
	}

	template <umm that_capacity>
	constexpr StaticList &operator=(StaticList<T, that_capacity> const &that) {
		return *new (this) StaticList(that);
	}

	template <umm that_capacity>
	constexpr StaticList &operator=(StaticList<T, that_capacity> &&that) = delete;

	constexpr StaticList &operator=(std::initializer_list<T> that) {
		return *new (this) StaticList(that);
	}

	forceinline constexpr T const *begin() const { return data; }
	forceinline constexpr T const *end() const { return data + count; }

	forceinline constexpr T *begin() { return data; }
	forceinline constexpr T *end() { return data + count; }

	forceinline constexpr bool empty() const { return count == 0; }
	forceinline constexpr bool full() const { return count == capacity; }

	forceinline constexpr T &front() { bounds_check(count); return data[0]; }
	forceinline constexpr T &back() { bounds_check(count); return data[count - 1]; }
	forceinline constexpr T &operator[](umm i) { bounds_check(count); return data[i]; }

	forceinline constexpr T const &front() const { bounds_check(count); return data[0]; }
	forceinline constexpr T const &back() const { bounds_check(count); return data[count - 1]; }
	forceinline constexpr T const &operator[](umm i) const { bounds_check(count); return data[i]; }

	constexpr void resize(umm new_count) {
		bounds_check(new_count <= capacity);
		if (new_count > count) {
			for (umm i = count; i < new_count; ++i) {
				new (data + i) T();
			}
		}
		count = new_count;
	}
	constexpr T &insert_at(T value, umm where) {
		bounds_check(where <= count);
		bounds_check(count < capacity);

		memmove(data + where + 1, data + where, (count - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++count;
		return data[where];
	}
	constexpr Span<T> insert_at(Span<T> span, umm where) {
		bounds_check(where <= count);
		bounds_check(count + span.count <= capacity);

		memmove(data + where + span.count, data + where, (count - where) * sizeof(T));
		memcpy(data + where, span.data, span.count * sizeof(T));

		count += span.count;
		return {data + where, span.count};
	}
	constexpr Span<T> insert(Span<T> span, T *where) {
		return insert_at(span, where - data);
	}

	forceinline constexpr StaticList &operator+=(T const &that) { add(that); return *this; }
	forceinline constexpr StaticList &operator+=(T &&that) { add(std::move(that)); return *this; }
	forceinline constexpr StaticList &operator+=(Span<T> that) { add(that); return *this; }
	template <umm capacity>
	forceinline constexpr StaticList &operator+=(StaticList<T, capacity> const &that) { add(as_span(that)); return *this; }
	forceinline constexpr StaticList &operator+=(std::initializer_list<T> that) { add(Span((T *)that.begin(), (T *)that.end())); return *this; }

	forceinline constexpr operator Span<T>() { return {data, count}; }

	forceinline constexpr T &add() {
		bounds_check(!full());
		return *new(data + count++) T();
	}

	forceinline constexpr T &add(T const &value) {
		bounds_check(!full());
		memcpy(data + count, &value, sizeof(T));
		return data[count++];
	}

	forceinline constexpr Span<T> add(Span<T> span) {
		bounds_check(count + span.count <= capacity);
		memcpy(data + count, span.data, span.count * sizeof(T));
		defer { count += span.count; };
		return {data + count, span.count};
	}

	constexpr T pop_back() {
		bounds_check(count);
		return data[--count];
	}
	constexpr T pop_front() {
		bounds_check(count);
		T popped = *data;
		memmove(data, data + 1, --count * sizeof(T));
		return popped;
	}
	forceinline constexpr void pop_back_unordered() { erase_unordered(&back()); }
	forceinline constexpr void pop_front_unordered() { erase_unordered(begin()); }

	T erase_at(umm where) {
		bounds_check(where < count);
		T erased = data[where];
		memmove(data + where, data + where + 1, (--count - where) * sizeof(T));
		return erased;
	}
	forceinline T erase(T *where) { return erase_at(where - data); }

	T erase_at_unordered(umm where) {
		bounds_check(where < count);
		T erased = data[where];
		--count;
		if (count != where) {
			data[where] = data[count];
		}
		return erased;
	}
	forceinline T erase_unordered(T *where) { return erase_at_unordered(where - data); }

	constexpr void clear() {
		count = 0;
	}

	umm count = 0;
	union {
		T data[capacity];
	};
};

template <class T, umm capacity>
umm index_of(StaticList<T, capacity> const &list, T const *value) {
	return value - list.data;
}

template <class T, umm capacity> Span<T> as_span(StaticList<T, capacity> const &list) { return {(T *)list.data, list.count}; }

template <class T, umm capacity> constexpr T *find(StaticList<T, capacity> &list, T const &value) { return find(as_span(list), value); }
template <class T, umm capacity> constexpr T *find(StaticList<T, capacity> &list, Span<T> cmp) { return find(as_span(list), cmp); }
template <class T, umm capacity> constexpr T const *find(StaticList<T, capacity> const &list, T const &value) { return find(as_span(list), value); }
template <class T, umm capacity> constexpr T const *find(StaticList<T, capacity> const &list, Span<T> cmp) { return find(as_span(list), cmp); }

template <class T, umm capacity, class Predicate> constexpr T *find_if(StaticList<T, capacity> &list, Predicate &&predicate) { return find_if(as_span(list), std::forward<Predicate>(predicate)); }

template <class T, umm capacity>
forceinline void erase(StaticList<T, capacity> &list, T *value) { list.erase(value); }

template <class Collection, class T>
T *find_previous(Collection collection, T value) {
	auto found = find(collection, value);
	if (found) return previous(collection, found);
	return 0;
}

template <class Collection, class T>
T *find_next(Collection collection, T value) {
	auto found = find(collection, value);
	if (found) return next(collection, found);
	return 0;
}

template <class Collection, class T>
bool find_and_erase(Collection &collection, T value) {
	auto found = find(collection, value);
	if (found) {
		erase(collection, found);
		return true;
	}
	return false;
}

using NativeWindowHandle = struct NativeWindow {} *;

enum AllocatorMode : u8 {
	Allocator_allocate,
	Allocator_reallocate,
	Allocator_free,
};

struct Allocator {
	void *(*func)(AllocatorMode mode, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *state) = 0;
	void *state = 0;
	forceinline operator bool() {
		return func != 0;
	}

	inline void *allocate_uninitialized(umm size, umm align = 1, std::source_location location = std::source_location::current()) {
		return func(Allocator_allocate, 0, 0, size, align, location, state);
	}
	inline void *allocate(umm size, umm align = 1, std::source_location location = std::source_location::current()) {
		auto result = allocate_uninitialized(size, align, location);
		if (result) {
			memset(result, 0, size);
		}
		return result;
	}

	template <class T>
	inline T *allocate_uninitialized(umm count = 1, umm align = alignof(T), std::source_location location = std::source_location::current()) {
		return (T *)func(Allocator_allocate, 0, 0, count * sizeof(T), align, location, state);
	}

	template <class T>
	inline T *allocate(umm count = 1, umm align = alignof(T), std::source_location location = std::source_location::current()) {
		T *result = allocate_uninitialized<T>(count, align, location);
		if (result) {
			for (auto it = result; it != result + count; ++it) {
				new (it) T();
			}
		}
		return result;
	}




	inline void *reallocate_uninitialized(void *data, umm old_size, umm new_size, umm align = 1, std::source_location location = std::source_location::current()) {
		return func(Allocator_reallocate, data, old_size, new_size, align, location, state);
	}
	inline void *reallocate(void *data, umm old_size, umm new_size, umm align = 1, std::source_location location = std::source_location::current()) {
		auto result = reallocate_uninitialized(data, old_size, new_size, align, location);
		if (result && (new_size > old_size)) {
			memset((u8 *)result + old_size, 0, new_size - old_size);
		}
		return result;
	}

	template <class T>
	inline T *reallocate_uninitialized(T *data, umm old_count, umm new_count, umm align = alignof(T), std::source_location location = std::source_location::current()) {
		return (T *)func(Allocator_reallocate, data, old_count * sizeof(T), new_count * sizeof(T), align, location, state);
	}

	template <class T>
	inline T *reallocate(T *data, umm old_count, umm new_count, umm align = alignof(T), std::source_location location = std::source_location::current()) {
		T *result = reallocate_uninitialized(data, old_count, new_count, align, location);
		for (auto it = result + old_count; it != result + new_count; ++it) {
			new (it) T();
		}
		return result;
	}


	void free(void *data, std::source_location location = std::source_location::current()) {
		func(Allocator_free, data, 0, 0, 0, location, state);
	}

};

#define tl_push(pusher, ...) if(auto CONCAT(_tl_, __LINE__)=pusher(__VA_ARGS__))
#define tl_scoped(current, new) auto CONCAT(_tl_,__LINE__)=current;current=(new);defer{current=CONCAT(_tl_,__LINE__);}

extern TL_API Allocator default_allocator;
extern TL_API thread_local Allocator temporary_allocator;
extern TL_API thread_local Allocator current_allocator;

extern TL_API void init_allocator(Allocator tempory_allocator_backup = default_allocator);
extern TL_API void deinit_allocator();
extern TL_API void clear_temporary_storage();


template <class T>
inline void allocate(T *&val) {
	val = current_allocator.allocate<T>();
}


}


// Specialize this function for custom current allocator
template <class T>
inline T get_current_allocator() {
	return {};
}

template <>
inline tl::Allocator get_current_allocator<tl::Allocator>() {
	return tl::current_allocator;
}


namespace tl {

#if TL_COUNT_ALLOCATIONS
extern umm frees_count;
extern umm allocations_count;
extern umm allocations_size;
#endif

struct AllocatorPusher {
	Allocator old_allocator;
	forceinline AllocatorPusher(Allocator new_allocator) {
		old_allocator = current_allocator;
		current_allocator = new_allocator;
	}
	forceinline ~AllocatorPusher() {
		current_allocator = old_allocator;
	}
	forceinline operator bool() { return true; }
};

#define push_allocator(allocator) tl_push(::tl::AllocatorPusher, allocator)
#define scoped_allocator(allocator) tl_scoped(::tl::current_allocator, allocator)

#define with(allocator, ...) ([&]()->decltype(auto){scoped_allocator(allocator);return __VA_ARGS__;}())

template <class T>
void rotate(Span<T> span, T *to_be_first) {
	umm left_count = to_be_first - span.data;
	umm right_count = span.count - left_count;

	if (right_count < left_count) {
		T *temp = temporary_allocator.allocate_uninitialized<T>(right_count);
		memcpy(temp, to_be_first, sizeof(T) * right_count);
		memmove(span.data + right_count, span.data, sizeof(T) * left_count);
		memcpy(span.data, temp, sizeof(T) * right_count);
	} else {
		T *temp = temporary_allocator.allocate_uninitialized<T>(left_count);
		memcpy(temp, span.data, sizeof(T) * left_count);
		memmove(span.data, span.data + left_count, sizeof(T) * right_count);
		memcpy(span.data + right_count, temp, sizeof(T) * left_count);
	}
}

template <class T>
void rotate(Span<T> span, smm to_be_first_index) {
	if (to_be_first_index < 0) {
		return rotate(span, span.end() + to_be_first_index);
	} else {
		return rotate(span, span.data + to_be_first_index);
	}
}

#ifdef TL_IMPL

#if OS_WINDOWS
#if COMPILER_MSVC
#define tl_allocate(size, align)         ::_aligned_malloc(size, align)
#define tl_reallocate(data, size, align) ::_aligned_realloc(data, size, align)
#define tl_free(data)                    ::_aligned_free(data)
#elif COMPILER_GCC
#define tl_allocate(size, align)         ::__mingw_aligned_malloc(size, align)
#define tl_reallocate(data, size, align) ::__mingw_aligned_realloc(data, size, align)
#define tl_free(data)                    ::__mingw_aligned_free(data)
#endif
#endif

#if TL_COUNT_ALLOCATIONS
umm frees_count = 0;
umm allocations_count = 0;
umm allocations_size = 0;
#endif

Allocator default_allocator = {
	[](AllocatorMode mode, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *) -> void * {
		switch (mode) {
			case Allocator_allocate: {
#if TL_COUNT_ALLOCATIONS
				++allocations_count;
				allocations_size += size;
#endif
				return tl_allocate(new_size, align);
			}
			case Allocator_reallocate: {
#if TL_COUNT_ALLOCATIONS
				++allocations_count;
				allocations_size += size;
#endif
				return tl_reallocate(data, new_size, align);
			}
			case Allocator_free:
#if TL_COUNT_ALLOCATIONS
				++frees_count;
#endif
				tl_free(data);
				return 0;
		}
		return 0;
	},
	0
};

struct TemporaryAllocatorState {
	struct Block {
		Block *next;
		umm size;
		umm capacity;
		forceinline u8 *data() { return (u8 *)(this + 1); }
	};
	Allocator allocator;
	Block *first = 0;
	Block *last = 0;
	umm last_block_capacity = 0x10000;
};

void free(TemporaryAllocatorState &state) {
	auto block = state.first;
	while (block) {
		auto next = block->next;
		state.allocator.free(block);
		block = next;
	}
	state = {};
}

thread_local TemporaryAllocatorState temporary_allocator_state;
thread_local Allocator temporary_allocator = {
	[](AllocatorMode mode, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *) -> void * {
		auto &state = temporary_allocator_state;
		switch (mode) {
			case tl::Allocator_allocate: {
				auto block = state.first;
				while (block) {
					auto candidate = (u8 *)ceil(block->data() + block->size, align);
					if (candidate + new_size <= block->data() + block->capacity) {
						block->size = (candidate - block->data()) + new_size;
						assert(block->size <= block->capacity);
						return candidate;
					}
					block = block->next;
				}

				// Block with enough space was not found. Create a new bigger one
				do {
					state.last_block_capacity *= 2;
				} while (state.last_block_capacity < new_size);

				block = (TemporaryAllocatorState::Block *)state.allocator.allocate_uninitialized(sizeof(TemporaryAllocatorState::Block) + state.last_block_capacity);
				block->size = new_size;
				block->capacity = state.last_block_capacity;
				block->next = 0;

				if (state.first) {
					state.last->next = block;
				} else {
					state.first = block;
				}
				state.last = block;

				return block->data();
			}
			case tl::Allocator_reallocate: {
				auto result = temporary_allocator.allocate_uninitialized(new_size);
				memcpy(result, data, old_size);
				return result;
			}
			case tl::Allocator_free:
				break;
		}
		return 0;
	},
	0
};

void clear_temporary_storage() {
	auto block = temporary_allocator_state.first;
	while (block) {
		block->size = 0;
		block = block->next;
	}
}

thread_local Allocator current_allocator;

void init_allocator(Allocator tempory_allocator_backup) {
	current_allocator = default_allocator;
	temporary_allocator_state.allocator = tempory_allocator_backup;
}

void deinit_allocator() {
	free(temporary_allocator_state);
}

#endif

} // namespace tl

#if COMPILER_MSVC
#pragma warning(pop)
#endif
