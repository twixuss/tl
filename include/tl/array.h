#pragma once
#include "common.h"

namespace tl {

template <class T, umm count_>
union Array {
	inline static constexpr umm count = count_;
	#include "_array_members.inc"
	struct { T x, y, z, w; };
	struct { v2 xy, zw; };
	struct { T _pad0; v2 yz; };
	struct { v3 xyz; };
	struct { T _pad1; v3 yzw; };
};

template <class T>
union Array<T, 1> {
	inline static constexpr umm count = 1;
	#include "_array_members.inc"
	T x;
};

template <class T>
union Array<T, 2> {
	inline static constexpr umm count = 2;
	#include "_array_members.inc"
	struct { T x, y; };
	forceinline constexpr v2 xx() const { return {x, x}; }
	forceinline constexpr v2 xy() const { return {x, y}; }
	forceinline constexpr v2 yx() const { return {y, x}; }
	forceinline constexpr v2 yy() const { return {y, y}; }
};

template <class T>
union Array<T, 3> {
	inline static constexpr umm count = 3;
	#include "_array_members.inc"
	struct { T x, y, z; };
	struct { v2 xy; };
	struct { T _pad0; v2 yz; };
	forceinline constexpr v2 xz() const { return {x,z}; }
	forceinline constexpr v2 yx() const { return {y,x}; }
	forceinline constexpr v2 zx() const { return {z,x}; }
	forceinline constexpr v2 zy() const { return {z,y}; }
	forceinline constexpr v3 yzx() const { return {y,z,x}; }
	forceinline constexpr v3 zxy() const { return {z,x,y}; }
	forceinline constexpr v3 xzy() const { return {x,z,y}; }
	forceinline constexpr v3 zyx() const { return {z,y,x}; }
};

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <class T, umm count>
inline static constexpr bool is_unsigned<Array<T, count>> = is_unsigned<T>;

template <class T>            inline static constexpr int array_nestedness = 0;
template <class T, umm count> inline static constexpr int array_nestedness<Array<T, count>> = array_nestedness<T> + 1;

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
	}                                                                                           \
	template <class U, class T, umm count>                                                      \
	forceinline constexpr auto operator op(Array<T, count> const &a, U const &b)                \
		requires requires(T t, U u) { t op u; } && (array_nestedness<U> <= array_nestedness<T>) \
	{                                                                                           \
		Array<std::remove_cvref_t<decltype(a.data[0] op b)>, count> result = {};                \
		for (umm i = 0; i < count; ++i)                                                         \
			result.data[i] = a.data[i] op b;                                                    \
		return result;                                                                          \
	}                                                                                           \
	template <class U, class T, umm count>                                                      \
	forceinline constexpr auto operator op(U const &a, Array<T, count> const &b)                \
		requires requires(U u, T t) { u op t; } && (array_nestedness<U> <= array_nestedness<T>) \
	{                                                                                           \
		Array<std::remove_cvref_t<decltype(a op b.data[0])>, count> result = {};                \
		for (umm i = 0; i < count; ++i)                                                         \
			result.data[i] = a op b.data[i];                                                    \
		return result;                                                                          \
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

template <int shift, class T, umm count>
forceinline constexpr Array<T, count> shift_left(Array<T, count> arr) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = arr.data[i] << shift;
	}
	return result;
}

template <int shift, class T, umm count>
forceinline constexpr Array<T, count> shift_right(Array<T, count> arr) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = arr.data[i] >> shift;
	}
	return result;
}

template <umm count, class T>
forceinline constexpr Array<T, count> broadcast_to_array(T value) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = value;
	}
	return result;
}

template <class T, umm count>
forceinline constexpr umm count_of(Array<T, count> const &) {
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
	T result = {};
	for (umm i = 0; i < count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

template <class T>
forceinline constexpr Array<T, 3> cross(Array<T, 3> const &a, Array<T, 3> const &b) {
	return a.yzx() * b.zxy() - a.zxy() * b.yzx();
}

template <class T, class Index, umm count>
forceinline constexpr Array<T, count> gather(T const *pointer, Array<Index, count> const &indices) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = pointer[indices.data[i]];
	return r;
}

#define OP(NAME, OPERATOR)                                                         \
	template <class T>                                                             \
	forceinline constexpr auto mask_##NAME(T a, T b) {                             \
		static_assert(sizeof(UintWithBits<sizeof(T)*8>) == sizeof(T));             \
		return (UintWithBits<sizeof(T)*8>)(a OPERATOR b ? -1 : 0);                 \
	}                                                                              \
	template <class T, umm count>                                                  \
	forceinline constexpr auto mask_##NAME(Array<T, count> a, Array<T, count> b) { \
		Array<UintWithBits<sizeof(T)*8>, count> r = {};                            \
		static_assert(sizeof(r) == sizeof(Array<T, count>));                       \
		for (umm i = 0; i < count; ++i)                                            \
			r.data[i] = mask_##NAME(a.data[i], b.data[i]);                         \
		return r;                                                                  \
	}

