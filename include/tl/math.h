#pragma once
#include "common.h"
#include "array.h"
#include "simd_macros.h"
#include "string.h"
#include "vector.h"
#include "static_list.h"

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif

#include <float.h>
#include <math.h>
#include <memory.h>
#include <utility>

#if COMPILER_MSVC
#pragma warning(pop)
#endif


#if COMPILER_MSVC
#pragma warning(push)
//#pragma warning(disable : 5045) // spectre
//#pragma warning(disable : 4146) // unsigned unary minus
//#pragma warning(disable : 4201) // unnamed struct
//#pragma warning(disable : 4820) // padding
#endif

namespace tl {

template <class M, class T> forceinline T &mask_assign(M mask, T &dst, T src) { return dst = select(mask, src, dst); }
template <class T> forceinline constexpr auto pow2(T v) { return v * v; }
template <class T> forceinline constexpr auto pow3(T v) { return v * v * v; }
template <class T> forceinline constexpr auto pow4(T v) { return pow2(v * v); }

namespace cpp_workarounds {

// You can't just specialize the function, you have to do this integral_constant shit.
template <smm power, class T> 
forceinline constexpr auto pow(T v, std::integral_constant<smm, power>) {
	if constexpr (power / 2 * 2 == power) {
		return pow<2>(pow<power/2>(v));
	} else {
		return pow<2>(pow<power/2>(v)) * v;
	}
}

template <class T> forceinline constexpr auto pow(T v, std::integral_constant<smm, 0>) { return convert<T>(1); }
template <class T> forceinline constexpr auto pow(T v, std::integral_constant<smm, 1>) { return v; }
template <class T> forceinline constexpr auto pow(T v, std::integral_constant<smm, 2>) { return v*v; }

}

template <smm power, class T> 
forceinline constexpr auto pow(T v) {
	return cpp_workarounds::pow<power>(v, {});
}

template <class T>
forceinline constexpr T smooth_min(T a, T b, f32 k) {
	f32 h = clamp<f32>((b - a) / k + .5f, 0, 1);
	return b + h * (a - b + k * 0.5f * (h - 1));
}

template <class T>
forceinline constexpr T smooth_max(T a, T b, f32 k) {
	return smooth_min(a, b, -k);
}

union m3;
union m4;

forceinline constexpr m4 M4();
forceinline constexpr m4 M4(v4f i, v4f j, v4f k, v4f l);
forceinline constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw);

forceinline void cos_sin(f32 v, f32& cos_out, f32& sin_out);
forceinline void cos_sin(v2f v, v2f& cos_out, v2f& sin_out);
forceinline void cos_sin(v3f v, v3f& cos_out, v3f& sin_out);
forceinline void cos_sin(v4f v, v4f& cos_out, v4f& sin_out);

union m2 {
	using Scalar = f32;
	using Vector = v2f;
	struct {
		v2f i, j;
	};
	f32 s[4];
	forceinline v2f operator*(v2f b) const {
		v2f x = V2f(b.x) * i;
		v2f y = V2f(b.y) * j;
		return x + y;
	}
	forceinline m2 operator*(m2 b) const { return {*this * b.i, *this * b.j}; }
	forceinline m2& operator*=(m2 b) { return *this = *this * b; }
	static forceinline m2 identity() {
		return {
			1, 0,
			0, 1,
		};
	}
	static forceinline m2 scale(f32 x, f32 y) {
		return {
			x, 0,
			0, y,
		};
	}
	static forceinline m2 scale(v2f v) { return scale(v.x, v.y); }
	static forceinline m2 scale(f32 v) { return scale(v, v); }
	static forceinline m2 rotation(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			c, s,
		   -s, c,
		};
	}
};

template<> inline constexpr v2f min_value<v2f> = {min_value<v2f::Scalar>, min_value<v2f::Scalar>};
template<> inline constexpr v2f max_value<v2f> = {max_value<v2f::Scalar>, max_value<v2f::Scalar>};

template<> inline constexpr v2s min_value<v2s> = {min_value<v2s::Scalar>, min_value<v2s::Scalar>};
template<> inline constexpr v2s max_value<v2s> = {max_value<v2s::Scalar>, max_value<v2s::Scalar>};

template<> inline constexpr v2u min_value<v2u> = {min_value<v2u::Scalar>, min_value<v2u::Scalar>};
template<> inline constexpr v2u max_value<v2u> = {max_value<v2u::Scalar>, max_value<v2u::Scalar>};

union m3 {
	using Scalar = f32;
	using Vector = v3f;
	struct {
		v3f i, j, k;
	};
	f32 s[9];
	forceinline v3f operator*(v3f b) const {
		v3f x = V3f(b.x) * i;
		v3f y = V3f(b.y) * j;
		v3f z = V3f(b.z) * k;
		return x + y + z;
	}
	forceinline m3 operator*(m3 b) const { return {*this * b.i, *this * b.j, *this * b.k}; }
	forceinline m3& operator*=(m3 b) { return *this = *this * b; }
	static forceinline m3 identity() {
		return {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,
		};
	}
	static forceinline m3 scale(f32 x, f32 y, f32 z) {
		return {
			x, 0, 0,
			0, y, 0,
			0, 0, z,
		};
	}
	static forceinline m3 scale(v3f v) { return scale(v.x, v.y, v.z); }
	static forceinline m3 scale(v2f xy, f32 z) { return scale(xy.x, xy.y, z); }
	static forceinline m3 scale(f32 v) { return scale(v, v, v); }
	static forceinline m3 translation(f32 x, f32 y) {
		return {
			1, 0, 0,
			0, 1, 0,
			x, y, 1,
		};
	}
	static forceinline m3 translation(v2f v) { return translation(v.x, v.y); }
	static forceinline m3 translation(f32 v) { return translation(v, v); }
	static forceinline m3 rotation_r_x(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			1, 0, 0,
			0, c, s,
			0,-s, c,
		};
	}
	static forceinline m3 rotation_r_y(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			c, 0,-s,
			0, 1, 0,
		    s, 0, c,
		};
	}
	static forceinline m3 rotation_r_z(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			 c, s, 0,
			-s, c, 0,
			 0, 0, 1,
		};
	}
	static forceinline m3 rotation_r_zxy(v3f a) {
		v3f s;
		v3f c;
		cos_sin(a, c, s);
		return {
			 c.z*c.y + s.z*s.x*s.y, s.z*c.x,  c.z*-s.y + s.z*s.x*c.y,
			-s.z*c.y + c.z*s.x*s.y, c.z*c.x, -s.z*-s.y + c.z*s.x*c.y,
			               c.x*s.y,    -s.x,                 c.x*c.y,
		};
	}
	static forceinline m3 rotation_r_zxy(f32 x, f32 y, f32 z) { return rotation_r_zxy({x, y, z}); }
	static forceinline m3 rotation_r_yxz(v3f a) {
		v3f s;
		v3f c;
		cos_sin(a, c, s);
		return {
			c.y*c.z + -s.y*-s.x*-s.z, c.y*s.z + -s.y*-s.x*c.z, -s.y*c.x,
			                c.x*-s.z,                 c.x*c.z,      s.x,
			 s.y*c.z + c.y*-s.x*-s.z,  s.y*s.z + c.y*-s.x*c.z,  c.y*c.x,
		};
	}
	static forceinline m3 rotation_r_yxz(f32 x, f32 y, f32 z) { return rotation_r_yxz({x, y, z}); }
};

#define C(v2f, v2s, s32) template<> constexpr v2s convert(v2f v) { return {convert<s32>(v.x), convert<s32>(v.y)}; }
C(v2f, v2s, s32) C(v2f, v2u, u32)
C(v2s, v2f, f32) C(v2s, v2u, u32)
C(v2u, v2f, f32) C(v2u, v2s, s32)
#undef C

#define C(v3f, v3s, s32) template<> constexpr v3s convert(v3f v) { return {convert<s32>(v.x), convert<s32>(v.y), convert<s32>(v.z)}; }
C(v3f, v3s, s32) C(v3f, v3u, u32)
C(v3s, v3f, f32) C(v3s, v3u, u32)
C(v3u, v3f, f32) C(v3u, v3s, s32)
#undef C

#define C(v4f, v4s, s32) template<> constexpr v4s convert(v4f v) { return {convert<s32>(v.x), convert<s32>(v.y), convert<s32>(v.z), convert<s32>(v.w)}; }
C(v4f, v4s, s32) C(v4f, v4u, u32)
C(v4s, v4f, f32) C(v4s, v4u, u32)
C(v4u, v4f, f32) C(v4u, v4s, s32)
#undef C

template <> forceinline constexpr v2f convert(f32 v) { return V2f(v); }
template <> forceinline constexpr v3f convert(f32 v) { return V3f(v); }
template <> forceinline constexpr v2f convert(s32 v) { return V2f((f32)v); }
template <> forceinline constexpr v3f convert(s32 v) { return V3f((f32)v); }
template <> forceinline constexpr v2f convert(u32 v) { return V2f((f32)v); }
template <> forceinline constexpr v3f convert(u32 v) { return V3f((f32)v); }

#define SHUFFLE(a, s0, s1, b, s2, s3) _mm_shuffle_ps(a, b, _MM_SHUFFLE(s3, s2, s1, s0))

forceinline v2f pack(v2f v) { return v; }
forceinline v3f pack(v3f v) { return v; }
forceinline v4f pack(v4f v) { return v; }
forceinline v2f unpack(v2f v) { return v; }
forceinline v3f unpack(v3f v) { return v; }
forceinline v4f unpack(v4f v) { return v; }

#if 0

#if ARCH_AVX2
forceinline void pack_32x4(any32x4 &x, any32x4 &y) {
	any32x8 c = combine_m16(x, y);
    c = _mm256_permutevar8x32_ps(c, _mm256_setr_epi32(0,2,4,6,1,3,5,7));
	x = m32_get_half(c, 0);
	y = m32_get_half(c, 1);
}
#else
forceinline void pack_32x4(any32x4 &x, any32x4 &y) {
	v2fx4 r;
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 2];
		}
	}
}
#endif

forceinline void pack_32x4(any32x4 &_x, any32x4 &_y, any32x4 &_z) {
	s32 src[12];
	s32 dst[12];

	memcpy(src +  0, &_x, sizeof(_x));
	memcpy(src + 16, &_y, sizeof(_y));
	memcpy(src + 32, &_z, sizeof(_z));

	for (u32 i = 0; i < 3; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 3];
		}
	}
}
forceinline v4fx4 pack(v4fx4 v) {
	v4fx4 r;
#if ARCH_AVX2
	__m256 &v0 = ((__m256 *)&v)[0];
	__m256 &v1 = ((__m256 *)&v)[1];
	__m256 &r0 = ((__m256 *)&r)[0];
	__m256 &r1 = ((__m256 *)&r)[1];
	__m256i idx = _mm256_setr_epi32(0,4,1,5,2,6,3,7);

    r1 = _mm256_unpacklo_ps(v0, v1);
    r0 = _mm256_unpackhi_ps(v0, v1);
    v1 = _mm256_permutevar8x32_ps(r1, idx);
    v0 = _mm256_permutevar8x32_ps(r0, idx);
    r0 = v1;
    r1 = v0;

#else
	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 4];
			r.s[i * 4 + j]  = v.s[i + j * 4];
			r.s[i * 4 + j]  = v.s[i + j * 4];
			r.s[i * 4 + j]  = v.s[i + j * 4];
		}
	}
