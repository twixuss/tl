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

#include <utility>

#if COMPILER_MSVC
#pragma warning(pop)
#endif

#if COMPILER_MSVC

#define FORCEINLINE			 __forceinline
#define DEBUG_BREAK()		 ::__debugbreak()
#define WRITE_BARRIER()		 ::_WriteBarrier()
#define READ_BARRIER()		 ::_ReadBarrier()
#define READ_WRITE_BARRIER() ::_ReadWriteBarrier()

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
#define ASSERTION_FAILURE(causeString, expression, ...) DEBUG_BREAK()
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

#pragma warning(push)
#pragma warning(disable : 4309)
template<class T> constexpr T min() = delete;
template<class T> constexpr T max() = delete;
template<> constexpr u8  min() { return 0; }
template<> constexpr u16 min() { return 0; }
template<> constexpr u32 min() { return 0; }
template<> constexpr u64 min() { return 0; }
template<> constexpr u8  max() { return 0xFF; }
template<> constexpr u16 max() { return 0xFFFF; }
template<> constexpr u32 max() { return 0xFFFFFFFF; }
template<> constexpr u64 max() { return 0xFFFFFFFFFFFFFFFF; }

template<> constexpr s8  min() { return 0x80; }
template<> constexpr s16 min() { return 0x8000; }
template<> constexpr s32 min() { return 0x80000000; }
template<> constexpr s64 min() { return 0x8000000000000000; }
template<> constexpr s8  max() { return 0x7F; }
template<> constexpr s16 max() { return 0x7FFF; }
template<> constexpr s32 max() { return 0x7FFFFFFF; }
template<> constexpr s64 max() { return 0x7FFFFFFFFFFFFFFF; }
#pragma warning(pop)

template <class... Callables>
struct Combine : public Callables... {
	constexpr Combine(Callables&&... c) : Callables(std::move(c))... {}
	using Callables::operator()...;
};

