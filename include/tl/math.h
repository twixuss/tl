#pragma once
#include "common.h"
#include "array.h"
#include "simd.h"
#include "string.h"
#include "vector.h"

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

constexpr f32 pi     = f32(3.1415926535897932384626433832795L);
constexpr f32 tau    = f32(6.283185307179586476925286766559L);
constexpr f32 inv_pi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2  = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3  = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5  = f32(2.2360679774997896964091736687313L);

template <class T> forceinline constexpr auto radians(T deg) { return deg * (pi / 180.0f); }
template <class T> forceinline constexpr auto degrees(T rad) { return rad * (180.0f / pi); }

template <class T>
forceinline constexpr auto clamp(T value, T min_bound, T max_bound) {
	minmax(min_bound, max_bound, min_bound, max_bound);
	return min(max(value, min_bound), max_bound);
}

// Does not check if min_bound is greater than max_bound
template <class T>
forceinline constexpr auto clamp_unchecked(T value, T min_bound, T max_bound) {
	return min(max(value, min_bound), max_bound);
}

template <class T>
forceinline constexpr auto map(T value, T source_min, T source_max, T dest_min, T dest_max) {
	if constexpr (is_integer_like<T>) { // Do multiplication first
		return (value - source_min) * (dest_max - dest_min) / (source_max - source_min) + dest_min;
	} else {
		return (value - source_min) / (source_max - source_min) * (dest_max - dest_min) + dest_min;
	}
}
template <class T>
forceinline constexpr auto map_clamped(T value, T source_min, T source_max, T dest_min, T dest_max) {
	return map(clamp(value, source_min, source_max), source_min, source_max, dest_min, dest_max);
}
template <class T> forceinline constexpr auto lerp(T a, T b, T t) { return a + (b - a) * t; }
template <class M, class T> forceinline T &mask_assign(M mask, T &dst, T src) { return dst = select(mask, src, dst); }
template <class T> forceinline constexpr auto pow2(T v) { return v * v; }
template <class T> forceinline constexpr auto pow3(T v) { return v * v * v; }
template <class T> forceinline constexpr auto pow4(T v) { return pow2(v * v); }

template <class T>
inline constexpr T smooth_min(T a, T b, f32 k) {
	f32 h = max(0, min(1, (b - a) / k + .5f));
	f32 m = h * (1 - h) * k;
	return lerp(b, a, h) - m * .5f;
}

template <class T>
inline constexpr T smooth_max(T a, T b, f32 k) {
	return smooth_min(a, b, -k);
}

namespace CE {

template <class To, class From>
forceinline constexpr To cvt(From v) {
	return (To)v;
}

} // namespace CE

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
		cos_sin(a, s, c);
		return {
			c, s,
		   -s, c,
		};
	}
};

template<> inline static constexpr v2f min_value<v2f> = {min_value<v2f::Scalar>, min_value<v2f::Scalar>};
template<> inline static constexpr v2f max_value<v2f> = {max_value<v2f::Scalar>, max_value<v2f::Scalar>};

template<> inline static constexpr v2s min_value<v2s> = {min_value<v2s::Scalar>, min_value<v2s::Scalar>};
template<> inline static constexpr v2s max_value<v2s> = {max_value<v2s::Scalar>, max_value<v2s::Scalar>};

template<> inline static constexpr v2u min_value<v2u> = {min_value<v2u::Scalar>, min_value<v2u::Scalar>};
template<> inline static constexpr v2u max_value<v2u> = {max_value<v2u::Scalar>, max_value<v2u::Scalar>};

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

namespace CE {

template <class T>
constexpr T sqrt_newton_raphson(T x, T curr, T prev) {
    return curr == prev ? curr : sqrt_newton_raphson(x, (T)(0.5 * (curr + x / curr)), curr);
};

template <class T>
constexpr auto sqrt(T x) {
	return sqrt_newton_raphson(x, x, T{});
}

template <class T>
constexpr auto length(T a) {
	return CE::sqrt(dot(a, a));
}
template <class T>
constexpr auto normalize(T a) {
	return a * (1.0f / CE::length(a));
}

} // namespace CE

namespace CE {

#define CVT(v2f, v2s, s32) template<> constexpr v2s cvt(v2f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y)}; }
CVT(v2f, v2s, s32) CVT(v2f, v2u, u32)
CVT(v2s, v2f, f32) CVT(v2s, v2u, u32)
CVT(v2u, v2f, f32) CVT(v2u, v2s, s32)
#undef CVT

#define CVT(v3f, v3s, s32) template<> constexpr v3s cvt(v3f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z)}; }
CVT(v3f, v3s, s32) CVT(v3f, v3u, u32)
CVT(v3s, v3f, f32) CVT(v3s, v3u, u32)
CVT(v3u, v3f, f32) CVT(v3u, v3s, s32)
#undef CVT

#define CVT(v4f, v4s, s32) template<> constexpr v4s cvt(v4f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z), CE::cvt<s32>(v.w)}; }
CVT(v4f, v4s, s32) CVT(v4f, v4u, u32)
CVT(v4s, v4f, f32) CVT(v4s, v4u, u32)
CVT(v4u, v4f, f32) CVT(v4u, v4s, s32)
#undef CVT
} // namespace CE

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

