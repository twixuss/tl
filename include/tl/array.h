#pragma once
#include "common.h"

namespace TL {

template <class T, umm size_>
struct Array {
	constexpr umm size() { return size_; }

	constexpr T *data() { return v; }
	constexpr T *begin() { return v; }
	constexpr T *end() { return v + size_; }
	constexpr T& operator[](umm i) { return v[i]; }

	constexpr T const *data() const { return v; }
	constexpr T const *begin() const { return v; }
	constexpr T const *end() const { return v + size_; }
	constexpr T const& operator[](umm i) const { return v[i]; }
	T v[size_];
};

}
