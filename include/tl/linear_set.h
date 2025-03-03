#pragma once
#include "common.h"

namespace tl {

// Collection of unique elements, stored contiguously in order of addition.
template <class T, class Allocator = Allocator, class Size_ = umm>
struct LinearSet : Span<T, Size_> {
	using ElementType = T;
	using Size = Size_;
	using Span = Span<T, Size>;

	using Span::data;
	using Span::count;
	using Span::begin;
	using Span::end;

	umm capacity = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	T &add(T const &value TL_LP) {
		for (auto &it : *this) {
			if (it == value) {
				return it;
			}
		}
		reserve_exponential(count + 1 TL_LA);
		return data[count++] = value;
	}

	bool try_add(T const &value TL_LP) {
		for (auto &it : *this) {
			if (it == value) {
				return false;
			}
		}
		reserve_exponential(count + 1 TL_LA);
		data[count++] = value;
		return true;
	}

	void reallocate(umm desired_capacity TL_LP) {
		T *new_data;
		if (data) {
			new_data = allocator.template reallocate_uninitialized<T>(data, capacity, desired_capacity TL_LA);
		} else {
			new_data = allocator.template allocate_uninitialized<T>(desired_capacity TL_LA);
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

		umm new_capacity = max((umm)1, capacity);
		while (new_capacity < desired_capacity) new_capacity *= 2;

		reallocate(new_capacity TL_LA);
	}

	void clear() {
		count = 0;
	}

	Optional<T> pop() {
		if (count)
			return data[--count];
		return {};
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
		bounds_check(assert(where.count <= count));
		bounds_check(assert(begin() <= where.begin() && where.begin() < end()));
		bounds_check(assert(where.end() <= end()));

		memmove(where.data, where.data + where.count, (count - where.count + data - where.data) * sizeof(T));
		count -= where.count;
	}
	void erase_at(umm where) {
		bounds_check(assert(where < count));
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
	
	Span span() { return *this; }
};

template <class T, class Allocator>
void free(LinearSet<T, Allocator> &set) {
	free((List<T, Allocator> &)set);
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
void erase_unordered_at(LinearSet<T> &set, umm index) {
	set[index] = set.back();
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
	result.data = result.allocator.template allocate<T>(result.count TL_LA);
	memcpy(result.data, that.data, result.count * sizeof(T));
	return result;
}


}
