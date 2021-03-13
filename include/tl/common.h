#pragma once
#include "system.h"

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif

#ifdef TL_IMPL
#if OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
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
#define ASSERTION_FAILURE(causeString, expression, ...) debug_break()
#endif

#define assert_always(x, ...)                              \
	do {                                                   \
		if (!(x)) {                                        \
			ASSERTION_FAILURE("assert", #x, __VA_ARGS__);  \
		}                                                  \
	} while (0)
#define assert(x, ...) assert_always(x, ...)  

#define invalid_code_path(...)                                   \
	do {                                                         \
		ASSERTION_FAILURE("invalid_code_path", "", __VA_ARGS__); \
	} while (0)

#ifdef TL_DEBUG
#define TL_BOUNDS_CHECK(x) assert(x, "Bounds check failed")
#else
#define TL_BOUNDS_CHECK(x)
#endif

#define TL_DISABLED_WARNINGS \
5026 /* implicitly deleted move constructor	*/ \
5027 /* implicitly deleted move assigmnent  */

#if COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: TL_DISABLED_WARNINGS)
#pragma warning(disable: 4820)
#endif

namespace TL {

inline constexpr umm length(char const *str) {
	umm result = 0;
	while (*str++)
		++result;
	return result;
}
inline constexpr umm length(wchar const *str) {
	umm result = 0;
	while (*str++)
		++result;
	return result;
}
inline constexpr umm length(char  *str) { return length((char  const *)str); }
inline constexpr umm length(wchar *str) { return length((wchar const *)str); }

#define TL_HANDLE_NAME(name) CONCAT(name, Impl)
#define TL_DECLARE_HANDLE(name) typedef struct TL_HANDLE_NAME(name) *name;
#define TL_DEFINE_HANDLE(name) struct TL_HANDLE_NAME(name)


template <class T, class U> inline constexpr bool isSame = false;
template <class T> inline constexpr bool isSame<T, T> = true;

template <class T> inline constexpr bool isInteger = false;
template <> inline constexpr bool isInteger<u8 > = true;
template <> inline constexpr bool isInteger<u16> = true;
template <> inline constexpr bool isInteger<u32> = true;
template <> inline constexpr bool isInteger<u64> = true;
template <> inline constexpr bool isInteger<s8 > = true;
template <> inline constexpr bool isInteger<s16> = true;
template <> inline constexpr bool isInteger<s32> = true;
template <> inline constexpr bool isInteger<s64> = true;
template <> inline constexpr bool isInteger<slong> = true;
template <> inline constexpr bool isInteger<ulong> = true;

template <class T> inline constexpr bool isChar = false;
template <> inline constexpr bool isChar<u8> = true;
template <> inline constexpr bool isChar<s8> = true;

template <class T> struct isSigned_ {};
template <> struct isSigned_<s8   > { inline static constexpr bool value = true; };
template <> struct isSigned_<s16  > { inline static constexpr bool value = true; };
template <> struct isSigned_<s32  > { inline static constexpr bool value = true; };
template <> struct isSigned_<s64  > { inline static constexpr bool value = true; };
template <> struct isSigned_<slong> { inline static constexpr bool value = true; };
template <> struct isSigned_<u8   > { inline static constexpr bool value = false; };
template <> struct isSigned_<u16  > { inline static constexpr bool value = false; };
template <> struct isSigned_<u32  > { inline static constexpr bool value = false; };
template <> struct isSigned_<u64  > { inline static constexpr bool value = false; };
template <> struct isSigned_<ulong> { inline static constexpr bool value = false; };

template <class T> inline constexpr bool isSigned = isSigned_<T>::value;

template <class T> inline constexpr bool isUnsigned = false;
template <> inline constexpr bool isUnsigned<u8 > = true;
template <> inline constexpr bool isUnsigned<u16> = true;
template <> inline constexpr bool isUnsigned<u32> = true;
template <> inline constexpr bool isUnsigned<u64> = true;
template <> inline constexpr bool isUnsigned<ulong> = true;

template <class T> inline constexpr bool isFloat = false;
template <> inline constexpr bool isFloat<f32> = true;
template <> inline constexpr bool isFloat<f64> = true;

template <class T> inline constexpr bool isPointer = false;
template <class T> inline constexpr bool isPointer<T *> = true;

template <class T> inline constexpr bool isLValueReference = false;
template <class T> inline constexpr bool isLValueReference<T&> = true;

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

template<> inline static constexpr u8  min_value<u8 > = 0;
template<> inline static constexpr u16 min_value<u16> = 0;
template<> inline static constexpr u32 min_value<u32> = 0;
template<> inline static constexpr u64 min_value<u64> = 0;
template<> inline static constexpr u8  max_value<u8 > = 0xFF;
template<> inline static constexpr u16 max_value<u16> = 0xFFFF;
template<> inline static constexpr u32 max_value<u32> = 0xFFFFFFFF;
template<> inline static constexpr u64 max_value<u64> = 0xFFFFFFFFFFFFFFFF;

template<> inline static constexpr s8  min_value<s8 > = 0x80;
template<> inline static constexpr s16 min_value<s16> = 0x8000;
template<> inline static constexpr s32 min_value<s32> = 0x80000000;
template<> inline static constexpr s64 min_value<s64> = 0x8000000000000000;
template<> inline static constexpr s8  max_value<s8 > = 0x7F;
template<> inline static constexpr s16 max_value<s16> = 0x7FFF;
template<> inline static constexpr s32 max_value<s32> = 0x7FFFFFFF;
template<> inline static constexpr s64 max_value<s64> = 0x7FFFFFFFFFFFFFFF;

template<> inline static constexpr ulong min_value<ulong> = (ulong)min_value<ulong_s>;
template<> inline static constexpr ulong max_value<ulong> = (ulong)max_value<ulong_s>;
template<> inline static constexpr slong min_value<slong> = (slong)min_value<slong_s>;
template<> inline static constexpr slong max_value<slong> = (slong)max_value<slong_s>;

template<> inline static constexpr f32 min_value<f32> = 1.175494351e-38f;
template<> inline static constexpr f32 max_value<f32> = 3.402823466e+38f;
template<> inline static constexpr f64 min_value<f64> = 2.2250738585072014e-308;
template<> inline static constexpr f64 max_value<f64> = 1.7976931348623158e+308;

#pragma warning(pop)

template <class T>
forceinline constexpr bool is_power_of_2(T v) { return (v != 0) && ((v & (v - 1)) == 0); }

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
	if constexpr (isSigned<T>) {
		return ((v < 0) ? ((v + 1) / s - 1) : (v / s)) * s;
	} else {
		return v / s * s;
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
	if constexpr(isSigned<T>) {
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

forceinline u32 count_leading_ones(u8  val) { return count_leading_zeros((u8 )~val); }
forceinline u32 count_leading_ones(u16 val) { return count_leading_zeros((u16)~val); }
forceinline u32 count_leading_ones(u32 val) { return count_leading_zeros((u32)~val); }
forceinline u32 count_leading_ones(u64 val) { return count_leading_zeros((u64)~val); }

forceinline s32 log2(u64 n) { return 63 - (s32)count_leading_zeros(n); }
forceinline s32 log2(u32 n) { return 31 - (s32)count_leading_zeros(n); }
forceinline s32 log2(u16 n) { return 15 - (s32)count_leading_zeros(n); }
forceinline s32 log2(u8  n) { return  7 - (s32)count_leading_zeros(n); }

#define S(k, m) if (n >= (decltype(n))m) { i += k; n /= (decltype(n))m; }
forceinline constexpr s32 log10(u64 n) { s32 i = -(n == 0); S(16,10000000000000000); S(8,100000000); S(4,10000); S(2,100); S(1,10); return i; }
forceinline constexpr s32 log10(u32 n) { s32 i = -(n == 0); S(8,100000000); S(4,10000); S(2,100); S(1,10); return i; }
forceinline constexpr s32 log10(u16 n) { s32 i = -(n == 0); S(4,10000); S(2,100); S(1,10); return i; }
forceinline constexpr s32 log10(u8  n) { s32 i = -(n == 0); S(2,100); S(1,10); return i; }
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
forceinline u8  rotateLeft (u8  v, s32 shift = 1) { return (v << shift) | (v >> ( 8 - shift)); }
forceinline u16 rotateLeft (u16 v, s32 shift = 1) { return (v << shift) | (v >> (16 - shift)); }
forceinline u32 rotateLeft (u32 v, s32 shift = 1) { return (v << shift) | (v >> (32 - shift)); }
forceinline u64 rotateLeft (u64 v, s32 shift = 1) { return (v << shift) | (v >> (64 - shift)); }
forceinline u8  rotateRight(u8  v, s32 shift = 1) { return (v >> shift) | (v << ( 8 - shift)); }
forceinline u16 rotateRight(u16 v, s32 shift = 1) { return (v >> shift) | (v << (16 - shift)); }
forceinline u32 rotateRight(u32 v, s32 shift = 1) { return (v >> shift) | (v << (32 - shift)); }
forceinline u64 rotateRight(u64 v, s32 shift = 1) { return (v >> shift) | (v << (64 - shift)); }
#else
forceinline u8  rotateLeft (u8  v, s32 shift = 1) { return _rotl8(v, (u8)shift); }
forceinline u16 rotateLeft (u16 v, s32 shift = 1) { return _rotl16(v, (u8)shift); }
forceinline u32 rotateLeft (u32 v, s32 shift = 1) { return _rotl(v, shift); }
forceinline u64 rotateLeft (u64 v, s32 shift = 1) { return _rotl64(v, shift); }
forceinline u8  rotateRight(u8  v, s32 shift = 1) { return _rotr8(v, (u8)shift); }
forceinline u16 rotateRight(u16 v, s32 shift = 1) { return _rotr16(v, (u8)shift); }
forceinline u32 rotateRight(u32 v, s32 shift = 1) { return _rotr(v, shift); }
forceinline u64 rotateRight(u64 v, s32 shift = 1) { return _rotr64(v, shift); }
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
	Range r;
	r._begin = x.rbegin();
	r._end = x.rend();
	return r;
}

template <class T>
struct Span {
	using ValueType = T;
	constexpr Span() = default;
	constexpr Span(ValueType &value) : data(std::addressof(value)), size(1) {}
	template <umm count>
	constexpr Span(ValueType (&array)[count]) : data(array), size(count) {}
	constexpr Span(ValueType *begin, ValueType *end) : data(begin), size(end - begin) {}
	constexpr Span(ValueType *begin, umm size) : data(begin), size(size) {}
	constexpr ValueType *begin() const { return data; }
	constexpr ValueType *end() const { return data + size; }
	constexpr ValueType &front() const { return *data; }
	constexpr ValueType &back() const { return data[size - 1]; }
	constexpr ValueType &operator[](umm i) const { return data[i]; }
	constexpr ValueType &at(umm i) const { return data[i]; }
	constexpr bool empty() const { return size == 0; }

	constexpr explicit operator Span<s8>() const { return {(s8 *)data, size * sizeof(ValueType)}; }
	constexpr explicit operator Span<u8>() const { return {(u8 *)data, size * sizeof(ValueType)}; }
	constexpr explicit operator Span<char>() const { return {(char *)data, size * sizeof(ValueType)}; } // Yes, there are three one-byte types

	constexpr bool operator==(Span<ValueType> that) const {
		if (size != that.size)
			return false;
		for (umm i = 0; i < size; ++i) {
			if (data[i] != that.data[i])
				return false;
		}
		return true;
	}

	ValueType *data = 0;
	umm size = 0;
};

forceinline Span<char> operator""s(char const *string, umm size) {
	return Span((char *)string, size);
}

//template <class T, umm size>
//inline constexpr Span<T const> as_span(T const (&str)[size]) { return Span(str, size); }

inline constexpr Span<char > as_span(char  const *str) { return Span((char  *)str, length(str)); }
inline constexpr Span<wchar> as_span(wchar const *str) { return Span((wchar *)str, length(str)); }

inline constexpr Span<char > as_span(char const *begin, char const *end) { return Span((char *)begin, (char *)end); }

template <class T>
inline constexpr Span<T> as_span(Span<T> span) { return span; }

template <class T> constexpr Span<u8> as_bytes(Span<T> span) { return {(u8 *)span.begin(), span.size * sizeof(T)}; }
template <class T> constexpr Span<char> as_chars(Span<T> span) { return {(char *)span.begin(), span.size * sizeof(T)}; }
template <class T> constexpr Span<u8> value_as_bytes(T &value) { return {(u8 *)&value, sizeof(T)}; }

template <class T> constexpr umm count_of(Span<T> span) { return span.size; }
template <class T> constexpr umm length (Span<T> span) { return span.size; }

template <class T>
constexpr void replace(Span<T> destination, Span<T> source, umm start_index = 0) {
	for (umm i = 0; i < source.size; ++i) {
		destination[start_index + i] = source[i];
	}
}

template <class T> constexpr T *find(Span<T> span, T const &value) { return find(span.begin(), span.end(), value); }
template <class T> constexpr T *find(Span<T> span, Span<T> cmp) { return find(span.begin(), span.end(), cmp.begin(), cmp.end()); }

template <class T, class Predicate>
constexpr T *find_if(Span<T> span, Predicate &&predicate) {
	for (auto &v : span) {
		if (predicate(v)) {
			return std::addressof(v);
		}
	}
	return 0;
}

inline constexpr bool is_whitespace(u32 c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
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

#define COMP_FUNCS(typeA, typeB)                               \
inline constexpr bool equals(typeA const *a, typeB const *b) { \
	for (;;)                                                   \
		if (*a++ != *b++)                                      \
			return false;                                      \
	return true;                                               \
}                                                              \
inline constexpr bool equals(Span<typeA> a, typeB const *b) { \
	for (auto ap = a.begin(); ap != a.end(); ++ap, ++b)       \
		if (*ap != *b)                                        \
			return false;                                     \
	return true;                                              \
}                                                             \
inline constexpr bool equals(typeA const *a, Span<typeB> b) { \
	for (auto bp = b.begin(); bp != b.end(); ++a, ++bp)       \
		if (*a != *bp)                                        \
			return false;                                     \
	return true;                                              \
}                                                             \
inline constexpr bool equals(Span<typeA> a, Span<typeB> b) { \
	if (a.size != b.size)                                    \
		return false;                                        \
	auto ap = a.begin();                                     \
	for (auto bp = b.begin(); ap != a.end(); ++ap, ++bp)     \
		if (*ap != *bp)                                      \
			return false;                                    \
	return true;                                             \
}                                                            \
inline constexpr bool starts_with(typeA const *str, typeB const *subStr) { \
	while (*subStr)                                                        \
		if (*str++ != *subStr++)                                           \
			return false;                                                  \
	return true;                                                           \
}                                                                          \
inline constexpr bool starts_with(typeA const *str, typeB const *subStr, umm substrLength) { \
	while (substrLength--)                                                                   \
		if (*str++ != *subStr++)                                                             \
			return false;                                                                    \
	return true;                                                                             \
}                                                                                            \
inline constexpr bool starts_with(typeA const *str, umm strLength, typeB const *subStr) { \
	while (*subStr && strLength--)                                                        \
		if (*str++ != *subStr++)                                                          \
			return false;                                                                 \
	return !*subStr;                                                                      \
}                                                                                         \
inline constexpr bool starts_with(typeA const *str, umm strLength, typeB const *subStr, umm substrLength) { \
	if (strLength < substrLength)                                                                           \
		return false;                                                                                       \
	while (strLength-- && substrLength--)                                                                   \
		if (*str++ != *subStr++)                                                                            \
			return false;                                                                                   \
	return true;                                                                                            \
}

COMP_FUNCS(char, char)
COMP_FUNCS(char, wchar)
COMP_FUNCS(wchar, char)
COMP_FUNCS(wchar, wchar)

#undef COMP_FUNCS

inline constexpr bool ends_with(Span<char> str, Span<char> sub_str, bool case_sensitive = true) {
	if (sub_str.size > str.size)
		return false;
	umm base_offset = str.size - sub_str.size;
	if (case_sensitive) {
		for (umm i = 0; i < sub_str.size; ++i) {
			if (str.data[i + base_offset] != sub_str.data[i]) {
				return false;
			}
		}
	} else {
		for (umm i = 0; i < sub_str.size; ++i) {
			if (to_lower_ascii(str.data[i + base_offset]) != to_lower_ascii(sub_str.data[i])) {
				return false;
			}
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

template <class CopyFn, class Char>
inline constexpr bool is_copy_fn = std::is_invocable_v<CopyFn, Span<Char>>;

template <class CopyFn, class Char>
using CopyFnRet = decltype(((CopyFn *)0)->operator()(Span<Char>((Char*)0, (umm)0)));

template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (isSigned<Int> ? 1 : 0);

template <class Int>
struct FormatInt {
	Int value;
	u32 radix;
	bool leading_zeros;
	explicit FormatInt(Int value, u32 radix = 10, bool leading_zeros = false) : value(value), radix(radix), leading_zeros(leading_zeros) {}
};

template <class Float>
struct FormatFloat {
	Float value;
	u32 precision;
	explicit FormatFloat(Float value, u32 precision = 3) : value(value), precision(precision) {}
};

template <class Char>
struct NullString{};
template <>
struct NullString<char> {
	inline static constexpr char value[] = "(null)";
};
template <>
struct NullString<wchar> {
	inline static constexpr wchar value[] = L"(null)";
};

template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> to_string(char  const &v, CopyFn &&copy_fn) { Char c = (Char)v; return copy_fn(Span(&c, 1)); }
template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> to_string(wchar const &v, CopyFn &&copy_fn) { Char c = (Char)v; return copy_fn(Span(&c, 1)); }
template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> to_string(Char const *v, CopyFn &&copy_fn) { if (!v) v = NullString<Char>::value; return copy_fn(as_span(v)); }
template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> to_string(Char       *v, CopyFn &&copy_fn) { if (!v) v = (Char *)NullString<Char>::value; return copy_fn(as_span(v)); }
template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> to_string(Span<Char> v, CopyFn &&copy_fn) { return copy_fn(v); }

template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> to_string(bool v, CopyFn &&copy_fn) {
	if constexpr (isSame<Char, char>)
		return copy_fn(as_span(v ? "true" : "false"));
	else
		return copy_fn(as_span(v ? L"true" : L"false"));
}

template <class Char, class CopyFn, class Int>
CopyFnRet<CopyFn, Char> to_string(FormatInt<Int> f, CopyFn &&copy_fn) {
	Int v = f.value;
	u32 radix = f.radix;
	constexpr u32 maxDigits = _intToStringSize<Int>;
	Char buf[maxDigits];
	constexpr char charMap[] = "0123456789ABCDEF";
	Char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if constexpr (isSigned<Int>) {
		if (v < 0) {
			negative = true;
			if (v == min_value<Int>) {
				*lsc-- = charMap[-(v % (Int)radix)];
				v /= (Int)radix;
				++charsWritten;
			} 
			v = -v;
		}
	}

	for (;;) {
		*lsc-- = charMap[v % (Int)radix];
		v /= (Int)radix;
		++charsWritten;
		if (v == 0)
			break;
	}
	if constexpr (isSigned<Int>) {
		if (negative) {
			++charsWritten;
			*lsc-- = '-';
		}
	} else {
		(void)negative;
	}
	if (f.leading_zeros) {
		u32 total_digits = (u32)ceil_to_int(log((f32)max_value<Int>, (f32)f.radix));
		for (u32 i = charsWritten; i < total_digits; ++i) {
			*lsc-- = '0';
		}
		charsWritten = total_digits;
	}
	return copy_fn(Span(lsc + 1, charsWritten));
}

template <class Char, class Int, class CopyFn, class = EnableIf<isInteger<Int> && is_copy_fn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> to_string(Int v, CopyFn &&copy_fn) {
	return to_string<Char>(FormatInt(v), std::forward<CopyFn>(copy_fn));
}

template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> to_string(void const *p, CopyFn &&copy_fn) {
	return to_string<Char>(FormatInt((umm)p, 16, true), std::forward<CopyFn>(copy_fn));
}

template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> to_string(f64 v, CopyFn &&copy_fn) {
	return to_string<Char>(FormatFloat(v), std::forward<CopyFn>(copy_fn));
}
template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> to_string(f32 v, CopyFn &&copy_fn) {
	return to_string<Char>((f64)v, std::forward<CopyFn>(copy_fn));
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

	forceinline constexpr T *begin() { return data; }
	forceinline constexpr T *end() { return data + size; }

	forceinline constexpr bool empty() const { return size == 0; }
	forceinline constexpr bool full() const { return size == capacity; }

	forceinline constexpr T &front() { TL_BOUNDS_CHECK(size); return data[0]; }
	forceinline constexpr T &back() { TL_BOUNDS_CHECK(size); return data[size - 1]; }
	forceinline constexpr T &operator[](umm i) { TL_BOUNDS_CHECK(size); return data[i]; }

	constexpr void resize(umm new_size) {
		TL_BOUNDS_CHECK(new_size <= capacity);
		if (new_size > size) {
			for (umm i = size; i < new_size; ++i) {
				new (data + i) T();
			}
		}
		size = new_size;
	}
	constexpr Span<T> insert_at(Span<T> span, umm where) {
		TL_BOUNDS_CHECK(where <= size);
		TL_BOUNDS_CHECK(size + span.size <= capacity);

		memcpy(data + where + span.size, data + where, span.size * sizeof(T));
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
	forceinline constexpr StaticList &operator+=(std::initializer_list<T> that) { add(Span(that.begin(), that.end())); return *this; }

	forceinline constexpr operator Span<T>() { return {data, size}; }

	forceinline constexpr T &add(T const &value) {
		TL_BOUNDS_CHECK(!full());
		memcpy(data + size, &value, sizeof(T));
		return data[size++];
	}
	
	forceinline constexpr Span<T> add(Span<T> span) {
		TL_BOUNDS_CHECK(size + span.size <= capacity);
		memcpy(data + size, span.data, span.size * sizeof(T));
		defer { size += span.size; };
		return {data + size, span.size};
	}
	constexpr T pop_back() {
		TL_BOUNDS_CHECK(size);
		return data[--size];
	}
	constexpr T pop_front() {
		TL_BOUNDS_CHECK(size);
		T popped = *data;
		memmove(data, data + 1, --size * sizeof(T));
		return popped;
	}
	forceinline constexpr void pop_back_unordered() { erase_unordered(&back()); }
	forceinline constexpr void pop_front_unordered() { erase_unordered(begin()); }
	
	T erase_at(umm where) {
		TL_BOUNDS_CHECK(where < size);
		T erased = data[where];
		memmove(data + where, data + where + 1, --size - where);
		return erased;
	}
	forceinline T erase(T *where) { return erase_at(where - data); }
	forceinline T erase(T &value) { return erase(&value); }

	T erase_at_unordered(umm where) {
		TL_BOUNDS_CHECK(where < size);
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
	
	union {
		T data[capacity];
	};
	umm size = 0;
};

template <class T, umm capacity> Span<T> as_span(StaticList<T, capacity> const &list) { return {(T *)list.data, list.size}; }

template <class T, umm capacity> constexpr T *find(StaticList<T, capacity> &list, T const &value) { return find(as_span(list), value); }
template <class T, umm capacity> constexpr T *find(StaticList<T, capacity> &list, Span<T> cmp) { return find(as_span(list), cmp); }
template <class T, umm capacity> constexpr T const *find(StaticList<T, capacity> const &list, T const &value) { return find(as_span(list), value); }
template <class T, umm capacity> constexpr T const *find(StaticList<T, capacity> const &list, Span<T> cmp) { return find(as_span(list), cmp); }

template <class T, umm capacity, class Predicate> constexpr T *find_if(StaticList<T, capacity> &list, Predicate &&predicate) { return find_if(as_span(list), std::forward<Predicate>(predicate)); }


template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> to_string(FormatFloat<f64> f, CopyFn &&copy_fn) {
	auto v = f.value;
	auto precision = f.precision;
	StaticList<Char, 64> buf;
	if (is_negative(v)) {
		buf += '-';
		v = -v;
	}
	to_string<Char>((u64)v, [&](Span<Char> span){
		buf += span;
	});
	buf += '.';
	for (u32 i = 0; i < precision; ++i) {
		v = v - (f64)(s64)v;
		v = (v < 0 ? v + 1 : v) * 10;
		buf += (Char)((u32)v + '0');
	}
	return copy_fn(as_span(buf));
}

#if COMPILER_GCC
forceinline u32 find_lowest_one_bit(u32 val) { val ? __builtin_ffs(val) : ~0; }
forceinline u32 find_lowest_one_bit(u64 val) { val ? __builtin_ffsll(val) : ~0; }
forceinline u32 find_highest_one_bit(u32 val) { val ? 32 - __builtin_clz(val) : ~0; }
forceinline u32 find_highest_one_bit(u64 val) { val ? 64 - __builtin_clzll(val) : ~0; }
#else
forceinline u32 find_lowest_one_bit(u32 val) {
	unsigned long result;
	return _BitScanForward(&result, (unsigned long)val) ? (u32)result : ~0;
}
forceinline u32 find_highest_one_bit(u32 val) {
	unsigned long result;
	return _BitScanReverse(&result, (unsigned long)val) ? (u32)result : ~0;
}
#if ARCH_X64
forceinline u32 find_lowest_one_bit(u64 val) {
	unsigned long result;
	return _BitScanForward64(&result, val) ? (u32)result : ~0;
}
forceinline u32 find_highest_one_bit(u64 val) {
	unsigned long result;
	return _BitScanReverse64(&result, val) ? (u32)result : ~0;
}
#else
#endif
#endif

forceinline u32 count_bits(u32 v) { return (u32)_mm_popcnt_u32(v); }

#if ARCH_X64
forceinline u32 count_bits(u64 v) { return (u32)_mm_popcnt_u64(v); }
#else
#endif

forceinline u32 count_bits(s32 v) { return count_bits((u32)v); }
forceinline u32 count_bits(s64 v) { return count_bits((u64)v); }

enum AllocatorMode {
	Allocator_allocate,
	Allocator_reallocate,
	Allocator_free,
};

struct Allocator {
	void *(*func)(AllocatorMode mode, umm size, umm align, void *data, void *state);
	void *state;
	operator bool() {
		return func != 0;
	}
};

template <class T>
T *_allocate(Allocator allocator, umm count = 1, umm align = alignof(T)) {
	return (T *)allocator.func(Allocator_allocate, sizeof(T) * count, align, 0, allocator.state);
}
inline void *_reallocate(Allocator allocator, void *data) {
	return allocator.func(Allocator_reallocate, 0, 0, data, allocator.state);
}
inline void _free(Allocator allocator, void *data) {
	allocator.func(Allocator_free, 0, 0, data, allocator.state);
}

#ifdef TL_TRACK_ALLOCATIONS

#ifdef TL_ALLOCATION_TRACKER
	#ifndef TL_DEALLOCATION_TRACKER
		#error TL_ALLOCATION_TRACKER and TL_DEALLOCATION_TRACKER must be defined if allocation tracking is enabled
	#endif
#else
	#ifdef TL_DEALLOCATION_TRACKER
		#error TL_ALLOCATION_TRACKER and TL_DEALLOCATION_TRACKER must be defined if allocation tracking is enabled
	#else
		#define TL_ALLOCATION_TRACKER(t, allocator, ...) (track_allocations ? ::TL::allocation_tracker<t>(get_stack_trace(), allocator, __VA_ARGS__) : ::TL::_allocate<t>(allocator, __VA_ARGS__))
		#define TL_DEALLOCATION_TRACKER(allocator, data) (track_allocations ? ::TL::deallocation_tracker(allocator, data) : ::TL::_free(allocator, data))
	#endif
#endif

extern bool track_allocations;

struct StackTrace;
StackTrace get_stack_trace();

template <class T>
T *allocation_tracker(StackTrace const &trace, Allocator allocator, umm count = 1, umm align = alignof(T));
void deallocation_tracker(Allocator allocator, void *data);

#define ALLOCATE(t, allocator, ...) TL_ALLOCATION_TRACKER(t, allocator, __VA_ARGS__)
#define FREE(allocator, data) TL_DEALLOCATION_TRACKER(allocator, data)

#else

#define ALLOCATE(t, allocator, ...) ::TL::_allocate<t>(allocator, __VA_ARGS__)
#define FREE(allocator, data) ::TL::_free(allocator, data)

#endif

extern TL_API void init_allocator();
extern TL_API Allocator default_allocator;
extern TL_API Allocator thread_local current_allocator;

struct AllocatorPusher {
	Allocator old_allocator;
	AllocatorPusher(Allocator new_allocator) {
		old_allocator = current_allocator;
		current_allocator = new_allocator;
	}
	~AllocatorPusher() {
		current_allocator = old_allocator;
	}
	operator bool() { return true; }
};

#define push_allocator(allocator) if(auto CONCAT(_,__LINE__) = AllocatorPusher(allocator))

#ifdef TL_IMPL

Allocator default_allocator = {
	[](AllocatorMode mode, umm size, umm align, void *data, void *) -> void * {
		switch (mode) {
#if OS_WINDOWS
#if COMPILER_MSVC
			case Allocator_allocate:   return _aligned_malloc(size, align);
			case Allocator_reallocate: return _aligned_realloc(data, size, align);
			case Allocator_free:       _aligned_free(data); return 0;
#elif COMPILER_GCC
			case Allocator_allocate:   return __mingw_aligned_malloc(size, align);
			case Allocator_reallocate: return __mingw_aligned_realloc(data, size, align);
			case Allocator_free:       __mingw_aligned_free(data); return 0;
#endif
#else
			case Allocator_allocate: {
				void *result = 0;
				posix_memalign(&result, max(align, sizeof(void *)), size);
				return result;
			}
			case Allocator_reallocate: return realloc(data, size);
			case Allocator_free:       free(data); return 0;
#endif
		}
		return 0;
	},
	0
};
thread_local Allocator current_allocator;

void init_allocator() {
	current_allocator = default_allocator;
}

#endif

} // namespace TL

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef TL_TRACK_ALLOCATIONS

#include "thread.h"
#include "debug.h"

namespace TL {

struct AllocationInfo {
	StackTrace trace;
};

std::unordered_map<void *, AllocationInfo> allocations;
Mutex allocations_mutex;
bool track_allocations = true;

template <class T>
T *allocation_tracker(StackTrace const &trace, Allocator allocator, umm count, umm align) {
	auto result = _allocate<T>(allocator, count, align);

	AllocationInfo info;
	info.trace = trace;

	scoped_lock(allocations_mutex);
	allocations[result] = info;

	return result;
}
void deallocation_tracker(Allocator allocator, void *data) {
	_free(allocator, data);

	scoped_lock(allocations_mutex);
	allocations.erase(data);
}

}

#endif