#define MINMAX(v2f, V2f)						  \
	forceinline auto min(v2f a) { return min(a.x, a.y); }\
	forceinline auto max(v2f a) { return max(a.x, a.y); }\
	forceinline auto min(v2f a, v2f b) { return V2f(min(a.x, b.x), min(a.y, b.y)); }\
	forceinline auto max(v2f a, v2f b) { return V2f(max(a.x, b.x), max(a.y, b.y)); }\
	forceinline void minmax(v2f a, v2f b, v2f& mn, v2f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
	}
MINMAX(v2f, V2f)
MINMAX(v2s, V2s)
MINMAX(v2u, V2u)
#undef MINMAX

#define MINMAX(v3f, V3f)						  \
	forceinline auto min(v3f a) { return min(a.x, a.y, a.z); }\
	forceinline auto max(v3f a) { return max(a.x, a.y, a.z); }\
	forceinline auto min(v3f a, v3f b) { return V3f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }\
	forceinline auto max(v3f a, v3f b) { return V3f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }\
	forceinline void minmax(v3f a, v3f b, v3f& mn, v3f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
	}
MINMAX(v3f, V3f)
MINMAX(v3s, V3s)
MINMAX(v3u, V3u)
#undef MINMAX

#define MINMAX(v4f, V4f)						  \
	forceinline auto min(v4f a) { return min(a.x, a.y, a.z, a.w); }\
	forceinline auto max(v4f a) { return max(a.x, a.y, a.z, a.w); }\
	forceinline auto min(v4f a, v4f b) { return V4f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w)); }\
	forceinline auto max(v4f a, v4f b) { return V4f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w)); }\
	forceinline void minmax(v4f a, v4f b, v4f& mn, v4f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
		minmax(a.w, b.w, mn.w, mx.w); 		  \
	}
MINMAX(v4f, V4f)
MINMAX(v4s, V4s)
MINMAX(v4u, V4u)
#undef MINMAX

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

forceinline f32 floor(f32 v) { return ::floorf(v); }
forceinline f64 floor(f64 v) { return ::floor(v); }
forceinline v2f floor(v2f v) { return { floor(v.x), floor(v.y) }; }
forceinline v3f floor(v3f v) { return { floor(v.x), floor(v.y), floor(v.z) }; }
forceinline v4f floor(v4f v) { return { floor(v.x), floor(v.y), floor(v.z), floor(v.w) }; }

forceinline v2u floor_to_power_of_2(v2u v) { return {floor_to_power_of_2(v.x), floor_to_power_of_2(v.y)}; }

#define FRAC(f32) forceinline f32 frac(f32 v) { return v - floor(v); }
FRAC(f32)
FRAC(f64)
#undef FRAC

forceinline v2f frac(v2f v) { return { frac(v.x), frac(v.y) }; }
forceinline v3f frac(v3f v) { return { frac(v.x), frac(v.y), frac(v.z) }; }
forceinline v4f frac(v4f v) { return { frac(v.x), frac(v.y), frac(v.z), frac(v.w) }; }

forceinline v2s frac(v2s v, v2s s) {
	__m128i vm = _mm_setr_epi32(v.x, v.y, 0, 0);
	__m128i sm = _mm_setr_epi32(s.x, s.y, 0, 0);
	__m128d vlo = _mm_cvtepi32_pd(vm);
	__m128d slo = _mm_cvtepi32_pd(sm);
	__m128i result = _mm_cvttpd_epi32(_mm_floor_pd(_mm_div_pd(vlo, slo)));
	result = _mm_sub_epi32(vm, _mm_mullo_epi32(result, sm));
	return *(v2s *)&result;
}
forceinline v3s frac(v3s v, v3s s) {
	return {
		frac(v.x, s.x),
		frac(v.y, s.y),
		frac(v.z, s.z),
	};
}
forceinline v4s frac(v4s v, v4s step) {
	return {
		frac(v.x, step.x),
		frac(v.y, step.y),
		frac(v.z, step.z),
		frac(v.w, step.w),
	};
}
forceinline v2s frac(v2s v, s32 s) { return frac(v, V2s(s)); }
forceinline v3s frac(v3s v, s32 s) { return frac(v, V3s(s)); }
forceinline v4s frac(v4s v, s32 s) { return frac(v, V4s(s)); }

#define FLOOR(v2s, s32, V2s) 		   \
forceinline v2s floor(v2s v, v2s s) {  \
	return { 						   \
		floor(v.x, s.x), 			   \
		floor(v.y, s.y), 			   \
	}; 								   \
} 									   \
forceinline v2s floor(v2s v, s32 s) { return floor(v, V2s(s)); }
FLOOR(v2s, s32, V2s)
FLOOR(v2u, u32, V2u)
#undef FLOOR

#define FLOOR(v3s, s32, V3s) 		  \
forceinline v3s floor(v3s v, v3s s) { \
	return { 						  \
		floor(v.x, s.x), 			  \
		floor(v.y, s.y), 			  \
		floor(v.z, s.z), 			  \
	}; 								  \
} 									  \
forceinline v3s floor(v3s v, s32 s) { return floor(v, V3s(s)); }
FLOOR(v3s, s32, V3s)
FLOOR(v3u, u32, V3u)
#undef FLOOR

