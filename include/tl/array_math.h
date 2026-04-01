#pragma once

#include "array.h"
#include "math.h"

namespace tl {

// SHIM_{#args}

#define SHIM_1(name)                                                \
	template <class T, umm count>                                   \
	forceinline constexpr Array<T, count> name(Array<T, count> a) { \
		for (umm i = 0; i < count; ++i)                             \
			a.data[i] = name(a.data[i]);                            \
		return a;                                                   \
	}

#define SHIM_2(name)                                                                   \
	template <class T, umm count>                                                      \
	forceinline constexpr Array<T, count> name(Array<T, count> a, Array<T, count> b) { \
		for (umm i = 0; i < count; ++i)                                                \
			a.data[i] = name(a.data[i], b.data[i]);                                    \
		return a;                                                                      \
	}                                                                                  \
	template <class T, umm count>                                                      \
	forceinline constexpr Array<T, count> name(T a, Array<T, count> b) {               \
		for (umm i = 0; i < count; ++i)                                                \
			a.data[i] = name(a, b.data[i]);                                            \
		return a;                                                                      \
	}                                                                                  \
	template <class T, umm count>                                                      \
	forceinline constexpr Array<T, count> name(Array<T, count> a, T b) {               \
		for (umm i = 0; i < count; ++i)                                                \
			a.data[i] = name(a.data[i], b);                                            \
		return a;                                                                      \
	}

SHIM_1(reciprocal);
SHIM_1(sqrt);
SHIM_1(absolute);
SHIM_1(sign);
SHIM_1(frac);
SHIM_2(frac);

#undef SHIM_1
#undef SHIM_2

#if TL_USE_SIMD


template <> forceinline constexpr Array<f32, 4> reciprocal(Array<f32, 4> a) { return bit_cast<Array<f32, 4>>(_mm_rcp_ps(bit_cast<__m128>(a))); }
#ifdef __AVX__
template <> forceinline constexpr Array<f32, 8> reciprocal(Array<f32, 8> a) { return bit_cast<Array<f32, 8>>(_mm256_rcp_ps(bit_cast<__m256>(a))); }
#endif
#ifdef __AVX512F__
template <> forceinline constexpr Array<f32, 16> reciprocal(Array<f32, 16> a) { return bit_cast<Array<f32, 16>>(_mm512_rcp14_ps(bit_cast<__m512>(a))); }
#endif


template <> forceinline constexpr Array<f32, 4> sqrt(Array<f32, 4> a) { return bit_cast<Array<f32, 4>>(_mm_sqrt_ps(bit_cast<__m128>(a))); }
#ifdef __AVX__
template <> forceinline constexpr Array<f32, 8> sqrt(Array<f32, 8> a) { return bit_cast<Array<f32, 8>>(_mm256_sqrt_ps(bit_cast<__m256>(a))); }
#endif
#ifdef __AVX512F__
template <> forceinline constexpr Array<f32, 16> sqrt(Array<f32, 16> a) { return bit_cast<Array<f32, 16>>(_mm512_sqrt_ps(bit_cast<__m256>(a))); }
#endif


template <> forceinline constexpr Array<f32, 4> rsqrt(Array<f32, 4> a) { return bit_cast<Array<f32, 4>>(_mm_rsqrt_ps(bit_cast<__m128>(a))); }
#ifdef __AVX__
template <> forceinline constexpr Array<f32, 8> rsqrt(Array<f32, 8> a) { return bit_cast<Array<f32, 8>>(_mm256_rsqrt_ps(bit_cast<__m256>(a))); }
#endif
#ifdef __AVX512F__
template <> forceinline constexpr Array<f32, 16> rsqrt(Array<f32, 16> a) { return bit_cast<Array<f32, 16>>(_mm512_rsqrt14_ps(bit_cast<__m512>(a))); }
#endif


#endif

}
