#pragma once
#define IS_WINDOWS 0
#define IS_LINUX   0

#if defined _WIN32 || defined _WIN64
#undef IS_WINDOWS
#define IS_WINDOWS 1
#else
#undef IS_LINUX
#define IS_LINUX 1
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

#define ARCH_X86 0
#define ARCH_X64 0
#define ARCH_AVX 0
#define ARCH_AVX2 0
#define ARCH_AVX512 0

#if COMPILER_MSVC
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

#if !(ARCH_X86|ARCH_X64)
#error "Unresolved target architecture"
#endif

#define STRINGIZE_(x) #x
#define STRINGIZE(x)  STRINGIZE_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y)  CONCAT_(x, y)

#define ASSERT(x, ...)                                                                                              \
	do {                                                                                                            \
		if (!(x)) {                                                                                                 \
			DEBUG_BREAK();                                                                                          \
			throw std::runtime_error("ASSERT\nFile: " __FILE__                                                      \
									 "\nLine: " STRINGIZE(__LINE__) "\nExpression: " #x "\nMessage: " __VA_ARGS__); \
		}                                                                                                           \
	} while (0)

#define INVALID_CODE_PATH(...)                                                              \
	do {                                                                                    \
		DEBUG_BREAK();                                                                      \
		throw std::runtime_error("INVALID_CODE_PATH\nFile: " __FILE__                       \
								 "\nLine: " STRINGIZE(__LINE__) "\nMessage: " __VA_ARGS__); \
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

constexpr size_t length(char const* string) {
	size_t result = 0;
	while (*string++)
		++result;
	return result;
}

size_t length(wchar const* string) {
	alignof(wchar);
	return wcslen(string);
}

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
	constexpr auto data() const { return _begin; }
	constexpr auto begin() const { return _begin; }
	constexpr auto end() const { return _end; }
	constexpr size_t size() const { return size_t(_end - _begin); }
	constexpr T const& operator[](size_t i) const { return _begin[i]; }

private:
	T const* _begin{};
	T const* _end{};
};

template <>
struct View<char> {
	using T = char;
	constexpr View() = default;
	constexpr View(T const* begin, T const* end) : _begin(begin), _end(end) {}
	constexpr View(T const* begin, size_t size) : View(begin, begin + size) {}
	constexpr View(Span<T> s) : View(s.begin(), s.end()) {}
	template<size_t size>
	constexpr View(T const (&string)[size]) : View(string, size) {}
	constexpr explicit View(T const* string) : View(string, length(string)) {}
	constexpr auto data() const { return _begin; }
	constexpr auto begin() const { return _begin; }
	constexpr auto end() const { return _end; }
	constexpr size_t size() const { return size_t(_end - _begin); }
	constexpr T const& operator[](size_t i) const { return _begin[i]; }

private:
	T const* _begin{};
	T const* _end{};
};

template <>
struct View<wchar> {
	using T = wchar;
	constexpr View() = default;
	constexpr View(T const* begin, T const* end) : _begin(begin), _end(end) {}
	constexpr View(T const* begin, size_t size) : View(begin, begin + size) {}
	constexpr View(Span<T> s) : View(s.begin(), s.end()) {}
	template<size_t size>
	constexpr View(T const (&string)[size]) : View(string, size) {}
	constexpr explicit View(T const* string) : View(string, length(string)) {}
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

#define DEFER ::TL::Deferrer CONCAT(_deferrer, __LINE__) = [&]()

template<class Cont>
auto reverse(Cont& cont) {
	struct {
		auto begin() {return _begin; }
		auto end() {return _end; }
		decltype(std::rbegin(cont)) _begin;
		decltype(std::rend(cont)) _end;
	} result;
	result._begin = std::rbegin(cont);
	result._end = std::rend(cont);
	return result;
}

} // namespace TL