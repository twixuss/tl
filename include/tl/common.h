#pragma once
#include "system.h"

#if COMPILER_MSVC
	#pragma warning(push, 0)
#endif

#if COMPILER_MSVC
	#include <intrin.h>
	#include <vcruntime_new.h>
#elif COMPILER_GCC
	#include <immintrin.h>
	#include <x86gprintrin.h>
	#include <malloc.h>
	#include <math.h>
#endif

#include <string.h>
#include <utility>
#include <type_traits>
#include <tuple>
#include <source_location>
#include <bit>

#if COMPILER_MSVC
	#pragma warning(pop)
#endif

#ifndef ASSERTION_FAILURE
#define ASSERTION_FAILURE(cause_string, expression_string, ...) debug_break()
#endif

#define assert_always(x, ...) (void)(!!(x) || ((ASSERTION_FAILURE("assert", #x __VA_OPT__(,) __VA_ARGS__)), false))

#ifndef assert
#define assert(x, ...) assert_always(x __VA_OPT__(,) __VA_ARGS__)
#endif

#if 1
// Tried to make these work with custom message, both old and new preprocessors are shit, don't work.
#define TL_assert_equal(a, b)         assert((a) == (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define TL_assert_not_equal(a, b)     assert((a) != (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define TL_assert_less(a, b)          assert((a) <  (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define TL_assert_greater(a, b)       assert((a) >  (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define TL_assert_less_equal(a, b)    assert((a) <= (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define TL_assert_greater_equal(a, b) assert((a) >= (b), "{} = {}; {} = {}. ", #a, a, #b, b)

#define TL_assert_equal_fmt(a, b, c, ...)         assert((a) == (b), "{} = {}; {} = {}. " c, #a, a, #b, b __VA_OPT__(,) __VA_ARGS__)
#define TL_assert_not_equal_fmt(a, b, c, ...)     assert((a) != (b), "{} = {}; {} = {}. " c, #a, a, #b, b __VA_OPT__(,) __VA_ARGS__)
#define TL_assert_less_fmt(a, b, c, ...)          assert((a) <  (b), "{} = {}; {} = {}. " c, #a, a, #b, b __VA_OPT__(,) __VA_ARGS__)
#define TL_assert_greater_fmt(a, b, c, ...)       assert((a) >  (b), "{} = {}; {} = {}. " c, #a, a, #b, b __VA_OPT__(,) __VA_ARGS__)
#define TL_assert_less_equal_fmt(a, b, c, ...)    assert((a) <= (b), "{} = {}; {} = {}. " c, #a, a, #b, b __VA_OPT__(,) __VA_ARGS__)
#define TL_assert_greater_equal_fmt(a, b, c, ...) assert((a) >= (b), "{} = {}; {} = {}. " c, #a, a, #b, b __VA_OPT__(,) __VA_ARGS__)

#define assert_equal(a, b, ...)         CONCAT(TL_assert_equal,         __VA_OPT__(_fmt))(a, b __VA_OPT__(,) __VA_ARGS__)
#define assert_not_equal(a, b, ...)     CONCAT(TL_assert_not_equal,     __VA_OPT__(_fmt))(a, b __VA_OPT__(,) __VA_ARGS__)
#define assert_less(a, b, ...)          CONCAT(TL_assert_less,          __VA_OPT__(_fmt))(a, b __VA_OPT__(,) __VA_ARGS__)
#define assert_greater(a, b, ...)       CONCAT(TL_assert_greater,       __VA_OPT__(_fmt))(a, b __VA_OPT__(,) __VA_ARGS__)
#define assert_less_equal(a, b, ...)    CONCAT(TL_assert_less_equal,    __VA_OPT__(_fmt))(a, b __VA_OPT__(,) __VA_ARGS__)
#define assert_greater_equal(a, b, ...) CONCAT(TL_assert_greater_equal, __VA_OPT__(_fmt))(a, b __VA_OPT__(,) __VA_ARGS__)
#else
#define assert_equal(a, b)         assert((a) == (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define assert_not_equal(a, b)     assert((a) != (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define assert_less(a, b)          assert((a) <  (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define assert_greater(a, b)       assert((a) >  (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define assert_less_equal(a, b)    assert((a) <= (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#define assert_greater_equal(a, b) assert((a) >= (b), "{} = {}; {} = {}. ", #a, a, #b, b)
#endif

#if COMPILER_MSVC
#define TL_ASSUME(x) __assume(x)
#else
#define TL_ASSUME(x) 0
#endif

#define invalid_code_path(...) (ASSERTION_FAILURE("invalid_code_path", "" __VA_OPT__(,) __VA_ARGS__), TL_ASSUME(0))
#define not_implemented(...) (ASSERTION_FAILURE("not_implemented", "" __VA_OPT__(,) __VA_ARGS__), TL_ASSUME(0))

#ifndef bounds_check
#define bounds_check(x) x
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
#define TL_LPC [[maybe_unused]] std::source_location location = std::source_location::current() // in empty parameter list (declaration)
#define TL_LPCD [[maybe_unused]] std::source_location location                                  // in empty parameter list (definition)
#define TL_LAC location                                                                         // in empty argument list
#define TL_LP , TL_LPC                                                                          // after some parameters (declaration)
#define TL_LPD , TL_LPCD                                                                        // after some parameters (definition)
#define TL_LA , TL_LAC                                                                          // after some arguments
#else
#define TL_LPC
#define TL_LPCD
#define TL_LAC
#define TL_LP
#define TL_LPD
#define TL_LA
#endif

#ifdef TL_ENABLE_TESTS
#ifndef TL_IMPL
#error TL_ENABLE_TESTS must be defined with TL_IMPL
#endif

namespace tl {
struct Test {
	char const *name;
	void (*func)();
};
Test tests_to_run[256] = {};
int tests_to_run_count = 0;

struct TestAdder {
	TestAdder const &operator+(char const *name) const {
		tests_to_run[tests_to_run_count].name = name;
		return *this;
	}
	int operator+(void (*fn)()) const {
		tests_to_run[tests_to_run_count++].func = fn;
		return 0;
	}
};
inline static constexpr TestAdder test_adder;

}

#define TL_TEST(name) static int _tl_test_##name = tl::test_adder + __FILE__ + []
#endif

#define KiB 0x400ull
#define MiB 0x100000ull
#define GiB 0x40000000ull
#define TiB 0x10000000000ull

#define breakable_scope for(bool CONCAT(_, __LINE__)=true;CONCAT(_, __LINE__);CONCAT(_, __LINE__)=false)

#define REDECLARE_VAL(name, expr) auto _##name = expr; auto name = _##name;
#define REDECLARE_REF(name, expr) auto &_##name = expr; auto &name = _##name;

// static_assert(false) is ill-formed in removed branch of constexpr if...
#define static_error_t(t, ...) static_assert(!sizeof(t*) __VA_OPT__(,) __VA_ARGS__)
#define static_error_v(v, ...) static_error_t(decltype(v) __VA_OPT__(,) __VA_ARGS__)

#define TL_DECLARE_CONCEPT(name)                                                \
	template <class T> struct S##name : std::false_type {};                     \
	template <class ...Args> struct S##name<name<Args...>> : std::true_type {}; \
	template <class T> concept C##name = S##name<std::remove_cvref_t<T>>::value

#define TL_MAKE_FIXED(T)              \
	T(const T &) = delete;            \
	T(T &&) = delete;                 \
	T &operator=(const T &) = delete; \
	T &operator=(T &&) = delete

#ifndef TL_DEBUG_ITERATORS
#define TL_DEBUG_ITERATORS TL_DEBUG
#endif

#ifdef TL_USE_CONTEXT
#define TL_GET_CURRENT(x) ::current_##x()
#define TL_GET_GLOBAL(x) ::global_##x()
#else
#define TL_GET_CURRENT(x) ::tl::current_##x
#define TL_GET_GLOBAL(x) x
#endif

#ifndef TL_ALIGN_RW_OPS
#define TL_ALIGN_RW_OPS 0
#endif

namespace tl {

inline constexpr umm string_char_count(ascii const *str) { umm result = 0; while (*str++) ++result; return result; }

inline constexpr umm string_unit_count(ascii const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_unit_count(utf8  const *str) { umm result = 0; while (*str++) ++result; return result; }
inline constexpr umm string_unit_count(utf16 const *str) { umm result = 0; while (*str++) ++result; return result;}
inline constexpr umm string_unit_count(utf32 const *str) { umm result = 0; while (*str++) ++result; return result;}

inline constexpr umm string_byte_count(ascii const *str) { return string_unit_count(str) * sizeof(ascii); }
inline constexpr umm string_byte_count(utf8  const *str) { return string_unit_count(str) * sizeof(utf8 ); }
inline constexpr umm string_byte_count(utf16 const *str) { return string_unit_count(str) * sizeof(utf16);}
inline constexpr umm string_byte_count(utf32 const *str) { return string_unit_count(str) * sizeof(utf32);}

template <class T> inline constexpr bool is_integer = std::is_integral_v<T>;
template <class T> inline constexpr bool is_integer_like = is_integer<T>;
template <class T> inline constexpr bool is_signed = std::is_signed_v<T>;
template <class T> inline constexpr bool is_unsigned = std::is_unsigned_v<T>;
template <class T> inline constexpr bool is_float = std::is_floating_point_v<T>;

struct Empty {};
constexpr bool operator==(Empty, Empty) { return true; }
constexpr bool operator!=(Empty, Empty) { return false; }

inline umm noop() { return 0; }

template <class ...T>
inline constexpr umm type_count_of = sizeof...(T);

template <u32 index, class ...Rest>
struct TypeAtT;

template <class First, class ...Rest>
struct TypeAtT<0, First, Rest...> { using Type = First; };

template <u32 index, class First, class ...Rest>
struct TypeAtT<index, First, Rest...> { using Type = typename TypeAtT<index-1, Rest...>::Type; };

template <u32 index, class ...Rest>
using TypeAt = typename TypeAtT<index, Rest...>::Type;

template <typename T, typename... Rest>
struct IndexOfT;

template <typename T, typename... Rest>
struct IndexOfT<T, T, Rest...> : std::integral_constant<std::size_t, 0> {};

template <typename T, typename U, typename... Rest>
struct IndexOfT<T, U, Rest...> : std::integral_constant<std::size_t, 1 + IndexOfT<T, Rest...>::value> {};

template <typename T, typename... Rest>
constexpr umm type_index_of = IndexOfT<T, Rest...>::value;

template <class First, class ...Rest>
concept AllSame = (std::is_same_v<First, Rest> && ...);

template <class...>
struct AllDifferentT : std::true_type {};
template <>
struct AllDifferentT<> : std::true_type {};
template <class First, class ...Rest>
struct AllDifferentT<First, Rest...> : std::conditional_t<(!std::is_same_v<First, Rest> && ...) && AllDifferentT<Rest...>::value, std::true_type, std::false_type> {};

template <class ...Ts>
concept AllDifferent = AllDifferentT<Ts...>::value;

template <class First, class ...Rest>
struct RequireAllSame {
	static_assert(AllSame<First, Rest...>);
	using Type = First;
};

template <class T, class ...Types>
concept OneOf = (std::is_same_v<T, Types> || ...);

template <class Container>
struct ElementOfT {
	using Type = typename Container::Element;
};

template <class T, umm count>
struct ElementOfT<T[count]> {
	using Type = T;
};

template <class T>
struct ElementOfT<T *> {
	using Type = T;
};

template <class T>
using ElementOf = ElementOfT<std::remove_cvref_t<T>>::Type;

template <class T, bool add_const>
struct MakeConstT {
	using Type = const T;
};
template <class T>
struct MakeConstT<T, false> {
	using Type = T;
};

template <class T, bool add_const = true>
using MakeConst = MakeConstT<T, add_const>::Type;

template <class ...Ts>
struct TypePackIterator {
	auto operator->*(auto fn) {
		[&] <umm ...I>(std::index_sequence<I...>) {
			((fn.template operator()<I, TypeAt<I, Ts...>>()), ...);
		}(std::make_index_sequence<sizeof...(Ts)>{});
	}
};

#define for_each_type(I, T, Ts) TypePackIterator<Ts...>{} ->* [&]<umm I, class T>()

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

template<> inline constexpr f32 min_value<f32> = -3.402823466e+38f;
template<> inline constexpr f32 max_value<f32> = +3.402823466e+38f;

template<> inline constexpr f64 min_value<f64> = -1.7976931348623158e+308;
template<> inline constexpr f64 max_value<f64> = +1.7976931348623158e+308;

template <class T> inline constexpr T epsilon = {};
template<> inline constexpr f32 epsilon<f32> = 1.175494351e-38f;
template<> inline constexpr f64 epsilon<f64> = 2.2250738585072014e-308;

constexpr struct {
	template <class T>
	constexpr operator T() const { 
		static_error_t(T, "value for this type is undefined");
	}
} undefined_constexpr_value;

template <class T> inline constexpr T infinity = undefined_constexpr_value;
#if COMPILER_MSVC
template<> inline constexpr f32 infinity<f32> = 1e300 * 1e300;
template<> inline constexpr f64 infinity<f64> = 1e300 * 1e300;
#else
template<> inline constexpr f32 infinity<f32> = (__builtin_inff ());
template<> inline constexpr f64 infinity<f64> = (__builtin_inff ());
#endif

template <class T> inline constexpr T nan = undefined_constexpr_value;
#if COMPILER_MSVC
template <> inline constexpr f32 nan<f32> = infinity<f32> * 0;
template <> inline constexpr f64 nan<f64> = infinity<f64> * 0;
#else
template <> inline constexpr f32 nan<f32> = __builtin_nanf("");
template <> inline constexpr f64 nan<f64> = __builtin_nan("");
#endif

#pragma warning(pop)

#define tl_enum_flags(t) \
forceinline constexpr t operator~(t a){using u=std::underlying_type_t<t>;return (t)~(u)a;} \
forceinline constexpr t operator|(t a,t b){using u=std::underlying_type_t<t>;return (t)((u)a|(u)b);} \
forceinline constexpr t operator&(t a,t b){using u=std::underlying_type_t<t>;return (t)((u)a&(u)b);} \

template <class T>
concept AFundamentalType = std::is_fundamental_v<T>;

template <class T>
concept APodType = std::is_pod_v<T>;

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


template <class Fn, class Ret, class ...Args>
concept Callable = requires(Fn fn) { { fn(std::declval<Args>()...) } -> std::same_as<Ret>; };

template <class Fn, class ...Args>
concept CallableAnyRet = requires(Fn fn) { fn(std::declval<Args>()...); };

template <class T, class ...Args>
concept APredicate = requires (T t, Args ...args) { 
	{ t(args...) } -> std::same_as<bool>;
};

constexpr auto predicate_equal = []<class T, class U>(T const &a, U const &b) { return a == b; };


// This function exists because C++ does not provide a way to convert
// fundamental types to structs without adding a constructor to a struct, 
// which will break designated initialization.
// If there's a need to do that, specialize this function.
template <class To, class From>
forceinline constexpr void convert(To &to, From from) {
	to = (To)from;
}
template <class To, class From>
forceinline constexpr To convert(From from) {
	if constexpr (requires {(To)from;}) {
		return (To)from;
	} else {
		To to;
		convert(to, from);
		return to;
	}
}

forceinline constexpr smm compare(bool a, bool b) { return (s8)a - (s8)b; }
forceinline constexpr smm compare(u8  a, u8  b) { return (s8 )a - (s8 )b; }
forceinline constexpr smm compare(u16 a, u16 b) { return (s16)a - (s16)b; }
forceinline constexpr smm compare(u32 a, u32 b) { return (s32)a - (s32)b; }
forceinline constexpr smm compare(u64 a, u64 b) { return (s64)a - (s64)b; }
forceinline constexpr smm compare(s8  a, s8  b) { return a - b; }
forceinline constexpr smm compare(s16 a, s16 b) { return a - b; }
forceinline constexpr smm compare(s32 a, s32 b) { return a - b; }
forceinline constexpr smm compare(s64 a, s64 b) { return a - b; }
forceinline constexpr smm compare(f32 a, f32 b) { return a < b ? -1 : a > b ? 1 : 0; }
forceinline constexpr smm compare(f64 a, f64 b) { return a < b ? -1 : a > b ? 1 : 0; }

template <class T>
forceinline constexpr smm compare(T a, T b) {
	if constexpr (requires { { a - b } -> std::signed_integral; }) {
		return a - b;
	} else {
		return a < b ? -1 : a > b ? 1 : 0;
	}
}

inline static constexpr auto default_comparer = []<class T>(T a, T b) { return compare(a, b); };

template <class Compare, class T>
concept ACompare = requires (Compare compare, T t) {
	{ compare(t, t) } -> std::signed_integral;
};

template <class Compare, class T>
concept ACompare1 = requires (Compare compare, T t) {
	{ compare(t) } -> std::signed_integral;
};

forceinline bool all(bool v) { return v; }
forceinline bool any(bool v) { return v; }

namespace ce {

forceinline constexpr void add_carry(u8 a, u8 b, bool carry_in, u8 *result, bool *carry_out) {
	u16 r = a + b + carry_in;
	*result = (u8)r;
	*carry_out = r & 0x100;
}
forceinline constexpr void add_carry(u16 a, u16 b, bool carry_in, u16 *result, bool *carry_out) {
	u32 r = a + b + carry_in;
	*result = (u16)r;
	*carry_out = r & 0x10000;
}
forceinline constexpr void add_carry(u32 a, u32 b, bool carry_in, u32 *result, bool *carry_out) {
	u64 r = (u64)a + b + carry_in;
	*result = (u32)r;
	*carry_out = r & 0x100000000;
}
forceinline constexpr void add_carry(u64 a, u64 b, bool carry_in, u64 *result, bool *carry_out) {
	*result = a + b + carry_in;
	if (a == 0xffff'ffff'ffff'ffff && (b || carry_in)) {
		*carry_out = true;
	} else {
		*carry_out = a + carry_in > 0xffff'ffff'ffff'ffff - b;
	}
}

forceinline constexpr void sub_borrow(u8 a, u8 b, bool carry_in, u8 *result, bool *carry_out) {
	auto r = (u16)a - b - carry_in;
	*result = (u8)r;
	*carry_out = r & 0x100;
}
forceinline constexpr void sub_borrow(u16 a, u16 b, bool carry_in, u16 *result, bool *carry_out) {
	auto r = (u32)a - b - carry_in;
	*result = (u16)r;
	*carry_out = r & 0x10000;
}
forceinline constexpr void sub_borrow(u32 a, u32 b, bool carry_in, u32 *result, bool *carry_out) {
	auto r = (u64)a - b - carry_in;
	*result = (u32)r;
	*carry_out = r & 0x100000000;
}
forceinline constexpr void sub_borrow(u64 a, u64 b, bool carry_in, u64 *result, bool *carry_out) {
	*result = a - b - carry_in;
	if (b == 0xffff'ffff'ffff'ffff) {
		*carry_out = b > a - carry_in;
	} else {
		*carry_out = b + carry_in > a;
	}
}

}

forceinline constexpr void add_carry(u8  a, u8  b, bool carry_in, u8  *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::add_carry(a, b, carry_in, result, carry_out);
	} else {
		#if COMPILER_MSVC
			*carry_out = (bool)_addcarry_u8(carry_in, a, b, result);
		#else COMPILER_GCC
			ce::add_carry(a, b, carry_in, result, carry_out);
		#endif
	}
 }
forceinline constexpr void add_carry(u16 a, u16 b, bool carry_in, u16 *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::add_carry(a, b, carry_in, result, carry_out);
	} else {
		#if COMPILER_MSVC
			*carry_out = (bool)_addcarry_u16(carry_in, a, b, result);
		#else COMPILER_GCC
			ce::add_carry(a, b, carry_in, result, carry_out);
		#endif
	}
}
forceinline constexpr void add_carry(u32 a, u32 b, bool carry_in, u32 *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::add_carry(a, b, carry_in, result, carry_out);
	} else {
		*carry_out = (bool)_addcarry_u32(carry_in, a, b, result);
	}
}
forceinline constexpr void add_carry(u64 a, u64 b, bool carry_in, u64 *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::add_carry(a, b, carry_in, result, carry_out);
	} else {
		#if ARCH_X64
			*carry_out = (bool)_addcarry_u64(carry_in, a, b, (unsigned long long *)result);
		#else
			ce::add_carry(a, b, carry_in, result, carry_out);
		#endif
	}
}

