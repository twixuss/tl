#pragma once
#define OS_WINDOWS 0
#define OS_LINUX   0

#if defined _WIN32 || defined _WIN64
#undef OS_WINDOWS
#define OS_WINDOWS 1
#else
#undef OS_LINUX
#define OS_LINUX 1
#endif

#define COMPILER_MSVC 0
#define COMPILER_GCC  0

// clang-format off
#if defined __GNUG__
	#undef  COMPILER_GCC
	#define COMPILER_GCC 1
#elif defined _MSC_VER
	#undef  COMPILER_MSVC
	#define COMPILER_MSVC 1
#else
	#pragma message "TL: Unresolved compiler"
#endif
// clang-format on

#if COMPILER_MSVC
#pragma warning(push, 0)
#include <intrin.h>
#include <vcruntime_new.h>
#endif

#include <utility>

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#if COMPILER_MSVC

#define FORCEINLINE			__forceinline
#define NOINLINE			__declspec(noinline)
#define DEBUG_BREAK			::__debugbreak()
#define WRITE_BARRIER		::_WriteBarrier()
#define READ_BARRIER		::_ReadBarrier()
#define READ_WRITE_BARRIER	::_ReadWriteBarrier()

#define ARCH_X86 0
#define ARCH_X64 0

#if defined _M_IX86
#undef ARCH_X86
#define ARCH_X86 1
#elif defined _M_X64
#undef ARCH_X64
#define ARCH_X64 1
#endif

#elif COMPILER_GCC

#define FORCEINLINE	__attribute__((always_inline))
#define DEBUG_BREAK ::__builtin_trap()
#if defined _X86_
#undef ARCH_X86
#define ARCH_X86 1
#elif defined __x86_64__
#undef ARCH_X64
#define ARCH_X64 1
#endif

#endif

// clang-format off
#if COMPILER_MSVC || COMPILER_GCC
	#ifndef ARCH_AVX
		#ifdef __AVX__
			#define ARCH_AVX 1
		#else
			#define ARCH_AVX 0
		#endif
	#endif
	#ifndef ARCH_AVX2 
		#ifdef __AVX2__
			#define ARCH_AVX2 1
		#else
			#define ARCH_AVX2 0
		#endif
	#endif
	#ifndef ARCH_AVX512F
		#ifdef __AVX512F__
			#define ARCH_AVX512F 1
		#else
			#define ARCH_AVX512F 0
		#endif
	#endif
	#ifndef ARCH_AVX512DQ
		#ifdef __AVX512DQ__
			#define ARCH_AVX512DQ 1
		#else
			#define ARCH_AVX512DQ 0
		#endif
	#endif
#endif
// clang-format on

#if !(ARCH_X86 | ARCH_X64)
#error "Unresolved target architecture"
#endif

#define STRINGIZE_(x) #x
#define STRINGIZE(x)  STRINGIZE_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y)  CONCAT_(x, y)

#ifndef ASSERTION_FAILURE
#define ASSERTION_FAILURE(causeString, expression, ...) DEBUG_BREAK
#endif

