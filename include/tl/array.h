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
	
	using IterOptions = ReverseIterOption;

	forceinline constexpr auto iter(this auto &&self, IterOptions options = {}) {
		return span_iter(self.data, self.data + count, options);
	}

	auto map(auto &&mapper) const
		requires requires { mapper(data[0]); }
	{
		Array<std::remove_cvref_t<decltype(mapper(data[0]))>, count> result = {};
		for (umm i = 0; i < count; ++i) {
			result.data[i] = mapper(data[i]);
		}
		return result;
	}

	template <class U>
	forceinline constexpr explicit operator Array<U, count>() const
		requires requires(T t) { (U)t; }
	{
		Array<U, count> result = {};
		for (umm i = 0; i < count; ++i)
			result.data[i] = (U)data[i];
		return result;
	};
};

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <class T, umm count>
inline static constexpr bool is_unsigned<Array<T, count>> = is_unsigned<T>;

#define OP(op)                                                                 \
	template <class T, umm count>                                              \
	forceinline constexpr auto operator op(Array<T, count> const &a)           \
		requires requires(T t) { op t; }                                       \
	{                                                                          \
		Array<std::remove_cvref_t<decltype(op a.data[0])>, count> result = {}; \
		for (umm i = 0; i < count; ++i)                                        \
			result.data[i] = op a.data[i];                                     \
		return result;                                                         \
	}
OP(+)
OP(-)
OP(~)
OP(!)
OP(*)
#undef OP
		
