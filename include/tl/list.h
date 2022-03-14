#pragma once
#include "common.h"

#pragma warning(push)
#pragma warning(disable : 4582 4624)

namespace tl {

template <class T>
struct List : Span<T> {
	using Span<T>::data;
	using Span<T>::count;
	using Span<T>::begin;
	using Span<T>::end;

	umm capacity = 0;
	Allocator allocator = current_allocator;

	void reallocate(umm desired_capacity TL_LP) {
		T *new_data;
		if (data) {
			new_data = allocator.reallocate_uninitialized<T>(data, count, desired_capacity TL_LA);
		} else {
			new_data = allocator.allocate_uninitialized<T>(desired_capacity TL_LA);
		}
		data = new_data;
		capacity = desired_capacity;
	}

	void set(Span<T> span TL_LP) {
		reserve(span.count TL_LA);
		count = span.count;
		memcpy(data, span.data, span.count * sizeof(T));
	}
	T &add(TL_LPC) {
		reserve_exponential(count + 1 TL_LA);
		return *new (data + count++) T();
	}
	T &add(T value TL_LP) {
		reserve_exponential(count + 1 TL_LA);
		auto dest = data + count;
		assert(dest == (data + count));
		memcpy(dest, &value, sizeof(T));
		return data[count++];
	}
	Span<T> add(Span<T> span TL_LP) {
		reserve_exponential(count + span.count TL_LA);
		memcpy(data + count, span.data, span.count * sizeof(T));
		count += span.count;
		return {data + count - span.count, span.count};
	}
	Span<T> add(std::initializer_list<T> list TL_LP) {
		reserve_exponential(count + list.size() TL_LA);
		memcpy(data + count, list.begin(), list.size() * sizeof(T));
		count += list.size();
		return {data + count - list.size(), list.size()};
	}

	T &add_front(T value TL_LP) {
		reserve_exponential(count + 1 TL_LA);
		for (umm i = count; i > 0; --i) {
			data[i] = data[i - 1];
		}
		data[0] = value;
		return data[0];
	}

	void reserve(umm desired_capacity TL_LP) {
		if (capacity >= desired_capacity) return;

		reallocate(desired_capacity TL_LA);
	}
	void reserve_exponential(umm desired_capacity TL_LP) {
		if (capacity >= desired_capacity) return;

		umm new_capacity = max(1u, capacity);
		while (new_capacity < desired_capacity) new_capacity *= 2;

		reallocate(new_capacity TL_LA);
	}
	void resize(umm new_count TL_LP) {
		reserve(new_count TL_LA);

		if (new_count > count) {
			for (umm i = count; i < new_count; ++i) {
				new (data + i) T();
			}
		}
		count = new_count;
	}

	void clear() {
		count = 0;
	}

	T pop() {
		--count;
		return data[count];
	}

	template <class U>
	explicit operator List<U>() const {
		List<U> result;
		result.allocator = allocator;
		result.data = (U *)data;
		if constexpr (sizeof(T) == sizeof(U)) {
			result.count      = count;
			result.capacity  = capacity;
		} else {
			if constexpr (sizeof(T) > sizeof(U)) {
				static_assert(sizeof(T) % sizeof(U) == 0);
			} else {
				static_assert(sizeof(U) % sizeof(T) == 0);
			}
			result.count      = count     * sizeof(T) / sizeof(U);
			result.capacity  = capacity * sizeof(T) / sizeof(U);
		}
		return result;
	}

