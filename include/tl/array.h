#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

template <class T, umm count_>
struct Array {
	inline static constexpr umm count = count_;

	inline operator Span<T>() const { return {(T *)data, count}; }

	constexpr T *begin() { return data; }
	constexpr T *end() { return data + count; }
	constexpr T& operator[](umm i) {
		bounds_check(i < count);
		return data[i];
	}

	constexpr T const *begin() const { return data; }
	constexpr T const *end() const { return data + count; }
	constexpr T const& operator[](umm i) const {
		bounds_check(i < count);
		return data[i];
	}
	
	constexpr Span<T> span() { return {begin(), end()}; }

	constexpr T &back() { return data[count - 1]; }
	constexpr T const &back() const { return data[count - 1]; }

	constexpr Span<T> operator+(umm i) {
		bounds_check(i <= count);
		return {data + i, count - i};
	}

	T data[count];
};

template <class T, umm count>
constexpr umm count_of(Array<T, count> const &arr) {
	return count;
}

template <class T, umm count>
constexpr umm index_of(Array<T, count> const &arr, T *pointer) {
	return pointer - arr.begin();
}

template <class T, umm count>
constexpr bool owns(Array<T, count> &array, T *pointer) {
	return index_of(array, pointer) < count;
}

template <class T, umm count>
constexpr Span<T> as_span(Array<T, count> &arr) {
	return {arr.data, arr.count};
}

template <class T, umm count>
constexpr Array<T, count> to_array(T const (&array)[count]) {
	Array<T, count> result{};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = array[i];
	}
	return result;
}

template <class T, umm _count_x, umm _count_y>
struct Array2 {
	inline static constexpr umm count_x = _count_x;
	inline static constexpr umm count_y = _count_y;
	inline static constexpr umm flat_count = count_x * count_y;

	constexpr T& at(umm x, umm y) {
		bounds_check(&data[y][x] < end());
		return data[y][x];
	}
	template <class Scalar>
	constexpr T& at(v2<Scalar> v) {
		return at(v.x, v.y);
	}
	template <class Scalar>
	constexpr T& operator[](v2<Scalar> v) { return at(v); }

	constexpr T *begin() { return (T *)data; }
	constexpr T *end() { return (T *)data + flat_count; }

	constexpr Span<T> span() { return {begin(), end()}; }

	T data[count_y][count_x];
};

template <class T, umm count_x, umm count_y>
v2umm index_of(Array2<T, count_x, count_y> &array, T *value) {
	umm flat_index = value - &array.data[0][0];
	return {
		flat_index % count_x,
		flat_index / count_x,
	};
}

template <class T, umm count_x, umm count_y>
constexpr bool owns(Array2<T, count_x, count_y> &array, T *pointer) {
	return index_of(array, pointer) < array.flat_count;
}

// Is there actually no way to have an Array2 that takes vector count of any type? Sadge.

template <class T, umm _count_x, umm _count_y, umm _count_z>
struct Array3 {
	inline static constexpr umm count_x = _count_x;
	inline static constexpr umm count_y = _count_y;
	inline static constexpr umm count_z = _count_z;

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
	constexpr T *end() { return (T *)data + count_x * count_y * count_z; }

	T data[count_z][count_y][count_x];
};

template <class T, umm count>
auto dot(Array<T, count> const &a, Array<T, count> const &b) {
	T result = 0;
	for (umm i = 0; i < count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

}
