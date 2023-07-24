#pragma once
#include "optional.h"

#pragma warning(push)
#pragma warning(disable : 4582 4624)

#ifndef TL_INITIAL_LIST_CAPACITY
#define TL_INITIAL_LIST_CAPACITY 4
#endif

namespace tl {

#pragma pack(push, 1)
template <class T, class Allocator_ = Allocator, class Size_ = umm>
struct List : Span<T, Size_> {
	using ElementType = T;
	using Allocator = Allocator_;
	using Size = Size_;
	using Span = Span<T, Size_>;

	using Span::data;
	using Span::count;
	using Span::begin;
	using Span::end;

	Size capacity = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	void reallocate(Size desired_capacity TL_LP) {
		T *new_data;
		if (data) {
			new_data = allocator.reallocate_uninitialized<T>(data, count, desired_capacity TL_LA);
		} else {
			new_data = allocator.allocate_uninitialized<T>(desired_capacity TL_LA);
		}
		data = new_data;
		capacity = desired_capacity;
	}

	void set(T value TL_LP) {
		reserve(1 TL_LA);
		count = 1;
		memcpy(data, &value, sizeof(T));
	}
	void set(Span span TL_LP) {
		reserve(span.count TL_LA);
		count = span.count;
		memcpy(data, span.data, span.count * sizeof(T));
	}
	T &add(TL_LPC) {
		auto resized = reserve_exponential(count + 1 TL_LA);
		return *new (data + count++) T();
	}
	T &add(T value TL_LP) {
		auto resized = reserve_exponential(count + 1 TL_LA);
		memcpy(data + count, &value, sizeof(T));
		return data[count++];
	}
	template <class Size>
	tl::Span<T> add(tl::Span<T, Size> span TL_LP) {
		auto resized = reserve_exponential(count + span.count TL_LA);
		memcpy(data + count, span.data, span.count * sizeof(T));
		count += span.count;
		return {data + count - span.count, span.count};
	}
	Span add(std::initializer_list<T> list TL_LP) {
		auto resized = reserve_exponential(count + list.size() TL_LA);
		memcpy(data + count, list.begin(), list.size() * sizeof(T));
		count += list.size();
		return {data + count - list.size(), (Size)list.size()};
	}

	T &add_front(T value TL_LP) {
		auto resized = reserve_exponential(count + 1 TL_LA);
		for (Size i = count; i > 0; --i) {
			data[i] = data[i - 1];
		}
		data[0] = value;
		count += 1;
		return data[0];
	}

	void reserve(Size desired_capacity TL_LP) {
		if (capacity >= desired_capacity) return;

		reallocate(desired_capacity TL_LA);
	}
	// returns true if the list was relocated
	bool reserve_exponential(Size desired_capacity TL_LP) {
		if (capacity >= desired_capacity) return false;

		Size new_capacity = max(TL_INITIAL_LIST_CAPACITY, capacity);
		while (new_capacity < desired_capacity) new_capacity *= 2;

		reallocate(new_capacity TL_LA);

		return true;
	}
	void resize(Size new_count TL_LP) {
		reserve(new_count TL_LA);

		if (new_count > count) {
			for (Size i = count; i < new_count; ++i) {
				new (data + i) T();
			}
		}
		count = new_count;
	}

	void ensure_capacity(Size desired_space) {
		reserve(count + desired_space);
	}

	void clear() {
		count = 0;
	}

	Optional<T> pop() {
		if (count == 0)
			return {};

		--count;
		return data[count];
	}

