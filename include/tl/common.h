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

#if defined __GNUG__
#undef COMPILER_GCC
#define COMPILER_GCC 1
#elif defined _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#pragma message "TL: Unresolved compiler"
#endif

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif

#include <stdexcept>
#include <utility>

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#define ARCH_X86	0
#define ARCH_X64	0
#define ARCH_AVX	0
#define ARCH_AVX2	0
#define ARCH_AVX512 0

#if COMPILER_MSVC
#define FORCEINLINE			 __forceinline
#define DEBUG_BREAK()		 ::__debugbreak()
#define WRITE_BARRIER()		 ::_WriteBarrier()
#define READ_BARRIER()		 ::_ReadBarrier()
#define READ_WRITE_BARRIER() ::_ReadWriteBarrier()
#if defined _M_IX86
#undef ARCH_X86
#define ARCH_X86 1
#elif defined _M_X64
#undef ARCH_X64
#define ARCH_X64 1
#endif
#ifdef __AVX__
#undef ARCH_AVX
#define ARCH_AVX 1
#endif
#ifdef __AVX2__
#undef ARCH_AVX2
#define ARCH_AVX2 1
#endif
#ifdef __AVX512F__
#undef ARCH_AVX512
#define ARCH_AVX512 1
#endif
#elif COMPILER_GCC
#define DEBUG_BREAK() ::__builtin_trap()
#endif

#if !(ARCH_X86 | ARCH_X64)
#error "Unresolved target architecture"
#endif

#define STRINGIZE_(x) #x
#define STRINGIZE(x)  STRINGIZE_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y)  CONCAT_(x, y)

#ifndef ASSERTION_FAILURE
#define ASSERTION_FAILURE(causeString, expression, ...)                                                                     \
	DEBUG_BREAK();                                                                                              \
	throw std::runtime_error(causeString "\nFile: " __FILE__ "\nLine: " STRINGIZE(__LINE__) "\nExpression: " expression \
																							"\nMessage: " __VA_ARGS__)
#endif

#define ASSERT(x, ...)                                \
	do {                                              \
		if (!(x)) {                                   \
			ASSERTION_FAILURE("ASSERT", #x, __VA_ARGS__); \
		}                                             \
	} while (0)

#define INVALID_CODE_PATH(...)                               \
	do {                                                     \
		ASSERTION_FAILURE("INVALID_CODE_PATH", "", __VA_ARGS__); \
	} while (0)

namespace TL {

typedef wchar_t wchar;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef s32 b32;
template <class... Callables>
struct Combine : public Callables... {
	constexpr Combine(Callables&&... c) : Callables(std::move(c))... {}
	using Callables::operator()...;
#pragma warning(suppress : 5027) // TODO
};

template <class Enum, class = std::enable_if_t<std::is_enum_v<Enum>>>
auto toInt(Enum e) {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class Fn>
struct Deferrer {
	Deferrer(Fn&& fn) : fn(std::forward<Fn>(fn)) {}
	~Deferrer() { fn(); }

private:
	Fn fn;
};

template <class T>
struct Span {
	Span() = default;
	Span(T* begin, T* end) : _begin(begin), _end(end) {}
	Span(T* begin, size_t size) : Span(begin, begin + size) {}
	auto data() const { return _begin; }
	auto begin() const { return _begin; }
	auto end() const { return _end; }
	size_t size() const { return size_t(_end - _begin); }
	T& operator[](size_t i) const { return _begin[i]; }

private:
	T* _begin{};
	T* _end{};
};

template <class T>
struct View {
	constexpr View() = default;
	constexpr View(T const* begin, T const* end) : _begin(begin), _end(end) {}
	constexpr View(T const* begin, size_t size) : View(begin, begin + size) {}
	constexpr View(Span<T> s) : View(s.begin(), s.end()) {}
	template <size_t size>
	constexpr View(T const (&arr)[size]) : View(arr, size) {}
	constexpr auto data() const { return _begin; }
	constexpr auto begin() const { return _begin; }
	constexpr auto end() const { return _end; }
	constexpr size_t size() const { return size_t(_end - _begin); }
	constexpr T const& operator[](size_t i) const { return _begin[i]; }

private:
	T const* _begin{};
	T const* _end{};
};

using StringSpan = Span<char>;
using StringView = View<char>;

template <class T>
struct UnorderedList {
	UnorderedList() = default;
	UnorderedList(UnorderedList&& that) {
		_begin = that._begin;
		_end = that._end;
		_allocEnd = that._allocEnd;
		that._begin = 0;
		that._end = 0;
		that._allocEnd = 0;
	}
	UnorderedList(UnorderedList const& that) {
		_begin = (T*)malloc(that.size() * sizeof(T));
		_allocEnd = _end = _begin + that.size();
		for (T *src = that._begin, *dst = _begin; dst != _end; ++src, ++dst) {
			new (dst) T(*src);
		}
	}
	~UnorderedList() {
		if (_begin)
			free(_begin);
		_begin = 0;
		_end = 0;
		_allocEnd = 0;
	}
	void reserve(size_t count) {
		if (count <= capacity())
			return;
		reallocate(count);
	}
	template <class... Args>
	void _push_back(Args&&... args) {
		if (remainingCapacity() == 0) {
			size_t newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			reallocate(newCapacity);
		}
		new (_end++) T(std::forward<Args>(args)...);
	}
	void push_back(T const& val) { _push_back(val); }
	void push_back(T&& val) { _push_back(std::move(val)); }
	size_t remainingCapacity() const { return (size_t)(_allocEnd - _end); }
	size_t capacity() const { return (size_t)(_allocEnd - _begin); }
	size_t size() const { return (size_t)(_end - _begin); }
	void erase(T& val) { erase(&val); }
	void erase(T* val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		new (val) T(std::move(*(_end-- - 1)));
	}

	T* data() { return _begin; }
	T* begin() { return _begin; }
	T* end() { return _end; }
	T& operator[](size_t i) { return _begin[i]; }

	T const* data() const { return _begin; }
	T const* begin() const { return _begin; }
	T const* end() const { return _end; }
	T const& operator[](size_t i) const { return _begin[i]; }

private:
	T* _begin = 0;
	T* _end = 0;
	T* _allocEnd = 0;

	void reallocate(size_t newCapacity) {
		size_t oldSize = size();
		T* newBegin = (T*)malloc(newCapacity * sizeof(T));
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			free(_begin);
		_begin = newBegin;
		_end = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
};
template <class T>
void erase(UnorderedList<T>& list, T& val) {
	list.erase(val);
}
template <class T>
void erase(UnorderedList<T>& list, T* val) {
	list.erase(val);
}

#if 0
template <class T>
void erase(std::vector<T>& vec, T* val) {
	vec.erase(vec.begin() + (val - vec.data()));
}
template <class T>
void erase(std::vector<T>& vec, T& val) {
	erase(vec, &val);
}
#endif

#define DEFER ::TL::Deferrer CONCAT(_deferrer, __LINE__) = [&]()

template <class Cont>
auto reverse(Cont& cont) {
	struct {
		auto begin() { return _begin; }
		auto end() { return _end; }
		decltype(std::rbegin(cont)) _begin;
		decltype(std::rend(cont)) _end;
	} result;
	result._begin = std::rbegin(cont);
	result._end	  = std::rend(cont);
	return result;
}

} // namespace TL