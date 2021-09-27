#pragma once
#include "common.h"

#ifndef null_pointer_check
#define null_pointer_check(x) assert(x)
#endif

namespace tl {

template <class T>
struct Pointer {
	Pointer(T *data) : data(data) {}
	T &get() const { null_pointer_check(data); return *data; }
	T &get_unchecked() const { return *data; }
	T *operator->() const { null_pointer_check(data); return data; }
	T &operator*() const { null_pointer_check(data); return *data; }

	T *raw() const { return data; }

	explicit operator bool() const { return data != 0; }

private:
	T *data;
};

}
