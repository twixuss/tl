#pragma once
#include "common.h"
#include <string.h>

#pragma warning(push)
#pragma warning(disable : 4582)

namespace TL {

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
	void push_back(T &&val) { emplace_back(move(val)); }
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
			new (dst) T(move(*src));
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
		new ((void *)_end++) T(forward<Args>(args)...);
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
			push_back(move(src));
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
			push_back(move(src));
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
		return *new (this) StaticList(move(that));
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
		new (_end++) T(forward<Args>(args)...);
		return true;
	}
	bool try_push_back(T const &val) { return try_emplace_back(val); }
	bool try_push_back(T &&val) { return try_emplace_back(move(val)); }

	template <class... Args>
	void emplace_back(Args &&... args) {
		ASSERT(try_emplace_back(forward<Args>(args)...));
	}
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(move(val)); }

	template <class... Args>
	void emplace_back_pop_front(Args &&... args) {
		if (full())
			pop_front();
		emplace_back(forward<Args>(args)...);
	}
	void push_back_pop_front(T const &val) { emplace_back_pop_front(val); }
	void push_back_pop_front(T &&val) { emplace_back_pop_front(move(val)); }

	void pop_back() {
		ASSERT(size());
		_end-- [-1].v.~T();
	}
	void pop_front() {
		ASSERT(size());
		--_end;
		for (auto dst = _begin; dst != _end; ++dst) {
			dst[0] = move(dst[1]);
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
		new (_end) T(move(_end[-1]));
		for (T *dest = _end - 1; dest > _begin; --dest) {
			*dest = move(dest[-1]);
		}
		++_end;
		new (_begin) T(forward<Args>(args)...);
	}
	void erase(T *val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		val->~T();
		--_end;
		for (T *dest = val; dest != _end; ++dest) {
			*dest = move(dest[1]);
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
		new (_end) T(move(*_begin));
		++_end;
		new (_begin) T(forward<Args>(args)...);
	}
	void erase(T *val) {
		ASSERT(_begin <= val && val < _end, "value is not in container");
		val->~T();
		new (val) T(move(*(_end-- - 1)));
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

		new (&last->value) T(forward<Args>(args)...);
	}
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(move(val)); }

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
	void push_back(T &&val) { emplace_back(move(val)); }
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
		new (_end++) T(forward<Args>(args)...);
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

	umm append(Span<char const> span) {
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
		return span.size();
	}
	umm append(char const *str) { return append(Span{str ? str : "(null)", str ? strlen(str) : 0}); }
	umm append(char c) { return append(Span{&c, 1}); }
	
	umm appendFormat(char const *fmt) {
		return append(Span{fmt, strlen(fmt)});
	}
	template <class Arg, class ...Args>
	umm appendFormat(char const *fmt, Arg const &arg, Args const &...args) {
		if (!*fmt)
			return 0;
		char const *c = fmt;
		char const *fmtBegin = 0;
		char const *fmtEnd = 0;
		bool argAsFormat = false;
		u32 align = 0;
		bool alignRight;
		while (*c) {
			if (*c == '{') {
				fmtBegin = c;
				++c;
				if (*c == '}') {
					fmtEnd = c + 1;
					break;
				}
				if (startsWith(c, "fmt}")) {
					argAsFormat = true;
					fmtEnd = c + 4;
					break;
				}
				if (*c == 'a') {
					++c;
					if (*c == 'r') {
						alignRight = true;
					} else if (*c == 'l') {
						alignRight = false;
					} else {
						INVALID_CODE_PATH("invalid format: bad align");
					}
					ASSERT(*++c == ':', "invalid format: missing ':' after align");
					++c;
					char buf[64];
					char *d = buf;
					while (*c != '}') {
						*d++ = *c++;
					}
					ASSERT(d != buf, "invalid format: missing number after ':'");
					*d = '\0';
					align = atoi(buf);
					ASSERT(align > 0, "invalid format: bad align");
					fmtEnd = c + 1;
					break;
				}
				INVALID_CODE_PATH("invalid format");
			}
			++c;
		}
		//ASSERT(fmtBegin && fmtEnd, "invalid format");
		if (!(fmtBegin && fmtEnd)) {
			return appendFormat(fmt);
		}

		umm charsAppended = append(Span{fmt, fmtBegin});

		auto appendArg = [&] (auto &builder) {
			umm result;
			if (argAsFormat) {
				if constexpr (isSame<RemoveConst<Arg>, char const *> || isSame<RemoveConst<Arg>, char *>)
					result = builder.appendFormat(*(char **)addressOf(arg), args...);
				else 
					INVALID_CODE_PATH("'{fmt}' arg is not a string");
			} else {
				toString(arg, [&] (char const *src, umm length) {
					result = length;
					builder.append(Span{src, length});
					return 0;
				});
			}
			charsAppended += result;
			return result;
		};
		
		if (align) {
			if (alignRight) {
				StringBuilder temp;
				umm argLen = appendArg(temp);
				if (align > argLen) {
					umm spaceCount = align - argLen;
					for (umm i = 0; i < spaceCount; ++i) {
						append(' ');
					}
					charsAppended += spaceCount;
				}
				append(temp.get());
			} else {
				umm argLen = appendArg(*this);
				if (align > argLen) {
					umm spaceCount = align - argLen;
					for (umm i = 0; i < spaceCount; ++i) {
						append(' ');
					}
					charsAppended += spaceCount;
				}
			}
		} else {
			appendArg(*this);
		}
		charsAppended += appendFormat(fmtEnd, args...);
		return charsAppended;
	}
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

// --- Format options: ---						--- Example ---							--- Result ---
// {} - append an argument using 'toString'		format("{}", 5)							"5"
// {ar:N} - align right, N is width				format("{ar:3}", 5)						"  5"
// {al:N} - align left, N is width				format("{al:3}", 5)						"5  "
// {fmt} - append an argument as a format		format("{} {fmt}", 1, "{} {}", 2, 3)	"1 2 3"		NOTE: this option should be last in the format string
template <class Allocator = OsAllocator, class ...Args>
String<Allocator> format(char const *fmt, Args const &...args) {
	StringBuilder<Allocator> builder;
	builder.appendFormat(fmt, args...);
	return builder.get();
}

template <class Allocator = OsAllocator, class ...Args>
String<Allocator> formatAndTerminate(char const *fmt, Args const &...args) {
	StringBuilder<Allocator> builder;
	builder.appendFormat(fmt, args...);
	builder.append('\0');
	return builder.get();
}

template <class T>
struct Optional {
	Optional() : _hasValue(false) {}
	Optional(T const &that) : _value(that), _hasValue(true) {}
	Optional(T &&that) : _value(move(that)), _hasValue(true) {}
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
		new (std::addressof(_value)) T(forward<Args>(args)...);
		_hasValue = true;
	}
	T &value() & { ASSERT(_hasValue); return _value; }
	T &&value() && { ASSERT(_hasValue); return _value; }
	T const &value() const & { ASSERT(_hasValue); return move(_value); }
	T const &&value() const && { ASSERT(_hasValue); return move(_value); }
	T *operator->() { return std::addressof(_value); }
	T const *operator->() const { return std::addressof(_value); }

	operator bool() const { return _hasValue; }
	bool has_value() const { return _hasValue; }

	T &operator*() & { ASSERT(_hasValue); return _value; }
	T &&operator*() && { ASSERT(_hasValue); return _value; }
	T const &operator*() const & { ASSERT(_hasValue); return move(_value); }
	T const &&operator*() const && { ASSERT(_hasValue); return move(_value); }

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

}