#define FLOOR(v4s, s32, V4s) 		  \
forceinline v4s floor(v4s v, v4s s) { \
	return {						  \
		floor(v.x, s.x),			  \
		floor(v.y, s.y),			  \
		floor(v.z, s.z),			  \
		floor(v.w, s.w),			  \
	};								  \
}									  \
forceinline v4s floor(v4s v, s32 s) { return floor(v, V4s(s)); }
FLOOR(v4s, s32, V4s)
FLOOR(v4u, u32, V4u)
#undef FLOOR

forceinline s32 floor_to_int(f32 v) { return (s32)floor(v); }
forceinline s64 floor_to_int(f64 v) { return (s64)floor(v); }
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

forceinline f32 round(f32 v) { return ::roundf(v); }
forceinline v2f round(v2f v) { return {round(v.x), round(v.y)}; }
forceinline v3f round(v3f v) { return {round(v.x), round(v.y), round(v.z)}; }
forceinline v4f round(v4f v) { return {round(v.x), round(v.y), round(v.z), round(v.w)}; }

forceinline s32 round_to_int(f32 v) { return (s32)lroundf(v); }
forceinline s64 round_to_int(f64 v) { return llround(v); }
forceinline v2s round_to_int(v2f v) { return {round_to_int(v.x), round_to_int(v.y)}; }
forceinline v3s round_to_int(v3f v) { return {round_to_int(v.x), round_to_int(v.y), round_to_int(v.z)}; }
forceinline v4s round_to_int(v4f v) { return {round_to_int(v.x), round_to_int(v.y), round_to_int(v.z), round_to_int(v.w)}; }

template <class Int, class T>
forceinline constexpr auto lerp_int(Int a, Int b, T t) {
	auto r = lerp((T)a, (T)b, t);
	if (a > b) {
		return floor_to_int(r);
	} else {
		return ceil_to_int(r);
	}
}
forceinline f32 absolute(f32 v) { return *(u32*)&v &= 0x7FFFFFFF, v; }
forceinline v2f absolute(v2f v) { return {absolute(v.x), absolute(v.y)}; }
forceinline v3f absolute(v3f v) { return {absolute(v.x), absolute(v.y), absolute(v.z)}; }
forceinline v4f absolute(v4f v) { return {absolute(v.x), absolute(v.y), absolute(v.z), absolute(v.w)}; }

forceinline s32 absolute(s32 v) { return v < 0 ? -v :v; }
forceinline v2s absolute(v2s a) { return {absolute(a.x), absolute(a.y)}; }
forceinline v3s absolute(v3s a) { return {absolute(a.x), absolute(a.y), absolute(a.z)}; }
forceinline v4s absolute(v4s a) { return {absolute(a.x), absolute(a.y), absolute(a.z), absolute(a.w)}; }

forceinline u32 absolute(u32 v) { return v; }

forceinline f64 absolute(f64 v) { return *(u64*)&v &= 0x7FFFFFFFFFFFFFFF, v; }

forceinline f32 set_sign(f32 dst, f32 src) {
    *(u32*)&dst &= 0x7FFFFFFF;
    *(u32*)&dst |= *(u32*)&src & 0x80000000;
    return dst;
}
forceinline f64 set_sign(f64 dst, f64 src) {
    *(u64*)&dst &= 0x7FFFFFFFFFFFFFFF;
    *(u64*)&dst |= *(u64*)&src & 0x8000000000000000;
    return dst;
}

forceinline s8 sign(s8  v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s16 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s32 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s64 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }

forceinline f32 sign(f32 v) { return set_sign(1.0f, v); }
forceinline v2f sign(v2f v) { return {sign(v.x), sign(v.y)}; }
forceinline v3f sign(v3f v) { return {sign(v.x), sign(v.y), sign(v.z)}; }
forceinline v4f sign(v4f v) { return {sign(v.x), sign(v.y), sign(v.z), sign(v.w)}; }

forceinline f32 trunc(f32 v) { return _mm_cvtss_f32(_mm_round_ps(_mm_set_ss(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)); };
forceinline v2f trunc(v2f v) { return {trunc(v.x), trunc(v.y)}; }
forceinline v3f trunc(v3f v) { return {trunc(v.x), trunc(v.y), trunc(v.z)}; }
forceinline v4f trunc(v4f v) { return {trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w)}; }