#endif
	return r;
}
forceinline v2fx8 pack(v2fx8 v) {
	v2fx8 r;
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 2];
		}
	}
	return r;
}
forceinline v3fx8 pack(v3fx8 v) {
	v3fx8 r;
	for (u32 i = 0; i < 3; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 3];
		}
	}
	return r;
}
forceinline v4fx8 pack(v4fx8 v) {
	v4fx8 r;
	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 4];
		}
	}
	return r;
}
forceinline v2fx4 unpack(v2fx4 v) {
	v2fx4 r;
	r.x.ps = _mm_unpacklo_ps(v.x.ps, v.y.ps);
	r.y.ps = _mm_unpackhi_ps(v.x.ps, v.y.ps);
	return r;
}
forceinline v3fx4 unpack(v3fx4 v) {
	auto xy = SHUFFLE(v.x.ps, 0, 2, v.y.ps, 0, 2);
	auto yz = SHUFFLE(v.y.ps, 1, 3, v.z.ps, 1, 3);
	auto xz = SHUFFLE(v.x.ps, 1, 3, v.z.ps, 0, 2);
	v.x.ps = SHUFFLE(xy, 0, 2, xz, 2, 0);
	v.y.ps = SHUFFLE(yz, 0, 2, xy, 1, 3);
	v.z.ps = SHUFFLE(xz, 3, 1, yz, 1, 3);
	return v;
}
forceinline v4fx4 unpack(v4fx4 v) {
	return pack(v);
}
forceinline v2fx8 unpack(v2fx8 v) {
	v2fx8 r;
#if ARCH_AVX
	__m256 m02 = _mm256_unpacklo_ps(v.x.ps, v.y.ps);
	__m256 m13 = _mm256_unpackhi_ps(v.x.ps, v.y.ps);
	r.x.ps = _mm256_setr_m128(_mm256_extractf128_ps(m02, 0), _mm256_extractf128_ps(m13, 0));
	r.y.ps = _mm256_setr_m128(_mm256_extractf128_ps(m02, 1), _mm256_extractf128_ps(m13, 1));
#else
	for (u32 i = 0; i < 8; ++i){
		for (u32 j = 0; j < 2; ++j){
			r.s[i * 2 + j] = v.s[i + j * 8];
		}
	}
#endif
	return r;
}
#if ARCH_AVX
#define UNPACK(v3fx8)													 \
forceinline v3fx8 unpack(v3fx8 v) {										 \
	auto xy = _mm256_shuffle_ps(v.x.ps, v.y.ps, MM_SHUFFLE(0, 2, 0, 2)); \
	auto yz = _mm256_shuffle_ps(v.y.ps, v.z.ps, MM_SHUFFLE(1, 3, 1, 3)); \
	auto xz = _mm256_shuffle_ps(v.x.ps, v.z.ps, MM_SHUFFLE(1, 3, 0, 2)); \
	auto rx = _mm256_shuffle_ps(xy, xz, MM_SHUFFLE(0, 2, 2, 0));		 \
	auto ry = _mm256_shuffle_ps(yz, xy, MM_SHUFFLE(0, 2, 1, 3));		 \
	auto rz = _mm256_shuffle_ps(xz, yz, MM_SHUFFLE(3, 1, 1, 3));		 \
	v.x.ps = _mm256_permute2f128_ps(rx, ry, MM256_PERM128(0, 2));		 \
	v.y.ps = _mm256_permute2f128_ps(rz, rx, MM256_PERM128(0, 3));		 \
	v.z.ps = _mm256_permute2f128_ps(ry, rz, MM256_PERM128(1, 3));		 \
	return v;															 \
}
#else
#define UNPACK(v3fx8)						 \
forceinline v3fx8 unpack(v3fx8 v) {			 \
	v3fx8 r;								 \
	for (u32 i = 0; i < 8; ++i) {			 \
		for (u32 j = 0; j < 3; ++j){		 \
			r.s[i * 3 + j] = v.s[i + j * 8]; \
		}									 \
	}										 \
	return r;								 \
}
#endif
UNPACK(v3fx8) UNPACK(v3sx8) UNPACK(v3ux8)
#undef UNPACK
forceinline v4fx8 unpack(v4fx8 v) {
	v4fx8 r;
	for (u32 i = 0; i < 8; ++i) {
		for (u32 j = 0; j < 4; ++j){
			r.s[i * 4 + j] = v.s[i + j * 8];
		}
	}
	return r;
}

#endif


#define VECTOR_REDUCE_FUNC(func) \
	template <class Scalar> forceinline auto func(v2<Scalar> a) { return func(a.x, a.y); } \
	template <class Scalar> forceinline auto func(v3<Scalar> a) { return func(a.x, a.y, a.z); } \
	template <class Scalar> forceinline auto func(v4<Scalar> a) { return func(a.x, a.y, a.z, a.w); }

#define VECTOR_COMPONENTWISE_FUNC_V(func) \
	template <class Scalar> forceinline v2<Scalar> func(v2<Scalar> a) { return {func(a.x), func(a.y)}; } \
	template <class Scalar> forceinline v3<Scalar> func(v3<Scalar> a) { return {func(a.x), func(a.y), func(a.z)}; } \
	template <class Scalar> forceinline v4<Scalar> func(v4<Scalar> a) { return {func(a.x), func(a.y), func(a.z), func(a.w)}; }

#define VECTOR_COMPONENTWISE_FUNC_VV(func) \
	template <class Scalar> forceinline v2<Scalar> func(v2<Scalar> a, v2<Scalar> b) { return {func(a.x, b.x), func(a.y, b.y)}; } \
	template <class Scalar> forceinline v3<Scalar> func(v3<Scalar> a, v3<Scalar> b) { return {func(a.x, b.x), func(a.y, b.y), func(a.z, b.z)}; } \
	template <class Scalar> forceinline v4<Scalar> func(v4<Scalar> a, v4<Scalar> b) { return {func(a.x, b.x), func(a.y, b.y), func(a.z, b.z), func(a.w, b.w)}; }

VECTOR_REDUCE_FUNC(min);
VECTOR_REDUCE_FUNC(max);
VECTOR_COMPONENTWISE_FUNC_VV(min);
VECTOR_COMPONENTWISE_FUNC_VV(max);

#define HALF(f32) forceinline f32 half(f32 v) { return v * 0.5f; }
HALF(f32)
HALF(v2f)
HALF(v3f)
HALF(v4f)
#undef HALF

#define HALF(s32) forceinline s32 half(s32 v) { return v / 2; }
HALF(s32)
HALF(v2s)
HALF(v3s)
HALF(v4s)
#undef HALF

#define HALF(u32) forceinline u32 half(u32 v) { return v >> 1; }
HALF(u32)
HALF(v2u)
HALF(v3u)
HALF(v4u)
#undef HALF

VECTOR_COMPONENTWISE_FUNC_V(floor);
VECTOR_COMPONENTWISE_FUNC_VV(floor);
VECTOR_COMPONENTWISE_FUNC_V(floor_to_power_of_2);
VECTOR_COMPONENTWISE_FUNC_V(frac);

#if 1
// 2.8x faster in a simple benchmark
// Change `#if 1` above to 0 to use default version.
forceinline v2s frac(v2s v, v2s s) {
	__m128i vm = _mm_setr_epi32(v.x, v.y, 0, 0);
	__m128i sm = _mm_setr_epi32(s.x, s.y, 0, 0);
	__m128d vlo = _mm_cvtepi32_pd(vm);
	__m128d slo = _mm_cvtepi32_pd(sm);
	__m128i result = _mm_cvttpd_epi32(_mm_floor_pd(_mm_div_pd(vlo, slo)));
	result = _mm_sub_epi32(vm, _mm_mullo_epi32(result, sm));
	return *(v2s *)&result;
}
#endif

forceinline v2s frac(v2s v, s32 s) { return frac(v, V2s(s)); }
forceinline v3s frac(v3s v, s32 s) { return frac(v, V3s(s)); }
forceinline v4s frac(v4s v, s32 s) { return frac(v, V4s(s)); }

#define FLOOR(v2s, s32, V2s) 		   \
	forceinline v2s floor(v2s v, s32 s) { return floor(v, V2s(s)); }
FLOOR(v2s, s32, V2s)
FLOOR(v2u, u32, V2u)
#undef FLOOR

#define FLOOR(v3s, s32, V3s) 		  \
	forceinline v3s floor(v3s v, s32 s) { return floor(v, V3s(s)); }
FLOOR(v3s, s32, V3s)
FLOOR(v3u, u32, V3u)
#undef FLOOR

#define FLOOR(v4s, s32, V4s) 		  \
	forceinline v4s floor(v4s v, s32 s) { return floor(v, V4s(s)); }
FLOOR(v4s, s32, V4s)
FLOOR(v4u, u32, V4u)
#undef FLOOR

forceinline v2s floor_to_int(v2f v) {
	return {
		floor_to_int(v.x),
		floor_to_int(v.y),
	};
}
forceinline v3s floor_to_int(v3f v) {
	return {
		floor_to_int(v.x),
		floor_to_int(v.y),
		floor_to_int(v.z),
	};
}
forceinline v4s floor_to_int(v4f v) {
	return {
		floor_to_int(v.x),
		floor_to_int(v.y),
		floor_to_int(v.z),
		floor_to_int(v.w),
	};
}

forceinline v2f ceil(v2f v) { return {ceil(v.x), ceil(v.y)}; }
forceinline v3f ceil(v3f v) { return {ceil(v.x), ceil(v.y), ceil(v.z)}; }
forceinline v4f ceil(v4f v) { return {ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)}; }

#define CEIL(t) forceinline t ceil(t v, t s) { return floor(v + s - 1, s); }
CEIL(v2s) CEIL(v3s) CEIL(v4s);
#undef CEIL

#define CEIL(s32, f32) forceinline s32 ceil_to_int(f32 v) { return (s32)ceil(v); }
CEIL(v2s, v2f)
CEIL(v3s, v3f)
CEIL(v4s, v4f)
#undef CEIL

forceinline v2f round(v2f v) { return {round(v.x), round(v.y)}; }
forceinline v3f round(v3f v) { return {round(v.x), round(v.y), round(v.z)}; }
forceinline v4f round(v4f v) { return {round(v.x), round(v.y), round(v.z), round(v.w)}; }

forceinline v2s round_to_int(v2f v) { return {round_to_int(v.x), round_to_int(v.y)}; }
forceinline v3s round_to_int(v3f v) { return {round_to_int(v.x), round_to_int(v.y), round_to_int(v.z)}; }
forceinline v4s round_to_int(v4f v) { return {round_to_int(v.x), round_to_int(v.y), round_to_int(v.z), round_to_int(v.w)}; }

forceinline v2s round_to_int(v2f v, v2s step) { return round_to_int(v / (v2f)step) * step; }
forceinline v3s round_to_int(v3f v, v3s step) { return round_to_int(v / (v3f)step) * step; }
forceinline v4s round_to_int(v4f v, v4s step) { return round_to_int(v / (v4f)step) * step; }

template <class Int, class T>
forceinline constexpr auto lerp_int(Int a, Int b, T t) {
	auto r = lerp((T)a, (T)b, t);
	if (a > b) {
		return floor_to_int(r);
	} else {
		return ceil_to_int(r);
	}
}

template <class T, class Float>
forceinline constexpr auto bezier(T a, T b, T c, Float t) {
	return a + t * (-2*a + 2*b + t * (a - 2*b + c));
}
template <class T, class Float>
forceinline constexpr auto bezier(T a, T b, T c, T d, Float t) {
	return a + t * (-3*a + 3*b + t * (3*a - 6*b + 3*c + t * (-a + 3*b - 3*c + d)));
}
template <class T, class Float>
forceinline constexpr auto bezier_derivative(T a, T b, T c, T d, Float t) {
	return -3*a + 3*b + t * (6*a - 12*b + 6*c + 3 * t * (-a + 3*b - 3*c + d));
}

// ax^3 + bx^2 + cx + d
template <class T>
struct Cubic {
	T a, b, c, d;
};

template <class T>
forceinline constexpr T evaluate(Cubic<T> c, T x) {
	T x2 = x*x;
	T x3 = x2*x;
	return c.a*x3 + c.b*x2 + c.c*x + c.d;
}

template <class T>
forceinline constexpr T derivative(Cubic<T> c, T x) {
	T x2 = x*x;
	return 3*c.a*x2 + 2*c.b*x + c.c;
}

template <class T>
Cubic<T> cubic_2y2d(T y0, T y1, T d0, T d1) {
    T r1 = y1 - y0 - convert<T>(0);
    T r2 = d1 - d0;
    T a = r2 - convert<T>(2) * r1;
    T b = r1 - a;
    T c = d0;
    T d = y0;
	return {a,b,c,d};
}

template <std::unsigned_integral T> 
forceinline constexpr T absolute(T v) { return v; }

template <std::signed_integral T> 
forceinline constexpr T absolute(T v) { return v < 0 ? -v :v; }

forceinline constexpr f32 absolute(f32 v) { return std::is_constant_evaluated() ? (v >= 0 ? v : -v) : fabsf(v); }
forceinline constexpr f64 absolute(f64 v) { return std::is_constant_evaluated() ? (v >= 0 ? v : -v) : fabs(v); }

template <class Scalar> forceinline constexpr v2<Scalar> absolute(v2<Scalar> v) { return {absolute(v.x), absolute(v.y)}; }
template <class Scalar> forceinline constexpr v3<Scalar> absolute(v3<Scalar> v) { return {absolute(v.x), absolute(v.y), absolute(v.z)}; }
template <class Scalar> forceinline constexpr v4<Scalar> absolute(v4<Scalar> v) { return {absolute(v.x), absolute(v.y), absolute(v.z), absolute(v.w)}; }

template <class T>
forceinline constexpr auto approximately(T a, T b, T epsilon) {
	return absolute(a - b) < epsilon;
}

template <class T>
forceinline constexpr auto relatively_equal(T a, T b, T relative_epsilon) {
	return absolute(a - b) < relative_epsilon * max(absolute(a), absolute(b));
}

forceinline f32 set_sign(f32 dst, f32 src) { return copysignf(dst, src); }
forceinline f64 set_sign(f64 dst, f64 src) { return copysign(dst, src); }

forceinline s8 sign(s8  v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s16 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s32 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s64 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }

forceinline f32 sign(f32 v) { return set_sign(1.0f, v); }

