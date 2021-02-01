#pragma once
#include "common.h"
#include <string.h>

#pragma warning(push)
#pragma warning(disable : 4582 4624)

namespace TL {

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct List {
	List() = default;
	explicit List(umm length) { resize(length); }
	List(Span<T const> span) {
		reserve(span.size());
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size();
	}
	List(T const *begin, umm length) : List(Span(begin, length)) {}
	List(List &&that) {
		_begin = that._begin;
		_end = that._end;
		_allocEnd = that._allocEnd;
		that._begin = 0;
		that._end = 0;
		that._allocEnd = 0;
	}
	List(List const &that) {
		_begin = ALLOCATE_T(Allocator, T, that.size(), 0);
		_allocEnd = _end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
	}
	List(std::initializer_list<T> v) : List(Span(v.begin(), v.end())) {}
	~List() {
		clear();
		if (_begin)
			DEALLOCATE(Allocator, _begin);
		_begin = 0;
		_end = 0;
		_allocEnd = 0;
	}
	List &set(Span<T const> span) {
		clear();
		if (span.size() > capacity()) {
			_reallocate(span.size());
		}
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size();
		return *this;
	}
	List &operator=(List const &that) {
		clear();
		reserve(that.size());
		_end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
		return *this;
	}
	List &operator=(List &&that) {
		this->~List();
		return *new(this) List(std::move(that));
	}
	List &operator=(std::initializer_list<T> v) { 
		this->~List();
		return *new(this) List(Span(v.begin(), v.end())); 
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
	T &front() { TL_BOUNDS_CHECK(size()); return *_begin; }
	T &back() { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T &operator[](umm i) { TL_BOUNDS_CHECK(size()); return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { TL_BOUNDS_CHECK(size()); return *_begin; }
	T const &back() const { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(size()); return _begin[i]; }

	void pop_back() { TL_BOUNDS_CHECK(size()); (--_end)->~T(); }
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
	
	T *insert(Span<T const> span, T *where) {
		TL_BOUNDS_CHECK(_begin <= where && where <= _end);

		umm where_index = where - _begin;
		umm required_size = size() + span.size();
		_grow_if_needed(required_size);
		where = _begin + where_index; 
		
		for (auto src = where; src != _end; ++src) {
			new (src + span.size()) T(std::move(*src));
		}
		for (umm i = 0; i < span.size(); ++i) {
			where[i] = span[i];
		}
		_end += span.size();
		return where;
	}

	operator Span<T>() { return {begin(), end()}; }
	operator Span<T const>() const { return {begin(), end()}; }

	T *_begin = 0;
	T *_end = 0;
	T *_allocEnd = 0;

	void _reallocate(umm newCapacity) {
		assert(capacity() < newCapacity);
		umm oldSize = size();
		T *newBegin = ALLOCATE_T(Allocator, T, newCapacity, 0);
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			DEALLOCATE(Allocator, _begin);
		_begin = newBegin;
		_end = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	bool _grow_if_needed(umm requiredSize) {
		if (requiredSize <= capacity())
			return false;
		umm newCapacity = capacity();
		if (newCapacity == 0)
			newCapacity = 1;
		while (newCapacity < requiredSize) {
			newCapacity *= 2;
		}
		_reallocate(newCapacity);
		return true;
	}
	template <class... Args>
	T &push_back(Args &&...args) {
		_grow_if_needed(size() + 1);
		return *new ((void *)_end++) T(std::forward<Args>(args)...);
	}

	template <class... Args>
	T &push_front(Args &&... args) {
		if (this->remainingCapacity() == 0) {
			umm newCapacity = this->capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			this->_reallocate(newCapacity);
		}
		new (this->_end) T(std::move(this->_end[-1]));
		for (T *dest = this->_end - 1; dest > this->_begin; --dest) {
			*dest = std::move(dest[-1]);
		}
		++this->_end;
		return *new (this->_begin) T(std::forward<Args>(args)...);
	}
	template <class... Args>
	void push_front_unordered(Args &&... args) {
		if (this->remainingCapacity() == 0) {
			umm newCapacity = this->capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			this->_reallocate(newCapacity);
		}
		new (this->_end) T(std::move(*this->_begin));
		++this->_end;
		new (this->_begin) T(std::forward<Args>(args)...);
	}
	void erase(T *val) {
		TL_BOUNDS_CHECK(this->_begin <= val && val < this->_end);
		val->~T();
		--this->_end;
		for (T *dest = val; dest != this->_end; ++dest) {
			*dest = std::move(dest[1]);
		}
		this->_end->~T();
	}
	void erase(T &val) { erase(std::addressof(val)); }

	void erase_unordered(T *val) {
		TL_BOUNDS_CHECK(this->_begin <= val && val < this->_end);
		val->~T();
		new (val) T(std::move(*(this->_end-- - 1)));
	}
	void erase_unordered(T &val) { erase_unordered(std::addressof(val)); }
	
	List &operator+=(T const &v) { this->push_back(v); return *this; }
	List &operator+=(T &&v) { this->push_back(std::move(v)); return *this; }
	List &operator+=(Span<T const> v) { this->insert(v, this->end()); return *this; }
	List &operator+=(List<T> const &v) { this->insert(as_span(v), this->end()); return *this; }
	List &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }

	void free() {
		this->~List();
	}
};

template <class T, class Allocator>
Span<T> as_span(List<T, Allocator> &list) {
	return (Span<T>)list;
}

template <class T, class Allocator>
Span<T const> as_span(List<T, Allocator> const &list) {
	return (Span<T const>)list;
}

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct Queue {
	Queue() = default;
	explicit Queue(umm size) {
		resize(size);
	}
	Queue(Span<T const> span) {
		_reallocate(span.size());
		for (auto &src : span) {
			push(src);
		}
	}
	Queue(Queue const &that) : Queue((Span<T const>)that) {}
	Queue(Queue &&that) {
		_allocBegin = that._allocBegin;
		_allocEnd = that._allocEnd;
		_begin = that._begin;
		_end = that._end;
		that._allocBegin = 0;
		that._allocEnd = 0;
		that._begin = 0;
		that._end = 0;
	}
	Queue &operator=(Queue const &that) { this->~Queue(); return *new (this) Queue(that); }
	Queue &operator=(Queue &&that) { this->~Queue(); return *new (this) Queue(std::move(that)); }
	~Queue() {
		clear();
		if (_allocBegin)
			DEALLOCATE(Allocator, _allocBegin);
		_begin = 0;
		_end = 0;
		_allocBegin = 0;
		_allocEnd = 0;
	}

	T *begin() { return _begin; }
	T *end() { return _end; }
	T const *begin() const { return _begin; }
	T const *end() const { return _end; }

	umm space_back() const { return (umm)(_allocEnd - _end); }

	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return _begin; }
	T &front() { TL_BOUNDS_CHECK(size()); return *_begin; }
	T &back() { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T &operator[](umm i) { TL_BOUNDS_CHECK(size()); return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { TL_BOUNDS_CHECK(size()); return *_begin; }
	T const &back() const { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(size()); return _begin[i]; }
	
	template <class... Args>
	void push(Args &&...args) {
		if (space_back() == 0) {
			_grow(size() + 1);
		}
		new (_end++) T(std::forward<Args>(args)...);
	}
	template <class... Args>
	void push_front_unordered(Args &&...args) {
		if (space_back() == 0) {
			_grow(size() + 1);
		}
		new (_end++) T(std::move(*_begin));
		new (_begin) T(std::forward<Args>(args)...);
	}

	void pop() { 
		TL_BOUNDS_CHECK(size());
		_begin++->~T();
	}
	void resize(umm newSize) {
		if (newSize > this->capacity())
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

	T *_begin = 0;
	T *_end = 0;
	T *_allocBegin = 0;
	T *_allocEnd = 0;
	void _reallocate(umm newCapacity) {
		umm oldSize = size();
		T *newBegin = ALLOCATE_T(Allocator, T, newCapacity, 0);
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			DEALLOCATE(Allocator, _allocBegin);
		_allocBegin = newBegin;
		_begin = newBegin;
		_end = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	void _grow(umm requiredSize) {
		umm newCapacity = (umm)(_allocEnd - _allocBegin);
		if (newCapacity == 0)
			newCapacity = 1;
		while (newCapacity < requiredSize) {
			newCapacity *= 2;
		}
		_reallocate(newCapacity);
	}

	void erase(T *val) {
		assert(this->_begin <= val && val < this->_end, "value is not in container");
		val->~T();

		umm leftCount = val - _begin;
		umm rightCount = _end - val - 1;

		if (leftCount <= rightCount) {
			// move left part +1
			for (T *dest = val; dest != this->_begin; --dest) {
				*dest = std::move(dest[-1]);
			}
			(this->_begin++)->~T();
		} else {
			// move right part -1
			--this->_end;
			for (T *dest = val; dest != this->_end; ++dest) {
				*dest = std::move(dest[1]);
			}
			this->_end->~T();
		}
	}
	void erase(T &val) { erase(std::addressof(val)); }
	
	bool _grow_if_needed(umm requiredSize) {
		if (requiredSize <= space_back())
			return false;
		umm newCapacity = space_back();
		if (newCapacity == 0)
			newCapacity = 1;
		while (newCapacity < requiredSize) {
			newCapacity *= 2;
		}
		_reallocate(newCapacity);
		return true;
	}
	T *insert(Span<T const> span, T *where) {
		TL_BOUNDS_CHECK(_begin <= where && where <= _end);

		umm where_index = where - _begin;
		umm required_size = size() + span.size();
		_grow_if_needed(required_size);
		where = _begin + where_index; 
		
		for (auto src = where; src != _end; ++src) {
			new (src + span.size()) T(std::move(*src));
		}
		for (umm i = 0; i < span.size(); ++i) {
			where[i] = span[i];
		}
		_end += span.size();
		return where;
	}

	Queue &operator+=(T const &v) { this->push(v); return *this; }
	Queue &operator+=(T &&v) { this->push(std::move(v)); return *this; }
	Queue &operator+=(Span<T const> v) { this->insert(v, this->end()); return *this; }
	Queue &operator+=(Queue<T> const &v) { this->insert(as_span(v), this->end()); return *this; }
	Queue &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }
};

template <class T, umm _capacity>
struct StaticCircularBuffer {
	using Storage = Storage<T>;
	struct Iterator {
		using value_type = T;
		StaticCircularBuffer *buffer;
		umm index;
		Iterator(StaticCircularBuffer *buffer, umm index) : buffer(buffer), index(index) {}
		T &operator*() { return buffer->_get(index); }
		T &operator*() const { return buffer->_get(index); }
		Iterator operator+(smm v) const { return Iterator(buffer, index + v); }
		Iterator operator-(smm v) const { return Iterator(buffer, index - v); }
		smm operator-(Iterator v) const { return index - v.index; }
		Iterator &operator++() { ++index; return *this; }
		Iterator &operator--() { --index; return *this; }
		Iterator operator++(int) { Iterator result = *this; ++*this; return result; }
		Iterator operator--(int) { Iterator result = *this; --*this; return result; }
		bool operator>(Iterator const &that) const { return index > that.index; }
		bool operator<(Iterator const &that) const { return index < that.index; }
		bool operator>=(Iterator const &that) const { return index >= that.index; }
		bool operator<=(Iterator const &that) const { return index <= that.index; }
		bool operator==(Iterator const &that) const { return index == that.index; }
		bool operator!=(Iterator const &that) const { return index != that.index; }
	};
	
	StaticCircularBuffer() = default;
	StaticCircularBuffer(StaticCircularBuffer const &that) {
		_size = that._size;
		umm dstIndex = that._begin;
		for (umm srcIndex = 0; srcIndex < _size; ++srcIndex) {
			new (&_storage[srcIndex].value) T(that._storage[dstIndex].value);
			if (++dstIndex == _capacity)
				dstIndex = 0;
		}
	}
	StaticCircularBuffer(StaticCircularBuffer &&that) {
		_size = that._size;
		umm dstIndex = that._begin;
		for (umm srcIndex = 0; srcIndex < _size; ++srcIndex) {
			new (&_storage[srcIndex].value) T(std::move(that._storage[dstIndex].value));
			if (++dstIndex == _capacity)
				dstIndex = 0;
		}
		that._begin = 0;
		that._size = 0;
	}
	StaticCircularBuffer &operator=(StaticCircularBuffer const &that) { clear(); return *new (this) StaticCircularBuffer(that); }
	StaticCircularBuffer &operator=(StaticCircularBuffer &&that) { clear(); return *new (this) StaticCircularBuffer(std::move(that)); }
	~StaticCircularBuffer() {
		for (umm i = 0; i < _size; ++i) {
			_get(i).~T();
		}
		_begin = 0;
		_size = 0;
	}
	template <class ...Args>
	void emplace_back(Args &&...args) {
		TL_BOUNDS_CHECK(_size != _capacity);
		new (&_storage[(_begin + _size++) % _capacity].value) T(std::forward<Args>(args)...);
	}
	template <class ...Args>
	void emplace_front(Args &&...args) {
		TL_BOUNDS_CHECK(_size != _capacity);
		++_size;
		if (_begin) --_begin; else _begin = _capacity - 1;
		new (&_storage[_begin].value) T(std::forward<Args>(args)...);
	}
	void push_back(T const &v) { emplace_back(v); }
	void push_back(T &&v) { emplace_back(std::move(v)); }
	void push_front(T const &v) { emplace_front(v); }
	void push_front(T &&v) { emplace_front(std::move(v)); }

	void pop_back() { _get(--_size).~T(); }
	void pop_front() { _storage[_begin].value.~T(); _incBegin(); --_size; }
	
	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, _size); }

	T &back() { TL_BOUNDS_CHECK(_size); return _get(_size - 1); }
	T &front() { TL_BOUNDS_CHECK(_size); return _get(0); }

	umm size() const { return _size; }
	bool empty() const { return _size == 0; }
	bool full() const { return _size == _capacity; }

	T &operator[](umm i) { TL_BOUNDS_CHECK(i < _size); return _get(i); }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(i < _size); return _get(i); }

	void erase(Iterator it) {
		umm index = it.index;
		TL_BOUNDS_CHECK(index < _size);
		--_size;
		if (index > _size / 2) {
			for (;index < _size; ++index) {
				_get(index) = std::move(_get(index + 1));
			}
			_get(_size).~T();
		} else {
			for (;index; --index) {
				_get(index) = std::move(_get(index - 1));
			}
			_get(0).~T();
			_incBegin();
		}
	}

	void clear() {
		for (umm i = 0; i < size(); ++i) {
			_get(i).~T();
		}
		_begin = 0;
		_size = 0;
	}

	T &_get(umm i) {
		return _storage[(_begin + i) % _capacity].value;
	}
	void _incBegin() {
		++_begin;
		if (_begin == _capacity) 
			_begin = 0;
	}

	Storage _storage[_capacity];
	umm _begin = 0;
	umm _size = 0;
};

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct CircularBuffer {
	struct Iterator {
		using value_type = T;
		CircularBuffer *buffer;
		umm index;
		Iterator(CircularBuffer *buffer, umm index) : buffer(buffer), index(index) {}
		T &operator*() { return buffer->_get(index); }
		T &operator*() const { return buffer->_get(index); }
		Iterator operator+(smm v) const { return Iterator(buffer, index + v); }
		Iterator operator-(smm v) const { return Iterator(buffer, index - v); }
		smm operator-(Iterator v) const { return index - v.index; }
		Iterator &operator++() { ++index; return *this; }
		Iterator &operator--() { --index; return *this; }
		Iterator operator++(int) { Iterator result = *this; ++*this; return result; }
		Iterator operator--(int) { Iterator result = *this; --*this; return result; }
		bool operator>(Iterator const &that) const { return index > that.index; }
		bool operator<(Iterator const &that) const { return index < that.index; }
		bool operator>=(Iterator const &that) const { return index >= that.index; }
		bool operator<=(Iterator const &that) const { return index <= that.index; }
		bool operator==(Iterator const &that) const { return index == that.index; }
		bool operator!=(Iterator const &that) const { return index != that.index; }
	};
	
	CircularBuffer() = default;
	explicit CircularBuffer(umm capacity) {
		_allocBegin = ALLOCATE_T(Allocator, T, capacity, 0);
		_allocEnd = _allocBegin + capacity;
	}
	CircularBuffer(CircularBuffer const &that) {
		_size = that._size;
		_allocBegin = ALLOCATE_T(Allocator, T, _size, 0);
		_allocEnd = _allocBegin + _size;

		umm dstIndex = that._firstIndex;
		for (umm srcIndex = 0; srcIndex < _size; ++srcIndex) {
			new (&_allocBegin[srcIndex]) T(that._allocBegin[dstIndex]);
			if (++dstIndex == that.capacity())
				dstIndex = 0;
		}
	}
	CircularBuffer(CircularBuffer &&that) {
		_allocBegin = that._allocBegin;
		_allocEnd = that._allocEnd;
		_firstIndex = that._firstIndex;
		_size = that._size;

		that._allocBegin = 0;
		that._allocEnd = 0;
		that._firstIndex = 0;
		that._size = 0;
	}
	CircularBuffer &operator=(CircularBuffer const &that) { clear(); return *new (this) CircularBuffer(that); }
	CircularBuffer &operator=(CircularBuffer &&that) { clear(); return *new (this) CircularBuffer(std::move(that)); }
	~CircularBuffer() {
		if (!_allocBegin)
			return;

		clear();

		DEALLOCATE(Allocator, _allocBegin);

		_allocBegin = 0;
		_allocEnd = 0;
	}
	template <class ...Args>
	void emplace_back(Args &&...args) {
		_growIfFull();
		new (&_allocBegin[(_firstIndex + _size++) % capacity()]) T(std::forward<Args>(args)...);
	}
	template <class ...Args>
	void emplace_front(Args &&...args) {
		_growIfFull();
		++_size;
		if (_firstIndex) --_firstIndex; else _firstIndex = capacity() - 1;
		new (&_allocBegin[_firstIndex]) T(std::forward<Args>(args)...);
	}
	void push_back(T const &v) { emplace_back(v); }
	void push_back(T &&v) { emplace_back(std::move(v)); }
	void push_front(T const &v) { emplace_front(v); }
	void push_front(T &&v) { emplace_front(std::move(v)); }

	void pop_back() { _get(--_size).~T(); }
	void pop_front() { _allocBegin[_firstIndex].~T(); _incBegin(); --_size; }
	
	void reserve(umm count) {
		if (count <= capacity())
			return;
		
		T *newBegin = ALLOCATE_T(Allocator, T, count, 0);

		for (umm i = 0; i < _size; ++i) {
			new (&newBegin[i]) T(std::move(_get(i)));
		}

		DEALLOCATE(Allocator, _allocBegin);

		_firstIndex = 0;
		_allocBegin = newBegin;
		_allocEnd = _allocBegin + count;
	}

	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, _size); }

	T &back() { TL_BOUNDS_CHECK(_size); return _get(_size - 1); }
	T &front() { TL_BOUNDS_CHECK(_size); return _get(0); }

	umm capacity() const { return (umm)(_allocEnd - _allocBegin); }
	umm size() const { return _size; }
	bool empty() const { return _size == 0; }
	bool full() const { return _size == capacity(); }

	T &operator[](umm i) { TL_BOUNDS_CHECK(i < _size); return _get(i); }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(i < _size); return _get(i); }

