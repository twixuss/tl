#pragma once
#include "common.h"

namespace TL {

template <class T, umm size_>
struct Array {
	constexpr umm size() const { return size_; }

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

template <class T, umm size>
inline constexpr umm count_of(Array<T, size> const &arr) {
	return arr.size();
}

template <class T, class _IndexType, _IndexType _size_x, _IndexType _size_y, _IndexType _size_z>
struct Array3 {
	using IndexType = _IndexType;
	inline static constexpr IndexType size_x = _size_x;
	inline static constexpr IndexType size_y = _size_y;
	inline static constexpr IndexType size_z = _size_z;

	constexpr T& at(IndexType x, IndexType y, IndexType z) {
		TL_BOUNDS_CHECK(x < size_x);
		TL_BOUNDS_CHECK(y < size_y);
		TL_BOUNDS_CHECK(z < size_z);
		return data[z][y][x];
	}

	constexpr T *begin() { return (T *)data; }
	constexpr T *end() { return (T *)data + size_z * size_y * size_x; }

	T data[size_z][size_y][size_x];
};

}
