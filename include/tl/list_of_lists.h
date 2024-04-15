#pragma once
#include "list.h"

namespace tl {

//
// Each list is allocated in the same buffer
//
// 
// If you want to read from list ensure that reading mode is enabled by calling list.enable_reading().
// 
// If you want to write to list ensure that writing mode is enabled by calling list.enable_writing(). It is enabled by default.
// 
// In reading mode 'data' member of lists' elements represent a pointer to the beginning of a list
// stored in 'buffer'.
// In writing mode 'data' represents an index into the 'buffer' where the element starts.
//
template <class T, class Allocator = Allocator, class Size = umm>
struct ListOfLists : List<Span<T>, Allocator, Size> {
	using Base = List<Span<T>, Allocator, Size>;
	List<T, Allocator, Size> buffer;
#if TL_DEBUG
	bool is_reading = false;
#endif

	void add(Span<T> string) {
#if TL_DEBUG
		assert(is_reading == false);
#endif

		Span<T> dest;
		dest.count = string.count;
		dest.data = (T *)buffer.count;
		Base::add(dest);

		buffer.add(string);
	}

	void enable_reading() {
#if TL_DEBUG
		assert(!is_reading);
		is_reading = true;
#endif
		for (auto &string : *this) {
			string.data = buffer.data + (umm)string.data;
		}
	}
	void enable_writing() {
#if TL_DEBUG
		assert(is_reading);
		is_reading = false;
#endif
		for (auto &string : *this) {
			string.data = (T *)(string.data - buffer.data);
		}
	}
	void clear() {
		Base::clear();
		buffer.clear();
	}

	Base &base() { return *this; }
};

template <class T, class Allocator, class Size>
void free(ListOfLists<T, Allocator, Size> &list) {
	free(list.base());
	free(list.buffer);
#if TL_DEBUG
	list.is_reading = false;
#endif
}

}