forceinline constexpr void sub_borrow(u8  a, u8  b, bool carry_in, u8  *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::sub_borrow(a, b, carry_in, result, carry_out);
	} else {
		#if COMPILER_MSVC
			*carry_out = (bool)_subborrow_u8(carry_in, a, b, result);
		#else COMPILER_GCC
			ce::sub_borrow(a, b, carry_in, result, carry_out);
		#endif
	}
 }
forceinline constexpr void sub_borrow(u16 a, u16 b, bool carry_in, u16 *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::sub_borrow(a, b, carry_in, result, carry_out);
	} else {
		#if COMPILER_MSVC
			*carry_out = (bool)_subborrow_u16(carry_in, a, b, result);
		#else COMPILER_GCC
			ce::sub_borrow(a, b, carry_in, result, carry_out);
		#endif
	}
}
forceinline constexpr void sub_borrow(u32 a, u32 b, bool carry_in, u32 *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::sub_borrow(a, b, carry_in, result, carry_out);
	} else {
		*carry_out = (bool)_subborrow_u32(carry_in, a, b, result);
	}
}
forceinline constexpr void sub_borrow(u64 a, u64 b, bool carry_in, u64 *result, bool *carry_out) {
	if (std::is_constant_evaluated()) {
		ce::sub_borrow(a, b, carry_in, result, carry_out);
	} else {
		#if ARCH_X64
			*carry_out = (bool)_subborrow_u64(carry_in, a, b, (unsigned long long *)result);
		#else
			ce::sub_borrow(a, b, carry_in, result, carry_out);
		#endif
	}
}

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
	forceinline u32 find_lowest_one_bit(u32 val) { return val ? __builtin_ffs(val) : ~0; }
	forceinline u32 find_lowest_one_bit(u64 val) { return val ? __builtin_ffsll(val) : ~0; }
	forceinline u32 find_highest_one_bit(u32 val) { return val ? 32 - __builtin_clz(val) : ~0; }
	forceinline u32 find_highest_one_bit(u64 val) { return val ? 64 - __builtin_clzll(val) : ~0; }
#elif COMPILER_MSVC
	forceinline u32 find_lowest_one_bit(u32 val) { unsigned long result; return _BitScanForward(&result, (unsigned long)val) ? (u32)result : ~0; }
	forceinline u32 find_highest_one_bit(u32 val) { unsigned long result; return _BitScanReverse(&result, (unsigned long)val) ? (u32)result : ~0; }
	#if ARCH_X64
		forceinline u32 find_lowest_one_bit(u64 val) { unsigned long result; return _BitScanForward64(&result, val) ? (u32)result : ~0; }
		forceinline u32 find_highest_one_bit(u64 val) { unsigned long result; return _BitScanReverse64(&result, val) ? (u32)result : ~0; }
	#endif
#endif
forceinline u32 find_lowest_one_bit(u8  val) { return find_lowest_one_bit((u32)val); }
forceinline u32 find_lowest_one_bit(u16 val) { return find_lowest_one_bit((u32)val); }
forceinline u32 find_highest_one_bit(u8  val) { return find_highest_one_bit((u32)val); }
forceinline u32 find_highest_one_bit(u16 val) { return find_highest_one_bit((u32)val); }
forceinline u32 find_lowest_zero_bit(u8  val) { return find_lowest_one_bit((u32)~val); }
forceinline u32 find_lowest_zero_bit(u16 val) { return find_lowest_one_bit((u32)~val); }
forceinline u32 find_lowest_zero_bit(u32 val) { return find_lowest_one_bit(~val); }
forceinline u32 find_lowest_zero_bit(u64 val) { return find_lowest_one_bit(~val); }
forceinline u32 find_highest_zero_bit(u8  val) { return find_highest_one_bit((u32)(u8 )~val); }
forceinline u32 find_highest_zero_bit(u16 val) { return find_highest_one_bit((u32)(u16)~val); }
forceinline u32 find_highest_zero_bit(u32 val) { return find_highest_one_bit(~val); }
forceinline u32 find_highest_zero_bit(u64 val) { return find_highest_one_bit(~val); }

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

template <class T>
inline constexpr void Swap(T &a, T &b) {
	T t = a;
	a = b;
	b = t;
}

template <class T, umm count>
constexpr umm index_of(T (&array)[count], T *pointer) {
	return pointer - array;
}

template <class T, umm count>
constexpr bool owns(T (&array)[count], T *pointer) {
	return (umm)(pointer - array) < count;
}

// NOTE: have to use const & to pass arrays, otherwise they rot.
template <class T> forceinline constexpr auto min(T const &a) { return a; }
template <class T> forceinline constexpr auto max(T const &a) { return a; }
template <class T> forceinline constexpr auto min(T const &a, T const &b) { return select(a < b, a, b); }
template <class T> forceinline constexpr auto max(T const &a, T const &b) { return select(a > b, a, b); }

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

template <class T, class ...Rest>
forceinline constexpr auto min(T first, Rest const &...rest) requires (sizeof...(Rest) > 1) && AllSame<T, Rest...> {
	return ((first = min(first, rest)), ...);
}

template <class T, class ...Rest>
forceinline constexpr auto max(T first, Rest const &...rest) requires (sizeof...(Rest) > 1) && AllSame<T, Rest...> {
	return ((first = max(first, rest)), ...);
}

template <class T> forceinline constexpr void sort_values(T& mn, T &mx) { if (mn > mx) { Swap(mn, mx); }; }

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
		return select(v < 0, (v + 1) % s + s - 1, v % s);
	} else {
		return v % s;
	}
}

forceinline f32 log(f32 x, f32 base) {
	return ::log2f(x) / ::log2f(base);
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

// Hacker's delight - FIGURE 5–24. Number of trailing zeros, Gaudet’s algorithm
constexpr u8 count_trailing_zeros(u8 x) {
	u8 y = x & (u8)-(s8)x;
	u32 bz = !y;
	u32 b2 = (y & 0x0F) ? 0 : 4;
	u32 b1 = (y & 0x33) ? 0 : 2;
	u32 b0 = (y & 0x55) ? 0 : 1;
	return (u8)(bz + b2 + b1 + b0);
}
constexpr u16 count_trailing_zeros(u16 x) {
	u16 y = x & (u16)-(s16)x;
	u32 bz = !y;
	u32 b3 = (y & 0x00FF) ? 0 : 8;
	u32 b2 = (y & 0x0F0F) ? 0 : 4;
	u32 b1 = (y & 0x3333) ? 0 : 2;
	u32 b0 = (y & 0x5555) ? 0 : 1;
	return (u16)(bz + b3 + b2 + b1 + b0);
}
constexpr u32 count_trailing_zeros(u32 x) {
	u32 y = x & (u32)-(s32)x;
	u32 bz = !y;
	u32 b4 = (y & 0x0000FFFF) ? 0 : 16;
	u32 b3 = (y & 0x00FF00FF) ? 0 : 8;
	u32 b2 = (y & 0x0F0F0F0F) ? 0 : 4;
	u32 b1 = (y & 0x33333333) ? 0 : 2;
	u32 b0 = (y & 0x55555555) ? 0 : 1;
	return bz + b4 + b3 + b2 + b1 + b0;
}
constexpr u32 count_trailing_zeros(u64 x) {
	u64 y = x & (u64)-(s64)x;
	u32 bz = !y;
	u32 b5 = (y & 0x00000000FFFFFFFF) ? 0 : 32;
	u32 b4 = (y & 0x0000FFFF0000FFFF) ? 0 : 16;
	u32 b3 = (y & 0x00FF00FF00FF00FF) ? 0 : 8;
	u32 b2 = (y & 0x0F0F0F0F0F0F0F0F) ? 0 : 4;
	u32 b1 = (y & 0x3333333333333333) ? 0 : 2;
	u32 b0 = (y & 0x5555555555555555) ? 0 : 1;
	return bz + b5 + b4 + b3 + b2 + b1 + b0;
}
}

#if COMPILER_MSVC
#if ARCH_LZCNT
forceinline constexpr u32 count_leading_zeros(u8  v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt16(v) - 8; }
forceinline constexpr u32 count_leading_zeros(u16 v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt16(v); }
forceinline constexpr u32 count_leading_zeros(u32 v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt32(v); }
forceinline constexpr u32 count_leading_zeros(u64 v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : (u32)__lzcnt64(v); }
#else
forceinline constexpr u32 count_leading_zeros(u8  v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 8  : (_BitScanReverse(&r, v),  7 - r))); }
forceinline constexpr u32 count_leading_zeros(u16 v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 16 : (_BitScanReverse(&r, v), 15 - r))); }
forceinline constexpr u32 count_leading_zeros(u32 v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 32 : (_BitScanReverse(&r, v), 31 - r))); }
#if ARCH_X64
forceinline constexpr u32 count_leading_zeros(u64 v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_leading_zeros(v) : ((v == 0) ? 64 : (_BitScanReverse64(&r, v), 63 - r))); }
#endif
#endif
#elif COMPILER_GCC
forceinline constexpr u32 count_leading_zeros(u8  v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt16(v) - 8; }
forceinline constexpr u32 count_leading_zeros(u16 v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt16(v); }
forceinline constexpr u32 count_leading_zeros(u32 v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : __lzcnt32(v); }
forceinline constexpr u32 count_leading_zeros(u64 v) { return std::is_constant_evaluated() ? ce::count_leading_zeros(v) : (u32)__lzcnt64(v); }
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

