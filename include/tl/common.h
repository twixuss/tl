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
#define ASSERTION_FAILURE(causeString, expression, ...) DEBUG_BREAK
#endif

#define ASSERT(x, ...)                                                                      \
	do {                                                                                    \
		if (!(x)) {                                                                         \
			ASSERTION_FAILURE("ASSERT", #x, __VA_ARGS__); \
		}                                                                                   \
	} while (0)

#define INVALID_CODE_PATH(...)                                                                     \
	do {                                                                                           \
		ASSERTION_FAILURE("INVALID_CODE_PATH", "", __VA_ARGS__); \
	} while (0)

#ifdef TL_NO_BOUNDS_CHECK
#define TL_BOUNDS_CHECK(x)
#else
#define TL_BOUNDS_CHECK(x) ASSERT(x, "Bounds check failed")
#endif

#ifdef TL_ALLOCATION_TRACKER
#ifndef TL_DEALLOCATION_TRACKER
#error TL_DEALLOCATION_TRACKER must be defined with TL_ALLOCATION_TRACKER
#endif
#endif

namespace TL {

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

template<> inline constexpr ulong min() { return (ulong)min<ulong_s>(); }
template<> inline constexpr ulong max() { return (ulong)max<ulong_s>(); }
template<> inline constexpr slong min() { return (slong)min<slong_s>(); }
template<> inline constexpr slong max() { return (slong)max<slong_s>(); }
#pragma warning(pop)

template <class T>
FORCEINLINE constexpr bool isPowerOf2(T v) { return (v != 0) && ((v & (v - 1)) == 0); }

template <class T, class U> FORCEINLINE constexpr auto min(T a, U b) { return a < b ? a : b; }
template <class T, class U> FORCEINLINE constexpr auto max(T a, U b) { return a > b ? a : b; }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto min(T a, U b, Rest... rest) { return min(min(a, b), rest...); }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto max(T a, U b, Rest... rest) { return max(max(a, b), rest...); }
template <class T, class U, class V, class W> FORCEINLINE constexpr void minmax(T a, U b, V& mn, W& mx) { mn = min(a, b); mx = max(a, b); }

template <class T>
FORCEINLINE constexpr T floor(T v, T s) {
	if constexpr (isSigned<T>) {
		return ((v < 0) ? ((v + 1) / s - 1) : (v / s)) * s;
	} else {
		return v / s * s;
	}
}

template <class T>
FORCEINLINE constexpr T ceil(T v, T s) {
	return floor(v + s - 1, s);
}

FORCEINLINE constexpr void *floor(void *v, umm s) { return (void *)floor((umm)v, s); }
FORCEINLINE constexpr void *ceil(void *v, umm s) { return floor((u8 *)v + s - 1, s); }

template <class T>
FORCEINLINE constexpr T frac(T v, T s) {
	if constexpr(isSigned<T>) {
		return (v < 0) ? ((v + 1) % s + s - 1) : (v % s);
	} else {
		return v % s;
	}
}

FORCEINLINE constexpr bool isNegative(f32 v) { return *(u32 *)&v & 0x80000000; }
FORCEINLINE constexpr bool isNegative(f64 v) { return *(u64 *)&v & 0x8000000000000000; }

template <class T> FORCEINLINE constexpr T select(bool mask, T a, T b) { return mask ? a : b; }

#if COMPILER_GCC
FORCEINLINE u8  rotateLeft (u8  v, s32 shift = 1) { return (v << shift) | (v >> ( 8 - shift)); }
FORCEINLINE u16 rotateLeft (u16 v, s32 shift = 1) { return (v << shift) | (v >> (16 - shift)); }
FORCEINLINE u32 rotateLeft (u32 v, s32 shift = 1) { return (v << shift) | (v >> (32 - shift)); }
FORCEINLINE u64 rotateLeft (u64 v, s32 shift = 1) { return (v << shift) | (v >> (64 - shift)); }
FORCEINLINE u8  rotateRight(u8  v, s32 shift = 1) { return (v >> shift) | (v << ( 8 - shift)); }
FORCEINLINE u16 rotateRight(u16 v, s32 shift = 1) { return (v >> shift) | (v << (16 - shift)); }
FORCEINLINE u32 rotateRight(u32 v, s32 shift = 1) { return (v >> shift) | (v << (32 - shift)); }
FORCEINLINE u64 rotateRight(u64 v, s32 shift = 1) { return (v >> shift) | (v << (64 - shift)); }
#else
FORCEINLINE u8  rotateLeft (u8  v, s32 shift = 1) { return _rotl8(v, (u8)shift); }
FORCEINLINE u16 rotateLeft (u16 v, s32 shift = 1) { return _rotl16(v, (u8)shift); }
FORCEINLINE u32 rotateLeft (u32 v, s32 shift = 1) { return _rotl(v, shift); }
FORCEINLINE u64 rotateLeft (u64 v, s32 shift = 1) { return _rotl64(v, shift); }
FORCEINLINE u8  rotateRight(u8  v, s32 shift = 1) { return _rotr8(v, (u8)shift); }
FORCEINLINE u16 rotateRight(u16 v, s32 shift = 1) { return _rotr16(v, (u8)shift); }
FORCEINLINE u32 rotateRight(u32 v, s32 shift = 1) { return _rotr(v, shift); }
FORCEINLINE u64 rotateRight(u64 v, s32 shift = 1) { return _rotr64(v, shift); }
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
	constexpr operator Span<void>() const { return {_begin, _end}; }
	constexpr operator Span<void const>() const { return {_begin, _end}; }

	constexpr bool operator==(Span<T> that) const { return _begin == that._begin && _end == that._end; }

	T *_begin{};
	T *_end{};
};

template <>
struct Span<void const> {
	constexpr Span() = default;
	constexpr Span(void const *begin, void const *end) : _begin(begin), _end(end) {}
	constexpr Span(void const *begin, umm size) : Span(begin, (u8 *)begin + size) {}
	constexpr void const *data() const { return _begin; }
	constexpr void const *begin() const { return _begin; }
	constexpr void const *end() const { return _end; }
	constexpr umm size() const { return umm((u8 *)_end - (u8 *)_begin); }
	constexpr bool empty() const { return _begin == _end; }
	constexpr explicit operator Span<char const>() const { return {(char const *)_begin, (char const *)_end}; }
	constexpr bool operator==(Span<void const> that) const { return _begin == that._begin && _end == that._end; }
	void const *_begin{};
	void const *_end{};
};

template <>
struct Span<void> {
	constexpr Span() = default;
	constexpr Span(void *begin, void *end) : _begin(begin), _end(end) {}
	constexpr Span(void *begin, umm size) : Span(begin, (u8 *)begin + size) {}
	constexpr void *data() const { return _begin; }
	constexpr void *begin() const { return _begin; }
	constexpr void *end() const { return _end; }
	constexpr umm size() const { return umm((u8 *)_end - (u8 *)_begin); }
	constexpr bool empty() const { return _begin == _end; }
	constexpr explicit operator Span<char>() const { return {(char *)_begin, (char *)_end}; }
	constexpr explicit operator Span<char const>() const { return {(char const *)_begin, (char const *)_end}; }
	constexpr operator Span<void const>() const { return {_begin, _end}; }
	constexpr bool operator==(Span<void> that) const { return _begin == that._begin && _end == that._end; }
	void *_begin{};
	void *_end{};
};

template <class T>
constexpr umm countof(Span<T const> span) { return span.size(); }

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

#ifdef TL_ALLOCATION_TRACKER

#define ALLOCATE(al, size, align)                    (TL_ALLOCATION_TRACKER<al>((umm)(size), (umm)(align), __FILE__, __LINE__))
#define ALLOCATE_MSG(al, size, align, message)       (TL_ALLOCATION_TRACKER<al>((umm)(size), (umm)(align), __FILE__, __LINE__, message))
#define ALLOCATE_T(al, t, count, align)              ((t*)TL_ALLOCATION_TRACKER<al>(sizeof(t) * (umm)(count), max((umm)(align), (umm)alignof(t)), __FILE__, __LINE__))
#define ALLOCATE_T_MSG(al, t, count, align, message) ((t*)TL_ALLOCATION_TRACKER<al>(sizeof(t) * (umm)(count), max((umm)(align), (umm)alignof(t)), __FILE__, __LINE__, message))

#define DEALLOCATE(al, data) (TL_DEALLOCATION_TRACKER(data), al::deallocate(data))

#else

#define ALLOCATE(al, size, align)                    al::allocate((umm)(size), (umm)(align))
#define ALLOCATE_MSG(al, size, align, message)       al::allocate((umm)(size), (umm)(align))
#define ALLOCATE_T(al, t, count, align)              (t*)al::allocate(sizeof(t) * (umm)(count), max((umm)(align), (umm)alignof(t)))
#define ALLOCATE_T_MSG(al, t, count, align, message) (t*)al::allocate(sizeof(t) * (umm)(count), max((umm)(align), (umm)alignof(t)))

#define DEALLOCATE(al, data) al::deallocate(data)

#endif


struct OsAllocator {
	TL_API static void *allocate(umm size, umm align = 0);
	TL_API static void deallocate(void *data);
};

#ifndef TL_DEFAULT_ALLOCATOR
#define TL_DEFAULT_ALLOCATOR ::TL::OsAllocator
#endif

inline constexpr char toLower(char c) {
	if (c >= 'A' && c <= 'Z')
		return (char)(c + ('a' - 'A'));
	return c;
}
inline constexpr wchar toLower(wchar c) {
	if (c >= L'A' && c <= L'Z')
		return (wchar)(c + (L'a' - L'A'));
	return c;
}

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

#define COMP_FUNCS(typeA, typeB)                                                                           \
inline constexpr bool equals(typeA const *a, typeB const *b) {                                             \
	for (;;)                                                                                               \
		if (*a++ != *b++)                                                                                  \
			return false;                                                                                  \
	return true;                                                                                           \
}                                                                                                          \
inline constexpr bool equals(Span<typeA const> a, typeB const *b) {                                        \
	for (auto ap = a.begin(); ap != a.end(); ++ap, ++b)                                                    \
		if (*ap != *b)                                                                                     \
			return false;                                                                                  \
	return true;                                                                                           \
}                                                                                                          \
inline constexpr bool equals(typeA const *a, Span<typeB const> b) {                                        \
	for (auto bp = b.begin(); bp != b.end(); ++a, ++bp)                                                    \
		if (*a != *bp)                                                                                     \
			return false;                                                                                  \
	return true;                                                                                           \
}                                                                                                          \
inline constexpr bool equals(Span<typeA const> a, Span<typeB const> b) {                                   \
	if (a.size() != b.size())                                                                              \
		return false;                                                                                      \
	auto ap = a.begin();                                                                                   \
	for (auto bp = b.begin(); ap != a.end(); ++ap, ++bp)                                                   \
		if (*ap != *bp)                                                                                    \
			return false;                                                                                  \
	return true;                                                                                           \
}                                                                                                          \
inline constexpr bool startsWith(typeA const *str, typeB const *subStr) {                                  \
	while (*subStr)                                                                                        \
		if (*str++ != *subStr++)                                                                           \
			return false;                                                                                  \
	return true;                                                                                           \
}                                                                                                          \
inline constexpr bool startsWith(typeA const *str, typeB const *subStr, umm substrLength) {                \
	while (substrLength--)                                                                                 \
		if (*str++ != *subStr++)                                                                           \
			return false;                                                                                  \
	return true;                                                                                           \
}                                                                                                          \
inline constexpr bool startsWith(typeA const *str, umm strLength, typeB const *subStr) {                   \
	while (*subStr && strLength--)                                                                         \
		if (*str++ != *subStr++)                                                                           \
			return false;                                                                                  \
	return !*subStr;                                                                                       \
}                                                                                                          \
inline constexpr bool startsWith(typeA const *str, umm strLength, typeB const *subStr, umm substrLength) { \
	if (strLength < substrLength)                                                                          \
		return false;                                                                                      \
	while (strLength-- && substrLength--)                                                                  \
		if (*str++ != *subStr++)                                                                           \
			return false;                                                                                  \
	return true;                                                                                           \
}

COMP_FUNCS(char, char)
COMP_FUNCS(char, wchar)
COMP_FUNCS(wchar, char)
COMP_FUNCS(wchar, wchar)

#undef COMP_FUNCS

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

inline constexpr void setToLowerCase(Span<wchar> span) {
	for (auto &c : span) {
		c = toLower(c);
	}
}

template <class CopyFn, class Char>
inline constexpr bool isCopyFn = std::is_invocable_v<CopyFn, Char *, umm>;

template <class CopyFn, class Char>
using CopyFnRet = decltype(((CopyFn *)0)->operator()((Char*)0, (umm)0));

template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (isSigned<Int> ? 1 : 0);

template <class Int>
struct FormatInt {
	Int value;
	u32 radix;
	explicit FormatInt(Int value, u32 radix = 10) : value(value), radix(radix) {}
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

template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> toString(char  const &v, CopyFn &&copyFn) { Char c = (Char)v; return copyFn(&c, 1); }
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> toString(wchar const &v, CopyFn &&copyFn) { Char c = (Char)v; return copyFn(&c, 1); }
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> toString(Char const *v, CopyFn &&copyFn) { if (!v) v = NullString<Char>::value; return copyFn(v, length(v)); }
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> toString(Char       *v, CopyFn &&copyFn) { if (!v) v = (Char *)NullString<Char>::value; return copyFn((Char const *)v, length(v)); }
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> toString(Span<Char const> v, CopyFn &&copyFn) { return copyFn(v.data(), v.size()); }
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>> CopyFnRet<CopyFn, Char> toString(Span<Char      > v, CopyFn &&copyFn) { return copyFn(v.data(), v.size()); }

template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> toString(bool v, CopyFn &&copyFn) {
	if constexpr (isSame<Char, char>)
		return copyFn(v ? "true" : "false", (umm)(v ? 4 : 5));
	else
		return copyFn(v ? L"true" : L"false", (umm)(v ? 4 : 5));
}

template <class Char, class CopyFn, class Int>
CopyFnRet<CopyFn, Char> toString(FormatInt<Int> f, CopyFn &&copyFn) {
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
template <class Char, class CopyFn, class Int, class = EnableIf<isInteger<Int> && isCopyFn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> toString(Int v, CopyFn &&copyFn) {
	return toString<Char>(FormatInt(v), std::forward<CopyFn>(copyFn));
}

template <class Char, class T>
Span<Char> toStringNT(T v, Char *outBuf) {
	return toString<Char>(v, [&](Char const *src, umm length) { 
		copyMemory(outBuf, src, length * sizeof(Char)); 
		outBuf[length] = (Char)0;
		return Span<Char>(outBuf, length);
	});
}

template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> toString(void const *p, CopyFn &&copyFn) {
	return toString<Char>(FormatInt((umm)p, 16), std::forward<CopyFn>(copyFn));
}

template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> toString(FormatFloat<f64> f, CopyFn &&copyFn) {
	auto v = f.value;
	auto precision = f.precision;
	Char buf[64];
	Char *c = buf;
	if (isNegative(v)) {
		*c++ = '-';
		v = -v;
	}
	c += toString<Char>((u64)v, [&](Char const *string, umm length){
		copyMemory(buf, string, length * sizeof(Char));
		return length;
	});
	*c++ = '.';
	for (u32 i = 0; i < precision; ++i) {
		v = v - (f64)(s64)v;
		v = (v < 0 ? v + 1 : v) * 10;
		*c++ = (Char)((u32)v + '0');
	}
	return copyFn(buf, (umm)(c - buf));
}
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> toString(f64 v, CopyFn &&copyFn) {
	return toString<Char>(FormatFloat(v), std::forward<CopyFn>(copyFn));
}
template <class Char, class CopyFn, class = EnableIf<isCopyFn<CopyFn, Char>>>
CopyFnRet<CopyFn, Char> toString(f32 v, CopyFn &&copyFn) {
	return toString<Char>((f64)v, std::forward<CopyFn>(copyFn));
}

template <class Allocator>
struct Buffer : Span<void> {
	using Span<void>::Span;
	operator bool() const { return data() != 0; }
};

template <class Allocator>
inline void free(Buffer<Allocator> buffer) {
	if (buffer)
		DEALLOCATE(Allocator, buffer.data());
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
	bool empty() const { return _end == _begin; }
	bool full() const { return _end == _begin + _capacity; }

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

	template <class... Args>
	void emplace_back(Args &&... args) {
		TL_BOUNDS_CHECK(!full());
		new (_end++) T(std::forward<Args>(args)...);
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
		for (auto it = _begin; it != _end; ++it) {
			it->v.~T();
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
FORCEINLINE u32 countLeadingZeros(u16 val) { return __lzcnt16(val); }
FORCEINLINE u32 countLeadingZeros(u32 val) { return __lzcnt(val); }
FORCEINLINE u32 countLeadingZeros(u64 val) { return (u32)__lzcnt64(val); }
#endif

FORCEINLINE u32 countBits(u32 v) { return (u32)_mm_popcnt_u32(v); }
FORCEINLINE u32 countBits(u64 v) { return (u32)_mm_popcnt_u64(v); }
FORCEINLINE u32 countBits(s32 v) { return countBits((u32)v); }
FORCEINLINE u32 countBits(s64 v) { return countBits((u64)v); }

#ifdef TL_IMPL

#if OS_WINDOWS
#if COMPILER_MSVC
	void *OsAllocator::allocate(umm size, umm align) { return _aligned_malloc(size, max(align, (umm)8)); }
	void OsAllocator::deallocate(void *data) { _aligned_free(data); }
#elif COMPILER_GCC
	void *OsAllocator::allocate(umm size, umm align) { return __mingw_aligned_malloc(size, max(align, (umm)8)); }
	void OsAllocator::deallocate(void *data) { __mingw_aligned_free(data); }
#endif
#else
	void *OsAllocator::allocate(umm size, umm align) {
		void *result = 0;
		posix_memalign(&result, max(align, sizeof(void *)), size);
		return result;
	}
	void OsAllocator::deallocate(void *data) { free(data); }
#endif

#endif

} // namespace TL