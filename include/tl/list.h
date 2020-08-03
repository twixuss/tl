#pragma once
#include "common.h"
#include <string.h>

#pragma warning(push)
#pragma warning(disable : 4582)

namespace TL {

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
		if (span.size() > capacity()) {
			_reallocate(span.size());
		}
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
			for (T *t = _end; t < _begin + newSize; ++t)
				new (t) T();
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
	void _grow(umm requiredSize) {
		umm newCapacity = capacity();
		if (newCapacity == 0)
			newCapacity = 1;
		while (newCapacity < requiredSize) {
			newCapacity *= 2;
		}
		_reallocate(newCapacity);
	}
	template <class... Args>
	void emplace_back(Args &&... args) {
		if (remainingCapacity() == 0) {
			_grow(size() + 1);
		}
		new ((void *)_end++) T(std::forward<Args>(args)...);
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
