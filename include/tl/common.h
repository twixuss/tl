#pragma once
#include "system.h"

#if COMPILER_MSVC
	#pragma warning(push, 0)
#endif

#ifdef TL_IMPL
	#if OS_WINDOWS
		#define NOMINMAX
		#include <Windows.h>
	#else
		#include <pthread.h>
	#endif
#endif // TL_IMPL

#if COMPILER_MSVC
	#include <intrin.h>
	#include <vcruntime_new.h>
#elif COMPILER_GCC
	#include <malloc.h>
#endif

#include <utility>
#include <type_traits>
#include <tuple>

#if COMPILER_MSVC
	#pragma warning(pop)
#endif

#ifndef ASSERTION_FAILURE
#define ASSERTION_FAILURE(cause_string, expression, ...) debug_break()
#endif

#define assert_always(x, ...) (void)((bool)(x) || ((ASSERTION_FAILURE("assert", #x, __VA_ARGS__)), false))
#define assert(x, ...) assert_always(x, __VA_ARGS__)

#define invalid_code_path(...) (ASSERTION_FAILURE("invalid_code_path", "", __VA_ARGS__))

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

#if !TL_ENABLE_PROFILER
#define timed_block(...)
#define timed_function(...)
#endif

namespace TL {

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

#pragma warning(pop)

forceinline void add_carry(u8  a, u8  b, u8  *result, bool *carry) { *carry = _addcarry_u8 (0, a, b, result); }
forceinline void add_carry(u16 a, u16 b, u16 *result, bool *carry) { *carry = _addcarry_u16(0, a, b, result); }
forceinline void add_carry(u32 a, u32 b, u32 *result, bool *carry) { *carry = _addcarry_u32(0, a, b, result); }
#if ARCH_X64
forceinline void add_carry(u64 a, u64 b, u64 *result, bool *carry) { *carry = _addcarry_u64(0, a, b, result); }
#endif

forceinline constexpr bool is_nan(f32 v) {
	union {
		u32 u;
		f32 f;
	};
	f = v;
	return ((u & 0x7f800000) == 0x7f800000) && (u & 0x007fffff);
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

forceinline u32 count_bits(u32 v) { return (u32)_mm_popcnt_u32(v); }

#if ARCH_X64
forceinline u32 count_bits(u64 v) { return (u32)_mm_popcnt_u64(v); }
#else
forceinline u32 count_bits(u64 v) { return count_bits((u32)v) + count_bits((u32)(v >> 32)); }
#endif

forceinline u32 count_bits(s32 v) { return count_bits((u32)v); }
forceinline u32 count_bits(s64 v) { return count_bits((u64)v); }

forceinline bool is_power_of_2(u8  v) { return count_bits(v) == 1; }
forceinline bool is_power_of_2(u16 v) { return count_bits(v) == 1; }
forceinline bool is_power_of_2(u32 v) { return count_bits(v) == 1; }
forceinline bool is_power_of_2(u64 v) { return count_bits(v) == 1; }
namespace CE {
template <class T>
constexpr bool is_power_of_2(T v) { return (v != 0) && ((v & (v - 1)) == 0); }
}

// forceinline u8  floor_to_power_of_2(u8  v) { return 1 << find_highest_one_bit(v); }
// forceinline u16 floor_to_power_of_2(u16 v) { return 1 << find_highest_one_bit(v); }
forceinline u32 floor_to_power_of_2(u32 v) { return (u32)1 << find_highest_one_bit(v); }
#if ARCH_X64
forceinline u64 floor_to_power_of_2(u64 v) { return (u64)1 << find_highest_one_bit(v); }
#endif

forceinline u32 ceil_to_power_of_2(u32 v) { return is_power_of_2(v) ? v : (floor_to_power_of_2(v) + 1); }

template <class T> forceinline constexpr T select(bool mask, T a, T b) { return mask ? a : b; }
template <class T, class U> forceinline constexpr auto min(T a, U b) { return a < b ? a : b; }
template <class T, class U> forceinline constexpr auto max(T a, U b) { return a > b ? a : b; }
template <class T, class U, class... Rest> forceinline constexpr auto min(T a, U b, Rest... rest) { return min(min(a, b), rest...); }
template <class T, class U, class... Rest> forceinline constexpr auto max(T a, U b, Rest... rest) { return max(max(a, b), rest...); }
template <class T, class U, class V, class W> forceinline constexpr void minmax(T a, U b, V& mn, W& mx) { mn = min(a, b); mx = max(a, b); }
template <class T, class U> forceinline constexpr void minmax(T& mn, U &mx) { minmax(mn, mx, mn, mx); }

template <class T, umm size>
forceinline constexpr auto min(T (&array)[size]) {
	T result = array[0];
	for (umm i = 1; i < size; ++i) {
		result = min(result, array[i]);
	}
	return result;
}

template <class T, umm size>
forceinline constexpr auto max(T (&array)[size]) {
	T result = array[0];
	for (umm i = 1; i < size; ++i) {
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

forceinline constexpr s32 pow(s32 base, u32 exp) {
	s32 res = 1;
	while (exp) {
		if (exp & 1)
			res *= base;
		exp >>= 1;
		base *= base;
	}
	return res;
}

namespace CE {

// https://stackoverflow.com/a/24748637
#define S(k) if (n >= ((decltype(n))1 << k)) { i += k; n >>= k; }
forceinline constexpr s32 log2(u8 n)  { s32 i = -(n == 0); S(4); S(2); S(1); return i; }
forceinline constexpr s32 log2(u16 n) { s32 i = -(n == 0); S(8); S(4); S(2); S(1); return i; }
forceinline constexpr s32 log2(u32 n) { s32 i = -(n == 0); S(16); S(8); S(4); S(2); S(1); return i; }
forceinline constexpr s32 log2(u64 n) { s32 i = -(n == 0); S(32); S(16); S(8); S(4); S(2); S(1); return i; }
#undef S

}

#if COMPILER_MSVC
#if ARCH_LZCNT
forceinline u32 count_leading_zeros(u8  val) { return __lzcnt16(val) - 8; }
forceinline u32 count_leading_zeros(u16 val) { return __lzcnt16(val); }
forceinline u32 count_leading_zeros(u32 val) { return __lzcnt32(val); }
forceinline u32 count_leading_zeros(u64 val) { return (u32)__lzcnt64(val); }
#else
forceinline u32 count_leading_zeros(u8  val) { ulong r; return _BitScanReverse  (&r, val) ?  7 - r :  8; }
forceinline u32 count_leading_zeros(u16 val) { ulong r; return _BitScanReverse  (&r, val) ? 15 - r : 16; }
forceinline u32 count_leading_zeros(u32 val) { ulong r; return _BitScanReverse  (&r, val) ? 31 - r : 32; }
#if ARCH_X64
forceinline u32 count_leading_zeros(u64 val) { ulong r; return _BitScanReverse64(&r, val) ? 63 - r : 64; }
#endif
#endif
#endif

forceinline u32 count_leading_zeros(s8  val) { return count_leading_zeros((u8 )val); }
forceinline u32 count_leading_zeros(s16 val) { return count_leading_zeros((u16)val); }
forceinline u32 count_leading_zeros(s32 val) { return count_leading_zeros((u32)val); }
#if ARCH_X64
forceinline u32 count_leading_zeros(s64 val) { return count_leading_zeros((u64)val); }
#endif

forceinline u32 count_leading_ones(u8  val) { return count_leading_zeros((u8 )~val); }
forceinline u32 count_leading_ones(u16 val) { return count_leading_zeros((u16)~val); }
forceinline u32 count_leading_ones(u32 val) { return count_leading_zeros((u32)~val); }
#if ARCH_X64
forceinline u32 count_leading_ones(u64 val) { return count_leading_zeros((u64)~val); }
#endif

forceinline u32 log2(u8  n) { return  7u - count_leading_zeros(n); }
forceinline u32 log2(u16 n) { return 15u - count_leading_zeros(n); }
forceinline u32 log2(u32 n) { return 31u - count_leading_zeros(n); }
#if ARCH_X64
forceinline u32 log2(u64 n) { return 63u - count_leading_zeros(n); }
#endif

forceinline u32 log2(s8  n) { return  7u - count_leading_zeros(n); }
forceinline u32 log2(s16 n) { return 15u - count_leading_zeros(n); }
forceinline u32 log2(s32 n) { return 31u - count_leading_zeros(n); }
#if ARCH_X64
forceinline u32 log2(s64 n) { return 63u - count_leading_zeros(n); }
#endif

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

forceinline f32 ceil(f32 v) { return ::ceilf(v); }
forceinline f64 ceil(f64 v) { return ::ceil(v); }
forceinline s32 ceil_to_int(f32 v) { return (s32)ceil(v); }
forceinline s64 ceil_to_int(f64 v) { return (s64)ceil(v); }

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

template <class T, umm size>
constexpr umm count_of(T const (&arr)[size]) { (void)arr; return size; }

template <class T, umm size>
constexpr T &back(T (&arr)[size]) { return arr[size - 1]; }
template <class T, umm size>
constexpr T const &back(T const (&arr)[size]) { return arr[size - 1]; }

template <class T, umm size>
constexpr T &front(T (&arr)[size]) { return arr[0]; }
template <class T, umm size>
constexpr T const &front(T const (&arr)[size]) { return arr[0]; }

inline void copyMemory(void *_dst, void const *_src, umm size) {
	u8 *dst = (u8 *)_dst;
	u8 const *src = (u8 const *)_src;
	while (size--) {
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
		if (*it == value) {
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
	umm src_size = (umm)(src_end - src_begin);
	umm cmp_size = (umm)(cmp_end - cmp_begin);
	for (umm i = 0; i < src_size - cmp_size + 1; ++i) {
		for (umm j = 0; j < cmp_size; ++j) {
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

#define defer ::TL::Deferrer CONCAT(_deferrer, __LINE__) = [&]

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

template <class T>
struct Span {
	using ValueType = T;
	constexpr Span(std::initializer_list<ValueType> list) : data((ValueType *)list.begin()), size(list.size()) {}
	constexpr Span() = default;
	constexpr Span(ValueType &value) : data(std::addressof(value)), size(1) {}
	template <umm count>
	constexpr Span(ValueType (&array)[count]) : data(array), size(count) {}
	constexpr Span(ValueType *begin, ValueType *end) : data(begin), size(end - begin) {}
	constexpr Span(ValueType const *begin, ValueType const *end) : data((ValueType *)begin), size(end - begin) {}
	constexpr Span(ValueType *begin, umm size) : data(begin), size(size) {}
	constexpr ValueType *begin() const { return data; }
	constexpr ValueType *end() const { return data + size; }
	constexpr ValueType &front() const {
		bounds_check(size);
		return *data;
	}
	constexpr ValueType &back() const {
		bounds_check(size);
		return data[size - 1];
	}
	constexpr ValueType &operator[](umm i) const {
		bounds_check(i < size);
		return data[i];
	}
	constexpr ValueType &at(umm i) const {
		bounds_check(i < size);
		return data[i];
	}
	constexpr bool empty() const { return size == 0; }

	template <class U>
	constexpr explicit operator Span<U>() const {
		static_assert(sizeof(U) == sizeof(T));
		return {(U *)data, size};
	}

	//constexpr operator Span<utf8>() const { // Really this cast should be defined only in Span<char>, but this is impossible to do in this language without copypasta
	//	static_assert(is_same<T, ascii>);
	//	return {(utf8 *)data, size * sizeof(ValueType)};
	//}

	constexpr bool operator==(Span<ValueType> that) const {
		if (size != that.size)
			return false;
		for (umm i = 0; i < size; ++i) {
			if (data[i] != that.data[i])
				return false;
		}
		return true;
	}
	constexpr bool operator!=(Span<ValueType> that) const { return !(*this == that); }

	ValueType *data = 0;
	umm size = 0;
};

forceinline constexpr Span<char > operator""s(char  const *string, umm size) { return Span((char  *)string, size); }
forceinline constexpr Span<utf8 > operator""s(utf8  const *string, umm size) { return Span((utf8  *)string, size); }
forceinline constexpr Span<utf16> operator""s(utf16 const *string, umm size) { return Span((utf16 *)string, size); }
forceinline constexpr Span<wchar> operator""s(wchar const *string, umm size) { return Span((wchar *)string, size); }
forceinline Span<u8> operator""b(char const *string, umm size) { return Span((u8 *)string, size); }

template <class T, umm size>
inline constexpr Span<T> array_as_span(T const (&arr)[size]) { return Span((T *)arr, size); }

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
constexpr Span<u8> as_bytes(T span_like) {
	return as_bytes(as_span(span_like));
}

template <class T>
constexpr Span<u8> as_bytes(Span<T> span) {
	return {(u8 *)span.begin(), span.size * sizeof(T)};
}

template <class T>
constexpr Span<char> as_chars(Span<T> span) {
	return {(char *)span.begin(), span.size * sizeof(T)};
}

template <class T>
constexpr Span<u8> value_as_bytes(T &value) {
	return {(u8 *)&value, sizeof(T)};
}

template <class T> constexpr umm count_of(Span<T> span) { return span.size; }

template <class T>
constexpr void replace(Span<T> destination, Span<T> source, umm start_index = 0) {
	for (umm i = 0; i < source.size; ++i) {
		destination[start_index + i] = source[i];
	}
}

template <class T, umm size> constexpr T *find(T (&arr)[size], T const &value) { return find(arr, arr + size, value); }
template <class T> constexpr T *find(Span<T> span, T const &value) { return find(span.begin(), span.end(), value); }
template <class T> constexpr T *find(Span<T> span, Span<T> cmp) { return find(span.begin(), span.end(), cmp.begin(), cmp.end()); }

template <class T>
constexpr T *find_last(Span<T> span, T const &value) {
	for (auto it = span.end() - 1; it >= span.begin(); --it) {
		if (*it == value)
			return it;
	}
	return 0;
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

inline constexpr bool is_whitespace(u32 c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

inline constexpr bool is_alpha(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline constexpr bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

inline constexpr char to_lower_ascii(char c) {
	if (c >= 'A' && c <= 'Z')
		return (char)(c + ('a' - 'A'));
	return c;
}
inline constexpr u32 to_lower_utf8(u32 c) {
	if (c >= 'A' && c <= 'Z')
		return c + ('a' - 'A');
	return c;
}
inline constexpr wchar to_lower_utf16(wchar c) {
	if (c >= L'A' && c <= L'Z')
		return (wchar)(c + (L'a' - L'A'));
	return c;
}

forceinline constexpr ascii to_lower(ascii c) { return to_lower_ascii(c); }
forceinline constexpr utf8  to_lower(utf8  c) { return to_lower_utf8(c); }
forceinline constexpr utf16 to_lower(utf16 c) { return to_lower_utf16(c); }
forceinline constexpr wchar to_lower(wchar c) { return to_lower((wchar_s)c); }

inline constexpr char to_upper_ascii(char c) {
	if (c >= 'a' && c <= 'z')
		return (char)(c - ('a' - 'A'));
	return c;
}
inline constexpr u32 to_upper_utf8(u32 c) {
	if (c >= 'a' && c <= 'z')
		return c - ('a' - 'A');
	return c;
}
inline constexpr wchar to_upper_utf16(wchar c) {
	if (c >= L'a' && c <= L'z')
		return (wchar)(c - (L'a' - L'A'));
	return c;
}

template <class T, class U>
inline constexpr bool equals(Span<T> a, Span<U> b) { \
	if (a.size != b.size)                                    \
		return false;                                        \
	auto ap = a.begin();                                     \
	for (auto bp = b.begin(); ap != a.end(); ++ap, ++bp)     \
		if (*ap != *bp)                                      \
			return false;                                    \
	return true;                                             \
}

template <class T, class U>
inline constexpr bool starts_with(Span<T> str, Span<U> sub_str) {
	if (sub_str.size > str.size)
		return false;
	for (umm i = 0; i < sub_str.size; ++i) {
		if (str.data[i] != sub_str.data[i]) {
			return false;
		}
	}
	return true;
}

template <class T, class U>
inline constexpr bool ends_with(Span<T> str, Span<U> sub_str) {
	if (sub_str.size > str.size)
		return false;
	umm base_offset = str.size - sub_str.size;
	for (umm i = 0; i < sub_str.size; ++i) {
		if (str.data[i + base_offset] != sub_str.data[i]) {
			return false;
		}
	}
	return true;
}

inline bool equals_case_insensitive(char a, char b) { return to_lower(a) == to_lower(b); }
inline bool equals_case_insensitive(utf8 a, utf8 b) { return to_lower(a) == to_lower(b); }

template <class T, class Predicate, class = EnableIf<is_invocable<Predicate, T, T>>>
inline constexpr bool ends_with(Span<T> str, Span<T> sub_str, Predicate &&predicate) {
	if (sub_str.size > str.size)
		return false;
	umm base_offset = str.size - sub_str.size;
	for (umm i = 0; i < sub_str.size; ++i) {
		if (!predicate(str.data[i + base_offset], sub_str.data[i])) {
			return false;
		}
	}
	return true;
}

inline constexpr void set_to_lower_case(Span<char> span) {
	for (auto &c : span) {
		c = to_lower_ascii(c);
	}
}
inline constexpr void set_to_lower_case(Span<wchar> span) {
	for (auto &c : span) {
		c = to_lower_utf16(c);
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
	if (span.size == 0) {
		return span;
	}
	while (is_any_of(span.front(), chars_to_skip)) {
		++span.data;
		--span.size;
		if (span.size == 0) {
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
		size = that.size;
		memcpy(data, that.data, size * sizeof(T));
	}

	template <umm that_capacity>
	constexpr StaticList(StaticList<T, that_capacity> &&that) = delete;

	constexpr StaticList(std::initializer_list<T> that) {
		size = that.size();
		memcpy(data, that.begin(), size * sizeof(T));
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
	forceinline constexpr T const *end() const { return data + size; }

	forceinline constexpr T *begin() { return data; }
	forceinline constexpr T *end() { return data + size; }

	forceinline constexpr bool empty() const { return size == 0; }
	forceinline constexpr bool full() const { return size == capacity; }

	forceinline constexpr T &front() { bounds_check(size); return data[0]; }
	forceinline constexpr T &back() { bounds_check(size); return data[size - 1]; }
	forceinline constexpr T &operator[](umm i) { bounds_check(size); return data[i]; }

	forceinline constexpr T const &front() const { bounds_check(size); return data[0]; }
	forceinline constexpr T const &back() const { bounds_check(size); return data[size - 1]; }
	forceinline constexpr T const &operator[](umm i) const { bounds_check(size); return data[i]; }

	constexpr void resize(umm new_size) {
		bounds_check(new_size <= capacity);
		if (new_size > size) {
			for (umm i = size; i < new_size; ++i) {
				new (data + i) T();
			}
		}
		size = new_size;
	}
	constexpr T &insert_at(T value, umm where) {
		bounds_check(where <= size);
		bounds_check(size < capacity);

		memmove(data + where + 1, data + where, (size - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++size;
		return data[where];
	}
	constexpr Span<T> insert_at(Span<T> span, umm where) {
		bounds_check(where <= size);
		bounds_check(size + span.size <= capacity);

		memmove(data + where + span.size, data + where, (size - where) * sizeof(T));
		memcpy(data + where, span.data, span.size * sizeof(T));

		size += span.size;
		return {data + where, span.size};
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

	forceinline constexpr operator Span<T>() { return {data, size}; }

	forceinline constexpr T &add() {
		bounds_check(!full());
		return *new(data + size++) T();
	}

	forceinline constexpr T &add(T const &value) {
		bounds_check(!full());
		memcpy(data + size, &value, sizeof(T));
		return data[size++];
	}

	forceinline constexpr Span<T> add(Span<T> span) {
		bounds_check(size + span.size <= capacity);
		memcpy(data + size, span.data, span.size * sizeof(T));
		defer { size += span.size; };
		return {data + size, span.size};
	}

	constexpr T pop_back() {
		bounds_check(size);
		return data[--size];
	}
	constexpr T pop_front() {
		bounds_check(size);
		T popped = *data;
		memmove(data, data + 1, --size * sizeof(T));
		return popped;
	}
	forceinline constexpr void pop_back_unordered() { erase_unordered(&back()); }
	forceinline constexpr void pop_front_unordered() { erase_unordered(begin()); }

	T erase_at(umm where) {
		bounds_check(where < size);
		T erased = data[where];
		memmove(data + where, data + where + 1, (--size - where) * sizeof(T));
		return erased;
	}
	forceinline T erase(T *where) { return erase_at(where - data); }
	forceinline T erase(T &value) { return erase(&value); }

	T erase_at_unordered(umm where) {
		bounds_check(where < size);
		T erased = data[where];
		--size;
		if (size != where) {
			data[where] = data[size];
		}
		return erased;
	}
	forceinline T erase_unordered(T *where) { return erase_at_unordered(where - data); }
	forceinline T erase_unordered(T &where) { return erase_unordered(&where); }

	constexpr void clear() {
		size = 0;
	}

	umm size = 0;
	union {
		T data[capacity];
	};
};

template <class T, umm capacity>
umm index_of(StaticList<T, capacity> const &list, T const *value) {
	return value - list.data;
}

template <class T, umm capacity> Span<T> as_span(StaticList<T, capacity> const &list) { return {(T *)list.data, list.size}; }

template <class T, umm capacity> constexpr T *find(StaticList<T, capacity> &list, T const &value) { return find(as_span(list), value); }
template <class T, umm capacity> constexpr T *find(StaticList<T, capacity> &list, Span<T> cmp) { return find(as_span(list), cmp); }
template <class T, umm capacity> constexpr T const *find(StaticList<T, capacity> const &list, T const &value) { return find(as_span(list), value); }
template <class T, umm capacity> constexpr T const *find(StaticList<T, capacity> const &list, Span<T> cmp) { return find(as_span(list), cmp); }

template <class T, umm capacity, class Predicate> constexpr T *find_if(StaticList<T, capacity> &list, Predicate &&predicate) { return find_if(as_span(list), std::forward<Predicate>(predicate)); }

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

struct SourceLocation {
	char const *file;
	char const *function;
	umm line;
};

#define get_source_location() SourceLocation{__FILE__, __FUNCTION__, __LINE__}

enum AllocatorMode {
	Allocator_allocate,
	Allocator_reallocate,
	Allocator_free,
};

#if TL_TRACK_ALLOCATIONS
using AllocatorSourceLocation = SourceLocation;
#else
struct AllocatorSourceLocation {};
#endif

struct Allocator {
	void *(*func)(AllocatorMode mode, umm size, umm align, void *data, AllocatorSourceLocation location, void *state) = 0;
	void *state = 0;
	forceinline operator bool() {
		return func != 0;
	}
	forceinline void *operator()(AllocatorMode mode, umm size, umm align, void *data, AllocatorSourceLocation location) {
		return func(mode, size, align, data, location, state);
	}
};

template <class T>
T *_allocate(Allocator allocator, AllocatorSourceLocation location, umm count = 1, umm align = alignof(T)) {
	auto data = (T *)allocator(Allocator_allocate, sizeof(T) * count, align, 0, location);
	for (umm i = 0; i < count; ++i) {
		new (data + i) T();
	}
	return data;
}
template <class T>
T *_allocate_noinit(Allocator allocator, AllocatorSourceLocation location, umm count = 1, umm align = alignof(T)) {
	return (T *)allocator(Allocator_allocate, sizeof(T) * count, align, 0, location);
}
inline void *_reallocate(Allocator allocator, void *data) {
	return allocator(Allocator_reallocate, 0, 0, data, {});
}
inline void _free(Allocator allocator, void *data) {
	allocator(Allocator_free, 0, 0, data, {});
}

#if TL_TRACK_ALLOCATIONS
#define ALLOCATE(t, allocator, ...) ::TL::_allocate<t>(allocator, get_source_location(), __VA_ARGS__)
#define ALLOCATE_NOINIT(t, allocator, ...) ::TL::_allocate_noinit<t>(allocator, get_source_location(), __VA_ARGS__)
#else
#define ALLOCATE(t, allocator, ...) ::TL::_allocate<t>(allocator, AllocatorSourceLocation {}, __VA_ARGS__)
#define ALLOCATE_NOINIT(t, allocator, ...) ::TL::_allocate_noinit<t>(allocator, AllocatorSourceLocation {}, __VA_ARGS__)
#endif

#define FREE(allocator, data) ::TL::_free(allocator, data)



#define tl_push(pusher, ...) if(auto CONCAT(_, __LINE__)=pusher(__VA_ARGS__))
#define tl_scoped(current, new) auto CONCAT(_,__LINE__)=current;current=(new);defer{current=CONCAT(_,__LINE__);}

extern TL_API void init_allocator();
extern TL_API void deinit_allocator();
extern TL_API void clear_temporary_storage();
extern TL_API Allocator default_allocator;
extern TL_API thread_local Allocator temporary_allocator;
extern TL_API thread_local Allocator current_allocator;
#if TL_TRACK_ALLOCATIONS
extern TL_API Allocator tracking_allocator;
#endif

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

#define push_allocator(allocator) tl_push(::TL::AllocatorPusher, allocator)
#define scoped_allocator(allocator) tl_scoped(::TL::current_allocator, allocator)

#define with(allocator, ...) ([&]{scoped_allocator(allocator);return __VA_ARGS__;}())

template <class T>
void rotate(Span<T> span, T *to_be_first) {
	umm left_count = to_be_first - span.data;
	umm right_count = span.size - left_count;

	if (right_count < left_count) {
		T *temp = ALLOCATE(T, temporary_allocator, right_count);
		memcpy(temp, to_be_first, sizeof(T) * right_count);
		memmove(span.data + right_count, span.data, sizeof(T) * left_count);
		memcpy(span.data, temp, sizeof(T) * right_count);
	} else {
		T *temp = ALLOCATE(T, temporary_allocator, left_count);
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
	[](AllocatorMode mode, umm size, umm align, void *data, AllocatorSourceLocation location, void *) -> void * {
		switch (mode) {
			case Allocator_allocate:
#if TL_COUNT_ALLOCATIONS
				++allocations_count;
				allocations_size += size;
#endif
				return tl_allocate(size, align);
			case Allocator_reallocate:
#if TL_COUNT_ALLOCATIONS
				++allocations_count;
				allocations_size += size;
#endif
				return tl_reallocate(data, size, align);
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

#ifndef TL_TEMP_STORAGE_LIMIT
#define TL_TEMP_STORAGE_LIMIT 0x10000000
#endif

struct TemporaryAllocatorState {
	struct Block {
		Block *next;
		umm size;
		umm capacity;
		forceinline u8 *data() { return (u8 *)(this + 1); }
	};
	Block *first = 0;
	Block *last = 0;
	umm last_block_capacity = 0x10000;
	umm total_block_capacity = 0;
};

void free(TemporaryAllocatorState &state) {
	auto block = state.first;
	while (block) {
		auto next = block->next;
		FREE(default_allocator, block);
		block = next;
	}
	state = {};
}

thread_local TemporaryAllocatorState temporary_allocator_state;
thread_local Allocator temporary_allocator = {
	[](AllocatorMode mode, umm size, umm align, void *data, AllocatorSourceLocation location, void *) -> void * {
		auto &state = temporary_allocator_state;
		switch (mode) {
			case TL::Allocator_allocate: {
				auto block = state.first;
				while (block) {
					auto candidate = (u8 *)ceil(block->data() + block->size, align);
					if (candidate + size <= block->data() + block->capacity) {
						block->size = (candidate - block->data()) + size;
						assert(block->size <= block->capacity);
						return candidate;
					}
					block = block->next;
				}

				// Block with enough space was not found. Create a new bigger one
				while (state.last_block_capacity < size) {
					state.last_block_capacity *= 2;
				}

				block = (TemporaryAllocatorState::Block *)ALLOCATE(u8, default_allocator, sizeof(TemporaryAllocatorState::Block) + state.last_block_capacity);
				block->size = size;
				block->capacity = state.last_block_capacity;
				block->next = 0;

				if (state.first) {
					state.last->next = block;
				} else {
					state.first = block;
				}
				state.last = block;

				state.total_block_capacity += block->capacity;

#if TL_TEMP_STORAGE_LIMIT != 0
				bounds_check(state.total_block_capacity < TL_TEMP_STORAGE_LIMIT,
					"Capacity of temporary storage has exceeded the limit of % bytes. "
					"You can either clear the storage by calling clear_temporary_storage (Make sure you don't reference previously allocated memory). "
					"Or you can #define TL_TEMP_STORAGE_LIMIT to some bigger number.", TL_TEMP_STORAGE_LIMIT);
#endif

				return block->data();
			}
			case TL::Allocator_reallocate:
				invalid_code_path();
				break;
			case TL::Allocator_free:
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

void init_allocator() {
#if TL_TRACK_ALLOCATIONS
	current_allocator = tracking_allocator;
#else
	current_allocator = default_allocator;
#endif
}

void deinit_allocator() {
	free(temporary_allocator_state);
}

#endif

} // namespace TL

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef TL_IMPL

#if TL_TRACK_ALLOCATIONS
#include "thread.h"
#include "debug.h"
#include <unordered_map>

namespace TL {

struct AllocationInfo {
	umm size;
	AllocatorSourceLocation location;
	CallStack call_stack;
};
std::unordered_map<void *, AllocationInfo> allocations;
Mutex allocations_mutex;
forceinline void track_allocation(void *pointer, umm size, AllocatorSourceLocation location) {
	scoped_allocator(default_allocator);

	scoped_lock(allocations_mutex);
	AllocationInfo a;
	a.size = size;
	a.location = location;
	a.call_stack = get_call_stack();
	allocations[pointer] = a;
}
forceinline void untrack_allocation(void *pointer) {
	scoped_lock(allocations_mutex);
	allocations.erase(pointer);
}
forceinline void retrack_allocation(void *old_pointer, void *new_pointer, umm size, AllocatorSourceLocation location) {
	scoped_allocator(default_allocator);

	scoped_lock(allocations_mutex);
	AllocationInfo a;
	a.size = size;
	a.location = location;
	a.call_stack = get_call_stack();
	allocations[new_pointer] = a;
	allocations.erase(old_pointer);
}
Allocator tracking_allocator = {
	[](AllocatorMode mode, umm size, umm align, void *data, AllocatorSourceLocation location, void *) -> void * {
		switch (mode) {
			case Allocator_allocate: {
				auto result = tl_allocate(size, align);
				track_allocation(result, size, location);
				return result;
			}
			case Allocator_reallocate: {
				auto result = tl_reallocate(data, size, align);
				retrack_allocation(data, result, size, location);
				return result;
			}
			case Allocator_free: {
				tl_free(data);
				untrack_allocation(data);
				break;
			}
		}
		return 0;
	},
	0
};

}
#endif

#undef tl_allocate
#undef tl_reallocate
#undef tl_free

#endif

#ifdef TL_MAIN
#ifdef TL_IMPL
#include "string.h"
extern TL::s32 tl_main(TL::Span<TL::Span<TL::utf8>> args);
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR command_line_16, int) {
	using namespace TL;
	init_allocator();
	defer { deinit_allocator(); };

	auto command_line = utf16_to_utf8(as_span((utf16 *)command_line_16));
	List<Span<utf8>> arguments;

	utf8 *c = command_line.data;
	while (c != command_line.end()) {
		while (is_whitespace(*c)) {
			++c;
			if (c == command_line.end())
				goto main;
		}

		if (*c == '"') {
			++c;
			auto begin = c;
			while (c != command_line.end()) {
				++c;
				if (*c == '"')
					break;
			}
			arguments.add(Span(begin, c));
			continue;
		}

		auto begin = c;
		while (c != command_line.end()) {
			++c;
			if (is_whitespace(*c))
				break;
		}
		arguments.add(Span(begin, c));
	}
main:
	return tl_main(arguments);
}
#endif
#endif

