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
#endif

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

#define FORCEINLINE	  __attribute__((always_inline))
#define DEBUG_BREAK() ::__builtin_trap()
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

template <class T> inline constexpr bool isLValueReference = false;
template <class T> inline constexpr bool isLValueReference<T&> = true;

template <class T> struct RemoveReferenceT       { using Type = T; };
template <class T> struct RemoveReferenceT<T &>  { using Type = T; };
template <class T> struct RemoveReferenceT<T &&> { using Type = T; };
template <class T> using RemoveReference = typename RemoveReferenceT<T>::Type;

template <bool v, class T, class F> struct ConditionalT { using Type = T; };
template <class T, class F> struct ConditionalT<false, T, F> { using Type = F; };
template <bool v, class T, class F> using Conditional = typename ConditionalT<v, T, F>::Type;

template <bool v, class T = void> struct EnableIfT {};
template <class T> struct EnableIfT<true, T> { using Type = T; };
template <bool v, class T = void> using EnableIf = typename EnableIfT<v, T>::Type;

template <class T> 
[[nodiscard]] inline constexpr RemoveReference<T>&& move(T&& val) noexcept {
    return static_cast<RemoveReference<T>&&>(val);
}

template <class T>
[[nodiscard]] inline constexpr T&& forward(RemoveReference<T>& val) noexcept {
    return static_cast<T&&>(val);
}

template <class T>
[[nodiscard]] inline constexpr T&& forward(RemoveReference<T>&& val) noexcept {
    static_assert(!isLValueReference<T>, "bad forward call");
    return static_cast<T&&>(val);
}

template <class T, class ...Args>
T *construct(T *val, Args &&...args) {
	return new(val) T(forward<Args>(args)...);
}