template <class T> forceinline v2<T> sign(v2<T> v) { return {sign(v.x), sign(v.y)}; }
template <class T> forceinline v3<T> sign(v3<T> v) { return {sign(v.x), sign(v.y), sign(v.z)}; }
template <class T> forceinline v4<T> sign(v4<T> v) { return {sign(v.x), sign(v.y), sign(v.z), sign(v.w)}; }

forceinline f32 trunc(f32 v) { return _mm_cvtss_f32(_mm_round_ps(_mm_set_ss(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)); };
forceinline v2f trunc(v2f v) { return {trunc(v.x), trunc(v.y)}; }
forceinline v3f trunc(v3f v) { return {trunc(v.x), trunc(v.y), trunc(v.z)}; }
forceinline v4f trunc(v4f v) { return {trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w)}; }

forceinline f64 trunc(f64 v) { return _mm_cvtsd_f64(_mm_round_pd(_mm_set_sd(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)); };

#define MOD(f32) \
forceinline f32 modulo(f32 a, f32 b) { return set_sign(a - trunc(a / b) * b, a); } \
forceinline f32 frac(f32 a, f32 b) { return frac(a / b) * b; }
MOD(f32)
MOD(f64)
#undef MOD

forceinline v2f modulo(v2f a, v2f b) { return {modulo(a.x, b.x), modulo(a.y, b.y)}; }
forceinline v3f modulo(v3f a, v3f b) { return {modulo(a.x, b.x), modulo(a.y, b.y), modulo(a.z, b.z)}; }
forceinline v4f modulo(v4f a, v4f b) { return {modulo(a.x, b.x), modulo(a.y, b.y), modulo(a.z, b.z), modulo(a.w, b.w)}; }

forceinline v2f frac(v2f a, v2f b) { return {frac(a.x, b.x), frac(a.y, b.y)}; }
forceinline v3f frac(v3f a, v3f b) { return {frac(a.x, b.x), frac(a.y, b.y), frac(a.z, b.z)}; }
forceinline v4f frac(v4f a, v4f b) { return {frac(a.x, b.x), frac(a.y, b.y), frac(a.z, b.z), frac(a.w, b.w)}; }

forceinline constexpr v2f sqrt(v2f v) { return {sqrt(v.x), sqrt(v.y)}; }
forceinline constexpr v3f sqrt(v3f v) { return {sqrt(v.x), sqrt(v.y), sqrt(v.z)}; }
forceinline constexpr v4f sqrt(v4f v) { return {sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w)}; }

// https://suraj.sh/fast-square-root-approximation
forceinline constexpr f32 fast_sqrt(f32 n) {
	s32 i = std::bit_cast<s32>(n);
	i = 0x1fbd3f7d + (i >> 1);
	f32 y = std::bit_cast<f32>(i);
	return (y * y + n) / y * 0.5f;
}

forceinline constexpr f32 reciprocal(f32 v) { return std::is_constant_evaluated() ? (1.0f / v) : _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss(v))); }
forceinline constexpr v2f reciprocal(v2f v) { return {reciprocal(v.x), reciprocal(v.y)}; }
forceinline constexpr v3f reciprocal(v3f v) { return {reciprocal(v.x), reciprocal(v.y), reciprocal(v.z)}; }
forceinline constexpr v4f reciprocal(v4f v) { return {reciprocal(v.x), reciprocal(v.y), reciprocal(v.z), reciprocal(v.w)}; }

forceinline f32 sin(f32 v) { return ::sinf(v); }
forceinline v2f sin(v2f v) { return {sin(v.x), sin(v.y)}; }
forceinline v3f sin(v3f v) { return {sin(v.x), sin(v.y), sin(v.z)}; }
forceinline v4f sin(v4f v) { return {sin(v.x), sin(v.y), sin(v.z), sin(v.w)}; }

forceinline f32 cos(f32 v) { return ::cosf(v); }
forceinline v2f cos(v2f v) { return {cos(v.x), cos(v.y)}; }
forceinline v3f cos(v3f v) { return {cos(v.x), cos(v.y), cos(v.z)}; }
forceinline v4f cos(v4f v) { return {cos(v.x), cos(v.y), cos(v.z), cos(v.w)}; }

forceinline f32 tan(f32 v) { return ::tanf(v); }
forceinline v2f tan(v2f v) { return {tan(v.x), tan(v.y)}; }
forceinline v3f tan(v3f v) { return {tan(v.x), tan(v.y), tan(v.z)}; }
forceinline v4f tan(v4f v) { return {tan(v.x), tan(v.y), tan(v.z), tan(v.w)}; }

forceinline void cos_sin(f32 v, f32 &_cos, f32 &_sin) { _cos = cos(v); _sin = sin(v); }
forceinline void cos_sin(v2f v, v2f &_cos, v2f &_sin) { _cos = cos(v); _sin = sin(v); }
forceinline void cos_sin(v3f v, v3f &_cos, v3f &_sin) { _cos = cos(v); _sin = sin(v); }
forceinline void cos_sin(v4f v, v4f &_cos, v4f &_sin) { _cos = cos(v); _sin = sin(v); }

forceinline void cos_sin(f32 v, v2f &result) { result.x = cos(v); result.y = sin(v); }

forceinline v2f cos_sin(f32 v) { return {cos(v), sin(v)}; }

namespace approximations {

namespace quadratic {

// Pretty bad but simple.

// Max  error: 0.056
// Mean error: 0.030
forceinline f32 sin(f32 x) {
	x = (frac(x * (1 / pi) + 1, 2) - 1) * 2;
	return (1 - pow2(1 - absolute(x))) * sign(x);
}
forceinline f32 cos(f32 x) {
	return sin(x + pi/2);
}

// Max  error: 0.066
// Mean error: 0.047
forceinline f32 asin(f32 x) {
	return (1 - sqrt(1 - absolute(x))) * sign(x) * (pi/2);
}
forceinline f32 acos(f32 x) {
	return asin(-x) + pi/2;
}

// Max error: infinity :)
//     1     on [0; 1.4]
//     0.1   on [0; 1  ]
//     0.052 on [0; 0.9]
forceinline f32 tan(f32 x) {
	return sin(x) / cos(x);
}

// Max error: 0.071
//forceinline f32 atan(f32 x) {
//	return (1 - 1 / (absolute(x) + 1)) * (pi/2) * sign(x);
//}

// Max error: 0.034
// Half the error for two extra muls
forceinline f32 atan(f32 x) {
	return (1 - 1 / pow2(absolute(x * 0.39193398f) + 1)) * (pi/2) * sign(x);
}

}

namespace bhaskara {

// Max  error: 0.018
// Mean error: todo
forceinline f32 sin(f32 v) {
	v = frac(v, pi * 2);
	auto mask = v >= pi;
	v = select(mask, v - pi, v);
	return (16 * v * (pi - v)) * reciprocal(5 * pi * pi - 4 * v * (pi - v)) * select(mask, -1.0f, 1.0f);
}
forceinline f32 cos(f32 v) { return sin(v + pi*0.5f); }

forceinline v2f cos_sin(f32 v) { return {cos(v), sin(v)}; }

}

}


forceinline f32 atan2(f32 y, f32 x) { return ::atan2f(y, x); }
forceinline f32 atan2(v2f v) { return atan2(v.y, v.x); }

forceinline f32 atan2_approx(f32 y, f32 x) {
	auto ay = absolute(y) + 1e-10f;
	auto mask = x < 0;
	auto r = select(mask, (x + ay) / (ay - x), (x - ay) / (ay + x));
	auto angle = select(mask, pi * 0.75f, pi * 0.25f);
	angle += (0.1963f * r * r - 0.9817f) * r;
	return select(y < 0, -angle, angle);
}
forceinline f32 atan2_approx(v2f v) { return atan2_approx(v.y, v.x); }

template <class A, class T>
forceinline constexpr auto lerp_wrapped(A a, A b, T t, A min, A max) {
	auto s = max - min;
	a -= min;
	b -= min;

	a = frac(a, s);
	b = frac(b, s);
	auto d = a - b;
	return min + select(absolute(d) > half(s),
				  frac(lerp(a, b+sign(d)*s, t), s),
				  lerp(a, b, t));
}

forceinline constexpr f32 angular_size(f32 distance, f32 radius) {
	f32 cos = radius / distance;
	if (cos < 1) {
		return pi - acos(cos)*2;
	} else {
		return tau;
	}
}

forceinline constexpr f32 dot(f32 a, f32 b) { return a * b; }

template <class T> forceinline constexpr T dot(v2<T> a, v2<T> b) { return a *= b, a.x + a.y; }
template <class T> forceinline constexpr T dot(v3<T> a, v3<T> b) { return a *= b, a.x + a.y + a.z; }
template <class T> forceinline constexpr T dot(v4<T> a, v4<T> b) { return a *= b, a.x + a.y + a.z + a.w; }

template <class T>
forceinline auto reflect(T v, T n) {
	return v - dot(v, n) * n * 2;
}

forceinline constexpr v2f perp(v2f a) { return {-a.y, a.x}; }
forceinline constexpr v2s perp(v2s a) { return {-a.y, a.x}; }

forceinline constexpr f32 cross(v2f a, v2f b) {
	return a.x * b.y - a.y * b.x;
}
forceinline constexpr v3f cross(v3f a, v3f b) {
	return a.yzx() * b.zxy() - a.zxy() * b.yzx();
}

template <class T>
forceinline auto sum(v2<T> v) { return v.x + v.y; }

template <class T>
forceinline auto sum(v3<T> v) { return v.x + v.y + v.z; }

template <class T>
forceinline auto sum(v4<T> v) { return v.x + v.y + v.z + v.w; }

forceinline f32 rsqrt(f32 v) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(v))); }

template <class T>
forceinline constexpr auto length_squared(T a) {
	return dot(a, a);
}
template <class T>
forceinline constexpr auto length(T a) {
	return sqrt(dot(a, a));
}
template <class T>
forceinline constexpr auto normalize_unchecked(T a) {
	return a / sqrt(dot(a, a));
}
template <class T>
forceinline constexpr void normalize_unchecked(T *a) {
	*a = normalize(*a);
}

template <class T>
forceinline constexpr auto normalize(T a, T fallback = {1}) {
	auto lsq = length_squared(a);
	if (lsq == decltype(lsq){})
		return fallback;
	return a * (1.0f / sqrt(lsq));
}
template <class T>
forceinline constexpr void normalize(T *a, T fallback = {1}) {
	*a = normalize(*a, fallback);
}
template <class T>
forceinline constexpr T clamp_length(T a, typename T::Scalar limit) {
	auto lsq = length_squared(a);
	if (lsq > limit*limit) {
		return a * (limit / sqrt(lsq));
	}
	return a;
}
template <class T>
forceinline constexpr auto distance_squared(T a, T b) {
	return length_squared(a - b);
}
template <class T>
forceinline constexpr auto distance(T a, T b) {
	return sqrt(distance_squared(a, b));
}
template <> forceinline constexpr auto distance(f32 a, f32 b) { return absolute(a - b); }
template <> forceinline constexpr auto distance(f64 a, f64 b) { return absolute(a - b); }
template <class T>
forceinline constexpr auto manhattan(T a, T b) {
	return sum(absolute(a - b));
}
forceinline s32 maxDistance(v3s a, v3s b) {
	a = absolute(a - b);
	return max(max(a.x, a.y), a.z);
}

template <class T>
forceinline constexpr auto move_toward(T a, T b, f32 t) {
	auto d = b - a;
	if (length_squared(d) > t * t) {
		return a + normalize(d) * t;
	} else {
		return b;
	}
}

template <class T>
forceinline constexpr auto move_toward_wrapped(T a, T b, f32 t, T min, T max) {
	if (a < b) {
		if (b - a < t)
			return b;
		auto dl = absolute(a - min + max - b);
		auto dr = absolute(b - a);
		if (dl < dr) {
			a -= t;
			if (a < min) {
				a += max - min;
			}
		} else {
			a += t;
			if (a > max) {
				a -= max - min;
			}
		}
	} else {
		if (a - b < t)
			return b;
		auto dl = absolute(a - b);
		auto dr = absolute(b - min + max - a);
		if (dl < dr) {
			a -= t;
			if (a < min) {
				a += max - min;
			}
		} else {
			a += t;
			if (a > max) {
				a -= max - min;
			}
		}
	}
	return a;
}

template <class T>
forceinline constexpr auto move_away(T a, T b, f32 t) {
	return a + normalize(a - b) * t;
}

template <class T>
forceinline constexpr T project_on_line(T vector, T normal) {
	return normal * dot(normal, vector);
}

template <class T>
forceinline constexpr T project_on_plane(T vector, T normal) {
	return vector - project_on_line(vector, normal);
}

template <class T>
forceinline constexpr T face_toward(T vector, T normal) {
	return vector * sign(dot(vector, normal));
}

