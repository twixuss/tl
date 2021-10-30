#pragma once
#include "common.h"

#pragma warning(push)
#pragma warning(disable : 4582 4624)

namespace tl {

template <class T>
struct List {
	List() = default;
	List(std::initializer_list<T> list) {
		reserve(list.size());
		size = list.size();
		memcpy(data, list.begin(), list.size() * sizeof(T));
	}
	void set(Span<T> span) {
		reserve(span.size);
		size = span.size;
		memcpy(data, span.data, span.size * sizeof(T));
	}
	T &add() {
		reserve_exponential(size + 1);
		return *new (data + size++) T();
	}
	T &add(T value) {
		reserve_exponential(size + 1);
		auto dest = data + size;
		assert(dest == (data + size));
		memcpy(dest, &value, sizeof(T));
		return data[size++];
	}
	Span<T> add(Span<T> span) {
		reserve_exponential(size + span.size);
		memcpy(data + size, span.data, span.size * sizeof(T));
		size += span.size;
		return {data + size - span.size, span.size};
	}
	Span<T> add(std::initializer_list<T> list) {
		reserve_exponential(size + list.size());
		memcpy(data + size, list.begin(), list.size() * sizeof(T));
		size += list.size();
		return {data + size - list.size(), list.size()};
	}

	T &add_front(T value) {
		reserve_exponential(size + 1);
		for (umm i = size; i > 0; --i) {
			data[i] = data[i - 1];
		}
		data[0] = value;
		return data[0];
	}

	void reallocate(umm desired_capacity) {
		T *new_data;
		if (data) {
			new_data = allocator.reallocate_uninitialized<T>(data, size, desired_capacity);
		} else {
			new_data = allocator.allocate_uninitialized<T>(desired_capacity);
		}
		data = new_data;
		capacity = desired_capacity;
	}

	void reserve(umm desired_capacity) {
		if (capacity >= desired_capacity) return;

		reallocate(desired_capacity);
	}
	void reserve_exponential(umm desired_capacity) {
		if (capacity >= desired_capacity) return;

		umm new_capacity = max(1u, capacity);
		while (new_capacity < desired_capacity) new_capacity *= 2;

		reallocate(new_capacity);
	}
	void resize(umm new_size) {
		reserve(new_size);

		if (new_size > size) {
			for (umm i = size; i < new_size; ++i) {
				new (data + i) T();
			}
		}
		size = new_size;
	}

	void clear() {
		size = 0;
	}

	T pop() {
		--size;
		return data[size];
	}

	T &operator[](umm i) const {
		bounds_check(i < size);
		return data[i];
	}

	T *begin() const { return data; }
	T *end() const { return data + size; }

	struct ReverseIterator {
		T *pointer;
		ReverseIterator(T *pointer) : pointer(pointer) {}
		ReverseIterator &operator++() { return --pointer, *this; }
		ReverseIterator operator++(int) { auto temp = *this; return --pointer, temp; }
		bool operator==(ReverseIterator that) const { return pointer == that.pointer; }
		bool operator!=(ReverseIterator that) const { return pointer != that.pointer; }
		T &operator*() { return *pointer; }
		T *operator->() { return pointer; }
	};

	ReverseIterator rbegin() const { return data + size - 1; }
	ReverseIterator rend() const { return data - 1; }

	T &front() const { bounds_check(size); return data[0]; }
	T &back() const { bounds_check(size); return data[size - 1]; }

	bool empty() const { return size == 0; }

	operator Span<T>() const { return {data, size}; }

	template <class U>
	explicit operator List<U>() const {
		List<U> result;
		result.allocator = allocator;
		result.data = (U *)data;
		if constexpr (sizeof(T) == sizeof(U)) {
			result.size      = size;
			result.capacity  = capacity;
		} else {
			if constexpr (sizeof(T) > sizeof(U)) {
				static_assert(sizeof(T) % sizeof(U) == 0);
			} else {
				static_assert(sizeof(U) % sizeof(T) == 0);
			}
			result.size      = size * sizeof(T) / sizeof(U);
			result.capacity  = capacity * sizeof(T) / sizeof(U);
		}
		return result;
	}