template <class T, class ...Args>
T &construct(T &val, Args &&...args) {
	return *new(std::addressof(val)) T(forward<Args>(args)...);
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
// clang-format on
#pragma warning(pop)

inline constexpr bool isPowerOf2(u8  v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(u16 v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(u32 v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(u64 v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(s8  v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(s16 v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(s32 v) { return v && ((v & (v - 1)) == 0); }
inline constexpr bool isPowerOf2(s64 v) { return v && ((v & (v - 1)) == 0); }

template <class T, class U> FORCEINLINE constexpr auto min(T a, U b) { return a < b ? a : b; }
template <class T, class U> FORCEINLINE constexpr auto max(T a, U b) { return a > b ? a : b; }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto min(T a, U b, Rest... rest) { return min(min(a, b), rest...); }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto max(T a, U b, Rest... rest) { return max(max(a, b), rest...); }
template <class T, class U, class V, class W> FORCEINLINE constexpr void minmax(T a, U b, V& mn, W& mx) { mn = min(a, b); mx = max(a, b); }

inline constexpr void *floor(void *v, u32 s) { return (void *)((umm)v / s * s); }
inline void *ceil(void *v, u32 s) { return floor((u8 *)v + ((s32)s - 1), s); }

inline constexpr u8  negate(u8  v) { return (u8 )((u8 )~v + 1); }
inline constexpr u16 negate(u16 v) { return (u16)((u16)~v + 1); }
inline constexpr u32 negate(u32 v) { return (u32)((u32)~v + 1); }
inline constexpr u64 negate(u64 v) { return (u64)((u64)~v + 1); }
inline constexpr s8  negate(s8  v) { return (s8 )((s8 )~v + 1); }
inline constexpr s16 negate(s16 v) { return (s16)((s16)~v + 1); }
inline constexpr s32 negate(s32 v) { return (s32)((s32)~v + 1); }
inline constexpr s64 negate(s64 v) { return (s64)((s64)~v + 1); }

template <class... Callables>
struct Combine : public Callables... {
	inline constexpr Combine(Callables &&... c) : Callables(std::move(c))... {}
	using Callables::operator()...;
};

template <class Enum, class = EnableIf<std::is_enum_v<Enum>>>
inline constexpr auto toInt(Enum e) {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class T>
inline void populate(void *_dst, T const &src, umm count) {
	T *dst = (T *)_dst;
	while (count--) {
		memcpy(dst++, std::addressof(src), sizeof(T));
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
	constexpr umm size() const { return umm(_end - _begin); }
	constexpr T &operator[](umm i) { return _begin[i]; }
	constexpr T const &operator[](umm i) const { return _begin[i]; }

	constexpr operator Span<T const>() const { return {_begin, _end}; }

	T *_begin{};
	T *_end{};
};

using StringView = Span<char const>;
using StringSpan = Span<char>;

template <class T, umm _size>
struct Array {
	constexpr umm size() { return _size; }

	constexpr T *data() { return v; }
	constexpr T& operator[](umm i) { return v[i]; }

	constexpr T const *data() const { return v; }
	constexpr T const& operator[](umm i) const { return v[i]; }
	T v[_size];
};

template <class T, class Allocator>
static T *allocate(umm count = 1) {
	return (T *)Allocator::allocate(count * sizeof(T), alignof(T));
}
struct OsAllocator {
	static void *allocate(umm size, umm align = 0) { return _aligned_malloc(size, max(align, (umm)8)); }
	static void deallocate(void *data) { _aligned_free(data); }
};

inline constexpr bool startsWith(char const *str, char const *subStr) {
	while (*subStr) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}
inline constexpr bool startsWith(char const *str, umm strLength, char const *subStr) {
	while (*subStr && strLength--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return !*subStr;
}
inline constexpr bool startsWith(char const *str, char const *subStr, umm subStrLength) {
	while (subStrLength--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}
inline constexpr bool startsWith(char const *str, umm strLength, char const *subStr, umm subStrLength) {
	if (strLength < subStrLength)
		return false;
	while (strLength-- && subStrLength--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}

template <class CopyFn> 
void toString(char *v, CopyFn &&copyFn) { 
	copyFn(v, strlen(v));
}
template <class CopyFn> 
void toString(char const *v, CopyFn &&copyFn) { 
	copyFn(v, strlen(v));
}

template <class CopyFn> 
void toString(Span<char> v, CopyFn &&copyFn) {
	copyFn(v.data(), v.size());
}
template <class CopyFn> 
void toString(Span<char const> v, CopyFn &&copyFn) {
	copyFn(v.data(), v.size());
}
template <class CopyFn>
void toString(bool v, CopyFn &&copyFn) {
	copyFn(v ? "true" : "false", (umm)(v ? 4 : 5));
}
template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (isSigned<Int> ? 1 : 0);

template <class Int, class CopyFn, class = EnableIf<isInteger<Int>>>
void toString(Int v, u32 radix, CopyFn &&copyFn) {
	constexpr u32 maxDigits = _intToStringSize<Int>;
	char buf[maxDigits];
	char charMap[] = "0123456789ABCDEF";
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if constexpr (std::is_signed_v<Int>) {
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
	if constexpr (std::is_signed_v<Int>) {
		if (negative) {
			++charsWritten;
			*lsc-- = '-';
		}
	} else {
		(void)negative;
	}
	copyFn(lsc + 1, charsWritten);
}
template <class Int, class CopyFn, class = EnableIf<isInteger<Int>>>
void toString(Int v, CopyFn &&copyFn) {
	toString(v, 10, copyFn);
}
template <class Int, class = EnableIf<isInteger<Int>>>
Span<char> toString(Int v, char *outBuf, u32 radix = 10) {
	Span<char> result;
	toString(v, radix, [&](char const *src, umm length) { 
		result = {outBuf, length};
		memcpy(outBuf, src, length); 
	});
	return result;
}
template <class Int, class = EnableIf<isInteger<Int>>>
Span<char> toStringNT(Int v, char *outBuf, u32 radix = 10) {
	Span<char> result;
	toString(v, radix, [&](char const *src, umm length) { 
		result = {outBuf, length};
		memcpy(outBuf, src, length); 
		outBuf[length] = '\0';
	});
	return result;
}
template <class CopyFn>
void toString(f64 v, CopyFn &&copyFn) {
	char buf[64];
	copyFn(buf, (umm)sprintf(buf, "%.2f", v));
}
template <class CopyFn>
void toString(f32 v, CopyFn &&copyFn) {
	toString((f64)v, copyFn);
}

#pragma warning(push)
#pragma warning(disable : 4582)

template <class T, class Allocator>
struct AllocListBase {
	AllocListBase() = default;
	explicit AllocListBase(umm length) {
		resize(length);
	}
	AllocListBase(Span<T const> span) {
		reserve(span.size());
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size();
	}
	AllocListBase(T const *begin, umm length) : AllocListBase(Span(begin, length)) {}
	AllocListBase(AllocListBase &&that) {
		_begin		   = that._begin;
		_end		   = that._end;
		_allocEnd	   = that._allocEnd;
		that._begin	   = 0;
		that._end	   = 0;
		that._allocEnd = 0;
	}
	AllocListBase(AllocListBase const &that) {
		_begin	  = allocate<T, Allocator>(that.size());
		_allocEnd = _end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
	}
	~AllocListBase() {
		clear();
		if (_begin)
			Allocator::deallocate(_begin);
		_begin	  = 0;
		_end	  = 0;
		_allocEnd = 0;
	}
	AllocListBase &set(Span<T const> span) {
		clear();
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size();
		return *this;
	}
	AllocListBase &operator=(AllocListBase const &that) {
		clear();
		reserve(that.size());
		_end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
		return *this;
	}
	AllocListBase &operator=(AllocListBase &&that) {
		clear();
		if (_begin)
			Allocator::deallocate(_begin);
		_begin		   = that._begin;
		_end		   = that._end;
		_allocEnd	   = that._allocEnd;
		that._begin	   = 0;
		that._end	   = 0;
		that._allocEnd = 0;
		return *this;
	}

	void _copyConstruct(T const *srcBegin, T const *srcEnd) {
		umm srcSize = (umm)(srcEnd - srcBegin);
		for (umm i = 0; i < srcSize; ++i) {
			new (_begin + i) T(srcBegin[i]);
		}
	}

	T *begin() { return _begin; }
	T *end() { return _end; }
	T const *begin() const { return _begin; }
	T const *end() const { return _end; }

	umm remainingCapacity() const { return (umm)(_allocEnd - _end); }
	umm capacity() const { return (umm)(_allocEnd - _begin); }
	
	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return _begin; }
	T &front() { return *_begin; }
	T &back() { return _end[-1]; }
	T &operator[](umm i) { return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { return *_begin; }
	T const &back() const { return _end[-1]; }
	T const &operator[](umm i) const { return _begin[i]; }

	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }
	void pop_back() { (--_end)->~T(); }
	void reserve(umm count) {
		if (count > capacity())
			_reallocate(count);
	}
	void resize(umm newSize) {
		if (newSize > capacity())
			_reallocate(newSize);
		if (newSize > size()) {
			for (T *t = _begin + size(); t < _begin + newSize; ++t)
				new (t) T;
			_end = _begin + newSize;
		} else if (newSize < size()) {
			for (T *t = _begin + newSize; t < _end; ++t)
				t->~T();
			_end = _begin + newSize;
		}
	}
	void clear() {
		for (auto &val : *this) {
			val.~T();
		}
		_end = _begin;
	}

	operator Span<T>() { return {begin(), end()}; }
	operator Span<T const>() const { return {begin(), end()}; }

	T *_begin	 = 0;
	T *_end		 = 0;
	T *_allocEnd = 0;

	void _reallocate(umm newCapacity) {
		ASSERT(capacity() < newCapacity);
		umm oldSize = size();
		T *newBegin = allocate<T, Allocator>(newCapacity);
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			Allocator::deallocate(_begin);
		_begin	  = newBegin;
		_end	  = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	template <class... Args>
	void emplace_back(Args &&... args) {
		if (remainingCapacity() == 0) {
			umm newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			_reallocate(newCapacity);
		}
		new (_end++) T(std::forward<Args>(args)...);
	}
};

template <class T, umm _capacity>
struct StaticList {
	StaticList() = default;
	template <umm thatCapacity>
	StaticList(StaticList<T, thatCapacity> const &that) {
		ASSERT(that.size() <= _capacity);
		for (auto &src : that) {
			push_back(src);
		}
	}
	template <umm thatCapacity>
	StaticList(StaticList<T, thatCapacity> &&that) {
		ASSERT(that.size() <= _capacity);
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
	~StaticList() { clear(); }

	T *begin() { return &_begin->v; }
	T *end() { return &_end->v; }
	T const *begin() const { return &_begin->v; }
	T const *end() const { return &_end->v; }

	umm capacity() { return _capacity; }
	
	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return std::addressof(_begin->v); }
	T &front() { return _begin->v; }
	T &back() { return _end[-1].v; }
	T &operator[](umm i) { return _begin[i].v; }

	T const *data() const { return std::addressof(_begin->v); }
	T const &front() const { return _begin->v; }
	T const &back() const { return _end[-1].v; }
	T const &operator[](umm i) const { return _begin[i].v; }

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

	template <class... Args>
	void emplace_back_pop_front(Args &&... args) {
		if (full())
			pop_front();
		emplace_back(std::forward<Args>(args)...);
	}
	void push_back_pop_front(T const &val) { emplace_back_pop_front(val); }
	void push_back_pop_front(T &&val) { emplace_back_pop_front(std::move(val)); }

	void pop_back() {
		ASSERT(size());
		_end-- [-1].v.~T();
	}
	void pop_front() {
		ASSERT(size());
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

template <class T, class Allocator = OsAllocator>
struct List : AllocListBase<T, Allocator> {
	using Base = AllocListBase<T, Allocator>;
	List() = default;
	List(T const *first, umm count) : Base(first, count) {}
	explicit List(umm size) : Base(size) {}
	List(Span<T const> span) : Base(span) {}
	List(List const &that) = default;
	List(List &&that) = default;
	List &operator=(List const &that) = default;
	List &operator=(List &&that) = default;
	List &set(Span<char const> span) { return Base::set(span), *this; }
	template <class... Args>
	void push_front(Args &&... args) {
		if (remainingCapacity() == 0) {
			umm newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			reallocate(newCapacity);
		}
		new (_end) T(std::move(_end[-1]));
		for (T *dest = _end - 1; dest > _begin; --dest) {
			*dest = std::move(dest[-1]);
		}
		++_end;
		new (_begin) T(std::forward<Args>(args)...);
	}
	void erase(T *val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		val->~T();
		--_end;
		for (T *dest = val; dest != _end; ++dest) {
			*dest = std::move(dest[1]);
		}
		_end->~T();
	}
	void erase(T &val) { erase(std::addressof(val)); }
};

template <class T, class Allocator = OsAllocator>
struct UnorderedList : AllocListBase<T, Allocator> {
	using Base = AllocListBase<T, Allocator>;
	UnorderedList() = default;
	explicit UnorderedList(umm size) : Base(size) {}
	UnorderedList(Span<T const> span) : Base(span) {}
	UnorderedList(UnorderedList const &that) = default;
	UnorderedList(UnorderedList &&that) = default;
	UnorderedList &operator=(UnorderedList const &that) = default;
	UnorderedList &operator=(UnorderedList &&that) = default;
	template <class... Args>
	void push_front(Args &&... args) {
		if (remainingCapacity() == 0) {
			umm newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			reallocate(newCapacity);
		}
		new (_end) T(std::move(*_begin));
		++_end;
		new (_begin) T(std::forward<Args>(args)...);
	}
	void erase(T *val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		val->~T();
		new (val) T(std::move(*(_end-- - 1)));
	}
	void erase(T &val) { erase(&val); }
};

template <class T, class Allocator = OsAllocator>
struct LinkedList {
	struct Node {
		T value;
		Node *next;
	};
	struct Iterator {
		Node *node;

		Iterator() : node() {}
		Iterator(Node *node) : node(node) {}
		T &operator*() { return node->value; }
		Iterator &operator++() {
			node = node->next;
			return *this;
		}
		Iterator operator++(int) {
			Node *prev = node;
			node	   = node->next;
			return prev;
		}
		bool operator==(Iterator const &that) const { return node == that.node; }
		bool operator!=(Iterator const &that) const { return node != that.node; }
	};

	LinkedList() = default;
	~LinkedList() { clear(); }

	void clear() {
		Node *node = head;
		for (;;) {
			if (node == 0)
				break;
			DEFER { Allocator::deallocate(node); };
			node = node->next;
		}
		head = last = 0;
	}

	template <class... Args>
	void emplace_back(Args &&... args) {
		if (head == 0) {
			head = last = allocate<Node, Allocator>();
		} else {
			last->next = allocate<Node, Allocator>();
			last	   = last->next;
		}
		last->next = 0;

		new (&last->value) T(std::forward<Args>(args)...);
	}
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }

	T &back() { return last->value; }
	T const &back() const { return last->value; }

	Iterator begin() { return head; }
	Iterator end() { return {}; }

private:
	Node *head = 0;
	Node *last = 0;
};

#if 0
template <class T, class Allocator>
struct StringBase {
	StringBase() = default;
	explicit StringBase(umm length) {
		resize(length);
	}
	StringBase(StringBase &&that) {
		_begin		   = that._begin;
		_end		   = that._end;
		_allocEnd	   = that._allocEnd;
		that._begin	   = 0;
		that._end	   = 0;
		that._allocEnd = 0;
	}
	StringBase(StringBase const &that) {
		_begin	  = allocate<T, Allocator>(that.size());
		_allocEnd = _end = _begin + that.size();
		for (T *src = that._begin, *dst = _begin; dst != _end; ++src, ++dst) {
			new (dst) T(*src);
		}
	}
	~StringBase() {
		clear();
		if (_begin)
			Allocator::deallocate(_begin);
		_begin	  = 0;
		_end	  = 0;
		_allocEnd = 0;
	}
	StringBase &operator=(StringBase const &that) {
		clear();
		reserve(that.size());
		_end = _begin + that.size();
		for (T *src = that._begin, *dst = _begin; src != that._end; ++src, ++dst) {
			new (dst) T(*src);
		}
		return *this;
	}
	StringBase &operator=(StringBase &&that) {
		clear();
		if (_begin)
			Allocator::deallocate(_begin);
		_begin		   = that._begin;
		_end		   = that._end;
		_allocEnd	   = that._allocEnd;
		that._begin	   = 0;
		that._end	   = 0;
		that._allocEnd = 0;
		return *this;
	}

	T *begin() { return _begin; }
	T *end() { return _end; }
	T const *begin() const { return _begin; }
	T const *end() const { return _end; }

	umm remainingCapacity() const { return (umm)(_allocEnd - _end); }
	umm capacity() const { return (umm)(_allocEnd - _begin); }
	
	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return _begin; }
	T &front() { return *_begin; }
	T &back() { return _end[-1]; }
	T &operator[](umm i) { return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { return *_begin; }
	T const &back() const { return _end[-1]; }
	T const &operator[](umm i) const { return _begin[i]; }

	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }
	void pop_back() { (--_end)->~T(); }
	void reserve(umm count) {
		if (count > capacity())
			_reallocate(count);
	}
	void resize(umm newSize, T c = {}) {
		if (newSize > capacity())
			_reallocate(newSize);
		if (newSize > size()) {
			for (T *t = _begin + size(); t < _begin + newSize; ++t)
				*t = c;
			_end = _begin + newSize;
		} else if (newSize < size()) {
			_end = _begin + newSize;
		}
	}
	void clear() {
		for (auto &val : *this) {
			val.~T();
		}
		_end = _begin;
	}

	operator Span<T>() { return {begin(), end()}; }
	operator Span<T const>() const { return {begin(), end()}; }

	T *_begin	 = 0;
	T *_end		 = 0;
	T *_allocEnd = 0;

	void _reallocate(umm newCapacity) {
		ASSERT(capacity() < newCapacity);
		umm oldSize = size();
		T *newBegin = allocate<T, Allocator>(newCapacity + 1);
		if (_begin) {
			memcpy(newBegin, _begin, sizeof(T) * (size() + 1)));
			Allocator::deallocate(_begin);
		}
		_begin	  = newBegin;
		_end	  = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	template <class... Args>
	void emplace_back(Args &&... args) {
		if (remainingCapacity() == 0) {
			umm newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			_reallocate(newCapacity);
		}
		new (_end++) T(std::forward<Args>(args)...);
	}
};

template <class Allocator = OsAllocator>
using String = StringBase<char, Allocator>;
#else 

template <class Allocator = OsAllocator>
struct String : List<char, Allocator> {
	using Base = List;
	String() = default;
	explicit String(umm size) : Base(size) {}
	String(Span<char const> span) : Base(span) {}
	String(String const &that) = default;
	String(String &&that) = default;
	String &operator=(String const &that) = default;
	String &operator=(String &&that) = default;
	String &set(Span<char const> span) { return Base::set(span), *this; }
	String(char const *str) : Base(str, strlen(str)) {}
};
#endif

template <class Allocator, class ...Args>
String<Allocator> toString(Args const &...args) {
	String<Allocator> result;
	toString(args..., [&](char const *src, umm length) {
		result.resize(length);
		memcpy(result.data(), src, length);
	}); 
	return result;
}

template <class Allocator, class ...Args>
String<Allocator> toStringNT(Args const &...args) {
	String<Allocator> result;
	toString(args..., [&](char const *src, umm length) {
		result.resize(length + 1);
		memcpy(result.data(), src, length);
		result[length] = '\0';
	}); 
	return result;
}

template <class Allocator>
String<Allocator> nullTerminate(Span<char const> span) {
	String<Allocator> result;
	result.resize(span.size() + 1);
	memcpy(result.data(), span.data(), span.size());
	result[span.size()] = '\0';
	return result;
}

template <class Allocator = OsAllocator, umm bufferSize = 4096>
struct StringBuilder {
	using String = String<Allocator>;
	struct Block {
		char buffer[bufferSize];
		char *end = buffer;
		Block *next = 0;

		Block() {}
		umm size() { return (umm)(end - buffer); }
		umm remaining() { return bufferSize - size(); }
	};
	Block first;
	Block *last = &first;

	StringBuilder() = default;
	StringBuilder(StringBuilder const &) = delete;
	StringBuilder(StringBuilder &&) = default;
	~StringBuilder() {
		Block *next = 0;
		for (Block *block = first.next; block != 0; block = next) {
			next = block->next;
			Allocator::deallocate(block);
		}
	}

	void append(Span<char const> span) {
		if (!span.data())
			span = {"(null)", 6};

		umm charsToWrite = span.size();
		while (last->remaining() < charsToWrite) {
			memcpy(last->end, span.data(), last->remaining());
			charsToWrite -= last->remaining();
			last->end += last->remaining();
			span = {span.begin() + last->remaining(), span.end()};

			Block *newBlock = construct(allocate<Block, Allocator>());
			last->next = newBlock;
			last = newBlock;
		}
		memcpy(last->end, span.data(), charsToWrite);
		last->end += charsToWrite;
	}
	void append(char const *str) { append(Span{str, str ? strlen(str) : 0}); }
	void append(char c) { append(Span{&c, 1}); }
	String get(bool terminate = false) {
		umm totalSize = 0;
		for (Block *block = &first; block != 0; block = block->next) {
			totalSize += block->size();
		}
		if (terminate)
			++totalSize;
		String result(totalSize);
		char *dst = result.data();
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dst, block->buffer, block->size());
			dst += block->size();
		}
		if (terminate)
			result.back() = '\0';
		return result;
	}
	String getTerminated() { return get(true); }
};

template <class T>
struct Optional {
	Optional() : _hasValue(false) {}
	Optional(T const &that) : _value(that), _hasValue(true) {}
	Optional(T &&that) : _value(std::move(that)), _hasValue(true) {}
	~Optional() {
		clear();
	}
	void clear() {
		if (_hasValue) {
			_value.~T();
		}
	}
	template <class ...Args>
	void emplace(Args &&...args) {
		if (_hasValue)
			clear();
		new (std::addressof(_value)) T(std::forward<Args>(args)...);
		_hasValue = true;
	}
	T &value() & { ASSERT(_hasValue); return _value; }
	T &&value() && { ASSERT(_hasValue); return _value; }
	T const &value() const & { ASSERT(_hasValue); return std::move(_value); }
	T const &&value() const && { ASSERT(_hasValue); return std::move(_value); }
	T *operator->() { return std::addressof(_value); }
	T const *operator->() const { return std::addressof(_value); }

	operator bool() const { return _hasValue; }

	union {
		T _value;
	};
	bool _hasValue;
};


template <class T>
void erase(UnorderedList<T> &list, T &val) {
	list.erase(val);
}
template <class T>
void erase(UnorderedList<T> &list, T *val) {
	list.erase(val);
}

#pragma warning(pop)
} // namespace TL