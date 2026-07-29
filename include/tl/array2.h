#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

// As in C's multidimensional arrays, last axis has the stride of a single element. first axis has the largest stride.
// E.g. Array2Layout::yx is equivalent to C's arr[count_y][count_x]
enum class Array2Layout : u8 {
	yx,
	xy,
};

template <class T, umm count_x, umm count_y, Array2Layout layout_ = Array2Layout::yx>
union Array2 {
	using Element = T;

	inline static constexpr v2umm count = {.x = count_x, .y = count_y}; // NOTE: must designate .x and .y so it is constexpr.
	inline static constexpr umm flat_count = count.x * count.y;
	
	inline static constexpr Array2Layout layout = layout_;
	
	inline static constexpr v2umm stride = [] () -> v2umm {
		switch (layout) {
			case Array2Layout::yx: return { 1, count.x };
			case Array2Layout::xy: return { count.y, 1 };
		}
	}();

	T data[flat_count];

	forceinline constexpr auto &at_unchecked(this auto &&self, umm x, umm y) {
		return self.data[x*stride.x + y*stride.y];
	}
	template <class Scalar>
	forceinline constexpr auto &at_unchecked(this auto &&self, v3<Scalar> i) {
		return self.at_unchecked(i.x, i.y);
	}
	forceinline constexpr auto &at(this auto &&self, umm x, umm y) {
		bounds_check(assert_less(x, count_x));
		bounds_check(assert_less(y, count_y));
		return self.at_unchecked(x, y);
	}
	template <class Scalar>
	forceinline constexpr auto &at(this auto &&self, v2<Scalar> i) {
		return self.at(i.x, i.y);
	}
	template <class Scalar>
	forceinline constexpr auto &operator[](this auto &&self, v2<Scalar> i) {
		return self.at(i.x, i.y);
	}

	forceinline constexpr auto begin(this auto &&self) { return &self.data[0]; }
	forceinline constexpr auto end(this auto &&self) { return &self.data[count_x*count_y]; }

	forceinline constexpr bool owns(T *pointer) const {
		return (umm)pointer - (umm)data < flat_count*sizeof(T);
	}

	forceinline constexpr auto &as_c_array(this auto &&self) {
		using CT = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const T, T>;
		
		if constexpr (false) {}
		else if constexpr (layout == Array2Layout::yx) return *(CT (*)[count_y][count_x])self.data;
		else if constexpr (layout == Array2Layout::xy) return *(CT (*)[count_x][count_y])self.data;
	}

	constexpr Span<T> span() const { return {begin(), end()}; }
	
	constexpr auto vectors() {
		if constexpr (false) {}
		else if constexpr (layout == Array2Layout::yx) return Span((Array<T, count.x> *)data, count.y);
		else if constexpr (layout == Array2Layout::xy) return Span((Array<T, count.y> *)data, count.x);
	}

	constexpr v2umm index_of(T const *value) const {
		umm i = value - data;
		
		if constexpr (false) {}
		else if constexpr (layout == Array2Layout::yx) return { i % count.x, i / count.x };
		else if constexpr (layout == Array2Layout::xy) return { i / count.y, i % count.y };
	}

	constexpr auto apply(this auto &&self, auto &&fn)
		requires requires { fn(self.data[0]); }
	{
		Array2<std::remove_cvref_t<decltype(fn(self.data[0]))>, count.x, count.y, layout> result = {};
		for (umm i = 0; i < flat_count; ++i)
			result.data[i] = fn(self.data[i]);
		return result;
	}
};

// Is there actually no way to have an Array2 that takes vector count of any type? Sadge.
// Tried struct specializations. msvc and clang implicitly cast values making specializations ambiguous,
// gcc only allows to use one specialization and fails on second one :) https://godbolt.org/z/4nKz8Y9z9  - 03.03.25

}