#define OP(op)                                                                                 \
	template <class T, umm count>                                                              \
	forceinline constexpr auto operator op(Array<T, count> const &a, Array<T, count> const &b) \
		requires requires(T t) { t op t; }                                                     \
	{                                                                                          \
		Array<std::remove_cvref_t<decltype(a.data[0] op b.data[0])>, count> result = {};       \
		for (umm i = 0; i < count; ++i)                                                        \
			result.data[i] = a.data[i] op b.data[i];                                           \
		return result;                                                                         \
	}                                                                                          \
	template <class U, class T, umm count>                                                     \
	forceinline constexpr auto operator op(Array<T, count> const &a, Array<U, count> const &b) \
		requires requires(T t, U u) { t op u; }                                                \
	{                                                                                          \
		Array<std::remove_cvref_t<decltype(a.data[0] op b.data[0])>, count> result = {};       \
		for (umm i = 0; i < count; ++i)                                                        \
			result.data[i] = a.data[i] op b.data[i];                                           \
		return result;                                                                         \
	}                                                                                          \
	template <class U, class T, umm count>                                       \
	forceinline constexpr auto operator op(Array<T, count> const &a, U const &b) \
		requires requires(T t, U u) { t op u; }                                  \
	{                                                                            \
		Array<std::remove_cvref_t<decltype(a.data[0] op b)>, count> result = {}; \
		for (umm i = 0; i < count; ++i)                                          \
			result.data[i] = a.data[i] op b;                                     \
		return result;                                                           \
	}                                                                            \
	template <class U, class T, umm count>                                       \
	forceinline constexpr auto operator op(U const &a, Array<T, count> const &b) \
		requires requires(U u, T t) { u op t; }                                  \
	{                                                                            \
		Array<std::remove_cvref_t<decltype(a op b.data[0])>, count> result = {}; \
		for (umm i = 0; i < count; ++i)                                          \
			result.data[i] = a op b.data[i];                                     \
		return result;                                                           \
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
		
#define OP(op)                                                                               \
	template <class U, class T, umm count>                                                   \
	forceinline constexpr auto &operator op##=(Array<T, count> &a, Array<U, count> const &b) \
		requires requires(T t, U u) { t op##= u; }                                           \
	{                                                                                        \
		for (umm i = 0; i < count; ++i)                                                      \
			a.data[i] op##= b.data[i];                                                       \
		return a;                                                                            \
	}                                                                                        \
	template <class U, class T, umm count>                                     \
	forceinline constexpr auto &operator op##=(Array<T, count> &a, U const &b) \
		requires requires(T t, U u) { t op##= u; }                             \
	{                                                                          \
		for (umm i = 0; i < count; ++i)                                        \
			a.data[i] op##= b;                                                 \
		return a;                                                              \
	}
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
#undef OP
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

template <class T, umm count>
forceinline constexpr Array<T, count> select(Array<bool, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = mask.data[i] ? a.data[i] : b.data[i];
	return r;
}

template <class U, class T, umm count>
	requires(count * sizeof(T) == count * sizeof(T) / sizeof(U) * sizeof(U))
forceinline constexpr Array<U, count * sizeof(T) / sizeof(U)> reinterpret(Array<T, count> const &a) {
	return std::bit_cast<Array<U, count * sizeof(T) / sizeof(U)>>(a);
}

template <class T, umm count, class Index, umm indices_count>
forceinline constexpr Array<T, count> shuffle(Array<T, count> const &a, Array<Index, indices_count> const &indices) {
	Array<T, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = a.data[indices.data[i % indices_count] % count];
	return r;
}

template <class T, umm count, class Index>
forceinline constexpr Array<T, count> shuffle(Array<T, count> const &a, Array<Index, count> const &indices) {
	Array<T, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = a.data[indices.data[i] % count];
	return r;
}

template <umm dups, class T, umm count>
forceinline constexpr Array<T, count * dups> dup(Array<T, count> const &a) {
	Array<T, count * dups> r;
	for (umm d = 0; d < dups; ++d)
		for (umm i = 0; i < count; ++i)
			r.data[d * count + i] = a.data[i];
	return r;
}

template <class T, umm count>
forceinline constexpr Array<T, count> reciprocal(Array<T, count> a) {
	for (umm i = 0; i < count; ++i)
		a.data[i] = 1 / a.data[i];
	return a;
}

template <class T, umm count>
forceinline constexpr Array<T, count> sqrt(Array<T, count> a) {
	for (umm i = 0; i < count; ++i)
		a.data[i] = sqrt(a.data[i]);
	return a;
}

template <class U, class T, umm count>
forceinline constexpr Array<U, count> element_cast(Array<T, count> a) {
	Array<U, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = (U)a.data[i];
	return r;
}


#if TL_USE_SIMD

template <class T, umm count>
	requires (count * sizeof(T) == 16)
forceinline constexpr Array<T, count> pshufb(Array<T, count> const &a, Array<s8, 16> const &indices) {
	return std::bit_cast<Array<T, count>>(_mm_shuffle_epi8(std::bit_cast<__m128i>(a), std::bit_cast<__m128i>(indices)));
}

template <class T, umm count>
	requires (count * sizeof(T) == 32)
forceinline constexpr Array<T, count> pshufb(Array<T, count> const &a, Array<s8, 32> const &indices) {
	return std::bit_cast<Array<T, count>>(_mm256_shuffle_epi8(std::bit_cast<__m256i>(a), std::bit_cast<__m256i>(indices)));
}

template <> forceinline constexpr Array<f32, 8> reciprocal(Array<f32, 8> a) { return std::bit_cast<Array<f32, 8>>(_mm256_rcp_ps(std::bit_cast<__m256>(a))); }

forceinline Array<f32, 8> min(Array<f32, 8> const &a, Array<f32, 8> const &b) { return std::bit_cast<Array<f32, 8>>(_mm256_min_ps(std::bit_cast<__m256>(a), std::bit_cast<__m256>(b))); }
forceinline Array<f32, 8> max(Array<f32, 8> const &a, Array<f32, 8> const &b) { return std::bit_cast<Array<f32, 8>>(_mm256_max_ps(std::bit_cast<__m256>(a), std::bit_cast<__m256>(b))); }

forceinline constexpr Array<f32, 8> operator+(Array<f32, 8> a, Array<f32, 8> b) { return std::bit_cast<Array<f32, 8>>(_mm256_add_ps(std::bit_cast<__m256>(a), std::bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator-(Array<f32, 8> a, Array<f32, 8> b) { return std::bit_cast<Array<f32, 8>>(_mm256_sub_ps(std::bit_cast<__m256>(a), std::bit_cast<__m256>(b))); }

#endif

}