#if COMPILER_MSVC
forceinline constexpr u32 count_trailing_zeros(u8  v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 8  : (_BitScanForward(&r, v), r))); }
forceinline constexpr u32 count_trailing_zeros(u16 v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 16 : (_BitScanForward(&r, v), r))); }
forceinline constexpr u32 count_trailing_zeros(u32 v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 32 : (_BitScanForward(&r, v), r))); }
forceinline constexpr u32 count_trailing_zeros(u64 v) { unsigned long r; return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 64 : (_BitScanForward64(&r, v), r))); }
#else
forceinline constexpr u32 count_trailing_zeros(u8  v) { return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 8  : (__builtin_ffs  (v)))); }
forceinline constexpr u32 count_trailing_zeros(u16 v) { return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 16 : (__builtin_ffs  (v)))); }
forceinline constexpr u32 count_trailing_zeros(u32 v) { return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 32 : (__builtin_ffs  (v)))); }
forceinline constexpr u32 count_trailing_zeros(u64 v) { return (std::is_constant_evaluated() ? ce::count_trailing_zeros(v) : ((v == 0) ? 64 : (__builtin_ffsll(v)))); }
#endif

forceinline constexpr u32 count_trailing_ones(u8  val) { return count_trailing_zeros((u8 )~val); }
forceinline constexpr u32 count_trailing_ones(u16 val) { return count_trailing_zeros((u16)~val); }
forceinline constexpr u32 count_trailing_ones(u32 val) { return count_trailing_zeros((u32)~val); }
forceinline constexpr u32 count_trailing_ones(u64 val) { return count_trailing_zeros((u64)~val); }

forceinline constexpr u32 log2(u8  v) { return 7  - count_leading_zeros(v); }
forceinline constexpr u32 log2(u16 v) { return 15 - count_leading_zeros(v); }
forceinline constexpr u32 log2(u32 v) { return 31 - count_leading_zeros(v); }
#if ARCH_X64
forceinline constexpr u32 log2(u64 v) { return 63 - count_leading_zeros(v); }
#endif

forceinline constexpr u32 log2(s8  v) { return log2((u8 )v); }
forceinline constexpr u32 log2(s16 v) { return log2((u16)v); }
forceinline constexpr u32 log2(s32 v) { return log2((u32)v); }
#if ARCH_X64
forceinline constexpr u32 log2(s64 v) { return log2((u64)v); }
#endif

forceinline constexpr u32 ceiled_log2(u8  v) { return log2(v - 1) + 1; }
forceinline constexpr u32 ceiled_log2(u16 v) { return log2(v - 1) + 1; }
forceinline constexpr u32 ceiled_log2(u32 v) { return log2(v - 1) + 1; }
forceinline constexpr u32 ceiled_log2(u64 v) { return log2(v - 1) + 1; }

forceinline constexpr u8  floor_to_power_of_2(u8  v) { return v == 0 ? (u8 )0 : (u8 )((u8 )1 << (u8 )log2(v)); }
forceinline constexpr u16 floor_to_power_of_2(u16 v) { return v == 0 ? (u16)0 : (u16)((u16)1 << (u16)log2(v)); }
forceinline constexpr u32 floor_to_power_of_2(u32 v) { return v == 0 ? (u32)0 : (u32)((u32)1 << (u32)log2(v)); }
#if ARCH_X64
forceinline constexpr u64 floor_to_power_of_2(u64 v) { return v == 0 ? (u64)0 : (u64)((u64)1 << (u64)log2(v)); }
#endif

namespace ce {

inline constexpr u8 floor_to_power_of_2(u8 x) {
	x |= (u8)(x >> 1);
	x |= (u8)(x >> 2);
	x |= (u8)(x >> 4);
	return (u8)(x - (u8)(x >> 1));
}
inline constexpr u16 floor_to_power_of_2(u16 x) {
	x |= (u16)(x >> 1);
	x |= (u16)(x >> 2);
	x |= (u16)(x >> 4);
	x |= (u16)(x >> 8);
	return (u16)(x - (u16)(x >> 1));
}
inline constexpr u32 floor_to_power_of_2(u32 x) {
	x |= (u32)(x >> 1);
	x |= (u32)(x >> 2);
	x |= (u32)(x >> 4);
	x |= (u32)(x >> 8);
	x |= (u32)(x >> 16);
	return (u32)(x - (u32)(x >> 1));
}
inline constexpr u64 floor_to_power_of_2(u64 x) {
	x |= (u64)(x >> 1);
	x |= (u64)(x >> 2);
	x |= (u64)(x >> 4);
	x |= (u64)(x >> 8);
	x |= (u64)(x >> 16);
	x |= (u64)(x >> 32);
	return (u64)(x - (u64)(x >> 1));
}

inline constexpr u64 rotate_left(u64 x, u64 bits) {
	return (x << bits) | (x >> (64 - bits));
}

}

forceinline constexpr u8  ceil_to_power_of_2(u8 v)  { return v == 0 ? 0 : (u8 )1 << (u8 )(8  - count_leading_zeros((u8 )(v - 1))); }
forceinline constexpr u16 ceil_to_power_of_2(u16 v) { return v == 0 ? 0 : (u16)1 << (u16)(16 - count_leading_zeros((u16)(v - 1))); }
forceinline constexpr u32 ceil_to_power_of_2(u32 v) { return v == 0 ? 0 : (u32)1 << (u32)(32 - count_leading_zeros((u32)(v - 1))); }
forceinline constexpr u64 ceil_to_power_of_2(u64 v) { return v == 0 ? 0 : (u64)1 << (u64)(64 - count_leading_zeros((u64)(v - 1))); }

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

forceinline constexpr f32 floor(f32 v, f32 s) { return floor(v / s) * s; }

forceinline s32 floor_to_int(f32 v) { return (s32)floor(v); }
forceinline s64 floor_to_int(f64 v) { return (s64)floor(v); }

forceinline f32 ceil(f32 v) { return ::ceilf(v); }
forceinline f64 ceil(f64 v) { return ::ceil(v); }
forceinline s32 ceil_to_int(f32 v) { return (s32)ceil(v); }
forceinline s64 ceil_to_int(f64 v) { return (s64)ceil(v); }

forceinline f32 round(f32 v) { return ::roundf(v); }
forceinline f64 round(f64 v) { return ::round(v); }
forceinline s32 round_to_int(f32 v) { return (s32)lroundf(v); }
forceinline s64 round_to_int(f64 v) { return llround(v); }

forceinline f32 frac(f32 v) { return v - floor(v); }
forceinline f64 frac(f64 v) { return v - floor(v); }

forceinline f32 pow(f32 x, f32 y) { return ::powf(x, y); }
forceinline f64 pow(f64 x, f64 y) { return ::pow(x, y); }

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
forceinline constexpr u8  rotate_left (u8  v, s32 shift = 1) { return (v << shift) | (v >> ( 8 - shift)); }
forceinline constexpr u16 rotate_left (u16 v, s32 shift = 1) { return (v << shift) | (v >> (16 - shift)); }
forceinline constexpr u32 rotate_left (u32 v, s32 shift = 1) { return (v << shift) | (v >> (32 - shift)); }
forceinline constexpr u64 rotate_left (u64 v, s32 shift = 1) { return (v << shift) | (v >> (64 - shift)); }
forceinline constexpr u8  rotate_right(u8  v, s32 shift = 1) { return (v >> shift) | (v << ( 8 - shift)); }
forceinline constexpr u16 rotate_right(u16 v, s32 shift = 1) { return (v >> shift) | (v << (16 - shift)); }
forceinline constexpr u32 rotate_right(u32 v, s32 shift = 1) { return (v >> shift) | (v << (32 - shift)); }
forceinline constexpr u64 rotate_right(u64 v, s32 shift = 1) { return (v >> shift) | (v << (64 - shift)); }
#else
forceinline constexpr u8  rotate_left (u8  v, s32 shift = 1) { return std::is_constant_evaluated() ? (v << shift) | (v >> (8  - shift)) : _rotl8(v, (u8)shift); }
forceinline constexpr u16 rotate_left (u16 v, s32 shift = 1) { return std::is_constant_evaluated() ? (v << shift) | (v >> (16 - shift)) : _rotl16(v, (u8)shift); }
forceinline constexpr u32 rotate_left (u32 v, s32 shift = 1) { return std::is_constant_evaluated() ? (v << shift) | (v >> (32 - shift)) : _rotl(v, shift); }
forceinline constexpr u64 rotate_left (u64 v, s32 shift = 1) { return std::is_constant_evaluated() ? (v << shift) | (v >> (64 - shift)) : _rotl64(v, shift); }
forceinline constexpr u8  rotate_right(u8  v, s32 shift = 1) { return std::is_constant_evaluated() ? (v >> shift) | (v << (8  - shift)) : _rotr8(v, (u8)shift); }
forceinline constexpr u16 rotate_right(u16 v, s32 shift = 1) { return std::is_constant_evaluated() ? (v >> shift) | (v << (16 - shift)) : _rotr16(v, (u8)shift); }
forceinline constexpr u32 rotate_right(u32 v, s32 shift = 1) { return std::is_constant_evaluated() ? (v >> shift) | (v << (32 - shift)) : _rotr(v, shift); }
forceinline constexpr u64 rotate_right(u64 v, s32 shift = 1) { return std::is_constant_evaluated() ? (v >> shift) | (v << (64 - shift)) : _rotr64(v, shift); }
#endif

constexpr f32 pi     = f32(3.1415926535897932384626433832795L);
constexpr f32 tau    = f32(6.283185307179586476925286766559L);
constexpr f32 inv_pi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2  = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3  = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5  = f32(2.2360679774997896964091736687313L);
constexpr f32 golden_ratio = f32(1.6180339887498948482045868343656L);

template <class T> forceinline constexpr auto radians(T deg) { return deg * (pi / 180.0f); }
template <class T> forceinline constexpr auto degrees(T rad) { return rad * (180.0f / pi); }

constexpr f32 sqrt_newton_raphson(f32 x, f32 curr, f32 prev) {
    return curr == prev ? curr : sqrt_newton_raphson(x, 0.5f * (curr + x / curr), curr);
};

forceinline constexpr f32 sqrt(f32 v) { return std::is_constant_evaluated() ? sqrt_newton_raphson(v, v, 0) : sqrtf(v); }

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

