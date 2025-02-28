#pragma once
#include "common.h"
#include "vector.h"

namespace tl {

template <class T, umm count_>
struct Array {
	using ElementType = T;

	inline static constexpr umm count = count_;

	constexpr auto begin()       { return data; }
	constexpr auto begin() const { return data; }
	constexpr auto end()       { return data + count; }
	constexpr auto end() const { return data + count; }

	inline operator Span<T>() const { return {(T *)data, count}; }
	constexpr Span<T> span() const { return {begin(), end()}; }

	constexpr auto &at_unchecked(umm i)       { return data[i]; }
	constexpr auto &at_unchecked(umm i) const { return data[i]; }
	constexpr auto &at(umm i) {
		bounds_check(assert_less(i, count));
		return at_unchecked(i);
	}
	constexpr auto &at(umm i) const {
		bounds_check(assert_less(i, count));
		return at_unchecked(i);
	}
	constexpr auto &operator[](umm i)       { return at(i); }
	constexpr auto &operator[](umm i) const { return at(i); }

	constexpr auto &front()       { return data[0]; }
	constexpr auto &front() const { return data[0]; }
	constexpr auto &back()       { return data[count - 1]; }
	constexpr auto &back() const { return data[count - 1]; }

	T data[count];
};

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <umm count, class T>
constexpr Array<T, count> broadcast_to_array(T value) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = value;
	}
	return result;
}

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

template <class T, umm count>
constexpr bool for_each(Array<T, count> &array, auto &&in_fn) {

	auto fn = wrap_foreach_fn<T &>(in_fn);
	
	constexpr ForEachFlags allowed_flags = ForEach_break;

	for (auto &it : array.data) {
		auto d = fn(it);
		
		assert(!(d & ~allowed_flags), "not supported");

		if (d & ForEach_break)
			return true;
	}
	return false;
}

template <class T, umm count>
constexpr auto operator==(Array<T, count> const &a, Array<T, count> const &b) {
	Array<decltype(a.data[0] == b.data[0]), count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = a.data[i] == b.data[i];
	}
	return result;
}

template <class T, umm count>
constexpr bool all(Array<T, count> const &a) {
	for (umm i = 0; i < count; ++i) {
		if (!all(a.data[i])) {
			return false;
		}
	}
	return true;
}

template <class T, umm count>
auto dot(Array<T, count> const &a, Array<T, count> const &b) {
	T result = 0;
	for (umm i = 0; i < count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

#define TL_ARRAY_DEFINE_APPLY_1(function)                                             \
	template <class T, umm count>                                                     \
	forceinline constexpr auto function(Array<T, count> input) {                      \
		Array<std::remove_cvref_t<decltype(function(v.data[0]))>, count> result = {}; \
		for (umm i = 0; i < count; ++i) {                                             \
			result.data[i] = function(input.data[i]);                                 \
		}                                                                             \
		return result;                                                                \
	}

template <class T, umm _count_x, umm _count_y>
struct Array2 {
	using ElementType = T;

	inline static constexpr umm count_x = _count_x;
	inline static constexpr umm count_y = _count_y;
	inline static constexpr umm flat_count = count_x * count_y;

	constexpr auto &at_unchecked(umm x, umm y)       { return data[y][x]; }
	constexpr auto &at_unchecked(umm x, umm y) const { return data[y][x]; }
	constexpr auto &at(umm x, umm y) {
		bounds_check(assert_less(x, count_x));
		bounds_check(assert_less(y, count_y));
		return at_unchecked(x, y);
	}
	constexpr auto &at(umm x, umm y) const {
		bounds_check(assert_less(x, count_x));
		bounds_check(assert_less(y, count_y));
		return at_unchecked(x, y);
	}
	template <class Scalar>
	constexpr auto &at_unchecked(v2<Scalar> v) {
		return at_unchecked(v.x, v.y);
	}
	template <class Scalar>
	constexpr auto &at_unchecked(v2<Scalar> v) const {
		return at_unchecked(v.x, v.y);
	}
	template <class Scalar>
	constexpr auto &at(v2<Scalar> v) {
		return at(v.x, v.y);
	}
	template <class Scalar>
	constexpr auto &at(v2<Scalar> v) const {
		return at(v.x, v.y);
	}
	constexpr auto &operator()(umm x, umm y) {
		return at(x,y);
	}
	constexpr auto &operator()(umm x, umm y) const {
		return at(x,y);
	}
	template <class Scalar>
	constexpr auto &operator[](v2<Scalar> v) { return at(v); }
	template <class Scalar>
	constexpr auto &operator[](v2<Scalar> v) const { return at(v); }

	constexpr auto begin()       { return &data[0][0]; }
	constexpr auto begin() const { return &data[0][0]; }
	constexpr auto end()       { return &data[0][0] + flat_count; }
	constexpr auto end() const { return &data[0][0] + flat_count; }

	constexpr Span<T> span() const { return {begin(), end()}; }

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
	using ElementType = T;

	inline static constexpr umm count_x = _count_x;
	inline static constexpr umm count_y = _count_y;
	inline static constexpr umm count_z = _count_z;

	constexpr T& at_unchecked(umm x, umm y, umm z) {
		return data[z][y][x];
	}
	template <class Scalar>
	constexpr T& at_unchecked(v3<Scalar> i) {
		return at_unchecked(i.x, i.y, i.z);
	}
	constexpr T& at(umm x, umm y, umm z) {
		bounds_check(assert_less(x, count_x));
		bounds_check(assert_less(y, count_y));
		bounds_check(assert_less(z, count_z));
		return at_unchecked(x,y,z);
	}
	template <class Scalar>
	constexpr T& at(v3<Scalar> i) {
		return at(i.x, i.y, i.z);
	}
	constexpr T& operator()(umm x, umm y, umm z) {
		return at(x,y,z);
	}
	template <class Scalar>
	constexpr T& operator[](v3<Scalar> i) {
		return at(i);
	}
	constexpr T& at(v3s v) { return at(v.x, v.y, v.z); }
	constexpr T& at(v3u v) { return at(v.x, v.y, v.z); }

	constexpr T *begin() { return (T *)data; }
	constexpr T *end() { return (T *)data + count_x * count_y * count_z; }

	T data[count_z][count_y][count_x];
};

}
