#pragma once
#include "common.h"

namespace tl {

template <class T, umm count_>
struct Array {
	using Element = T;
	
	inline static constexpr umm count = count_;

	T data[count];

	forceinline constexpr auto begin(this auto &&self) { return self.data; }
	forceinline constexpr auto end(this auto &&self) { return self.data + self.count; }

	inline operator Span<T>() const { return {(T *)data, count}; }
	forceinline constexpr Span<T> span() const { return {begin(), end()}; }

	forceinline constexpr auto &at(this auto &&self, umm i) {
		bounds_check(assert_less(i, self.count));
		return self.data[i];
	}
	forceinline constexpr auto &operator[](this auto &&self, umm i) { return self.at(i); }

	forceinline constexpr auto &front(this auto &&self) { return self.data[0]; }
	forceinline constexpr auto &back(this auto &&self) { return self.data[self.count - 1]; }
	
	forceinline constexpr umm index_of(T const *pointer) const { return pointer - data; }
	forceinline constexpr bool owns(T *pointer) { return (umm)(pointer - data) < count; }
	
	forceinline constexpr auto iter(ReverseIterOption options = {}) {
		return span_iter(data, data + count, options);
	}

	#define OP(op)                                                               \
		forceinline constexpr auto operator op() const requires requires(T t) { op t; } {    \
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
		
	template <class U>
	forceinline constexpr operator Array<U, count>() const requires requires(T t) { (U)t; } {
		Array<U, count> result = {};
		for (umm i = 0; i < count; ++i)
			result.data[i] = (U)data[i];
		return result;
	};
	
	#define OP(op)                                                                                              \
		template <class U>                                                                                      \
		forceinline constexpr auto operator op(Array<U, count> const &that) const requires requires(T t, U u) { t op u; } { \
			Array<std::remove_cvref_t<decltype(data[0] op that.data[0])>, count> result = {};                   \
			for (umm i = 0; i < count; ++i)                                                                     \
				result.data[i] = data[i] op that.data[i];                                                       \
			return result;                                                                                      \
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
	OP(<<)
	OP(>>)
	OP(&&)
	OP(||)
	#undef OP
		
	#define OP(op)                                                                            \
		template <class U>                                                                    \
		forceinline constexpr auto operator op(U const &that) const                                       \
			requires requires(T t, U u) { t op u; }                                           \
		{                                                                                     \
			Array<std::remove_cvref_t<decltype(data[0] op that)>, count> result = {};         \
			for (umm i = 0; i < count; ++i)                                                   \
				result.data[i] = data[i] op that;                                             \
			return result;                                                                    \
		}                                                                                     \
		template <class U>                                                                    \
		friend forceinline constexpr auto operator op(U const &that, Array<T, count> const &self)         \
			requires requires(U u, T t) { u op t; }                                           \
		{                                                                                     \
			Array<std::remove_cvref_t<decltype(that op data[0])>, count> result = {};         \
			for (umm i = 0; i < count; ++i)                                                   \
				result.data[i] = that op self.data[i];                                        \
			return result;                                                                    \
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
	OP(<<)
	OP(>>)
	OP(&&)
	OP(||)
	#undef OP
};

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <umm count, class T>
forceinline constexpr Array<T, count> broadcast_to_array(T value) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = value;
	}
	return result;
}

template <class T, umm count>
forceinline constexpr umm count_of(Array<T, count> const &arr) {
	return count;
}

template <class T, umm count>
forceinline constexpr Span<T> as_span(Array<T, count> &arr) {
	return {arr.data, arr.count};
}

template <class T, umm count>
forceinline constexpr Array<T, count> to_array(T const (&array)[count]) {
	Array<T, count> result{};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = array[i];
	}
	return result;
}

template <class T, umm count>
forceinline constexpr bool all(Array<T, count> const &a) {
	for (umm i = 0; i < count; ++i) {
		if (!all(a.data[i])) {
			return false;
		}
	}
	return true;
}

template <class T, umm count>
forceinline constexpr T dot(Array<T, count> const &a, Array<T, count> const &b) {
	T result = 0;
	for (umm i = 0; i < count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

template <class T, class Index, umm count>
forceinline constexpr Array<T, count> gather(T const *pointer, Array<Index, count> const &indices) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = pointer[indices.data[i]];
	return r;
}

template <class T, class Mask, umm count>
forceinline constexpr Array<T, count> blend(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = (mask.data[i] >> (sizeof(Mask)*8-1)) ? a.data[i] : b.data[i];
	return r;
}

}