forceinline f32 signed_angle(v2f a, v2f b) {
	return atan2(a.x*b.y - a.y*b.x, dot(a, b));
}

forceinline f32 signed_angle(v3f a, v3f b, v3f n) {
	return atan2(dot(n, cross(a, b)), dot(a, b));
}

forceinline f32 angle(v2f a, v2f b) {
	return abs(signed_angle(a, b));
}

forceinline f32 angle(v3f a, v3f b) {
	return acos(dot(a, b)/sqrt(length_squared(a) * length_squared(b)));
}

forceinline f32 cos01(f32 t) { return 0.5f - cosf(t * pi) * 0.5f; }

forceinline v3f rotate_around(v3f v, v3f axis, f32 angle) {
    v3f a = normalize(axis);

    v2f cs = cos_sin(angle);
    f32 ic = 1.0f - cs.x;
	v3f as = a * cs.y;
	v3f aic = a * ic;

    m3 r = {
        a.x * aic.x + cs.x,  a.x * aic.y - as.z,  a.x * aic.z + as.y,
        a.y * aic.x + as.z,  a.y * aic.y + cs.x,  a.y * aic.z - as.x,
        a.z * aic.x - as.y,  a.z * aic.y + as.x,  a.z * aic.z + cs.x,
    };

    return r * v;
}

forceinline v3f hsv_to_rgb(f32 h, f32 s, f32 v) {
	h = frac(h);
	f32 c = v * s;
	f32 x = c * (1 - absolute(modulo(h * 6, 2) - 1));
	v3f m = V3f(v - c);
	     if (h < ( 60 / 360.0f)) { m += v3f{c, x, 0}; }
	else if (h < (120 / 360.0f)) { m += v3f{x, c, 0}; }
	else if (h < (180 / 360.0f)) { m += v3f{0, c, x}; }
	else if (h < (240 / 360.0f)) { m += v3f{0, x, c}; }
	else if (h < (300 / 360.0f)) { m += v3f{x, 0, c}; }
	else                         { m += v3f{c, 0, x}; }

	return m;
}
forceinline v4f hsv_to_rgb(f32 h, f32 s, f32 v, f32 a) { return V4f(hsv_to_rgb(h, s, v), a); }
forceinline v3f hsv_to_rgb(v3f hsv) { return hsv_to_rgb(hsv.x, hsv.y, hsv.z); }
forceinline v4f hsv_to_rgb(v4f hsv) { hsv.xyz = hsv_to_rgb(hsv.xyz); return hsv; }

forceinline v3f rgb_to_hsv(f32 r, f32 g, f32 b) {
	f32 cMax = max(max(r, g), b);
	f32 cMin = min(min(r, g), b);
	f32 delta = cMax - cMin;
	v3f result;

    result.z = cMax;
    if (delta == 0) {
        result.x = result.y = 0;
        return result;
    }
    if (cMax == 0) {
        result.x = result.y = 0;
        return result;
    } else {
        result.y = (delta / cMax);
    }
         if (r >= cMax) result.x = (g - b) / delta;
    else if (g >= cMax) result.x = (b - r) / delta + 2.0f;
    else                result.x = (r - g) / delta + 4.0f;

    result.x = frac(result.x * 1.0f / 6.0f);

    return result;
}
forceinline v3f rgb_to_hsv(v3f rgb) { return rgb_to_hsv(rgb.x, rgb.y, rgb.z); }
forceinline v4f rgb_to_hsv(v4f rgb) { rgb.xyz = rgb_to_hsv(rgb.xyz); return rgb; }

// This is a line with no bounds
template<class T>
struct line {
	T a, b;
	template <class U>
	explicit operator line<U>() const {
		return {(U)a, (U)b};
	}
};

template<class T>
struct line_segment {
	T a, b;
	template <class U>
	explicit operator line_segment<U>() const {
		return {(U)a, (U)b};
	}
};

template <class T>
forceinline line<T> line_begin_end(T begin, T end) {
	return {begin, end};
}

template <class T>
forceinline line<T> line_begin_dir(T begin, T dir) {
	return {begin, begin + dir};
}

template <class T>
forceinline line_segment<T> line_segment_begin_end(T begin, T end) {
	return {begin, end};
}

template <class T>
forceinline line_segment<T> line_segment_begin_dir(T begin, T dir) {
	return {begin, begin + dir};
}

template <class T>
forceinline auto length_squared(line_segment<T> line) {
	return distance_squared(line.a, line.b);
}

template <class T>
forceinline auto length(line_segment<T> line) {
	return distance(line.a, line.b);
}

template <class T>
forceinline auto center(line_segment<T> line) {
	return (line.a + line.b) / 2;
}

template <class T>
forceinline auto closest_point(T point, line<T> line) {
	auto l2 = distance_squared(line.a, line.b);
	if (l2 == 0) return T{};

	using Scalar = decltype(l2);

	auto t = dot(point - line.a, line.b - line.a) / l2;
	return line.a + t * (line.b - line.a);
}
template <class T>
forceinline auto closest_point(line<T> line, T point) {
	return closest_point(point, line);
}

template <class T>
forceinline auto closest_point(T point, line_segment<T> line) {
	auto l2 = length_squared(line);
	if (l2 == 0) return line.a;

	using Scalar = decltype(l2);

	auto t = clamp(dot(point - line.a, line.b - line.a) / l2, (Scalar)0, (Scalar)1);
	return line.a + t * (line.b - line.a);
}
template <class T>
forceinline auto closest_point(line_segment<T> line, T point) {
	return closest_point(point, line);
}

template <class T>
forceinline auto closest_point(line<T> a, line<T> b) {
	auto d1 = normalize(a.b - a.a);
	auto d2 = normalize(b.b - b.a);
    auto n1 = cross(d1, d2);
    auto n2 = cross(d2, n1);
    return a.a + dot(b.a - a.a, n2) / dot(d1, n2) * d1;
}

template <class T>
forceinline auto distance(T point, line<T> line) {
	auto l2 = distance_squared(line.a, line.b);
	if (l2 == 0) return (decltype(distance(T{}, T{})))-1;

	using Scalar = decltype(l2);

	auto t = dot(point - line.a, line.b - line.a) / l2;
	auto projection = line.a + t * (line.b - line.a);
	return distance(point, projection);
}
template <class T>
forceinline auto distance(line<T> line, T point) {
	return distance(point, line);
}

template <class T>
forceinline auto distance(T point, line_segment<T> line) {
	auto l2 = length_squared(line);
	if (l2 == 0) return distance(point, line.a);

	using Scalar = decltype(l2);

	auto t = clamp(dot(point - line.a, line.b - line.a) / l2, (Scalar)0, (Scalar)1);
	auto projection = line.a + t * (line.b - line.a);
	return distance(point, projection);
}
template <class T>
forceinline auto distance(line_segment<T> line, T point) {
	return distance(point, line);
}

// NOTE: `direction` is assumed to be normalized
template<class T>
struct ray {
	T origin, direction;
};

template <class T>
forceinline ray<T> ray_origin_end(T origin, T end) {
	return {origin, normalize(end - origin)};
}

template <class T>
forceinline ray<T> ray_origin_direction(T origin, T direction) {
	return {origin, normalize(direction)};
}

template <class T>
forceinline line<T> as_line(ray<T> r) {
	return {r.origin, r.origin + r.direction};
}

template <class T>
forceinline line<T> as_line(ray<T> r, auto length) {
	return {r.origin, r.origin + r.direction * length};
}

template <class T>
struct plane {
	T point;
	T normal;
};

template <class T>
plane<T> plane_point_normal(T point, T normal) {
	return {point, normal};
}

template <class T>
T intersect(ray<T> ray, plane<T> plane) {
	auto d = dot(plane.point, -plane.normal);
	auto t = -(d + dot(ray.origin, plane.normal)) / dot(ray.direction, plane.normal);
	return ray.origin + t * ray.direction;
}

template <class T>
union triangle {
	struct {
		T a, b, c;
	};
	T array[3];
};

template <class T>
forceinline constexpr T normal_of(triangle<T> t) {
	return normalize(cross(t.a - t.b, t.a - t.c));
}

template <class Scalar>
v3<Scalar> barycentric(triangle<v2<Scalar>> t, v2<Scalar> p)
{
	v2<Scalar> ac = t.a - t.c;
	v2<Scalar> bc = t.b - t.c;
	v3<Scalar> result = {};
	Scalar determinant = ac.x * bc.y - bc.x * ac.y;
	result.x = bc.y * (p.x - t.c.x) + bc.x * (t.c.y - p.y);
	result.y = ac.y * (t.c.x - p.x) + ac.x * (p.y - t.c.y);
	result.z = determinant - result.x - result.y;
	return result / determinant;
}

template<class T>
struct aabb {
	T min, max;
	T size() const { return max - min; }
	T center() const {
		return half(max + min);
	}
	template <class U> requires requires(T t, U u) { t * u; } aabb<T> operator*(U const &b) { return {min * b, max * b}; }
	template <class U> requires requires(T t, U u) { t / u; } aabb<T> operator/(U const &b) { return {min / b, max / b}; }
	auto operator==(aabb const &that) const { return min == that.min && max == that.max; }
	auto operator!=(aabb const &that) const { return min != that.min || max != that.max; }
	template <class U>
	explicit operator aabb<U>() const { return { (U)min, (U)max }; }

	aabb operator-(T b) { return {min - b, max - b}; }
	aabb operator+(T b) { return {min + b, max + b}; }
	friend aabb operator-(T a, aabb<T> b) { return {a - b.min, a - b.max}; }
	friend aabb operator+(T a, aabb<T> b) { return {a + b.min, a + b.max}; }
	aabb &operator-=(T b) { return min -= b, max -= b, *this; }
	aabb &operator+=(T b) { return min += b, max += b, *this; }

	aabb<T> with_size(T new_size, T local_pivot = convert<T>(0.5f)) requires std::is_floating_point_v<typename T::Scalar> {
		auto pivot = lerp(min, max, local_pivot);
		return {
			.min = pivot - new_size / 2,
			.max = pivot + new_size / 2,
		};
	}

	void set_size(T new_size, T local_pivot = convert<T>(0.5f)) requires std::is_floating_point_v<typename T::Scalar> {
		*this = with_size(new_size, local_pivot);
	}

	struct Iterator {
		aabb<T> range;
		T value;

		T operator*() const {
			return value;
		}
		Iterator &operator++() {
			increment(value);
			return *this;
		}
		bool operator==(Iterator const &that) const {
			return all(value == that.value);
		}

		template <class Scalar>
		void increment(Scalar &value) {
			++value;
		}
		template <class Scalar>
		void increment(v2<Scalar> &value) {
			++value.x;
			if (value.x == range.max.x) {
				value.x = range.min.x;
				++value.y;
			}
		}
		template <class Scalar>
		void increment(v3<Scalar> &value) {
			++value.x;
			if (value.x == range.max.x) {
				value.x = range.min.x;
				++value.y;
				if (value.y == range.max.y) {
					value.y = range.min.y;
					++value.z;
				}
			}
		}
	};

	template <class Scalar>
	static Scalar sentinel(aabb<Scalar> aabb) {
		return aabb.max;
	}
	template <class Scalar>
	static v2<Scalar> sentinel(aabb<v2<Scalar>> aabb) {
		return {aabb.min.x, aabb.max.y};
	}
	template <class Scalar>
	static v3<Scalar> sentinel(aabb<v3<Scalar>> aabb) {
		return {aabb.min.x, aabb.min.y, aabb.max.z};
	}

	Iterator begin() {
		if (all(min < max))
			return {*this, min};
		return {*this, sentinel(*this)};
	}
	Iterator end() { return {*this, sentinel(*this)}; }
};

template <class T>
forceinline aabb<T> aabb_min_max(T min, T max) {
	return {min, max};
}
template <class T>
forceinline aabb<T> aabb_min_max_sorted(T a, T b) {
	return {
		min(a, b), 
		max(a, b)
	};
}
template <class T>
forceinline aabb<T> aabb_min_size(T min, T size) {
	return {min, min + size};
}
template <class T>
forceinline aabb<T> aabb_center_diameter(T center, T diameter) {
	auto radius = half(diameter);
	return {center - radius, center + radius};
}
template <class T>
forceinline aabb<T> aabb_center_radius(T center, T radius) {
	return {center - radius, center + radius};
}

template <ForEachFlags flags = 0, std::integral T>
forceinline bool for_each(aabb<v2<T>> b, auto fn) {
	using Ret = decltype(fn(v2<T>{}));

	auto start = b.min;
	auto end = b.max;
	auto step = v2<T>{1, 1};
	if constexpr (flags & ForEach_reverse) {
		start = b.max - 1;
		end = b.min - 1;
		step = -step;
	}

	v2<T> p;
	for (p.y = start.y; p.y != end.y; p.y += step.y) {
	for (p.x = start.x; p.x != end.x; p.x += step.x) {
		if constexpr (std::is_same_v<Ret, ForEachDirective>) {
			switch (fn(p)) {
				case ForEach_break:
					return true;
			}
		} else {
			fn(p);

		}
		}
	}
	return false;
}

