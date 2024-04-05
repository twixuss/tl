#pragma once

#include "list.h"

namespace tl {

template <class T, class Allocator = Allocator, class Size = umm>
struct SortedList : private List<T, Allocator, Size> {
	using Base = List<T, Allocator, Size>;

	using Base::begin;
	using Base::end;
	using Base::data;
	using Base::count;
	using Base::capacity;
	using Base::span;
	using Base::clear;
	using Base::pop;
	using Base::operator[];

	void add(T value TL_LP) {
		Base::reserve_exponential(count + 1);
		Base::insert(value, binary_search(Base::span(), value).would_be_at);
	}
};

}