forceinline f64 trunc(f64 v) { return _mm_cvtsd_f64(_mm_round_pd(_mm_set_sd(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)); };

#define MOD(f32) \
forceinline f32 modulo(f32 a, f32 b) { return set_sign(a - trunc(a / b) * b, a); } \
forceinline f32 positive_modulo(f32 a, f32 b) { return frac(a / b) * b; }
MOD(f32)
MOD(f64)
#undef MOD

forceinline v2f modulo(v2f a, v2f b) { return {modulo(a.x, b.x), modulo(a.y, b.y)}; }
forceinline v3f modulo(v3f a, v3f b) { return {modulo(a.x, b.x), modulo(a.y, b.y), modulo(a.z, b.z)}; }
forceinline v4f modulo(v4f a, v4f b) { return {modulo(a.x, b.x), modulo(a.y, b.y), modulo(a.z, b.z), modulo(a.w, b.w)}; }

forceinline v2f positive_modulo(v2f a, v2f b) { return {positive_modulo(a.x, b.x), positive_modulo(a.y, b.y)}; }
forceinline v3f positive_modulo(v3f a, v3f b) { return {positive_modulo(a.x, b.x), positive_modulo(a.y, b.y), positive_modulo(a.z, b.z)}; }
forceinline v4f positive_modulo(v4f a, v4f b) { return {positive_modulo(a.x, b.x), positive_modulo(a.y, b.y), positive_modulo(a.z, b.z), positive_modulo(a.w, b.w)}; }

forceinline f32 sqrt(f32 v) { return sqrtf(v); }
forceinline v2f sqrt(v2f v) { return {sqrtf(v.x), sqrtf(v.y)}; }
forceinline v3f sqrt(v3f v) { return {sqrtf(v.x), sqrtf(v.y), sqrtf(v.z)}; }
forceinline v4f sqrt(v4f v) { return {sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w)}; }

forceinline f32 reciprocal(f32 v) { return _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss(v))); }
forceinline v2f reciprocal(v2f v) { return {reciprocal(v.x), reciprocal(v.y)}; }
forceinline v3f reciprocal(v3f v) { return {reciprocal(v.x), reciprocal(v.y), reciprocal(v.z)}; }
forceinline v4f reciprocal(v4f v) { return {reciprocal(v.x), reciprocal(v.y), reciprocal(v.z), reciprocal(v.w)}; }

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

forceinline f32 sin_bhaskara(f32 v) {
	v = positive_modulo(v, pi * 2);
	auto mask = v >= pi;
	v = select(mask, v - pi, v);
	return (16 * v * (pi - v)) * reciprocal(5 * pi * pi - 4 * v * (pi - v)) * select(mask, -1.0f, 1.0f);
}
forceinline f32 cos_bhaskara(f32 v) { return sin_bhaskara(v + pi*0.5f); }

forceinline v2f sincos_bhaskara(f32 v) { return {cos_bhaskara(v), sin_bhaskara(v)}; }

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

template <class A, class B, class T, class S>
forceinline constexpr auto lerpWrap(A a, B b, T t, S s) {
	a = positiveModulo(a, s);
	b = positiveModulo(b, s);
	auto d = a - b;
	return select(absolute(d) > half(s),
				  positiveModulo(lerp(a, b+sign(d)*s, t), s),
				  lerp(a, b, t));
}

forceinline constexpr f32 dot(f32 a, f32 b) { return a * b; }

#define DOT(f32, v2f) forceinline constexpr f32 dot(v2f a, v2f b) { return a *= b, a.x + a.y; }
DOT(f32, v2f)
DOT(s32, v2s)
DOT(u32, v2u)
DOT(s64, v3s64)
#undef DOT

#define DOT(f32, v3f) forceinline constexpr f32 dot(v3f a, v3f b) { return a *= b, a.x + a.y + a.z; }
DOT(f32, v3f)
DOT(s32, v3s)
DOT(u32, v3u)
#undef DOT

#define DOT(f32, v4f) forceinline constexpr f32 dot(v4f a, v4f b) { return a *= b, a.x + a.y + a.z + a.w; }
forceinline f32 dot(v4f a, v4f b) { return f32x4_dot(f32x4_load(&a), f32x4_load(&b)); }
DOT(s32, v4s)
DOT(u32, v4u)
#undef DOT

template <class T>
forceinline auto reflect(T v, T n) {
	return v - dot(v, n) * n * 2;
}

forceinline v2f cross(v2f a) { return {a.y, -a.x}; }
forceinline v2s cross(v2s a) { return {a.y, -a.x}; }
forceinline v3f cross(v3f a, v3f b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

forceinline f32 sum(v2f v) { return v.x + v.y; }

forceinline f32 rsqrt(f32 v) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(v))); }