template <ForEachFlags flags = 0, std::integral T>
forceinline bool for_each(aabb<v3<T>> b, auto fn) {
	using Ret = decltype(fn(v3<T>{}));

	auto start = b.min;
	auto end = b.max;
	auto step = v3<T>{1, 1, 1};
	if constexpr (flags & ForEach_reverse) {
		start = b.max - 1;
		end = b.min - 1;
		step = -step;
	}

	v3<T> p;
	for (p.z = start.z; p.z != end.z; p.z += step.z) {
	for (p.y = start.y; p.y != end.y; p.y += step.y) {
	for (p.x = start.x; p.x != end.x; p.x += step.x) {
		if constexpr (std::is_same_v<Ret, ForEachDirective>) {
			switch (fn(p)) {
				case ForEach_break:
					return true;
			}
		} else {
			fn(p);
		}
	}
	}
	}
	return false;
}

#define for_aabb3(x, y, z, condition, box)           \
for (auto z = box.min.z; z condition box.max.z; ++z) \
for (auto y = box.min.y; y condition box.max.y; ++y) \
for (auto x = box.min.x; x condition box.max.x; ++x)

template <class T>
forceinline aabb<T> include(aabb<T> box, T point) {
	box.min = min(box.min, point);
	box.max = max(box.max, point);
	return box;
}

template <class T>
forceinline aabb<T> include(aabb<T> box, aabb<T> x) {
	return include(include(box, x.min), x.max);
}

template <class T>
forceinline aabb<T> aabb_including_points(Span<T> points) {
	aabb<T> result = {points[0], points[0]};
	for (auto point : points.skip(1)) {
		result = include(result, point);
	}
	return result;
}

template <class T>
forceinline aabb<T> aabb_including_points(std::initializer_list<T> points) {
	return aabb_including_points(Span(points));
}

template <class T>
forceinline aabb<T> extend(aabb<T> box, T radius) {
	box.min -= radius;
	box.max += radius;
	return box;
}

template <class T>
forceinline aabb<T> extend(aabb<T> box, T min_offset, T max_offset) {
	box.min -= min_offset;
	box.max += max_offset;
	return box;
}

template <class T>
forceinline aabb<T> to_zero(aabb<T> box) {
	box.max -= box.min;
	box.min = {};
	return box;
}


template <class T>
forceinline aabb<T> intersection(aabb<T> box, aabb<T> bounds) {
	box.min = clamp(box.min, bounds.min, bounds.max);
	box.max = clamp(box.max, bounds.min, bounds.max);
	return box;
}

template <class T>
forceinline aabb<T> constrain(aabb<T> box, T bounds_min, T bounds_max) {
	auto diff = max(bounds_min - box.min, T{});
	box.min += diff;
	box.max += diff;

	diff = min(bounds_max - box.max, T{});
	box.min += diff;
	box.max += diff;

	return box;
}

template <class T>
forceinline aabb<T> constrain(aabb<T> box, aabb<T> bounds) {
	return constrain(box, bounds.min, bounds.max);
}
template <class T>
forceinline T clamp(T a, aabb<T> b) {
	return clamp(a, b.min, b.max);
}
template <class Scalar>
forceinline bool in_bounds(Scalar a, aabb<Scalar> b) {
	return
		(a >= b.min) &&
		(a < b.max);
}
template <class Scalar>
forceinline bool in_bounds(v2<Scalar> a, aabb<v2<Scalar>> b) {
	return
		(a.x >= b.min.x) &&
		(a.y >= b.min.y) &&
		(a.x < b.max.x) &&
		(a.y < b.max.y);
}
template <class Scalar>
forceinline bool in_bounds(v3<Scalar> a, aabb<v3<Scalar>> b) {
	return
		(a.x >= b.min.x) &&
		(a.y >= b.min.y) &&
		(a.z >= b.min.z) &&
		(a.x < b.max.x) &&
		(a.y < b.max.y) &&
		(a.z < b.max.z);
}
template <class Scalar>
forceinline bool in_bounds(v4<Scalar> a, aabb<v4<Scalar>> b) {
	return
		(a.x >= b.min.x) &&
		(a.y >= b.min.y) &&
		(a.z >= b.min.z) &&
		(a.w >= b.min.w) &&
		(a.x < b.max.x) &&
		(a.y < b.max.y) &&
		(a.z < b.max.z) &&
		(a.w < b.max.w);
}

template <class T>
forceinline bool in_bounds(aabb<T> inner, aabb<T> outer) {
	return all(outer.min <= inner.min && inner.max <= outer.max);
}

forceinline bool in_bounds_angle(f32 angle, aabb<f32> bounds) {
	angle = frac(angle, tau);
	return 
		in_bounds(angle + floor(bounds.min, tau), bounds) ||
		in_bounds(angle + floor(bounds.max, tau), bounds);
}

template <class Scalar>
forceinline bool intersects(aabb<Scalar> a, aabb<Scalar> b) {
	return
		(a.min < b.max) &&
		(a.max > b.min);
}
template <class Scalar>
forceinline bool intersects(aabb<v2<Scalar>> a, aabb<v2<Scalar>> b) {
	return
		(a.min.x < b.max.x) &&
		(a.min.y < b.max.y) &&
		(a.max.x > b.min.x) &&
		(a.max.y > b.min.y);
}
template <class Scalar>
forceinline bool intersects(aabb<v3<Scalar>> a, aabb<v3<Scalar>> b) {
	return
		(a.min.x < b.max.x) &&
		(a.min.y < b.max.y) &&
		(a.min.z < b.max.z) &&
		(a.max.x > b.min.x) &&
		(a.max.y > b.min.y) &&
		(a.max.z > b.min.z);
}
template <class Scalar>
forceinline bool intersects(aabb<v4<Scalar>> a, aabb<v4<Scalar>> b) {
	return
		(a.min.x < b.max.x) &&
		(a.min.y < b.max.y) &&
		(a.min.z < b.max.z) &&
		(a.min.w < b.max.w) &&
		(a.max.x > b.min.x) &&
		(a.max.y > b.min.y) &&
		(a.max.z > b.min.z) &&
		(a.max.w > b.min.w);
}

template <class Scalar>
forceinline auto volume(aabb<v2<Scalar>> const &box) {
	auto diameter = box.max - box.min;
	return diameter.x * diameter.y;
}

template <class Scalar>
forceinline auto volume(aabb<v3<Scalar>> const &box) {
	auto diameter = box.max - box.min;
	return diameter.x * diameter.y * diameter.z;
}

template <class Scalar>
forceinline bool has_volume(aabb<v2<Scalar>> rect) {
	return rect.min.x < rect.max.x
		&& rect.min.y < rect.max.y;
}

template <class Scalar>
forceinline bool has_volume(aabb<v3<Scalar>> rect) {
	return rect.min.x < rect.max.x
		&& rect.min.y < rect.max.y
		&& rect.min.z < rect.max.z;
}

template <class Vector>
forceinline aabb<Vector> floor_ceil(aabb<Vector> rect) {
	return {
		floor(rect.min),
		ceil (rect.max),
	};
}

template <class Vector>
forceinline aabb<Vector> floor_ceil(aabb<Vector> rect, Vector step) {
	return {
		floor(rect.min, step),
		ceil (rect.max, step),
	};
}

template <class S>
Optional<aabb<v2<S>>> merge_into_one(aabb<v2<S>> a, aabb<v2<S>> b) {
	using Result = aabb<v2<S>>;
	if (all(a.maxmin() == b.minmin()) && all(a.maxmax() == b.minmax())) return Result{a.minmin(), b.maxmax()};
	if (all(a.maxmax() == b.maxmin()) && all(a.minmax() == b.minmin())) return Result{a.minmin(), b.maxmax()};
	if (all(a.minmax() == b.maxmax()) && all(a.minmin() == b.maxmin())) return Result{b.minmin(), a.maxmax()};
	if (all(a.minmin() == b.minmax()) && all(a.maxmin() == b.maxmax())) return Result{b.minmin(), a.maxmax()};
	return {};
}

// Axis aligned bounding box subrtacion routines
// a - b
// subtract b from a
//
// subtract_volume treats a and b as volumes.
// Returned adjacent aabbs have the same min and max
//
// subtract_points treats a and b as sets of points;
// Every aabb in result contains unique set of points.
//
// In both cases result's max point is inclusive
//

forceinline StaticList<aabb<v2s>, 8> subtract_volumes(aabb<v2s> a, aabb<v2s> b) {
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	StaticList<aabb<v2s>, 8> boxes = {
		aabb_min_max(a.min, b.min),
		aabb_min_max(b.max, a.max),
		aabb_min_max(v2s{a.min.x, b.max.y}, v2s{b.min.x, a.max.y}),
		aabb_min_max(v2s{b.max.x, a.min.y}, v2s{a.max.x, b.min.y}),
		aabb_min_max(v2s{b.min.x, a.min.y}, v2s{b.max.x, b.min.y}),
		aabb_min_max(v2s{b.min.x, b.max.y}, v2s{b.max.x, a.max.y}),
		aabb_min_max(v2s{a.min.x, b.min.y}, v2s{b.min.x, b.max.y}),
		aabb_min_max(v2s{b.max.x, b.min.y}, v2s{a.max.x, b.max.y}),
	};
	for (u32 i = 0; i < boxes.count; ++i) {
		if (volume(boxes[i]) <= 0) {
			boxes.erase_at_unordered(i--);
		}
	}
	return boxes;
}
forceinline StaticList<aabb<v3s>, 26> subtract_volumes(aabb<v3s> a, aabb<v3s> b) {
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	StaticList<aabb<v3s>, 26> boxes = {
		// Corners
		aabb_min_max(a.min, b.min),                                                   // left bottom back
		aabb_min_max(b.max, a.max),                                                   // right top front
		aabb_min_max(v3s{a.min.x, b.max.y, a.min.z}, v3s{b.min.x, a.max.y, b.min.z}), // left top back
		aabb_min_max(v3s{b.max.x, b.max.y, a.min.z}, v3s{a.max.x, a.max.y, b.min.z}), // right top back
		aabb_min_max(v3s{a.min.x, a.min.y, b.max.z}, v3s{b.min.x, b.min.y, a.max.z}), // left bottom front
		aabb_min_max(v3s{b.max.x, a.min.y, b.max.z}, v3s{a.max.x, b.min.y, a.max.z}), // right bottom front
		aabb_min_max(v3s{b.max.x, a.min.y, a.min.z}, v3s{a.max.x, b.min.y, b.min.z}), // right bottom back
		aabb_min_max(v3s{a.min.x, b.max.y, b.max.z}, v3s{b.min.x, a.max.y, a.max.z}), // left top front

		// Edges
		aabb_min_max(v3s{b.min.x, a.min.y, a.min.z}, v3s{b.max.x, b.min.y, b.min.z}), // bottom back
		aabb_min_max(v3s{b.min.x, b.max.y, a.min.z}, v3s{b.max.x, a.max.y, b.min.z}), // top back
		aabb_min_max(v3s{b.min.x, a.min.y, b.max.z}, v3s{b.max.x, b.min.y, a.max.z}), // bottom front
		aabb_min_max(v3s{b.min.x, b.max.y, b.max.z}, v3s{b.max.x, a.max.y, a.max.z}), // top front
		aabb_min_max(v3s{a.min.x, b.min.y, a.min.z}, v3s{b.min.x, b.max.y, b.min.z}), // right back
		aabb_min_max(v3s{b.max.x, b.min.y, a.min.z}, v3s{a.max.x, b.max.y, b.min.z}), // left back
		aabb_min_max(v3s{a.min.x, b.min.y, b.max.z}, v3s{b.min.x, b.max.y, a.max.z}), // right front
		aabb_min_max(v3s{b.max.x, b.min.y, b.max.z}, v3s{a.max.x, b.max.y, a.max.z}), // left front
		aabb_min_max(v3s{a.min.x, b.max.y, b.min.z}, v3s{b.min.x, a.max.y, b.max.z}), // left top
		aabb_min_max(v3s{b.max.x, b.max.y, b.min.z}, v3s{a.max.x, a.max.y, b.max.z}), // right top
		aabb_min_max(v3s{a.min.x, a.min.y, b.min.z}, v3s{b.min.x, b.min.y, b.max.z}), // left bottom
		aabb_min_max(v3s{b.max.x, a.min.y, b.min.z}, v3s{a.max.x, b.min.y, b.max.z}), // right bottom

		// Faces
		aabb_min_max(v3s{b.min.x, b.min.y, a.min.z}, v3s{b.max.x, b.max.y, b.min.z}), // back
		aabb_min_max(v3s{b.min.x, b.min.y, b.max.z}, v3s{b.max.x, b.max.y, a.max.z}), // front
		aabb_min_max(v3s{a.min.x, b.min.y, b.min.z}, v3s{b.min.x, b.max.y, b.max.z}), // left
		aabb_min_max(v3s{b.max.x, b.min.y, b.min.z}, v3s{a.max.x, b.max.y, b.max.z}), // right
		aabb_min_max(v3s{b.min.x, b.max.y, b.min.z}, v3s{b.max.x, a.max.y, b.max.z}), // top
		aabb_min_max(v3s{b.min.x, a.min.y, b.min.z}, v3s{b.max.x, b.min.y, b.max.z}), // bottom
	};
	for (u32 i = 0; i < boxes.count; ++i) {
		if (volume(boxes[i]) <= 0) {
			boxes.erase_at_unordered(i--);
		}
	}
	return boxes;
}
forceinline StaticList<aabb<v2s>, 8> subtract_points(aabb<v2s> a, aabb<v2s> b) {
	aabb<v2s> original_b = b;
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	auto boxes = subtract_volumes(a, b);
	for (auto &box : boxes) {
		box.min.x += (original_b.max.x == b.max.x && box.min.x == b.max.x);
		box.min.y += (original_b.max.y == b.max.y && box.min.y == b.max.y);
		box.max.x -= (original_b.min.x == b.min.x && box.max.x == b.min.x);
		box.max.y -= (original_b.min.y == b.min.y && box.max.y == b.min.y);
	}
	return boxes;
}
forceinline StaticList<aabb<v3s>, 26> subtract_points(aabb<v3s> a, aabb<v3s> b) {
	aabb<v3s> original_b = b;
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	auto boxes = subtract_volumes(a, b);
	for (auto &box : boxes) {
		box.min.x += (original_b.max.x == b.max.x && box.min.x == b.max.x);
		box.min.y += (original_b.max.y == b.max.y && box.min.y == b.max.y);
		box.min.z += (original_b.max.z == b.max.z && box.min.z == b.max.z);
		box.max.x -= (original_b.min.x == b.min.x && box.max.x == b.min.x);
		box.max.y -= (original_b.min.y == b.min.y && box.max.y == b.min.y);
		box.max.z -= (original_b.min.z == b.min.z && box.max.z == b.min.z);
	}
	return boxes;
}

