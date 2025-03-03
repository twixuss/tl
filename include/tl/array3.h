#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

template <class T, umm count_x, umm count_y, umm count_z>
struct Array3 {
	using Element = T;

	inline static constexpr v3umm count = {count_x, count_y, count_z};
	inline static constexpr umm flat_count = count_x * count_y * count_z;
	
	T data[count_z][count_y][count_x];

	constexpr auto &at_unchecked(this auto &&self, umm x, umm y, umm z) {
		return self.data[z][y][x];
	}
	template <class Scalar>
	constexpr auto &at_unchecked(this auto &&self, v3<Scalar> i) {
		return self.data[i.z][i.y][i.x];
	}
	constexpr auto &at(this auto &&self, umm x, umm y, umm z) {
		bounds_check(assert_less(x, count_x));
		bounds_check(assert_less(y, count_y));
		bounds_check(assert_less(z, count_z));
		return self.data[z][y][x];
	}
	template <class Scalar>
	constexpr auto &at(this auto &&self, v3<Scalar> i) {
		return self.at(i.x, i.y, i.z);
	}
	template <class Scalar>
	constexpr auto &operator[](this auto &&self, v3<Scalar> i) {
		return self.at(i.x, i.y, i.z);
	}

	constexpr auto begin(this auto &&self) { return &self.data[0][0][0]; }
	constexpr auto end(this auto &&self) { return &self.data[count_z][count_y][count_x]; }

	constexpr bool owns(T *pointer) const {
		return (umm)(pointer - &data[0][0][0]) < flat_count;
	}
};

}