	void erase(Iterator it) {
		umm index = it.index;
		TL_BOUNDS_CHECK(index < _size);
		--_size;
		if (index > _size / 2) {
			for (;index < _size; ++index) {
				_get(index) = std::move(_get(index + 1));
			}
			_get(_size).~T();
		} else {
			for (;index; --index) {
				_get(index) = std::move(_get(index - 1));
			}
			_get(0).~T();
			_incBegin();
		}
	}

	void clear() {
		for (umm i = 0; i < size(); ++i) {
			_get(i).~T();
		}
		_firstIndex = 0;
		_size = 0;
	}

	T &_get(umm i) {
		return _allocBegin[(_firstIndex + i) % capacity()];
	}
	void _incBegin() {
		++_firstIndex;
		if (_firstIndex == capacity())
			_firstIndex = 0;
	}
	void _growIfFull() {
		if (full())
			reserve(max(capacity() * 2, (umm)1));
	}

	T *_allocBegin = 0;
	T *_allocEnd = 0;
	umm _firstIndex = 0;
	umm _size = 0;
};

template <class T, umm _capacity>
struct StaticCircularQueue : private StaticCircularBuffer<T, _capacity> {
	using Base = StaticCircularBuffer<T, _capacity>;
	using Base::begin;
	using Base::end;
	using Base::size;
	using Base::empty;
	using Base::erase;
	using Base::front;
	using Base::clear;
	using Base::operator[];
	void push(T const &v) { this->push_back(v); }
	void push(T &&v) { this->push_back(std::move(v)); }
	void pop() { this->pop_front(); }
};

