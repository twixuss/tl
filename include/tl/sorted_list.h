#pragma once

#include "list.h"

namespace tl {

template <class T, auto sort_by = identity_value, class Allocator = Allocator, class Size = umm>
struct SortedList : private List<T, Allocator, Size> {
	using Base = List<T, Allocator, Size>;

	using Base::begin;
	using Base::end;
	using Base::data;
	using Base::count;
	using Base::capacity;
	using Base::allocator;
	using Base::span;
	using Base::clear;
	using Base::pop;
	using Base::operator[];

	void add(T value TL_LP) {
		Base::reserve(count + 1);
		Base::insert(value, binary_search(Base::span(), value, sort_by).would_be_at);
	}
};

}