template <class Value, class FromMin, class FromMax, class ToMin, class ToMax>
forceinline constexpr auto map_relaxed(Value value, FromMin source_min, FromMax source_max, ToMin dest_min, ToMax dest_max) {
	if constexpr (is_integer_like<Value>) { // Do multiplication first
		return (value - source_min) * (dest_max - dest_min) / (source_max - source_min) + dest_min;
	} else {
		return (value - source_min) / (source_max - source_min) * (dest_max - dest_min) + dest_min;
	}
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
constexpr T midpoint(T a, T b) {
	sort_values(a, b);
	return a + (b - a) / 2;
}

template <umm bits>
using UintWithBits = TypeAt<log2(ceil_to_power_of_2(bits) / 8), u8, u16, u32, u64>;

template <umm count>
using UintForCount = TypeAt<log2(log2(max(count, (umm)255))) - 2, u8, u16, u32, u64>;

template <class T>
constexpr T *midpoint(T *a, T *b) {
	sort_values(a, b);
	return a + ((umm)(b - a) >> 1);
}

template <class T>
struct AutoCastable {
#pragma warning(suppress: 4180) // const is meaningless for function types. No idea why and I don't care.
	T const &value;
	template <class U>
	forceinline constexpr operator U() {
		return (U)value;
	}
};

struct AutoCaster {
	template <class T>
	forceinline constexpr auto operator->*(T const &value) {
		return AutoCastable{value};
	}
};

// NOTE: MSVC breaks when this is used in initial value of a struct member.
// This makes the struct not default-constructible, and when you try to default-construct it
// the error is not about that, but a syntax one about unexpected '(' or something. It's strange.
#define autocast AutoCaster{} ->* 

template <class... Callables>
struct Combine : public Callables... {
	inline constexpr Combine(Callables &&... c) : Callables(std::move(c))... {}
	using Callables::operator()...;
};

template <class T>
struct Repeat {
	T value;
	umm count;
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

template <class From, class To>
concept ExplicitlyConvertibleTo = std::is_constructible_v<To, From> && !std::is_convertible_v<From, To>;

template <class Iter>
concept AnIter = requires(Iter iter) {
	{ iter } -> ExplicitlyConvertibleTo<bool>;
	{ iter.next() } -> std::same_as<void>;
	iter.value();
};

template <class Collection>
struct IterOfT {
	using Type = typename Collection::Iter;
};

template <class Collection>
using IterOf = IterOfT<std::remove_cvref_t<Collection>>::Type;

template <class Collection>
auto to_iter(Collection &&collection)
	requires requires { { collection.iter() } -> AnIter; }
{
	return collection.iter();
}

template <class Collection>
auto to_iter(Collection &&collection, typename std::remove_cvref_t<Collection>::IterOptions options)
	requires requires { { collection.iter(options) } -> AnIter; }
{
	return collection.iter(options);
}

template <AnIter Iter>
auto to_iter(Iter &&iter) {
	return iter;
}

template <class T>
concept Collection = requires(T collection) {
	{ to_iter(collection) } -> AnIter;
};

#define foreach(it, collection, ...) for (auto it = to_iter(collection __VA_OPT__(,) __VA_ARGS__); it; it.next())

enum ForEachDirective {
	ForEach_continue        = 0x0,
	ForEach_break           = 0x1,
	ForEach_erase           = 0x2, // ordered
	ForEach_erase_unordered = 0x4,
	ForEach_erase_mask      = 0x6,
	ForEach_dont_recurse    = 0x8,
};

constexpr ForEachDirective operator|(ForEachDirective a, ForEachDirective b) { return (ForEachDirective)(to_underlying(a) | to_underlying(b)); }

// TODO: use this in all of for_each functions, DRY
template <class ...Args, class Fn>
auto wrap_foreach_fn(Fn &&fn) {
	using FnRet = std::invoke_result_t<std::remove_cvref_t<Fn>, Args...>;
	if constexpr (std::is_same_v<FnRet, void>) {
		return [&] (auto &&...args) {
			fn(args...);
			return ForEach_continue;
		};
	} else if constexpr (std::is_same_v<FnRet, ForEachDirective>) {
		return fn;
	} else {
		static_error_v(fn, "Invalid return type of for_each function");
	}
}

template <class T, class ...Args>
concept ForEachIterator = requires(T fn, Args ...args) {
	requires OneOf<decltype(fn(args...)), void, ForEachDirective>;
};

using ForEachFlags = u8;
enum : ForEachFlags {
	ForEach_reverse = 0x1,
};

template <class Fn, class Item>
auto for_each_default_action(Fn &&fn, Item &&item) {
	if constexpr (std::is_same_v<decltype(fn(item)), ForEachDirective>) {
		return ForEach_continue;
	} else {
		return;
	}
}

#define tl_self_const std::is_const_v<std::remove_reference_t<decltype(self)>>


template <class Iter>
struct IterAdapter {
	Iter iter;

	auto &operator*() {
		return *iter;
	}
	auto &operator++() {
		iter.next();
		return *this;
	}
	bool operator!=(Empty) {
		return !!iter;
	}
};

constexpr umm count(Collection auto const &collection) {
	if constexpr (requires { { collection.count } -> std::integral; }) {
		return collection.count;
	} else if constexpr (requires { { collection.count() } -> std::integral; }) {
		return collection.count();
	} else if constexpr (std::is_array_v<std::remove_cvref_t<decltype(collection)>>) {
		return sizeof(collection) / sizeof(collection[0]);
	} else {
		umm result = 0;
		foreach(it, collection) {
			++result;
		}
		return result;
	}
}

template <Collection Collection, APredicate<ElementOf<Collection>> Predicate>
constexpr umm count(Collection const &collection, Predicate predicate) {
	umm result = 0;
	foreach(it, collection) {
		result += predicate(it.value());
	}
	return result;
}

constexpr auto identity_value = [] (auto &&x) -> decltype(auto) {
	return x;
};

template <Collection Collection, class Mapper = decltype(identity_value)>
auto sum(Collection const &collection, Mapper &&mapper = identity_value) {
	std::remove_cvref_t<decltype(mapper(*(ElementOf<Collection> *)0))> result = {};
	foreach (it, collection)
		result += mapper(*it);
	return result;
}

template <Collection Collection>
auto stddev(Collection const &collection) {
	using Element = ElementOf<Collection>;

	auto count = count_of(collection);
	Element average = {};

	foreach (it, collection) average += *it;
	average /= count;

	Element variance = {};
	foreach (it, collection) variance += pow2(*it - average);
	variance /= count;

	return tl::sqrt(variance);
}

template <class Predicate = decltype(identity_value)>
bool all(Collection auto collection, Predicate predicate = {}) {
	foreach (it, collection) {
		if (!predicate(*it))
			return false;
	}
	return true;
}

template <class Predicate = decltype(identity_value)>
bool any(Collection auto collection, Predicate predicate = {}) {
	foreach (it, collection) {
		if (predicate(*it))
			return true;
	}
	return false;
}

template <Collection Collection>
constexpr auto find_if(Collection const &collection, auto &&predicate) -> decltype(iter(collection).pointer()) {
	foreach (it, collection) {
		if (predicate(*it)) {
			return it.pointer();
		}
	}
	return {};
}

template <class Iterator>
constexpr Iterator find_if(Iterator begin, Iterator end, APredicate<decltype(*begin)> auto &&predicate) {
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
		if (all(*it == value)) {
			return it;
		}
	}
	return 0;
}
template <class T, class Iterator>
constexpr Iterator find(Iterator begin, Iterator end, decltype(*begin) const &value, APredicate<T, T> auto &&predicate) {
	for (Iterator it = begin; it != end; ++it) {
		if (predicate(*it, value)) {
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

template <class Fn>
struct OffableDeferrer {
	inline OffableDeferrer(Fn &&fn) : fn(std::move(fn)) {}
	inline ~OffableDeferrer() noexcept(false) { if (enabled) fn(); }

	bool enabled = true;

private:
	Fn fn;
#if COMPILER_MSVC
#pragma warning(suppress: 4626)
};
#else
};
#endif

#define offable_defer(name) ::tl::OffableDeferrer name = [&]

#define scoped_replace(dst, src) \
	auto CONCAT(old_, __LINE__) = dst; \
	dst = src; \
	defer { dst = CONCAT(old_, __LINE__); }

#define scoped_replace_if(dst, src, condition) \
	Optional<decltype(dst)> CONCAT(old_, __LINE__); \
	if (condition) { CONCAT(old_, __LINE__) = dst; dst = src; } \
	defer { if (CONCAT(old_, __LINE__)) dst = CONCAT(old_, __LINE__).value(); }

#define scoped_exchange(dst, src) \
	Swap(dst, src);               \
	defer { Swap(dst, src); }

template <class T>
constexpr auto reversed(T x) {
	struct Range {
		constexpr auto begin() { return _begin; }
		constexpr auto end() { return _end; }
		decltype(x.rbegin()) _begin;
		decltype(x.rend()) _end;
	};
	Range r = {
		x.rbegin(),
		x.rend(),
	};
	return r;
}

template <class Iterator>
struct ReverseIterator {
	using Element = decltype(*Iterator{});

	Iterator it;
	constexpr ReverseIterator(Iterator it) : it(it) {}
	constexpr ReverseIterator &operator++() { return --it, *this; }
	constexpr ReverseIterator operator++(int) { auto temp = *this; return --it, temp; }
	constexpr bool operator==(ReverseIterator that) const { return it == that.it; }
	constexpr bool operator!=(ReverseIterator that) const { return it != that.it; }
	constexpr Element &operator*() { return *it; }
	constexpr Iterator operator->() { return it; }
};

inline constexpr struct null_opt_t {} null_opt;

template <class T>
struct OptionalBaseTrivial {
	union {
		T _value;
	};
	bool _has_value;
	constexpr OptionalBaseTrivial() {
		this->_has_value = false;
	}
};

template <>
struct OptionalBaseTrivial<void> {
	bool _has_value;
	constexpr OptionalBaseTrivial() {
		this->_has_value = false;
	}
};

template <class T>
struct OptionalBaseNonTrivial {
	union {
		T _value;
	};
	bool _has_value;
	constexpr OptionalBaseNonTrivial() {
		this->_has_value = false;
	}
	constexpr OptionalBaseNonTrivial(OptionalBaseNonTrivial const &that) {
		if (that._has_value) {
			new (&this->_value) T(that._value);
		}
		this->_has_value = that._has_value;
	}
	constexpr OptionalBaseNonTrivial& operator=(OptionalBaseNonTrivial const &that) {
		if (this->_has_value) {
			if (that._has_value) {
				this->_value = that._value;
			} else {
				this->_has_value = false;
				this->_value.~T();
			}
		} else {
			if (that._has_value) {
				new(&this->_value) T(that._value);
				this->_has_value = true;
			} else {
			}
		}
		return *this;
	}
	constexpr OptionalBaseNonTrivial& operator=(OptionalBaseNonTrivial &&that) {
		if (this->_has_value) {
			if (that._has_value) {
				this->_value = std::move(that._value);
			} else {
				this->_has_value = false;
				this->_value.~T();
			}
		} else {
			if (that._has_value) {
				new(&this->_value) T(std::move(that._value));
				this->_has_value = true;
			} else {
			}
		}
		return *this;
	}
	constexpr ~OptionalBaseNonTrivial() {
		if (this->_has_value)
			this->_value.~T();
	}
};

template <class T>
struct Optional : std::conditional_t<std::is_trivially_destructible_v<T>, OptionalBaseTrivial<T>, OptionalBaseNonTrivial<T>> {
	using Element = T;
	constexpr Optional() {}
	constexpr Optional(Element that) {
		new (&this->_value) Element(that);
		this->_has_value = true;
	}
	constexpr Optional(null_opt_t) {
		this->_has_value = false;
	}

	template <class ...Args>
	constexpr void emplace(Args &&...args) {
		if (this->_has_value) {
			this->_value = T(std::forward<Args>(args)...);
		} else {
			new (&this->_value) T(std::forward<Args>(args)...);
			this->_has_value = true;
		}
	}

	constexpr void reset() {
		this->_has_value = false;
	}

	constexpr explicit operator bool() const { return this->_has_value; }

	constexpr bool has_value() const { return this->_has_value; }
	constexpr auto &value() { assert_always(this->_has_value); return this->_value; }
	constexpr auto &value_unchecked() { return this->_value; }
	constexpr auto &value() const { assert_always(this->_has_value); return this->_value; }
	constexpr auto &value_unchecked() const { return this->_value; }

	template <class Fallback>
	constexpr Element value_or(Fallback &&fallback) requires requires { (Element)fallback(); } {
		if (this->_has_value)
			return this->_value;
		return fallback();
	}

	constexpr Element value_or(Element fallback) {
		if (this->_has_value)
			return this->_value;
		return fallback;
	}


	template <class U>
	constexpr Optional<U> map() {
		if (this->_has_value) {
			return (U)this->_value;
		}
		return null_opt;
	}

	template <std::invocable<Element> Fn>
	constexpr auto map(Fn fn) const -> Optional<decltype(fn(std::declval<Element>()))> {
		if (this->_has_value) {
			return fn(this->_value);
		}
		return null_opt;
	}

	constexpr void apply(auto &&fn) {
		if (this->_has_value)
			fn(this->_value);
	}

#pragma warning(suppress: 4820)
};

template <>
struct Optional<void> : OptionalBaseTrivial<void> {
	using Element = void;
	constexpr Optional() {}
	constexpr Optional(null_opt_t) {
		this->_has_value = false;
	}

	constexpr void reset() {
		this->_has_value = false;
	}

	constexpr explicit operator bool() const { return this->_has_value; }

	constexpr bool has_value() const { return this->_has_value; }

	template <class Fallback>
	constexpr Element value_or(Fallback &&fallback) requires requires { (Element)fallback(); } {
		if (this->_has_value)
			return void();
		return fallback();
	}

	template <std::invocable<Element> Fn>
	constexpr auto map(Fn fn) const -> Optional<decltype(fn(std::declval<Element>()))> {
		if (this->_has_value) {
			return fn();
		}
		return null_opt;
	}

	void _set_has_value(bool x) { this->_has_value = x; }

#pragma warning(suppress: 4820)
};

template <class T> struct std::tuple_size<Optional<T>> : std::integral_constant<size_t, 2> {};
template <class T> struct std::tuple_element<0, Optional<T>> { using type = T; };
template <class T> struct std::tuple_element<1, Optional<T>> { using type = bool; };

template <size_t i, class T>
auto get(Optional<T> x) {
	if constexpr (i == 0) return x._value;
	if constexpr (i == 1) return x._has_value;
}

template <class T>
struct IsOptionalT : std::false_type {};
template <class T>
struct IsOptionalT<Optional<T>> : std::true_type {};
template <class T>
inline static constexpr bool is_optional = IsOptionalT<T>::value;

template <class T>
Optional<T> to_optional(T *pointer) {
	return pointer ? *pointer : Optional<T>{};
}

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

template <class T> 
bool operator==(Optional<T> a, Optional<T> b) { 
	if (a) {
		if (b) {
			return a.value_unchecked() == b.value_unchecked();
		} else {
			return false;
		}
	} else {
		return !b;
	}
}

template <class T> bool operator!=(Optional<T> a, Optional<T> b) { return !(a == b); }
template <class T> bool operator==(Optional<T> a, T b) { return a == Optional<T>(b); }
template <class T> bool operator!=(Optional<T> a, T b) { return !(a == b); }
template <class T> bool operator==(T a, Optional<T> b) { return Optional<T>(a) == b; }
template <class T> bool operator!=(T a, Optional<T> b) { return !(a == b); }

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

	constexpr Value value_or(Value fallback) {
		if (_is_value)
			return _value;
		return fallback;
	}

	constexpr Error error_or(Error fallback) {
		if (!_is_value)
			return _error;
		return fallback;
	}
	
	constexpr auto map(auto fn) requires requires(Value v) { fn(v); } {
		using R = decltype(fn(_value));
		Optional<R> result;
		if constexpr (std::is_same_v<R, void>) {
			if (_is_value) {
				fn(_value);
				result._set_has_value(true);
			}
		} else {
			if (_is_value) {
				result = fn(_value);
			}
		}
		return result;
	}

	constexpr auto or_else(auto fn) requires requires { fn(); } {
		using R = decltype(fn());
		if (_is_value) {
			return Optional<R>{};
		}
		return Optional<R>{ fn() };
	}

private:
	bool _is_value;
	union {
		Value _value;
		Error _error;
	};
};

#define TRY(name, expression)           \
	auto name##_or_error = expression;  \
	if (name##_or_error.is_error())     \
		return name##_or_error.error(); \
	auto name = name##_or_error.value()

enum class Overflow {
	clamp,
	wrap,
};

enum class Interpolation {
	linear,
	nearest,
};

struct SampleParams {
	Overflow overflow = Overflow::clamp;
	Interpolation interpolation = Interpolation::linear;
	bool normalized = true;
};

static constexpr bool max_is_divisible_by_min(umm a, umm b) {
	return max(a, b) % min(a, b) == 0;
}

/*
interface Iter {
	// Returns false if there is no more elements.
	explicit operator bool();

	// Advance to next element.
	void next();

	// Current element. Assert that `valid()` is true.
	T value();

	[[optional]] K key();     // key corresponding to the value
}
*/

struct ReverseIterOption {
	bool reverse = false;
};

struct IterBase {
	auto key_value(this auto &&self) requires requires { self.key(); self.value(); } {
		return std::pair<decltype(self.key()), decltype(self.value())>{self.key(), self.value()};
	}
	decltype(auto) operator*(this auto &&self) { return self.value(); }

	void skip(this auto &&self, umm n) {
		while (n--) self.next();
	}
};

template <class T>
struct SpanIter : IterBase {
	T *data = 0;
	umm count = 0;

	T *it = 0;
	int step = 0;

	constexpr explicit operator bool() {
		return (umm)(it - data) < count;
	}
	constexpr void next() {
		it += step;
	}
	constexpr void skip(umm n) {
		it += n * step;
	}

	constexpr umm key() { return it - data; }
	constexpr T &value() { return *it; }
};

template <class T>
constexpr SpanIter<T> span_iter(T *start, T *end, ReverseIterOption options = {}) {
	return {
		.data = start,
		.count = (umm)(end - start),
		.it   = options.reverse ? end - 1 : start,
		.step = options.reverse ? -1      : 1,
	};
}

template <class T, umm count>
constexpr auto to_iter(T (&arr)[count], ReverseIterOption options = ReverseIterOption{} /* MSVC bug requires type name */) {
	return span_iter(arr, arr + count, options);
}

#pragma pack(push, 1)
template <class T, class Size_ = umm>
struct Span {
	using Element = T;
	using Size = Size_;
	using ReverseIterator = ReverseIterator<T *>;
	using Iter = SpanIter<T>;
	using IterOptions = ReverseIterOption;

	constexpr Span(std::initializer_list<Element> list) : data((Element *)list.begin()), count(list.size()) {}
	constexpr Span() = default;
	// constexpr Span(Element &value) : data(std::addressof(value)), count(1) {}
	template <Size count>
	constexpr Span(Element (&array)[count]) : data(array), count(count) {}
	constexpr Span(Element *begin, Element *end) : data(begin), count(end - begin) {
		assert_equal(count, (umm)(end - begin));
	}
	constexpr Span(Element const *begin, Element const *end) : data((Element *)begin), count(end - begin) {}
	constexpr Span(Element *begin, Size count) : data(begin), count(count) {}
	constexpr Span(Element const *begin, Size count) : data((Element *)begin), count(count) {}
	constexpr Element *begin() const { return data; }
	constexpr Element *end() const { return data + count; }

	constexpr ReverseIterator rbegin() const { return data + count - 1; }
	constexpr ReverseIterator rend() const { return data - 1; }

	constexpr Element &front() const {
		bounds_check(assert(count));
		return *data;
	}
	constexpr Element &back() const {
		bounds_check(assert(count));
		return data[count - 1];
	}
	constexpr Element &operator[](Size i) const {
		bounds_check(assert_less(i, count));
		return data[i];
	}
	constexpr Element &at(Size i) const {
		bounds_check(assert_less(i, count));
		return data[i];
	}

	[[deprecated("use `sample` instead")]]
	constexpr Element at_interpolated(f64 i) const {
		auto a = at(floor_to_int(i));
		auto b = at(ceil_to_int(i));
		return lerp(a, b, frac(i));
	}

	[[deprecated("use `sample` instead")]]
	constexpr Element at_interpolated_clamped(f64 i) const {
		auto a = at(max((Size)0, (Size)floor_to_int(i)));
		auto b = at(min(count - 1, (Size)ceil_to_int(i)));
		return lerp(a, b, frac(i));
	}

	constexpr Element sample(std::floating_point auto i, SampleParams params) const {
		using I = decltype(i);
		if (params.normalized) {
			if (params.overflow == Overflow::clamp) {
				i *= count - 1;
			} else {
				i *= count;
			}
		}
		using SSize = std::make_signed_t<Size>;
		switch (params.interpolation) {
			default:
			case Interpolation::linear: {
				switch (params.overflow) {
					default:
					case Overflow::clamp: {
						i = clamp<I>(i, 0, (I)(count-1));
						auto a = at((SSize)floor_to_int(i));
						auto b = at((SSize)ceil_to_int(i));
						return lerp(a, b, frac(i));
					}
					case Overflow::wrap: {
						auto a = at(frac((SSize)floor_to_int(i), (SSize)count));
						auto b = at(frac((SSize)ceil_to_int(i), (SSize)count));
						return lerp(a, b, frac(i));
					}
				}
				break;
			}
			case Interpolation::nearest: {
				switch (params.overflow) {
					default:
					case Overflow::clamp: {
						return at(clamp<SSize>(floor_to_int(i), 0, count - 1));
					}
					case Overflow::wrap: {
						return at(frac((SSize)floor_to_int(i), (SSize)count));
					}
				}
				break;
			}
		}
	}

	constexpr bool is_empty() const { return count == 0; }

	template <class U, class ThatSize>
	constexpr explicit operator Span<U, ThatSize>() const {
		static_assert(sizeof(U) == sizeof(T));
		assert_equal((ThatSize)count, count);
		return {(U *)data, (ThatSize)count};
	}

	template <class ThatSize>
	constexpr operator Span<T, ThatSize>() const {
		assert_equal((ThatSize)count, count);
		return {data, (ThatSize)count};
	}

	template <class ThatSize>
	constexpr bool operator==(Span<T, ThatSize> that) const requires requires(T t) { { t == t } -> std::convertible_to<bool>; } {
		if (count != that.count)
			return false;

		if (!std::is_constant_evaluated()) {
			if constexpr (std::is_integral_v<T>) {
				u8 *ap = (u8 *)data;
				u8 *bp = (u8 *)that.data;

				u8 *endp = (u8 *)end();
			
				#if TL_USE_SIMD
				while (endp - ap >= 16) {
					__m128i a = _mm_loadu_si128((__m128i *)ap); ap += 16;
					__m128i b = _mm_loadu_si128((__m128i *)bp); bp += 16;
					__m128i c = _mm_cmpeq_epi32(a, b);
					int m = _mm_movemask_ps(_mm_castsi128_ps(c));
					if (m != 0xF) {
						return false;
					}
				}
				#endif
				while (endp - ap >= 8) {
					u64 a = *(u64 *)ap; ap += 8;
					u64 b = *(u64 *)bp; bp += 8;
					if (a != b) {
						return false;
					}
				}
				while (endp - ap) {
					if (*ap++ != *bp++) {
						return false;
					}
				}
				return true;
			}
			#if TL_USE_SIMD
			if constexpr (std::is_same_v<T, f32>) {
				umm remaining_count = count;
				f32 *ap = data;
				f32 *bp = that.data;

				#if TL_ALIGN_RW_OPS
				// Align `ap` to 16 bytes.
				while (remaining_count && (((umm)ap & 15) != 0)) {
					if (*ap != *bp)
						return false;
					++ap;
					++bp;
					--remaining_count;
				}
				#endif
				while (remaining_count >= 4) {
					#if TL_ALIGN_RW_OPS
					__m128 a = _mm_load_ps(ap);
					#else
					__m128 a = _mm_loadu_ps(ap);
					#endif
					__m128 b = _mm_loadu_ps(bp);
					__m128 c = _mm_cmpeq_ps(a, b);
					int m = _mm_movemask_ps(c);
					if (m != 0xF) {
						return false;
					}
					ap += 4;
					bp += 4;
					remaining_count -= 4;
				}
				while (remaining_count) {
					if (*ap != *bp)
						return false;
					++ap;
					++bp;
					--remaining_count;
				}
				return true;
			}
			#endif
		}
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
			amount = clamp<smm>(amount, 0, count);
			return {data + amount, (Size)((smm)count - amount)};
		} else {
			amount = clamp<smm>(amount, -(smm)count, 0);
			return {data, (Size)(count + amount)};
		}
	}
	constexpr Span take(smm amount) const {
		if (amount >= 0) {
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

	Span operator||(Span that) const {
		if (count) 
			return *this;
		return that;
	}

	void set(Span that) const {
		assert(count == that.count);
		for (umm i = 0; i < count; ++i) {
			data[i] = that.data[i];
		}
	}
	
	constexpr bool owns(T const *pointer) {
		return (umm)(pointer - data) < count;
	}

	auto iter(this auto &&self, ReverseIterOption options = {}) {
		return span_iter(self.data, self.data + self.count, options);
	}

	Element *data = 0;
	Size count = 0;
};
#pragma pack(pop)

template <class T>
inline constexpr bool is_span = false;

template <class T, class Size>
inline constexpr bool is_span<Span<T, Size>> = true;

template <class T>
concept ASpan = is_span<T>;

TL_DECLARE_CONCEPT(Span);

template <class T, umm x>                                                  inline Span<T> flatten(T (&array)[x]                     ) { return {(T *)array, x              }; }
template <class T, umm x, umm y>                                           inline Span<T> flatten(T (&array)[x][y]                  ) { return {(T *)array, x*y            }; }
template <class T, umm x, umm y, umm z>                                    inline Span<T> flatten(T (&array)[x][y][z]               ) { return {(T *)array, x*y*z          }; }
template <class T, umm x, umm y, umm z, umm w>                             inline Span<T> flatten(T (&array)[x][y][z][w]            ) { return {(T *)array, x*y*z*w        }; }
template <class T, umm x, umm y, umm z, umm w, umm r>                      inline Span<T> flatten(T (&array)[x][y][z][w][r]         ) { return {(T *)array, x*y*z*w*r      }; }
template <class T, umm x, umm y, umm z, umm w, umm r, umm g>               inline Span<T> flatten(T (&array)[x][y][z][w][r][g]      ) { return {(T *)array, x*y*z*w*r*g    }; }
template <class T, umm x, umm y, umm z, umm w, umm r, umm g, umm b>        inline Span<T> flatten(T (&array)[x][y][z][w][r][g][b]   ) { return {(T *)array, x*y*z*w*r*g*b  }; }
template <class T, umm x, umm y, umm z, umm w, umm r, umm g, umm b, umm a> inline Span<T> flatten(T (&array)[x][y][z][w][r][g][b][a]) { return {(T *)array, x*y*z*w*r*g*b*a}; }

template <class T, umm count>
struct IterOfT<T[count]> { using Iter = SpanIter<T>; };

// MSVC bug: {} instead of ReverseIterOption{} does not compile
template <class T, umm count>
constexpr auto iter(T (&array)[count], ReverseIterOption options = ReverseIterOption{}) {
	return span_iter(array, array + count, options);
}

template <class T>
struct IterOfT<std::initializer_list<T>> { using Iter = SpanIter<T>; };

template <class T>
constexpr auto iter(std::initializer_list<T> list, ReverseIterOption options = {}) {
	return span_iter(list.begin(), list.end(), options);
}

forceinline constexpr Span<ascii> operator""s(ascii const *string, umm count) { return Span((ascii *)string, count); }
forceinline constexpr Span<utf8 > operator""s(utf8  const *string, umm count) { return Span((utf8  *)string, count); }
forceinline constexpr Span<utf16> operator""s(utf16 const *string, umm count) { return Span((utf16 *)string, count); }
forceinline constexpr Span<utf32> operator""s(utf32 const *string, umm count) { return Span((utf32 *)string, count); }
forceinline constexpr Span<ascii> operator""ts(ascii const *string, umm count) { return Span((ascii *)string, count + 1); }
forceinline constexpr Span<utf8 > operator""ts(utf8  const *string, umm count) { return Span((utf8  *)string, count + 1); }
forceinline constexpr Span<utf16> operator""ts(utf16 const *string, umm count) { return Span((utf16 *)string, count + 1); }
forceinline constexpr Span<utf32> operator""ts(utf32 const *string, umm count) { return Span((utf32 *)string, count + 1); }
forceinline constexpr Span<u8> operator""b(char const *string, umm count) { return Span((u8 *)string, count); }

forceinline constexpr Span<utf8, u32> operator""s32(utf8  const *string, umm count) { return Span((utf8  *)string, (u32)count); }

template <class T, umm count>
inline constexpr Span<T> array_as_span(T const (&arr)[count]) { return Span((T *)arr, count); }

template <class T>
inline constexpr Span<T> as_span(std::initializer_list<T> list) { return Span((T *)list.begin(), list.size()); }

inline constexpr Span<ascii> as_span(ascii const *str) { return Span((ascii *)str, string_unit_count(str)); }
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

template <class Size>
constexpr Span<utf8, Size> as_utf8(Span<ascii, Size> span) {
	return {(utf8 *)span.begin(), span.count};
}
template <class Size>
constexpr Span<utf8, Size> as_utf8(Span<u8, Size> span) {
	return {(utf8 *)span.begin(), span.count};
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
constexpr Span<ascii, Size> as_chars(Span<T, Size> span) {
	return {(ascii *)span.begin(), span.count * sizeof(T)};
}
template <class T>
constexpr Span<ascii> as_chars(T span_like) {
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

template <class T, class Size, ACompare<T> Compare = decltype(default_comparer)>
constexpr smm compare(Span<T, Size> a, Span<T, Size> b, Compare compare = default_comparer) {
	if (a.count != b.count) {
		return a.count - b.count;
	}

	for (umm i = 0; i < a.count; ++i) {
		smm r = compare(a.data[i], b.data[i]);
		if (r)
			return r;
	}

	return 0;
}

template <class T, class Size, ACompare<T> Compare = decltype(default_comparer)>
constexpr smm compare_lexical(Span<T, Size> a, Span<T, Size> b, Compare compare = default_comparer) {
	umm min_count = min(a.count, b.count);

	for (umm i = 0; i < min_count; ++i) {
		if (a.data[i] != b.data[i])
			return compare(a.data[i], b.data[i]);
	}
	
	if (a.count != b.count) {
		return a.count - b.count;
	}

	return 0;
}

inline static constexpr auto lexical_comparer = []<class T>(T a, T b) { return compare_lexical(a, b); };

template <class T, class TSize, class U, class USize, APredicate<T, U> Predicate = decltype(predicate_equal)>
inline constexpr bool starts_with(Span<T, TSize> str, Span<U, USize> sub_str, Predicate predicate = predicate_equal) {
	if (sub_str.count > str.count)
		return false;
	for (USize i = 0; i < sub_str.count; ++i) {
		if (!predicate(str.data[i], sub_str.data[i])) {
			return false;
		}
	}
	return true;
}

template <class T, class TSize, class U, class USize, APredicate<T, U> Predicate = decltype(predicate_equal)>
inline constexpr bool ends_with(Span<T, TSize> str, Span<U, USize> sub_str, Predicate predicate = predicate_equal) {
	if (sub_str.count > str.count)
		return false;
	auto base_offset = str.count - sub_str.count;
	for (USize i = 0; i < sub_str.count; ++i) {
		if (!predicate(str.data[i + base_offset], sub_str.data[i])) {
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
	if ((smm)(where.count - what.count + 1) <= 0)
		return 0;

	for (umm i = 0; i < where.count - what.count + 1; ++i) {
		auto where_part = Span(where.data + i, what.count);
		if (where_part == what) {
			return where_part.data;
		}
	}
	return 0;
}

template <class T, class SizeA, class SizeB>
constexpr T *find(Span<T, SizeA> where, Span<T, SizeB> what, auto &&compare) {
	if ((smm)(where.count - what.count + 1) <= 0)
		return 0;

	for (umm i = 0; i < where.count - what.count + 1; ++i) {
		auto window = where.data + i;
		for (umm j = 0; j < what.count; ++j) {
			if (!compare(window[j], what.data[j])) {
				goto continue_outer;
			}
		}
		return window;
	continue_outer:;
	}
	return 0;
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

template <class T, class Predicate>
constexpr T *find_last_if(Span<T> span, Predicate &&predicate) {
	for (auto i = span.end() - 1; i >= span.begin(); --i) {
		if (predicate(*i)) {
			return i;
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
	foreach(a, where, {.reverse = true}) {
		foreach(b, what) {
			if (*a == *b) {
				return &*a;
			}
		}
	}
	return 0;
}

//
// Default implementations for collection algorithms.
// If collection has method with the same name, it is used.
// Otherwise a potentially slower default implementation is utilized.
//

// Const correctess is pain. So much code for so little value.
// Lambdas in requires are broken. redefinition. ???
template <Collection Collection>
auto find(Collection &&collection, ElementOf<Collection> element) {
	if constexpr (requires { collection.find(element); }) {
		return collection.find(element);
	} else if constexpr (requires { to_iter(collection); }) {
		foreach (it, collection) {
			if (all(*it == element)) {
				return &*it;
			}
		}
		return decltype(&*collection.iter()){};
	} else {
		for (auto &it : collection) {
			if (all(it == element)) {
				return &it;
			}
		}
		return decltype(&*collection.begin()){};
	}
}

template <class Collection>
umm find_index_of(Collection &collection, const ElementOf<Collection> &value) {
	return index_of(collection, find(collection, value));
}

template <class Collection, class Predicate>
umm find_index_of_if(Collection &collection, Predicate predicate) {
	return index_of(collection, find_if(collection, predicate));
}

template <class T>
constexpr void replace(Span<T> destination, Span<T> source, umm start_index = 0) {
	for (umm i = 0; i < source.count; ++i) {
		destination[start_index + i] = source[i];
	}
}

template <class T, class Size>
void replace_inplace(Span<T, Size> where, T what, T with) {
	for (auto &v : where) {
		v = v == what ? with : v;
	}
}

struct ReplaceInplaceOptions {
	// Number of writable elements after `where.data` to allow for growth.
	// If 0, use where.count.
	umm capacity = 0;

	// When true, replacements will work from end to start.
	//   This might do more work that will be discarded,
	//   but all occurences will be replaced without leaving cut off parts.
	// When false, replacements will go forward.
	//   This will do less work, mut might leave cut off parts you're
	//   trying to replace. 
	// Ignored if no growth happens.
	bool reverse = true;
};

// Replaces occurrences of `what` in `where` with `with`.
template <class T, class Size>
void replace_inplace(Span<T, Size> &where, Span<T> what, Span<T> with, ReplaceInplaceOptions options = {}) {
	if (options.capacity == 0) {
		options.capacity = where.count;
	}
	assert(options.capacity >= where.count);

	T *buf_end = where.data + options.capacity;
	T *found = where.begin();
	if (with.count <= what.count) {
		// `where` is going to shink or stay the same size. No need for bounds checks.
		while (found = find(Span(found, where.end()), what)) {
			memmove(found + with.count, found + what.count, (where.end() - (found + what.count)) * sizeof(T));
			memcpy(found, with.data, with.count * sizeof(T));
			where.count += with.count - what.count;
			found += with.count;
		}
	} else {
		// `where` is going to grow. Ensure don't overflow `options.capacity`.

		auto spread_and_insert = [&]() {
			T *d = found + with.count;
			T *s = found + what.count;
			smm count = max((smm)0, min(where.end() - s, buf_end - d));
			memmove(d, s, count * sizeof(T));

			d = found;
			s = with.data;
			count = min((smm)with.count, buf_end - found);
			memcpy(d, s, count * sizeof(T));

			where.count = min(where.count + with.count - what.count, options.capacity);
		};

		if (options.reverse) {
			found = where.end();
			while (found = find_last(Span(where.begin(), found), what).data) {
				spread_and_insert();
			}
		} else {
			while (found < where.end() && (found = find(Span(found, where.end()), what))) {
				spread_and_insert();
				found += with.count;
			}
		}
	}
}

template <class T>
struct BinarySearchResult {
	T *found = 0;
	T *would_be_at = 0;
};

template <class T>
BinarySearchResult<T> binary_search(Span<T> span, ACompare1<T> auto compare) {
	auto begin = span.begin();
	auto end   = span.end();
	while (1) {
		if (begin == end)
			return {.would_be_at = begin};

		auto mid = begin + (end - begin) / 2;

		auto c = compare(*mid);

		if (c == 0)
			return {.found = mid, .would_be_at = mid};

		if (c < 0) {
			end = mid;
		} else {
			begin = mid + 1;
		}
	}
}

template <class T>
BinarySearchResult<T> binary_search(Span<T> span, T value, ACompare<T> auto compare) {
	return binary_search(span, [&](T it) { return compare(value, it); });
}

template <class T>
BinarySearchResult<T> binary_search(Span<T> span, T value) {
	return binary_search(span, [&](T it) { return compare(value, it); });
}

template <class T, class U>
BinarySearchResult<T> binary_search(Span<T> span, U value, Callable<U, T> auto map) {
	return binary_search(span, [&](T it) { return compare(value, map(it)); });
}

template <class T, class Size>
auto split_by_one(Span<T, Size> what, T by) {
	struct Iter : IterBase {
		Span<T, Size> what = {};
		T *word_start = 0;
		T *cursor = 0;
		T by = {};

		explicit operator bool() {
			return word_start < what.end();
		}
		void next() {
			word_start = cursor;
			while (cursor < what.end() && *cursor != by)
				++cursor;
			++cursor;
		}
		Span<T, Size> value() {
			return {word_start, cursor - 1};
		}
	};

	auto iter = Iter{
		.what = what,
		.cursor = what.data,
		.by = by,
	};

	iter.next();

	return iter;
}

template <class T, class Size, class Fn>
void split_by_one(Span<T, Size> what, T by, Fn &&in_fn) {
	auto fn = wrap_foreach_fn<Span<T, Size>>(in_fn);

	umm start = 0;
	umm what_start = 0;

	for (; what_start < what.count;) {
		if (what.data[what_start] == by) {
			if (fn(what.subspan(start, what_start - start)) == ForEach_break)
				return;

			start = what_start + 1;
		}
		++what_start;
	}

	fn(Span(what.data + start, what.end()));
}

// If `what` contains `by`, returns first part before `by`, rest assigned to `what`.
// If not found, returns `what`, `what` is cleared.
template <class T, class Size, APredicate<T, T> Predicate = decltype(predicate_equal)>
Span<T, Size> split_by_one_first(Span<T, Size> *what, T by, Predicate predicate = predicate_equal) {
	Span<T, Size> result = {};
	for (umm i = 0; i < what->count; ++i) {
		if (predicate(what->data[i], by)) {
			result = {what->data, i};
			*what = {what->data + i + 1, what->end()};
			return result;
		}
	}

	result = *what;
	*what = {what->end(), (umm)0};
	return result;
}

template <class T, class Size, class Fn>
void split_by_any(Span<T, Size> what, Span<T> by, Fn &&in_fn) {
	auto fn = wrap_foreach_fn<Span<T, Size>>(in_fn);

	umm start = 0;
	umm what_start = 0;

	for (; what_start < what.count;) {
		if (find(by, what.data[what_start])) {
			if (fn(what.subspan(start, what_start - start)) == ForEach_break)
				return;
			start = what_start + 1;
		}
		++what_start;
	}

	fn(Span(what.data + start, what.end()));
}

template <class T, class Size, class Fn>
void split_by_seq(Span<T, Size> what, Span<T> by, Fn &&in_fn) {
	auto fn = wrap_foreach_fn<Span<T, Size>>(in_fn);

	umm start = 0;
	umm what_start = 0;

	for (; what_start < what.count;) {
		if (starts_with(what.skip(what_start), by)) {
			if (fn(what.subspan(start, what_start - start)) == ForEach_break)
				return;
			start = what_start + by.count;
		}
		++what_start;
	}

	fn(Span(what.data + start, what.end()));
}

template <class T, class Size, class Selector, class GroupProcessor>
void group_by(Span<T, Size> span, Selector selector, GroupProcessor processor) {
	if (!span.count) {
		return;
	}

	auto first = selector(span.data[0]);
	Size first_index = 0;
	umm group_index = 0;

	auto process_group = [&](Span<T, Size> group) {
		if constexpr (requires { processor(group); }) {
			processor(group);
		} else if constexpr (requires { processor(group_index, group); }) {
			processor(group_index++, group);
		} else {
			static_error_t(T, "Invalid group processor signature");
		}
	};

	for (Size i = 1; i < span.count; ++i) {
		auto next = selector(span.data[i]);
		if (first != next) {
			process_group(span.subspan(first_index, i - first_index));
			first = next;
			first_index = i;
		}
	}

	process_group(span.subspan(first_index, span.count - first_index));
}

template <class T, class Size>
Span<T, Size> trim_left(Span<T, Size> span, auto &&predicate) {
	while (span.count && predicate(span.data[0])) {
		span.data++;
		span.count--;
	}
	return span;
}

template <class T, class Size>
Span<T, Size> trim_right(Span<T, Size> span, auto &&predicate) {
	while (span.count && predicate(span.data[span.count - 1])) {
		span.count--;
	}
	return span;
}

template <class T, class Size>
Span<T, Size> trim(Span<T, Size> span, auto &&predicate) {
	return trim_right(trim_left(span, predicate), predicate);
}

template <class T, class Size>
Span<T, Size> trim_left(Span<T, Size> span, T &&value) {
	return trim_left(span, [&](T const &it) { return it == value; });
}

template <class T, class Size>
Span<T, Size> trim_right(Span<T, Size> span, T &&value) {
	return trim_right(span, [&](T const &it) { return it == value; });
}

template <class T, class Size>
Span<T, Size> trim(Span<T, Size> span, T &&value) {
	return trim(span, [&](T const &it) { return it == value; });
}

template <class T>
constexpr T dot(Span<T> a, Span<T> b) {
	assert_equal(a.count, b.count);
	T result = {};
	for (umm i = 0; i < a.count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

template <class T, class Size>
Span<T, Size> erase_all_compacting(Span<T, Size> span, auto &&predicate)
	requires requires(T v) { predicate(v); }
{
	auto end = span.data + span.count;
	auto dst = span.data;
	for (auto it = span.data; it != end; ++it) {
		if (predicate(*it)) {
			it->~T();
		} else {
			*dst++ = (T &&)*it;
		}
	}
	return {span.data, (Size)(dst - span.data)};
}

#define passthrough(function) ([&]<class ...Args>(Args &&...args) -> decltype(auto) { return function(std::forward<Args>(args)...); })

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
	return (u32)(c - '0') < 10;
}
inline constexpr bool is_digit(utf32 c) {
	return (u32)(c - '0') < 10;
}

inline constexpr bool is_hex_digit(utf32 c) {
	return ((u32)(c - '0') < 10)
	     | ((u32)(c - 'a') < 6)
	     | ((u32)(c - 'A') < 6);
}
inline constexpr Optional<u8> hex_digit_to_int(utf32 c) {
	if (c >= '0' && c <= '9') return (u8)(c - '0');
	if (c >= 'a' && c <= 'f') return (u8)(c - 'a' + 10);
	if (c >= 'A' && c <= 'F') return (u8)(c - 'A' + 10);
	return {};
}

// Branchless version for cases when you know `c` is valid.
inline constexpr u8 hex_digit_to_int_unchecked(utf32 c) {
    u32 a = c - 'A' + 10;
    u32 b = c - 'a' + 10;
    a = b < 16 ? b : a;
    b = c - '0';
    a = b < 10 ? b : a;
    return (u8)a;
}

inline constexpr bool is_punctuation(utf32 c) {
	bool result = false;

	#define R(a, b) result |= ((c - a) <= (b - a));
	R(33, 47);   // !"#$%&'()*+,-./
	R(58, 64);   // :;<=>?@
	R(91, 96);   // [\]^_`
	R(123, 126); // {|}~
	#undef R

	return result;
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

inline bool equals_case_insensitive(Span<char> a, Span<char> b) {
	if (a.count != b.count)
		return false;
	for (auto ap = a.begin(), bp = b.begin(); ap != a.end(); ++ap, ++bp)
		if (to_lower(*ap) != to_lower(*bp))
			return false;
	return true;
}

inline bool equals_case_insensitive(Span<utf8> a, Span<utf8> b) {
	if (a.count != b.count)
		return false;
	auto ap = a.begin();
	for (auto bp = b.begin(); ap != a.end(); ++ap, ++bp)
		if (to_lower(*ap) != to_lower(*bp))
			return false;
	return true;
}

constexpr auto predicate_equal_case_insensitive = []<class T>(T const &a, T const &b) { return equals_case_insensitive(a, b); };

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

template <class T>
inline constexpr Span<T> skip_front(Span<T> span, Span<T> to_skip) {
	if (span.count == 0) {
		return span;
	}
	while (span.count && is_any_of(span.front(), to_skip)) {
		++span.data;
		--span.count;
	}
	return span;
}

template <class T>
void reverse_in_place(Span<T> span) {
	for (umm i = 0; i < span.count / 2; ++i) {
		Swap(span.data[i], span.data[span.count - i - 1]);
	}
}

template <umm size>
struct BitSet {
	using Word = umm;
	inline static constexpr auto bit_count = size;
	inline static constexpr auto bits_in_word = sizeof(Word) * 8;
	inline static constexpr auto word_count = ceil(size, bits_in_word) / bits_in_word;
	Word words[word_count] = {};

	bool get(umm i) const {
		bounds_check(assert(i < size));
		return word(i) & bit(i);
	}
	void set(umm i, bool value) {
		bounds_check(assert(i < size));
		if (value) {
			word(i) |= bit(i);
		} else {
			word(i) &= ~bit(i);
		}
	}
	void flip(umm i) {
		bounds_check(assert(i < size));
		word(i) ^= bit(i);
	}

	auto &word(umm i) { return words[i / bits_in_word]; }
	auto &word(umm i) const { return words[i / bits_in_word]; }

	umm bit(umm i) const {
		return (Word)1 << (i % bits_in_word);
	}

	umm count() const {
		umm result = 0;
		if constexpr (size % bits_in_word == 0) {
			// All words are popcountable
			for (auto word : words) {
				result += count_bits(word);
			}
		} else {
			// Last word should be trimmed
			auto words_span = array_as_span(words);
			for (auto word : words_span.skip(-1)) {
				result += count_bits(word);
			}

			Word last_word = words_span.back();
			
			Word mask = ((Word)1 << (size % bits_in_word)) - 1;

			result += count_bits(last_word & mask);
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
			auto index = word_index * bits_in_word + bit_index;
			if (index >= size)
				return {};
			return index;
		}
		return {};
	}
	inline constexpr auto operator<=>(BitSet const &) const = default;
	inline constexpr BitSet operator~() const {
		auto result = *this;
		for (auto &word : result.words)
			word = ~word;
		return result;
	}
	inline constexpr friend BitSet operator&(BitSet a, BitSet const &b) {
		for (umm i = 0; i < a.word_count; ++i) {
			a.words[i] &= b.words[i];
		}
		return a;
	}
};

template <ForEachFlags flags = 0, umm size>
bool for_each(BitSet<size> &set, auto &&in_fn)
	requires requires { in_fn((umm)0); }
{
	using Word = typename BitSet<size>::Word;

	auto fn = wrap_foreach_fn<umm>(in_fn);
					
	constexpr ForEachFlags allowed_flags = ForEach_break;

	if constexpr (flags & ForEach_reverse) {
		for (umm word_index = count_of(set.words) - 1; word_index != -1; --word_index) {
			auto word = set.words[word_index];
			if (!word)
				continue;

			for (u64 bit_index = set.bits_in_word - 1; bit_index != -1; --bit_index) {
				if (word & ((Word)1 << bit_index)) {
					auto absolute_index = word_index * set.bits_in_word + bit_index;
					if (absolute_index >= size)
						continue;

					auto d = fn(absolute_index);
					
					assert(!(d & ~allowed_flags), "not supported");

					if (d & ForEach_break)
						return true;
				}
			}
		}
	} else {
		for (umm word_index = 0; word_index < count_of(set.words); ++word_index) {
			auto word = set.words[word_index];
			if (!word)
				continue;

			for (u64 bit_index = 0; bit_index < set.bits_in_word; ++bit_index) {
				if (word & ((Word)1 << bit_index)) {
					auto absolute_index = word_index * set.bits_in_word + bit_index;
					if (absolute_index >= size)
						return false;
					
					auto d = fn(absolute_index);
					
					assert(!(d & ~allowed_flags), "not supported");

					if (d & ForEach_break)
						return true;
				}
			}
		}
	}
	return false;
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

template <class Collection>
auto erase(Collection &collection, typename Collection::Element *value) {
	return collection.erase(value);
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
template <class Collection, class Predicate>
bool find_if_and_erase_unordered(Collection &collection, Predicate predicate) {
	auto found = find_if(collection, predicate);
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

#ifndef TL_USE_CONTEXT
extern TL_API thread_local Allocator current_allocator;
#endif

struct AllocationResult {
	u8 *data = 0;
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
			if (new_count > old_count) {
				for (auto it = (T *)result.data + old_count; it != (T *)result.data + new_count; ++it) {
					new (it) T();
				}
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

	inline static Allocator current() { return TL_GET_CURRENT(allocator); }

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

struct TL_API DefaultAllocator : AllocatorBase<DefaultAllocator> {

	static DefaultAllocator current() { return {}; }

	bool is_valid() {
		return true;
	}

	AllocationResult allocate_impl(umm size, umm alignment TL_LP);
	AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP);
	void deallocate_impl(void *data, umm size, umm alignment TL_LP);
};
struct ArenaAllocator : AllocatorBase<ArenaAllocator> {
	Allocator parent_allocator;
	umm buffer_size = 0;
	u8 *base = 0;
	u8 *cursor = 0;

	struct Checkpoint {
		u8 *cursor;
	};

	forceinline static ArenaAllocator create(umm size TL_LP) {
		ArenaAllocator result = {};
		result.parent_allocator = TL_GET_CURRENT(allocator);
		result.buffer_size = size;
		result.cursor = result.base = result.parent_allocator.allocate_uninitialized<u8>(size TL_LA);
		return result;
	}

	forceinline static ArenaAllocator current() { return {}; }

	forceinline Checkpoint checkpoint() { 
		return {cursor};
	}
	forceinline void reset(Checkpoint checkpoint) { 
		cursor = checkpoint.cursor;
	}

	forceinline AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		assert(cursor, "arena allocator was not initialized");
		auto target = ceil(cursor, alignment);
		cursor = target + size;
		assert(cursor <= base + buffer_size, "Out of arena memory");
		return AllocationResult { .data = target, .count = size, .is_zeroed = false };
	}
	forceinline AllocationResult reallocate_impl(void *old_data, umm old_size, umm new_size, umm alignment TL_LP) {
		auto new_data = allocate_impl(new_size, alignment);
		memcpy(new_data.data, old_data, old_size);
		return new_data;
	}
	forceinline void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
		(void)data;
		(void)size;
		(void)alignment;
	}

	forceinline operator Allocator() {
		return {
			.func = [](AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) -> AllocationResult {
				return ((ArenaAllocator *)state)->execute(action, data, old_size, new_size, align TL_LA);
			},
			.state = this
		};
	}

	forceinline void clear() {
		cursor = base;
	}
	forceinline void free() {
		if (!base)
			return;

		parent_allocator.free(base);
		base = 0;
		cursor = 0;
		buffer_size = 0;
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
	#ifdef TL_USE_CONTEXT
	ArenaAllocator arena;

	forceinline void init() {
		arena = ArenaAllocator::create(TL_TEMPORARY_STORAGE_CAPACITY);
	}

	forceinline static TemporaryAllocator current() { return TL_GET_CURRENT(temporary_allocator); }

	forceinline AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		return arena.allocate_impl(size, alignment TL_LA);
	}
	forceinline AllocationResult reallocate_impl(void *old_data, umm old_size, umm new_size, umm alignment TL_LP) {
		return arena.reallocate_impl(old_data, old_size, new_size, alignment TL_LA);
	}
	forceinline void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
		return arena.deallocate_impl(data, size, alignment TL_LA);
	}

	static AllocationResult func(AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) {
		return ((TemporaryAllocator *)state)->execute(action, data, old_size, new_size, align TL_LA);
	}

	forceinline operator Allocator() {
		return {
			.func = func,
			.state = this
		};
	}

	forceinline void clear() {
		arena.clear();
	}
	#else
	inline static thread_local ArenaAllocator arena;

	forceinline static void init() {
		arena = ArenaAllocator::create(TL_TEMPORARY_STORAGE_CAPACITY);
	}

	forceinline static TemporaryAllocator current() { return {}; }

	forceinline static AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		return arena.allocate_impl(size, alignment TL_LA);
	}
	forceinline static AllocationResult reallocate_impl(void *old_data, umm old_size, umm new_size, umm alignment TL_LP) {
		return arena.reallocate_impl(old_data, old_size, new_size, alignment TL_LA);
	}
	forceinline static void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
		return arena.deallocate_impl(data, size, alignment TL_LA);
	}

	static AllocationResult func(AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) {
		return ((TemporaryAllocator *)state)->execute(action, data, old_size, new_size, align TL_LA);
	}

	forceinline operator Allocator() {
		return {
			.func = func,
			.state = this
		};
	}
	
	forceinline ArenaAllocator::Checkpoint checkpoint() { 
		return arena.checkpoint();
	}
	forceinline void reset(ArenaAllocator::Checkpoint checkpoint) { 
		arena.reset(checkpoint);
	}

	forceinline static void clear() {
		arena.clear();
	}
	#endif
};

#ifndef TL_USE_CONTEXT
extern TL_API thread_local TemporaryAllocator current_temporary_allocator;
#endif

inline struct TemporaryStorageCheckpoint {} temporary_storage_checkpoint;
inline struct TemporaryAllocatorAndCheckpoint {} temporary_allocator_and_checkpoint;

// Example use:
// auto str = TL_TMP(format("{}, {}", a, b));
#define TL_TMP(x) with(TL_GET_CURRENT(temporary_allocator), x)

extern TL_API void init_allocator(Allocator tempory_allocator_backup = os_allocator);
extern TL_API void deinit_allocator();

#define tl_push(pusher, ...) if(auto CONCAT(_tl_, __LINE__)=pusher(__VA_ARGS__))
#define tl_scoped(current, new) auto CONCAT(_tl_,__LINE__)=current;current=(new);defer{current=CONCAT(_tl_,__LINE__);}

template <class T>
inline void allocate(T *&val) {
	val = TL_GET_CURRENT(allocator).allocate<T>();
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


namespace tl {

struct AllocatorPusher {
	Allocator old_allocator;
	forceinline AllocatorPusher(Allocator new_allocator) {
		old_allocator = TL_GET_CURRENT(allocator);
		TL_GET_CURRENT(allocator) = new_allocator;
	}
	forceinline ~AllocatorPusher() {
		TL_GET_CURRENT(allocator) = old_allocator;
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

#define make_scoped(name, new_thing) \
	::tl::Scoped<std::remove_cvref_t<decltype(new_thing)>> name; \
	name.enter(new_thing); \
	defer { name.exit(); }
#define make_scoped_if(name, new_thing, condition) \
	::tl::Optional<::tl::Scoped<std::remove_cvref_t<decltype(new_thing)>>> name; \
	if (condition) { \
		name.emplace(); \
		name.value().enter(new_thing); \
	} \
	defer { if (name) name.value().exit(); }
#define scoped(new_thing) make_scoped(CONCAT(_scoped_, __LINE__), new_thing)
#define scoped_if(new_thing, condition) make_scoped_if(CONCAT(_scoped_, __LINE__), new_thing, condition)
#define with(new_thing, ...) ([&]()->decltype(auto){scoped(new_thing);return __VA_ARGS__;}())

template <class Thing>
struct ScopedBlock {
	Thing storage;
	Thing *pointer;
	ScopedBlock(Thing &thing) : pointer(&thing) {}
	ScopedBlock(Thing &&thing) : storage(thing), pointer(&storage) {}

	template <class Fn>
	decltype(auto) operator+(Fn &&fn) {
		scoped(*pointer);
		return fn();
	}
};

template <class Thing>
ScopedBlock(Thing) -> ScopedBlock<Thing>;

// Example use:
// withs(temporary_allocator) {
//     do_some_stuff();
//     do_other_stuff();
// };
#define withs(new_thing) ScopedBlock(new_thing) + [&]()

struct ValueChanger {
	template <class Fn>
	struct Impl {
		Fn fn;

		template <class T>
		friend auto operator%(T it, Impl impl) {
			impl.fn(it);
			return it;
		}
	};

	template <class Fn>
	auto operator->*(Fn &&fn) {
		return Impl{fn};
	}
};

// Example use:
// auto my_vector = get_vector() withx { it.x = sqrt(it.x); };
#define withx % ValueChanger{} ->* [&](auto &it)

template <>
struct Scoped<Allocator> {
	Allocator old_allocator;
	void enter(Allocator new_allocator) {
		old_allocator = TL_GET_CURRENT(allocator);
		TL_GET_CURRENT(allocator) = new_allocator;
	}
	void exit() {
		TL_GET_CURRENT(allocator) = old_allocator;
	}
};

template <>
struct Scoped<ArenaAllocator> : Scoped<Allocator> {
	void enter(ArenaAllocator &arena) {
		Scoped<Allocator>::enter(arena);
	}
	void exit() {
		Scoped<Allocator>::exit();
	}
};
template <>
struct Scoped<DefaultAllocator> : Scoped<Allocator> {
	void enter(DefaultAllocator) {
		Scoped<Allocator>::enter(default_allocator);
	}
	void exit() {
		Scoped<Allocator>::exit();
	}
};
template <>
struct Scoped<TemporaryAllocator> : Scoped<Allocator> {
	void enter(TemporaryAllocator) {
		Scoped<Allocator>::enter(TL_GET_CURRENT(temporary_allocator));
	}
	void exit() {
		Scoped<Allocator>::exit();
	}
};

template <>
struct Scoped<TemporaryStorageCheckpoint> {
	ArenaAllocator::Checkpoint checkpoint;

	void enter(TemporaryStorageCheckpoint) {
		checkpoint = TL_GET_CURRENT(temporary_allocator).checkpoint();
	}
	void exit() {
		TL_GET_CURRENT(temporary_allocator).reset(checkpoint);
	}
};

template <>
struct Scoped<TemporaryAllocatorAndCheckpoint> : Scoped<Allocator>, Scoped<TemporaryStorageCheckpoint> {
	void enter(TemporaryAllocatorAndCheckpoint) {
		Scoped<Allocator>::enter(TL_GET_CURRENT(temporary_allocator));
		Scoped<TemporaryStorageCheckpoint>::enter({});
	}
	void exit() {
	    Scoped<TemporaryStorageCheckpoint>::exit();
		Scoped<Allocator>::exit();
	}
};

template <class T>
void rotate(Span<T> span, T *to_be_first) {
	umm left_count = to_be_first - span.data;
	umm right_count = span.count - left_count;

	scoped(temporary_storage_checkpoint);

	if (right_count < left_count) {
		T *temp = TL_GET_CURRENT(temporary_allocator).allocate_uninitialized<T>(right_count);
		memcpy(temp, to_be_first, sizeof(T) * right_count);
		memmove(span.data + right_count, span.data, sizeof(T) * left_count);
		memcpy(span.data, temp, sizeof(T) * right_count);
	} else {
		T *temp = TL_GET_CURRENT(temporary_allocator).allocate_uninitialized<T>(left_count);
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

enum class QuickSortPivot {
	middle,
	last,
};

template <QuickSortPivot pivot_mode = QuickSortPivot::middle, class T>
void quick_sort(Span<T> span, auto less) requires requires(T a, T b) { { less(a, b) } -> std::same_as<bool>; } {
	switch (span.count) {
		case 0:
		case 1:
			return;
		case 2: {
			if (!less(span[0], span[1])) {
				Swap(span[0], span[1]);
			}
			return;
		}
	}

	T pivot = {};
	T *mid = 0;

	switch (pivot_mode) {
		case QuickSortPivot::middle: {
			auto p = midpoint(span.begin(), span.end());
			pivot = *p;
			Swap(*p, span.end()[-1]);
			break;
		}
		case QuickSortPivot::last: {
			pivot = span.end()[-1];
			break;
		}
	}

	mid = span.begin();
	for (auto i = span.begin(); i < span.end() - 1; i++) {
		if (less(*i, pivot)) {
			Swap(*i, *mid);
			mid++;
		}
	}
	Swap(*mid, span.end()[-1]);

	quick_sort(Span<T>{span.begin(), mid}, less);
	quick_sort(Span<T>{mid + 1, span.end()}, less);
}
template <QuickSortPivot pivot_mode = QuickSortPivot::middle, class T>
void quick_sort(Span<T> span, auto selector) requires requires(T a) { selector(a) < selector(a); } {
	return quick_sort(span, [&](T a, T b) { return selector(a) < selector(b); });
}
template <QuickSortPivot pivot_mode = QuickSortPivot::middle, class T>
void quick_sort(Span<T> span) {
	quick_sort<pivot_mode>(span, [](T a, T b) { return a < b; });
}
template <QuickSortPivot pivot_mode = QuickSortPivot::middle, class T, umm size>
void quick_sort(T (&array)[size], auto ...args) {
	quick_sort<pivot_mode>(array_as_span(array), args...);
}

}

#ifdef TL_IMPL

namespace tl {

#if OS_WINDOWS
	#if COMPILER_MSVC
		AllocationResult DefaultAllocator::allocate_impl(umm size, umm alignment TL_LPD) {
			return {
				.data = (u8 *)::_aligned_malloc(size, alignment),
				.count = size,
				.is_zeroed = false,
			};
		}
		AllocationResult DefaultAllocator::reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LPD) {
			(void)old_size;
			return {
				.data = (u8 *)::_aligned_realloc(data, new_size, alignment),
				.count = new_size,
				.is_zeroed = false,
			};
		}
		void DefaultAllocator::deallocate_impl(void *data, umm size, umm alignment TL_LPD) {
			(void)size;
			(void)alignment;
			::_aligned_free(data);
		}
	#elif COMPILER_GCC
		AllocationResult DefaultAllocator::allocate_impl(umm size, umm alignment TL_LPD) {
			return {
				.data = ::__mingw_aligned_malloc(size, alignment),
				.count = size,
				.is_zeroed = false,
			};
		}
		AllocationResult DefaultAllocator::reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LPD) {
			(void)old_size;
			return {
				.data = ::__mingw_aligned_realloc(data, new_size, alignment),
				.count = new_size,
				.is_zeroed = false,
			};
		}
		void DefaultAllocator::deallocate_impl(void *data, umm size, umm alignment TL_LPD) {
			(void)size;
			(void)alignment;
			::__mingw_aligned_free(data);
		}

	#endif
#elif OS_LINUX
	AllocationResult DefaultAllocator::allocate_impl(umm size, umm alignment TL_LPD) {
		// I could not find existing aligned realloc, so I have to implement it myself.
		// realloc needs to know old size so it can memcpy the data. Store it before the data.
		umm extra_storage = max(sizeof(size), alignment);
		void *result; 
		::posix_memalign(&result, alignment, size + extra_storage);
		result = ((u8 *)result) + extra_storage;
		((umm *)result)[-1] = size;
		return {
			.data = result,
			.count = size,
			.is_zeroed = false,
		};
	}
	AllocationResult DefaultAllocator::reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LPD) {
		if (old_size == 0) {
			old_size = ((umm *)data)[-1];
		}
		void *result; 
		::posix_memalign(&result, alignment, new_size);
		memcpy(result, data, old_size);
		free(data);
		return {
			.data = result,
			.count = new_size,
			.is_zeroed = false,
		};
	}
	void DefaultAllocator::deallocate_impl(void *data, umm size, umm alignment TL_LPD) {
		(void)size;
		(void)alignment;
		::free(data);
	}
#endif

#if OS_WINDOWS
AllocationResult page_allocator_proc(AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) {
	(void)state;
#if TL_PARENT_SOURCE_LOCATION
	(void)location;
#endif
	switch (action) {
		case Allocator_allocate: {
			assert(align <= 4096);
			return {
				.data = (u8 *)VirtualAlloc(0, new_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE),
				.count = new_size / 4096 * 4096,
				.is_zeroed = true,
			};
		}
		case Allocator_reallocate: {
			assert(align <= 4096);

			if (old_size / 4096 == new_size / 4096) {
				return {
					.data = (u8 *)data,
					.count = new_size,
					.is_zeroed = true,
				};
			}

			auto ceiled_old_size = ceil(old_size, (umm)4096);
			auto ceiled_new_size = ceil(new_size, (umm)4096);

			if (VirtualAlloc((u8 *)data + ceiled_old_size, ceiled_new_size - ceiled_old_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE)) {
				return {
					.data = (u8 *)data,
					.count = new_size,
					.is_zeroed = true,
				};
			}

			auto new_data = VirtualAlloc(0, ceiled_new_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
			memcpy(new_data, data, old_size);
			VirtualFree(data, 0, MEM_RELEASE);

			return {
				.data = (u8 *)new_data,
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
#elif OS_LINUX

AllocationResult page_allocator_proc(AllocatorAction action, void *data, umm old_size, umm new_size, umm align, void *state TL_LPD) {
	not_implemented();
	return {};
}
#endif

Allocator os_allocator = make_allocator_from<DefaultAllocator>(0);
Allocator page_allocator = {
	.func = page_allocator_proc,
	.state = 0
};
Allocator default_allocator = os_allocator;

#ifndef TL_USE_CONTEXT
thread_local TemporaryAllocator current_temporary_allocator;
thread_local Allocator current_allocator;
#endif

void init_allocator(Allocator temporary_allocator_backup) {
	TL_GET_CURRENT(allocator) = default_allocator;
	withs (temporary_allocator_backup) {
		TL_GET_CURRENT(temporary_allocator).init();
	};
}

void deinit_allocator() {
}

} // namespace tl

#endif

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef TL_ENABLE_TESTS

TL_TEST(common) {
	using namespace tl;
	using namespace integer_literals;

	constexpr auto test_carry = [] {
		u8 ru8;
		u16 ru16;
		u32 ru32;
		u64 ru64;
		bool carry;
	
		add_carry((u8)0xfe, (u8)1, false, &ru8, &carry); assert(ru8 == 0xff); assert(carry == 0);
		add_carry((u8)0xff, (u8)1, false, &ru8, &carry); assert(ru8 == 0); assert(carry == 1);
		add_carry((u8)0xff, (u8)0, true,  &ru8, &carry); assert(ru8 == 0); assert(carry == 1);
		add_carry((u8)0xff, (u8)1, true,  &ru8, &carry); assert(ru8 == 1); assert(carry == 1);
		add_carry((u8)1, (u8)0xfe, false, &ru8, &carry); assert(ru8 == 0xff); assert(carry == 0);
		add_carry((u8)1, (u8)0xff, false, &ru8, &carry); assert(ru8 == 0); assert(carry == 1);
		add_carry((u8)0, (u8)0xff, true,  &ru8, &carry); assert(ru8 == 0); assert(carry == 1);
		add_carry((u8)1, (u8)0xff, true,  &ru8, &carry); assert(ru8 == 1); assert(carry == 1);
	
		add_carry((u16)0xfffe, (u16)1, false, &ru16, &carry); assert(ru16 == 0xffff); assert(carry == 0);
		add_carry((u16)0xffff, (u16)1, false, &ru16, &carry); assert(ru16 == 0); assert(carry == 1);
		add_carry((u16)0xffff, (u16)0, true,  &ru16, &carry); assert(ru16 == 0); assert(carry == 1);
		add_carry((u16)0xffff, (u16)1, true,  &ru16, &carry); assert(ru16 == 1); assert(carry == 1);
		add_carry((u16)1, (u16)0xfffe, false, &ru16, &carry); assert(ru16 == 0xffff); assert(carry == 0);
		add_carry((u16)1, (u16)0xffff, false, &ru16, &carry); assert(ru16 == 0); assert(carry == 1);
		add_carry((u16)0, (u16)0xffff, true,  &ru16, &carry); assert(ru16 == 0); assert(carry == 1);
		add_carry((u16)1, (u16)0xffff, true,  &ru16, &carry); assert(ru16 == 1); assert(carry == 1);
	
		add_carry((u32)0xfffffffe, (u32)1, false, &ru32, &carry); assert(ru32 == 0xffffffff); assert(carry == 0);
		add_carry((u32)0xffffffff, (u32)1, false, &ru32, &carry); assert(ru32 == 0); assert(carry == 1);
		add_carry((u32)0xffffffff, (u32)0, true,  &ru32, &carry); assert(ru32 == 0); assert(carry == 1);
		add_carry((u32)0xffffffff, (u32)1, true,  &ru32, &carry); assert(ru32 == 1); assert(carry == 1);
		add_carry((u32)1, (u32)0xfffffffe, false, &ru32, &carry); assert(ru32 == 0xffffffff); assert(carry == 0);
		add_carry((u32)1, (u32)0xffffffff, false, &ru32, &carry); assert(ru32 == 0); assert(carry == 1);
		add_carry((u32)0, (u32)0xffffffff, true,  &ru32, &carry); assert(ru32 == 0); assert(carry == 1);
		add_carry((u32)1, (u32)0xffffffff, true,  &ru32, &carry); assert(ru32 == 1); assert(carry == 1);
	
		add_carry((u64)0xfffffffffffffffe, (u64)1, false, &ru64, &carry); assert(ru64 == 0xffffffffffffffff); assert(carry == 0);
		add_carry((u64)0xffffffffffffffff, (u64)1, false, &ru64, &carry); assert(ru64 == 0); assert(carry == 1);
		add_carry((u64)0xffffffffffffffff, (u64)0, true,  &ru64, &carry); assert(ru64 == 0); assert(carry == 1);
		add_carry((u64)0xffffffffffffffff, (u64)1, true,  &ru64, &carry); assert(ru64 == 1); assert(carry == 1);
		add_carry((u64)1, (u64)0xfffffffffffffffe, false, &ru64, &carry); assert(ru64 == 0xffffffffffffffff); assert(carry == 0);
		add_carry((u64)1, (u64)0xffffffffffffffff, false, &ru64, &carry); assert(ru64 == 0); assert(carry == 1);
		add_carry((u64)0, (u64)0xffffffffffffffff, true,  &ru64, &carry); assert(ru64 == 0); assert(carry == 1);
		add_carry((u64)1, (u64)0xffffffffffffffff, true,  &ru64, &carry); assert(ru64 == 1); assert(carry == 1);

		sub_borrow((u8)0, (u8)1, false, &ru8, &carry); assert(ru8 == 0xff); assert(carry == 1);
		sub_borrow((u8)0, (u8)0, true,  &ru8, &carry); assert(ru8 == 0xff); assert(carry == 1);
		sub_borrow((u8)0, (u8)1, true,  &ru8, &carry); assert(ru8 == 0xfe); assert(carry == 1);
		sub_borrow((u8)1, (u8)0, true,  &ru8, &carry); assert(ru8 == 0); assert(carry == 0);

		sub_borrow((u16)0, (u16)1, false, &ru16, &carry); assert(ru16 == 0xffff); assert(carry == 1);
		sub_borrow((u16)0, (u16)0, true,  &ru16, &carry); assert(ru16 == 0xffff); assert(carry == 1);
		sub_borrow((u16)0, (u16)1, true,  &ru16, &carry); assert(ru16 == 0xfffe); assert(carry == 1);
		sub_borrow((u16)1, (u16)0, true,  &ru16, &carry); assert(ru16 == 0); assert(carry == 0);

		sub_borrow((u32)0, (u32)1, false, &ru32, &carry); assert(ru32 == 0xffffffff); assert(carry == 1);
		sub_borrow((u32)0, (u32)0, true,  &ru32, &carry); assert(ru32 == 0xffffffff); assert(carry == 1);
		sub_borrow((u32)0, (u32)1, true,  &ru32, &carry); assert(ru32 == 0xfffffffe); assert(carry == 1);
		sub_borrow((u32)1, (u32)0, true,  &ru32, &carry); assert(ru32 == 0); assert(carry == 0);

		sub_borrow((u64)0, (u64)1, false, &ru64, &carry); assert(ru64 == 0xffffffffffffffff); assert(carry == 1);
		sub_borrow((u64)0, (u64)0, true,  &ru64, &carry); assert(ru64 == 0xffffffffffffffff); assert(carry == 1);
		sub_borrow((u64)0, (u64)1, true,  &ru64, &carry); assert(ru64 == 0xfffffffffffffffe); assert(carry == 1);
		sub_borrow((u64)1, (u64)0, true,  &ru64, &carry); assert(ru64 == 0); assert(carry == 0);
	};

	constexpr int x = (test_carry(), 1);
	test_carry();

	constexpr auto test_split = [] (Span<char> input, char sep, Span<Span<char>> expected) {

		umm i = 0;

		split_by_one(input, sep, [&](Span<char> span) {
			assert(span == expected[i++]);
		});

		i = 0;
		foreach(it, split_by_one(input, sep)) {
			assert(it.value() == expected[i++]);
		}

	};
	test_split("hello world !"s, ' ', { "hello"s, "world"s, "!"s });
	test_split(" hello world ! "s, ' ', { ""s, "hello"s, "world"s, "!"s, ""s });


	/* replace_inplace */ {

		/* replace with smaller */ {
			int arr[] = { 1, 2, 3, 1, 2, 3};

			auto s = Span(arr, 6_umm);
			replace_inplace(s, Span{2, 3}, Span{4});

			assert(s.data == arr);
			assert(s.count == 4);
			assert(arr[0] == 1);
			assert(arr[1] == 4);
			assert(arr[2] == 1);
			assert(arr[3] == 4);
		}

		/* replace with bigger, forward */ {
			int arr[] = { 1, 2, 3, 1, 2, 3, 1, 2, 3, 0};

			auto s = Span(arr, 9_umm);
			replace_inplace(s, Span{2, 3}, Span{4, 5, 6}, {.capacity = count_of(arr), .reverse = false});

			assert(s.data == arr);
			assert(s.count == 10);
			assert(arr[0] == 1);
			assert(arr[1] == 4);
			assert(arr[2] == 5);
			assert(arr[3] == 6);
			assert(arr[4] == 1);
			assert(arr[5] == 4);
			assert(arr[6] == 5);
			assert(arr[7] == 6);
			assert(arr[8] == 1);
			assert(arr[9] == 2); // this was cut off so not replaced.
		}

		/* replace with bigger, reverse */ {
			int arr[] = { 1, 2, 3, 1, 2, 3, 1, 2, 3, 0};

			auto s = Span(arr, 9_umm);
			replace_inplace(s, Span{2, 3}, Span{4, 5, 6}, {.capacity = count_of(arr), .reverse = true});

			assert(s.data == arr);
			assert(s.count == 10);
			assert(arr[0] == 1);
			assert(arr[1] == 4);
			assert(arr[2] == 5);
			assert(arr[3] == 6);
			assert(arr[4] == 1);
			assert(arr[5] == 4);
			assert(arr[6] == 5);
			assert(arr[7] == 6);
			assert(arr[8] == 1);
			assert(arr[9] == 4); // should be replaced even when cut off
		}
	}
};

#endif
