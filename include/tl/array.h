#pragma once
#include "common.h"

namespace TL {

template <class T, umm _size>
struct Array {
	constexpr umm size() { return _size; }

	constexpr T *data() { return v; }
	constexpr T& operator[](umm i) { return v[i]; }

	constexpr T const *data() const { return v; }
	constexpr T const& operator[](umm i) const { return v[i]; }
	T v[_size];
};

}