	List &operator+=(T const &v) { add(v); return *this; }
	List &operator+=(Span<T> v) { add(v); return *this; }
	List &operator+=(List<T> const &v) { add(as_span(v)); return *this; }
	List &operator+=(std::initializer_list<T> v) { add(Span((T *)v.begin(), (T *)v.end())); return *this; }

	T &insert_at(T value, umm where) {
		bounds_check(where <= size);

		reserve_exponential(size + 1);

		memmove(data + where + 1, data + where, (size - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++size;
		return data[where];
	}
	Span<T> insert_at(Span<T> span, umm where) {
		bounds_check(where <= size);

		reserve_exponential(size + span.size);

		memmove(data + where + span.size, data + where, (size - where) * sizeof(T));
		memcpy(data + where, span.data, span.size * sizeof(T));

		size += span.size;
		return {data + where, span.size};
	}
	T &insert(T value, T *where) { return insert_at(value, where - data); }
	Span<T> insert(Span<T> span, T *where) { return insert_at(span, where - data); }

	void erase(Span<T> where) {
		bounds_check(
			where.size <= size &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data, where.data + where.size, size - where.size + data - where.data);
		size -= where.size;
	}
	void erase_at(umm where) {
		bounds_check(where < size);
		--size;
		for (umm i = where; i < size; ++i) {
			data[i] = data[i + 1];
		}
	}

	void replace(Span<T> where, T with_what) {
		bounds_check(
			where.size <= size &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data + 1, where.data + where.size, end() - where.end());
		*where.data = with_what;

		size -= where.size - 1;
	}

	void replace(Span<T> where, Span<T> with_what) {
		assert(begin() <= where.begin());
		bounds_check(
			where.size <= size &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		T *old_data = data;
		reserve_exponential(size - where.size + with_what.size);
		where.data += data - old_data;

		memmove(
			where.data + with_what.size,
			where.data + where.size,
			(end() - where.end()) * sizeof(T)
		);
		memcpy(where.data, with_what.data, with_what.size * sizeof(T));

		size -= where.size - with_what.size;
	}

	bool operator==(List that) const {
		if (size != that.size)
			return false;

		for (umm i = 0; i < size; ++i) {
			if (data[i] != that.data[i])
				return false;
		}

		return true;
	}
	bool operator!=(List that) const { return !(*this == that); }

	bool operator==(Span<T> that) const {
		if (size != that.size)
			return false;

		for (umm i = 0; i < size; ++i) {
			if (data[i] != that.data[i])
				return false;
		}

		return true;
	}
	bool operator!=(Span<T> that) const { return !(*this == that); }

	T *data = 0;
	umm size = 0;
	umm capacity = 0;
	Allocator allocator = current_allocator;
};

template <class T>
void erase(List<T> &list, T *value) { list.erase_at(value - list.data); }

template <class T>
void erase_unordered_at(List<T> &list, umm index) {
	bounds_check(index < list.size);
	memcpy(list.data + index, &list.back(), sizeof(T));
	--list.size;
}

template <class T>
void free(List<T> &list) {
	if (list.data == 0) return;

	list.allocator.free(list.data);
	list.data = 0;
	list.size = 0;
	list.capacity = 0;
}

template <class T>
List<T> copy(List<T> that) {
	List<T> result;
	result.size = that.size;
	result.capacity = result.size;
	result.data = result.allocator.allocate<T>(result.size);
	memcpy(result.data, that.data, result.size * sizeof(T));
	return result;
}

template <class T>
List<T> as_list(Span<T> that) {
	List<T> result;
	result.size = that.size;
	result.capacity = result.size;
	result.data = result.allocator.allocate<T>(result.size);
	memcpy(result.data, that.data, result.size * sizeof(T));
	return result;
}

//template <class T>
//List<T> as_list(Span<T> span) {
//	List<T> result;
//	result.reserve(span.size);
//	result.size = span.size;
//	memcpy(result.data, span.data, result.size * sizeof(T));
//	return result;
//}

template <class T>
T *next(List<T> list, T *value) {
	return ++value == list.end() ? 0 : value;
}

template <class T>
T *previous(List<T> list, T *value) {
	return value-- == list.data ? 0 : value;
}

template <class T> constexpr T *find(List<T> list, T const &value) { return find(as_span(list), value); }
template <class T> constexpr T *find(List<T> list, Span<T> cmp) { return find(as_span(list), cmp); }
template <class T> constexpr T *find_last(List<T> list, T const &value) { return find_last(as_span(list), value); }

template <class T>
struct BadList {
	BadList() = default;
	explicit BadList(umm length) { resize(length); }
	BadList(Span<T> span) {
		reserve(span.size);
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size;
	}
	BadList(T const *begin, umm length) : BadList(Span(begin, length)) {}
	BadList(BadList &&that) {
		_begin = that._begin;
		_end = that._end;
		_allocEnd = that._allocEnd;
		that._begin = 0;
		that._end = 0;
		that._allocEnd = 0;
	}
	BadList(BadList const &that) {
		_begin = allocator.allocate<T>(that.size());
		_allocEnd = _end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
	}
	BadList(std::initializer_list<T> v) : BadList(Span(v.begin(), v.end())) {}
	~BadList() {
		clear();
		if (_begin)
			allocator.free(_begin);
		_begin = 0;
		_end = 0;
		_allocEnd = 0;
	}
	BadList &set(Span<T> span) {
		clear();
		if (span.size > capacity()) {
			_reallocate(span.size);
		}
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size;
		return *this;
	}
	BadList &operator=(BadList const &that) {
		clear();
		reserve(that.size());
		_end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
		return *this;
	}
	BadList &operator=(BadList &&that) {
		this->~BadList();
		return *new(this) BadList(std::move(that));
	}
	BadList &operator=(std::initializer_list<T> v) {
		this->~BadList();
		return *new(this) BadList(Span(v.begin(), v.end()));
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
	T &front() { bounds_check(size()); return *_begin; }
	T &back() { bounds_check(size()); return _end[-1]; }
	T &operator[](umm i) { bounds_check(size()); return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { bounds_check(size()); return *_begin; }
	T const &back() const { bounds_check(size()); return _end[-1]; }
	T const &operator[](umm i) const { bounds_check(size()); return _begin[i]; }

	void pop_back() { bounds_check(size()); (--_end)->~T(); }
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

	T *insert(Span<T> span, T *where) {
		bounds_check(_begin <= where && where <= _end);

		umm where_index = where - _begin;
		umm required_size = size() + span.size;
		_grow_if_needed(required_size);
		where = _begin + where_index;

		for (auto src = where; src != _end; ++src) {
			new (src + span.size) T(std::move(*src));
		}
		for (umm i = 0; i < span.size; ++i) {
			where[i] = span[i];
		}
		_end += span.size;
		return where;
	}

	operator Span<T>() { return {begin(), end()}; }
	operator Span<T>() const { return {begin(), end()}; }

	void _reallocate(umm newCapacity) {
		assert(capacity() < newCapacity);
		umm oldSize = size();
		T *newBegin = allocator.allocate<T>(newCapacity);
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			allocator.free(_begin);
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
	T &emplace_back(Args &&...args) {
		_grow_if_needed(size() + 1);
		return *new ((void *)_end++) T(std::forward<Args>(args)...);
	}
	T &push_back(T const &value) { return emplace_back(value); }
	T &push_back(T &&value) { return emplace_back(std::move(value)); }
	T &add(T const &value) { return emplace_back(value); }
	T &add(T &&value) { return emplace_back(std::move(value)); }

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
		bounds_check(this->_begin <= val && val < this->_end);
		val->~T();
		--this->_end;
		for (T *dest = val; dest != this->_end; ++dest) {
			*dest = std::move(dest[1]);
		}
		this->_end->~T();
	}
	void erase(T &val) { erase(std::addressof(val)); }

	void erase_unordered(T *val) {
		bounds_check(this->_begin <= val && val < this->_end);
		val->~T();
		new (val) T(std::move(*(this->_end-- - 1)));
	}
	void erase_unordered(T &val) { erase_unordered(std::addressof(val)); }

	BadList &operator+=(T const &v) { this->push_back(v); return *this; }
	BadList &operator+=(T &&v) { this->push_back(std::move(v)); return *this; }
	BadList &operator+=(Span<T> v) { this->insert(v, this->end()); return *this; }
	BadList &operator+=(BadList<T> const &v) { this->insert(as_span(v), this->end()); return *this; }
	BadList &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }

	void free() {
		this->~List();
	}

	Allocator allocator = current_allocator;
	T *_begin = 0;
	T *_end = 0;
	T *_allocEnd = 0;
};

template <class T>
Span<T> as_span(List<T> const &list) {
	return (Span<T>)list;
}

template <class T, class Predicate>
constexpr T *find_if(List<T> &list, Predicate &&predicate) {
	return find_if(as_span(list), std::forward<Predicate>(predicate));
}

template <class T, class Predicate>
constexpr T const *find_if(List<T> const &list, Predicate &&predicate) {
	return find_if(as_span(list), std::forward<Predicate>(predicate));
}

template <class T, class Fn>
constexpr void for_each(List<T> list, Fn &&fn) {
	using ReturnType = decltype(fn(*(T*)0));
	constexpr bool breakable = is_same<bool, ReturnType>;
	for (auto &it : list) {
		if constexpr (breakable) {
			if (fn(it))
				return;
		} else {
			fn(it);
		}
	}
}

template <class T>
struct Queue {
	Queue() = default;
	Queue(Queue const &that) = default;
	Queue(Queue &&that) = delete;
	Queue &operator=(Queue const &that) = default;
	Queue &operator=(Queue &&that) = delete;

	T *begin() { return data; }
	T *end() { return data + size; }
	T const *begin() const { return data; }
	T const *end() const { return data + size; }

	umm space_back() const { return (umm)((alloc_data + alloc_size) - (data + size)); }

	bool empty() const { return size == 0; }

	T &front() { bounds_check(size); return data[0]; }
	T &back() { bounds_check(size); return data[size - 1]; }
	T &operator[](umm i) { bounds_check(size); return data[i]; }

	void push(T const &value) {
		_grow_if_needed(size + 1);
		data[size++] = value;
	}

	void push_front_unordered(T const &value) {
		_grow_if_needed(size + 1);
		if (size != 0) {
			data[size++] = data[0];
		}
		data[0] = value;
	}

	void pop() {
		bounds_check(size);
		++data;
		--size;
	}
	//void resize(umm new_size) {
	//	if (new_size > capacity())
	//		_reallocate(new_size);
	//	if (new_size > size()) {
	//		for (T *t = _end; t < _begin + new_size; ++t)
	//			new (t) T();
	//		_end = _begin + new_size;
	//	} else if (new_size < size()) {
	//		for (T *t = _begin + new_size; t < _end; ++t)
	//			t->~T();
	//		_end = _begin + new_size;
	//	}
	//}
	void clear() {
		data = alloc_data;
		size = 0;
	}

	operator Span<T>() { return {data, size}; }

	void _reallocate(umm new_capacity) {
	}

	void erase_at(umm where) {
		assert(where < size, "value is not in container");

		umm left_count = where;
		umm right_count = size - where - 1;

		--size;

		if (left_count <= right_count) {
			// move left half one to the right
			for (umm i = where; i != 0; --i) {
				data[i] = data[i - 1];
			}
			++data;
		} else {
			// move right half one to the left
			for (umm i = where; i < size; ++i) {
				data[i] = data[i + 1];
			}
		}
	}
	void erase(T *val) { erase_at(val - data); }
	void erase(T &val) { erase(&val); }

	void _grow_if_needed(umm required_size) {
		if (required_size <= space_back())
			return;
		umm new_capacity = space_back();
		if (new_capacity == 0)
			new_capacity = 1;
		while (new_capacity < required_size) {
			new_capacity *= 2;
		}

		T *new_data = allocator.allocate<T>(new_capacity);
		for (umm i = 0; i < size; ++i) {
			new_data[i] = data[i];
		}
		if (alloc_data)
			allocator.free(alloc_data);

		alloc_data = new_data;
		data = new_data;
		alloc_size = new_capacity;
	}
	T *insert(Span<T const> span, umm where) {
		bounds_check(where <= size);

		_grow_if_needed(size + span.size);

		for (umm i = where; i < size; ++i) {
			data[span.size + i] = data[i];
		}
		for (umm i = 0; i < span.size; ++i) {
			data[where + i] = span[i];
		}
		size += span.size;
		return data + where;
	}
	T *insert(Span<T const> span, T *where) {
		return insert(span, where - data);
	}

	Queue &operator+=(T const &v) { push(v); return *this; }
	Queue &operator+=(Span<T const> v) { insert(v, end()); return *this; }
	Queue &operator+=(Queue<T> const &v) { insert(as_span(v), end()); return *this; }
	Queue &operator+=(std::initializer_list<T> v) { insert(Span(v.begin(), v.end()), this->end()); return *this; }

	Allocator allocator = current_allocator;
	T *data = 0;
	umm size = 0;
	T *alloc_data = 0;
	umm alloc_size = 0;
};

template <class T>
void free(Queue<T> &queue) {
	if (queue.alloc_data == 0) return;

	queue.allocator.free(queue.alloc_data);
	queue.data = 0;
	queue.size = 0;
	queue.alloc_data = 0;
	queue.alloc_size = 0;
}

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
		bounds_check(_size != _capacity);
		new (&_storage[(_begin + _size++) % _capacity].value) T(std::forward<Args>(args)...);
	}
	template <class ...Args>
	void emplace_front(Args &&...args) {
		bounds_check(_size != _capacity);
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

	T &back() { bounds_check(_size); return _get(_size - 1); }
	T &front() { bounds_check(_size); return _get(0); }

	umm size() const { return _size; }
	bool empty() const { return _size == 0; }
	bool full() const { return _size == _capacity; }

	T &operator[](umm i) { bounds_check(i < _size); return _get(i); }
	T const &operator[](umm i) const { bounds_check(i < _size); return _get(i); }

	void erase(Iterator it) {
		umm index = it.index;
		bounds_check(index < _size);
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

#if 0

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

	T &back() { bounds_check(_size); return _get(_size - 1); }
	T &front() { bounds_check(_size); return _get(0); }

	umm capacity() const { return (umm)(_allocEnd - _allocBegin); }
	umm size() const { return _size; }
	bool empty() const { return _size == 0; }
	bool full() const { return _size == capacity(); }

	T &operator[](umm i) { bounds_check(i < _size); return _get(i); }
	T const &operator[](umm i) const { bounds_check(i < _size); return _get(i); }

	void erase(Iterator it) {
		umm index = it.index;
		bounds_check(index < _size);
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

#endif

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

template <class T>
struct LinearSet : private List<T> {
	using Base = List<T>;
	using Base::begin;
	using Base::end;
	using Base::clear;

	T &insert(T const &value) {
		for (auto &it : *this) {
			if (it == value) {
				return it;
			}
		}
		return this->add(value);
	}
};

template <class T>
void free(LinearSet<T> &set) {
	free((List<T> &)set);
}

// Returns an index of the value
// If value is not in the list, this function will return index >= list.size
template <class T>
umm index_of(List<T> const &list, T const *value) {
	return value - list.data;
}

template <class T> umm count_of(List<T> const &list) { return list.size(); }

template <class T> T const &front(List<T> const &list) { return list.front(); }
template <class T> T &front(List<T> &list) { return list.front(); }

template <class T> T const &back(List<T> const &list) { return list.back(); }
template <class T> T &back(List<T> &list) { return list.back(); }

#pragma warning(pop)

}