#define ASSERT(x, ...)                                    \
	do {                                                  \
		if (!(x)) {                                       \
			ASSERTION_FAILURE("ASSERT", #x, __VA_ARGS__); \
		}                                                 \
	} while (0)

#define INVALID_CODE_PATH(...)                                   \
	do {                                                         \
		ASSERTION_FAILURE("INVALID_CODE_PATH", "", __VA_ARGS__); \
	} while (0)

namespace TL {

using wchar = wchar_t;
using s8	= char;
using s16	= short;
using s32	= int;
using s64	= long long;
using u8	= unsigned char;
using u16	= unsigned short;
using u32	= unsigned int;
using u64	= unsigned long long;
using f32	= float;
using f64	= double;
using b32	= s32;
using b64	= s64;

#if ARCH_X64
typedef u64 umm;
typedef s64 smm;
#else
typedef u32 umm;
typedef s32 smm;
#endif

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
template <> inline constexpr bool isInteger<long> = true;
template <> inline constexpr bool isInteger<unsigned long> = true;

template <class T> inline constexpr bool isChar = false;
template <> inline constexpr bool isChar<u8> = true;
template <> inline constexpr bool isChar<s8> = true;

template <class T> inline constexpr bool isSigned = false;
template <> inline constexpr bool isSigned<s8 > = true;
template <> inline constexpr bool isSigned<s16> = true;
template <> inline constexpr bool isSigned<s32> = true;
template <> inline constexpr bool isSigned<s64> = true;
template <> inline constexpr bool isSigned<long> = true;

template <class T> inline constexpr bool isUnsigned = false;
template <> inline constexpr bool isUnsigned<u8 > = true;
template <> inline constexpr bool isUnsigned<u16> = true;
template <> inline constexpr bool isUnsigned<u32> = true;
template <> inline constexpr bool isUnsigned<u64> = true;
template <> inline constexpr bool isUnsigned<unsigned long> = true;

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
T *construct(T *val, Args &&...args) {
	return new(val) T(std::forward<Args>(args)...);
}

template <class T, class ...Args>
T &construct(T &val, Args &&...args) {
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

FORCEINLINE constexpr u8  negate(u8  v) { return (u8 )(~v + 1); }
FORCEINLINE constexpr u16 negate(u16 v) { return (u16)(~v + 1); }
FORCEINLINE constexpr u32 negate(u32 v) { return (u32)(~v + 1); }
FORCEINLINE constexpr u64 negate(u64 v) { return (u64)(~v + 1); }
FORCEINLINE constexpr s8  negate(s8  v) { return -v; }
FORCEINLINE constexpr s16 negate(s16 v) { return -v; }
FORCEINLINE constexpr s32 negate(s32 v) { return -v; }
FORCEINLINE constexpr s64 negate(s64 v) { return -v; }

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
constexpr umm countof(T const (&arr)[size]) { return size; }

void copyMemory(void *_dst, void const *_src, umm size) {
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
};
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

	constexpr operator Span<T const>() const { return {_begin, _end}; }

	T *_begin{};
	T *_end{};
};

using StringView = Span<char const>;
using StringSpan = Span<char>;

struct Allocator {
	void *(*allocate  )(void *state, umm size, umm align) = 0;
	void  (*deallocate)(void *state, void *data) = 0;
	void *state = 0;
};

Allocator makeAllocator(void *state, void *(*allocate)(void *state, umm size, umm align), void  (*deallocate)(void *state, void *data)) {
	Allocator result;
	result.allocate = allocate;
	result.deallocate = deallocate;
	result.state = state;
	return result;
}

extern Allocator osAllocator;

static void *  allocate(Allocator al, umm size, umm align = 0) { return al.allocate(al.state, size, align); }
static void  deallocate(Allocator al, void *data) { al.deallocate(al.state, data); }

template <class T> static T *allocate(Allocator al, umm count = 1, umm align = 0) { return (T *)allocate(al, count * sizeof(T), max(alignof(T), align)); }

template <class T, class Allocator>
static T *allocate(umm count = 1, umm align = 0) {
	return (T *)Allocator::allocate(count * sizeof(T), max(alignof(T), align));
}

struct OsAllocator {
#if OS_WINDOWS
	static void *allocate(umm size, umm align = 0) { return _aligned_malloc(size, max(align, (umm)8)); }
	static void deallocate(void *data) { _aligned_free(data); }
#else
	static void *allocate(umm size, umm align = 0) {
		void *result = 0;
		posix_memalign(&result, max(align, (umm)8), size);
		return result;
	}
	static void deallocate(void *data) { free(data); }
#endif
};

bool equals(StringView a, StringView b) {
	if (a.size() != b.size())
		return false;
	for (auto ap = a.begin(), bp = b.begin(); ap != a.end(); ++ap, ++bp) {
		if (*ap != *bp)
			return false;
	}
	return true;
}
bool equals(char const *a, StringView b) {
	for (auto bp = b.begin(); bp != b.end(); ++a, ++bp) {
		if (*a != *bp)
			return false;
	}
	return true;
}
bool equals(StringView a, char const *b) {
	for (auto ap = a.begin(); ap != a.end(); ++ap, ++b) {
		if (*ap != *b)
			return false;
	}
	return true;
}


inline constexpr bool startsWith(char const *str, char const *subStr) {
	while (*subStr) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}
inline constexpr bool startsWith(char const *str, umm lengthgth, char const *subStr) {
	while (*subStr && lengthgth--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return !*subStr;
}
inline constexpr bool startsWith(char const *str, char const *subStr, umm sublengthgth) {
	while (sublengthgth--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}
inline constexpr bool startsWith(char const *str, umm lengthgth, char const *subStr, umm sublengthgth) {
	if (lengthgth < sublengthgth)
		return false;
	while (lengthgth-- && sublengthgth--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}

namespace Fmt {

constexpr u32 precisionMask = 0x3F;
constexpr u32 precisionBit = 0;

constexpr u32 radixMask = 0xF;
constexpr u32 radixBit = 6;

constexpr u32 asCharMask = 0x1;
constexpr u32 asCharBit = 10;

struct Flags;

Flags precision(u32 value);
Flags radix(u32 value);

struct Flags {
	u32 value;
	Flags(u32 value) : value(value) {}
	Flags() : Flags(Fmt::precision(5) | Fmt::radix(10)) {}
	Flags operator|(Flags b) { return value | b.value; }

	u32 precision() { return (value >> precisionBit) & precisionMask; }
	u32 radix() { return ((value >> radixBit) & radixMask) + 1; }
	u32 asChar() { return (value >> asCharBit) & asCharMask; }
};

Flags precision(u32 value) { return (value & precisionMask) << precisionBit; }
Flags radix(u32 value) { return ((value - 1) & radixMask) << radixBit; }
Flags asChar(bool value) { return (u32)value << asCharBit; }

}

template <class CopyFn>
inline constexpr bool isCopyFn = std::is_invocable_v<CopyFn, char *, umm>;

umm length(char const *str) {
	umm result = 0;
	while (*str++) {
		++result;
	}
	return result;	
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, char *v, Fmt::Flags flags = {}) { 
	return copyFn(v, length(v));
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, char const *v, Fmt::Flags flags = {}) { 
	return copyFn(v, length(v));
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, StringSpan v, Fmt::Flags flags = {}) {
	return copyFn(v.data(), v.size());
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>> 
StringSpan toString(CopyFn &&copyFn, Span<char const> v, Fmt::Flags flags = {}) {
	return copyFn(v.data(), v.size());
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, bool v, Fmt::Flags flags = {}) {
	return copyFn(v ? "true" : "false", (umm)(v ? 4 : 5));
}
template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (isSigned<Int> ? 1 : 0);

template <class CopyFn, class Int, class = EnableIf<isInteger<Int> && isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, Int v, Fmt::Flags flags = {}) {
	if (flags.asChar()) {
		char c = (char)v;
		return copyFn(&c, 1);
	}
	constexpr u32 maxDigits = _intToStringSize<Int>;
	char buf[maxDigits];
	char charMap[] = "0123456789ABCDEF";
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	u32 radix = flags.radix();

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
StringSpan toString(Int v, char *outBuf, Fmt::Flags flags = {}) {
	return toString([&](char const *src, umm length) { 
		copyMemory(outBuf, src, length); 
		return StringSpan(outBuf, length);
	}, v, flags);
}
template <class Int, class = EnableIf<isInteger<Int>>>
StringSpan toStringNT(Int v, char *outBuf, Fmt::Flags flags = {}) {
	return toString([&](char const *src, umm length) { 
		copyMemory(outBuf, src, length); 
		outBuf[length] = '\0';
		return StringSpan(outBuf, length);
	}, v, flags);
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, void const *p, Fmt::Flags flags = Fmt::radix(16)) {
	return toString(std::forward<CopyFn>(copyFn), (umm)p, flags);
}

template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, f64 v, Fmt::Flags flags = {}) {
	char buf[64];
	return copyFn(buf, (umm)sprintf(buf, "%.*f", flags.precision(), v));
}
template <class CopyFn, class = EnableIf<isCopyFn<CopyFn>>>
StringSpan toString(CopyFn &&copyFn, f32 v, Fmt::Flags flags = {}) {
	return toString(copyFn, (f64)v, flags);
}

#ifdef TL_IMPL
Allocator osAllocator = makeAllocator(0,
	[](void *, umm size, umm align)             -> void * { return OsAllocator::allocate(size, align); },
	[](void *, void *data)                                { return OsAllocator::deallocate(data); }
);
#endif

} // namespace TL