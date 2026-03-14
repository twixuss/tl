#pragma once

#include "list.h"

namespace tl {

template <class T>
struct SortedListDefaultCompare {
	forceinline constexpr smm operator()(T a, T b) {
		return compare(a, b);
	}
};

template <
	class T,
	auto map = identity_value,
	auto compare = SortedListDefaultCompare<std::remove_cvref_t<decltype(map(*(T *)0))>> {},
	class Allocator = Allocator,
	class Size = umm
>
struct SortedList : private List<T, Allocator, Size> {
	using Base = List<T, Allocator, Size>;
	using Element = T;

	using Base::begin;
	using Base::end;
	using Base::data;
	using Base::count;
	using Base::capacity;
	using Base::allocator;
	using Base::span;
	using Base::clear;
	using Base::pop;
	using Base::iter;
	using Base::erase;
	using Base::erase_at;
	using Base::operator[];
	using Base::owns;

	T &add(T value TL_LP) {
		Base::reserve(count + 1);
		auto search_result = search(value);
		return Base::insert(value, search_result.would_be_at);
	}
	
	T *find(T value) {
		return search(value).found;
	}
	
	BinarySearchResult<T> search(T const &value) {
		return binary_search(Base::span(), [&](T &it) {
			return compare(map(value), map(it));
		});
	}

	template <class U>
	T *find(U const &value)
		requires requires {
			{ map(data[0]) } -> std::same_as<U>;
		}
	{
		return binary_search(Base::span(), [&](T &it) {
			return compare(value, map(it));
		}).found;
	}

	// Erase `where` and add `new_value` in a single operation.
	T &replace(T *where, T new_value) {
		// TODO: Optimize. This implementation always moves memory to the right of `where` left once,
		//       then moves memory to the right of `new_value` right once. There is some
		//       overlap between these regions, meaning it ends up in same place. Wasteful.
		erase(where);
		return add(new_value);
	}
	
	Base &_list() {
		return *this;
	}
	Base const &_list() const {
		return *this;
	}
};

template <class T, auto map, auto compare, class Allocator, class Size>
void free(SortedList<T, map, compare, Allocator, Size> &list) {
	free(list._list());
}

}