forceinline StaticList<aabb<v2s>, 9> combine_volumes(aabb<v2s> const &a, aabb<v2s> const &b) {
	StaticList<aabb<v2s>, 9> result = { a };
	result += subtract_volumes(b, a);
	return result;
}
forceinline StaticList<aabb<v3s>, 27> combine_volumes(aabb<v3s> const &a, aabb<v3s> const &b) {
	StaticList<aabb<v3s>, 27> result = { a };
	result += subtract_volumes(b, a);
	return result;
}
forceinline StaticList<aabb<v2s>, 9> combine_points(aabb<v2s> const &a, aabb<v2s> const &b) {
	StaticList<aabb<v2s>, 9> result = { a };
	result += subtract_points(b, a);
	return result;
}
forceinline StaticList<aabb<v3s>, 27> combine_points(aabb<v3s> const &a, aabb<v3s> const &b) {
	StaticList<aabb<v3s>, 27> result = { a };
	result += subtract_points(b, a);
	return result;
}

forceinline bool intersects(line<v2f> line, aabb<v2f> aabb) {
	v2f dir = normalize(line.b - line.a);
	v2f rdir = 1.0f / dir;
	v2f vMin = (aabb.min - line.a) * rdir;
	v2f vMax = (aabb.max - line.a) * rdir;

	f32 tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y));
	f32 tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y));

	return tMax > 0 && tMin < tMax;
}

forceinline bool intersects(line<v3f> line, aabb<v3f> aabb) {
	v3f dir = normalize(line.b - line.a);
	v3f rdir = 1.0f / dir;
	v3f vMin = (aabb.min - line.a) * rdir;
	v3f vMax = (aabb.max - line.a) * rdir;

	f32 tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y), min(vMin.z, vMax.z));
	f32 tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y), max(vMin.z, vMax.z));

	return tMax > 0 && tMin < tMax;
}

template <class Scalar>
Array<line_segment<v2<Scalar>>, 4> to_lines(aabb<v2<Scalar>> box) {
	return {
		line_segment<v2<Scalar>>{box.min.x, box.min.y, box.min.x, box.max.y},
		line_segment<v2<Scalar>>{box.min.x, box.min.y, box.max.x, box.min.y},
		line_segment<v2<Scalar>>{box.max.x, box.max.y, box.min.x, box.max.y},
		line_segment<v2<Scalar>>{box.max.x, box.max.y, box.max.x, box.min.y},
	};
}

template <class Scalar>
Array<triangle<v3<Scalar>>, 12> to_triangles(aabb<v3<Scalar>> box) {
	v3<Scalar> a = {box.min.x, box.min.y, box.min.z};
	v3<Scalar> b = {box.max.x, box.min.y, box.min.z};
	v3<Scalar> c = {box.min.x, box.max.y, box.min.z};
	v3<Scalar> d = {box.max.x, box.max.y, box.min.z};
	v3<Scalar> e = {box.min.x, box.min.y, box.max.z};
	v3<Scalar> f = {box.max.x, box.min.y, box.max.z};
	v3<Scalar> g = {box.min.x, box.max.y, box.max.z};
	v3<Scalar> h = {box.max.x, box.max.y, box.max.z};
	return {
		triangle<v3<Scalar>>{a, b, d},
		triangle<v3<Scalar>>{a, d, c},
		triangle<v3<Scalar>>{e, h, f},
		triangle<v3<Scalar>>{e, g, h},
		triangle<v3<Scalar>>{a, c, g},
		triangle<v3<Scalar>>{a, g, e},
		triangle<v3<Scalar>>{b, f, h},
		triangle<v3<Scalar>>{b, h, d},
		triangle<v3<Scalar>>{c, d, h},
		triangle<v3<Scalar>>{c, h, g},
		triangle<v3<Scalar>>{a, e, f},
		triangle<v3<Scalar>>{a, f, b},
	};
}

template <class T>
struct sphere {
	T center;
	T::Scalar radius;
};

template <class T>
sphere<T> sphere_center_radius(T center, typename T::Scalar radius) {
	return {center, radius};
}

// https://paulbourke.net/geometry/circlesphere/
forceinline StaticList<v2f, 2> intersection(sphere<v2f> a, sphere<v2f> b) {
	v2f d = b.center - a.center;
	f32 dl = length_squared(d);

	if (dl > pow2(a.radius + b.radius)) {
		/* no solution. circles do not intersect. */
		return {};
	}
	if (dl <= pow2(fabs(a.radius - b.radius))) {
		/* no solution. one circle is contained in the other */
		// This includes identical circles case
		// TODO: maybe reflect this in return type?
		return {};
	}

	dl = tl::sqrt(dl);

	f32 c = (pow2(a.radius) - pow2(b.radius) + pow2(dl)) / (dl * 2);

	v2f j = a.center + (d * c / dl);

	f32 h = tl::sqrt(pow2(a.radius) - pow2(c));

	v2f r = perp(d * (h / dl));

	if (all(r == v2f{})) {
		return {j};
	} else {
		return {j - r, j + r};
	}
}

#if 0
forceinline bool raycastLine(v2f a, v2f b, v2f c, v2f d, v2f& point, v2f& normal) {
	v2f ba = b - a;
	v2f dc = d - c;
	v2f ac = a - c;

	v2f s = v2f{ba.x, dc.x} * ac.y - v2f{ba.y, dc.y} * ac.x;
	s /= ba.x * dc.y - dc.x * ba.y;

	if (s.x >= 0 && s.x <= 1 && s.y >= 0 && s.y <= 1) {
		point  = a + (s.y * ba);
		normal = cross(normalize(c - d));
		if (dot(b - a, normal) > 0)
			normal *= -1;
		return true;
	}
	return false;
}

forceinline bool raycastPlane(v3f a, v3f b, v3f p1, v3f p2, v3f p3, v3f& point, v3f& normal) {
	auto p21 = p2 - p1;
	auto p31 = p3 - p1;
	normal	 = normalize(cross(p21, p31));

	auto r = a - b;

	f32 nr = dot(normal, r);

	if (nr <= 1e-5f) {
		return false;
	}

	f32 t = dot(-normal, a - p1) / nr;
	point = a + r * t;

	auto dp1 = point - p1;
	f32 u	 = dot(dp1, p21);
	f32 v	 = dot(dp1, p31);

	if (length_squared(a - point) > length_squared(a - b) || dot(a - point, a - b) <= 0) {
		return false;
	}

	return u >= 0.0f && u <= dot(p21, p21) && v >= 0.0f && v <= dot(p31, p31);
}
#endif
template <class Vector>
struct RaycastHit {
	Vector position = {};
	Vector normal = {};
	f32 distance = {};
};

template <class Vector>
RaycastHit<Vector> min(RaycastHit<Vector> a, RaycastHit<Vector> b) {
	return a.distance < b.distance ? a : b;
}

template <class Vector>
Optional<RaycastHit<Vector>> min(Optional<RaycastHit<Vector>> a, Optional<RaycastHit<Vector>> b) {
	if (a) {
		if (b) {
			return a.value().distance < b.value().distance ? a : b;
		} else {
			return a;
		}
	} else {
		if (b) {
			return b;
		} else {
			return {};
		}
	}
}

forceinline Optional<RaycastHit<v2f>> raycast(ray<v2f> ray, line_segment<v2f> line) {
	auto v1 = ray.origin - line.a;
	auto v2 = line.b - line.a;
	auto v3 = perp(ray.direction);

	auto d = dot(v2, v3);
	if (absolute(d) < 1e-6f)
		return {};

	auto t1 = cross(v2, v1) / d;
	auto t2 = dot(v1, v3) / d;

	if (t1 < 0 || t2 < 0 || t2 > 1.0)
		return {};

	auto n = perp(normalize(line.a - line.b));

	return RaycastHit<v2f>{
		.position = ray.origin + t1 * ray.direction,
		.normal = n * -sign(dot(n, ray.direction)),
		.distance = t1,
	};
}

template <class Vector>
forceinline Optional<RaycastHit<Vector>> raycast(ray<Vector> ray, sphere<Vector> sphere) {
	auto oc = ray.origin - sphere.center;
	auto b = 2 * dot(oc, ray.direction);
	auto c = dot(oc,oc) - sphere.radius*sphere.radius;
	auto d = b*b - 4*c;

	if (d < 0)
		return {};

	auto point = ray.origin + ray.direction * ((-b - sqrt(d)) / 2);
	return RaycastHit<Vector>{
		.position = point,
		.normal = normalize(point - sphere.center),
		.distance = distance(ray.origin, point),
	};
}

forceinline Optional<RaycastHit<v2f>> raycast(ray<v2f> ray, triangle<v2f> tri) {
	return min(
		raycast(ray, line_segment_begin_end(tri.a, tri.b)),
		raycast(ray, line_segment_begin_end(tri.b, tri.c)),
		raycast(ray, line_segment_begin_end(tri.c, tri.a))
	);
}

struct RaycastTriangleOptions {
	f32 planar_epsilon = 1e-6f;
	f32 edge_epsilon = 1e-6f;
	f32 min_distance = 1e-6f;
};

forceinline Optional<RaycastHit<v3f>> raycast(ray<v3f> ray, triangle<v3f> tri, RaycastTriangleOptions options = {}) {
	v3f e1 = tri.b - tri.a;
    v3f e2 = tri.c - tri.a;

    v3f pvec = cross(ray.direction, e2);
    f32 det = dot(e1, pvec);

    if (absolute(det) < options.planar_epsilon) {
		return {};
    }

    f32 inv_det = 1 / det;
    v3f tvec = ray.origin - tri.a;
    f32 u = dot(tvec, pvec) * inv_det;
	if (u < options.edge_epsilon || u > 1 - options.edge_epsilon) {
		return {};
	}

    v3f qvec = cross(tvec, e1);
    f32 v = dot(ray.direction, qvec) * inv_det;
	if (v < options.edge_epsilon || u + v > 1 - options.edge_epsilon) {
		return {};
	}
    f32 t = dot(e2, qvec) * inv_det;

	if (t < options.min_distance)
		return {};

	RaycastHit<v3f> hit;
	hit.distance = t;
	hit.position = ray.origin + ray.direction* t;
	hit.normal = normalize(cross(e1, e2));
	return hit;
}

struct RaycastAABBOptions {
	bool from_inside = true;
};

