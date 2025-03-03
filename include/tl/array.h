#pragma once
#include "common.h"

namespace tl {

template <class T, umm count_>
struct Array {
	using Element = T;
	
	inline static constexpr umm count = count_;

	T data[count];

	constexpr auto begin(this auto &&self) { return self.data; }
	constexpr auto end(this auto &&self) { return self.data + self.count; }

	inline operator Span<T>() const { return {(T *)data, count}; }
	constexpr Span<T> span() const { return {begin(), end()}; }

	constexpr auto &at(this auto &&self, umm i) {
		bounds_check(assert_less(i, self.count));
		return self.data[i];
	}
	constexpr auto &operator[](this auto &&self, umm i) { return self.at(i); }

	constexpr auto &front(this auto &&self) { return self.data[0]; }
	constexpr auto &back(this auto &&self) { return self.data[self.count - 1]; }
	
	constexpr umm index_of(T const *pointer) const { return pointer - data; }
	constexpr bool owns(T *pointer) { return (umm)(pointer - data) < count; }
	
	constexpr bool for_each(auto &&in_fn) {

		auto fn = wrap_foreach_fn<T *>(in_fn);
	
		constexpr ForEachFlags allowed_flags = ForEach_break;

		for (auto &it : data) {
			auto d = fn(it);
		
			assert(!(d & ~allowed_flags), "not supported");

			if (d & ForEach_break)
				return true;
		}
		return false;
	}

	#define OP(op)                                                               \
		constexpr auto operator op() const requires requires(T t) { op t; } {    \
			Array<std::remove_cvref_t<decltype(op data[0])>, count> result = {}; \
			for (umm i = 0; i < count; ++i)                                      \
				result.data[i] = op data[i];                                     \
			return result;                                                       \
		}
	OP(+)
	OP(-)
	OP(~)
	OP(!)
	OP(*)
	#undef OP
		
	#define OP(op)                                                                               \
		constexpr auto operator op(Array const &that) const requires requires(T t) { t op t; } { \
			Array<std::remove_cvref_t<decltype(data[0] op data[0])>, count> result = {};         \
			for (umm i = 0; i < count; ++i)                                                      \
				result.data[i] = data[i] op that.data[i];                                        \
			return result;                                                                       \
		}
	OP(==)
	OP(!=)
	OP(<)
	OP(>)
	OP(<=)
	OP(>=)
	OP(+)
	OP(-)
	OP(*)
	OP(/)
	OP(%)
	OP(^)
	OP(&)
	OP(|)
	OP(&&)
	OP(||)
	#undef OP
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

}
