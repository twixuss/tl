#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

template <class T, umm count_x, umm count_y>
struct Array2 {
	using Element = T;

	inline static constexpr v2umm count = {count_x, count_y};
	inline static constexpr umm flat_count = count.x * count.y;
	
	T data[count.y][count.x];

	constexpr auto &at_unchecked(this auto &&self, umm x, umm y) {
		return self.data[y][x];
	}
	template <class Scalar>
	constexpr auto &at_unchecked(this auto &&self, v2<Scalar> v) {
		return self.data[v.y][v.x];
	}
	constexpr auto &at(this auto &&self, umm x, umm y) {
		bounds_check(assert_less(x, count.x));
		bounds_check(assert_less(y, count.y));
		return self.data[y][x];
	}
	template <class Scalar>
	constexpr auto &at(this auto &&self, v2<Scalar> v) {
		bounds_check(assert_less(v.x, count.x));
		bounds_check(assert_less(v.y, count.y));
		return self.data[v.y][v.x];
	}
	template <class Scalar>
	constexpr auto &operator[](this auto &&self, v2<Scalar> v) {
		bounds_check(assert_less(v.x, count.x));
		bounds_check(assert_less(v.y, count.y));
		return self.data[v.y][v.x];
	}

	constexpr auto begin(this auto &&self) { return &self.data[0][0]; }
	constexpr auto end(this auto &&self) { return &self.data[0][0] + flat_count; }

	constexpr Span<T> span() const { return {begin(), end()}; }
	
	constexpr v2umm index_of(T const *value) const {
		umm flat_index = value - &data[0][0];
		return {
			flat_index % count.x,
			flat_index / count.x,
		};
	}

	constexpr bool owns(T *pointer) {
		return (umm)(pointer - &data[0][0]) < flat_count;
	}
};

// Is there actually no way to have an Array2 that takes vector count of any type? Sadge.
// Tried struct specializations. msvc and clang implicitly cast values making specializations ambiguous,
// gcc only allows to use one specialization and fails on second one :) https://godbolt.org/z/4nKz8Y9z9  - 03.03.25

}