	template <class U, class ThatSize>
	explicit operator List<U, Allocator, ThatSize>() const {
		assert((ThatSize)count == count);
		assert((ThatSize)capacity == capacity);
		List<U, Allocator, ThatSize> result;
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

	template <class ThatSize>
	operator List<T, Allocator, ThatSize>() const {
		assert((ThatSize)count == count);
		assert((ThatSize)capacity == capacity);
		List<T, Allocator, ThatSize> result;
		result.data = data;
		result.count = (ThatSize)count;
		result.capacity = (ThatSize)capacity;
		result.allocator = allocator;
		return result;
	}

	T &insert_at(T value, Size where TL_LP) {
		bounds_check(where <= count);

		reserve_exponential(count + 1 TL_LA);

		memmove(data + where + 1, data + where, (count - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++count;
		return data[where];
	}
	Span insert_at(Span span, Size where TL_LP) {
		bounds_check(where <= count);

		reserve_exponential(count + span.count TL_LA);

		memmove(data + where + span.count, data + where, (count - where) * sizeof(T));
		memcpy(data + where, span.data, span.count * sizeof(T));

		count += span.count;
		return {data + where, span.count};
	}
	T &insert(T value, T *where TL_LP) { return insert_at(value, where - data TL_LA); }
	Span insert(Span span, T *where TL_LP) { return insert_at(span, where - data TL_LA); }

	Span insert_n_at(T const &value, Size where, Size n) {
		reserve(count + n);
		auto to_move_count = count - where;
		memmove(data + where + n, data + where, to_move_count * sizeof(T));
		for (Size i = 0; i != n; ++i)
			memcpy(data + where + i, &value, sizeof(T));
		count += n;
		return {data + where, n};
	}

	void erase(Span where_) {
		bounds_check(
			where_.count <= count &&
			begin() <= where_.begin() && where_.begin() < end() &&
			where_.end() <= end()
		);

		memmove(where_.data, where_.data + where_.count, (count - where_.count + data - where_.data) * sizeof(T));
		count -= where_.count;
	}
	void erase_at(Size where) {
		bounds_check(where < count);
		--count;
		for (Size i = where; i < count; ++i) {
			data[i] = data[i + 1];
		}
	}

	void replace(Span where, T with_what) {
		bounds_check(
			where.count <= count &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data + 1, where.data + where.count, (end() - where.end()) * sizeof(T));
		*where.data = with_what;

		count -= where.count - 1;
	}

	void replace(Span where, Span with_what) {
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
	void move_at(T *from, Size destination_index) {
		move(from, data + destination_index);
	}

	Span span() { return *this; }
};
#pragma pack(pop)

template <class T, class Allocator, class Size>
void erase(List<T, Allocator, Size> &list, T *value) { list.erase_at(value - list.data); }

template <class T, class Allocator, class Size>
void erase_unordered_at(List<T, Allocator, Size> &list, umm index) {
	bounds_check(index < list.count);
	memcpy(list.data + index, &list.back(), sizeof(T));
	--list.count;
}

template <class T, class Allocator, class Size>
void erase_unordered(List<T, Allocator, Size> &list, T *at) {
	*at = list.back();
	list.count--;
}

template <class T, class Allocator, class Size>
void free(List<T, Allocator, Size> &list) {
	if (list.data == 0) return;

	list.allocator.free_t(list.data, list.count);
	list.data = 0;
	list.count = 0;
	list.capacity = 0;
}

template <class T, class Allocator, class Size>
List<T, Allocator, Size> copy(List<T, Allocator, Size> that TL_LP) {
	List<T, Allocator, Size> result;
	result.count = that.count;
	result.capacity = result.count;
	result.data = result.allocator.allocate<T>(result.count TL_LA);
	memcpy(result.data, that.data, result.count * sizeof(T));
	return result;
}

template <class Allocator = Allocator, class Size, class T>
List<T, Allocator, Size> to_list(Span<T, Size> that, Allocator allocator = Allocator::current() TL_LP) {
	List<T, Allocator, Size> result;
	result.data = allocator.allocate<T>(that.count TL_LA);
	result.count = that.count;
	result.capacity = result.count;
	result.allocator = allocator;
	memcpy(result.data, that.data, result.count * sizeof(T));
	return result;
}

template <class T, class Allocator, class Size>
T *next(List<T, Allocator, Size> list, T *value) {
	return ++value == list.end() ? 0 : value;
}

template <class T, class Allocator, class Size>
T *previous(List<T, Allocator, Size> list, T *value) {
	return value-- == list.data ? 0 : value;
}

template <class T, class Allocator, class Size> constexpr T *find(List<T, Allocator, Size> list, T const &value) { return find(as_span(list), value); }
template <class T, class Allocator, class Size> constexpr T *find(List<T, Allocator, Size> list, Span<T> cmp) { return find(as_span(list), cmp); }
template <class T, class Allocator, class Size> constexpr T *find_last(List<T, Allocator, Size> list, T const &value) { return find_last(as_span(list), value); }

template <class T>
void find_all(Span<T> where, Span<T> what, auto &&on_find) {
	if (what.count > where.count)
		return;

	for (umm where_start = 0; where_start != where.count - what.count + 1; ++where_start) {
		for (umm what_index = 0; what_index < what.count; ++what_index) {
			if (where.data[where_start + what_index] != what.data[what_index]) {
				goto continue_where;
			}
		}

		on_find(Span<T>(where.data + where_start, what.count));

	continue_where:;
	}
}

template <class T, class Allocator = Allocator, class Size = umm>
List<Span<T>, Allocator, Size> find_all(Span<T> where, Span<T> what) {
	List<Span<T>, Allocator, Size> result;
	find_all(where, what, [&](auto v) { result.add(v); });
	return result;
}

template <class T, class Allocator, class Size>
Span<T> as_span(List<T, Allocator, Size> const &list) {
	return (Span<T>)list;
}

template <class T, class Allocator, class Size, class Predicate>
constexpr T *find_if(List<T, Allocator, Size> &list, Predicate &&predicate) {
	return find_if(as_span(list), std::forward<Predicate>(predicate));
}

template <class T, class Allocator, class Size, class Predicate>
constexpr T const *find_if(List<T, Allocator, Size> const &list, Predicate &&predicate) {
	return find_if(as_span(list), std::forward<Predicate>(predicate));
}

template <class T, class Allocator, class Size, class Fn>
constexpr void for_each(List<T, Allocator, Size> list, Fn &&fn) {
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

template <class T, class Allocator, class Size, class Fn>
umm count(List<T, Allocator, Size> list, Fn &&fn) {
	umm result = 0;
	for (auto &v : list) {
		if (fn(v)) {
			result += 1;
		}
	}
	return result;
}

template <class T, class Allocator = Allocator, class Size = umm>
List<Span<T>, Allocator, Size> split(Span<T> what, Span<T> by TL_LP) {
	List<Span<T>, Allocator, Size> result;

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

template <class T, class Size, class Fn>
void split(Span<T, Size> what, T by, Fn &&callback) {
	umm start = 0;
	umm what_start = 0;

	for (; what_start < what.count;) {
		if (what.data[what_start] == by) {
			callback(what.subspan(start, what_start - start));
			start = what_start + 1;
		}
		++what_start;
	}

	callback(Span(what.data + start, what.end()));
}
template <class T, class SSize, class Allocator = Allocator, class DSize = umm>
List<Span<T>, Allocator, DSize> split(Span<T, SSize> what, T by TL_LP) {
	List<Span<T>, Allocator, DSize> result;

	split(what, by, [&](auto part) {
		result.add(part TL_LA);
	});

	return result;
}

template <class T, class Allocator, class Size>
List<T, Allocator, Size> replace(Span<T> where, T what, T with TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		result.add(v == what ? with : v);
	}
	return result;
}

template <class Allocator, class T, class Size>
List<T, Allocator, Size> erase_all(Span<T, Size> where, T what TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		if (v != what)
			result.add(v);
	}
	return result;
}

template <class T, class Size, class Fn>
auto map(Span<T, Size> span, Fn &&fn TL_LP) {
	using U = decltype(fn(*(T*)0));
	List<U> result;
	result.reserve(span.count TL_LA);

	for (auto &x : span) {
		result.add(fn(x));
	}

	return result;
}

template <class T, class Allocator = Allocator, class Size = umm>
List<T, Allocator, Size> make_list(std::initializer_list<T> list TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(list.size() TL_LA);
	result.count = list.size();
	memcpy(result.data, list.begin(), list.size() * sizeof(T));
	return result;
}

// Returns an index of the value
// If value is not in the list, this function will return index >= list.size
template <class T, class Allocator, class Size>
umm index_of(List<T, Allocator, Size> const &list, T const *value) {
	return value - list.data;
}

template <class T, class Allocator, class Size> umm count_of(List<T, Allocator, Size> const &list) { return list.size(); }

template <class T, class Allocator, class Size> T const &front(List<T, Allocator, Size> const &list) { return list.front(); }
template <class T, class Allocator, class Size> T &front(List<T, Allocator, Size> &list) { return list.front(); }

template <class T, class Allocator, class Size> T const &back(List<T, Allocator, Size> const &list) { return list.back(); }
template <class T, class Allocator, class Size> T &back(List<T, Allocator, Size> &list) { return list.back(); }

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

	void push(T const &value TL_LP) {
		_grow_if_needed(count + 1 TL_LA);
		data[count++] = value;
	}

	void push_front_unordered(T const &value TL_LP) {
		_grow_if_needed(count + 1 TL_LA);
		if (count != 0) {
			data[count++] = data[0];
		}
		data[0] = value;
	}

	Optional<T> pop() {
		if (!count)
			return {};
		defer {
			++data;
			--count;
		};
		return *data;
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

	void _grow_if_needed(umm required_count TL_LP) {
		if (required_count <= space_back())
			return;
		umm new_capacity = space_back();
		if (new_capacity == 0)
			new_capacity = 1;
		while (new_capacity < required_count) {
			new_capacity *= 2;
		}

		T *new_data = allocator.allocate<T>(new_capacity TL_LA);
		for (umm i = 0; i < count; ++i) {
			new_data[i] = data[i];
		}
		if (alloc_data)
			allocator.free_t(alloc_data, alloc_count);

		alloc_data = new_data;
		data = new_data;
		alloc_count = new_capacity;
	}
	T *insert(Span<T const> span, umm where TL_LP) {
		bounds_check(where <= count);

		_grow_if_needed(count + span.count TL_LA);

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
struct StaticRingBuffer {
	inline static constexpr umm capacity = _capacity;
	struct Iterator {
		using value_type = T;
		StaticRingBuffer *buffer;
		umm index;
		Iterator(StaticRingBuffer *buffer, umm index) : buffer(buffer), index(index) {}
		T &operator*() { return buffer->at(index); }
		T &operator*() const { return buffer->at(index); }
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
#if 0
	StaticRingBuffer() = default;
	StaticRingBuffer(StaticRingBuffer const &that) {
		count = that.count;

		if (that.data + that.count > that.storage + that.capacity) {
			auto first_count  = that.capacity - that.data;
			auto second_count = that.count - first_count;
			memcpy(data, that.data, first_count * sizeof(T));
			memcpy(data, that.storage, second_count * sizeof(T));
		} else {
			memcpy(data, that.data, that.count * sizeof(T));
		}
	}
	StaticRingBuffer(StaticRingBuffer &&that) {
		count = that.count;
		umm dstIndex = that.start;
		for (umm srcIndex = 0; srcIndex < count; ++srcIndex) {
			new (&storage[srcIndex].value) T(std::move(that.storage[dstIndex].value));
			if (++dstIndex == capacity)
				dstIndex = 0;
		}
		that.start = 0;
		that.count = 0;
	}
	StaticRingBuffer &operator=(StaticRingBuffer const &that) { clear(); return *new (this) StaticRingBuffer(that); }
	StaticRingBuffer &operator=(StaticRingBuffer &&that) { clear(); return *new (this) StaticRingBuffer(std::move(that)); }
	~StaticRingBuffer() {
		for (umm i = 0; i < count; ++i) {
			at(i).~T();
		}
		start = 0;
		count = 0;
	}
#endif
	T &push_back(T value) {
		bounds_check(count != capacity);
		auto dst = data + count;
		count += 1;
		return *(dst - capacity * (dst >= storage + capacity)) = value;
	}
	void push_back(Span<T> span) {
		bounds_check(count + span.count <= capacity);
		if (data + span.count > storage + capacity) {
			auto first_count  = storage + capacity - data;
			auto second_count = span.count - first_count;
			memcpy(data, span.data, first_count * sizeof(T));
			memcpy(storage, span.data + first_count, second_count * sizeof(T));
		} else {
			memcpy(data, span.data, span.count * sizeof(T));
		}

		count += span.count;
	}
	T &push_front(T value) {
		bounds_check(count != capacity);
		count += 1;
		data = sub_wrap(data, 1);
		return *data = value;
	}

	Optional<T> pop_back() {
		if (!count)
			return {};
		return at(count -= 1);
	}
	Optional<T> pop_front() {
		if (!count)
			return {};
		auto result_ptr = data;
		data = add_wrap(data, 1);
		count -= 1;
		return *result_ptr;
	}

	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, count); }

	T &back() { bounds_check(count); return at(count - 1); }
	T &front() { bounds_check(count); return at(0); }

	bool is_empty() const { return count == 0; }
	bool is_full() const { return count == capacity; }

	T &operator[](umm i) { bounds_check(i < count); return at(i); }
	T const &operator[](umm i) const { bounds_check(i < count); return at(i); }
#if 0
	void erase(Iterator it) {
		umm index = it.index;
		bounds_check(index < count);
		--count;
		if (index > count / 2) {
			for (;index < count; ++index) {
				at(index) = std::move(at(index + 1));
			}
			at(count).~T();
		} else {
			for (;index; --index) {
				at(index) = std::move(at(index - 1));
			}
			at(0).~T();
			_incBegin();
		}
	}

	void _incBegin() {
		data += 1;
		data -= capacity * (data >= storage + capacity);
	}
#endif

	void clear() {
		data  = storage;
		count = 0;
	}

	T &at(umm i) {
		return *add_wrap(data, i);
	}

	T *add_wrap(T *p, umm i) {
		p += i;
		return p - capacity * (p >= storage + capacity);
	}
	T *sub_wrap(T *p, umm i) {
		p -= i;
		return p + capacity * (p < storage);
	}

	union {
		T storage[capacity];
	};
	T *data = storage;
	umm count = 0;
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
struct StaticRingQueue : private StaticRingBuffer<T, _capacity> {
	using Base = StaticRingBuffer<T, _capacity>;
	using Base::data;
	using Base::count;
	using Base::capacity;
	using Base::begin;
	using Base::end;
	using Base::clear;
	using Base::operator[];
	T &push(T value) { return this->push_back(value); }
	void push(Span<T> span) { return this->push_back(span); }
	Optional<T> pop() { return this->pop_front(); }
};

// Collection of unique elements, stored contiguously in order of addition.
template <class T, class Size_ = umm>
struct LinearSet : Span<T, Size_> {
	using ElementType = T;
	using Allocator = Allocator;
	using Size = Size_;
	using Span = Span<T, Size>;

	using Span::data;
	using Span::count;
	using Span::begin;
	using Span::end;

	umm capacity = 0;
	Allocator allocator = current_allocator;

	T &add(T const &value TL_LP) {
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

	void erase(Span where) {
		bounds_check(
			where.count <= count &&
			begin() <= where.begin() && where.begin() < end() &&
			where.end() <= end()
		);

		memmove(where.data, where.data + where.count, (count - where.count + data - where.data) * sizeof(T));
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

	void set(T value TL_LP) {
		reserve(1 TL_LA);
		count = 1;
		memcpy(data, &value, sizeof(T));
	}
	void set(Span span TL_LP) {
		reserve(span.count TL_LA);
		count = span.count;
		memcpy(data, span.data, span.count * sizeof(T));
	}
	void add(Span span TL_LP) {
		for (auto &value : span) {
			add(value);
		}
	}
	void add(std::initializer_list<T> list TL_LP) {
		for (auto &value : list) {
			add(value);
		}
	}
};

template <class T>
void free(LinearSet<T> &set) {
	free((List<T> &)set);
}

template <class T>
umm index_of(LinearSet<T> const &list, T const *pointer) {
	return pointer - list.data;
}
template <class T>
void erase(LinearSet<T> &set, T *pointer) { return erase((List<T> &)set, pointer); }

template <class T>
void erase_unordered(LinearSet<T> &set, T *pointer) {
	set[index_of(set, pointer)] = set.back();
	set.count--;
}

template <class T>
void erase(LinearSet<T> &set, T value) {
	for (auto &existing : set) {
		if (existing == value) {
			erase(set, &existing);
		}
	}
}

template <class T, class Size>
LinearSet<T, Size> copy(LinearSet<T, Size> that TL_LP) {
	LinearSet<T, Size> result;
	result.count = that.count;
	result.capacity = result.count;
	result.data = result.allocator.allocate<T>(result.count TL_LA);
	memcpy(result.data, that.data, result.count * sizeof(T));
	return result;
}

#pragma warning(pop)

}
