#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

// As in C's multidimensional arrays, last axis has the stride of a single element. first axis has the largest stride.
// E.g. Array3Layout::zyx is equivalent to C's arr[count_z][count_y][count_x]
enum class Array3Layout : u8 {
	zyx,
	yzx,
	zxy,
	xzy,
	yxz,
	xyz,
};

template <class T, umm count_x, umm count_y, umm count_z, Array3Layout layout_ = Array3Layout::zyx>
struct Array3 {
	using Element = T;

	inline static constexpr v3umm count = {count_x, count_y, count_z};
	inline static constexpr umm flat_count = count_x * count_y * count_z;

	inline static constexpr Array3Layout layout = layout_;
	
	inline static constexpr v3umm stride = [] () -> v3umm {
		switch (layout) {
			case Array3Layout::zyx: return { 1,               count_x,         count_x*count_y };
			case Array3Layout::yzx: return { 1,               count_x*count_z, count_x         };
			case Array3Layout::zxy: return { count_y,         1,               count_y*count_x };
			case Array3Layout::xzy: return { count_y*count_z, 1,               count_y         };
			case Array3Layout::yxz: return { count_z,         count_z*count_x, 1               };
			case Array3Layout::xyz: return { count_z*count_y, count_z,         1               };
		}
	}();

	T data[flat_count];

	forceinline constexpr auto &at_unchecked(this auto &&self, umm x, umm y, umm z) {
		return self.data[x*stride.x + y*stride.y + z*stride.z];
	}
	template <class Scalar>
	forceinline constexpr auto &at_unchecked(this auto &&self, v3<Scalar> i) {
		return self.at_unchecked(i.x, i.y, i.z);
	}
	forceinline constexpr auto &at(this auto &&self, umm x, umm y, umm z) {
		bounds_check(assert_less(x, count_x));
		bounds_check(assert_less(y, count_y));
		bounds_check(assert_less(z, count_z));
		return self.at_unchecked(x, y, z);
	}
	template <class Scalar>
	forceinline constexpr auto &at(this auto &&self, v3<Scalar> i) {
		return self.at(i.x, i.y, i.z);
	}
	template <class Scalar>
	forceinline constexpr auto &operator[](this auto &&self, v3<Scalar> i) {
		return self.at(i.x, i.y, i.z);
	}

	forceinline constexpr auto begin(this auto &&self) { return &self.data[0]; }
	forceinline constexpr auto end(this auto &&self) { return &self.data[count_x*count_y*count_z]; }

	forceinline constexpr bool owns(T *pointer) const {
		return (umm)pointer - (umm)data < flat_count*sizeof(T);
	}
};

}