struct BlockListIndex {
	umm block_index;
	umm value_index;
};

template <class T, umm blockCapacity_, class Allocator = TL_DEFAULT_ALLOCATOR>
struct BlockList {
	static constexpr umm blockCapacity = blockCapacity_;
	using Storage = Storage<T>;
	struct Block {
		Storage buffer[blockCapacity];
		Storage *end;
		Block *next;
		Block *previous;

		Block() {}
		~Block() {}

		umm size() const { return (umm)(end - buffer); }
		umm availableSpace() const { return blockCapacity - size(); }
	};
	struct Index {
		Block *block;
		umm value_index;
	};

	Block first;
	Block *last = &first;
	Block *allocLast = &first;

	BlockList() {
		init_block(&first);
	}
	BlockList(BlockList const &that) {
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(that.first.buffer[i].value);
		}
		
		auto previous = &first;
		for (auto srcBlock = that.first.next; srcBlock != 0;) {
			if (srcBlock->size()) {
				last = allocLast = allocateBlock(previous);
				for (u32 i = 0; i < srcBlock->size(); ++i) {
					new (&last->buffer[i].value) T(srcBlock->buffer[i].value);
				}
				previous = last;
			}
		}
	}
	BlockList(BlockList &&that) {
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(std::move(that.first.buffer[i].value));
		}

		first.next = that.first.next;
		last = that.last;
		allocLast = that.allocLast;

		that.first.end = that.first.buffer;
		that.last = that.allocLast = &that.first;
	}
	BlockList &operator=(BlockList const &that) {
		clear();
		
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(that.first.buffer[i].value);
		}
		
		last = &first;
		for (auto srcBlock = that.first.next; srcBlock != 0;) {
			if (srcBlock->size()) {
				if (!last->next)
					last->next = allocLast = allocateBlock(last);
				last = last->next;
				for (u32 i = 0; i < srcBlock->size(); ++i) {
					new (&last->buffer[i].value) T(srcBlock->buffer[i].value);
				}
			}
		}

		return *this;
	}
	BlockList &operator=(BlockList &&that) {
		clear();
		return *new (this) BlockList(std::move(that));
	}
	~BlockList() {
		for (u32 i = 0; i < first.size(); ++i) {
			first.buffer[i].value.~T();
		}
		first.end = first.buffer;

		for (auto block = first.next; block != 0;) {
			for (u32 i = 0; i < block->size(); ++i) {
				block->buffer[i].value.~T();
			}
			auto next = block->next;
			DEALLOCATE(Allocator, block);
			block = next;
		}

		last = &first;
		allocLast = &first;
	}

	void clear() {
		for (u32 i = 0; i < first.size(); ++i) {
			first.buffer[i].value.~T();
		}
		first.end = first.buffer;

		for (auto block = first.next; block != 0; block = block->next) {
			for (u32 i = 0; i < block->size(); ++i) {
				block->buffer[i].value.~T();
			}
			block->end = block->buffer;
		}
		last = &first;
	}
	umm availableSpace() {
		umm space = 0;
		auto block = last;
		while (block) {
			space += block->availableSpace();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = availableSpace();
		while (space < amount) {
			allocLast = allocLast->next = allocateBlock(allocLast);
			space += blockCapacity;
		}
	}
	void ensureConsecutiveSpace(umm amount) {
		assert(amount <= blockCapacity, "reserving this amount of consecutive space is impossible");
		if (last->availableSpace() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocateBlock(last);
				last = allocLast = last->next;
			}
		}
	}
	void push_back(T const &value) {
		auto dstBlock = last;
		while (dstBlock && (dstBlock->availableSpace() == 0)) {
			dstBlock = dstBlock->next;
		}

		if (!dstBlock) {
			auto new_block = allocateBlock(allocLast);
			allocLast->next = new_block;
			dstBlock = last = allocLast = new_block;
		}
		new (dstBlock->end++) T(value);
	}
	umm size() const {
		umm totalSize = 0;
		for (auto block = &first; block != 0; block = block->next) {
			totalSize += block->size();
		}
		return totalSize;
	}
	bool empty() const { return (last == &first) && (first.size() == 0); }

	T &back() { return last->end[-1].value; }
	T const &back() const { return last->end[-1].value; }

	void pop_back() {
		if (last == &first)
			TL_BOUNDS_CHECK(first.size());

		(last->end--)[-1].value.~T();
		if (last->end == last->buffer && last != &first) {
			last = last->previous;
		}
	}

	template <class Fn>
	void for_each_block(Fn &&fn) const {
		auto block = &first;
		do {
			auto blockCapacity = block->size();
			if (blockCapacity) {
				fn(block->buffer, blockCapacity);
			}
			block = block->next;
		} while (block);
	}

	T &operator[](Index index) {
#ifndef TL_NO_BOUNDS_CHECK
		{
			auto block = &first;
			do {
				if (block == index.block) {
					goto check_value_index;
				}
				block = block->next;
			} while (block);
			TL_BOUNDS_CHECK(false);
		check_value_index:

			TL_BOUNDS_CHECK(index.value_index < block->size());
		}
#endif
		return index.block->buffer[index.value_index].value;
	}
	T &operator[](BlockListIndex index) {
		auto block = &first;
		while (index.block_index--) {
			block = block->next;
		}
		TL_BOUNDS_CHECK(block);
		TL_BOUNDS_CHECK(index.value_index < block->size());
		return block->buffer[index.value_index].value;
	}
	
	BlockList &operator+=(T const &v) { this->push_back(v); return *this; }
	BlockList &operator+=(T &&v) { this->push_back(std::move(v)); return *this; }
	BlockList &operator+=(Span<T const> v) { this->insert(v, this->end()); return *this; }
	BlockList &operator+=(BlockList const &v) { this->insert(as_span(v), this->end()); return *this; }
	BlockList &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }

private:
	void init_block(Block *block) {
		block->end = block->buffer;
		block->next = 0;
		block->previous = 0;
	}
	Block *allocateBlock(Block *previous) {
		Block *result = ALLOCATE_T(Allocator, Block, 1, 0);
		init_block(result);
		result->previous = previous;
		return result;
	}
};

template <class T, class Allocator, umm block_size, class Fn>
void for_each(BlockList<T, block_size, Allocator> &list, Fn &&fn) {
	auto block = &list.first;
	do {
		for (auto it = block->buffer; it != block->end; ++it) {
			using Ret = decltype(fn(*it));
			if constexpr (isSame<bool, Ret>) {
				if (fn(*it))
					return;
			} else {
				fn(*it);
			}
		}
		block = block->next;
	} while (block);
}

template <class T, class Allocator, umm block_size>
T *find(BlockList<T, block_size, Allocator> &list, T const &to_find, BlockListIndex *result_index = 0) {
	BlockListIndex index = {};
	auto block = &list.first;
	do {
		index.value_index = 0;
		for (auto it = block->buffer; it != block->end; ++it) {
			if (it->value == to_find) {
				if (result_index)
					*result_index = index;
				return &it->value;
			}
			++index.value_index;
		}
		block = block->next;
		++index.block_index;
	} while (block);

	return 0;
}