OP(equal, ==)
OP(not_equal, !=)
OP(less, <)
OP(less_equal, <=)
OP(greater, >)
OP(greater_equal, >=)

#undef OP

// Mask is not an integral, e.g. Array
template <class T, class Mask, umm count>
forceinline constexpr Array<T, count> blend(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = blend(mask.data[i], a.data[i], b.data[i]);
	return r;
}

// Mask is an integral
template <class T, std::integral Mask, umm count>
	requires (sizeof(T) == sizeof(Mask))
forceinline constexpr Array<T, count> blend(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	using MaskInt = UintWithBits<sizeof(Mask) * 8>;
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i) {
		MaskInt mask_int = bit_cast<MaskInt>(mask.data[i]);
		r.data[i] = (mask_int >> (sizeof(Mask)*8-1)) ? a.data[i] : b.data[i];
	}
	return r;
}


template <class Mask, class T, umm count>
forceinline constexpr Array<T, count> select(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = select(mask.data[i], a.data[i], b.data[i]);
	return r;
}

template <class U, class T, umm count>
	requires(count * sizeof(T) == count * sizeof(T) / sizeof(U) * sizeof(U) && !std::is_same_v<U, T>)
forceinline constexpr Array<U, count * sizeof(T) / sizeof(U)> reinterpret(Array<T, count> const &a) {
	return bit_cast<Array<U, count * sizeof(T) / sizeof(U)>>(a);
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

template <class U, class T, umm count>
forceinline constexpr Array<U, count> element_cast(Array<T, count> a) {
	Array<U, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = convert<U>(a.data[i]);
	return r;
}


#if TL_USE_SIMD

forceinline constexpr Array<f32, 4> operator+(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_add_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline constexpr Array<f32, 4> operator-(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_sub_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline constexpr Array<f32, 4> operator*(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_mul_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline constexpr Array<f32, 4> operator/(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_div_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }

template <int shift> forceinline constexpr Array<u32, 4> shift_left(Array<u32, 4> arr) { return bit_cast<Array<u32, 4>>(_mm_slli_epi32(bit_cast<__m128i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 4> shift_left(Array<s32, 4> arr) { return bit_cast<Array<s32, 4>>(_mm_slli_epi32(bit_cast<__m128i>(arr), shift)); }
template <int shift> forceinline constexpr Array<u32, 4> shift_right(Array<u32, 4> arr) { return bit_cast<Array<u32, 4>>(_mm_srli_epi32(bit_cast<__m128i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 4> shift_right(Array<s32, 4> arr) { return bit_cast<Array<s32, 4>>(_mm_srai_epi32(bit_cast<__m128i>(arr), shift)); }

template <>
forceinline auto lerp(Array<f32, 4> a_, Array<f32, 4> b_, Array<f32, 4> t_) {
	__m128 a = bit_cast<__m128>(a_);
	__m128 b = bit_cast<__m128>(b_);
	__m128 t = bit_cast<__m128>(t_);

	#ifdef __AVX__
	__m128 r = _mm_fmadd_ps(_mm_sub_ps(b, a), t, a);
	#else
	__m128 r = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b, a), t), a);
	#endif

	return bit_cast<Array<f32, 4>>(r);
}

forceinline Array<f32, 4> min(Array<f32, 4> const &a, Array<f32, 4> const &b) { return bit_cast<Array<f32, 4>>(_mm_min_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline Array<f32, 4> max(Array<f32, 4> const &a, Array<f32, 4> const &b) { return bit_cast<Array<f32, 4>>(_mm_max_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }

template<class T>
	requires (sizeof(T) == 1)
forceinline Array<T, 16> blend(Array<u8, 16> mask, Array<T, 16> a, Array<T, 16> b) {
	return bit_cast<Array<T, 16>>(_mm_blendv_epi8(bit_cast<__m128i>(b), bit_cast<__m128i>(a), bit_cast<__m128i>(mask)));
}

template<class T>
	requires (sizeof(T) == 4)
forceinline Array<T, 4> blend(Array<u32, 4> mask, Array<T, 4> a, Array<T, 4> b) {
	return bit_cast<Array<T, 4>>(_mm_blendv_ps(bit_cast<__m128>(b), bit_cast<__m128>(a), bit_cast<__m128>(mask)));
}

template<class T>
	requires (sizeof(T) == 8)
forceinline Array<T, 2> blend(Array<u64, 2> mask, Array<T, 2> a, Array<T, 2> b) {
	return bit_cast<Array<T, 2>>(_mm_blendv_pd(bit_cast<__m128d>(b), bit_cast<__m128d>(a), bit_cast<__m128d>(mask)));
}

template <class T, umm count>
	requires (count * sizeof(T) == 16)
forceinline constexpr Array<T, count> pshufb(Array<T, count> const &a, Array<s8, 16> const &indices) {
	return bit_cast<Array<T, count>>(_mm_shuffle_epi8(bit_cast<__m128i>(a), bit_cast<__m128i>(indices)));
}

#ifdef __AVX__

forceinline constexpr Array<f32, 8> operator+(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_add_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator-(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_sub_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator*(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_mul_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator/(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_div_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }

template <>
forceinline auto lerp(Array<f32, 8> a_, Array<f32, 8> b_, Array<f32, 8> t_) {
	__m256 a = bit_cast<__m256>(a_);
	__m256 b = bit_cast<__m256>(b_);
	__m256 t = bit_cast<__m256>(t_);

	__m256 r = _mm256_fmadd_ps(_mm256_sub_ps(b, a), t, a);

	return bit_cast<Array<f32, 8>>(r);
}

forceinline Array<f32, 8> min(Array<f32, 8> const &a, Array<f32, 8> const &b) { return bit_cast<Array<f32, 8>>(_mm256_min_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline Array<f32, 8> max(Array<f32, 8> const &a, Array<f32, 8> const &b) { return bit_cast<Array<f32, 8>>(_mm256_max_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }

#endif

#ifdef __AVX2__

template <int shift> forceinline constexpr Array<u32, 8> shift_left(Array<u32, 8> arr) { return bit_cast<Array<u32, 8>>(_mm256_slli_epi32(bit_cast<__m256i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 8> shift_left(Array<s32, 8> arr) { return bit_cast<Array<s32, 8>>(_mm256_slli_epi32(bit_cast<__m256i>(arr), shift)); }
template <int shift> forceinline constexpr Array<u32, 8> shift_right(Array<u32, 8> arr) { return bit_cast<Array<u32, 8>>(_mm256_srli_epi32(bit_cast<__m256i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 8> shift_right(Array<s32, 8> arr) { return bit_cast<Array<s32, 8>>(_mm256_srai_epi32(bit_cast<__m256i>(arr), shift)); }

forceinline constexpr Array<u32, 4> shift_left(Array<u32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<u32, 4>>(_mm_sllv_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }
forceinline constexpr Array<s32, 4> shift_left(Array<s32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<s32, 4>>(_mm_sllv_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }
forceinline constexpr Array<u32, 4> shift_right(Array<u32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<u32, 4>>(_mm_srlv_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }
forceinline constexpr Array<s32, 4> shift_right(Array<s32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<s32, 4>>(_mm_srav_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }

forceinline constexpr Array<u32, 8> shift_left(Array<u32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<u32, 8>>(_mm256_sllv_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }
forceinline constexpr Array<s32, 8> shift_left(Array<s32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<s32, 8>>(_mm256_sllv_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }
forceinline constexpr Array<u32, 8> shift_right(Array<u32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<u32, 8>>(_mm256_srlv_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }
forceinline constexpr Array<s32, 8> shift_right(Array<s32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<s32, 8>>(_mm256_srav_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }

template<class T>
	requires (sizeof(T) == 1)
forceinline Array<T, 32> blend(Array<u8, 32> mask, Array<T, 32> a, Array<T, 32> b) {
	return bit_cast<Array<T, 32>>(_mm256_blendv_epi8(bit_cast<__m256i>(b), bit_cast<__m256i>(a), bit_cast<__m256i>(mask)));
}

template<class T>
	requires (sizeof(T) == 4)
forceinline Array<T, 8> blend(Array<u32, 8> mask, Array<T, 8> a, Array<T, 8> b) {
	return bit_cast<Array<T, 8>>(_mm256_blendv_ps(bit_cast<__m256>(b), bit_cast<__m256>(a), bit_cast<__m256>(mask)));
}

template<class T>
	requires (sizeof(T) == 8)
forceinline Array<T, 4> blend(Array<u64, 4> mask, Array<T, 4> a, Array<T, 4> b) {
	return bit_cast<Array<T, 4>>(_mm256_blendv_pd(bit_cast<__m256d>(b), bit_cast<__m256d>(a), bit_cast<__m256d>(mask)));
}

template <class T, umm count>
	requires (count * sizeof(T) == 32)
forceinline constexpr Array<T, count> pshufb(Array<T, count> const &a, Array<s8, 32> const &indices) {
	return bit_cast<Array<T, count>>(_mm256_shuffle_epi8(bit_cast<__m256i>(a), bit_cast<__m256i>(indices)));
}

#endif

#endif

}
