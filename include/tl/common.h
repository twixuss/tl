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
#define ASSERTION_FAILURE(cause_string, expression_string, ...) debug_break()
#endif

#define assert_always(x, ...) (void)((x) || ((ASSERTION_FAILURE("assert", #x, __VA_ARGS__)), false))

#ifndef assert
#define assert(x, ...) assert_always(x, __VA_ARGS__)
#endif

#define invalid_code_path(...) (ASSERTION_FAILURE("invalid_code_path", "", __VA_ARGS__), __assume(0))
#define not_implemented(...) (ASSERTION_FAILURE("not_implemented", "", __VA_ARGS__), __assume(0))

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



// These are macros to propagate caller's location down the call stack.
// The reason they exist is because I wanted to track allocations,
// specifically the first non-tl entry in the call stack. I tried to
// walk the stack on every allocation, but it turned way way slower and
// excessively complicated than this method. The only downside of this
// is noisy parameter and argument lists.
// Comment after macro tells where to put it.
// For examples just search for any data structure that does allocations.

#ifndef TL_PARENT_SOURCE_LOCATION
#define TL_PARENT_SOURCE_LOCATION TL_DEBUG
#endif

#if TL_PARENT_SOURCE_LOCATION
#define TL_LPC std::source_location location = std::source_location::current() // in empty parameter list (declaration)
#define TL_LPCD std::source_location location                                  // in empty parameter list (definition)
#define TL_LAC location														   // in empty argument list
#define TL_LP , TL_LPC														   // after some parameters (declaration)
#define TL_LPD , TL_LPCD													   // after some parameters (definition)
#define TL_LA , TL_LAC														   // after some arguments
#else
#define TL_LPC
#define TL_LPCD
#define TL_LAC
#define TL_LP
#define TL_LPD
#define TL_LA
#endif



#define KiB 0x400ull
#define MiB 0x100000ull
#define GiB 0x40000000ull
#define TiB 0x10000000000ull

#define breakable_scope for(bool CONCAT(_, __LINE__)=true;CONCAT(_, __LINE__);CONCAT(_, __LINE__)=false)

#define REDECLARE_VAL(name, expr) auto _##name = expr; auto name = _##name;
#define REDECLARE_REF(name, expr) auto &_##name = expr; auto &name = _##name;

// static_assert(false) is ill-formed in removed branch of constexpr if...
#define static_error_t(t, ...) static_assert(!sizeof(t*), __VA_ARGS__)
#define static_error_v(v, ...) static_error_t(decltype(v), __VA_ARGS__)

#define TL_DECLARE_CONCEPT(name) \
template <class T> struct S##name : std::false_type {}; \
template <class ...Args> struct S##name<name<Args...>> : std::true_type {}; \
template <class T> concept C##name = S##name<T>::value

