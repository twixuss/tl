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

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#ifndef ASSERTION_FAILURE
#define ASSERTION_FAILURE(file, line, function, causeString, expression, ...) DEBUG_BREAK
#endif

#define ASSERT(x, ...)                                    \
	do {                                                  \
		if (!(x)) {                                       \
			ASSERTION_FAILURE(__FILE__, __LINE__, __FUNCTION__, "ASSERT", #x, __VA_ARGS__); \
		}                                                 \
	} while (0)

#define INVALID_CODE_PATH(...)                                   \
	do {                                                         \
		ASSERTION_FAILURE(__FILE__, __LINE__, __FUNCTION__, "INVALID_CODE_PATH", "", __VA_ARGS__); \
	} while (0)

#ifdef TL_NO_BOUNDS_CHECK
#define TL_BOUNDS_CHECK(x)
#else
#define TL_BOUNDS_CHECK(x) ASSERT(x, "Bounds check failed")
#endif

namespace TL {

#define TL_DECLARE_HANDLE(name) typedef struct {} *name;

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

template <class T> inline constexpr bool isSigned = false;
template <> inline constexpr bool isSigned<s8 > = true;
template <> inline constexpr bool isSigned<s16> = true;
template <> inline constexpr bool isSigned<s32> = true;
template <> inline constexpr bool isSigned<s64> = true;
template <> inline constexpr bool isSigned<slong> = true;

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

template <class T> struct RemoveReferenceT       { using Type = T; };
template <class T> struct RemoveReferenceT<T &>  { using Type = T; };
template <class T> struct RemoveReferenceT<T &&> { using Type = T; };
template <class T> using RemoveReference = typename RemoveReferenceT<T>::Type;

template <class T> struct RemoveConstT		    { using Type = T; };
template <class T> struct RemoveConstT<T const> { using Type = T; };
template <class T> using RemoveConst = typename RemoveConstT<T>::Type;

template <class T> struct RemoveVolatileT		      { using Type = T; };
template <class T> struct RemoveVolatileT<T volatile> { using Type = T; };
template <class T> using RemoveVolatile = typename RemoveVolatileT<T>::Type;

template <class T> using RemoveCVRef = RemoveConst<RemoveVolatile<RemoveReference<T>>>;

template <bool v, class T, class F> struct ConditionalT { using Type = T; };
template <class T, class F> struct ConditionalT<false, T, F> { using Type = F; };
template <bool v, class T, class F> using Conditional = typename ConditionalT<v, T, F>::Type;

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
// clang-format off
template<class T> inline constexpr T min() = delete;
template<class T> inline constexpr T max() = delete;
template<> inline constexpr u8  min() { return 0; }
template<> inline constexpr u16 min() { return 0; }
template<> inline constexpr u32 min() { return 0; }
template<> inline constexpr u64 min() { return 0; }
template<> inline constexpr u8  max() { return 0xFF; }
template<> inline constexpr u16 max() { return 0xFFFF; }
template<> inline constexpr u32 max() { return 0xFFFFFFFF; }
template<> inline constexpr u64 max() { return 0xFFFFFFFFFFFFFFFF; }

template<> inline constexpr s8  min() { return 0x80; }
template<> inline constexpr s16 min() { return 0x8000; }
template<> inline constexpr s32 min() { return 0x80000000; }
template<> inline constexpr s64 min() { return 0x8000000000000000; }
template<> inline constexpr s8  max() { return 0x7F; }
template<> inline constexpr s16 max() { return 0x7FFF; }
template<> inline constexpr s32 max() { return 0x7FFFFFFF; }
template<> inline constexpr s64 max() { return 0x7FFFFFFFFFFFFFFF; }

template<> inline constexpr long unsigned min() { return (long unsigned)min<u32>(); }
template<> inline constexpr long unsigned max() { return (long unsigned)max<u32>(); }
template<> inline constexpr long   signed min() { return (long   signed)min<s32>(); }
template<> inline constexpr long   signed max() { return (long   signed)max<s32>(); }
// clang-format on
#pragma warning(pop)

FORCEINLINE constexpr bool isPowerOf2(u8  v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(u16 v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(u32 v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(u64 v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(s8  v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(s16 v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(s32 v) { return v && ((v & (v - 1)) == 0); }
FORCEINLINE constexpr bool isPowerOf2(s64 v) { return v && ((v & (v - 1)) == 0); }

template <class T, class U> FORCEINLINE constexpr auto min(T a, U b) { return a < b ? a : b; }
template <class T, class U> FORCEINLINE constexpr auto max(T a, U b) { return a > b ? a : b; }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto min(T a, U b, Rest... rest) { return min(min(a, b), rest...); }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto max(T a, U b, Rest... rest) { return max(max(a, b), rest...); }
template <class T, class U, class V, class W> FORCEINLINE constexpr void minmax(T a, U b, V& mn, W& mx) { mn = min(a, b); mx = max(a, b); }

FORCEINLINE constexpr u8  floor(u8  v, u8  s) { return (u8 )(v / s * s); }
FORCEINLINE constexpr u16 floor(u16 v, u16 s) { return (u16)(v / s * s); }
FORCEINLINE constexpr u32 floor(u32 v, u32 s) { return (u32)(v / s * s); }
FORCEINLINE constexpr u64 floor(u64 v, u64 s) { return (u64)(v / s * s); }

FORCEINLINE constexpr u8  ceil(u8  v, u8  s) { return floor((u8 )(v + s - 1), s); }
FORCEINLINE constexpr u16 ceil(u16 v, u16 s) { return floor((u16)(v + s - 1), s); }
FORCEINLINE constexpr u32 ceil(u32 v, u32 s) { return floor((u32)(v + s - 1), s); }
FORCEINLINE constexpr u64 ceil(u64 v, u64 s) { return floor((u64)(v + s - 1), s); }

FORCEINLINE constexpr void *floor(void *v, umm s) { return (void *)floor((umm)v, s); }
FORCEINLINE constexpr void *ceil(void *v, umm s) { return floor((u8 *)v + s - 1, s); }

FORCEINLINE constexpr bool isNegative(f32 v) { return *(u32 *)&v & 0x80000000; }
FORCEINLINE constexpr bool isNegative(f64 v) { return *(u64 *)&v & 0x8000000000000000; }

template <class T> FORCEINLINE constexpr T select(bool mask, T a, T b) { return mask ? a : b; }

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
constexpr umm countof(T const (&arr)[size]) { (void)arr; return size; }

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

#define DEFER ::TL::Deferrer CONCAT(_deferrer, __LINE__) = [&]()

template <class T>
struct Range {
	struct Iterator {
		T value;
		constexpr Iterator(T value) : value(value) {}
		constexpr Iterator &operator++() { return ++value, *this; }
		constexpr Iterator operator++(int) { return value++; }
		constexpr bool operator==(Iterator const &that) const { return value == that.value; }
		constexpr bool operator!=(Iterator const &that) const { return value != that.value; }
		constexpr T &operator*() { return value; }
		constexpr T *operator->() { return &value; }
	};
	constexpr Range(T const &begin, T const &end) : _begin(begin), _end(end) {}
	constexpr Range(T const &end) : _begin(), _end(end) {}
	constexpr Iterator begin() { return _begin; }
	constexpr Iterator end() { return _end; }
	T _begin{};
	T _end{};
};

template <class T> Range<T> exclusiveRange(T begin, T end) { return Range<T>(begin, end); }
template <class T> Range<T> inclusiveRange(T begin, T end) { return Range<T>(begin, end + 1); }

template <class T>
auto reverse(T &x) {
	using Ptr = decltype(x.begin());
	struct Iter {
		Iter &operator++() { --ptr; return *this; }
		Iter &operator--() { ++ptr; return *this; }
		Iter operator++(int) { Iter prev = *this; --ptr; return prev; }
		Iter operator--(int) { Iter prev = *this; ++ptr; return prev; }
		bool operator==(Iter const &that) { return ptr == that.ptr; }
		bool operator!=(Iter const &that) { return ptr != that.ptr; }
		auto &operator*() { return *ptr; }
		Ptr ptr;
	};
	struct Range {
		Iter begin() { return {_begin}; }
		Iter end() { return {_end}; }
		Ptr _begin, _end;
	};
	Range r;
	r._begin = x.end() - 1;
	r._end = x.begin() - 1;
	return r;
}

template <class T>
struct Span {
	constexpr Span() = default;
	constexpr Span(T &value) : _begin(std::addressof(value)), _end(_begin + 1) {}
	template <umm count>
	constexpr Span(T (&value)[count]) : _begin(value), _end(_begin + count) {}
	constexpr Span(T *begin, T *end) : _begin(begin), _end(end) {}
	constexpr Span(T *begin, umm size) : Span(begin, begin + size) {}
	constexpr T *data() const { return _begin; }
	constexpr T *begin() const { return _begin; }
	constexpr T *end() const { return _end; }
	constexpr T &front() const { return *_begin; }
	constexpr T &back() const { return _end[-1]; }
	constexpr umm size() const { return umm(_end - _begin); }
	constexpr T &operator[](umm i) { return _begin[i]; }
	constexpr T const &operator[](umm i) const { return _begin[i]; }
	constexpr T &at(umm i) { return _begin[i]; }
	constexpr T const &at(umm i) const { return _begin[i]; }

	constexpr bool empty() const { return _begin == _end; }

	constexpr operator Span<T const>() const { return {_begin, _end}; }

	T *_begin{};
	T *_end{};
};

template <class T>
constexpr umm countof(Span<T const> span) { return span.size(); }

using StringView = Span<char const>;
using StringSpan = Span<char>;

using FnAllocate   = void *(*)(void *state, umm size, umm align);
using FnDeallocate = void  (*)(void *state, void *data);

struct Allocator {
	FnAllocate   _allocate   = 0;
	FnDeallocate _deallocate = 0;
	void *_state = 0;
	
	void *allocate(umm size, umm align = 0) { return _allocate(_state, size, align); }

	template <class T>
	T *allocate(umm count = 1, umm align = 0) {
		return (T *)allocate(count * sizeof(T), max(alignof(T), align));
	}

	void deallocate(void *data) { _deallocate(_state, data); }
};

inline Allocator makeAllocator(void *state, FnAllocate allocate, FnDeallocate deallocate) {
	Allocator result;
	result._allocate = allocate;
	result._deallocate = deallocate;
	result._state = state;
	return result;
}

extern Allocator osAllocator;

template <class T, class Allocator>
static T *allocate(umm count = 1, umm align = 0) {
	return (T *)Allocator::allocate(count * sizeof(T), max(alignof(T), align));
}

struct OsAllocator {
#if OS_WINDOWS
#if COMPILER_MSVC
	inline static void *allocate(umm size, umm align = 0) { return _aligned_malloc(size, max(align, (umm)8)); }
	inline static void deallocate(void *data) { _aligned_free(data); }
#elif COMPILER_GCC
	inline static void *allocate(umm size, umm align = 0) { return __mingw_aligned_malloc(size, max(align, (umm)8)); }
	inline static void deallocate(void *data) { __mingw_aligned_free(data); }
#endif
#else
	inline static void *allocate(umm size, umm align = 0) {
		void *result = 0;
		posix_memalign(&result, max(align, (umm)8), size);
		return result;
	}
	inline static void deallocate(void *data) { free(data); }
#endif
};

#ifndef TL_DEFAULT_ALLOCATOR
#define TL_DEFAULT_ALLOCATOR ::TL::OsAllocator
#endif

inline constexpr char toLower(char c) {
	if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
	return c;
}
inline constexpr wchar toLower(wchar c) {
	if (c >= 'A' && c <= 'Z') return (wchar)(c + ('a' - 'A'));
	return c;
}

inline constexpr umm length(char const *str) {
	umm result = 0;
	while (*str++) ++result;
	return result;	
}
inline constexpr umm length(wchar const *str) {
	umm result = 0;
	while (*str++) ++result;
	return result;	
}
inline constexpr umm length(char *str) {
	return length((char const *)str);
}
inline constexpr umm length(wchar *str) {
	return length((wchar const *)str);
}

inline constexpr bool equals(StringView a, StringView b) {
	if (a.size() != b.size()) return false;
	for (auto ap = a.begin(), bp = b.begin(); ap != a.end(); ++ap, ++bp)
		if (*ap != *bp)
			return false;
	return true;
}
inline constexpr bool equals(char const *a, StringView b) {
	for (auto bp = b.begin(); bp != b.end(); ++a, ++bp)
		if (*a != *bp)
			return false;
	return true;
}
inline constexpr bool equals(StringView a, char const *b) {
	for (auto ap = a.begin(); ap != a.end(); ++ap, ++b)
		if (*ap != *b)
			return false;
	return true;
}

inline constexpr bool startsWith(char const *str, char const *subStr) {
	while (*subStr)
		if (*str++ != *subStr++)
			return false;
	return true;
}
inline constexpr bool startsWith(char const *str, umm strLength, char const *subStr) {
	while (*subStr && strLength--)
		if (*str++ != *subStr++)
			return false;
	return !*subStr;
}
inline constexpr bool startsWith(char const *str, char const *subStr, umm substrLength) {
	while (substrLength--)
		if (*str++ != *subStr++)
			return false;
	return true;
}
inline constexpr bool startsWith(char const *str, umm strLength, char const *subStr, umm substrLength) {
	if (strLength < substrLength) return false;
	while (strLength-- && substrLength--)
		if (*str++ != *subStr++)
			return false;
	return true;
}

inline constexpr bool endsWith(char const *str, char const *subStr) {
	auto strLen = length(str);
	auto subStrLen = length(subStr);
	if (subStrLen > strLen)
		return false;
	str += strLen - subStrLen;
	while (*subStr)
		if (*str++ != *subStr++)
			return false;
	return true;
}

inline constexpr bool endsWithCI(char const *str, char const *subStr) {
	auto strLen = length(str);
	auto subStrLen = length(subStr);
	if (subStrLen > strLen)
		return false;
	str += strLen - subStrLen;
	while (*subStr)
		if (toLower(*str++) != toLower(*subStr++))
			return false;
	return true;
}

inline constexpr bool endsWith(wchar const *str, wchar const *subStr) {
	auto strLen = length(str);
	auto subStrLen = length(subStr);
	if (subStrLen > strLen)
		return false;
	str += strLen - subStrLen;
	while (*subStr)
		if (*str++ != *subStr++)
			return false;
	return true;
}

inline constexpr bool endsWithCI(wchar const *str, wchar const *subStr) {
	auto strLen = length(str);
	auto subStrLen = length(subStr);
	if (subStrLen > strLen)
		return false;
	str += strLen - subStrLen;
	while (*subStr)
		if (toLower(*str++) != toLower(*subStr++))
			return false;
	return true;
}

template <class CopyFn>
inline constexpr bool isCopyFn = std::is_invocable_v<CopyFn, char *, umm>;

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, char *v) { 
	return copyFn(v, length(v));
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, char const *v) { 
	return copyFn(v, length(v));
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, StringSpan v) {
	return copyFn(v.data(), v.size());
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, Span<char const> v) {
	return copyFn(v.data(), v.size());
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, bool v) {
	return copyFn(v ? "true" : "false", (umm)(v ? 4 : 5));
}
template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (isSigned<Int> ? 1 : 0);

template <class CopyFn, class Int, class = EnableIf<isInteger<Int> && isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, Int v, u32 radix = 10) {
	constexpr u32 maxDigits = _intToStringSize<Int>;
	char buf[maxDigits];
	char charMap[] = "0123456789ABCDEF";
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if constexpr (isSigned<Int>) {
		if (v < 0) {
			negative = true;
			if (v == min<Int>()) {
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
	return copyFn(lsc + 1, charsWritten);
}

template <class Int, class = EnableIf<isInteger<Int> && !isChar<Int>>>
StringSpan toString(Int v, char *outBuf) {
	return toString([&](char const *src, umm length) { 
		copyMemory(outBuf, src, length); 
		return StringSpan(outBuf, length);
	}, v);
}
template <class Int, class = EnableIf<isInteger<Int>>>
StringSpan toStringNT(Int v, char *outBuf) {
	return toString([&](char const *src, umm length) { 
		copyMemory(outBuf, src, length); 
		outBuf[length] = '\0';
		return StringSpan(outBuf, length);
	}, v);
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, void const *p) {
	return toString(std::forward<CopyFn>(copyFn), (umm)p, 16);
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, f64 v, u32 precision = 3) {
	char buf[64];
	char *c = buf;
	if (isNegative(v)) {
		*c++ = '-';
		v = -v;
	}
	c += toString((u64)v, c).size();
	*c++ = '.';
	for (u32 i = 0; i < precision; ++i) {
		v = v - (f64)(s64)v;
		v = select(v < 0, v + 1, v) * 10;
		*c++ = (char)((u32)v + '0');
	}
	return copyFn(buf, (umm)(c - buf));
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, f32 v, u32 precision = 3) {
	return toString(copyFn, (f64)v, precision);
}

template <class T, umm _capacity>
struct StaticList {
	StaticList() = default;
	template <umm thatCapacity>
	StaticList(StaticList<T, thatCapacity> const &that) {
		for (auto &src : that) {
			push_back(src);
		}
	}
	template <umm thatCapacity>
	StaticList(StaticList<T, thatCapacity> &&that) {
		for (auto &src : that) {
			push_back(std::move(src));
		}
		that.clear();
	}
	StaticList(StaticList const &that) {
		for (auto &src : that) {
			push_back(src);
		}
	}
	StaticList(StaticList &&that) {
		for (auto &src : that) {
			push_back(std::move(src));
		}
	}
	template <umm thatCapacity>
	StaticList &operator=(StaticList<T, thatCapacity> const &that) {
		clear();
		return *new (this) StaticList(that);
	}
	template <umm thatCapacity>
	StaticList &operator=(StaticList<T, thatCapacity> &&that) {
		clear();
		return *new (this) StaticList(std::move(that));
	}
	StaticList &operator=(StaticList const &that) {
		clear();
		return *new (this) StaticList(that);
	}
	StaticList &operator=(StaticList &&that) {
		clear();
		return *new (this) StaticList(std::move(that));
	}
	~StaticList() { clear(); }

	T *begin() { return &_begin->v; }
	T *end() { return &_end->v; }
	T const *begin() const { return &_begin->v; }
	T const *end() const { return &_end->v; }

	umm capacity() { return _capacity; }
	
	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return std::addressof(_begin->v); }
	T &front() { TL_BOUNDS_CHECK(size()); return _begin->v; }
	T &back() { TL_BOUNDS_CHECK(size()); return _end[-1].v; }
	T &operator[](umm i) { TL_BOUNDS_CHECK(size()); return _begin[i].v; }

	T const *data() const { return std::addressof(_begin->v); }
	T const &front() const { TL_BOUNDS_CHECK(size()); return _begin->v; }
	T const &back() const { TL_BOUNDS_CHECK(size()); return _end[-1].v; }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(size()); return _begin[i].v; }

	operator Span<T>() { return {begin(), end()}; }
	operator Span<T const>() const { return {begin(), end()}; }

	bool full() const { return size() == _capacity; }

	template <class... Args>
	bool try_emplace_back(Args &&... args) {
		if (full())
			return false;
		new (_end++) T(std::forward<Args>(args)...);
		return true;
	}
	bool try_push_back(T const &val) { return try_emplace_back(val); }
	bool try_push_back(T &&val) { return try_emplace_back(std::move(val)); }

	template <class... Args>
	void emplace_back(Args &&... args) {
		ASSERT(try_emplace_back(std::forward<Args>(args)...));
	}
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }

	void pop_back() {
		TL_BOUNDS_CHECK(size());
		_end-- [-1].v.~T();
	}
	void pop_front() {
		TL_BOUNDS_CHECK(size());
		--_end;
		for (auto dst = _begin; dst != _end; ++dst) {
			dst[0] = std::move(dst[1]);
		}
		_end->v.~T();
	}

	void clear() {
		for (auto &v : *this) {
			v.~T();
		}
		_end = _begin;
	}

	template <class Pred>
	T *find_if(Pred pred) {
		Storage *it = _begin;
		for (; it < _end; ++it) {
			if (pred(it->v)) {
				break;
			}
		}
		return &it->v;
	}

private:
	union Storage {
		T v;
		Storage() {}
		~Storage() {}
	};
	Storage _begin[_capacity];
	Storage *_end = _begin;
};

#if COMPILER_GCC
FORCEINLINE u32 findLowestOneBit(u32 val) { val ? __builtin_ffs(val) : ~0; }
FORCEINLINE u32 findLowestOneBit(u64 val) { val ? __builtin_ffsll(val) : ~0; }
FORCEINLINE u32 findHighestOneBit(u32 val) { val ? 32 - __builtin_clz(val) : ~0; }
FORCEINLINE u32 findHighestOneBit(u64 val) { val ? 64 - __builtin_clzll(val) : ~0; }
#else
FORCEINLINE u32 findLowestOneBit(u32 val) {
	unsigned long result;
	return _BitScanForward(&result, (unsigned long)val) ? (u32)result : ~0;
}
FORCEINLINE u32 findLowestOneBit(u64 val) {
	unsigned long result;
	return _BitScanForward64(&result, val) ? (u32)result : ~0;
}
FORCEINLINE u32 findHighestOneBit(u32 val) {
	unsigned long result;
	return _BitScanReverse(&result, (unsigned long)val) ? (u32)result : ~0;
}
FORCEINLINE u32 findHighestOneBit(u64 val) {
	unsigned long result;
	return _BitScanReverse64(&result, val) ? (u32)result : ~0;
}
#endif

FORCEINLINE u32 countBits(u32 v) { return (u32)_mm_popcnt_u32(v); }
FORCEINLINE u32 countBits(u64 v) { return (u32)_mm_popcnt_u64(v); }
FORCEINLINE u32 countBits(s32 v) { return countBits((u32)v); }
FORCEINLINE u32 countBits(s64 v) { return countBits((u64)v); }

#ifdef TL_IMPL
Allocator osAllocator = makeAllocator(0,
	[](void *, umm size, umm align) -> void * { return OsAllocator::allocate(size, align); },
	[](void *, void *data)                    { return OsAllocator::deallocate(data); }
);

#endif // TL_IMPL

} // namespace TL