#pragma once
#include "common.h"

#pragma warning(push)
#pragma warning(disable : 4582 4624)

#ifndef TL_INITIAL_LIST_CAPACITY
#define TL_INITIAL_LIST_CAPACITY 4
#endif

#ifndef TL_INITIAL_QUEUE_CAPACITY
#define TL_INITIAL_QUEUE_CAPACITY TL_INITIAL_LIST_CAPACITY
#endif

namespace tl {

#pragma pack(push, 1)
template <class T, class Allocator_ = Allocator, class Size_ = umm>
struct List : Span<T, Size_> {
	using Element = T;
	using Allocator = Allocator_;
	using Size = Size_;
	using Span = Span<T, Size_>;

	using Span::data;
	using Span::count;
	using Span::begin;
	using Span::end;
	using Span::front;
	using Span::back;

	Size capacity = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

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
		reserve_exponential(count + 1 TL_LA);
		return *new (data + count++) T();
	}
	T &add(T value TL_LP) {
		reserve_exponential(count + 1 TL_LA);
		memcpy(data + count, &value, sizeof(T));
		return data[count++];
	}
	template <class ThatSize>
	Span add(tl::Span<T, ThatSize> span TL_LP) {
		reserve_exponential(count + span.count TL_LA);
		memcpy(data + count, span.data, span.count * sizeof(T));
		count += span.count;
		return {data + count - span.count, (Size)span.count};
	}
	Span add(std::initializer_list<T> list TL_LP) {
		reserve_exponential(count + list.size() TL_LA);
		memcpy(data + count, list.begin(), list.size() * sizeof(T));
		count += list.size();
		return {data + count - list.size(), (Size)list.size()};
	}

	// Moves the data! Slow!
	T &add_front(T value TL_LP) {
		reserve_exponential(count + 1 TL_LA);
		memmove(data + 1, data, count * sizeof(T));
		data[0] = value;
		count += 1;
		return data[0];
	}
	
	void reallocate(Size desired_capacity TL_LP) {
		T *new_data;
		if (data) {
			new_data = allocator.template reallocate_uninitialized<T>(data, capacity, desired_capacity TL_LA);
		} else {
			new_data = allocator.template allocate_uninitialized<T>(desired_capacity TL_LA);
		}
		data = new_data;
		capacity = desired_capacity;
	}

	// Reserves no more than `desired_capacity` elements.
	// Returns true if `data` was relocated.
	bool reserve(Size desired_capacity TL_LP) {
		if (capacity >= desired_capacity) {
			return false;
		}

		reallocate(desired_capacity TL_LA);
		return true;
	}

	// Returns true if `data` was relocated
	bool reserve_exponential(Size desired_capacity TL_LP) {
		if (capacity >= desired_capacity) {
			return false;
		}

		Size new_capacity = ceil_to_power_of_2(max((Size)TL_INITIAL_LIST_CAPACITY, desired_capacity));

		reallocate(new_capacity TL_LA);

		return true;
	}
	void resize(Size new_count, T new_value = {} TL_LP) {
		reserve(new_count TL_LA);

		for (Size i = count; i < new_count; ++i) {
			new (data + i) T(new_value);
		}

		count = new_count;
	}

	void reserve_after_end(Size desired_space) {
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
		assert_equal((ThatSize)count, count);
		assert_equal((ThatSize)capacity, capacity);
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
		assert_equal((ThatSize)count, count);
		assert_equal((ThatSize)capacity, capacity);
		List<T, Allocator, ThatSize> result;
		result.data = data;
		result.count = (ThatSize)count;
		result.capacity = (ThatSize)capacity;
		result.allocator = allocator;
		return result;
	}