namespace tl {

inline constexpr umm string_char_count(ascii const *str) { umm result = 0; while (*str++) ++result; return result; }

inline constexpr umm string_unit_count(ascii const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_unit_count(utf8  const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_unit_count(utf16 const *str) { umm result = 0; while (*str++) ++result; return result;}
inline constexpr umm string_unit_count(utf32 const *str) { umm result = 0; while (*str++) ++result; return result;}
inline constexpr umm string_unit_count(wchar const *str) { umm result = 0; while (*str++) ++result; return result;}

inline constexpr umm string_byte_count(ascii const *str) { return string_unit_count(str) * sizeof(ascii); }
inline constexpr umm string_byte_count(utf8  const *str) { return string_unit_count(str) * sizeof(utf8 ); }
inline constexpr umm string_byte_count(utf16 const *str) { return string_unit_count(str) * sizeof(utf16);}
inline constexpr umm string_byte_count(utf32 const *str) { return string_unit_count(str) * sizeof(utf32);}
inline constexpr umm string_byte_count(wchar const *str) { return string_unit_count(str) * sizeof(wchar);}

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

template <class ToFind, class First, class ...Rest>
inline constexpr u32 type_index(u32 start) {
	if constexpr (std::is_same_v<ToFind, First>) {
		return start;
	} else {
		return type_index<ToFind, Rest...>(start + 1);
	}
}

template <u32 index, class ...Rest>
struct TypeAtT;

template <class First, class ...Rest>
struct TypeAtT<0, First, Rest...> { using Type = First; };

template <u32 index, class First, class ...Rest>
struct TypeAtT<index, First, Rest...> { using Type = typename TypeAtT<index-1, Rest...>::Type; };

template <u32 index, class ...Rest>
using TypeAt = typename TypeAtT<index, Rest...>::Type;

template <class First, class ...Rest>
concept AllSame = (std::is_same_v<First, Rest> && ...);

template <class T, class ...Types>
concept OneOf = (std::is_same_v<T, Types> || ...);

template <class T, class ...Args>
constexpr T &construct(T &val, Args &&...args) {
	return *new(std::addressof(val)) T(std::forward<Args>(args)...);
}
template <class T>
constexpr void destruct(T &val) {
	val.~T();
}

#pragma warning(push)
#pragma warning(disable : 4309)

template <class T> inline constexpr T min_value = {};
template <class T> inline constexpr T max_value = {};

template <class T> inline constexpr T min_value<T &> = min_value<T>;
template <class T> inline constexpr T max_value<T &> = max_value<T>;

template<> inline constexpr u8  min_value<u8 > = 0;
template<> inline constexpr u8  max_value<u8 > = 0xFF;

template<> inline constexpr u16 min_value<u16> = 0;
template<> inline constexpr u16 max_value<u16> = 0xFFFF;

template<> inline constexpr u32 min_value<u32> = 0;
template<> inline constexpr u32 max_value<u32> = 0xFFFFFFFF;

template<> inline constexpr u64 min_value<u64> = 0;
template<> inline constexpr u64 max_value<u64> = 0xFFFFFFFFFFFFFFFF;

template<> inline constexpr s8  min_value<s8 > = 0x80;
template<> inline constexpr s8  max_value<s8 > = 0x7F;

template<> inline constexpr s16 min_value<s16> = 0x8000;
template<> inline constexpr s16 max_value<s16> = 0x7FFF;

template<> inline constexpr s32 min_value<s32> = 0x80000000;
template<> inline constexpr s32 max_value<s32> = 0x7FFFFFFF;

template<> inline constexpr s64 min_value<s64> = 0x8000000000000000;
template<> inline constexpr s64 max_value<s64> = 0x7FFFFFFFFFFFFFFF;

template<> inline constexpr ulong min_value<ulong> = (ulong)min_value<ulong_s>;
template<> inline constexpr ulong max_value<ulong> = (ulong)max_value<ulong_s>;

template<> inline constexpr slong min_value<slong> = (slong)min_value<slong_s>;
template<> inline constexpr slong max_value<slong> = (slong)max_value<slong_s>;

template<> inline constexpr f32 min_value<f32> = -3.402823466e+38f;
template<> inline constexpr f32 max_value<f32> = +3.402823466e+38f;

template<> inline constexpr f64 min_value<f64> = -1.7976931348623158e+308;
template<> inline constexpr f64 max_value<f64> = +1.7976931348623158e+308;

template <class T> inline constexpr T epsilon = {};
template<> inline constexpr f32 epsilon<f32> = 1.175494351e-38f;
template<> inline constexpr f64 epsilon<f64> = 2.2250738585072014e-308;

template <class T> inline constexpr T infinity = {};
template<> inline constexpr f32 infinity<f32> = 1e300 * 1e300;
template<> inline constexpr f64 infinity<f64> = 1e300 * 1e300;

template <class T> inline constexpr T nan = infinity<T> * 0;

#pragma warning(pop)

#define tl_enum_flags(t) \
forceinline constexpr t operator~(t a){using u=std::underlying_type_t<t>;return (t)~(u)a;} \
forceinline constexpr t operator|(t a,t b){using u=std::underlying_type_t<t>;return (t)((u)a|(u)b);} \
forceinline constexpr t operator&(t a,t b){using u=std::underlying_type_t<t>;return (t)((u)a&(u)b);} \

template <class T>
concept AnEnum = std::is_enum_v<T>;

forceinline constexpr auto to_underlying(AnEnum auto e) { return (std::underlying_type_t<decltype(e)>)e; }

template <AnEnum Enum>
requires requires { { Enum::count }; }
forceinline constexpr auto enum_values() {
	struct EnumValues {
		using Underlying = std::underlying_type_t<Enum>;
		struct Iterator {
			Underlying value;
			forceinline constexpr Enum operator*() const { return (Enum)value; }
			forceinline constexpr Iterator &operator++() { ++value; }
			forceinline constexpr bool operator==(Iterator const &that) { return value == that.value; }
		};

		forceinline constexpr Iterator begin() { return {}; }
		forceinline constexpr Iterator end() { return {(Underlying)Enum::count}; }
	};

	return EnumValues{};
}



// This function exists because C++ does not provide a way to convert
// fundamental types to structs without adding a constructor to a struct.
// If there's a need to do that, specialize this function.
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


forceinline void add_carry(u8  a, u8  b, u8  *result, bool *carry_out) { *carry_out = (bool)_addcarry_u8 (0, a, b, result); }
forceinline void add_carry(u16 a, u16 b, u16 *result, bool *carry_out) { *carry_out = (bool)_addcarry_u16(0, a, b, result); }
forceinline void add_carry(u32 a, u32 b, u32 *result, bool *carry_out) { *carry_out = (bool)_addcarry_u32(0, a, b, result); }
#if ARCH_X64
forceinline void add_carry(u64 a, u64 b, u64 *result, bool *carry_out) { *carry_out = (bool)_addcarry_u64(0, a, b, result); }
#endif

forceinline void add_carry(u8  a, u8  b, bool carry_in, u8  *result, bool *carry_out) { *carry_out = (bool)_addcarry_u8 (carry_in, a, b, result); }
forceinline void add_carry(u16 a, u16 b, bool carry_in, u16 *result, bool *carry_out) { *carry_out = (bool)_addcarry_u16(carry_in, a, b, result); }
forceinline void add_carry(u32 a, u32 b, bool carry_in, u32 *result, bool *carry_out) { *carry_out = (bool)_addcarry_u32(carry_in, a, b, result); }
#if ARCH_X64
forceinline void add_carry(u64 a, u64 b, bool carry_in, u64 *result, bool *carry_out) { *carry_out = (bool)_addcarry_u64(carry_in, a, b, result); }
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
forceinline constexpr bool is_power_of_2(s8  v) { return v > 0 && count_bits(v) == 1; }
forceinline constexpr bool is_power_of_2(s16 v) { return v > 0 && count_bits(v) == 1; }
forceinline constexpr bool is_power_of_2(s32 v) { return v > 0 && count_bits(v) == 1; }
forceinline constexpr bool is_power_of_2(s64 v) { return v > 0 && count_bits(v) == 1; }

// A better version of conditional operator with ability to use conditions other that booleans.
template <class T> forceinline constexpr T select(bool mask, T a, T b) { return mask ? a : b; }

// NOTE: have to use const & to pass arrays, otherwise they rot.
template <class T> forceinline constexpr auto min(T const &a) { return a; }
template <class T> forceinline constexpr auto max(T const &a) { return a; }
template <class T, umm count>
forceinline constexpr auto min(T const (&array)[count]) {
	T result = array[0];
	for (umm i = 1; i < count; ++i) {
		result = min(result, array[i]);
	}
	return result;
}

template <class T, umm count>
forceinline constexpr auto max(T const (&array)[count]) {
	T result = array[0];
	for (umm i = 1; i < count; ++i) {
		result = max(result, array[i]);
	}
	return result;
}

template <class T> forceinline constexpr auto min(T const &a, T const &b) { return a < b ? a : b; }
template <class T> forceinline constexpr auto max(T const &a, T const &b) { return a > b ? a : b; }

template <class T, class ...Rest>
forceinline constexpr auto min(T first, Rest const &...rest) requires (sizeof...(Rest) > 1) && AllSame<T, Rest...> {
	return ((first = min(first, rest)), ...);
}

template <class T, class ...Rest>
forceinline constexpr auto max(T first, Rest const &...rest) requires (sizeof...(Rest) > 1) && AllSame<T, Rest...> {
	return ((first = max(first, rest)), ...);
}

template <class T> forceinline constexpr void sort_values(T& mn, T &mx) { if (mn > mx) { auto tmp = mx; mx = mn; mn = tmp; }; }

template <class T>
forceinline constexpr T floor(T v, T s) {
	if constexpr (is_signed<T>) {
		return ((v < 0) ? ((v + 1) / s - 1) : (v / s)) * s;
	} else if constexpr (is_unsigned<T>) {
		return v / s * s;
	} else {
		static_error_t(T, "floor(T, T) can be used with integers only");
	}
}

template <class T>
forceinline constexpr T ceil(T v, T s) {
	return floor(v + s - 1, s);
}

forceinline constexpr u8 *floor(u8 *v, umm s) { return (u8 *)floor((umm)v, s); }
forceinline constexpr u8 *ceil(u8 *v, umm s) { return floor(v + s - 1, s); }

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

template <class Base, class Exponent>
requires is_integer_like<Base> && is_integer_like<Exponent>
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


forceinline constexpr u8  floor_to_power_of_2(u8  v) { return v == 0 ? (u8 )0 : (u8 )((u8 )1 << (u8 )log2(v)); }
forceinline constexpr u16 floor_to_power_of_2(u16 v) { return v == 0 ? (u16)0 : (u16)((u16)1 << (u16)log2(v)); }
forceinline constexpr u32 floor_to_power_of_2(u32 v) { return v == 0 ? (u32)0 : (u32)((u32)1 << (u32)log2(v)); }
#if ARCH_X64
forceinline constexpr u64 floor_to_power_of_2(u64 v) { return v == 0 ? (u64)0 : (u64)((u64)1 << (u64)log2(v)); }
#endif

namespace ce {

inline constexpr u8 floor_to_power_of_2(u8 x) {
	x = (u8)(x | (u8)(x >> 1));
	x = (u8)(x | (u8)(x >> 2));
	x = (u8)(x | (u8)(x >> 4));
	return (u8)(x - (u8)(x >> 1));
}
inline constexpr u16 floor_to_power_of_2(u16 x) {
	x = (u16)(x | (u16)(x >> 1));
	x = (u16)(x | (u16)(x >> 2));
	x = (u16)(x | (u16)(x >> 4));
	x = (u16)(x | (u16)(x >> 8));
	return (u16)(x - (u16)(x >> 1));
}
inline constexpr u32 floor_to_power_of_2(u32 x) {
	x = (u32)(x | (u32)(x >> 1));
	x = (u32)(x | (u32)(x >> 2));
	x = (u32)(x | (u32)(x >> 4));
	x = (u32)(x | (u32)(x >> 8));
	x = (u32)(x | (u32)(x >> 16));
	return (u32)(x - (u32)(x >> 1));
}
inline constexpr u64 floor_to_power_of_2(u64 x) {
	x = (u64)(x | (u64)(x >> 1));
	x = (u64)(x | (u64)(x >> 2));
	x = (u64)(x | (u64)(x >> 4));
	x = (u64)(x | (u64)(x >> 8));
	x = (u64)(x | (u64)(x >> 16));
	x = (u64)(x | (u64)(x >> 32));
	return (u64)(x - (u64)(x >> 1));
}

inline constexpr u64 rotate_left(u64 x, u64 bits) {
	return (x << bits) | (x >> (64 - bits));
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

forceinline s32 floor_to_int(f32 v) { return (s32)floor(v); }
forceinline s64 floor_to_int(f64 v) { return (s64)floor(v); }

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

template <class T> forceinline T rotate_left_8  (T v, s32 shift = 1) { return (v << shift) | (v >> ( 8 - shift)); }
template <class T> forceinline T rotate_left_16 (T v, s32 shift = 1) { return (v << shift) | (v >> (16 - shift)); }
template <class T> forceinline T rotate_left_32 (T v, s32 shift = 1) { return (v << shift) | (v >> (32 - shift)); }
template <class T> forceinline T rotate_left_64 (T v, s32 shift = 1) { return (v << shift) | (v >> (64 - shift)); }
template <class T> forceinline T rotate_right_8 (T v, s32 shift = 1) { return (v >> shift) | (v << ( 8 - shift)); }
template <class T> forceinline T rotate_right_16(T v, s32 shift = 1) { return (v >> shift) | (v << (16 - shift)); }
template <class T> forceinline T rotate_right_32(T v, s32 shift = 1) { return (v >> shift) | (v << (32 - shift)); }
template <class T> forceinline T rotate_right_64(T v, s32 shift = 1) { return (v >> shift) | (v << (64 - shift)); }

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

constexpr f32 pi     = f32(3.1415926535897932384626433832795L);
constexpr f32 tau    = f32(6.283185307179586476925286766559L);
constexpr f32 inv_pi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2  = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3  = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5  = f32(2.2360679774997896964091736687313L);

template <class T> forceinline constexpr auto radians(T deg) { return deg * (pi / 180.0f); }
template <class T> forceinline constexpr auto degrees(T rad) { return rad * (180.0f / pi); }

// Does not check if min_bound is greater than max_bound
// There is `clamp_checked` for that.
template <class T>
forceinline constexpr auto clamp(T value, T min_bound, T max_bound) {
	return min(max(value, min_bound), max_bound);
}

template <class T>
forceinline constexpr auto clamp_checked(T value, T min_bound, T max_bound) {
	sort_values(min_bound, max_bound);
	return min(max(value, min_bound), max_bound);
}

template <class From, class To>
forceinline constexpr auto map(From value, From source_min, From source_max, To dest_min, To dest_max) {
	if constexpr (is_integer_like<From>) { // Do multiplication first
		return (value - source_min) * (dest_max - dest_min) / (source_max - source_min) + dest_min;
	} else {
		return (value - source_min) / (source_max - source_min) * (dest_max - dest_min) + dest_min;
	}
}
template <class From, class To>
forceinline constexpr auto map_clamped(From value, From source_min, From source_max, To dest_min, To dest_max) {
	return map(clamp(value, min(source_min, source_max), max(source_min, source_max)), source_min, source_max, dest_min, dest_max);
}
template <class T, class U> forceinline constexpr auto lerp(T a, T b, U t) { return a + (b - a) * t; }

template <class T>
inline constexpr void Swap(T &a, T &b) {
	T t = a;
	a = b;
	b = t;
}

template <class T>
constexpr T midpoint(T a, T b) {
	sort_values(a, b);
	return a + (b - a) / 2;
}

template <class T>
constexpr T *midpoint(T *a, T *b) {
	sort_values(a, b);
	return a + ((umm)(b - a) >> 1);
}

template <class T>
struct AutoCastable {
	T const &value;
	template <class U>
	forceinline constexpr operator U() {
		return (U)value;
	}
};

struct AutoCaster {
	template <class T>
	forceinline constexpr auto operator*(T const &value) {
		return AutoCastable{value};
	}
};

#define autocast AutoCaster{} * 

template <class... Callables>
struct Combine : public Callables... {
	inline constexpr Combine(Callables &&... c) : Callables(std::move(c))... {}
	using Callables::operator()...;
};

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

inline void copy_memory(void *_dst, void const *_src, umm count) {
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

using ForEachFlags = u8;
enum : ForEachFlags {
	ForEach_reverse = 0x1,
};

template <ForEachFlags flags=0, class T, umm count, class Fn>
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
		if constexpr (std::is_same_v<FnRet, void>) {
			fn(*it);
		} else if constexpr (std::is_same_v<FnRet, ForEachDirective>) {
			switch (fn(*it)) {
				case ForEach_break: return;
			}
		} else {
			static_error_t(T, "Invalid return type of for_each function");
		}
	}
}

bool all(auto x, auto predicate) {
	for (auto v : x) {
		if (!predicate(v))
			return false;
	}
	return true;
}

bool any(auto x, auto predicate) {
	for (auto v : x) {
		if (predicate(v))
			return true;
	}
	return false;
}

/*
template <ForEachFlags flags, umm count, class Fn, class ...Ts>
constexpr void for_each(Fn &&fn, Ts ...ts) {
	(fn(ts), ...);
}
*/

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
			return ForEach_break;
		}
		return ForEach_continue;
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
	if ((smm)src_count - (smm)cmp_count + 1 <= 0)
		return 0;

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
	inline ~Deferrer() noexcept(false) { fn(); }

private:
	Fn fn;
#if COMPILER_MSVC
#pragma warning(suppress: 4626)
};
#else
};
#endif

#define defer ::tl::Deferrer CONCAT(_deferrer, __COUNTER__) = [&]

#define scoped_replace(dst, src) \
	auto CONCAT(old_, __LINE__) = dst; \
	dst = src; \
	defer { dst = CONCAT(old_, __LINE__); };

#define scoped_replace_if(dst, src, condition) \
	Optional<decltype(dst)> CONCAT(old_, __LINE__); \
	if (condition) { CONCAT(old_, __LINE__) = dst; dst = src; } \
	defer { if (CONCAT(old_, __LINE__)) dst = CONCAT(old_, __LINE__).value(); };

template <class T>
auto reversed(T x) {
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


inline constexpr struct null_opt_t {} null_opt;

template <class T>
struct Optional {
	constexpr Optional() {
		_has_value = false;
	}
	constexpr Optional(T that) {
		_value = that;
		_has_value = true;
	}
	constexpr Optional(null_opt_t) {
		_has_value = false;
	}
	constexpr ~Optional() {
		if (_has_value)
			_value.~T();
	}
	constexpr Optional &operator=(T that) {
		return *new(this) Optional(that);
	}
	constexpr Optional &operator=(null_opt_t) {
		return *new(this) Optional();
	}

	constexpr void reset() {
		_has_value = false;
	}

	constexpr explicit operator bool() const { return _has_value; }

	constexpr bool has_value() const { return _has_value; }
	constexpr T const &value() const { assert_always(_has_value); return _value; }
	constexpr T       &value()       { assert_always(_has_value); return _value; }
	constexpr T const &value_unchecked() const { return _value; }
	constexpr T       &value_unchecked()       { return _value; }

	template <class Fallback>
	constexpr T value_or(Fallback &&fallback) requires requires { (T)fallback(); } {
		if (_has_value)
			return _value;
		return fallback();
	}

	constexpr T value_or(T fallback) {
		if (_has_value)
			return _value;
		return fallback;
	}


	template <class U>
	constexpr Optional<U> map() {
		if (_has_value) {
			return (U)_value;
		}
		return null_opt;
	}

	constexpr void apply(auto &&fn) {
		if (_has_value)
			fn(_value);
	}

private:
	union {
		T _value;
	};
	bool _has_value;
#pragma warning(suppress: 4820)
};

template <class T> Optional<T> operator+(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() + b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator-(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() - b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator*(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() * b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator/(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() / b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator%(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() % b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator^(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() ^ b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator&(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() & b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator|(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() | b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator<<(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() << b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator>>(Optional<T> a, Optional<T> b) { return (a && b) ? Optional<T>{a.value_unchecked() >> b.value_unchecked()} : Optional<T>{}; }

template <class Value, class Error> 
struct Result {

	Result(Value value) : _is_value(true), _value(value) {}
	Result(Error error) : _is_value(false), _error(error) {}
	Result(const Result &that) { memcpy(this, &that, sizeof *this); }
	Result(Result &&that) { memcpy(this, &that, sizeof *this); }
	~Result() {
		if (_is_value)
			_value.~Value();
		else
			_error.~Error();
	}
	Result &operator=(const Result &that) { memcpy(this, &that, sizeof *this); return *this; }
	Result &operator=(Result &&that) { memcpy(this, &that, sizeof *this); return *this; }

	Value value() { assert(_is_value); return _value; }
	Error error() { assert(!_is_value); return _error; }

	bool is_value() { return _is_value; }
	bool is_error() { return !_is_value; }

	explicit operator bool() { return _is_value; }

private:
	bool _is_value;
	union {
		Value _value;
		Error _error;
	};
};

#pragma pack(push, 1)
template <class T, class Size_ = umm>
struct Span {
	using ValueType = T;
	using Size = Size_;
	using ReverseIterator = ReverseIterator<T *>;

	constexpr Span(std::initializer_list<ValueType> list) : data((ValueType *)list.begin()), count(list.size()) {}
	constexpr Span() = default;
	// constexpr Span(ValueType &value) : data(std::addressof(value)), count(1) {}
	template <Size count>
	constexpr Span(ValueType (&array)[count]) : data(array), count(count) {}
	constexpr Span(ValueType *begin, ValueType *end) : data(begin), count(end - begin) {
		assert(count == (umm)(end - begin));
	}
	constexpr Span(ValueType const *begin, ValueType const *end) : data((ValueType *)begin), count(end - begin) {}
	constexpr Span(ValueType *begin, Size count) : data(begin), count(count) {}
	constexpr Span(ValueType const *begin, Size count) : data((ValueType *)begin), count(count) {}
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
	constexpr ValueType &operator[](Size i) const {
		bounds_check(i < count);
		return data[i];
	}
	constexpr ValueType &at(Size i) const {
		bounds_check(i < count);
		return data[i];
	}

	constexpr ValueType at_interpolated(f64 i) const {
		auto a = at(floor_to_int(i));
		auto b = at(ceil_to_int(i));
		return lerp(a, b, frac(i));
	}

	constexpr ValueType at_interpolated_clamped(f64 i) const {
		auto a = at(floor_to_int(i));
		auto b = at(min(count - 1, ceil_to_int(i)));
		return lerp(a, b, frac(i));
	}

	constexpr bool is_empty() const { return count == 0; }

	template <class U, class ThatSize>
	constexpr explicit operator Span<U, ThatSize>() const {
		static_assert(sizeof(U) == sizeof(T));
		assert((ThatSize)count == count);
		return {(U *)data, (ThatSize)count};
	}

	template <class ThatSize>
	constexpr operator Span<T, ThatSize>() const {
		assert((ThatSize)count == count);
		return {data, (ThatSize)count};
	}

	template <class ThatSize>
	constexpr bool operator==(Span<T, ThatSize> that) const {
		if (count != that.count)
			return false;
		for (Size i = 0; i < count; ++i) {
			if (data[i] != that.data[i])
				return false;
		}
		return true;
	}
	constexpr bool operator!=(Span that) const { return !(*this == that); }

	constexpr Span subspan(Size subspan_start, Size subspan_count) const {
		return {
			min(data + subspan_start, end()),
			min(data + subspan_start + subspan_count, end()),
		};
	}
	constexpr Span skip(smm amount) const {
		if (amount >= 0) {
			if ((Size)amount >= count)
				return {};
			return {data + amount, (Size)((smm)count - amount)};
		} else {
			if ((Size)-amount >= count)
				return {};
			return {data, (Size)(count + amount)};
		}
	}
	constexpr Span take(smm amount) const {
		if (amount > 0) {
			return { 
				begin(), 
				min(end(), begin() + amount)
			};
		} else {
			return { 
				max(begin(), end() + amount),
				end(),
			};
		}
	}
	constexpr void set_begin(T *new_begin) {
		count = end() - new_begin;
		data = new_begin;
	}
	constexpr void set_end(T *new_end) {
		count = new_end - data;
	}

	template <class U, class ThatSize=Size>
	constexpr Span<U, ThatSize> unsafe_as() const {
		if constexpr (sizeof(T) > sizeof(U))
			static_assert(sizeof(T) % sizeof(U) == 0);
		else
			static_assert(sizeof(U) % sizeof(T) == 0);

		return {(U *)data, (ThatSize)(count * sizeof(T) / sizeof(U))};
	}

	constexpr explicit operator bool() const { return count; }

	ValueType *data = 0;
	Size count = 0;
};
#pragma pack(pop)

template <class T>
inline constexpr bool is_span = false;

template <class T, class Size>
inline constexpr bool is_span<Span<T, Size>> = true;

template <class T>
concept ASpan = is_span<T>;

template <class T, umm x>               inline Span<T> flatten(T (&array)[x]      ) { return {(T *)array, x    }; }
template <class T, umm x, umm y>        inline Span<T> flatten(T (&array)[x][y]   ) { return {(T *)array, x*y  }; }
template <class T, umm x, umm y, umm z> inline Span<T> flatten(T (&array)[x][y][z]) { return {(T *)array, x*y*z}; }

template <ForEachFlags flags=0, class T, class Fn>
constexpr void for_each(Span<T> span, Fn &&fn) {
	using FnRet = decltype(fn(*(T*)0));

	T *start = 0;
	T *end = 0;
	umm step = 0;
	if constexpr (flags & ForEach_reverse) {
		start = span.data + span.count - 1;
		end = span.data - 1;
		step = (umm)-1;
	} else {
		start = span.data;
		end = span.data + span.count;
		step = (umm)1;
	}

	for (auto it = start; it != end; it += step) {
		if constexpr (std::is_same_v<FnRet, void>) {
			fn(*it);
		} else if constexpr (std::is_same_v<FnRet, ForEachDirective>) {
			if (fn(*it) == ForEach_break) {
				break;
			}
		} else {
			static_error_t(T, "Invalid return type of for_each function");
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
forceinline constexpr Span<u8> operator""b(char const *string, umm count) { return Span((u8 *)string, count); }

forceinline constexpr Span<utf8, u32> operator""s32(utf8  const *string, umm count) { return Span((utf8  *)string, (u32)count); }

template <class T, umm count>
inline constexpr Span<T> array_as_span(T const (&arr)[count]) { return Span((T *)arr, count); }

template <class T>
inline constexpr Span<T> as_span(std::initializer_list<T> list) { return Span((T *)list.begin(), list.size()); }

inline constexpr Span<char > as_span(char  const *str) { return Span((char  *)str, string_unit_count(str)); }
inline constexpr Span<wchar> as_span(wchar const *str) { return Span((wchar *)str, string_unit_count(str)); }
inline constexpr Span<utf8 > as_span(utf8  const *str) { return Span((utf8  *)str, string_unit_count(str)); }
inline constexpr Span<utf16> as_span(utf16 const *str) { return Span((utf16 *)str, string_unit_count(str)); }
inline constexpr Span<utf32> as_span(utf32 const *str) { return Span((utf32 *)str, string_unit_count(str)); }

template <class T, class Size>
inline constexpr Span<T, Size> as_span(Span<T, Size> span) {
	return span;
}

template <class T, class Size>
inline constexpr Span<T, Size> as_span_of(Span<u8, Size> span) {
	return {
		(T *)span.data,
		span.count / sizeof(T),
	};
}

template <class T, class Size>
constexpr Span<utf8, Size> as_utf8(Span<T, Size> span) {
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

template <class T, class Size>
constexpr Span<u8, Size> as_bytes(Span<T, Size> span) {
	return {(u8 *)span.begin(), span.count * sizeof(T)};
}

template <class T, class Size>
constexpr Span<char, Size> as_chars(Span<T, Size> span) {
	return {(char *)span.begin(), span.count * sizeof(T)};
}
template <class T>
constexpr Span<char> as_chars(T span_like) {
	return as_chars(as_span(span_like));
}


template <class T>
constexpr Span<u8> value_as_bytes(T const &value) {
	return {(u8 *)&value, sizeof(T)};
}
template <class T>
constexpr Span<T> value_as_span(T const &value) {
	return {&value, 1};
}

template <class T, class Size>
constexpr umm count_of(Span<T, Size> span) { return span.count; }

template <class T, class Size, class Fn>
umm count(Span<T, Size> span, Fn &&fn) {
	umm result = 0;
	for (auto &v : span) {
		if (fn(v)) {
			result += 1;
		}
	}
	return result;
}

template <class T>
constexpr void replace(Span<T> destination, Span<T> source, umm start_index = 0) {
	for (umm i = 0; i < source.count; ++i) {
		destination[start_index + i] = source[i];
	}
}

template <class T, class TSize, class U, class USize>
inline constexpr bool starts_with(Span<T, TSize> str, Span<U, USize> sub_str) {
	if (sub_str.count > str.count)
		return false;
	for (USize i = 0; i < sub_str.count; ++i) {
		if (str.data[i] != sub_str.data[i]) {
			return false;
		}
	}
	return true;
}

template <class T, class TSize, class U, class USize>
inline constexpr bool ends_with(Span<T, TSize> str, Span<U, USize> sub_str) {
	if (sub_str.count > str.count)
		return false;
	auto base_offset = str.count - sub_str.count;
	for (USize i = 0; i < sub_str.count; ++i) {
		if (str.data[i + base_offset] != sub_str.data[i]) {
			return false;
		}
	}
	return true;
}

template <class T, umm count>
constexpr T *find(T (&where)[count], T const &what) {
	return find(where, where + count, what);
}

template <class T, class Size>
constexpr T *find(Span<T, Size> where, T const &what) {
	return find(where.begin(), where.end(), what);
}

template <class T, class SizeA, class SizeB>
constexpr T *find(Span<T, SizeA> where, Span<T, SizeB> what) {
	return find(where.begin(), where.end(), what.begin(), what.end());
}

template <class T>
constexpr T *find(Span<T> where, Span<Span<T>> whats) {
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

template <class T, class Size>
constexpr T *find_last(Span<T, Size> span, T const &value) {
	if (span.count == 0)
		return 0;

	for (auto it = span.end() - 1; it >= span.begin(); --it) {
		if (*it == value)
			return it;
	}
	return 0;
}

template <class T, class SizeA, class SizeB>
constexpr Span<T, SizeA> find_last(Span<T, SizeA> span, Span<T, SizeB> sub) {
	Span<T, SizeB> dest = {span.end() - sub.count, sub.count};
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
				return ForEach_break;
			}
		}
		return ForEach_continue;
	});
	return result;
}

template <class Collection>
umm find_index_of(Collection &collection, const ValueTypeOf<Collection> &value) {
	return index_of(collection, find(collection, value));
}

template <class Collection, class Predicate>
umm find_index_of_if(Collection &collection, Predicate predicate) {
	return index_of(collection, find_if(collection, predicate));
}

template <class T>
T *binary_search(Span<T> span, T value) {
	auto begin = span.begin();
	auto end   = span.end();
	while (1) {
		if (begin == end)
			return 0;

		auto mid = begin + (end - begin) / 2;
		if (value == *mid)
			return mid;

		if (value < *mid) {
			end = mid;
		} else {
			begin = mid + 1;
		}
	}
}

template <class T, class U, class Fn>
T *binary_search(Span<T> span, U value, Fn get_value) {
	auto begin = span.begin();
	auto end   = span.end();
	while (1) {
		if (begin == end)
			return 0;

		auto mid = begin + (end - begin) / 2;
		if (value == get_value(*mid))
			return mid;

		if (value < get_value(*mid)) {
			end = mid;
		} else {
			begin = mid + 1;
		}
	}
}

template <class T>
void flip_order(Span<T> span) {
	for (umm i = 0; i < span.count / 2; ++i) {
		Swap(span[i], span[span.count-i-1]);
	}
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
	constexpr utf32 ranges[][2] {
		{0x41, 0x5a},
		{0x61, 0x7a},
		{0xc0, 0xd6},
		{0xd8, 0xf6},
		{0xf8, 0x148},
		{0x14a, 0x24f},
		{0x400, 0x482},
		{0x1e02, 0x1ef3},
	};
	for (auto range : ranges) {
		if (range[0] <= c && c <= range[1]) {
			return true;
		}
	}
	return false;
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
		return (ascii)(c + ('a' - 'A'));
	return c;
}
inline constexpr utf8 to_lower(utf8 c) {
	if (c >= 'A' && c <= 'Z')
		return (utf8)(c + ('a' - 'A'));
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

inline bool equals_case_insensitive(Span<utf8> a, Span<utf8> b) {
	if (a.count != b.count)
		return false;
	auto ap = a.begin();
	for (auto bp = b.begin(); ap != a.end(); ++ap, ++bp)
		if (to_lower(*ap) != to_lower(*bp))
			return false;
	return true;
}

template <class T, class Predicate>
requires std::is_invocable_v<Predicate, T, T>
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

template <class T, umm _capacity>
struct StaticList {
	using ValueType = T;

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

	forceinline constexpr ReverseIterator<T *> rbegin() { return data + count - 1; }
	forceinline constexpr ReverseIterator<T *> rend() { return data - 1; }

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

	template <class Size = umm>
	forceinline constexpr Span<T, Size> span() { return {data, count}; }

	template <class U, class ThatSize>
	constexpr explicit operator Span<U, ThatSize>() const {
		static_assert(sizeof(U) == sizeof(T));
		assert((ThatSize)count == count);
		return {(U *)data, (ThatSize)count};
	}

	template <class ThatSize>
	constexpr operator Span<T, ThatSize>() const {
		assert((ThatSize)count == count);
		return {data, (ThatSize)count};
	}

	forceinline constexpr T &add() {
		bounds_check(!full());
		return *new(data + count++) T();
	}

	forceinline constexpr T &add(T const &value) {
		bounds_check(!full());
		return data[count++] = value;
	}

	template <class Size>
	forceinline constexpr Span<T> add(Span<T, Size> span) {
		bounds_check(count + span.count <= capacity);
		memcpy(data + count, span.data, span.count * sizeof(T));
		defer { count += span.count; };
		return {data + count, span.count};
	}

	constexpr Optional<T> pop() {
		if (count == 0)
			return {};
		return data[--count];
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

template <class T, umm capacity>
struct StaticSet {

	T *begin() { return data; }
	T const *begin() const { return data; }
	T *end() { return data + count; }
	T const *end() const { return data + count; }

	T *find(T const &value) {
		for (auto &it : *this) {
			if (it == value)
				return &it;
		}
		return 0;
	}
	T &get_or_insert(T const &value) {
		if (auto found = find(value))
			return *found;
		return data[count++] = value;
	}

	bool remove(T const &value) {
		if (auto found = find(value)) {
			--count;
			memcpy(found, found + 1, sizeof(T) * (end() - found));
			return true;
		}
		return false;
	}

	Optional<T> pop() {
		if (count)
			return data[--count];
		return {};
	}

	union {
		T data[capacity];
	};
	umm count = 0;
};

template <umm size>
struct BitSet {
	using Word = umm;
	inline static constexpr auto bits_in_word = sizeof(Word) * 8;
	Word words[ceil(size, bits_in_word) / bits_in_word] = {};

	bool get(umm i) {
		return word(i) & bit(i);
	}
	void set(umm i) {
		word(i) |= bit(i);
	}
	void unset(umm i) {
		word(i) &= ~bit(i);
	}
	void flip(umm i) {
		word(i) ^= bit(i);
	}

	umm &word(umm i) {
		return words[i / bits_in_word];
	}
	umm bit(umm i) {
		return (Word)1 << (i % bits_in_word);
	}

	umm count() {
		umm result = 0;
		for (auto word : words) {
			result += count_bits(word);
		}
		return result;
	}

	Optional<umm> pop() {
		for (umm word_index = 0; word_index < count_of(words); ++word_index) {
			auto &word = words[word_index];

			auto bit_index = find_lowest_one_bit(word);
			if (bit_index == ~0)
				continue;
			word &= ~(1 << bit_index);
			return word_index * bits_in_word + bit_index;
		}
		return {};
	}
	inline constexpr auto operator<=>(BitSet const &) const = default;
};

template <ForEachFlags flags = 0, umm size>
void for_each(BitSet<size> set, auto &&fn)
	requires requires { fn((umm)0); }
{
	using Word = typename decltype(set)::Word;

	if constexpr (flags & ForEach_reverse) {
		for (umm word_index = count_of(set.words) - 1; word_index != -1; --word_index) {
			auto word = set.words[word_index];
			if (!word)
				continue;

			for (u64 bit = set.bits_in_word - 1; bit != -1; --bit) {
				if (word & ((Word)1 << bit)) {
					auto absolute_index = word_index * set.bits_in_word + bit;
					if constexpr (std::is_same_v<decltype(fn(absolute_index)), ForEachDirective>) {
						if (fn(absolute_index) == ForEach_break)
							return;
					} else {
						fn(absolute_index);
					}
				}
			}
		}
	} else {
		for (umm word_index = 0; word_index < count_of(set.words); ++word_index) {
			auto word = set.words[word_index];
			if (!word)
				continue;

			for (u64 bit = 0; bit < set.bits_in_word; ++bit) {
				if (word & ((Word)1 << bit)) {
					auto absolute_index = word_index * set.bits_in_word + bit;
					if constexpr (std::is_same_v<decltype(fn(absolute_index)), ForEachDirective>) {
						if (fn(absolute_index) == ForEach_break)
							return;
					} else {
						fn(absolute_index);
					}
				}
			}
		}
	}
}

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
template <class Collection, class T>
bool find_and_erase_unordered(Collection &collection, T value) {
	auto found = find(collection, value);
	if (found) {
		erase_unordered(collection, found);
		return true;
	}
	return false;
}

/*
template <class ...Types>
struct Tuple;

template <>
struct Tuple<> {
};

template <class First, class ...Rest>
struct Tuple<First, Rest...> : First, Tuple<Rest...> {
};

template <umm index, class ...Args>
auto &get(Tuple<Args...> tuple) {
	if constexpr (index == 0) {
		return (TypeAt<0, Args...>&)tuple;
	} else {
		return get<index - 1>(tuple.rest);
	}
}

*/

using NativeWindowHandle = struct NativeWindow {} *;

enum AllocatorAction : u8 {
	Allocator_allocate,
	Allocator_reallocate,
	Allocator_free,
};

#ifndef TL_DEFAULT_ALIGNMENT
#define TL_DEFAULT_ALIGNMENT ((::tl::umm)16)
#endif

struct Allocator;

extern TL_API Allocator os_allocator;
extern TL_API Allocator page_allocator;
extern TL_API Allocator default_allocator;

extern TL_API thread_local Allocator current_allocator;

struct AllocationResult {
	void *data = 0;
	umm count = 0;
	bool is_zeroed : 1 = false;
};

//////////////////////////////////////////
// HOW TO IMPLEMENT A STATIC ALLOCATOR? //
//////////////////////////////////////////
#if 0
// Deriving from `AllocatorBase` provides useful function overloads.
// This is necessary if you want to use this allocator with containers in `tl`.
struct MyAllocator : AllocatorBase<MyAllocator> {

	// This is an interface you have to implement:

	// Must allocate at least `size` bytes with specified alignment.
	// The actual allocated byte count should go into `AllocationResult::count`.
	// Resulting memory is allowed to be not initialized. Overloads from `AllocatorBase` will zero it out for the users.
	// If allocated memory is zeroed out, set `AllocationResult::is_zeroed` to true, so `AllocatorBase` does no have to do this again.
	AllocationResult allocate_impl(umm size, umm alignment TL_LP);

	// Must allocate at least `new_size` bytes with specified alignment.
	// Old memory `{data, old_size}` must be copied to resulting memory.
	// Rules for memory contents are the same as for `allocate_impl`.
	AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP);

	// Must free the `data`.
	// `size` and `alignment` are optional and are equal to 0 if unknown.
	// Assert that they are not zero if you rely on them.
	void deallocate_impl(void *data, umm size, umm alignment TL_LP);

	// `current` will be used for default-initialized structures like `List`, etc.
	static MyAllocator current();

	// Not much to say here.
	bool is_valid();

	// Returns true if pointers from this and that can be exchanged.
	bool operator==(MyAllocator that);
};
#endif

template <class T>
concept DefaultConstructible = requires { new T(); };

// TODO:
// Some allocators may give more memory than requested, so the caller should know about this.
// I think `allocate` should return `Span<u8>` instead of `void *` when requesting raw bytes.
// But in case of generic types it's not clear what to do with excess objects.
// For now allocation of T's will not report extra space.

template <class Derived>
struct AllocatorBase {
	forceinline Derived *derived() { return (Derived*)this; }

	inline void *allocate_uninitialized(umm size, umm alignment = TL_DEFAULT_ALIGNMENT TL_LP) {
		return derived()->allocate_impl(size, alignment TL_LA).data;
	}
	inline void *allocate_uninitialized(umm size, std::source_location location) {
		return derived()->allocate_impl(size, TL_DEFAULT_ALIGNMENT TL_LA).data;
	}
	inline void *allocate(umm size, umm alignment = TL_DEFAULT_ALIGNMENT TL_LP) {
		auto result = derived()->allocate_impl(size, alignment TL_LA);
		if (!result.is_zeroed) {
			memset(result.data, 0, size);
		}
		return result.data;
	}
	template <class T>
	inline T *allocate_uninitialized(umm count = 1, umm alignment = alignof(T) TL_LP) {
		return (T *)allocate_uninitialized(count * sizeof(T), alignment TL_LA);
	}

	template <class T>
	inline T *allocate_uninitialized(umm count, std::source_location location) {
		return (T *)allocate_uninitialized(count * sizeof(T), alignof(T) TL_LA);
	}


	template <class T>
	inline T *allocate(umm count = 1, umm alignment = alignof(T) TL_LP) {
		auto result = derived()->allocate_impl(count * sizeof(T), alignment TL_LA);
		if (result.data) {
			for (auto it = (T *)result.data; it != (T *)result.data + count; ++it) {
				if constexpr (DefaultConstructible<T>) {
					new (it) T();
				} else {
					memset(it, 0, sizeof(T));
				}
			}
		}
		return (T *)result.data;
	}

	template <class T>
	inline T *allocate(umm count, std::source_location location) {
		return allocate<T>(count, alignof(T) TL_LA);
	}

	template <class T>
	inline T *allocate(std::source_location location) {
		return allocate<T>(1, alignof(T) TL_LA);
	}


	inline void *reallocate_uninitialized(void *data, umm old_size, umm new_size, umm align = TL_DEFAULT_ALIGNMENT TL_LP) {
		return derived()->reallocate_impl(data, old_size, new_size, align TL_LA).data;
	}
	inline void *reallocate(void *data, umm old_size, umm new_size, umm align = TL_DEFAULT_ALIGNMENT TL_LP) {
		auto result = derived()->reallocate_impl(data, old_size, new_size, align TL_LA);
		if (!result.is_zeroed && result.data && (new_size > old_size)) {
			memset((u8 *)result.data + old_size, 0, new_size - old_size);
		}
		return result.data;
	}

	template <class T>
	inline T *reallocate_uninitialized(T *data, umm old_count, umm new_count, umm align = alignof(T) TL_LP) {
		return (T *)reallocate_uninitialized((void *)data, old_count * sizeof(T), new_count * sizeof(T), align TL_LA);
	}
	template <class T>
	inline T *reallocate_uninitialized(T *data, umm old_count, umm new_count, std::source_location location) {
		return reallocate_uninitialized<T>(data, old_count, new_count, alignof(T) TL_LA);
	}

	template <class T>
	inline T *reallocate(T *data, umm old_count, umm new_count, umm align = alignof(T) TL_LP) {
		auto result = derived()->reallocate_impl(data, old_count * sizeof(T), new_count * sizeof(T), align TL_LA);
		if (result.data) {
			for (auto it = (T *)result.data + old_count; it != (T *)result.data + new_count; ++it) {
				new (it) T();
			}
		}
		return (T *)result.data;
	}


	template <class T>
	inline void free_t(T *data, umm count = 0, umm alignment = alignof(T) TL_LP) {
		mark_dead((u8 *)data, count * sizeof(T));
		derived()->deallocate_impl(data, count * sizeof(T), alignment TL_LA);
	}

	inline void free(void *data, umm count = 0, umm alignment = 0 TL_LP) {
		mark_dead((u8 *)data, count);
		derived()->deallocate_impl(data, count, alignment TL_LA);
	}

	void mark_dead(u8 *data, umm size) {
#if TL_DEBUG
		u8 mark[] { 0xde, 0xad };
		for (umm i = 0; i != size; ++i) {
			data[i] = mark[i&1];
		}
#endif
	}

	forceinline explicit operator bool() { return derived()->is_valid(); }

	inline AllocationResult execute(AllocatorAction action, void *data, umm old_size, umm new_size, umm alignment TL_LP) {
		switch (action) {
			case ::tl::Allocator_allocate:   return derived()->allocate_impl(new_size, alignment TL_LA);
			case ::tl::Allocator_reallocate: return derived()->reallocate_impl(data, old_size, new_size, alignment TL_LA);
			case ::tl::Allocator_free:       derived()->deallocate_impl(data, new_size, alignment TL_LA); break;
		}
		return {};
	}
};

struct Allocator : AllocatorBase<Allocator> {
	// When deallocating size goes into `new_size`
	AllocationResult (*func)(AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) = 0;
	void *state = 0;

	inline AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		return func(Allocator_allocate, 0, 0, size, alignment, state TL_LA);
	}
	inline AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP) {
		return func(Allocator_reallocate, data, old_size, new_size, alignment, state TL_LA);
	}
	inline void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
		func(Allocator_free, data, 0, size, alignment, state TL_LA);
	}

	inline static Allocator current() { return current_allocator; }

	inline bool is_valid() { return func != 0; }

	inline bool operator==(Allocator that) {
		return this->func == that.func && this->state == that.state;
	}
};

template <class CustomAllocator>
Allocator make_allocator_from(CustomAllocator *allocator) {
	return {
		.func = [](AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) -> AllocationResult {
			switch (action) {
				case Allocator_allocate: {
					return ((CustomAllocator *)state)->allocate_impl(new_size, align TL_LA);
				}
				case Allocator_reallocate: {
					return ((CustomAllocator *)state)->reallocate_impl(data, old_size, new_size, align TL_LA);
				}
				case Allocator_free: {
					((CustomAllocator *)state)->deallocate_impl(data, new_size, align TL_LA);
					break;
				}
			}
			return {};
		},
		.state = allocator
	};
}

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

struct DefaultAllocator : AllocatorBase<DefaultAllocator> {

	static DefaultAllocator current() { return {}; }

	AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		return {
			.data = tl_allocate(size, alignment),
			.count = size,
			.is_zeroed = false,
		};
	}
	AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP) {
		return {
			.data = tl_reallocate(data, new_size, alignment),
			.count = new_size,
			.is_zeroed = false,
		};
	}
	void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
		tl_free(data);
	}
};

// This library provides a temporary allocator, which you can use for fast allocations.
// It is a very simple implementation, which just bumps a cursor. This of course means you can't just free
// a portion of memory at arbitrary time and reuse it, but you can:
//     1. Clear the entire temporary storage.
//     2. Remember the cursor, do some allocations, reset the cursor.
// 
// Previously, temporary allocator used to create a new block if it ran out of space.
// But that added some complexity to `allocate` function, and made it easy to forget
// how much memory was used. For these reasons I decided to make it use a single block
// of predefined size (TL_TEMPORARY_STORAGE_CAPACITY), which you can override.
//
// You can use this allocator as a regular one, e.g. in collections, etc.
// 
// Use `scoped(temporary_allocator)` to make temporary allocator current in the current scope.
//
// Optionally you can use `scoped(temporary_storage_checkpoint)` to remember the cursor and 
// reset it to its previous state at the end of the scope. That way more memory space is utilized.
// But you need to know the lifetimes.
//
// Use `scoped(temporary_allocator_and_checkpoint)` to use them both.
//

#ifndef TL_TEMPORARY_STORAGE_CAPACITY
#define TL_TEMPORARY_STORAGE_CAPACITY (1 * MiB)
#endif

struct TemporaryAllocator : AllocatorBase<TemporaryAllocator> {

	inline static constexpr umm buffer_size = TL_TEMPORARY_STORAGE_CAPACITY;
	inline static thread_local u8 *base = 0;
	inline static thread_local u8 *cursor = 0;

	forceinline static void init() {
		cursor = base = current_allocator.allocate_uninitialized<u8>(buffer_size);
	}

	forceinline static TemporaryAllocator current() { return {}; }

	forceinline static AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		assert(cursor, "temporary allocator was not initialized");

		auto target = ceil(cursor, alignment);
		cursor = target + size;
		assert(cursor <= base + buffer_size, "Out of temporary memory");
		return AllocationResult { .data = target, .count = size, .is_zeroed = true };
	}
	forceinline static AllocationResult reallocate_impl(void *old_data, umm old_size, umm new_size, umm alignment TL_LP) {
		auto new_data = allocate_impl(new_size, alignment);
		memcpy(new_data.data, old_data, old_size);
		return new_data;
	}
	forceinline static void deallocate_impl(void *data, umm size, umm alignment TL_LP) {}

	forceinline operator Allocator() {
		return {
			.func = [](AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) -> AllocationResult {
				return ((TemporaryAllocator *)state)->execute(action, data, old_size, new_size, align TL_LA);
			},
			.state = this
		};
	}

	forceinline static void clear() {
		cursor = base;
	}
};

extern TL_API thread_local TemporaryAllocator temporary_allocator;

inline struct TemporaryStorageCheckpoint {} temporary_storage_checkpoint;
inline struct TemporaryAllocatorAndCheckpoint {} temporary_allocator_and_checkpoint;

extern TL_API void init_allocator(Allocator tempory_allocator_backup = os_allocator);
extern TL_API void deinit_allocator();

#define tl_push(pusher, ...) if(auto CONCAT(_tl_, __LINE__)=pusher(__VA_ARGS__))
#define tl_scoped(current, new) auto CONCAT(_tl_,__LINE__)=current;current=(new);defer{current=CONCAT(_tl_,__LINE__);}

template <class T>
inline void allocate(T *&val) {
	val = current_allocator.allocate<T>();
}

#define MAKE_ALLOCATOR_FROM_TYPE(type, name, ...) \
type CONCAT(_state, __LINE__) = {__VA_ARGS__}; \
Allocator name = { \
	[](AllocatorAction action, void *data, umm old_size, umm new_size, umm alignment, std::source_location location, void *state) -> void * { \
		switch (action) { \
			case ::tl::Allocator_allocate:   return ((type *)state)->allocate(new_size, alignment TL_LA); \
			case ::tl::Allocator_reallocate: return ((type *)state)->reallocate(data, old_size, new_size, alignment TL_LA); \
			case ::tl::Allocator_free:       return ((type *)state)->deallocate(data, new_size, alignment TL_LA), (void *)0; \
		} \
	}, \
	&CONCAT(_state, __LINE__) \
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

template <class Thing>
struct Scoped {
	Scoped(Thing &) { static_error_t(Thing, "scoped replacer for that type was not defined. for an example check Scoped<Allocator> down below."); }
	Scoped(Thing &&) { static_error_t(Thing, "scoped replacer for that type was not defined. for an example check Scoped<Allocator> down below."); }
};
template <class Thing>
Scoped(Thing) -> Scoped<Thing>;

// Example use:
// auto str = with(temporary_allocator, format("{}, {}", a, b));
#define with(new_thing, ...) ([&]()->decltype(auto){auto replacer=Scoped(new_thing);return __VA_ARGS__;}())
#define scoped(new_thing) auto CONCAT(_scoped_, __LINE__) = Scoped(new_thing)

template <class Thing>
struct ScopedBlock {
	Thing storage;
	Thing *pointer;
	ScopedBlock(Thing &thing) : pointer(&thing) {}
	ScopedBlock(Thing &&thing) : storage(thing), pointer(&storage) {}

	template <class Fn>
	void operator+(Fn &&fn) {
		scoped(*pointer);
		fn();
	}
};

template <class Thing>
ScopedBlock(Thing) -> ScopedBlock<Thing>;

// Example use:
// withs(temporary_allocator) {
//     do_some_stuff();
//     do_other_stuff();
// };
#define withs(new_thing) ScopedBlock(new_thing) + [&]

template <>
struct Scoped<Allocator> {
	Allocator old_allocator;
	Scoped(Allocator new_allocator) {
		old_allocator = current_allocator;
		current_allocator = new_allocator;
	}
	~Scoped() {
		current_allocator = old_allocator;
	}
};

template <>
struct Scoped<TemporaryAllocator> {
	Allocator old_allocator;
	Scoped(TemporaryAllocator) {
		old_allocator = current_allocator;
		current_allocator = temporary_allocator;
	}
	~Scoped() {
		current_allocator = old_allocator;
	}
};

template <>
struct Scoped<TemporaryStorageCheckpoint> {
	u8 *initial_cursor = 0;

	Scoped(TemporaryStorageCheckpoint) {
		initial_cursor = temporary_allocator.cursor;
	}
	~Scoped() {
		temporary_allocator.cursor = initial_cursor;
	}
};

template <>
struct Scoped<TemporaryAllocatorAndCheckpoint> {
	Allocator old_allocator;
	u8 *initial_cursor = 0;

	Scoped(TemporaryAllocatorAndCheckpoint) {
		old_allocator = current_allocator;
		current_allocator = temporary_allocator;
		initial_cursor = temporary_allocator.cursor;
	}
	~Scoped() {
		temporary_allocator.cursor = initial_cursor;
		current_allocator = old_allocator;
	}
};

template <class T>
void rotate(Span<T> span, T *to_be_first) {
	umm left_count = to_be_first - span.data;
	umm right_count = span.count - left_count;

	auto initial_cursor = temporary_allocator.cursor;
	defer { temporary_allocator.cursor = initial_cursor; };

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

template <class T>
void quick_sort(Span<T> span, auto fn) {
	if (span.count < 2)
		return;

	auto compare = [&] (T a, T b) {
		if constexpr (std::is_invocable_r_v<bool, decltype(fn), T, T>) {
			return fn(a, b);
		} else {
			return fn(a) < fn(b);
		}
	};

	auto partition = [&](T *begin, T *end) {
		auto pivot = end[-1];
		auto mid = begin;
		for (auto i = begin; i < end-1; i++)
		{
			if (compare(*i, pivot))
			{
				Swap(*i, *mid);
				mid++;
			}
		}

		Swap(*mid, end[-1]);

		return mid;
	};

	T *p = partition(span.begin(), span.end());
	quick_sort(Span<T>{span.begin(), p}, fn);
	quick_sort(Span<T>{p+1, span.end()}, fn);
}
template <class T>
void quick_sort(Span<T> span) {
	quick_sort(span, [](T a, T b) { return a < b; });
}
template <class T, umm size>
void quick_sort(T (&array)[size], auto ...args) {
	quick_sort(array_as_span(array), args...);
}

#ifdef TL_IMPL

AllocationResult page_allocator_proc(AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) {
	(void)state;
#if TL_PARENT_SOURCE_LOCATION
	(void)location;
#endif
	switch (action) {
		case Allocator_allocate: {
			assert(align <= 4096);
			return {
				.data = VirtualAlloc(0, new_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE),
				.count = new_size / 4096 * 4096,
				.is_zeroed = true,
			};
		}
		case Allocator_reallocate: {
			assert(align <= 4096);

			if (old_size / 4096 == new_size / 4096) {
				return {
					.data = data,
					.count = new_size,
					.is_zeroed = true,
				};
			}

			auto ceiled_old_size = ceil(old_size, (umm)4096);
			auto ceiled_new_size = ceil(new_size, (umm)4096);

			if (VirtualAlloc((u8 *)data + ceiled_old_size, ceiled_new_size - ceiled_old_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE)) {
				return {
					.data = data,
					.count = new_size,
					.is_zeroed = true,
				};
			}

			auto new_data = VirtualAlloc(0, ceiled_new_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
			memcpy(new_data, data, old_size);
			VirtualFree(data, 0, MEM_RELEASE);

			return {
				.data = new_data,
				.count = ceiled_new_size,
				.is_zeroed = true,
			};
		}
		case Allocator_free:
			VirtualFree(data, 0, MEM_RELEASE);
			return {};
	}
	return {};
}

Allocator os_allocator = make_allocator_from<DefaultAllocator>(0);
Allocator page_allocator = {
	.func = page_allocator_proc,
	.state = 0
};
Allocator default_allocator = os_allocator;

thread_local TemporaryAllocator temporary_allocator;

thread_local Allocator current_allocator;

void init_allocator(Allocator temporary_allocator_backup) {
	current_allocator = default_allocator;
	withs (temporary_allocator_backup) {
		TemporaryAllocator::init();
	};
}

void deinit_allocator() {
}

#endif

} // namespace tl

#if COMPILER_MSVC
#pragma warning(pop)
#endif
