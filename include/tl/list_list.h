#pragma once
#include "list.h"

namespace TL {

//
// Each list is allocated in the same buffer
//
// Note: To start using 'lists' member ensure that list is in 'absolute mode',
// so 'data' member of lists' elements represent a pointer to the beginning of a list
// stored in 'buffer'
//
// Addition is allowed only In 'relative mode', where 'data' member of list's elements holds
// an index to the 'buffer'
//
template <class T>
struct ListList : List<Span<T>> {
	using Base = List<Span<T>>;
	List<T> buffer;
#ifdef TL_DEBUG
	bool is_absolute = false;
#endif

	void add(Span<T> string) {
#ifdef TL_DEBUG
		assert(is_absolute == false);
#endif

		Span<T> dest;
		dest.size = string.size;
		dest.data = (T *)buffer.size;
		Base::add(dest);

		buffer += string;
	}

	void make_absolute() {
#ifdef TL_DEBUG
		is_absolute = true;
#endif
		for (auto &string : *this) {
			string.data = buffer.data + (umm)string.data;
		}
	}
	void make_relative() {
#ifdef TL_DEBUG
		is_absolute = false;
#endif
		for (auto &string : *this) {
			string.data = (T *)(string.data - buffer.data);
		}
	}

	Base &base() { return *this; }
};

template <class T>
void free(ListList<T> &list) {
	free(list.base());
	free(list.buffer);
#ifdef TL_DEBUG
	list.is_absolute = false;
#endif
}

}