	T &insert_at(T value, Size where TL_LP) {
		bounds_check(assert(where <= count));

		reserve_exponential(count + 1 TL_LA);

		memmove(data + where + 1, data + where, (count - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++count;
		return data[where];
	}
	Span insert_at(Span span, Size where TL_LP) {
		bounds_check(assert(where <= count));

		reserve_exponential(count + span.count TL_LA);

		memmove(data + where + span.count, data + where, (count - where) * sizeof(T));
		memcpy(data + where, span.data, span.count * sizeof(T));

		count += span.count;
		return {data + where, span.count};
	}
	T &insert(T value, T *where TL_LP) { return insert_at(value, where - data TL_LA); }
	Span insert(Span span, T *where TL_LP) { return insert_at(span, where - data TL_LA); }

	Span insert_at(Repeat<T> repeat, Size where) {
		reserve(count + repeat.count);
		auto to_move_count = count - where;
		memmove(data + where + repeat.count, data + where, to_move_count * sizeof(T));
		for (Size i = 0; i != repeat.count; ++i)
			memcpy(data + where + i, &repeat.value, sizeof(T));
		count += repeat.count;
		return {data + where, repeat.count};
	}

	void erase(Span where_) {
		bounds_check(assert(begin() <= where_.begin() && where_.begin() <= end()));
		bounds_check(assert(begin() <= where_.end() && where_.end() <= end()));

		memmove(where_.data, where_.data + where_.count, (count - where_.count + data - where_.data) * sizeof(T));
		count -= where_.count;
	}
	Optional<T> erase_at(Size where) {
		if (where < count) {
			--count;
			auto result = data[where];
			for (Size i = where; i < count; ++i) {
				data[i] = data[i + 1];
			}
			return result;
		}
		return {};
	}
	
	void erase_all(auto &&predicate) requires requires(T v) { predicate(v); } {
		auto end = data + count;
		auto dst = data;
		for (auto it = data; it != end; ++it) {
			if (predicate(*it)) {
				it->~T();
				--count;
			} else {
				*dst++ = *it;
			}
		}
	}
	
	void here_map(auto &&map) requires requires(T v) { { map(v) } -> std::same_as<T>; } {
		auto end = data + count;
		for (auto it = data; it != end; ++it) {
			*it = map(*it);
		}
	}
	
	void here_map(auto &&map) requires requires(T v) { { map(v) } -> std::same_as<Optional<T>>; } {
		auto end = data + count;
		auto dst = data;
		for (auto it = data; it != end; ++it) {
			auto mapped = map(*it);
			if (mapped) {
				*dst++ = mapped.value();
			} else {
				it->~T();
				--count;
			}
		}
	}

	void erase(T *value) { 
		erase_at(value - data);
	}

	void erase_unordered_at(Size index) {
		bounds_check(assert(index < count));
		memcpy(data + index, &back(), sizeof(T));
		--count;
	}

	void erase_unordered(T *at) {
		*at = back();
		count--;
	}

	
	// Same as:
	// index = list.index_of(where.begin());
	// erase(where);
	// insert_at(with_what, index)
	void replace(Span where, T with_what) {
		bounds_check(assert(where.count <= count));
		bounds_check(assert(begin() <= where.begin() && where.begin() < end()));
		bounds_check(assert(where.end() <= end()));

		memmove(where.data + 1, where.data + where.count, (end() - where.end()) * sizeof(T));
		*where.data = with_what;

		count -= where.count - 1;
	}
	
	// Same as:
	// index = list.index_of(where.begin());
	// erase(where);
	// insert_at(with_what, index)
	void replace(Span where, Span with_what) {
		bounds_check(assert(begin() <= where.begin() && where.begin() <= end()));
		bounds_check(assert(begin() <= where.end()   && where.end()   <= end()));

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

	// Same as:
	// index = list.index_of(to);
	// value = list.erase(from);
	// list.insert_at(value, index)
	void move(T *from, T *to) {
		T temp = *from;
		if (to < from) {
			memmove(to + 1, to, sizeof(T) * (from - to));
		} else {
			memmove(from, from + 1, sizeof(T) * (to - from));
		}
		*to = temp;
	}
	
	Span span() const { return *this; }


	bool for_each(ForEachIterator<T *> auto in_fn) {
		auto fn = wrap_foreach_fn<T *>(in_fn);

		for (auto it = data; it != data + count; ++it) {
			auto d = fn(it);
			switch (d & ForEach_erase_mask) {
				case ForEach_erase:
					// NOTE: This shifts the array on each iteration.
					// If no shifting is done the array will be in an
					// invalid state until the end of the loop,
					// which might be observed by the iterator.
					erase(it--);
					break;
				case ForEach_erase_unordered:
					erase_unordered(it--);
					break;
			}

			if (d & ForEach_break)
				return true;
		}

		return false;
	}
};
#pragma pack(pop)

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
	result.set(as_span(that));
	return result;
}

template <class Allocator = Allocator, class T>
List<T, Allocator> to_list(std::initializer_list<T> that, Allocator allocator = Allocator::current() TL_LP) {
	List<T, Allocator> result;
	result.allocator = allocator;
	result.set(as_span(that));
	return result;
}

template <class Allocator = Allocator, class Size, class T>
List<T, Allocator, Size> to_list(Span<T, Size> that, Allocator allocator = Allocator::current() TL_LP) {
	List<T, Allocator, Size> result;
	result.allocator = allocator;
	result.set(as_span(that));
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
constexpr bool for_each(List<T, Allocator, Size> &list, Fn &&fn) {
	using ReturnType = decltype(fn(*(T*)0));
	for (auto it = list.begin(); it != list.end(); ++it) {
		if constexpr (std::is_same_v<ReturnType, void>) {
			fn(*it);
		} else if constexpr (std::is_same_v<ReturnType, ForEachDirective>) {
			auto d = fn(*it);

			assert(!((d & ForEach_erase) && (d & ForEach_erase_unordered)));
			if (d & ForEach_erase) list.erase(it--);
			if (d & ForEach_erase_unordered) list.erase_unordered(it--);
			if (d & ForEach_break) return true;
		} else {
			static_assert("Invalid return type in for_each iterator");
		}
	}
	return false;
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
List<Span<T>, Allocator, Size> split_by_seq(Span<T> what, Span<T> by TL_LP) {
	List<Span<T>, Allocator, Size> result;
	
	split_by_seq(what, by, [&] (Span<T> part) {
		result.add(part);
	});

	return result;
}

template <class Allocator = Allocator, class T, class Size>
List<Span<T>, Allocator, Size> split_by_one(Span<T, Size> what, T by TL_LP) {
	List<Span<T>, Allocator, Size> result;

	split_by_one(what, by, [&](auto part) {
		result.add(part TL_LA);
	});

	return result;
}

template <class T, class Size>
void replace_inplace(Span<T, Size> where, T what, T with) {
	for (auto &v : where) {
		v = v == what ? with : v;
	}
}

template <class T, class Size>
void replace_inplace(Span<T, Size> &where, Span<T> what, Span<T> with) {
	T *found = where.begin();
	while (found = find(Span(found, where.end()), what)) {
		memcpy(found, with.data, with.count * sizeof(T));
		memmove(found + with.count, found + what.count, (where.end() - (found + what.count)) * sizeof(T));
		where.count += with.count - what.count;
	}
}

template <class Allocator = Allocator, class T, class Size>
List<T, Allocator, Size> replace(Span<T, Size> where, T what, T with TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		result.add(v == what ? with : v);
	}
	return result;
}

template <class Allocator = Allocator, class T, class Size>
List<T, Allocator, Size> erase_all(Span<T, Size> where, T what TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		if (v != what)
			result.add(v);
	}
	return result;
}

template <class Allocator = Allocator, class T, class Size, class Fn>
auto map(Span<T, Size> span, Fn &&fn TL_LP) {
	using U = decltype(fn(*(T*)0));
	List<U, Allocator, Size> result;
	result.reserve(span.count TL_LA);

	for (auto &x : span) {
		result.add(fn(x));
	}

	return result;
}

template <class Allocator = Allocator, class T, class Size = umm>
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

template <class T, class Allocator, class Size> umm count_of(List<T, Allocator, Size> const &list) { return list.count; }

template <class T, class Allocator, class Size> T const &front(List<T, Allocator, Size> const &list) { return list.front(); }
template <class T, class Allocator, class Size> T &front(List<T, Allocator, Size> &list) { return list.front(); }

template <class T, class Allocator, class Size> T const &back(List<T, Allocator, Size> const &list) { return list.back(); }
template <class T, class Allocator, class Size> T &back(List<T, Allocator, Size> &list) { return list.back(); }

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
		bounds_check(assert(count != capacity));
		auto dst = data + count;
		count += 1;
		return *(dst - capacity * (dst >= storage + capacity)) = value;
	}
	void push_back(Span<T> span) {
		bounds_check(assert(count + span.count <= capacity));
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
		bounds_check(assert(count != capacity));
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

	T &back() { bounds_check(assert(count); return at(count - 1); )}
	T &front() { bounds_check(assert(count); return at(0); )}

	bool is_empty() const { return count == 0; }
	bool is_full() const { return count == capacity; }

	T &operator[](umm i) { bounds_check(assert(i < count); return at(i); )}
	T const &operator[](umm i) const { bounds_check(assert(i < count); return at(i); )}
#if 0
	void erase(Iterator it) {
		umm index = it.index;
		bounds_check(assert(index < count));
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

	T &back() { bounds_check(assert(_size); return _get(_size - 1); )}
	T &front() { bounds_check(assert(_size); return _get(0); )}

	umm capacity() const { return (umm)(_allocEnd - _allocBegin); }
	umm size() const { return _size; }
	bool empty() const { return _size == 0; }
	bool full() const { return _size == capacity(); }

	T &operator[](umm i) { bounds_check(assert(i < _size); return _get(i); )}
	T const &operator[](umm i) const { bounds_check(assert(i < _size); return _get(i); )}

	void erase(Iterator it) {
		umm index = it.index;
		bounds_check(assert(index < _size));
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

#pragma warning(pop)

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	List<int> l;

	assert(l.count == 0);
	assert(l.capacity == 0);
	assert(l.data == 0);
	l.add(42);
	assert(l.count == 1);
	assert(l[0] == 42);
	l.add(69);
	assert(l.count == 2);
	assert(l[0] == 42);
	assert(l[1] == 69);

	assert(l.pop().value() == 69);

	assert(l.count == 1);

	free(l);
};

#endif