	T &insert_at(T value, umm where TL_LP) {
		bounds_check(where <= count);

		reserve_exponential(count + 1 TL_LA);

		memmove(data + where + 1, data + where, (count - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++count;
		return data[where];
	}
	Span<T> insert_at(Span<T> span, umm where TL_LP) {
		bounds_check(where <= count);

		reserve_exponential(count + span.count TL_LA);

		memmove(data + where + span.count, data + where, (count - where) * sizeof(T));
		memcpy(data + where, span.data, span.count * sizeof(T));

		count += span.count;
		return {data + where, span.count};
	}
	T &insert(T value, T *where TL_LP) { return insert_at(value, where - data TL_LA); }
	Span<T> insert(Span<T> span, T *where TL_LP) { return insert_at(span, where - data TL_LA); }

	Span<T> insert_n_at(T const &value, umm where, umm n) {
		reserve(count + n);
		auto to_move_count = count - where;
		memmove(data + where + n, data + where, to_move_count * sizeof(T));
		for (umm i = 0; i != n; ++i)
			memcpy(data + where + i, &value, sizeof(T));
		return {data + where, n};
	}

	void erase(Span<T> where) {
		bounds_check(
			where.count <= count &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data, where.data + where.count, count - where.count + data - where.data);
		count -= where.count;
	}
	void erase_at(umm where) {
		bounds_check(where < count);
		--count;
		for (umm i = where; i < count; ++i) {
			data[i] = data[i + 1];
		}
	}

	void replace(Span<T> where, T with_what) {
		bounds_check(
			where.count <= count &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data + 1, where.data + where.count, end() - where.end());
		*where.data = with_what;

		count -= where.count - 1;
	}

	void replace(Span<T> where, Span<T> with_what) {
		assert(begin() <= where.begin());
		bounds_check(
			where.count <= count &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		T *old_data = data;
		reserve_exponential(count - where.count + with_what.count);
		where.data += data - old_data;

		memmove(
			where.data + with_what.count,
			where.data + where.count,
			(end() - where.end()) * sizeof(T)
		);
		memcpy(where.data, with_what.data, with_what.count * sizeof(T));

		count -= where.count - with_what.count;
	}

	void move(T *from, T *to) {
		T temp = *from;
		if (to < from) {
			memmove(to + 1, to, sizeof(T) * (from - to));
		} else {
			memmove(from, from + 1, sizeof(T) * (to - from));
		}
		*to = temp;
	}
	void move_at(T *from, umm destination_index) {
		move(from, data + destination_index);
	}
};

template <class T>
List<T> make_list(std::initializer_list<T> list TL_LP) {
	List<T> result;
	result.reserve(list.size() TL_LA);
	result.count = list.size();
	memcpy(result.data, list.begin(), list.size() * sizeof(T));
	return result;
}

template <class T>
void erase(List<T> &list, T *value) { list.erase_at(value - list.data); }

template <class T>
void erase_unordered_at(List<T> &list, umm index) {
	bounds_check(index < list.count);
	memcpy(list.data + index, &list.back(), sizeof(T));
	--list.count;
}

template <class T>
void free(List<T> &list) {
	if (list.data == 0) return;

	list.allocator.free_t(list.data, list.count);
	list.data = 0;
	list.count = 0;
	list.capacity = 0;
}

template <class T>
List<T> copy(List<T> that TL_LP) {
	List<T> result;
	result.count = that.count;
	result.capacity = result.count;
	result.data = result.allocator.allocate<T>(result.count TL_LA);
	memcpy(result.data, that.data, result.count * sizeof(T));
	return result;
}

template <class T>
List<T> to_list(Span<T> that, Allocator allocator = current_allocator TL_LP) {
	List<T> result;
	result.data = allocator.allocate<T>(that.count TL_LA);
	result.count = that.count;
	result.capacity = result.count;
	result.allocator = allocator;
	memcpy(result.data, that.data, result.count * sizeof(T));
	return result;
}

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
List<Span<T>> find_all(Span<T> where, Span<T> what) {
	List<Span<T>> result;

	for (umm where_start = 0; where_start != where.count - what.count + 1; ++where_start) {
		for (umm what_index = 0; what_index < what.count; ++what_index) {
			if (where.data[where_start + what_index] != what.data[what_index]) {
				goto continue_where;
			}
		}

		result.add(Span<T>(where.data + where_start, what.count));

	continue_where:;
	}

	return result;
}

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

template <class T, class Fn>
umm count(List<T> list, Fn &&fn) {
	umm result = 0;
	for (auto &v : list) {
		if (fn(v)) {
			result += 1;
		}
	}
	return result;
}

template <class T>
List<Span<T>> split(Span<T> what, Span<T> by TL_LP) {
	List<Span<T>> result;

	umm start = 0;
	umm what_start = 0;

	for (; what_start < what.count - by.count + 1;) {
		if (what.subspan(what_start, by.count) == by) {
			result.add(what.subspan(start, what_start - start));
			what_start += by.count;
			start = what_start;
		} else {
			++what_start;
		}
	}

	result.add(Span(what.data + start, what.end()) TL_LA);

	return result;
}

template <class T>
List<Span<T>> split(Span<T> what, T by TL_LP) {
	List<Span<T>> result;

	umm start = 0;
	umm what_start = 0;

	for (; what_start < what.count;) {
		if (what.data[what_start] == by) {
			result.add(what.subspan(start, what_start - start));
			start = what_start + 1;
		}
		++what_start;
	}

	result.add(Span(what.data + start, what.end()) TL_LA);

	return result;
}

template <class T>
List<T> replace(Span<T> where, T what, T with TL_LP) {
	List<T> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		result.add(v == what ? with : v);
	}
	return result;
}

template <class T, class Fn>
auto map(List<T> list, Fn &&fn TL_LP) {
	using U = decltype(fn(*(T*)0));
	List<U> result;
	result.reserve(list.count TL_LA);

	for (auto &x : list) {
		result.add(fn(x));
	}

	return result;
}

template <class T>
struct Queue : Span<T> {
	using Span<T>::data;
	using Span<T>::count;

	Allocator allocator = current_allocator;
	T *alloc_data = 0;
	umm alloc_count = 0;

	Queue() = default;
	Queue(Queue const &that) = default;
	Queue(Queue &&that) = delete;
	Queue &operator=(Queue const &that) = default;
	Queue &operator=(Queue &&that) = delete;

	T *begin() { return data; }
	T *end() { return data + count; }
	T const *begin() const { return data; }
	T const *end() const { return data + count; }

	umm space_back() const { return (umm)((alloc_data + alloc_count) - (data + count)); }

	bool empty() const { return count == 0; }

	T &front() { bounds_check(count); return data[0]; }
	T &back() { bounds_check(count); return data[count - 1]; }
	T &operator[](umm i) { bounds_check(count); return data[i]; }

	void push(T const &value) {
		_grow_if_needed(count + 1);
		data[count++] = value;
	}

	void push_front_unordered(T const &value) {
		_grow_if_needed(count + 1);
		if (count != 0) {
			data[count++] = data[0];
		}
		data[0] = value;
	}

	void pop() {
		bounds_check(count);
		++data;
		--count;
	}
	//void resize(umm new_count) {
	//	if (new_count > capacity())
	//		_reallocate(new_count);
	//	if (new_count > count()) {
	//		for (T *t = _end; t < _begin + new_count; ++t)
	//			new (t) T();
	//		_end = _begin + new_count;
	//	} else if (new_count < count()) {
	//		for (T *t = _begin + new_count; t < _end; ++t)
	//			t->~T();
	//		_end = _begin + new_count;
	//	}
	//}
	void clear() {
		data = alloc_data;
		count = 0;
	}

	operator Span<T>() { return {data, count}; }

	void _reallocate(umm new_capacity) {
	}

	void erase_at(umm where) {
		assert(where < count, "value is not in container");

		umm left_count = where;
		umm right_count = count - where - 1;

		--count;

		if (left_count <= right_count) {
			// move left half one to the right
			for (umm i = where; i != 0; --i) {
				data[i] = data[i - 1];
			}
			++data;
		} else {
			// move right half one to the left
			for (umm i = where; i < count; ++i) {
				data[i] = data[i + 1];
			}
		}
	}
	void erase(T *val) { erase_at(val - data); }
	void erase(T &val) { erase(&val); }

	void _grow_if_needed(umm required_count) {
		if (required_count <= space_back())
			return;
		umm new_capacity = space_back();
		if (new_capacity == 0)
			new_capacity = 1;
		while (new_capacity < required_count) {
			new_capacity *= 2;
		}

		T *new_data = allocator.allocate<T>(new_capacity);
		for (umm i = 0; i < count; ++i) {
			new_data[i] = data[i];
		}
		if (alloc_data)
			allocator.free(alloc_data);

		alloc_data = new_data;
		data = new_data;
		alloc_count = new_capacity;
	}
	T *insert(Span<T const> span, umm where) {
		bounds_check(where <= count);

		_grow_if_needed(count + span.count);

		for (umm i = where; i < count; ++i) {
			data[span.count + i] = data[i];
		}
		for (umm i = 0; i < span.count; ++i) {
			data[where + i] = span[i];
		}
		count += span.count;
		return data + where;
	}
	T *insert(Span<T const> span, T *where) {
		return insert(span, where - data);
	}
};

template <class T>
void free(Queue<T> &queue) {
	if (queue.alloc_data == 0) return;

	queue.allocator.free(queue.alloc_data);
	queue.data = 0;
	queue.count = 0;
	queue.alloc_data = 0;
	queue.alloc_count = 0;
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
struct LinearSet : Span<T> {
	using Span<T>::data;
	using Span<T>::count;
	using Span<T>::begin;
	using Span<T>::end;

	umm capacity = 0;
	Allocator allocator = current_allocator;

	T &insert(T const &value TL_LP) {
		for (auto &it : *this) {
			if (it == value) {
				return it;
			}
		}
		reserve_exponential(count + 1 TL_LA);
		return data[count++] = value;
	}

	void reallocate(umm desired_capacity TL_LP) {
		T *new_data;
		if (data) {
			new_data = allocator.reallocate_uninitialized<T>(data, count, desired_capacity TL_LA);
		} else {
			new_data = allocator.allocate_uninitialized<T>(desired_capacity TL_LA);
		}
		data = new_data;
		capacity = desired_capacity;
	}

	void reserve(umm desired_capacity TL_LP) {
		if (capacity >= desired_capacity) return;

		reallocate(desired_capacity TL_LA);
	}
	void reserve_exponential(umm desired_capacity TL_LP) {
		if (capacity >= desired_capacity) return;

		umm new_capacity = max(1u, capacity);
		while (new_capacity < desired_capacity) new_capacity *= 2;

		reallocate(new_capacity TL_LA);
	}

	void clear() {
		count = 0;
	}

	T pop() {
		bounds_check(count);
		--count;
		return data[count];
	}

	template <class U>
	explicit operator LinearSet<U>() const {
		LinearSet<U> result;
		result.allocator = allocator;
		result.data = (U *)data;
		if constexpr (sizeof(T) == sizeof(U)) {
			result.count      = count;
			result.capacity  = capacity;
		} else {
			if constexpr (sizeof(T) > sizeof(U)) {
				static_assert(sizeof(T) % sizeof(U) == 0);
			} else {
				static_assert(sizeof(U) % sizeof(T) == 0);
			}
			result.count      = count     * sizeof(T) / sizeof(U);
			result.capacity  = capacity * sizeof(T) / sizeof(U);
		}
		return result;
	}

	void erase(Span<T> where) {
		bounds_check(
			where.count <= count &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data, where.data + where.count, count - where.count + data - where.data);
		count -= where.count;
	}
	void erase_at(umm where) {
		bounds_check(where < count);
		--count;
		for (umm i = where; i < count; ++i) {
			data[i] = data[i + 1];
		}
	}

	void move(T *from, T *to) {
		T temp = *from;
		if (to < from) {
			memmove(to + 1, to, sizeof(T) * (from - to));
		} else {
			memmove(from, from + 1, sizeof(T) * (to - from));
		}
		*to = temp;
	}
	void move_at(T *from, umm destination_index) {
		move(from, data + destination_index);
	}

private:
	Span<T> add(Span<T> span TL_LP) {
		reserve_exponential(count + span.count TL_LA);
		memcpy(data + count, span.data, span.count * sizeof(T));
		count += span.count;
		return {data + count - span.count, span.count};
	}
	Span<T> add(std::initializer_list<T> list TL_LP) {
		reserve_exponential(count + list.size() TL_LA);
		memcpy(data + count, list.begin(), list.size() * sizeof(T));
		count += list.size();
		return {data + count - list.size(), list.size()};
	}
};

template <class T>
void free(LinearSet<T> &set) {
	free((List<T> &)set);
}

template <class T>
void erase(LinearSet<T> &set, T *value) { return erase((List<T> &)set, value); }

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