// result[0] is the closest one
// If ray intersects box from inside then result.count is 1
forceinline StaticList<RaycastHit<v2f>, 2> raycast(ray<v2f> ray, aabb<v2f> box, RaycastAABBOptions options = {}) {
	v2f inv_dir = 1.0f / ray.direction;
	v2f t1 = (box.min - ray.origin) * inv_dir;
	v2f t2 = (box.max - ray.origin) * inv_dir;
	
	f32 tmin = max(min(t1, t2));
	f32 tmax = min(max(t1, t2));

	if (tmax < 0 || tmin > tmax) {
		return {};
	}

	auto get_hit = [&] (f32 t) {
		RaycastHit<v2f> hit = {};

		hit.position = ray.origin + ray.direction * t;

		hit.distance = t;

		       if (t == t1.x)   hit.normal = {-1, 0};
		else   if (t == t2.x)   hit.normal = { 1, 0};
		else   if (t == t1.y)   hit.normal = { 0,-1};
		else /*if (t == t2.y)*/ hit.normal = { 0, 1};

		return hit;
	};

	StaticList<RaycastHit<v2f>, 2> result;
	if (tmin >= 0) {
		result.add(get_hit(tmin));
	}
	result.add(get_hit(tmax));
	return result;
}

// result[0] is the closest one
// If ray intersects box from inside then result.count is 1
forceinline StaticList<RaycastHit<v3f>, 2> raycast(ray<v3f> ray, aabb<v3f> box, RaycastAABBOptions options = {}) {
	v3f inv_dir = 1.0f / ray.direction;
	v3f t1 = (box.min - ray.origin) * inv_dir;
	v3f t2 = (box.max - ray.origin) * inv_dir;
	
	f32 tmin = max(min(t1, t2));
	f32 tmax = min(max(t1, t2));

	if (tmax < 0 || tmin > tmax) {
		return {};
	}

	auto get_hit = [&] (f32 t) {
		RaycastHit<v3f> hit = {};
		hit.position = ray.origin + ray.direction * t;
		hit.distance = t;

		       if (t == t1.x)   hit.normal = {-1, 0, 0};
		else   if (t == t2.x)   hit.normal = { 1, 0, 0};
		else   if (t == t1.y)   hit.normal = { 0,-1, 0};
		else   if (t == t2.y)   hit.normal = { 0, 1, 0};
		else   if (t == t1.z)   hit.normal = { 0, 0,-1};
		else /*if (t == t2.z)*/ hit.normal = { 0, 0, 1};

		return hit;
	};
		
	StaticList<RaycastHit<v3f>, 2> result;
	if (tmin >= 0) {
		result.add(get_hit(tmin));
	}
	result.add(get_hit(tmax));
	return result;
}

union m4 {
	using Scalar = f32;
	using Vector = v4f;
	struct {
		simd::f32x4 im, jm, km, lm;
	};
	struct {
		v4f i, j, k, l;
	};
	simd::f32x4 m[4];
	v4f vectors[4];
	f32 s[16];
	forceinline v4f operator*(v4f b) const {
		// v4f x = V4f(b.x) * i;
		// v4f y = V4f(b.y) * j;
		// v4f z = V4f(b.z) * k;
		// v4f w = V4f(b.w) * l;
		// return x + y + z + w;

		union {
			v4f v;
			simd::f32x4 f;
		};
		f = f32x4_add(
			f32x4_muladd(f32x4_set1(b.x), im, f32x4_mul(f32x4_set1(b.y), jm)),
			f32x4_muladd(f32x4_set1(b.z), km, f32x4_mul(f32x4_set1(b.w), lm))
		);
		return v;
	}
	forceinline v3f operator*(v3f b) const {
		auto x = V4f(b.x) * i;
		auto y = V4f(b.y) * j;
		auto z = V4f(b.z) * k;
		auto r = x + y + z;
		v3f result;
		memcpy(&result, &r, 12);
		return result;
	}
	forceinline m4 operator*(m4 b) const {
		// return {
		// 	.i = b.i.x*i + b.i.y*j + b.i.z*k + b.i.w*l,
		// 	.j = b.j.x*i + b.j.y*j + b.j.z*k + b.j.w*l,
		// 	.k = b.k.x*i + b.k.y*j + b.k.z*k + b.k.w*l,
		// 	.l = b.l.x*i + b.l.y*j + b.l.z*k + b.l.w*l,
		// };
		return {
			.im = f32x4_add(f32x4_muladd(f32x4_set1(b.i.x), im, f32x4_mul(f32x4_set1(b.i.y), jm)), f32x4_muladd(f32x4_set1(b.i.z), km, f32x4_mul(f32x4_set1(b.i.w), lm))),
			.jm = f32x4_add(f32x4_muladd(f32x4_set1(b.j.x), im, f32x4_mul(f32x4_set1(b.j.y), jm)), f32x4_muladd(f32x4_set1(b.j.z), km, f32x4_mul(f32x4_set1(b.j.w), lm))),
			.km = f32x4_add(f32x4_muladd(f32x4_set1(b.k.x), im, f32x4_mul(f32x4_set1(b.k.y), jm)), f32x4_muladd(f32x4_set1(b.k.z), km, f32x4_mul(f32x4_set1(b.k.w), lm))),
			.lm = f32x4_add(f32x4_muladd(f32x4_set1(b.l.x), im, f32x4_mul(f32x4_set1(b.l.y), jm)), f32x4_muladd(f32x4_set1(b.l.z), km, f32x4_mul(f32x4_set1(b.l.w), lm))),
		};

		// "smart" compiler could not figure this out...
		// it is so smart that even after i directly said it what to do,
		// it still produces 68 instructions half of which just shuffle the registers!!
		// btw on platforms with fma all of this is doable in just 32 instructions.

		// xmm0 = i
		// xmm1 = j
		// xmm2 = k
		// xmm3 = l
		// xmm4 = b.i
		// xmm5 = b.j
		// xmm6 = b.k
		// xmm7 = b.l
		// compute b.i.x*i + b.i.y*j + b.i.z*k + b.i.w*l
		// xmm9 = b.i.x // just shuffle
		// xmm8 = xmm9*xmm0
		// xmm9 = b.i.y
		// xmm8 = xmm9*xmm1 + xmm8
		// xmm9 = b.i.z
		// xmm8 = xmm9*xmm2 + xmm8
		// xmm9 = b.i.w
		// xmm8 = xmm9*xmm3 + xmm8
		// do same thing for j, k and l.

	}
	forceinline m4& operator*=(m4 b) { return *this = *this * b; }
	static forceinline m4 scale(f32 x, f32 y, f32 z) {
		return {.s = {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1,
		}};
	}
	static forceinline m4 identity() {
		return {.s = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		}};
	}
	static forceinline m4 scale(v3f v) { return scale(v.x, v.y, v.z); }
	static forceinline m4 scale(v2f xy, f32 z) { return scale(xy.x, xy.y, z); }
	static forceinline m4 scale(f32 v) { return scale(v, v, v); }
	static forceinline m4 translation(f32 x, f32 y, f32 z) {
		return {.s = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1,
		}};
	}
	static forceinline m4 translation(v3f v) { return translation(v.x, v.y, v.z); }
	static forceinline m4 translation(v2f xy, f32 z) { return translation(xy.x, xy.y, z); }
	static forceinline m4 translation(f32 v) { return translation(v, v, v); }
	static forceinline m4 perspective_left_handed(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 h	   = 1.0f / tanf(fov * 0.5f);
		f32 w	   = h / aspect;
		f32 fzdfmn = fz / (fz - nz);
		return {.s = {
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, fzdfmn, 1,
			0, 0, -fzdfmn * nz, 0,
		}};
	}
	static forceinline m4 perspective_right_handed(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 xymax = nz * tan(fov * 0.5f);
		f32 depth = fz - nz;
		f32 q = -(fz + nz) / depth;
		f32 qn = -2 * (fz * nz) / depth;
		f32 h = nz / xymax;
		f32 w = h / aspect;

		return {.s = {
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, q, -1,
			0, 0, qn, 0,
		}};
	}
	static forceinline m4 ortho_right_handed(f32 height, f32 aspect, f32 nz, f32 fz) {
		f32 h = 1.0f / (height * 0.5f);
		f32 rz = fz - nz;
		return {.s = {
			h / aspect, 0, 0, 0,
			0, h, 0, 0,
			0, 0, 1.0f / rz, 0,
			0, 0, -nz / rz, 1,
		}};
	}
	static forceinline m4 rotation_r_x(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {.s = {
			1, 0, 0, 0,
			0, c, s, 0,
			0,-s, c, 0,
			0, 0, 0, 1,
		}};
	}
	static forceinline m4 rotation_r_y(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {.s = {
			c, 0,-s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1,
		}};
	}
	static forceinline m4 rotation_r_z(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {.s = {
			 c, s, 0, 0,
			-s, c, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1,
		}};
	}
	static forceinline m4 rotation_r_zxy(v3f a) {
		v3f s;
		v3f c;
		cos_sin(a, c, s);
		return {.s = {
			 c.z*c.y + s.z*s.x*s.y, s.z*c.x,  c.z*-s.y + s.z*s.x*c.y, 0,
			-s.z*c.y + c.z*s.x*s.y, c.z*c.x, -s.z*-s.y + c.z*s.x*c.y, 0,
			               c.x*s.y,    -s.x,                 c.x*c.y, 0,
			                     0,       0,                       0, 1,
		}};
	}
	static forceinline m4 rotation_r_zxy(f32 x, f32 y, f32 z) { return rotation_r_zxy({x, y, z}); }
	static forceinline m4 rotation_r_yxz(v3f a) {
		v3f s;
		v3f c;
		cos_sin(a, c, s);
		return {.s = {
			c.y*c.z + -s.y*-s.x*-s.z, c.y*s.z + -s.y*-s.x*c.z, -s.y*c.x, 0,
			                c.x*-s.z,                 c.x*c.z,      s.x, 0,
			 s.y*c.z + c.y*-s.x*-s.z,  s.y*s.z + c.y*-s.x*c.z,  c.y*c.x, 0,
			                       0,                       0,        0, 1,
		}};
	}
	static forceinline m4 rotation_r_yxz(f32 x, f32 y, f32 z) { return rotation_r_yxz({x, y, z}); }
};


forceinline m4 to_m4(m3 m) {
	return {.s = {
		m.i.x, m.i.y, m.i.z, 0,
		m.j.x, m.j.y, m.j.z, 0,
		m.k.x, m.k.y, m.k.z, 0,
			0,     0,     0, 1,
	}};
}

forceinline constexpr m3 transpose(m3 const& m) {
	return {.s = {
		m.i.x, m.j.x, m.k.x,
		m.i.y, m.j.y, m.k.y,
		m.i.z, m.j.z, m.k.z,
	}};
}
forceinline m4 transpose(m4 const& m) {
	using namespace simd;
	f32x4 tmp0 = _mm_unpacklo_ps(m.im, m.jm);
	f32x4 tmp1 = _mm_unpackhi_ps(m.im, m.jm);
	f32x4 tmp2 = _mm_unpacklo_ps(m.km, m.lm);
	f32x4 tmp3 = _mm_unpackhi_ps(m.km, m.lm);

	m4 result;
	result.im = _mm_movelh_ps(tmp0, tmp2);
	result.jm = _mm_movehl_ps(tmp2, tmp0);
	result.km = _mm_movelh_ps(tmp1, tmp3);
	result.lm = _mm_movehl_ps(tmp3, tmp1);
	return result;
}

forceinline m3 inverse(m3 const &m) {
	// computes the inverse of a matrix m
	f32 det = m.i.x * (m.j.y * m.k.z - m.k.y * m.j.z) -
	          m.i.y * (m.j.x * m.k.z - m.j.z * m.k.x) +
	          m.i.z * (m.j.x * m.k.y - m.j.y * m.k.x);

	f32 invdet = 1 / det;

	return {
		(m.j.y * m.k.z - m.k.y * m.j.z) * invdet,
		(m.i.z * m.k.y - m.i.y * m.k.z) * invdet,
		(m.i.y * m.j.z - m.i.z * m.j.y) * invdet,
		(m.j.z * m.k.x - m.j.x * m.k.z) * invdet,
		(m.i.x * m.k.z - m.i.z * m.k.x) * invdet,
		(m.j.x * m.i.z - m.i.x * m.j.z) * invdet,
		(m.j.x * m.k.y - m.k.x * m.j.y) * invdet,
		(m.k.x * m.i.y - m.i.x * m.k.y) * invdet,
		(m.i.x * m.j.y - m.j.x * m.i.y) * invdet,
	};
}