template <class Enum, class = std::enable_if_t<std::is_enum_v<Enum>>>
auto toInt(Enum e) {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

template<class T>
void populate(void *_dst, T const &src, umm size) {
	T *dst = (T *)_dst;
	while (size--) {
		memcpy(dst++, std::addressof(src), sizeof(T));
	}
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
	Span(T* begin, umm size) : Span(begin, begin + size) {}
	auto data() const { return _begin; }
	auto begin() const { return _begin; }
	auto end() const { return _end; }
	umm size() const { return umm(_end - _begin); }
	T& operator[](umm i) const { return _begin[i]; }

private:
	T* _begin{};
	T* _end{};
};

template <class T>
struct View {
	constexpr View() = default;
	constexpr View(T const* begin, T const* end) : _begin(begin), _end(end) {}
	constexpr View(T const* begin, umm size) : View(begin, begin + size) {}
	constexpr View(Span<T> s) : View(s.begin(), s.end()) {}
	template <umm size>
	constexpr View(T const (&arr)[size]) : View(arr, size) {}
	constexpr View(T const &value) : View(&value, 1) {}
	constexpr auto data() const { return _begin; }
	constexpr auto begin() const { return _begin; }
	constexpr auto end() const { return _end; }
	constexpr umm size() const { return umm(_end - _begin); }
	constexpr T const& operator[](umm i) const { return _begin[i]; }

private:
	T const* _begin{};
	T const* _end{};
};

using StringSpan = Span<char>;
using StringView = View<char>;

bool startsWith(char const *str, char const *subStr) {
	while (*subStr) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}
bool startsWith(char const *str, umm strLength, char const *subStr) {
	while (*subStr && strLength--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return !*subStr;
}
bool startsWith(char const *str, char const *subStr, umm subStrLength) {
	while (subStrLength--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}
bool startsWith(char const *str, umm strLength, char const *subStr, umm subStrLength) {
	if (strLength < subStrLength)
		return false;
	while (strLength-- && subStrLength--) {
		if (*str++ != *subStr++) {
			return false;
		}
	}
	return true;
}

template<class T, class Derived>
struct ListBase {
	umm size() const { return (umm)(_d().end() - _d().begin()); }
	bool empty() const { return size() == 0; }

	T *data() { return _d().begin(); }
	T &front() { return *_d().begin(); }
	T &back() { return _d().end()[-1]; }
	T &operator[](umm i) { return _d().begin()[i]; }
	
	T const *data() const { return _d().begin(); }
	T const &front() const { return *_d().begin(); }
	T const &back() const { return _d().end()[-1]; }
	T const &operator[](umm i) const { return _d().begin()[i]; }

	Derived &_d() { return *(Derived *)this; }
	Derived const &_d() const { return *(Derived *)this; }
};

template<class T, class Derived>
struct HeapListBase : ListBase<T, Derived> {
	void _init(HeapListBase &&that) {
		_begin		   = that._begin;
		_end		   = that._end;
		_allocEnd	   = that._allocEnd;
		that._begin	   = 0;
		that._end	   = 0;
		that._allocEnd = 0;
	}
	void _init(HeapListBase const &that) {
		_begin	  = (T *)malloc(that.size() * sizeof(T));
		_allocEnd = _end = _begin + that.size();
		for (T *src = that._begin, *dst = _begin; dst != _end; ++src, ++dst) {
			new (dst) T(*src);
		}
	}
	void _deinit() {
		clear();
		if (_begin)
			free(_begin);
		_begin	  = 0;
		_end	  = 0;
		_allocEnd = 0;
	}
	void _set(HeapListBase const &that) {
		clear();
		reserve(that.size());
		_end = _begin + that.size();
		for (T *src = that._begin, *dst = _begin; src != that._end; ++src, ++dst) {
			new (dst) T(*src);
		}
	}
	void _set(HeapListBase &&that) {
		clear();
		_begin		   = that._begin;
		_end		   = that._end;
		_allocEnd	   = that._allocEnd;
		that._begin	   = 0;
		that._end	   = 0;
		that._allocEnd = 0;
	}
	
	T *begin() { return _begin; }
	T *end() { return _end; }
	T const *begin() const { return _begin; }
	T const *end() const { return _end; }

	umm remainingCapacity() const { return (umm)(_allocEnd - _end); }
	umm capacity() const { return (umm)(_allocEnd - _begin); }

	void push_back(T const &val) { _push_back(val); }
	void push_back(T &&val) { _push_back(std::move(val)); }
	void pop_back() { (--_end)->~T(); }
	void reserve(umm count) {
		if (count > capacity())
			reallocate(count);
	}
	void resize(umm newSize) {
		if (newSize > capacity())
			reallocate(newSize);
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
	void erase(T &val) { _d().erase(&val); }

	operator View<T>() const { return {begin(), end()}; }
	operator Span<T>() { return {begin(), end()}; }

	T *_begin	 = 0;
	T *_end		 = 0;
	T *_allocEnd = 0;

	void reallocate(umm newCapacity) {
		ASSERT(capacity() < newCapacity);
		umm oldSize = size();
		T *newBegin = (T *)malloc(newCapacity * sizeof(T));
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			free(_begin);
		_begin	  = newBegin;
		_end	  = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	template <class... Args>
	void _push_back(Args &&... args) {
		if (remainingCapacity() == 0) {
			umm newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			reallocate(newCapacity);
		}
		new (_end++) T(std::forward<Args>(args)...);
	}
};

template <class T, umm capacity>
struct StaticList : ListBase<T, StaticList<T, capacity>> {
	StaticList() = default;
	template<umm thatCapacity>
	StaticList(StaticList<T, thatCapacity> const &that) {
		ASSERT(that.size() <= capacity);
		for (auto &src : that) {
			push_back(src);
		}
	}
	template<umm thatCapacity>
	StaticList(StaticList<T, thatCapacity> &&that) {
		ASSERT(that.size() <= capacity);
		for (auto &src : that) {
			push_back(std::move(src));
		}
		that.clear();
	}
	template<umm thatCapacity>
	StaticList &operator=(StaticList<T, thatCapacity> const &that) {
		clear();
		return *new (this) StaticList(that);
	}
	template<umm thatCapacity>
	StaticList &operator=(StaticList<T, thatCapacity> &&that) {
		clear();
		return *new (this) StaticList(std::move(that));
	}
	~StaticList() {
		clear();
	}

	T *begin() { return &_begin->v; }
	T *end() { return &_end->v; }
	T const *begin() const { return &_begin->v; }
	T const *end() const { return &_end->v; }
	
	bool full() const { return size() == capacity; }

	template <class ...Args>
	bool try_emplace_back(Args &&...args) { 
		if (full()) 
			return false; 
		new (_end++) T(std::forward<Args>(args)...); 
		return true;
	}
	bool try_push_back(T const &val) { return try_emplace_back(val); }
	bool try_push_back(T &&val) { return try_emplace_back(std::move(val)); }
	
	template <class ...Args>
	void emplace_back(Args &&...args) { ASSERT(try_emplace_back(std::forward<Args>(args)...)); }
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }
	
	template <class ...Args>
	void emplace_back_pop_front(Args &&...args) { 
		if (full()) 
			pop_front(); 
		emplace_back(std::forward<Args>(args)...); 
	}
	void push_back_pop_front(T const &val) { emplace_back_pop_front(val); }
	void push_back_pop_front(T &&val) { emplace_back_pop_front(std::move(val)); }
	
	void pop_back() {
		ASSERT(size());
		_end--[-1].v.~T();
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

	template<class Pred>
	T *find_if(Pred pred) {
		Storage *it = _begin;
		for (;it < _end; ++it) {
			if (pred(it->v)) {
				break;
			}
		}
		return &it->v;
	}
private:
	union Storage {
		T v;
#pragma warning(suppress : 4582)
		Storage(){}
		~Storage(){}
	};
	Storage _begin[capacity];
	Storage *_end = _begin;
};

template <class T>
struct List : HeapListBase<T, List<T>> {
	List() = default;
	List(List const &that) { _init(that); }
	List(List &&that) { _init(std::move(that)); }
	List &operator=(List const &that) { return _set(that), *this; }
	List &operator=(List &&that) { return _set(std::move(that)), *this; }
	~List() { _deinit(); }
	template <class... Args>
	void push_front(Args&&... args) {
		if (remainingCapacity() == 0) {
			umm newCapacity = capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			reallocate(newCapacity);
		}
		new (_end) T(std::move(_end[-1]));
		for (T* dest = _end - 1; dest > _begin; --dest) {
			*dest = std::move(dest[-1]);
		}
		++_end;
		new (_begin) T(std::forward<Args>(args)...);
	}
	void erase(T* val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		val->~T();
		--_end;
		for (T* dest = val; dest != _end; ++dest) {
			*dest = std::move(dest[1]);
		}
		_end->~T();
	}
	using HeapListBase::erase;
};

template <class T>
struct UnorderedList : HeapListBase<T, UnorderedList<T>> {
	UnorderedList() = default;
	UnorderedList(UnorderedList const &that) { _init(that); }
	UnorderedList(UnorderedList &&that) { _init(std::move(that)); }
	UnorderedList &operator=(UnorderedList const &that) { return _set(that), *this; }
	UnorderedList &operator=(UnorderedList &&that) { return _set(std::move(that)), *this; }
	~UnorderedList() { _deinit(); }
	template <class... Args>
	void push_front(Args&&... args) {
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
	void erase(T* val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		val->~T();
		new (val) T(std::move(*(_end-- - 1)));
	}
	using HeapListBase::erase;
};

template <class T>
void erase(UnorderedList<T>& list, T& val) {
	list.erase(val);
}
template <class T>
void erase(UnorderedList<T>& list, T* val) {
	list.erase(val);
}

#undef LIST_BASE

#define DEFER ::TL::Deferrer CONCAT(_deferrer, __LINE__) = [&]()

} // namespace TL