template <class T>
forceinline auto length_squared(T a) {
	return dot(a, a);
}
template <class T>
forceinline auto length(T a) {
	return sqrt(dot(a, a));
}
template <class T>
forceinline auto normalize(T a) {
	return a / sqrt(dot(a, a));
}
template <class T>
forceinline auto normalize(T a, T fallback) {
	auto lsq = length_squared(a);
	if (lsq == decltype(lsq){})
		return fallback;
	return a * (1.0f / sqrt(lsq));
}
template <class T>
forceinline auto distance_squared(T a, T b) {
	return length_squared(a - b);
}
template <class T>
forceinline auto distance(T a, T b) {
	return sqrt(distance_squared(a, b));
}
template <> forceinline auto distance(f32 a, f32 b) { return absolute(a - b); }
template <> forceinline auto distance(f64 a, f64 b) { return absolute(a - b); }
template <class T>
forceinline auto manhattan(T a, T b) {
	return sum(abs(a - b));
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
forceinline constexpr auto moveAway(T a, T b, f32 t) {
	return a + normalize(a - b) * t;
}

template <class T>
forceinline constexpr T project(T vector, T normal) {
	return normal * dot(normal, vector);
}

forceinline f32 cos01(f32 t) { return 0.5f - cosf(t * pi) * 0.5f; }

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

inline v3f rgb_to_hsv(f32 r, f32 g, f32 b) {
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

template<class T>
struct ray {
	T origin, direction;
	explicit operator line<T>() {
		return {origin, origin + direction};
	}
};

template <class T>
forceinline ray<T> ray_origin_end(T origin, T end) {
	return {origin, end - origin};
}

template <class T>
forceinline ray<T> ray_origin_direction(T origin, T direction) {
	return {origin, direction};
}

template <class T>
forceinline line<T> as_line(ray<T> r) {
	return {r.origin, r.origin + r.direction};
}

template <class T>
forceinline ray<T> normalize(ray<T> r) {
	return {r.origin, normalize(r.direction)};
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
struct triangle {
	T a, b, c;
};

template<class T>
struct aabb {
	T min, max;
	T size() const { return max - min; }
	T center() const {
		return half(max + min);
	}
	template <class U>
	aabb<T> operator*(U const &b) { return {min * b, max * b}; }
	auto operator==(aabb const &that) const { return min == that.min && max == that.max; }
	auto operator!=(aabb const &that) const { return min != that.min || max != that.max; }
	template <class U>
	explicit operator aabb<U>() const { return { (U)min, (U)max }; }

	aabb operator-(T b) { return {min - b, max - b}; }
	aabb operator+(T b) { return {min + b, max + b}; }
	aabb &operator-=(T b) { return min -= b, max -= b, *this; }
	aabb &operator+=(T b) { return min += b, max += b, *this; }
};

template <class T>
forceinline aabb<T> aabb_min_max(T min, T max) {
	return {min, max};
}
template <class T>
forceinline aabb<T> aabb_min_size(T min, T size) {
	return {min, min + size};
}
template <class T>
forceinline aabb<T> aabbCenterDim(T center, T dim) {
	dim = half(dim);
	return {center - dim, center + dim};
}
template <class T>
forceinline aabb<T> aabbCenterRadius(T center, T radius) {
	return {center - radius, center + radius};
}

template <class T>
forceinline aabb<T> include(aabb<T> box, T point) {
	box.min = min(box.min, point);
	box.max = max(box.max, point);
	return box;
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
forceinline aabb<T> mask(aabb<T> box, aabb<T> bounds) {
	box.min = clamp(box.min, bounds.min, bounds.max);
	box.max = clamp(box.max, bounds.min, bounds.max);
	return box;
}

template <class T>
forceinline aabb<T> clamp(aabb<T> box, aabb<T> bounds) {
	auto diff = max(bounds.min - box.min, T{});
	box.min += diff;
	box.max += diff;

	diff = min(bounds.max - box.max, T{});
	box.min += diff;
	box.max += diff;

	return box;
}

#define for_aabb3(x, y, z, condition, box)           \
for (auto z = box.min.z; z condition box.max.z; ++z) \
for (auto y = box.min.y; y condition box.max.y; ++y) \
for (auto x = box.min.x; x condition box.max.x; ++x)

#define IN_BOUNDS2(bool, v2fA, v2fB) 			       \
	forceinline bool in_bounds(v2fA v, aabb<v2fB> b) { \
		return 								           \
			(v.x >= b.min.x) && 			           \
			(v.y >= b.min.y) && 			           \
			(v.x < b.max.x) && 				           \
			(v.y < b.max.y); 				           \
	}
IN_BOUNDS2(bool, v2f, v2f)
IN_BOUNDS2(bool, v2s, v2s)
IN_BOUNDS2(bool, v2u, v2u)
#undef IN_BOUNDS2

#define IN_BOUNDS3(bool, v3fA, v3fB) 			       \
	forceinline bool in_bounds(v3fA v, aabb<v3fB> b) { \
		return 								           \
			(v.x >= b.min.x) && 			           \
			(v.y >= b.min.y) &&				           \
			(v.z >= b.min.z) &&				           \
			(v.x < b.max.x) &&				           \
			(v.y < b.max.y) &&				           \
			(v.z < b.max.z);				           \
	}
IN_BOUNDS3(bool, v3f, v3f)
IN_BOUNDS3(bool, v3s, v3s)
IN_BOUNDS3(bool, v3u, v3u)
#undef IN_BOUNDS3

#define INTERSECTS1(bool, scl)						        \
	forceinline bool intersects(aabb<scl> a, aabb<scl> b) { \
		return 										        \
			(a.min < b.max) && 					            \
			(a.max > b.min); 					            \
	}
INTERSECTS1(bool, s64)
#undef INTERSECTS1

#define INTERSECTS2(bool, v2f)						        \
	forceinline bool intersects(aabb<v2f> a, aabb<v2f> b) { \
		return 										        \
			(a.min.x < b.max.x) && 					        \
			(a.min.y < b.max.y) && 					        \
			(a.max.x > b.min.x) && 					        \
			(a.max.y > b.min.y); 					        \
	}
INTERSECTS2(bool, v2f)
INTERSECTS2(bool, v2s)
INTERSECTS2(bool, v2u)
#undef INTERSECTS2

#define INTERSECTS3(bool, v3f)						        \
	forceinline bool intersects(aabb<v3f> a, aabb<v3f> b) { \
		return 										        \
			(a.min.x < b.max.x) && 					        \
			(a.min.y < b.max.y) && 					        \
			(a.min.z < b.max.z) && 					        \
			(a.max.x > b.min.x) && 					        \
			(a.max.y > b.min.y) && 					        \
			(a.max.z > b.min.z); 					        \
	}
INTERSECTS3(bool, v3f)
INTERSECTS3(bool, v3s)
INTERSECTS3(bool, v3u)
#undef INTERSECTS3

template <class T>
forceinline aabb<T> Union(aabb<T> const &a, aabb<T> const &b) {
	aabb<T> result;
	result.min = max(a.min, b.min);
	result.max = min(a.max, b.max);
	return result;
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

// Axis aligned bounding box subrtacion routines
// a - b
// subtract b from a
//
// subtract_volume treats a and b as volumes
//
// subtract_points treats a and b as sets of points;
// Every aabb in result contains unique set of points.
//
// In both cases result's max point is inclusive
//

inline StaticList<aabb<v2s>, 8> subtract_volumes(aabb<v2s> a, aabb<v2s> b) {
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
	for (u32 i = 0; i < boxes.size; ++i) {
		if (volume(boxes[i]) <= 0) {
			boxes.erase_at_unordered(i--);
		}
	}
	return boxes;
}
inline StaticList<aabb<v3s>, 26> subtract_volumes(aabb<v3s> a, aabb<v3s> b) {
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
	for (u32 i = 0; i < boxes.size; ++i) {
		if (volume(boxes[i]) <= 0) {
			boxes.erase_at_unordered(i--);
		}
	}
	return boxes;
}
inline StaticList<aabb<v2s>, 8> subtract_points(aabb<v2s> a, aabb<v2s> b) {
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
inline StaticList<aabb<v3s>, 26> subtract_points(aabb<v3s> a, aabb<v3s> b) {
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

inline StaticList<aabb<v2s>, 9> combine_volumes(aabb<v2s> const &a, aabb<v2s> const &b) {
	StaticList<aabb<v2s>, 9> result = { a };
	result += subtract_volumes(b, a);
	return result;
}
inline StaticList<aabb<v3s>, 27> combine_volumes(aabb<v3s> const &a, aabb<v3s> const &b) {
	StaticList<aabb<v3s>, 27> result = { a };
	result += subtract_volumes(b, a);
	return result;
}
inline StaticList<aabb<v2s>, 9> combine_points(aabb<v2s> const &a, aabb<v2s> const &b) {
	StaticList<aabb<v2s>, 9> result = { a };
	result += subtract_points(b, a);
	return result;
}
inline StaticList<aabb<v3s>, 27> combine_points(aabb<v3s> const &a, aabb<v3s> const &b) {
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
struct RaycastHit {
	bool hit = false;
	v3f position = {};
	v3f normal = {};
	operator bool() {
		return hit;
	}
};

inline RaycastHit raycast(ray<v3f> ray, triangle<v3f> tri) {
    v3f e1 = tri.b - tri.a;
    v3f e2 = tri.c - tri.a;
    // Вычисление вектора нормали к плоскости
    v3f pvec = cross(ray.direction, e2);
    f32 det = dot(e1, pvec);

    // Луч параллелен плоскости
    if (det < 1e-8 && det > -1e-8) {
		return {};
    }

    f32 inv_det = 1 / det;
    v3f tvec = ray.origin - tri.a;
    f32 u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) {
		return {};
    }

    v3f qvec = cross(tvec, e1);
    f32 v = dot(ray.direction, qvec) * inv_det;
    if (v < 0 || u + v > 1) {
		return {};
    }
    f32 t = dot(e2, qvec) * inv_det;

	if (t < 0)
		return {};

	RaycastHit hit;
	hit.hit = true;
	hit.position = ray.origin + ray.direction* t;
	hit.normal = normalize(cross(e1, e2));
	return hit;
}

inline RaycastHit raycast(ray<v3f> ray, aabb<v3f> box) {
	v3f dirfrac = 1.0f / ray.direction;
	v3f t1 = (box.min - ray.origin)*dirfrac;
	v3f t2 = (box.max - ray.origin)*dirfrac;

	f32 tmin = max(min(t1.x, t2.x), min(t1.y, t2.y), min(t1.z, t2.z));
	f32 tmax = min(max(t1.x, t2.x), max(t1.y, t2.y), max(t1.z, t2.z));

	if (tmin < 0 || tmax < 0 || tmin > tmax) {
		return {};
	}

	RaycastHit hit = {};
	hit.hit = true;
	hit.position = ray.origin + ray.direction * tmin;
	assert(dot(ray.direction, hit.position - ray.origin) > 0);
	return hit;
}

template <class T, umm size>
forceinline constexpr T linearSample(const T (&arr)[size], f32 t) noexcept {
	f32 f = frac(t) * size;
	s32 a = (s32)f;
	s32 b = a + 1;
	if (b == size)
		b = 0;
	return lerp(arr[a], arr[b], frac(f));
}

union m4 {
	using Scalar = f32;
	using Vector = v4f;
	struct {
		v4f i, j, k, l;
	};
	struct {
		f32x4 im, jm, km, lm;
	};
	f32x4 m[4];
	v4f vectors[4];
	f32 s[16];
	forceinline v4f operator*(v4f const &b) const {
		v4f x = V4f(b.x) * i;
		v4f y = V4f(b.y) * j;
		v4f z = V4f(b.z) * k;
		v4f w = V4f(b.w) * l;
		return x + y + z + w;
	}
	forceinline v3f operator*(v3f const &b) const {
		auto x = V4f(b.x) * i;
		auto y = V4f(b.y) * j;
		auto z = V4f(b.z) * k;
		auto r = x + y + z;
		v3f result;
		memcpy(&result, &r, 12);
		return result;
	}
	forceinline m4 operator*(m4 const &b) const { return {*this * b.i, *this * b.j, *this * b.k, *this * b.l}; }
	forceinline m4& operator*=(m4 const &b) { return *this = *this * b; }
	static forceinline m4 scale(f32 x, f32 y, f32 z) {
		return {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		};
	}
	static forceinline m4 identity() {
		m4 m;
		m.s[0] = 1;
		m.s[1] = 0;
		m.s[2] = 0;
		m.s[3] = 0;

		m.s[4] = 0;
		m.s[5] = 1;
		m.s[6] = 0;
		m.s[7] = 0;

		m.s[8] = 0;
		m.s[9] = 0;
		m.s[10] = 1;
		m.s[11] = 0;

		m.s[12] = 0;
		m.s[13] = 0;
		m.s[14] = 0;
		m.s[15] = 1;
		return m;
	}
	static forceinline m4 scale(v3f v) { return scale(v.x, v.y, v.z); }
	static forceinline m4 scale(v2f xy, f32 z) { return scale(xy.x, xy.y, z); }
	static forceinline m4 scale(f32 v) { return scale(v, v, v); }
	static forceinline m4 translation(f32 x, f32 y, f32 z) {
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1,
		};
	}
	static forceinline m4 translation(v3f v) { return translation(v.x, v.y, v.z); }
	static forceinline m4 translation(v2f xy, f32 z) { return translation(xy.x, xy.y, z); }
	static forceinline m4 translation(f32 v) { return translation(v, v, v); }
	static forceinline m4 perspective_left_handed(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 h	   = 1.0f / tanf(fov * 0.5f);
		f32 w	   = h / aspect;
		f32 fzdfmn = fz / (fz - nz);
		return {
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, fzdfmn, 1,
			0, 0, -fzdfmn * nz, 0
		};
	}
	static forceinline m4 perspective_right_handed(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 xymax = nz * tan(fov * 0.5f);
		f32 depth = fz - nz;
		f32 q = -(fz + nz) / depth;
		f32 qn = -2 * (fz * nz) / depth;
		f32 h = nz / xymax;
		f32 w = h / aspect;

		return {
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, q, -1,
			0, 0, qn, 0,
		};
	}
	static forceinline m4 ortho_right_handed(f32 height, f32 aspect, f32 nz, f32 fz) {
		f32 h = 1.0f / (height * 0.5f);
		f32 rz = fz - nz;
		return {
			h / aspect, 0, 0, 0,
			0, h, 0, 0,
			0, 0, 1.0f / rz, 0,
			0, 0, -nz / rz, 1,
		};
	}
	static forceinline m4 rotation_r_x(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			1, 0, 0, 0,
			0, c, s, 0,
			0,-s, c, 0,
			0, 0, 0, 1
		};
	}
	static forceinline m4 rotation_r_y(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			c, 0,-s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1
		};
	}
	static forceinline m4 rotation_r_z(f32 a) {
		f32 s, c;
		cos_sin(a, c, s);
		return {
			 c, s, 0, 0,
			-s, c, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		};
	}
	static forceinline m4 rotation_r_zxy(v3f a) {
		v3f s;
		v3f c;
		cos_sin(a, c, s);
		return {
			 c.z*c.y + s.z*s.x*s.y, s.z*c.x,  c.z*-s.y + s.z*s.x*c.y, 0,
			-s.z*c.y + c.z*s.x*s.y, c.z*c.x, -s.z*-s.y + c.z*s.x*c.y, 0,
			               c.x*s.y,    -s.x,                 c.x*c.y, 0,
			                     0,       0,                       0, 1,
		};
	}
	static forceinline m4 rotation_r_zxy(f32 x, f32 y, f32 z) { return rotation_r_zxy({x, y, z}); }
	static forceinline m4 rotation_r_yxz(v3f a) {
		v3f s;
		v3f c;
		cos_sin(a, c, s);
		return {
			c.y*c.z + -s.y*-s.x*-s.z, c.y*s.z + -s.y*-s.x*c.z, -s.y*c.x, 0,
			                c.x*-s.z,                 c.x*c.z,      s.x, 0,
			 s.y*c.z + c.y*-s.x*-s.z,  s.y*s.z + c.y*-s.x*c.z,  c.y*c.x, 0,
			                       0,                       0,        0, 1,
		};
	}
	static forceinline m4 rotation_r_yxz(f32 x, f32 y, f32 z) { return rotation_r_yxz({x, y, z}); }
};


forceinline m4 to_m4(m3 m) {
	return {
		m.i.x, m.i.y, m.i.z, 0,
		m.j.x, m.j.y, m.j.z, 0,
		m.k.x, m.k.y, m.k.z, 0,
			0,     0,     0, 1,
	};
}

forceinline constexpr m3 transpose(m3 const& m) {
	return {
		m.i.x, m.j.x, m.k.x,
		m.i.y, m.j.y, m.k.y,
		m.i.z, m.j.z, m.k.z,
	};
}
forceinline m4 transpose(m4 const& m) {
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

inline m4 inverse(m4 const &m) {
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
		return {};

	det = 1 / det;

	return m4 {
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
	};
}

forceinline constexpr m4 M4(f32 v = 0.0f) { return m4{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; }
forceinline constexpr m4 M4(v4f i, v4f j, v4f k, v4f l) { return m4{i, j, k, l}; }
forceinline constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l) {
	m4 r{};
	r.im = i;
	r.jm = j;
	r.km = k;
	r.lm = l;
	return r;
}
forceinline constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw) {
	return {ix, iy, iz, iw, jx, jy, jz, jw, kx, ky, kz, kw, lx, ly, lz, lw};
}

forceinline m4 M4(m3 v) {
	return {
		V4f(v.i, 0),
		V4f(v.j, 0),
		V4f(v.k, 0),
		V4f(0, 0, 0, 1),
	};
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
	for (auto p : planes) {
		if (dot(v3f{p.x, p.y, p.z}, position) + p.w + radius < 0) {
			return false;
		}
	}
	return true;
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
		float s = msign(q + h) * pow(absolute(q + h), 1.0f / 3.0f);
		float t = msign(q - h) * pow(absolute(q - h), 1.0f / 3.0f);
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

namespace CE {

forceinline constexpr v4s frac(v4s v, s32 step) {
	return {
		tl::frac(v.x, step),
		tl::frac(v.y, step),
		tl::frac(v.z, step),
		tl::frac(v.w, step),
	};
}

} // namespace CE

#define TO_STRING_V2(v2f)                           \
inline void append(StringBuilder &builder, v2f v) { \
	append(builder, "{"s);                          \
	append(builder, v.x);                           \
	append(builder, ", "s);                         \
	append(builder, v.y);                           \
	append(builder, "}"s);                          \
}

TO_STRING_V2(v2f)
TO_STRING_V2(v2u)
TO_STRING_V2(v2s)

#undef TO_STRING_V2

#define TO_STRING_V3(v3f)                           \
inline void append(StringBuilder &builder, v3f v) { \
	append(builder, "{"s);                          \
	append(builder, v.x);                           \
	append(builder, ", "s);                         \
	append(builder, v.y);                           \
	append(builder, ", "s);                         \
	append(builder, v.z);                           \
	append(builder, "}"s);                          \
}

TO_STRING_V3(v3f)
TO_STRING_V3(v3u)
TO_STRING_V3(v3s)

#undef TO_STRING_V3

#define TO_STRING_V4(v4f)                           \
inline void append(StringBuilder &builder, v4f v) { \
	append(builder, "{"s);                          \
	append(builder, v.x);                           \
	append(builder, ", "s);                         \
	append(builder, v.y);                           \
	append(builder, ", "s);                         \
	append(builder, v.z);                           \
	append(builder, ", "s);                         \
	append(builder, v.w);                           \
	append(builder, "}"s);                          \
}

TO_STRING_V4(v4f)
TO_STRING_V4(v4u)
TO_STRING_V4(v4s)

#undef TO_STRING_V3

inline void append(StringBuilder &builder, m4 m) {
	append_format(builder, "{%, %, %, %}", m.i, m.j, m.k, m.l);
}

template <class T>
void append(StringBuilder &builder, aabb<T> v) {
	append(builder, "{ min: "s);
	append(builder, v.min);
	append(builder, ", max: "s);
	append(builder, v.max);
	append(builder, " }"s);
}

template <class T>
void append(StringBuilder &builder, ray<T> r) {
	append_format(builder, "ray{origin=%, direction=%}", r.origin, r.direction);
}

template <class To, class From> forceinline To cvt(From v) { return (To)v; }
template <> forceinline v2f cvt(f32 v) { return V2f(v); }
template <> forceinline v3f cvt(f32 v) { return V3f(v); }
template <> forceinline v2f cvt(s32 v) { return V2f((f32)v); }
template <> forceinline v3f cvt(s32 v) { return V3f((f32)v); }
template <> forceinline v2f cvt(u32 v) { return V2f((f32)v); }
template <> forceinline v3f cvt(u32 v) { return V3f((f32)v); }

template <class T>
forceinline T saturate(T t) {
	return clamp(t, cvt<T>(0), cvt<T>(1));
}

template <class T>
forceinline auto smoothstep(T x) {
	return x*x*x*(x*(6*x - 15) + 10);
}

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