template <class T, class Allocator, umm block_size>
T *find(BlockList<T, block_size, Allocator> &list, T const &to_find, typename BlockList<T, block_size, Allocator>::Index *result_index = 0) {
	typename BlockList<T, block_size, Allocator>::Index index = {};
	auto block = &list.first;
	do {
		index.block = block;
		index.value_index = 0;
		auto blockCapacity = block->size();
		for (auto it = block->buffer; it != block->end; ++it) {
			if (it == to_find) {
				if (result_index)
					*result_index = index;
				return &it;
			}
			++index.value_index;
		}
		block = block->next;
	} while (block);

	return 0;
}

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct LinkedList {
	using ValueType = T;

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
			node = node->next;
			return prev;
		}
		bool operator==(Iterator const &that) const { return node == that.node; }
		bool operator!=(Iterator const &that) const { return node != that.node; }
		T *operator->() { return std::addressof(node->value); }
	};

	LinkedList() = default;
	~LinkedList() { clear(); }

	void clear() {
		Node *node = head;
		for (;;) {
			if (node == 0)
				break;
			defer { DEALLOCATE(Allocator, node); };
			node = node->next;
		}
		head = last = 0;
	}

	template <class ...Args>
	void emplace_back(Args &&...args) {
		if (head == 0) {
			head = last = ALLOCATE_T(Allocator, Node, 1, 0);
		} else {
			last->next = ALLOCATE_T(Allocator, Node, 1, 0);
			last = last->next;
		}
		last->next = 0;

		new (&last->value) T(std::forward<Args>(args)...);
	}
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }

	template <class ...Args>
	void emplace_front(Args &&...args) {
		if (head == 0) {
			head = last = ALLOCATE_T(Allocator, Node, 1, 0);
			head->next = 0;
		} else {
			auto prevHead = head;
			head = ALLOCATE_T(Allocator, Node, 1, 0);
			head->next = prevHead;
		}
		new (&head->value) T(std::forward<Args>(args)...);
	}
	void push_front(T const &val) { emplace_front(val); }
	void push_front(T &&val) { emplace_front(std::move(val)); }

	T &back() { return last->value; }
	T const &back() const { return last->value; }

	Iterator begin() { return head; }
	Iterator end() { return {}; }

private:
	Node *head = 0;
	Node *last = 0;
};

template <class T>
umm index_of(List<T> const &list, T const *value) {
	return value - list._begin;
}

template <class T, umm capacity>
umm index_of(StaticList<T, capacity> const &list, T const *value) {
	return value - list._begin;
}

template <class T> umm count_of(List<T> const &list) { return list.size(); }

template <class T> T const &front(List<T> const &list) { return list.front(); }
template <class T> T &front(List<T> &list) { return list.front(); }

template <class T> T const &back(List<T> const &list) { return list.back(); }
template <class T> T &back(List<T> &list) { return list.back(); }

#pragma warning(pop)

}
