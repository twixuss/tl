#pragma once
#include "common.h"
#include "list.h"

namespace tl {

// Collection of unique elements, stored contiguously in order of addition.
template <class T, class Allocator = Allocator, class Size_ = umm>
struct LinearSet : List<T, Allocator, Size_> {
	using ElementType = T;
	using Size = Size_;
	using Span = Span<T, Size>;
	using List = List<T, Allocator, Size>;

	using Span::data;
	using Span::count;
	using Span::begin;
	using Span::end;

	umm capacity = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	T &add(TL_LPC) = delete;
	T &add(T value TL_LP) {
		for (auto &it : *this) {
			if (it == value) {
				return it;
			}
		}
		return List::add(value);
	}
	template <class ThatSize>
	void add(tl::Span<T, ThatSize> span TL_LP) {
		for (auto x : span)
			add(x);
	}
	void add(std::initializer_list<T> list TL_LP) {
		for (auto x : list)
			add(x);
	}
	
	// Moves the data! Slow!
	T &add_front(T value = {} TL_LP) {
		for (auto &it : *this) {
			if (it == value) {
				return it;
			}
		}
		return List::add_front(value);
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
