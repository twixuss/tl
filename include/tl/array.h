#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

template <class T, umm size_>
struct Array {
	inline static constexpr umm size = size_;

	inline operator Span<T>() const { return {(T *)data, size}; }

	constexpr T *begin() { return data; }
	constexpr T *end() { return data + size; }
	constexpr T& operator[](umm i) {
		bounds_check(i < size);
		return data[i];
	}

	constexpr T const *begin() const { return data; }
	constexpr T const *end() const { return data + size; }
	constexpr T const& operator[](umm i) const {
		bounds_check(i < size);
		return data[i];
	}

	constexpr T &back() { return data[size - 1]; }
	constexpr T const &back() const { return data[size - 1]; }

	constexpr Span<T> operator+(umm i) {
		bounds_check(i <= size);
		return {data + i, size - i};
	}

	T data[size];
};

template <class T, umm size>
inline constexpr umm count_of(Array<T, size> const &arr) {
	return size;
}


template <class T, umm size>
inline constexpr Span<T> as_span(Array<T, size> &arr) {
	return {arr.data, arr.size};
}

template <class T, umm size>
inline constexpr Array<T, size> to_array(T const (&array)[size]) {
	Array<T, size> result{};
	for (umm i = 0; i < size; ++i) {
		result.data[i] = array[i];
	}
	return result;
}

template <class T, umm _size_x, umm _size_y>
struct Array2 {
	inline static constexpr umm size_x = _size_x;
	inline static constexpr umm size_y = _size_y;

	constexpr T& at(umm x, umm y) {
		bounds_check(&data[y][x] < end());
		return data[y][x];
	}

	constexpr T *begin() { return (T *)data; }
	constexpr T *end() { return (T *)data + size_x * size_y; }

	T data[size_y][size_x];
};

template <class T, umm _size_x, umm _size_y, umm _size_z>
struct Array3 {
	inline static constexpr umm size_x = _size_x;
	inline static constexpr umm size_y = _size_y;
	inline static constexpr umm size_z = _size_z;

	constexpr T& at(umm x, umm y, umm z) {
		bounds_check(&data[z][y][x] < end());
		return data[z][y][x];
	}
	constexpr T& operator()(umm x, umm y, umm z) {
		bounds_check(&data[z][y][x] < end());
		return data[z][y][x];
	}
	constexpr T& at(v3s v) { return at(v.x, v.y, v.z); }
	constexpr T& at(v3u v) { return at(v.x, v.y, v.z); }

	constexpr T *begin() { return (T *)data; }
	constexpr T *end() { return (T *)data + size_x * size_y * size_z; }

	T data[size_z][size_y][size_x];
};

}