forceinline m4 inverse(m4 const &m) {
	f32 A2323 = m.k.z * m.l.w - m.k.w * m.l.z;
	f32 A1323 = m.k.y * m.l.w - m.k.w * m.l.y;
	f32 A1223 = m.k.y * m.l.z - m.k.z * m.l.y;
	f32 A0323 = m.k.x * m.l.w - m.k.w * m.l.x;
	f32 A0223 = m.k.x * m.l.z - m.k.z * m.l.x;
	f32 A0123 = m.k.x * m.l.y - m.k.y * m.l.x;
	f32 A2313 = m.j.z * m.l.w - m.j.w * m.l.z;
	f32 A1313 = m.j.y * m.l.w - m.j.w * m.l.y;
	f32 A1213 = m.j.y * m.l.z - m.j.z * m.l.y;
	f32 A2312 = m.j.z * m.k.w - m.j.w * m.k.z;
	f32 A1312 = m.j.y * m.k.w - m.j.w * m.k.y;
	f32 A1212 = m.j.y * m.k.z - m.j.z * m.k.y;
	f32 A0313 = m.j.x * m.l.w - m.j.w * m.l.x;
	f32 A0213 = m.j.x * m.l.z - m.j.z * m.l.x;
	f32 A0312 = m.j.x * m.k.w - m.j.w * m.k.x;
	f32 A0212 = m.j.x * m.k.z - m.j.z * m.k.x;
	f32 A0113 = m.j.x * m.l.y - m.j.y * m.l.x;
	f32 A0112 = m.j.x * m.k.y - m.j.y * m.k.x;

	f32 det = m.i.x * (m.j.y * A2323 - m.j.z * A1323 + m.j.w * A1223)
		    - m.i.y * (m.j.x * A2323 - m.j.z * A0323 + m.j.w * A0223)
		    + m.i.z * (m.j.x * A1323 - m.j.y * A0323 + m.j.w * A0123)
		    - m.i.w * (m.j.x * A1223 - m.j.y * A0223 + m.j.z * A0123);

	if (det == 0)
		return m4{}; // gcc does not understand `return {};`

	det = 1 / det;

	return m4 {.s = {
		det *  (m.j.y * A2323 - m.j.z * A1323 + m.j.w * A1223),
		det * -(m.i.y * A2323 - m.i.z * A1323 + m.i.w * A1223),
		det *  (m.i.y * A2313 - m.i.z * A1313 + m.i.w * A1213),
		det * -(m.i.y * A2312 - m.i.z * A1312 + m.i.w * A1212),
		det * -(m.j.x * A2323 - m.j.z * A0323 + m.j.w * A0223),
		det *  (m.i.x * A2323 - m.i.z * A0323 + m.i.w * A0223),
		det * -(m.i.x * A2313 - m.i.z * A0313 + m.i.w * A0213),
		det *  (m.i.x * A2312 - m.i.z * A0312 + m.i.w * A0212),
		det *  (m.j.x * A1323 - m.j.y * A0323 + m.j.w * A0123),
		det * -(m.i.x * A1323 - m.i.y * A0323 + m.i.w * A0123),
		det *  (m.i.x * A1313 - m.i.y * A0313 + m.i.w * A0113),
		det * -(m.i.x * A1312 - m.i.y * A0312 + m.i.w * A0112),
		det * -(m.j.x * A1223 - m.j.y * A0223 + m.j.z * A0123),
		det *  (m.i.x * A1223 - m.i.y * A0223 + m.i.z * A0123),
		det * -(m.i.x * A1213 - m.i.y * A0213 + m.i.z * A0113),
		det *  (m.i.x * A1212 - m.i.y * A0212 + m.i.z * A0112),
	}};
}

forceinline constexpr m4 M4(f32 v = 0.0f) { return m4{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; }
forceinline constexpr m4 M4(v4f i, v4f j, v4f k, v4f l) { return m4{.i=i, .j=j, .k=k, .l=l}; }
forceinline constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l) { return m4{.im=i, .jm=j, .km=k, .lm=l}; }
forceinline constexpr m4 M4(
	f32 ix, f32 iy, f32 iz, f32 iw,
	f32 jx, f32 jy, f32 jz, f32 jw,
	f32 kx, f32 ky, f32 kz, f32 kw,
	f32 lx, f32 ly, f32 lz, f32 lw
) {
	return {.s = {
		ix, iy, iz, iw,
		jx, jy, jz, jw,
		kx, ky, kz, kw,
		lx, ly, lz, lw
	}};
}

forceinline m4 M4(m3 v) {
	return m4{.vectors = {
		V4f(v.i, 0),
		V4f(v.j, 0),
		V4f(v.k, 0),
		V4f(0, 0, 0, 1)
	}};
}

using FrustumPlanes = Array<v4f, 6>;

forceinline FrustumPlanes create_frustum_planes_d3d(m4 m) {
	FrustumPlanes planes;

	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	planes[0].x = m.i.w + m.i.x;
	planes[0].y = m.j.w + m.j.x;
	planes[0].z = m.k.w + m.k.x;
	planes[0].w = m.l.w + m.l.x;
	planes[1].x = m.i.w - m.i.x;
	planes[1].y = m.j.w - m.j.x;
	planes[1].z = m.k.w - m.k.x;
	planes[1].w = m.l.w - m.l.x;
	planes[2].x = m.i.w + m.i.y;
	planes[2].y = m.j.w + m.j.y;
	planes[2].z = m.k.w + m.k.y;
	planes[2].w = m.l.w + m.l.y;
	planes[3].x = m.i.w - m.i.y;
	planes[3].y = m.j.w - m.j.y;
	planes[3].z = m.k.w - m.k.y;
	planes[3].w = m.l.w - m.l.y;
	planes[4].x = m.i.z;
	planes[4].y = m.j.z;
	planes[4].z = m.k.z;
	planes[4].w = m.l.z;
	planes[5].x = m.i.w - m.i.z;
	planes[5].y = m.j.w - m.j.z;
	planes[5].z = m.k.w - m.k.z;
	planes[5].w = m.l.w - m.l.z;

	for (auto& p : planes) {
		p /= length(v3f{p.x, p.y, p.z});
	}
	return planes;
}
forceinline FrustumPlanes create_frustum_planes_gl(m4 m) {
	FrustumPlanes planes;

	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	planes[0].x = m.i.w + m.i.x;
	planes[0].y = m.j.w + m.j.x;
	planes[0].z = m.k.w + m.k.x;
	planes[0].w = m.l.w + m.l.x;
	planes[1].x = m.i.w - m.i.x;
	planes[1].y = m.j.w - m.j.x;
	planes[1].z = m.k.w - m.k.x;
	planes[1].w = m.l.w - m.l.x;
	planes[2].x = m.i.w + m.i.y;
	planes[2].y = m.j.w + m.j.y;
	planes[2].z = m.k.w + m.k.y;
	planes[2].w = m.l.w + m.l.y;
	planes[3].x = m.i.w - m.i.y;
	planes[3].y = m.j.w - m.j.y;
	planes[3].z = m.k.w - m.k.y;
	planes[3].w = m.l.w - m.l.y;
	planes[4].x = m.i.w + m.i.z;
	planes[4].y = m.j.w + m.j.z;
	planes[4].z = m.k.w + m.k.z;
	planes[4].w = m.l.w + m.l.z;
	planes[5].x = m.i.w - m.i.z;
	planes[5].y = m.j.w - m.j.z;
	planes[5].z = m.k.w - m.k.z;
	planes[5].w = m.l.w - m.l.z;

	for (auto& p : planes) {
		p /= length(v3f{p.x, p.y, p.z});
	}
	return planes;
}
forceinline bool contains_sphere(FrustumPlanes const &planes, v3f position, f32 radius) {
#if ARCH_AVX2
	using namespace simd;
	f32x8 plane_x = f32x8_set(planes[0].x, planes[1].x, planes[2].x, planes[3].x, planes[4].x, planes[5].x, 0, 0);
	f32x8 plane_y = f32x8_set(planes[0].y, planes[1].y, planes[2].y, planes[3].y, planes[4].y, planes[5].y, 0, 0);
	f32x8 plane_z = f32x8_set(planes[0].z, planes[1].z, planes[2].z, planes[3].z, planes[4].z, planes[5].z, 0, 0);
	f32x8 plane_w = f32x8_set(planes[0].w, planes[1].w, planes[2].w, planes[3].w, planes[4].w, planes[5].w, 0, 0);
	f32x8 position_x = f32x8_set1(position.x);
	f32x8 position_y = f32x8_set1(position.y);
	f32x8 position_z = f32x8_set1(position.z);
	f32x8 negative_radius = f32x8_set1(-radius);

	f32x8 f = f32x8_add(f32x8_muladd(plane_x, position_x, f32x8_muladd(plane_y, position_y, f32x8_mul(plane_z, position_z))), plane_w);
	auto mask = b32x8_get_mask(f32x8_lt(f, f32x8_set1(-radius)));
	return !(mask & 0b11111100);
#else
	for (auto p : planes) {
		if (dot(v3f{p.x, p.y, p.z}, position) + p.w < -radius) {
			return false;
		}
	}
	return true;
#endif
}

forceinline f32 sdf_torus(v3f point, f32 radius, f32 thickness) {
	return absolute(length(point) - radius) - thickness;
}

// https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
// https://www.shadertoy.com/view/4sS3zz
forceinline f32 sdf_ellipse(v2f point, v2f radius) {
	v2f p = point;
	v2f ab = radius;
	if (ab.x == ab.y)
		return length(p) - ab.x;

	p = absolute(p);
	if (p.x > p.y) {
		p = p.yx();
		ab = ab.yx();
	}

	float l = ab.y * ab.y - ab.x * ab.x;

	float m = ab.x * p.x / l;
	float n = ab.y * p.y / l;
	float m2 = m * m;
	float n2 = n * n;

	float c = (m2 + n2 - 1.0f) / 3.0f;
	float c3 = c * c * c;

	float d = c3 + m2 * n2;
	float q = d + m2 * n2;
	float g = m + m * n2;

	float co;

	auto msign = [](float x) { return (x < 0.0f) ? -1.0f : 1.0f; };

	if (d < 0.0f) {
		float h = acos(q / c3) / 3.0f;
		float s = cos(h) + 2.0f;
		float t = sin(h) * sqrt(3.0f);
		float rx = sqrt(m2 - c * (s + t));
		float ry = sqrt(m2 - c * (s - t));
		co = ry + sign(l) * rx + absolute(g) / (rx * ry);
	} else {
		float h = 2.0f * m * n * sqrt(d);
		float s = msign(q + h) * powf(absolute(q + h), 1.0f / 3.0f);
		float t = msign(q - h) * powf(absolute(q - h), 1.0f / 3.0f);
		float rx = -(s + t) - c * 4.0f + 2.0f * m2;
		float ry = (s - t) * sqrt(3.0f);
		float rm = sqrt(rx * rx + ry * ry);
		co = ry / sqrt(rm - rx) + 2.0f * g / rm;
	}
	co = (co - m) / 2.0f;

	float si = sqrt(max(1.0f - co * co, 0.0f));

	v2f r = ab * v2f{co,si};

	return length(r - p) * msign(p.y - r.y);
}

namespace ce {

forceinline constexpr v4s frac(v4s v, s32 step) {
	return {
		tl::frac(v.x, step),
		tl::frac(v.y, step),
		tl::frac(v.z, step),
		tl::frac(v.w, step),
	};
}

} // namespace ce

forceinline umm append(StringBuilder &builder, FormatFloat<v2f> f) {
	return append_format(builder, "({}, {})",
		FormatFloat{.value = (f32)f.value.x, .precision = f.precision, .format = f.format},
		FormatFloat{.value = (f32)f.value.y, .precision = f.precision, .format = f.format}
	);
}

forceinline umm append(StringBuilder &builder, m4 m) {
	return append_format(builder, "({}, {}, {}, {})", m.i, m.j, m.k, m.l);
}

template <class T>
umm append(StringBuilder &builder, aabb<T> v) {
	return append_format(builder, "(min={}, max={})"s, v.min, v.max);
}

template <class T>
umm append(StringBuilder &builder, ray<T> r) {
	return append_format(builder, "(origin={}, direction={})", r.origin, r.direction);
}

template <class T>
forceinline T saturate(T t) {
	return clamp(t, convert<T>(0), convert<T>(1));
}

forceinline auto smoothstep3_unclamped(auto x) { return (3 - 2 * x) * x * x; }
forceinline auto smoothstep5_unclamped(auto x) { return x*x*x*(x*(6*x - 15) + 10); }
template <class T> forceinline auto smoothstep3(T x) { return smoothstep3_unclamped(clamp(x, convert<T>(0), convert<T>(1))); }
template <class T> forceinline auto smoothstep5(T x) { return smoothstep5_unclamped(clamp(x, convert<T>(0), convert<T>(1))); }

} // namespace tl

#undef MOP
#undef MOPS
#undef OPS
#undef SOP

#undef MM_SHUFFLE
#undef MM256_PERM128

#undef f32x4_lt
#undef f32x4_gt
#undef f32x4_le
#undef f32x4_ge
#undef f32x4_eq
#undef f32x4_ne
#undef f64x2_lt
#undef f64x2_gt
#undef f64x2_le
#undef f64x2_ge
#undef f64x2_eq
#undef f64x2_ne

#if COMPILER_MSVC
#pragma warning(pop)
#endif

template <class T>
tl::u64 get_hash(tl::aabb<T> const &x) {
	return 
		get_hash(x.min) * 13043817825332782231ull +
		get_hash(x.max) * 6521908912666391129ull;
}
