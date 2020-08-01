#pragma once
#include "common.h"
#include "array.h"

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif
#include <float.h>
#include <math.h>
#include <memory.h>
#include <utility>

#if COMPILER_MSVC
#pragma warning(disable : 5045) // spectre
#endif

namespace TL {

#ifndef TL_UNDEF_UNSUPPORTED_INTRINSICS
#define TL_UNDEF_UNSUPPORTED_INTRINSICS 1
#endif

#if TL_UNDEF_UNSUPPORTED_INTRINSICS
#include "undef_unsupported_intrinsics.inl"
#endif

#define _mm_castpi_ps(x) _mm_castsi128_ps(x)
#define _mm_castps_pi(x) _mm_castps_si128(x)
#define _mm_castpi_pd(x) _mm_castsi128_pd(x)
#define _mm_castpd_pi(x) _mm_castpd_si128(x)
#define _mm_setzero_pi() _mm_setzero_si128()
#define _mm_xor_pi(a, b) _mm_castps_pi(_mm_xor_ps(_mm_castpi_ps(a), _mm_castpi_ps(b)))
#define _mm_blendv_epi32(a, b, m) _mm_castps_pi(_mm_blendv_ps(_mm_castpi_ps(a), _mm_castpi_ps(b), _mm_castpi_ps(m)))
#define _mm_slli_pi(a, b) _mm_slli_si128(a, b)
#define _mm_setone_ps() _mm_castpi_ps(_mm_set1_epi8(0xFF))
#define _mm_not_ps(x) _mm_xor_ps(x, _mm_setone_ps())
#define _mm_not_epi32_ps(x) _mm_not_ps(_mm_castpi_ps(x))
#define _mm_not_epi32(x) _mm_castps_pi(_mm_not_epi32_ps(x))
#define _mm_cmplt_epi32(a, b) _mm_cmpgt_epi32(b, a)
#define _mm_cmple_epi32(a, b) _mm_not_epi32(_mm_cmpgt_epi32(a, b))
#define _mm_cmpge_epi32(a, b) _mm_not_epi32(_mm_cmpgt_epi32(b, a))
#define _mm_cmpne_epi32(a, b) _mm_not_epi32(_mm_cmpeq_epi32(b, a))
#define _mm_cmpgt_epu32(a, b) _mm_cmpgt_epi32(_mm_xor_pi(a, _mm_set1_epi32(0x80000000)), _mm_xor_pi(b, _mm_set1_epi32(0x80000000)))
#define _mm_sllv_epi32(a, b)  _mm_setr_epi32(_mm_extract_epi32(a, 0) << _mm_extract_epi32(b, 0), _mm_extract_epi32(a, 1) << _mm_extract_epi32(b, 1), _mm_extract_epi32(a, 2) << _mm_extract_epi32(b, 2), _mm_extract_epi32(a, 3) << _mm_extract_epi32(b, 3))
#define _mm_srlv_epi32(a, b)  _mm_setr_epi32(_mm_extract_epi32(a, 0) >> _mm_extract_epi32(b, 0), _mm_extract_epi32(a, 1) >> _mm_extract_epi32(b, 1), _mm_extract_epi32(a, 2) >> _mm_extract_epi32(b, 2), _mm_extract_epi32(a, 3) >> _mm_extract_epi32(b, 3))
#define _mm_srlv_epi32(a, b)  _mm_setr_epi32((u32)_mm_extract_epi32(a, 0) >> (u32)_mm_extract_epi32(b, 0), (u32)_mm_extract_epi32(a, 1) >> (u32)_mm_extract_epi32(b, 1), (u32)_mm_extract_epi32(a, 2) >> (u32)_mm_extract_epi32(b, 2), (u32)_mm_extract_epi32(a, 3) >> (u32)_mm_extract_epi32(b, 3))
#define _mm_slliv_epi32(a, b) _mm_setr_epi32(a << _mm_extract_epi32(b, 0), a << _mm_extract_epi32(b, 1), a << _mm_extract_epi32(b, 2), a << _mm_extract_epi32(b, 3))
#define _mm_srliv_epi32(a, b) _mm_setr_epi32(a >> _mm_extract_epi32(b, 0), a >> _mm_extract_epi32(b, 1), a >> _mm_extract_epi32(b, 2), a >> _mm_extract_epi32(b, 3))
#define _mm_srliv_epi32(a, b) _mm_setr_epi32((u32)a >> (u32)_mm_extract_epi32(b, 0), (u32)a >> (u32)_mm_extract_epi32(b, 1), (u32)a >> (u32)_mm_extract_epi32(b, 2), (u32)a >> (u32)_mm_extract_epi32(b, 3))

#define _mm256_castpi_ps(x) _mm256_castsi256_ps(x)
#define _mm256_castps_pi(x) _mm256_castps_si256(x)
#define _mm256_castpi_pd(x) _mm256_castsi256_pd(x)
#define _mm256_castpd_pi(x) _mm256_castpd_si256(x)
#define _mm256_setzero_pi() _mm256_setzero_si256()
#define _mm256_xor_pi(a, b) _mm256_castps_pi(_mm256_xor_ps(_mm256_castpi_ps(a), _mm256_castpi_ps(b)))
#define _mm256_setone_ps() _mm256_castpi_ps(_mm256_set1_epi8(0xFF))
#define _mm256_not_ps(x) _mm256_xor_ps(x, _mm256_setone_ps())
#define _mm256_not_epi32_ps(x) _mm256_not_ps(_mm256_castpi_ps(x))
#define _mm256_not_epi32(x) _mm256_castps_pi(_mm256_not_epi32_ps(x))
#define _mm256_cmplt_epi32(a, b) _mm256_cmpgt_epi32(b, a)
#define _mm256_cmple_epi32(a, b) _mm256_not_epi32(_mm256_cmpgt_epi32(a, b))
#define _mm256_cmpge_epi32(a, b) _mm256_not_epi32(_mm256_cmpgt_epi32(b, a))
#define _mm256_cmpne_epi32(a, b) _mm256_not_epi32(_mm256_cmpeq_epi32(b, a))
#define _mm256_cmpgt_epu32(a, b) _mm256_cmpgt_epi32(_mm256_xor_pi(a, _mm256_set1_epi32(0x80000000)), _mm256_xor_pi(b, _mm256_set1_epi32(0x80000000)))
#define _mm256_sllv_epi32(a, b)  _mm256_setr_epi32(_mm256_extract_epi32(a, 0) << _mm256_extract_epi32(b, 0), _mm256_extract_epi32(a, 1) << _mm256_extract_epi32(b, 1), _mm256_extract_epi32(a, 2) << _mm256_extract_epi32(b, 2), _mm256_extract_epi32(a, 3) << _mm256_extract_epi32(b, 3), _mm256_extract_epi32(a, 4) << _mm256_extract_epi32(b, 4), _mm256_extract_epi32(a, 5) << _mm256_extract_epi32(b, 5), _mm256_extract_epi32(a, 6) << _mm256_extract_epi32(b, 6), _mm256_extract_epi32(a, 7) << _mm256_extract_epi32(b, 7))
#define _mm256_srav_epi32(a, b)  _mm256_setr_epi32(_mm256_extract_epi32(a, 0) >> _mm256_extract_epi32(b, 0), _mm256_extract_epi32(a, 1) >> _mm256_extract_epi32(b, 1), _mm256_extract_epi32(a, 2) >> _mm256_extract_epi32(b, 2), _mm256_extract_epi32(a, 3) >> _mm256_extract_epi32(b, 3), _mm256_extract_epi32(a, 4) >> _mm256_extract_epi32(b, 4), _mm256_extract_epi32(a, 5) >> _mm256_extract_epi32(b, 5), _mm256_extract_epi32(a, 6) >> _mm256_extract_epi32(b, 6), _mm256_extract_epi32(a, 7) >> _mm256_extract_epi32(b, 7))
#define _mm256_srlv_epi32(a, b)  _mm256_setr_epi32((u32)_mm256_extract_epi32(a, 0) >> (u32)_mm256_extract_epi32(b, 0), (u32)_mm256_extract_epi32(a, 1) >> (u32)_mm256_extract_epi32(b, 1), (u32)_mm256_extract_epi32(a, 2) >> (u32)_mm256_extract_epi32(b, 2), (u32)_mm256_extract_epi32(a, 3) >> (u32)_mm256_extract_epi32(b, 3), (u32)_mm256_extract_epi32(a, 4) >> (u32)_mm256_extract_epi32(b, 4), (u32)_mm256_extract_epi32(a, 5) >> (u32)_mm256_extract_epi32(b, 5), (u32)_mm256_extract_epi32(a, 6) >> (u32)_mm256_extract_epi32(b, 6), (u32)_mm256_extract_epi32(a, 7) >> (u32)_mm256_extract_epi32(b, 7))
#define _mm256_slliv_epi32(a, b) _mm256_setr_epi32(a << _mm256_extract_epi32(b, 0), a << _mm256_extract_epi32(b, 1), a << _mm256_extract_epi32(b, 2), a << _mm256_extract_epi32(b, 3), a << _mm256_extract_epi32(b, 4), a << _mm256_extract_epi32(b, 5), a << _mm256_extract_epi32(b, 6), a << _mm256_extract_epi32(b, 7))
#define _mm256_sraiv_epi32(a, b) _mm256_setr_epi32(a >> _mm256_extract_epi32(b, 0), a >> _mm256_extract_epi32(b, 1), a >> _mm256_extract_epi32(b, 2), a >> _mm256_extract_epi32(b, 3), a >> _mm256_extract_epi32(b, 4), a >> _mm256_extract_epi32(b, 5), a >> _mm256_extract_epi32(b, 6), a >> _mm256_extract_epi32(b, 7))
#define _mm256_srliv_epi32(a, b) _mm256_setr_epi32((u32)a >> (u32)_mm256_extract_epi32(b, 0), (u32)a >> (u32)_mm256_extract_epi32(b, 1), (u32)a >> (u32)_mm256_extract_epi32(b, 2), (u32)a >> (u32)_mm256_extract_epi32(b, 3), (u32)a >> (u32)_mm256_extract_epi32(b, 4), (u32)a >> (u32)_mm256_extract_epi32(b, 5), (u32)a >> (u32)_mm256_extract_epi32(b, 6), (u32)a >> (u32)_mm256_extract_epi32(b, 7))

#define _mm_cvtipd_epi32(x) _mm_cvtpd_epi32(_mm_round_pd(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC))
#define _mm256_cvtipd_epi32(x) _mm256_cvtpd_epi32(_mm256_round_pd(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC))


#define MM256_PERM128(a, b) ((a) | ((b) << 4))
#define MM_SHUFFLE(a,b,c,d) _MM_SHUFFLE(d,c,b,a)

union M16 {
	__m128  ps;
	__m128i pi;
	__m128d pd;
};

#if ARCH_AVX
union M32 {
	__m256  ps;
	__m256i pi;
	__m256d pd;
};
#else
union M32 {
	M16 lo, hi;
};
#endif

#if ARCH_AVX512F
union M64 {
	__m512  ps;
	__m512i pi;
	__m512d pd;
};
#else
union M64 {
	M32 lo, hi;
};
#endif

 // simd register width in bytes
#if ARCH_AVX512F
static constexpr u32 simdWidth = 64;
#elif ARCH_AVX
static constexpr u32 simdWidth = 32;
#else
static constexpr u32 simdWidth = 16;
#endif

// clang-format off

constexpr f32 pi    = f32(3.1415926535897932384626433832795L);
constexpr f32 invPi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2 = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3 = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5 = f32(2.2360679774997896964091736687313L);

template <class T> FORCEINLINE constexpr auto radians(T deg) { return deg * (T)(pi / 180.0L); }
template <class T> FORCEINLINE constexpr auto degrees(T rad) { return rad * (T)(180.0L / pi); }
template <class T, class SN, class SX, class DN, class DX> FORCEINLINE constexpr auto map(T v, SN sn, SX sx, DN dn, DX dx) { return (v - sn) / (sx - sn) * (dx - dn) + dn; }
template <class T, class U, class V> FORCEINLINE constexpr auto clamp(T a, U mi, V ma) { return min(max(a, mi), ma); }
template <class A, class B, class T> FORCEINLINE constexpr auto lerp(A a, B b, T t) { return a + (b - a) * t; }
template <class T> FORCEINLINE constexpr T select(bool mask, T a, T b) { return mask ? a : b; }
template <class M, class T> FORCEINLINE T &maskAssign(M mask, T &dst, T src) { return dst = select(mask, src, dst); }
template <class T> FORCEINLINE constexpr auto pow2(T v) { return v * v; }
template <class T> FORCEINLINE constexpr auto pow3(T v) { return v * v * v; }
template <class T> FORCEINLINE constexpr auto pow4(T v) { return pow2(v * v); }
// clang-format on

template <class A, class B, class T, class S> FORCEINLINE constexpr auto wrapLerp(A a, B b, T t, S s) {
	a = positiveModulo(a, s);
	b = positiveModulo(b, s);
	auto d = a - b;
	return select(absolute(d) > half(s), 
				  positiveModulo(lerp(a, b+sign(d)*s, t), s), 
				  lerp(a, b, t));
}

namespace CE {

template <class To, class From>
FORCEINLINE constexpr To cvt(From v) {
	return (To)v;
}

} // namespace CE

// clang-format off
template<umm> union b32x; using b32x1 = b32x<1>; using b32x4 = b32x<4>; using b32x8 = b32x<8>;
template<umm> union f32x; using f32x1 = f32x<1>; using f32x4 = f32x<4>; using f32x8 = f32x<8>;
template<umm> union s32x; using s32x1 = s32x<1>; using s32x4 = s32x<4>; using s32x8 = s32x<8>;
template<umm> union u32x; using u32x1 = u32x<1>; using u32x4 = u32x<4>; using u32x8 = u32x<8>;
template<umm> union b64x; using b64x2 = b64x<2>; using b64x4 = b64x<4>;
template<umm> union f64x; using f64x2 = f64x<2>; using f64x4 = f64x<4>;

#if 0
template<class, umm> union _v2;
template<class, umm> union _v2;
template<class, umm> union _v2;
template<class, umm> union _v3;
template<class, umm> union _v3;
template<class, umm> union _v3;
template<class, umm> union _v4;
template<class, umm> union _v4;
template<class, umm> union _v4;

template<umm pack> using v2fx = _v2<f32, pack>; using v2fx2 = v2fx<2>; using v2fx4 = v2fx<4>; using v2fx8 = v2fx<8>;
template<umm pack> using v2sx = _v2<s32, pack>; using v2sx2 = v2sx<2>; using v2sx4 = v2sx<4>; using v2sx8 = v2sx<8>;
template<umm pack> using v2ux = _v2<u32, pack>; using v2ux2 = v2ux<2>; using v2ux4 = v2ux<4>; using v2ux8 = v2ux<8>;
template<umm pack> using v3fx = _v3<f32, pack>; using v3fx2 = v3fx<2>; using v3fx4 = v3fx<4>; using v3fx8 = v3fx<8>;
template<umm pack> using v3sx = _v3<s32, pack>; using v3sx2 = v3sx<2>; using v3sx4 = v3sx<4>; using v3sx8 = v3sx<8>;
template<umm pack> using v3ux = _v3<u32, pack>; using v3ux2 = v3ux<2>; using v3ux4 = v3ux<4>; using v3ux8 = v3ux<8>;
template<umm pack> using v4fx = _v4<f32, pack>; using v4fx2 = v4fx<2>; using v4fx4 = v4fx<4>; using v4fx8 = v4fx<8>;
template<umm pack> using v4sx = _v4<s32, pack>; using v4sx2 = v4sx<2>; using v4sx4 = v4sx<4>; using v4sx8 = v4sx<8>;
template<umm pack> using v4ux = _v4<u32, pack>; using v4ux2 = v4ux<2>; using v4ux4 = v4ux<4>; using v4ux8 = v4ux<8>;

using v2f = v2fx<1>;
using v2s = v2sx<1>;
using v2u = v2ux<1>;
using v3f = v3fx<1>;
using v3s = v3sx<1>;
using v3u = v3ux<1>;
using v4f = v4fx<1>;
using v4s = v4sx<1>;
using v4u = v4ux<1>;
#else
template<umm pack> union v2fx; using v2fx2 = v2fx<2>; using v2fx4 = v2fx<4>; using v2fx8 = v2fx<8>;
template<umm pack> union v2sx; using v2sx2 = v2sx<2>; using v2sx4 = v2sx<4>; using v2sx8 = v2sx<8>;
template<umm pack> union v2ux; using v2ux2 = v2ux<2>; using v2ux4 = v2ux<4>; using v2ux8 = v2ux<8>;
template<umm pack> union v3fx; using v3fx2 = v3fx<2>; using v3fx4 = v3fx<4>; using v3fx8 = v3fx<8>;
template<umm pack> union v3sx; using v3sx2 = v3sx<2>; using v3sx4 = v3sx<4>; using v3sx8 = v3sx<8>;
template<umm pack> union v3ux; using v3ux2 = v3ux<2>; using v3ux4 = v3ux<4>; using v3ux8 = v3ux<8>;
template<umm pack> union v4fx; using v4fx2 = v4fx<2>; using v4fx4 = v4fx<4>; using v4fx8 = v4fx<8>;
template<umm pack> union v4sx; using v4sx2 = v4sx<2>; using v4sx4 = v4sx<4>; using v4sx8 = v4sx<8>;
template<umm pack> union v4ux; using v4ux2 = v4ux<2>; using v4ux4 = v4ux<4>; using v4ux8 = v4ux<8>;

using b32xm = b32x<simdWidth / sizeof(b32)>;
using f32xm = f32x<simdWidth / sizeof(f32)>;
using f64xm = f64x<simdWidth / sizeof(f64)>;
using s32xm = s32x<simdWidth / sizeof(s32)>;
using u32xm = u32x<simdWidth / sizeof(u32)>;

using v2fxm = v2fx<simdWidth / sizeof(f32)>;
using v2sxm = v2sx<simdWidth / sizeof(s32)>;
using v2uxm = v2ux<simdWidth / sizeof(u32)>;
using v3fxm = v3fx<simdWidth / sizeof(f32)>;
using v3sxm = v3sx<simdWidth / sizeof(s32)>;
using v3uxm = v3ux<simdWidth / sizeof(u32)>;
using v4fxm = v4fx<simdWidth / sizeof(f32)>;
using v4sxm = v4sx<simdWidth / sizeof(s32)>;
using v4uxm = v4ux<simdWidth / sizeof(u32)>;

using v2f = v2fx<1>;
using v2s = v2sx<1>;
using v2u = v2ux<1>;
using v3f = v3fx<1>;
using v3s = v3sx<1>;
using v3u = v3ux<1>;
using v4f = v4fx<1>;
using v4s = v4sx<1>;
using v4u = v4ux<1>;
#endif
// clang-format on

union m3;
union m4;

template <class T>
inline static constexpr umm _widthOf = 0;
template <class T>
inline static constexpr umm widthOf = _widthOf<RemoveCVRef<T>>;

// clang-format off
template<umm width> inline static constexpr umm _widthOf<f32x<width>> = width;
template<umm width> inline static constexpr umm _widthOf<s32x<width>> = width;
template<umm width> inline static constexpr umm _widthOf<u32x<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v2fx<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v2sx<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v2ux<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v3fx<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v3sx<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v3ux<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v4fx<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v4sx<width>> = width;
template<umm width> inline static constexpr umm _widthOf<v4ux<width>> = width;
// clang-format on

FORCEINLINE f32x8 F32x8(f32x4, f32x4);
FORCEINLINE s32x8 S32x8(s32x4, s32x4);
FORCEINLINE u32x8 U32x8(u32x4, u32x4);
FORCEINLINE f64x4 F64x4(f64x2, f64x2);

template<umm width> FORCEINLINE b32x<width> B32x(b32 = false);
template<umm width> FORCEINLINE f32x<width> F32x(f32 = 0);
template<umm width> FORCEINLINE s32x<width> S32x(s32 = 0);
template<umm width> FORCEINLINE u32x<width> U32x(u32 = 0);

FORCEINLINE constexpr f32x4 F32x4(M16);
FORCEINLINE constexpr s32x4 S32x4(M16);
FORCEINLINE constexpr u32x4 U32x4(M16);

FORCEINLINE constexpr f32x8 F32x8(M32);
FORCEINLINE constexpr s32x8 S32x8(M32);
FORCEINLINE constexpr u32x8 U32x8(M32);

FORCEINLINE constexpr f64x2 F64x2(M16);
//FORCEINLINE constexpr s64x2 S64x2(M16);
//FORCEINLINE constexpr u64x2 U64x2(M16);

FORCEINLINE constexpr f64x4 F64x4(M32);
//FORCEINLINE constexpr s64x4 S64x4(M32);
//FORCEINLINE constexpr u64x4 U64x4(M32);

FORCEINLINE f32x4 F32x4(f32 = 0);
FORCEINLINE s32x4 S32x4(s32 = 0);
FORCEINLINE u32x4 U32x4(u32 = 0);

FORCEINLINE f64x2 F64x2(f32 = 0);
FORCEINLINE f64x4 F64x4(f32 = 0);

FORCEINLINE f32x8 F32x8(f32 = 0);
FORCEINLINE s32x8 S32x8(s32 = 0);
FORCEINLINE u32x8 U32x8(u32 = 0);

FORCEINLINE f32x4 F32x4(f32, f32, f32, f32);
FORCEINLINE s32x4 S32x4(s32, s32, s32, s32);
FORCEINLINE u32x4 U32x4(u32, u32, u32, u32);

FORCEINLINE f32x8 F32x8(f32, f32, f32, f32, f32, f32, f32, f32);
FORCEINLINE s32x8 S32x8(s32, s32, s32, s32, s32, s32, s32, s32);
FORCEINLINE u32x8 U32x8(u32, u32, u32, u32, u32, u32, u32, u32);

FORCEINLINE constexpr v2f V2f(f32 = 0);
FORCEINLINE constexpr v2s V2s(s32 = 0);
FORCEINLINE constexpr v2u V2u(u32 = 0);

FORCEINLINE constexpr v3f V3f(f32 = 0);
FORCEINLINE constexpr v3s V3s(s32 = 0);
FORCEINLINE constexpr v3u V3u(u32 = 0);

FORCEINLINE constexpr v4f V4f(f32 = 0);
FORCEINLINE constexpr v4s V4s(s32 = 0);
FORCEINLINE constexpr v4u V4u(u32 = 0);

template<umm ps> FORCEINLINE v2fx<ps> V2fx(f32 = 0);
template<umm ps> FORCEINLINE v2sx<ps> V2sx(s32 = 0);
template<umm ps> FORCEINLINE v2ux<ps> V2ux(u32 = 0);
template<umm ps> FORCEINLINE v3fx<ps> V3fx(f32 = 0);
template<umm ps> FORCEINLINE v3sx<ps> V3sx(s32 = 0);
template<umm ps> FORCEINLINE v3ux<ps> V3ux(u32 = 0);
template<umm ps> FORCEINLINE v4fx<ps> V4fx(f32 = 0);
template<umm ps> FORCEINLINE v4sx<ps> V4sx(s32 = 0);
template<umm ps> FORCEINLINE v4ux<ps> V4ux(u32 = 0);

template<umm ps> FORCEINLINE v2fx<ps> V2fx(f32, f32);
template<umm ps> FORCEINLINE v2sx<ps> V2sx(s32, s32);
template<umm ps> FORCEINLINE v2ux<ps> V2ux(u32, u32);
template<umm ps> FORCEINLINE v3fx<ps> V3fx(f32, f32, f32);
template<umm ps> FORCEINLINE v3sx<ps> V3sx(s32, s32, s32);
template<umm ps> FORCEINLINE v3ux<ps> V3ux(u32, u32, u32);
template<umm ps> FORCEINLINE v4fx<ps> V4fx(f32, f32, f32, f32);
template<umm ps> FORCEINLINE v4sx<ps> V4sx(s32, s32, s32, s32);
template<umm ps> FORCEINLINE v4ux<ps> V4ux(u32, u32, u32, u32);

template<umm ps> FORCEINLINE v2fx<ps> V2fx(f32x<ps>);
template<umm ps> FORCEINLINE v2sx<ps> V2sx(s32x<ps>);
template<umm ps> FORCEINLINE v2ux<ps> V2ux(u32x<ps>);
template<umm ps> FORCEINLINE v3fx<ps> V3fx(f32x<ps>);
template<umm ps> FORCEINLINE v3sx<ps> V3sx(s32x<ps>);
template<umm ps> FORCEINLINE v3ux<ps> V3ux(u32x<ps>);
template<umm ps> FORCEINLINE v4fx<ps> V4fx(f32x<ps>);
template<umm ps> FORCEINLINE v4sx<ps> V4sx(s32x<ps>);
template<umm ps> FORCEINLINE v4ux<ps> V4ux(u32x<ps>);

template<umm ps> FORCEINLINE v2fx<ps> V2fx(f32x<ps>, f32x<ps>);
template<umm ps> FORCEINLINE v2sx<ps> V2sx(s32x<ps>, s32x<ps>);
template<umm ps> FORCEINLINE v2ux<ps> V2ux(u32x<ps>, u32x<ps>);
template<umm ps> FORCEINLINE v3fx<ps> V3fx(f32x<ps>, f32x<ps>, f32x<ps>);
template<umm ps> FORCEINLINE v3sx<ps> V3sx(s32x<ps>, s32x<ps>, s32x<ps>);
template<umm ps> FORCEINLINE v3ux<ps> V3ux(u32x<ps>, u32x<ps>, u32x<ps>);
template<umm ps> FORCEINLINE v4fx<ps> V4fx(f32x<ps>, f32x<ps>, f32x<ps>, f32x<ps>);
template<umm ps> FORCEINLINE v4sx<ps> V4sx(s32x<ps>, s32x<ps>, s32x<ps>, s32x<ps>);
template<umm ps> FORCEINLINE v4ux<ps> V4ux(u32x<ps>, u32x<ps>, u32x<ps>, u32x<ps>);

template<umm ps> FORCEINLINE v2fx<ps> V2fx(v2f);
template<umm ps> FORCEINLINE v2sx<ps> V2sx(v2s);
template<umm ps> FORCEINLINE v2ux<ps> V2ux(v2u);
template<umm ps> FORCEINLINE v3fx<ps> V3fx(v3f);
template<umm ps> FORCEINLINE v3sx<ps> V3sx(v3s);
template<umm ps> FORCEINLINE v3ux<ps> V3ux(v3u);
template<umm ps> FORCEINLINE v4fx<ps> V4fx(v4f);
template<umm ps> FORCEINLINE v4sx<ps> V4sx(v4s);
template<umm ps> FORCEINLINE v4ux<ps> V4ux(v4u);

FORCEINLINE constexpr v2f V2f(f32 x, f32 y);
FORCEINLINE constexpr v2s V2s(s32 x, s32 y);
FORCEINLINE constexpr v2u V2u(u32 x, u32 y);

FORCEINLINE constexpr v3f V3f(f32 x, f32 y, f32 z);
FORCEINLINE constexpr v3s V3s(s32 x, s32 y, s32 z);
FORCEINLINE constexpr v3u V3u(u32 x, u32 y, u32 z);

FORCEINLINE constexpr v4f V4f(f32x4);
FORCEINLINE constexpr v4s V4s(s32x4);
FORCEINLINE constexpr v4u V4u(u32x4);

FORCEINLINE v2fx4 V2fx4(f32 = 0);
FORCEINLINE v2sx4 V2sx4(s32 = 0);
FORCEINLINE v2ux4 V2ux4(u32 = 0);

FORCEINLINE constexpr v2fx4 V2fx4(f32x4);
FORCEINLINE constexpr v2sx4 V2sx4(s32x4);
FORCEINLINE constexpr v2ux4 V2ux4(u32x4);

FORCEINLINE constexpr v3fx4 V3fx4(f32x4);
FORCEINLINE constexpr v3sx4 V3sx4(s32x4);
FORCEINLINE constexpr v3ux4 V3ux4(u32x4);

FORCEINLINE v3fx4 V3fx4(f32 v);
FORCEINLINE v3sx4 V3sx4(s32 v);
FORCEINLINE v3ux4 V3ux4(u32 v);

FORCEINLINE v2fx8 V2fx8(f32 v);
FORCEINLINE v2sx8 V2sx8(s32 v);
FORCEINLINE v2ux8 V2ux8(u32 v);

FORCEINLINE constexpr v2fx8 V2fx8(f32x8 v);
FORCEINLINE constexpr v2sx8 V2sx8(s32x8 v);
FORCEINLINE constexpr v2ux8 V2ux8(u32x8 v);

FORCEINLINE v3fx8 V3fx8(f32 v);
FORCEINLINE v3sx8 V3sx8(s32 v);
FORCEINLINE v3ux8 V3ux8(u32 v);

FORCEINLINE constexpr v3fx8 V3fx8(f32x8 v);
FORCEINLINE constexpr v3sx8 V3sx8(s32x8 v);
FORCEINLINE constexpr v3ux8 V3ux8(u32x8 v);

FORCEINLINE v4fx4 V4fx4(f32);
FORCEINLINE v4sx4 V4sx4(s32);
FORCEINLINE v4ux4 V4ux4(u32);

FORCEINLINE constexpr v4fx4 V4fx4(f32x4);
FORCEINLINE constexpr v4sx4 V4sx4(s32x4);
FORCEINLINE constexpr v4ux4 V4ux4(u32x4);

FORCEINLINE v4fx8 V4fx8(f32);
FORCEINLINE v4sx8 V4sx8(s32);
FORCEINLINE v4ux8 V4ux8(u32);

FORCEINLINE constexpr v4fx8 V4fx8(f32x8);
FORCEINLINE constexpr v4sx8 V4sx8(s32x8);
FORCEINLINE constexpr v4ux8 V4ux8(u32x8);

FORCEINLINE constexpr m4 M4();
FORCEINLINE constexpr m4 M4(v4f i, v4f j, v4f k, v4f l);
FORCEINLINE constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw);

FORCEINLINE void sincos(v2f v, v2f& sinOut, v2f& cosOut);
FORCEINLINE void sincos(v3f v, v3f& sinOut, v3f& cosOut);
FORCEINLINE void sincos(v4f v, v4f& sinOut, v4f& cosOut);

#define MOP(op, vec)			FORCEINLINE vec& operator op##=(vec b) { return *this = *this op b; }
#define MOPS(op, vec, scl, con)	FORCEINLINE vec& operator op##=(scl b) { return *this op##= con(b); }
#define OPS(op, vec, scl, con)	FORCEINLINE vec operator op(scl b) const { return *this op con(b); }
#define SOP(op, vec, scl, con)	FORCEINLINE friend vec operator op(scl a, vec b) { return con(a) op b; }

#define AOP(op, vec, scl, con) MOP(op, vec) MOPS(op, vec, scl, con) OPS(op, vec, scl, con) SOP(op, vec, scl, con)

#define MEMFUNS_BASIC_SCL(vec, scl, con) \
	SOP(+, vec, scl, con)                \
	SOP(-, vec, scl, con)                \
	SOP(*, vec, scl, con)                \
	SOP(/, vec, scl, con)                \
	OPS(+, vec, scl, con)                \
	OPS(-, vec, scl, con)                \
	OPS(*, vec, scl, con)                \
	OPS(/, vec, scl, con)                \
	MOPS(+, vec, scl, con)               \
	MOPS(-, vec, scl, con)               \
	MOPS(*, vec, scl, con)               \
	MOPS(/, vec, scl, con)

#define MEMFUNS_BASIC(vec, scl, con) \
	MOP(+, vec)                      \
	MOP(-, vec)                      \
	MOP(*, vec)                      \
	MOP(/, vec)                      \
	MEMFUNS_BASIC_SCL(vec, scl, con)

#define NEQ(vec) \
	FORCEINLINE bool operator!=(vec b) const { return !(*this == b); }

#define MEMFUNS_DATA(scl)               \
	FORCEINLINE constexpr scl* data() { return s; } \
	FORCEINLINE constexpr scl const* data() const { return s; }

#define INDEX_S(scl)                                          \
	FORCEINLINE constexpr scl operator[](umm i) const { return s[i]; } \
	FORCEINLINE constexpr scl& operator[](umm i) { return s[i]; }

#define MEMFUNS_INT(vec, scl, con)                              \
	FORCEINLINE vec& operator%=(vec b) { return *this = *this % b; }        \
	FORCEINLINE vec& operator^=(vec b) { return *this = *this ^ b; }        \
	FORCEINLINE vec& operator|=(vec b) { return *this = *this | b; }        \
	FORCEINLINE vec& operator&=(vec b) { return *this = *this & b; }        \
	FORCEINLINE vec& operator<<=(vec b) { return *this = *this << b; }      \
	FORCEINLINE vec& operator>>=(vec b) { return *this = *this >> b; }      \
	FORCEINLINE vec operator%(scl b) const { return *this % con(b); }       \
	FORCEINLINE vec operator^(scl b) const { return *this ^ con(b); }       \
	FORCEINLINE vec operator|(scl b) const { return *this | con(b); }       \
	FORCEINLINE vec operator&(scl b) const { return *this & con(b); }       \
	FORCEINLINE vec& operator%=(scl b) { return *this %= con(b); }          \
	FORCEINLINE vec& operator^=(scl b) { return *this ^= con(b); }          \
	FORCEINLINE vec& operator|=(scl b) { return *this |= con(b); }          \
	FORCEINLINE vec& operator&=(scl b) { return *this &= con(b); }          \
	FORCEINLINE vec& operator<<=(scl b) { return *this = *this << b; }      \
	FORCEINLINE vec& operator>>=(scl b) { return *this = *this >> b; }      \
	FORCEINLINE friend vec operator%(scl a, vec b) { return con(a) % b; }   \
	FORCEINLINE friend vec operator^(scl a, vec b) { return con(a) ^ b; }   \
	FORCEINLINE friend vec operator|(scl a, vec b) { return con(a) | b; }   \
	FORCEINLINE friend vec operator&(scl a, vec b) { return con(a) & b; }   \
	FORCEINLINE friend vec operator<<(scl a, vec b) { return con(a) << b; } \
	FORCEINLINE friend vec operator>>(scl a, vec b) { return con(a) >> b; }

#define MEMFUNS_SHIFT_SCL(vec, scl, con)                    \
	FORCEINLINE vec operator<<(scl b) const { return *this << con(b); } \
	FORCEINLINE vec operator>>(scl b) const { return *this >> con(b); }

#define MEMFUNS_SHIFT_SCL_M(vec, scl, con)              \
	FORCEINLINE vec operator<<(scl b) const { return con(m << b); } \
	FORCEINLINE vec operator>>(scl b) const { return con(m >> b); }

#define MEMFUNS_SHIFT_SCL_2(vec, scl)                        \
	FORCEINLINE vec operator<<(scl b) const { return {x << b, y << b}; } \
	FORCEINLINE vec operator>>(scl b) const { return {x >> b, y >> b}; }

#define MEMFUNS_SHIFT_SCL_3(vec, scl)                                \
	FORCEINLINE vec operator<<(scl b) const { return {x << b, y << b, z << b}; } \
	FORCEINLINE vec operator>>(scl b) const { return {x >> b, y >> b, z >> b}; }

#define MEMFUNS_SHIFT_SCL_4(vec, scl)                                        \
	FORCEINLINE vec operator<<(scl b) const { return {x << b, y << b, z << b, w << b}; } \
	FORCEINLINE vec operator>>(scl b) const { return {x >> b, y >> b, z >> b, w >> b}; }

#define FALLBACK_UN_OP(ret, op, con, _128)			FORCEINLINE ret operator op() const { return con(op _128[0], op _128[1]); }
#define FALLBACK_BIN_OP(ret, op, con, _128)			FORCEINLINE ret operator op(ret b) const { return con(_128[0] op b._128[0], _128[1] op b._128[1]); }
#define FALLBACK_BIN_OP_S(ret, op, scl, con, _128)	FORCEINLINE ret operator op(scl b) const { return con(_128[0] op b, _128[1] op b); }
#define FALLBACK_CMP_OP(ret, op, param, _128)		FORCEINLINE ret operator op(param b) const { return {_128[0] op b._128[0], _128[1] op b._128[1]}; }

#define FALLBACK_F32X_OPERATORS(f32x8, F32x8, b32x8, _128) \
	FALLBACK_UN_OP(f32x8, -, F32x8, _128)                     \
	FALLBACK_BIN_OP(f32x8, +, F32x8, _128)                    \
	FALLBACK_BIN_OP(f32x8, -, F32x8, _128)                    \
	FALLBACK_BIN_OP(f32x8, *, F32x8, _128)                    \
	FALLBACK_BIN_OP(f32x8, /, F32x8, _128)                    \
	FALLBACK_CMP_OP(b32x8, <, f32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, >, f32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, <=, f32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, >=, f32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, ==, f32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, !=, f32x8, _128)

#define FALLBACK_S32X_OPERATORS(s32x8, S32x8, b32x8, _128) \
	FALLBACK_UN_OP(s32x8, -, S32x8, _128)                     \
	FALLBACK_BIN_OP(s32x8, +, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, -, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, *, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, /, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, %, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, ^, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, |, S32x8, _128)                    \
	FALLBACK_BIN_OP(s32x8, &, S32x8, _128)                    \
	FALLBACK_BIN_OP_S(s32x8, <<, s32, S32x8, _128)            \
	FALLBACK_BIN_OP_S(s32x8, >>, s32, S32x8, _128)            \
	FALLBACK_CMP_OP(b32x8, <, s32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, >, s32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, <=, s32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, >=, s32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, ==, s32x8, _128)

#define FALLBACK_U32X_OPERATORS(u32x8, U32x8, b32x8, _128) \
	FALLBACK_BIN_OP(u32x8, +, U32x8, _128)                 \
	FALLBACK_BIN_OP(u32x8, -, U32x8, _128)                 \
	FALLBACK_BIN_OP(u32x8, /, U32x8, _128)                 \
	FALLBACK_BIN_OP(u32x8, %, U32x8, _128)                 \
	FALLBACK_BIN_OP(u32x8, ^, U32x8, _128)                 \
	FALLBACK_BIN_OP(u32x8, |, U32x8, _128)                 \
	FALLBACK_BIN_OP(u32x8, &, U32x8, _128)                 \
	FALLBACK_BIN_OP_S(u32x8, <<, u32, U32x8, _128)         \
	FALLBACK_BIN_OP_S(u32x8, >>, u32, U32x8, _128)         \
	FALLBACK_CMP_OP(b32x8, <, u32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, >, u32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, <=, u32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, >=, u32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, ==, u32x8, _128)				   \
	FALLBACK_CMP_OP(b32x8, !=, u32x8, _128)

#define MASK_IMPL(b32, b32x4, M16, width)								\
	b32 s[width];														\
	M16 m;																\
	FORCEINLINE b32x4() = default;										\
	FORCEINLINE b32x4(M16 m) : m(m) {}									\
	FORCEINLINE b32x4  operator!() const { return !m; }					\
	FORCEINLINE b32x4  operator~() const { return ~m; }					\
	FORCEINLINE b32x4  operator==(b32x4 b) const { return m == b.m; }	\
	FORCEINLINE b32x4  operator!=(b32x4 b) const { return m != b.m; }	\
	FORCEINLINE b32x4  operator^ (b32x4 b) const { return m ^  b.m; }	\
	FORCEINLINE b32x4  operator& (b32x4 b) const { return m &  b.m; }	\
	FORCEINLINE b32x4  operator| (b32x4 b) const { return m |  b.m; }	\
	FORCEINLINE b32x4  operator&&(b32x4 b) const { return m &  b.m; }	\
	FORCEINLINE b32x4  operator||(b32x4 b) const { return m |  b.m; }	\
	FORCEINLINE b32x4& operator^=(b32x4 b) { return m ^= b.m, *this; }	\
	FORCEINLINE b32x4& operator&=(b32x4 b) { return m &= b.m, *this; }	\
	FORCEINLINE b32x4& operator|=(b32x4 b) { return m |= b.m, *this; }	\
	FORCEINLINE b32 operator[](umm index) const { return s[index]; }	\
	FORCEINLINE b32& operator[](umm index) { return s[index]; }

struct TrueMask {
	FORCEINLINE constexpr operator bool() const { return true; }
	FORCEINLINE constexpr operator u8 () const { return ~0; }
	FORCEINLINE constexpr operator u16() const { return ~0; }
	FORCEINLINE constexpr operator u32() const { return ~0; }
	FORCEINLINE constexpr operator u64() const { return ~0; }
	FORCEINLINE constexpr operator s8 () const { return ~0; }
	FORCEINLINE constexpr operator s16() const { return ~0; }
	FORCEINLINE constexpr operator s32() const { return ~0; }
	FORCEINLINE constexpr operator s64() const { return ~0; }
	FORCEINLINE operator b32x4() const;
	FORCEINLINE operator b32x8() const;
};
struct FalseMask {
	FORCEINLINE constexpr operator bool() const { return false; }
	FORCEINLINE constexpr operator u8 () const { return 0; }
	FORCEINLINE constexpr operator u16() const { return 0; }
	FORCEINLINE constexpr operator u32() const { return 0; }
	FORCEINLINE constexpr operator u64() const { return 0; }
	FORCEINLINE constexpr operator s8 () const { return 0; }
	FORCEINLINE constexpr operator s16() const { return 0; }
	FORCEINLINE constexpr operator s32() const { return 0; }
	FORCEINLINE constexpr operator s64() const { return 0; }
	FORCEINLINE operator b32x4() const;
	FORCEINLINE operator b32x8() const;
};

inline static constexpr TrueMask trueMask;
inline static constexpr FalseMask falseMask;

template <>
union b32x<1> {
	b32 m;
	b32 s[1];
	FORCEINLINE b32x operator!() const { return {~m}; }
	FORCEINLINE b32x operator~() const { return {~m}; }
	FORCEINLINE b32x operator==(b32x b) const { return {-(m == b.m)}; }
	FORCEINLINE b32x operator!=(b32x b) const { return {-(m != b.m)}; }
	FORCEINLINE b32x operator^ (b32x b) const { return {m ^ b.m}; }
	FORCEINLINE b32x operator& (b32x b) const { return {m & b.m}; }
	FORCEINLINE b32x operator| (b32x b) const { return {m | b.m}; }
	FORCEINLINE b32x operator&&(b32x b) const { return {m & b.m}; }
	FORCEINLINE b32x operator||(b32x b) const { return {m | b.m}; }
	FORCEINLINE b32x &operator^=(b32x b) { return m ^= b.m, *this; }
	FORCEINLINE b32x &operator&=(b32x b) { return m &= b.m, *this; }
	FORCEINLINE b32x &operator|=(b32x b) { return m |= b.m, *this; }
	FORCEINLINE b32 operator[](umm index) const { return s[index]; }
	FORCEINLINE b32 &operator[](umm index) { return s[index]; }
};

template <>
union b32x<4> {
	M16 m;
	b32 s[4];
	FORCEINLINE b32x operator!() const { return {_mm_xor_ps(m.ps, _mm_setone_ps())}; }
	FORCEINLINE b32x operator~() const { return {_mm_xor_ps(m.ps, _mm_setone_ps())}; }
	FORCEINLINE b32x operator==(b32x b) const { return {_mm_castpi_ps(_mm_cmpeq_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE b32x operator!=(b32x b) const { return {_mm_xor_ps(_mm_castpi_ps(_mm_cmpeq_epi32(m.pi, b.m.pi)), _mm_setone_ps())}; }
	FORCEINLINE b32x operator^ (b32x b) const { return {_mm_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator& (b32x b) const { return {_mm_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator| (b32x b) const { return {_mm_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator&&(b32x b) const { return {_mm_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator||(b32x b) const { return {_mm_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b32x &operator^=(b32x b) { return m.ps = _mm_xor_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b32x &operator&=(b32x b) { return m.ps = _mm_and_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b32x &operator|=(b32x b) { return m.ps = _mm_or_ps (m.ps, b.m.ps), *this; }
	FORCEINLINE b32 operator[](umm index) const { return s[index]; }
	FORCEINLINE b32 &operator[](umm index) { return s[index]; }
};

template<>
union b32x<8> {
#if ARCH_AVX
	M32 m;
	FORCEINLINE b32x operator!() const { return {_mm256_xor_ps(m.ps, _mm256_setone_ps())}; }
	FORCEINLINE b32x operator~() const { return {_mm256_xor_ps(m.ps, _mm256_setone_ps())}; }
	FORCEINLINE b32x operator==(b32x b) const { return {_mm256_not_ps(_mm256_xor_ps(m.ps, b.m.ps))}; }
	FORCEINLINE b32x operator!=(b32x b) const { return {_mm256_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator^ (b32x b) const { return {_mm256_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator& (b32x b) const { return {_mm256_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator| (b32x b) const { return {_mm256_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator&&(b32x b) const { return {_mm256_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b32x operator||(b32x b) const { return {_mm256_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b32x &operator^=(b32x b) { return m.ps = _mm256_xor_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b32x &operator&=(b32x b) { return m.ps = _mm256_and_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b32x &operator|=(b32x b) { return m.ps = _mm256_or_ps (m.ps, b.m.ps), *this; }
#else
	struct {
		M16 lo, hi;
	}; 
	FORCEINLINE b32x operator!() const { return {_mm_xor_ps(lo.ps, _mm_setone_ps()), _mm_xor_ps(hi.ps, _mm_setone_ps())}; }
	FORCEINLINE b32x operator~() const { return {_mm_xor_ps(lo.ps, _mm_setone_ps()), _mm_xor_ps(hi.ps, _mm_setone_ps())}; }
	FORCEINLINE b32x operator==(b32x b) const { return {_mm_castpi_ps(_mm_cmpeq_epi8(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpeq_epi8(hi.pi, b.hi.pi))}; }
	FORCEINLINE b32x operator!=(b32x b) const { return {_mm_xor_ps(_mm_castpi_ps(_mm_cmpeq_epi8(lo.pi, b.lo.pi)), _mm_setone_ps()), _mm_xor_ps(_mm_castpi_ps(_mm_cmpeq_epi8(hi.pi, b.hi.pi)), _mm_setone_ps())}; }
	FORCEINLINE b32x operator^ (b32x b) const { return {_mm_xor_ps(lo.ps, b.lo.ps), _mm_xor_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE b32x operator& (b32x b) const { return {_mm_and_ps(lo.ps, b.lo.ps), _mm_and_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE b32x operator| (b32x b) const { return {_mm_or_ps (lo.ps, b.lo.ps), _mm_or_ps (hi.ps, b.hi.ps)}; }
	FORCEINLINE b32x operator&&(b32x b) const { return {_mm_and_ps(lo.ps, b.lo.ps), _mm_and_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE b32x operator||(b32x b) const { return {_mm_or_ps (lo.ps, b.lo.ps), _mm_or_ps (hi.ps, b.hi.ps)}; }
	FORCEINLINE b32x &operator^=(b32x b) { lo.ps = _mm_xor_ps(lo.ps, b.lo.ps); hi.ps = _mm_xor_ps(hi.ps, b.hi.ps); return *this; }
	FORCEINLINE b32x &operator&=(b32x b) { lo.ps = _mm_and_ps(lo.ps, b.lo.ps); hi.ps = _mm_and_ps(hi.ps, b.hi.ps); return *this; }
	FORCEINLINE b32x &operator|=(b32x b) { lo.ps = _mm_or_ps (lo.ps, b.lo.ps); hi.ps = _mm_or_ps (hi.ps, b.hi.ps); return *this; }
#endif
	b32 s[8];
	FORCEINLINE b32 operator[](umm index) const { return s[index]; }
	FORCEINLINE b32 &operator[](umm index) { return s[index]; }
};

template<>
union b64x<2> {
	M16 m;
	b64 s[2];
	FORCEINLINE b64x operator!() const { return {_mm_xor_ps(m.ps, _mm_setone_ps())}; }
	FORCEINLINE b64x operator~() const { return {_mm_xor_ps(m.ps, _mm_setone_ps())}; }
	FORCEINLINE b64x operator==(b64x b) const { return {_mm_castpi_ps(_mm_cmpeq_epi8(m.pi, b.m.pi))}; }
	FORCEINLINE b64x operator!=(b64x b) const { return {_mm_xor_ps(_mm_castpi_ps(_mm_cmpeq_epi8(m.pi, b.m.pi)), _mm_setone_ps())}; }
	FORCEINLINE b64x operator^ (b64x b) const { return {_mm_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator& (b64x b) const { return {_mm_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator| (b64x b) const { return {_mm_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator&&(b64x b) const { return {_mm_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator||(b64x b) const { return {_mm_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b64x &operator^=(b64x b) { return m.ps = _mm_xor_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b64x &operator&=(b64x b) { return m.ps = _mm_and_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b64x &operator|=(b64x b) { return m.ps = _mm_or_ps (m.ps, b.m.ps), *this; }
	FORCEINLINE b64 operator[](umm index) const { return s[index]; }
	FORCEINLINE b64 &operator[](umm index) { return s[index]; }
};

template<>
union b64x<4> {
#if ARCH_AVX
	M32 m;
	FORCEINLINE b64x operator!() const { return {_mm256_xor_ps(m.ps, _mm256_setone_ps())}; }
	FORCEINLINE b64x operator~() const { return {_mm256_xor_ps(m.ps, _mm256_setone_ps())}; }
	FORCEINLINE b64x operator==(b64x b) const { return {_mm256_not_ps(_mm256_xor_ps(m.ps, b.m.ps))}; }
	FORCEINLINE b64x operator!=(b64x b) const { return {_mm256_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator^ (b64x b) const { return {_mm256_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator& (b64x b) const { return {_mm256_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator| (b64x b) const { return {_mm256_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator&&(b64x b) const { return {_mm256_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE b64x operator||(b64x b) const { return {_mm256_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE b64x &operator^=(b64x b) { return m.ps = _mm256_xor_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b64x &operator&=(b64x b) { return m.ps = _mm256_and_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE b64x &operator|=(b64x b) { return m.ps = _mm256_or_ps (m.ps, b.m.ps), *this; }
#else
	struct {
		M16 lo, hi;
	}; 
	FORCEINLINE b64x operator!() const { return {_mm_xor_ps(lo.ps, _mm_setone_ps()), _mm_xor_ps(hi.ps, _mm_setone_ps())}; }
	FORCEINLINE b64x operator~() const { return {_mm_xor_ps(lo.ps, _mm_setone_ps()), _mm_xor_ps(hi.ps, _mm_setone_ps())}; }
	FORCEINLINE b64x operator==(b64x b) const { return {_mm_castpi_ps(_mm_cmpeq_epi8(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpeq_epi8(hi.pi, b.hi.pi))}; }
	FORCEINLINE b64x operator!=(b64x b) const { return {_mm_xor_ps(_mm_castpi_ps(_mm_cmpeq_epi8(lo.pi, b.lo.pi)), _mm_setone_ps()), _mm_xor_ps(_mm_castpi_ps(_mm_cmpeq_epi8(hi.pi, b.hi.pi)), _mm_setone_ps())}; }
	FORCEINLINE b64x operator^ (b64x b) const { return {_mm_xor_ps(lo.ps, b.lo.ps), _mm_xor_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE b64x operator& (b64x b) const { return {_mm_and_ps(lo.ps, b.lo.ps), _mm_and_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE b64x operator| (b64x b) const { return {_mm_or_ps (lo.ps, b.lo.ps), _mm_or_ps (hi.ps, b.hi.ps)}; }
	FORCEINLINE b64x operator&&(b64x b) const { return {_mm_and_ps(lo.ps, b.lo.ps), _mm_and_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE b64x operator||(b64x b) const { return {_mm_or_ps (lo.ps, b.lo.ps), _mm_or_ps (hi.ps, b.hi.ps)}; }
	FORCEINLINE b64x &operator^=(b64x b) { lo.ps = _mm_xor_ps(lo.ps, b.lo.ps); hi.ps = _mm_xor_ps(hi.ps, b.hi.ps); return *this; }
	FORCEINLINE b64x &operator&=(b64x b) { lo.ps = _mm_and_ps(lo.ps, b.lo.ps); hi.ps = _mm_and_ps(hi.ps, b.hi.ps); return *this; }
	FORCEINLINE b64x &operator|=(b64x b) { lo.ps = _mm_or_ps (lo.ps, b.lo.ps); hi.ps = _mm_or_ps (hi.ps, b.hi.ps); return *this; }
#endif
	b64 s[4];
	FORCEINLINE b64 operator[](umm index) const { return s[index]; }
	FORCEINLINE b64 &operator[](umm index) { return s[index]; }
};
#undef MASK_IMPL

FORCEINLINE bool anyTrue (b32x1 v) { return v.m != 0;   }
FORCEINLINE bool allTrue (b32x1 v) { return v.m == 1; }
FORCEINLINE bool allFalse(b32x1 v) { return v.m == 0;   }
FORCEINLINE bool anyFalse(b32x1 v) { return v.m != 1; }

FORCEINLINE TrueMask::operator b32x4() const { return {_mm_setone_ps()}; }
FORCEINLINE FalseMask::operator b32x4() const { return {_mm_setzero_ps()}; }
FORCEINLINE u64 compressMask(b32x4 a) { return _mm_movemask_ps(a.m.ps); }
FORCEINLINE u64 compressMask(b64x2 a) { return _mm_movemask_pd(a.m.pd); }
FORCEINLINE b32x4 B32x4(bool v) { return {v ? _mm_setone_ps() : _mm_setzero_ps()}; }
FORCEINLINE b32x4 andNot(b32x4 a, b32x4 b) { return {_mm_andnot_ps(b.m.ps, a.m.ps)}; }
FORCEINLINE bool anyTrue (b32x4 v) { return _mm_movemask_ps(v.m.ps) != 0;   }
FORCEINLINE bool allTrue (b32x4 v) { return _mm_movemask_ps(v.m.ps) == 0xF; }
FORCEINLINE bool allFalse(b32x4 v) { return _mm_movemask_ps(v.m.ps) == 0;   }
FORCEINLINE bool anyFalse(b32x4 v) { return _mm_movemask_ps(v.m.ps) != 0xF; }
FORCEINLINE bool anyTrue (b64x2 v) { return _mm_movemask_pd(v.m.pd) != 0;   }
FORCEINLINE bool allTrue (b64x2 v) { return _mm_movemask_pd(v.m.pd) == 0x3; }
FORCEINLINE bool allFalse(b64x2 v) { return _mm_movemask_pd(v.m.pd) == 0;   }
FORCEINLINE bool anyFalse(b64x2 v) { return _mm_movemask_pd(v.m.pd) != 0x3; }

#if ARCH_AVX
FORCEINLINE TrueMask::operator b32x8() const { return {_mm256_setone_ps()}; }
FORCEINLINE FalseMask::operator b32x8() const { return {_mm256_setzero_ps()}; }
FORCEINLINE u64 compressMask(b32x8 a) { return _mm256_movemask_ps(a.m.ps); }
FORCEINLINE u64 compressMask(b64x4 a) { return _mm256_movemask_pd(a.m.pd); }
FORCEINLINE b32x8 B32x8(bool v) { return {v ? _mm256_setone_ps() : _mm256_setzero_ps()}; }
FORCEINLINE b32x8 andNot(b32x8 a, b32x8 b) { return {_mm256_andnot_ps(b.m.ps, a.m.ps)}; }
FORCEINLINE bool anyTrue (b32x8 v) { return _mm256_movemask_ps(v.m.ps) != 0;    }
FORCEINLINE bool allTrue (b32x8 v) { return _mm256_movemask_ps(v.m.ps) == 0xFF; }
FORCEINLINE bool allFalse(b32x8 v) { return _mm256_movemask_ps(v.m.ps) == 0;    }
FORCEINLINE bool anyFalse(b32x8 v) { return _mm256_movemask_ps(v.m.ps) != 0xFF; }
FORCEINLINE bool anyTrue (b64x4 v) { return _mm256_movemask_pd(v.m.pd) != 0;   }
FORCEINLINE bool allTrue (b64x4 v) { return _mm256_movemask_pd(v.m.pd) == 0xF; }
FORCEINLINE bool allFalse(b64x4 v) { return _mm256_movemask_pd(v.m.pd) == 0;   }
FORCEINLINE bool anyFalse(b64x4 v) { return _mm256_movemask_pd(v.m.pd) != 0xF; }
#else
FORCEINLINE TrueMask::operator b32x8() const { return {_mm_setone_ps(), _mm_setone_ps()}; }
FORCEINLINE FalseMask::operator b32x8() const { return {_mm_setzero_ps(), _mm_setzero_ps()}; }
FORCEINLINE u64 compressMask(b32x8 a) { return (_mm_movemask_ps(a.lo.ps) << 8) | _mm_movemask_ps(a.hi.ps); }
FORCEINLINE u64 compressMask(b64x4 a) { return (_mm_movemask_pd(a.lo.pd) << 4) | _mm_movemask_pd(a.hi.pd); }
FORCEINLINE b32x8 B32x8(bool v) { return v ? b32x8{_mm_setone_ps(), _mm_setone_ps()} : b32x8{_mm_setzero_ps(), _mm_setzero_ps()}; }
FORCEINLINE b32x8 andNot(b32x8 a, b32x8 b) { return {_mm_andnot_ps(b.lo.ps, a.lo.ps), _mm_andnot_ps(b.hi.ps, a.hi.ps)}; }
FORCEINLINE bool anyTrue (b32x8 a) { return ((_mm_movemask_ps(a.lo.ps) << 8) | _mm_movemask_ps(a.hi.ps)) != 0;    }
FORCEINLINE bool allTrue (b32x8 a) { return ((_mm_movemask_ps(a.lo.ps) << 8) | _mm_movemask_ps(a.hi.ps)) == 0xFF; }
FORCEINLINE bool allFalse(b32x8 a) { return ((_mm_movemask_ps(a.lo.ps) << 8) | _mm_movemask_ps(a.hi.ps)) == 0;    }
FORCEINLINE bool anyFalse(b32x8 a) { return ((_mm_movemask_ps(a.lo.ps) << 8) | _mm_movemask_ps(a.hi.ps)) != 0xFF; }
FORCEINLINE bool anyTrue (b64x4 a) { return ((_mm_movemask_pd(a.lo.pd) << 4) | _mm_movemask_pd(a.hi.pd)) != 0;   }
FORCEINLINE bool allTrue (b64x4 a) { return ((_mm_movemask_pd(a.lo.pd) << 4) | _mm_movemask_pd(a.hi.pd)) == 0xF; }
FORCEINLINE bool allFalse(b64x4 a) { return ((_mm_movemask_pd(a.lo.pd) << 4) | _mm_movemask_pd(a.hi.pd)) == 0;   }
FORCEINLINE bool anyFalse(b64x4 a) { return ((_mm_movemask_pd(a.lo.pd) << 4) | _mm_movemask_pd(a.hi.pd)) != 0xF; }
#endif

#define SCLX_CMP(b32x8, f32x8, f32, F32x8)                                        \
	FORCEINLINE b32x8 operator<(f32 b) const { return *this < F32x8(b); }         \
	FORCEINLINE b32x8 operator>(f32 b) const { return *this > F32x8(b); }         \
	FORCEINLINE b32x8 operator<=(f32 b) const { return *this <= F32x8(b); }       \
	FORCEINLINE b32x8 operator>=(f32 b) const { return *this >= F32x8(b); }       \
	FORCEINLINE b32x8 operator==(f32 b) const { return *this == F32x8(b); }       \
	FORCEINLINE b32x8 operator!=(f32 b) const { return *this != F32x8(b); }       \
	FORCEINLINE friend b32x8 operator<(f32 a, f32x8 b) { return F32x8(a) < b; }   \
	FORCEINLINE friend b32x8 operator>(f32 a, f32x8 b) { return F32x8(a) > b; }   \
	FORCEINLINE friend b32x8 operator<=(f32 a, f32x8 b) { return F32x8(a) <= b; } \
	FORCEINLINE friend b32x8 operator>=(f32 a, f32x8 b) { return F32x8(a) >= b; } \
	FORCEINLINE friend b32x8 operator==(f32 a, f32x8 b) { return F32x8(a) == b; } \
	FORCEINLINE friend b32x8 operator!=(f32 a, f32x8 b) { return F32x8(a) != b; }

#define CVT(other) explicit FORCEINLINE operator other() const

template<>
union f32x<1> {
	f32 m;
	f32 s[1];

	FORCEINLINE        f32x1  operator-()          const { return {-m}; }
	FORCEINLINE	       f32x1  operator+ (f32x1 b)  const { return {m +  b.m}; }
	FORCEINLINE	       f32x1  operator- (f32x1 b)  const { return {m -  b.m}; }
	FORCEINLINE	       f32x1  operator* (f32x1 b)  const { return {m *  b.m}; }
	FORCEINLINE	       f32x1  operator/ (f32x1 b)  const { return {m /  b.m}; }
	FORCEINLINE	       b32x1  operator< (f32x1 b)  const { return {m <  b.m}; }
	FORCEINLINE	       b32x1  operator> (f32x1 b)  const { return {m >  b.m}; }
	FORCEINLINE	       b32x1  operator<=(f32x1 b)  const { return {m <= b.m}; }
	FORCEINLINE	       b32x1  operator>=(f32x1 b)  const { return {m >= b.m}; }
	FORCEINLINE	       b32x1  operator==(f32x1 b)  const { return {m == b.m}; }
	FORCEINLINE	       b32x1  operator!=(f32x1 b)  const { return {m != b.m}; }
	FORCEINLINE	       f32x1  operator+ (f32   b)  const { return {m +  b  }; }
	FORCEINLINE	       f32x1  operator- (f32   b)  const { return {m -  b  }; }
	FORCEINLINE	       f32x1  operator* (f32   b)  const { return {m *  b  }; }
	FORCEINLINE	       f32x1  operator/ (f32   b)  const { return {m /  b  }; }
	FORCEINLINE	       b32x1  operator< (f32   b)  const { return {m <  b  }; }
	FORCEINLINE	       b32x1  operator> (f32   b)  const { return {m >  b  }; }
	FORCEINLINE	       b32x1  operator<=(f32   b)  const { return {m <= b  }; }
	FORCEINLINE	       b32x1  operator>=(f32   b)  const { return {m >= b  }; }
	FORCEINLINE	       b32x1  operator==(f32   b)  const { return {m == b  }; }
	FORCEINLINE	       b32x1  operator!=(f32   b)  const { return {m != b  }; }
	FORCEINLINE friend f32x1  operator+ (f32 a, f32x1 b) { return {a +  b.m}; }
	FORCEINLINE friend f32x1  operator- (f32 a, f32x1 b) { return {a -  b.m}; }
	FORCEINLINE friend f32x1  operator* (f32 a, f32x1 b) { return {a *  b.m}; }
	FORCEINLINE friend f32x1  operator/ (f32 a, f32x1 b) { return {a /  b.m}; }
	FORCEINLINE friend b32x1  operator< (f32 a, f32x1 b) { return {a <  b.m}; }
	FORCEINLINE friend b32x1  operator> (f32 a, f32x1 b) { return {a >  b.m}; }
	FORCEINLINE friend b32x1  operator<=(f32 a, f32x1 b) { return {a <= b.m}; }
	FORCEINLINE friend b32x1  operator>=(f32 a, f32x1 b) { return {a >= b.m}; }
	FORCEINLINE friend b32x1  operator==(f32 a, f32x1 b) { return {a == b.m}; }
	FORCEINLINE friend b32x1  operator!=(f32 a, f32x1 b) { return {a != b.m}; }
	FORCEINLINE        f32x1 &operator+=(f32x1 b)        { return m += b.m, *this; }
	FORCEINLINE        f32x1 &operator-=(f32x1 b)        { return m -= b.m, *this; }
	FORCEINLINE        f32x1 &operator*=(f32x1 b)        { return m *= b.m, *this; }
	FORCEINLINE        f32x1 &operator/=(f32x1 b)        { return m /= b.m, *this; }
	FORCEINLINE        f32x1 &operator+=(f32   b)        { return m += b  , *this; }
	FORCEINLINE        f32x1 &operator-=(f32   b)        { return m -= b  , *this; }
	FORCEINLINE        f32x1 &operator*=(f32   b)        { return m *= b  , *this; }
	FORCEINLINE        f32x1 &operator/=(f32   b)        { return m /= b  , *this; }
	FORCEINLINE        f32x1 &operator++ ()              { return ++m, *this; }
	FORCEINLINE        f32x1 &operator-- ()              { return --m, *this; }
	FORCEINLINE        f32x1  operator++ (int)           { return {m++}; }
	FORCEINLINE        f32x1  operator-- (int)           { return {m--}; }

	CVT(s32x1);
	CVT(u32x1);
	MEMFUNS_DATA(f32);
	INDEX_S(f32);
};

#if ARCH_AVX
#define CMPLTF32x4(a, b) _mm_cmp_ps(a, b, _CMP_LT_OQ)
#define CMPGTF32x4(a, b) _mm_cmp_ps(a, b, _CMP_GT_OQ)
#define CMPLEF32x4(a, b) _mm_cmp_ps(a, b, _CMP_LE_OQ)
#define CMPGEF32x4(a, b) _mm_cmp_ps(a, b, _CMP_GE_OQ)
#define CMPEQF32x4(a, b) _mm_cmp_ps(a, b, _CMP_EQ_OQ)
#define CMPNEF32x4(a, b) _mm_cmp_ps(a, b, _CMP_NEQ_OQ)

#define CMPLTF64x2(a, b) _mm_cmp_pd(a, b, _CMP_LT_OQ)
#define CMPGTF64x2(a, b) _mm_cmp_pd(a, b, _CMP_GT_OQ)
#define CMPLEF64x2(a, b) _mm_cmp_pd(a, b, _CMP_LE_OQ)
#define CMPGEF64x2(a, b) _mm_cmp_pd(a, b, _CMP_GE_OQ)
#define CMPEQF64x2(a, b) _mm_cmp_pd(a, b, _CMP_EQ_OQ)
#define CMPNEF64x2(a, b) _mm_cmp_pd(a, b, _CMP_NEQ_OQ)
#else
#define CMPLTF32x4(a, b) _mm_cmplt_ps (a, b)
#define CMPGTF32x4(a, b) _mm_cmpgt_ps (a, b)
#define CMPLEF32x4(a, b) _mm_cmple_ps (a, b)
#define CMPGEF32x4(a, b) _mm_cmpge_ps (a, b)
#define CMPEQF32x4(a, b) _mm_cmpeq_ps (a, b)
#define CMPNEF32x4(a, b) _mm_cmpneq_ps(a, b)

#define CMPLTF64x2(a, b) _mm_cmplt_pd (a, b)
#define CMPGTF64x2(a, b) _mm_cmpgt_pd (a, b)
#define CMPLEF64x2(a, b) _mm_cmple_pd (a, b)
#define CMPGEF64x2(a, b) _mm_cmpge_pd (a, b)
#define CMPEQF64x2(a, b) _mm_cmpeq_pd (a, b)
#define CMPNEF64x2(a, b) _mm_cmpneq_pd(a, b)
#endif

template<>
union f32x<4> {
	M16 m;
	f32 s[4];
	
	FORCEINLINE        f32x4  operator-()          const { return {_mm_xor_ps(m.ps, _mm_set1_ps(-0.0f))}; }
	FORCEINLINE	       f32x4  operator+ (f32x4 b)  const { return {_mm_add_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x4  operator- (f32x4 b)  const { return {_mm_sub_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x4  operator* (f32x4 b)  const { return {_mm_mul_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x4  operator/ (f32x4 b)  const { return {_mm_div_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x4  operator< (f32x4 b)  const { return {CMPLTF32x4(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x4  operator> (f32x4 b)  const { return {CMPGTF32x4(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x4  operator<=(f32x4 b)  const { return {CMPLEF32x4(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x4  operator>=(f32x4 b)  const { return {CMPGEF32x4(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x4  operator==(f32x4 b)  const { return {CMPEQF32x4(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x4  operator!=(f32x4 b)  const { return {CMPNEF32x4(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x4  operator+ (f32   b)  const { return {_mm_add_ps(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       f32x4  operator- (f32   b)  const { return {_mm_sub_ps(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       f32x4  operator* (f32   b)  const { return {_mm_mul_ps(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       f32x4  operator/ (f32   b)  const { return {_mm_div_ps(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x4  operator< (f32   b)  const { return {CMPLTF32x4(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x4  operator> (f32   b)  const { return {CMPGTF32x4(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x4  operator<=(f32   b)  const { return {CMPLEF32x4(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x4  operator>=(f32   b)  const { return {CMPGEF32x4(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x4  operator==(f32   b)  const { return {CMPEQF32x4(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x4  operator!=(f32   b)  const { return {CMPNEF32x4(m.ps, _mm_set1_ps(b))}; }
	FORCEINLINE friend f32x4  operator+ (f32 a, f32x4 b) { return {_mm_add_ps(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend f32x4  operator- (f32 a, f32x4 b) { return {_mm_sub_ps(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend f32x4  operator* (f32 a, f32x4 b) { return {_mm_mul_ps(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend f32x4  operator/ (f32 a, f32x4 b) { return {_mm_div_ps(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x4  operator< (f32 a, f32x4 b) { return {CMPLTF32x4(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x4  operator> (f32 a, f32x4 b) { return {CMPGTF32x4(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x4  operator<=(f32 a, f32x4 b) { return {CMPLEF32x4(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x4  operator>=(f32 a, f32x4 b) { return {CMPGEF32x4(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x4  operator==(f32 a, f32x4 b) { return {CMPEQF32x4(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x4  operator!=(f32 a, f32x4 b) { return {CMPNEF32x4(_mm_set1_ps(a), b.m.ps)}; }
	FORCEINLINE        f32x4 &operator+=(f32x4 b)        { return m.ps = _mm_add_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x4 &operator-=(f32x4 b)        { return m.ps = _mm_sub_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x4 &operator*=(f32x4 b)        { return m.ps = _mm_mul_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x4 &operator/=(f32x4 b)        { return m.ps = _mm_div_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x4 &operator+=(f32   b)        { return m.ps = _mm_add_ps(m.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x4 &operator-=(f32   b)        { return m.ps = _mm_sub_ps(m.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x4 &operator*=(f32   b)        { return m.ps = _mm_mul_ps(m.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x4 &operator/=(f32   b)        { return m.ps = _mm_div_ps(m.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x4 &operator++ ()              { return m.ps = _mm_add_ps(m.ps, _mm_set1_ps(1)), *this; }
	FORCEINLINE        f32x4 &operator-- ()              { return m.ps = _mm_sub_ps(m.ps, _mm_set1_ps(1)), *this; }
	FORCEINLINE        f32x4  operator++ (int)           { auto result = *this; m.ps = _mm_add_ps(m.ps, _mm_set1_ps(1)); return result; }
	FORCEINLINE        f32x4  operator-- (int)           { auto result = *this; m.ps = _mm_sub_ps(m.ps, _mm_set1_ps(1)); return result; }

	CVT(s32x4);
	CVT(u32x4);
	MEMFUNS_DATA(f32);
	INDEX_S(f32);
};

template <>
union f32x<8> {
#if ARCH_AVX
	M32 m;
	FORCEINLINE        f32x8  operator-()          const { return {_mm256_xor_ps(m.ps, _mm256_set1_ps(-0.0f))}; }
	FORCEINLINE	       f32x8  operator+ (f32x8 b)  const { return {_mm256_add_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x8  operator- (f32x8 b)  const { return {_mm256_sub_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x8  operator* (f32x8 b)  const { return {_mm256_mul_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       f32x8  operator/ (f32x8 b)  const { return {_mm256_div_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       b32x8  operator< (f32x8 b)  const { return {_mm256_cmp_ps(m.ps, b.m.ps, _CMP_LT_OQ )}; }
	FORCEINLINE	       b32x8  operator> (f32x8 b)  const { return {_mm256_cmp_ps(m.ps, b.m.ps, _CMP_GT_OQ )}; }
	FORCEINLINE	       b32x8  operator<=(f32x8 b)  const { return {_mm256_cmp_ps(m.ps, b.m.ps, _CMP_LE_OQ )}; }
	FORCEINLINE	       b32x8  operator>=(f32x8 b)  const { return {_mm256_cmp_ps(m.ps, b.m.ps, _CMP_GE_OQ )}; }
	FORCEINLINE	       b32x8  operator==(f32x8 b)  const { return {_mm256_cmp_ps(m.ps, b.m.ps, _CMP_EQ_OQ )}; }
	FORCEINLINE	       b32x8  operator!=(f32x8 b)  const { return {_mm256_cmp_ps(m.ps, b.m.ps, _CMP_NEQ_OQ)}; }
	FORCEINLINE	       f32x8  operator+ (f32   b)  const { return {_mm256_add_ps(m.ps, _mm256_set1_ps(b))}; }
	FORCEINLINE	       f32x8  operator- (f32   b)  const { return {_mm256_sub_ps(m.ps, _mm256_set1_ps(b))}; }
	FORCEINLINE	       f32x8  operator* (f32   b)  const { return {_mm256_mul_ps(m.ps, _mm256_set1_ps(b))}; }
	FORCEINLINE	       f32x8  operator/ (f32   b)  const { return {_mm256_div_ps(m.ps, _mm256_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator< (f32   b)  const { return {_mm256_cmp_ps(m.ps, _mm256_set1_ps(b), _CMP_LT_OQ )}; }
	FORCEINLINE	       b32x8  operator> (f32   b)  const { return {_mm256_cmp_ps(m.ps, _mm256_set1_ps(b), _CMP_GT_OQ )}; }
	FORCEINLINE	       b32x8  operator<=(f32   b)  const { return {_mm256_cmp_ps(m.ps, _mm256_set1_ps(b), _CMP_LE_OQ )}; }
	FORCEINLINE	       b32x8  operator>=(f32   b)  const { return {_mm256_cmp_ps(m.ps, _mm256_set1_ps(b), _CMP_GE_OQ )}; }
	FORCEINLINE	       b32x8  operator==(f32   b)  const { return {_mm256_cmp_ps(m.ps, _mm256_set1_ps(b), _CMP_EQ_OQ )}; }
	FORCEINLINE	       b32x8  operator!=(f32   b)  const { return {_mm256_cmp_ps(m.ps, _mm256_set1_ps(b), _CMP_NEQ_OQ)}; }
	FORCEINLINE friend f32x8  operator+ (f32 a, f32x8 b) { return {_mm256_add_ps(_mm256_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend f32x8  operator- (f32 a, f32x8 b) { return {_mm256_sub_ps(_mm256_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend f32x8  operator* (f32 a, f32x8 b) { return {_mm256_mul_ps(_mm256_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend f32x8  operator/ (f32 a, f32x8 b) { return {_mm256_div_ps(_mm256_set1_ps(a), b.m.ps)}; }
	FORCEINLINE friend b32x8  operator< (f32 a, f32x8 b) { return {_mm256_cmp_ps(_mm256_set1_ps(a), b.m.ps, _CMP_LT_OQ )}; }
	FORCEINLINE friend b32x8  operator> (f32 a, f32x8 b) { return {_mm256_cmp_ps(_mm256_set1_ps(a), b.m.ps, _CMP_GT_OQ )}; }
	FORCEINLINE friend b32x8  operator<=(f32 a, f32x8 b) { return {_mm256_cmp_ps(_mm256_set1_ps(a), b.m.ps, _CMP_LE_OQ )}; }
	FORCEINLINE friend b32x8  operator>=(f32 a, f32x8 b) { return {_mm256_cmp_ps(_mm256_set1_ps(a), b.m.ps, _CMP_GE_OQ )}; }
	FORCEINLINE friend b32x8  operator==(f32 a, f32x8 b) { return {_mm256_cmp_ps(_mm256_set1_ps(a), b.m.ps, _CMP_EQ_OQ )}; }
	FORCEINLINE friend b32x8  operator!=(f32 a, f32x8 b) { return {_mm256_cmp_ps(_mm256_set1_ps(a), b.m.ps, _CMP_NEQ_OQ)}; }
	FORCEINLINE        f32x8 &operator+=(f32x8 b)        { return m.ps = _mm256_add_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x8 &operator-=(f32x8 b)        { return m.ps = _mm256_sub_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x8 &operator*=(f32x8 b)        { return m.ps = _mm256_mul_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x8 &operator/=(f32x8 b)        { return m.ps = _mm256_div_ps(m.ps, b.m.ps), *this; }
	FORCEINLINE        f32x8 &operator+=(f32   b)        { return m.ps = _mm256_add_ps(m.ps, _mm256_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator-=(f32   b)        { return m.ps = _mm256_sub_ps(m.ps, _mm256_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator*=(f32   b)        { return m.ps = _mm256_mul_ps(m.ps, _mm256_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator/=(f32   b)        { return m.ps = _mm256_div_ps(m.ps, _mm256_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator++ ()              { return m.ps = _mm256_add_ps(m.ps, _mm256_set1_ps(1)), *this; }
	FORCEINLINE        f32x8 &operator-- ()              { return m.ps = _mm256_sub_ps(m.ps, _mm256_set1_ps(1)), *this; }
	FORCEINLINE        f32x8  operator++ (int)           { auto result = *this; m.ps = _mm256_add_ps(m.ps, _mm256_set1_ps(1)); return result; }
	FORCEINLINE        f32x8  operator-- (int)           { auto result = *this; m.ps = _mm256_sub_ps(m.ps, _mm256_set1_ps(1)); return result; }
#else
	struct {
		M16 lo, hi;
	};
	FORCEINLINE        f32x8  operator-()          const { return {_mm_xor_ps(lo.ps, _mm_set1_ps(-0.0f)), _mm_xor_ps(hi.ps, _mm_set1_ps(-0.0f))}; }
	FORCEINLINE	       f32x8  operator+ (f32x8 b)  const { return {_mm_add_ps(lo.ps, b.lo.ps), _mm_add_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       f32x8  operator- (f32x8 b)  const { return {_mm_sub_ps(lo.ps, b.lo.ps), _mm_sub_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       f32x8  operator* (f32x8 b)  const { return {_mm_mul_ps(lo.ps, b.lo.ps), _mm_mul_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       f32x8  operator/ (f32x8 b)  const { return {_mm_div_ps(lo.ps, b.lo.ps), _mm_div_ps(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       b32x8  operator< (f32x8 b)  const { return {CMPLTF32x4(lo.ps, b.lo.ps), CMPLTF32x4(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       b32x8  operator> (f32x8 b)  const { return {CMPGTF32x4(lo.ps, b.lo.ps), CMPGTF32x4(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       b32x8  operator<=(f32x8 b)  const { return {CMPLEF32x4(lo.ps, b.lo.ps), CMPLEF32x4(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       b32x8  operator>=(f32x8 b)  const { return {CMPGEF32x4(lo.ps, b.lo.ps), CMPGEF32x4(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       b32x8  operator==(f32x8 b)  const { return {CMPEQF32x4(lo.ps, b.lo.ps), CMPEQF32x4(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       b32x8  operator!=(f32x8 b)  const { return {CMPNEF32x4(lo.ps, b.lo.ps), CMPNEF32x4(hi.ps, b.hi.ps)}; }
	FORCEINLINE	       f32x8  operator+ (f32   b)  const { return {_mm_add_ps(lo.ps, _mm_set1_ps(b)), _mm_add_ps(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       f32x8  operator- (f32   b)  const { return {_mm_sub_ps(lo.ps, _mm_set1_ps(b)), _mm_sub_ps(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       f32x8  operator* (f32   b)  const { return {_mm_mul_ps(lo.ps, _mm_set1_ps(b)), _mm_mul_ps(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       f32x8  operator/ (f32   b)  const { return {_mm_div_ps(lo.ps, _mm_set1_ps(b)), _mm_div_ps(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator< (f32   b)  const { return {CMPLTF32x4(lo.ps, _mm_set1_ps(b)), CMPLTF32x4(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator> (f32   b)  const { return {CMPGTF32x4(lo.ps, _mm_set1_ps(b)), CMPGTF32x4(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator<=(f32   b)  const { return {CMPLEF32x4(lo.ps, _mm_set1_ps(b)), CMPLEF32x4(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator>=(f32   b)  const { return {CMPGEF32x4(lo.ps, _mm_set1_ps(b)), CMPGEF32x4(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator==(f32   b)  const { return {CMPEQF32x4(lo.ps, _mm_set1_ps(b)), CMPEQF32x4(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE	       b32x8  operator!=(f32   b)  const { return {CMPNEF32x4(lo.ps, _mm_set1_ps(b)), CMPNEF32x4(hi.ps, _mm_set1_ps(b))}; }
	FORCEINLINE friend f32x8  operator+ (f32 a, f32x8 b) { return {_mm_add_ps(_mm_set1_ps(a), b.lo.ps), _mm_add_ps(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend f32x8  operator- (f32 a, f32x8 b) { return {_mm_sub_ps(_mm_set1_ps(a), b.lo.ps), _mm_sub_ps(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend f32x8  operator* (f32 a, f32x8 b) { return {_mm_mul_ps(_mm_set1_ps(a), b.lo.ps), _mm_mul_ps(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend f32x8  operator/ (f32 a, f32x8 b) { return {_mm_div_ps(_mm_set1_ps(a), b.lo.ps), _mm_div_ps(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend b32x8  operator< (f32 a, f32x8 b) { return {CMPLTF32x4(_mm_set1_ps(a), b.lo.ps), CMPLTF32x4(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend b32x8  operator> (f32 a, f32x8 b) { return {CMPGTF32x4(_mm_set1_ps(a), b.lo.ps), CMPGTF32x4(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend b32x8  operator<=(f32 a, f32x8 b) { return {CMPLEF32x4(_mm_set1_ps(a), b.lo.ps), CMPLEF32x4(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend b32x8  operator>=(f32 a, f32x8 b) { return {CMPGEF32x4(_mm_set1_ps(a), b.lo.ps), CMPGEF32x4(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend b32x8  operator==(f32 a, f32x8 b) { return {CMPEQF32x4(_mm_set1_ps(a), b.lo.ps), CMPEQF32x4(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE friend b32x8  operator!=(f32 a, f32x8 b) { return {CMPNEF32x4(_mm_set1_ps(a), b.lo.ps), CMPNEF32x4(_mm_set1_ps(a), b.hi.ps)}; }
	FORCEINLINE        f32x8 &operator+=(f32x8 b)        { return lo.ps = _mm_add_ps(lo.ps, b.lo.ps), hi.ps = _mm_add_ps(hi.ps, b.hi.ps), *this; }
	FORCEINLINE        f32x8 &operator-=(f32x8 b)        { return lo.ps = _mm_sub_ps(lo.ps, b.lo.ps), hi.ps = _mm_sub_ps(hi.ps, b.hi.ps), *this; }
	FORCEINLINE        f32x8 &operator*=(f32x8 b)        { return lo.ps = _mm_mul_ps(lo.ps, b.lo.ps), hi.ps = _mm_mul_ps(hi.ps, b.hi.ps), *this; }
	FORCEINLINE        f32x8 &operator/=(f32x8 b)        { return lo.ps = _mm_div_ps(lo.ps, b.lo.ps), hi.ps = _mm_div_ps(hi.ps, b.hi.ps), *this; }
	FORCEINLINE        f32x8 &operator+=(f32   b)        { return lo.ps = _mm_add_ps(lo.ps, _mm_set1_ps(b)), hi.ps = _mm_add_ps(hi.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator-=(f32   b)        { return lo.ps = _mm_sub_ps(lo.ps, _mm_set1_ps(b)), hi.ps = _mm_sub_ps(hi.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator*=(f32   b)        { return lo.ps = _mm_mul_ps(lo.ps, _mm_set1_ps(b)), hi.ps = _mm_mul_ps(hi.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator/=(f32   b)        { return lo.ps = _mm_div_ps(lo.ps, _mm_set1_ps(b)), hi.ps = _mm_div_ps(hi.ps, _mm_set1_ps(b)), *this; }
	FORCEINLINE        f32x8 &operator++ ()              { return lo.ps = _mm_add_ps(lo.ps, _mm_set1_ps(1)), hi.ps = _mm_add_ps(hi.ps, _mm_set1_ps(1)), *this; }
	FORCEINLINE        f32x8 &operator-- ()              { return lo.ps = _mm_sub_ps(lo.ps, _mm_set1_ps(1)), hi.ps = _mm_sub_ps(hi.ps, _mm_set1_ps(1)), *this; }
	FORCEINLINE        f32x8  operator++ (int)           { auto result = *this; lo.ps = _mm_add_ps(lo.ps, _mm_set1_ps(1)); hi.ps = _mm_add_ps(hi.ps, _mm_set1_ps(1)); return result; }
	FORCEINLINE        f32x8  operator-- (int)           { auto result = *this; lo.ps = _mm_sub_ps(lo.ps, _mm_set1_ps(1)); hi.ps = _mm_sub_ps(hi.ps, _mm_set1_ps(1)); return result; }
#endif
	f32 s[8];

	CVT(s32x8);
	CVT(u32x8);
	MEMFUNS_DATA(f32);
	INDEX_S(f32);
};

template <>
union f64x<2> {
	M16 m;
	f64 s[2];

	FORCEINLINE        f64x2  operator-()          const { return {_mm_castpd_ps(_mm_xor_pd(m.pd, _mm_set1_pd(-0.0f)))}; }
	FORCEINLINE	       f64x2  operator+ (f64x2 b)  const { return {_mm_castpd_ps(_mm_add_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x2  operator- (f64x2 b)  const { return {_mm_castpd_ps(_mm_sub_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x2  operator* (f64x2 b)  const { return {_mm_castpd_ps(_mm_mul_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x2  operator/ (f64x2 b)  const { return {_mm_castpd_ps(_mm_div_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x2  operator< (f64x2 b)  const { return {_mm_castpd_ps(CMPLTF64x2(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x2  operator> (f64x2 b)  const { return {_mm_castpd_ps(CMPGTF64x2(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x2  operator<=(f64x2 b)  const { return {_mm_castpd_ps(CMPLEF64x2(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x2  operator>=(f64x2 b)  const { return {_mm_castpd_ps(CMPGEF64x2(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x2  operator==(f64x2 b)  const { return {_mm_castpd_ps(CMPEQF64x2(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x2  operator!=(f64x2 b)  const { return {_mm_castpd_ps(CMPNEF64x2(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x2  operator+ (f64   b)  const { return {_mm_castpd_ps(_mm_add_pd(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       f64x2  operator- (f64   b)  const { return {_mm_castpd_ps(_mm_sub_pd(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       f64x2  operator* (f64   b)  const { return {_mm_castpd_ps(_mm_mul_pd(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       f64x2  operator/ (f64   b)  const { return {_mm_castpd_ps(_mm_div_pd(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x2  operator< (f64   b)  const { return {_mm_castpd_ps(CMPLTF64x2(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x2  operator> (f64   b)  const { return {_mm_castpd_ps(CMPGTF64x2(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x2  operator<=(f64   b)  const { return {_mm_castpd_ps(CMPLEF64x2(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x2  operator>=(f64   b)  const { return {_mm_castpd_ps(CMPGEF64x2(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x2  operator==(f64   b)  const { return {_mm_castpd_ps(CMPEQF64x2(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x2  operator!=(f64   b)  const { return {_mm_castpd_ps(CMPNEF64x2(m.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE friend f64x2  operator+ (f64 a, f64x2 b) { return {_mm_castpd_ps(_mm_add_pd(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend f64x2  operator- (f64 a, f64x2 b) { return {_mm_castpd_ps(_mm_sub_pd(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend f64x2  operator* (f64 a, f64x2 b) { return {_mm_castpd_ps(_mm_mul_pd(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend f64x2  operator/ (f64 a, f64x2 b) { return {_mm_castpd_ps(_mm_div_pd(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x2  operator< (f64 a, f64x2 b) { return {_mm_castpd_ps(CMPLTF64x2(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x2  operator> (f64 a, f64x2 b) { return {_mm_castpd_ps(CMPGTF64x2(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x2  operator<=(f64 a, f64x2 b) { return {_mm_castpd_ps(CMPLEF64x2(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x2  operator>=(f64 a, f64x2 b) { return {_mm_castpd_ps(CMPGEF64x2(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x2  operator==(f64 a, f64x2 b) { return {_mm_castpd_ps(CMPEQF64x2(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x2  operator!=(f64 a, f64x2 b) { return {_mm_castpd_ps(CMPNEF64x2(_mm_set1_pd(a), b.m.pd))}; }
	FORCEINLINE        f64x2 &operator+=(f64x2 b)        { return m.pd = _mm_add_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x2 &operator-=(f64x2 b)        { return m.pd = _mm_sub_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x2 &operator*=(f64x2 b)        { return m.pd = _mm_mul_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x2 &operator/=(f64x2 b)        { return m.pd = _mm_div_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x2 &operator+=(f64   b)        { return m.pd = _mm_add_pd(m.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x2 &operator-=(f64   b)        { return m.pd = _mm_sub_pd(m.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x2 &operator*=(f64   b)        { return m.pd = _mm_mul_pd(m.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x2 &operator/=(f64   b)        { return m.pd = _mm_div_pd(m.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x2 &operator++ ()              { return m.pd = _mm_add_pd(m.pd, _mm_set1_pd(1)), *this; }
	FORCEINLINE        f64x2 &operator-- ()              { return m.pd = _mm_sub_pd(m.pd, _mm_set1_pd(1)), *this; }
	FORCEINLINE        f64x2  operator++ (int)           { auto result = *this; m.pd = _mm_add_pd(m.pd, _mm_set1_pd(1)); return result; }
	FORCEINLINE        f64x2  operator-- (int)           { auto result = *this; m.pd = _mm_sub_pd(m.pd, _mm_set1_pd(1)); return result; }

	//CVT(s64x2);
	//CVT(u64x2);
	MEMFUNS_DATA(f64);
	INDEX_S(f64);
};
template <>
union f64x<4> {
#if ARCH_AVX
	M32 m;
#endif
	struct {
		M16 lo, hi;
	};
	f64 s[4];

#if ARCH_AVX
	FORCEINLINE        f64x4  operator-()          const { return {_mm256_castpd_ps(_mm256_xor_pd(m.pd, _mm256_set1_pd(-0.0f)))}; }
	FORCEINLINE	       f64x4  operator+ (f64x4 b)  const { return {_mm256_castpd_ps(_mm256_add_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x4  operator- (f64x4 b)  const { return {_mm256_castpd_ps(_mm256_sub_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x4  operator* (f64x4 b)  const { return {_mm256_castpd_ps(_mm256_mul_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       f64x4  operator/ (f64x4 b)  const { return {_mm256_castpd_ps(_mm256_div_pd(m.pd, b.m.pd))}; }
	FORCEINLINE	       b64x4  operator< (f64x4 b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, b.m.pd, _CMP_LT_OQ ))}; }
	FORCEINLINE	       b64x4  operator> (f64x4 b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, b.m.pd, _CMP_GT_OQ ))}; }
	FORCEINLINE	       b64x4  operator<=(f64x4 b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, b.m.pd, _CMP_LE_OQ ))}; }
	FORCEINLINE	       b64x4  operator>=(f64x4 b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, b.m.pd, _CMP_GE_OQ ))}; }
	FORCEINLINE	       b64x4  operator==(f64x4 b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, b.m.pd, _CMP_EQ_OQ ))}; }
	FORCEINLINE	       b64x4  operator!=(f64x4 b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, b.m.pd, _CMP_NEQ_OQ))}; }
	FORCEINLINE	       f64x4  operator+ (f64   b)  const { return {_mm256_castpd_ps(_mm256_add_pd(m.pd, _mm256_set1_pd(b)))}; }
	FORCEINLINE	       f64x4  operator- (f64   b)  const { return {_mm256_castpd_ps(_mm256_sub_pd(m.pd, _mm256_set1_pd(b)))}; }
	FORCEINLINE	       f64x4  operator* (f64   b)  const { return {_mm256_castpd_ps(_mm256_mul_pd(m.pd, _mm256_set1_pd(b)))}; }
	FORCEINLINE	       f64x4  operator/ (f64   b)  const { return {_mm256_castpd_ps(_mm256_div_pd(m.pd, _mm256_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator< (f64   b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, _mm256_set1_pd(b), _CMP_LT_OQ ))}; }
	FORCEINLINE	       b64x4  operator> (f64   b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, _mm256_set1_pd(b), _CMP_GT_OQ ))}; }
	FORCEINLINE	       b64x4  operator<=(f64   b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, _mm256_set1_pd(b), _CMP_LE_OQ ))}; }
	FORCEINLINE	       b64x4  operator>=(f64   b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, _mm256_set1_pd(b), _CMP_GE_OQ ))}; }
	FORCEINLINE	       b64x4  operator==(f64   b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, _mm256_set1_pd(b), _CMP_EQ_OQ ))}; }
	FORCEINLINE	       b64x4  operator!=(f64   b)  const { return {_mm256_castpd_ps(_mm256_cmp_pd(m.pd, _mm256_set1_pd(b), _CMP_NEQ_OQ))}; }
	FORCEINLINE friend f64x4  operator+ (f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_add_pd(_mm256_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend f64x4  operator- (f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_sub_pd(_mm256_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend f64x4  operator* (f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_mul_pd(_mm256_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend f64x4  operator/ (f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_div_pd(_mm256_set1_pd(a), b.m.pd))}; }
	FORCEINLINE friend b64x4  operator< (f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_cmp_pd(_mm256_set1_pd(a), b.m.pd, _CMP_LT_OQ ))}; }
	FORCEINLINE friend b64x4  operator> (f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_cmp_pd(_mm256_set1_pd(a), b.m.pd, _CMP_GT_OQ ))}; }
	FORCEINLINE friend b64x4  operator<=(f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_cmp_pd(_mm256_set1_pd(a), b.m.pd, _CMP_LE_OQ ))}; }
	FORCEINLINE friend b64x4  operator>=(f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_cmp_pd(_mm256_set1_pd(a), b.m.pd, _CMP_GE_OQ ))}; }
	FORCEINLINE friend b64x4  operator==(f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_cmp_pd(_mm256_set1_pd(a), b.m.pd, _CMP_EQ_OQ ))}; }
	FORCEINLINE friend b64x4  operator!=(f64 a, f64x4 b) { return {_mm256_castpd_ps(_mm256_cmp_pd(_mm256_set1_pd(a), b.m.pd, _CMP_NEQ_OQ))}; }
	FORCEINLINE        f64x4 &operator+=(f64x4 b)        { return m.pd = _mm256_add_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x4 &operator-=(f64x4 b)        { return m.pd = _mm256_sub_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x4 &operator*=(f64x4 b)        { return m.pd = _mm256_mul_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x4 &operator/=(f64x4 b)        { return m.pd = _mm256_div_pd(m.pd, b.m.pd), *this; }
	FORCEINLINE        f64x4 &operator+=(f64   b)        { return m.pd = _mm256_add_pd(m.pd, _mm256_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator-=(f64   b)        { return m.pd = _mm256_sub_pd(m.pd, _mm256_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator*=(f64   b)        { return m.pd = _mm256_mul_pd(m.pd, _mm256_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator/=(f64   b)        { return m.pd = _mm256_div_pd(m.pd, _mm256_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator++ ()              { return m.pd = _mm256_add_pd(m.pd, _mm256_set1_pd(1)), *this; }
	FORCEINLINE        f64x4 &operator-- ()              { return m.pd = _mm256_sub_pd(m.pd, _mm256_set1_pd(1)), *this; }
	FORCEINLINE        f64x4  operator++ (int)           { auto result = *this; m.pd = _mm256_add_pd(m.pd, _mm256_set1_pd(1)); return result; }
	FORCEINLINE        f64x4  operator-- (int)           { auto result = *this; m.pd = _mm256_sub_pd(m.pd, _mm256_set1_pd(1)); return result; }
#else
	FORCEINLINE        f64x4  operator-()          const { return {_mm_castpd_ps(_mm_xor_pd(lo.pd, _mm_set1_pd(-0.0f))), _mm_castpd_ps(_mm_xor_pd(hi.pd, _mm_set1_pd(-0.0f)))}; }
	FORCEINLINE	       f64x4  operator+ (f64x4 b)  const { return {_mm_castpd_ps(_mm_add_pd(lo.pd, b.lo.pd)), _mm_castpd_ps(_mm_add_pd(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       f64x4  operator- (f64x4 b)  const { return {_mm_castpd_ps(_mm_sub_pd(lo.pd, b.lo.pd)), _mm_castpd_ps(_mm_sub_pd(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       f64x4  operator* (f64x4 b)  const { return {_mm_castpd_ps(_mm_mul_pd(lo.pd, b.lo.pd)), _mm_castpd_ps(_mm_mul_pd(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       f64x4  operator/ (f64x4 b)  const { return {_mm_castpd_ps(_mm_div_pd(lo.pd, b.lo.pd)), _mm_castpd_ps(_mm_div_pd(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       b64x4  operator< (f64x4 b)  const { return {_mm_castpd_ps(CMPLTF64x2(lo.pd, b.lo.pd)), _mm_castpd_ps(CMPLTF64x2(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       b64x4  operator> (f64x4 b)  const { return {_mm_castpd_ps(CMPGTF64x2(lo.pd, b.lo.pd)), _mm_castpd_ps(CMPGTF64x2(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       b64x4  operator<=(f64x4 b)  const { return {_mm_castpd_ps(CMPLEF64x2(lo.pd, b.lo.pd)), _mm_castpd_ps(CMPLEF64x2(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       b64x4  operator>=(f64x4 b)  const { return {_mm_castpd_ps(CMPGEF64x2(lo.pd, b.lo.pd)), _mm_castpd_ps(CMPGEF64x2(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       b64x4  operator==(f64x4 b)  const { return {_mm_castpd_ps(CMPEQF64x2(lo.pd, b.lo.pd)), _mm_castpd_ps(CMPEQF64x2(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       b64x4  operator!=(f64x4 b)  const { return {_mm_castpd_ps(CMPNEF64x2(lo.pd, b.lo.pd)), _mm_castpd_ps(CMPNEF64x2(hi.pd, b.hi.pd))}; }
	FORCEINLINE	       f64x4  operator+ (f64   b)  const { return {_mm_castpd_ps(_mm_add_pd(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(_mm_add_pd(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       f64x4  operator- (f64   b)  const { return {_mm_castpd_ps(_mm_sub_pd(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(_mm_sub_pd(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       f64x4  operator* (f64   b)  const { return {_mm_castpd_ps(_mm_mul_pd(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(_mm_mul_pd(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       f64x4  operator/ (f64   b)  const { return {_mm_castpd_ps(_mm_div_pd(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(_mm_div_pd(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator< (f64   b)  const { return {_mm_castpd_ps(CMPLTF64x2(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(CMPLTF64x2(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator> (f64   b)  const { return {_mm_castpd_ps(CMPGTF64x2(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(CMPGTF64x2(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator<=(f64   b)  const { return {_mm_castpd_ps(CMPLEF64x2(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(CMPLEF64x2(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator>=(f64   b)  const { return {_mm_castpd_ps(CMPGEF64x2(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(CMPGEF64x2(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator==(f64   b)  const { return {_mm_castpd_ps(CMPEQF64x2(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(CMPEQF64x2(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE	       b64x4  operator!=(f64   b)  const { return {_mm_castpd_ps(CMPNEF64x2(lo.pd, _mm_set1_pd(b))), _mm_castpd_ps(CMPNEF64x2(hi.pd, _mm_set1_pd(b)))}; }
	FORCEINLINE friend f64x4  operator+ (f64 a, f64x4 b) { return {_mm_castpd_ps(_mm_add_pd(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(_mm_add_pd(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend f64x4  operator- (f64 a, f64x4 b) { return {_mm_castpd_ps(_mm_sub_pd(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(_mm_sub_pd(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend f64x4  operator* (f64 a, f64x4 b) { return {_mm_castpd_ps(_mm_mul_pd(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(_mm_mul_pd(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend f64x4  operator/ (f64 a, f64x4 b) { return {_mm_castpd_ps(_mm_div_pd(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(_mm_div_pd(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend b64x4  operator< (f64 a, f64x4 b) { return {_mm_castpd_ps(CMPLTF64x2(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(CMPLTF64x2(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend b64x4  operator> (f64 a, f64x4 b) { return {_mm_castpd_ps(CMPGTF64x2(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(CMPGTF64x2(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend b64x4  operator<=(f64 a, f64x4 b) { return {_mm_castpd_ps(CMPLEF64x2(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(CMPLEF64x2(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend b64x4  operator>=(f64 a, f64x4 b) { return {_mm_castpd_ps(CMPGEF64x2(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(CMPGEF64x2(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend b64x4  operator==(f64 a, f64x4 b) { return {_mm_castpd_ps(CMPEQF64x2(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(CMPEQF64x2(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE friend b64x4  operator!=(f64 a, f64x4 b) { return {_mm_castpd_ps(CMPNEF64x2(_mm_set1_pd(a), b.lo.pd)), _mm_castpd_ps(CMPNEF64x2(_mm_set1_pd(a), b.hi.pd))}; }
	FORCEINLINE        f64x4 &operator+=(f64x4 b)        { return lo.pd = _mm_add_pd(lo.pd, b.lo.pd), hi.pd = _mm_add_pd(hi.pd, b.hi.pd), *this; }
	FORCEINLINE        f64x4 &operator-=(f64x4 b)        { return lo.pd = _mm_sub_pd(lo.pd, b.lo.pd), hi.pd = _mm_sub_pd(hi.pd, b.hi.pd), *this; }
	FORCEINLINE        f64x4 &operator*=(f64x4 b)        { return lo.pd = _mm_mul_pd(lo.pd, b.lo.pd), hi.pd = _mm_mul_pd(hi.pd, b.hi.pd), *this; }
	FORCEINLINE        f64x4 &operator/=(f64x4 b)        { return lo.pd = _mm_div_pd(lo.pd, b.lo.pd), hi.pd = _mm_div_pd(hi.pd, b.hi.pd), *this; }
	FORCEINLINE        f64x4 &operator+=(f64   b)        { return lo.pd = _mm_add_pd(lo.pd, _mm_set1_pd(b)), hi.pd = _mm_add_pd(hi.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator-=(f64   b)        { return lo.pd = _mm_sub_pd(lo.pd, _mm_set1_pd(b)), hi.pd = _mm_sub_pd(hi.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator*=(f64   b)        { return lo.pd = _mm_mul_pd(lo.pd, _mm_set1_pd(b)), hi.pd = _mm_mul_pd(hi.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator/=(f64   b)        { return lo.pd = _mm_div_pd(lo.pd, _mm_set1_pd(b)), hi.pd = _mm_div_pd(hi.pd, _mm_set1_pd(b)), *this; }
	FORCEINLINE        f64x4 &operator++ ()              { return lo.pd = _mm_add_pd(lo.pd, _mm_set1_pd(1)), hi.pd = _mm_add_pd(hi.pd, _mm_set1_pd(1)), *this; }
	FORCEINLINE        f64x4 &operator-- ()              { return lo.pd = _mm_sub_pd(lo.pd, _mm_set1_pd(1)), hi.pd = _mm_sub_pd(hi.pd, _mm_set1_pd(1)), *this; }
	FORCEINLINE        f64x4  operator++ (int)           { auto result = *this; lo.pd = _mm_add_pd(lo.pd, _mm_set1_pd(1)); hi.pd = _mm_add_pd(hi.pd, _mm_set1_pd(1)); return result; }
	FORCEINLINE        f64x4  operator-- (int)           { auto result = *this; lo.pd = _mm_sub_pd(lo.pd, _mm_set1_pd(1)); hi.pd = _mm_sub_pd(hi.pd, _mm_set1_pd(1)); return result; }
#endif

	CVT(u32x4);
	//CVT(s64x4);
	//CVT(u64x4);
	MEMFUNS_DATA(f64);
	INDEX_S(f64);
};

template<>
union s32x<1> {
	s32 m;
	s32 s[1];
	
	FORCEINLINE        s32x1  operator-()          const { return {-m}; }
	FORCEINLINE	       s32x1  operator+ (s32x1 b)  const { return {m +  b.m}; }
	FORCEINLINE	       s32x1  operator- (s32x1 b)  const { return {m -  b.m}; }
	FORCEINLINE	       s32x1  operator* (s32x1 b)  const { return {m *  b.m}; }
	FORCEINLINE	       s32x1  operator/ (s32x1 b)  const { return {m /  b.m}; }
	FORCEINLINE	       s32x1  operator% (s32x1 b)  const { return {m %  b.m}; }
	FORCEINLINE	       s32x1  operator^ (s32x1 b)  const { return {m ^  b.m}; }
	FORCEINLINE	       s32x1  operator& (s32x1 b)  const { return {m &  b.m}; }
	FORCEINLINE	       s32x1  operator| (s32x1 b)  const { return {m |  b.m}; }
	FORCEINLINE	       b32x1  operator< (s32x1 b)  const { return {m <  b.m}; }
	FORCEINLINE	       b32x1  operator> (s32x1 b)  const { return {m >  b.m}; }
	FORCEINLINE	       b32x1  operator<=(s32x1 b)  const { return {m <= b.m}; }
	FORCEINLINE	       b32x1  operator>=(s32x1 b)  const { return {m >= b.m}; }
	FORCEINLINE	       b32x1  operator==(s32x1 b)  const { return {m == b.m}; }
	FORCEINLINE	       b32x1  operator!=(s32x1 b)  const { return {m != b.m}; }
	FORCEINLINE	       s32x1  operator+ (s32   b)  const { return {m +  b  }; }
	FORCEINLINE	       s32x1  operator- (s32   b)  const { return {m -  b  }; }
	FORCEINLINE	       s32x1  operator* (s32   b)  const { return {m *  b  }; }
	FORCEINLINE	       s32x1  operator/ (s32   b)  const { return {m /  b  }; }
	FORCEINLINE	       s32x1  operator% (s32   b)  const { return {m %  b  }; }
	FORCEINLINE	       s32x1  operator^ (s32   b)  const { return {m ^  b  }; }
	FORCEINLINE	       s32x1  operator& (s32   b)  const { return {m &  b  }; }
	FORCEINLINE	       s32x1  operator| (s32   b)  const { return {m |  b  }; }
	FORCEINLINE	       b32x1  operator< (s32   b)  const { return {m <  b  }; }
	FORCEINLINE	       b32x1  operator> (s32   b)  const { return {m >  b  }; }
	FORCEINLINE	       b32x1  operator<=(s32   b)  const { return {m <= b  }; }
	FORCEINLINE	       b32x1  operator>=(s32   b)  const { return {m >= b  }; }
	FORCEINLINE	       b32x1  operator==(s32   b)  const { return {m == b  }; }
	FORCEINLINE	       b32x1  operator!=(s32   b)  const { return {m != b  }; }
	FORCEINLINE friend s32x1  operator+ (s32 a, s32x1 b) { return {a +  b.m}; }
	FORCEINLINE friend s32x1  operator- (s32 a, s32x1 b) { return {a -  b.m}; }
	FORCEINLINE friend s32x1  operator* (s32 a, s32x1 b) { return {a *  b.m}; }
	FORCEINLINE friend s32x1  operator/ (s32 a, s32x1 b) { return {a /  b.m}; }
	FORCEINLINE	friend s32x1  operator% (s32 a, s32x1 b) { return {a %  b.m}; }
	FORCEINLINE	friend s32x1  operator^ (s32 a, s32x1 b) { return {a ^  b.m}; }
	FORCEINLINE	friend s32x1  operator& (s32 a, s32x1 b) { return {a &  b.m}; }
	FORCEINLINE	friend s32x1  operator| (s32 a, s32x1 b) { return {a |  b.m}; }
	FORCEINLINE friend b32x1  operator< (s32 a, s32x1 b) { return {a <  b.m}; }
	FORCEINLINE friend b32x1  operator> (s32 a, s32x1 b) { return {a >  b.m}; }
	FORCEINLINE friend b32x1  operator<=(s32 a, s32x1 b) { return {a <= b.m}; }
	FORCEINLINE friend b32x1  operator>=(s32 a, s32x1 b) { return {a >= b.m}; }
	FORCEINLINE friend b32x1  operator==(s32 a, s32x1 b) { return {a == b.m}; }
	FORCEINLINE friend b32x1  operator!=(s32 a, s32x1 b) { return {a != b.m}; }
	FORCEINLINE        s32x1 &operator+=(s32x1 b)        { return m += b.m, *this; }
	FORCEINLINE        s32x1 &operator-=(s32x1 b)        { return m -= b.m, *this; }
	FORCEINLINE        s32x1 &operator*=(s32x1 b)        { return m *= b.m, *this; }
	FORCEINLINE        s32x1 &operator/=(s32x1 b)        { return m /= b.m, *this; }
	FORCEINLINE	       s32x1 &operator%=(s32x1 b)        { return m %= b.m, *this; }
	FORCEINLINE	       s32x1 &operator^=(s32x1 b)        { return m ^= b.m, *this; }
	FORCEINLINE	       s32x1 &operator&=(s32x1 b)        { return m &= b.m, *this; }
	FORCEINLINE	       s32x1 &operator|=(s32x1 b)        { return m |= b.m, *this; }
	FORCEINLINE        s32x1 &operator+=(s32   b)        { return m += b  , *this; }
	FORCEINLINE        s32x1 &operator-=(s32   b)        { return m -= b  , *this; }
	FORCEINLINE        s32x1 &operator*=(s32   b)        { return m *= b  , *this; }
	FORCEINLINE        s32x1 &operator/=(s32   b)        { return m /= b  , *this; }
	FORCEINLINE	       s32x1 &operator%=(s32   b)        { return m %= b  , *this; }
	FORCEINLINE	       s32x1 &operator^=(s32   b)        { return m ^= b  , *this; }
	FORCEINLINE	       s32x1 &operator&=(s32   b)        { return m &= b  , *this; }
	FORCEINLINE	       s32x1 &operator|=(s32   b)        { return m |= b  , *this; }
	FORCEINLINE        s32x1 &operator++ ()              { return ++m, *this; }
	FORCEINLINE        s32x1 &operator-- ()              { return --m, *this; }
	FORCEINLINE        s32x1  operator++ (int)           { return {m++}; }
	FORCEINLINE        s32x1  operator-- (int)           { return {m--}; }

	CVT(f32x1);
	CVT(u32x1);
	MEMFUNS_DATA(s32);
	INDEX_S(s32);
};

template<>
union s32x<4> {
	M16 m;
	s32 s[4];

	FORCEINLINE        s32x4  operator-()          const { return {_mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), m.pi))}; }
	FORCEINLINE        s32x4  operator~()          const { return {_mm_not_ps(m.ps)}; }
	FORCEINLINE	       s32x4  operator+ (s32x4 b)  const { return {_mm_castpi_ps(_mm_add_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator- (s32x4 b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator* (s32x4 b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator/ (s32x4 b)  const { return {_mm_castpi_ps(_mm_div_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator% (s32x4 b)  const { return {_mm_castpi_ps(_mm_rem_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator^ (s32x4 b)  const { return {_mm_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       s32x4  operator& (s32x4 b)  const { return {_mm_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       s32x4  operator| (s32x4 b)  const { return { _mm_or_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       s32x4  operator<<(s32x4 b)  const { return {_mm_castpi_ps(_mm_sllv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator>>(s32x4 b)  const { return {_mm_castpi_ps(_mm_srlv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator< (s32x4 b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator> (s32x4 b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator<=(s32x4 b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator>=(s32x4 b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator==(s32x4 b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator!=(s32x4 b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x4  operator+ (s32   b)  const { return {_mm_castpi_ps(_mm_add_epi32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator- (s32   b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator* (s32   b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator/ (s32   b)  const { return {_mm_castpi_ps(_mm_div_epi32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator% (s32   b)  const { return {_mm_castpi_ps(_mm_rem_epi32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator^ (s32   b)  const { return {_mm_xor_ps(m.ps, _mm_castpi_ps(_mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator& (s32   b)  const { return {_mm_and_ps(m.ps, _mm_castpi_ps(_mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator| (s32   b)  const { return { _mm_or_ps(m.ps, _mm_castpi_ps(_mm_set1_epi32(b)))}; }
	FORCEINLINE	       s32x4  operator<<(s32   b)  const { return {_mm_castpi_ps(_mm_slli_epi32(m.pi, b))}; }
	FORCEINLINE	       s32x4  operator>>(s32   b)  const { return {_mm_castpi_ps(_mm_srli_epi32(m.pi, b))}; }
	FORCEINLINE	       b32x4  operator< (s32   b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator> (s32   b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator<=(s32   b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator>=(s32   b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator==(s32   b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator!=(s32   b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE friend s32x4  operator+ (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend s32x4  operator- (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend s32x4  operator* (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend s32x4  operator/ (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_div_epi32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend s32x4  operator% (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_rem_epi32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend s32x4  operator^ (s32 a, s32x4 b) { return {_mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend s32x4  operator& (s32 a, s32x4 b) { return {_mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend s32x4  operator| (s32 a, s32x4 b) { return { _mm_or_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend s32x4  operator<<(s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_slliv_epi32(a, b.m.pi))}; }
	FORCEINLINE	friend s32x4  operator>>(s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_srliv_epi32(a, b.m.pi))}; }
	FORCEINLINE friend b32x4  operator< (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator> (s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator<=(s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator>=(s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator==(s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator!=(s32 a, s32x4 b) { return {_mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE        s32x4 &operator+= (s32x4 b)       { return m.pi = _mm_add_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x4 &operator-= (s32x4 b)       { return m.pi = _mm_sub_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x4 &operator*= (s32x4 b)       { return m.pi = _mm_mullo_epi32(m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x4 &operator/= (s32x4 b)       { return m.pi = _mm_div_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       s32x4 &operator%= (s32x4 b)       { return m.pi = _mm_rem_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       s32x4 &operator^= (s32x4 b)       { return m.ps = _mm_xor_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       s32x4 &operator&= (s32x4 b)       { return m.ps = _mm_and_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       s32x4 &operator|= (s32x4 b)       { return m.ps = _mm_or_ps      (m.ps, b.m.ps), *this; }
	FORCEINLINE	       s32x4 &operator<<=(s32x4 b)       { return m.pi = _mm_sllv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE	       s32x4 &operator>>=(s32x4 b)       { return m.pi = _mm_srlv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x4 &operator+= (s32   b)       { return m.pi = _mm_add_epi32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE        s32x4 &operator-= (s32   b)       { return m.pi = _mm_sub_epi32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE        s32x4 &operator*= (s32   b)       { return m.pi = _mm_mullo_epi32(m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE        s32x4 &operator/= (s32   b)       { return m.pi = _mm_div_epi32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE	       s32x4 &operator%= (s32   b)       { return m.pi = _mm_rem_epi32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE	       s32x4 &operator^= (s32   b)       { return m.ps = _mm_xor_ps     (m.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x4 &operator&= (s32   b)       { return m.ps = _mm_and_ps     (m.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x4 &operator|= (s32   b)       { return m.ps = _mm_or_ps      (m.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x4 &operator<<=(s32   b)       { return m.pi = _mm_slli_epi32(m.pi, b), *this; }
	FORCEINLINE	       s32x4 &operator>>=(s32   b)       { return m.pi = _mm_srli_epi32(m.pi, b), *this; }
	FORCEINLINE        s32x4 &operator++ ()              { return m.pi = _mm_add_epi32  (m.pi, _mm_set1_epi32(1)), *this; }
	FORCEINLINE        s32x4 &operator-- ()              { return m.pi = _mm_sub_epi32  (m.pi, _mm_set1_epi32(1)), *this; }
	FORCEINLINE        s32x4  operator++ (int)           { auto result = *this; m.pi = _mm_add_epi32  (m.pi, _mm_set1_epi32(1)); return result; }
	FORCEINLINE        s32x4  operator-- (int)           { auto result = *this; m.pi = _mm_sub_epi32  (m.pi, _mm_set1_epi32(1)); return result; }

	CVT(f32x4);
	CVT(u32x4);
	MEMFUNS_DATA(s32);
	INDEX_S(s32);
};

template<>
union s32x<8> {
#if ARCH_AVX
	M32 m;
#endif
	struct {
		M16 lo, hi;
	};
#if ARCH_AVX2
	FORCEINLINE        s32x8  operator-()          const { return {_mm256_castpi_ps(_mm256_sub_epi32(_mm256_setzero_pi(), m.pi))}; }
	FORCEINLINE        s32x8  operator~()          const { return {_mm256_not_ps(m.ps)}; }
	FORCEINLINE	       s32x8  operator+ (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_add_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator- (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_sub_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator* (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_mullo_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator/ (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_div_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator% (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_rem_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator^ (s32x8 b)  const { return {_mm256_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       s32x8  operator& (s32x8 b)  const { return {_mm256_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       s32x8  operator| (s32x8 b)  const { return {_mm256_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE	       s32x8  operator<<(s32x8 b)  const { return {_mm256_castpi_ps(_mm256_sllv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator>>(s32x8 b)  const { return {_mm256_castpi_ps(_mm256_srav_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator< (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmplt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator> (s32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpgt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator<=(s32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmple_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator>=(s32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpge_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator==(s32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpeq_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator!=(s32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpne_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       s32x8  operator+ (s32   b)  const { return {_mm256_castpi_ps(_mm256_add_epi32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator- (s32   b)  const { return {_mm256_castpi_ps(_mm256_sub_epi32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator* (s32   b)  const { return {_mm256_castpi_ps(_mm256_mullo_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator/ (s32   b)  const { return {_mm256_castpi_ps(_mm256_div_epi32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator% (s32   b)  const { return {_mm256_castpi_ps(_mm256_rem_epi32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator^ (s32   b)  const { return {_mm256_xor_ps(m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator& (s32   b)  const { return {_mm256_and_ps(m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator| (s32   b)  const { return {_mm256_or_ps (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b)))}; }
	FORCEINLINE	       s32x8  operator<<(s32   b)  const { return {_mm256_castpi_ps(_mm256_slli_epi32(m.pi, b))}; }
	FORCEINLINE	       s32x8  operator>>(s32   b)  const { return {_mm256_castpi_ps(_mm256_srai_epi32(m.pi, b))}; }
	FORCEINLINE	       b32x8  operator< (s32   b)  const { return {_mm256_castpi_ps(_mm256_cmplt_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator> (s32   b)  const { return {_mm256_castpi_ps(_mm256_cmpgt_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator<=(s32   b)  const { return {_mm256_castpi_ps(_mm256_cmple_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator>=(s32   b)  const { return {_mm256_castpi_ps(_mm256_cmpge_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator==(s32   b)  const { return {_mm256_castpi_ps(_mm256_cmpeq_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator!=(s32   b)  const { return {_mm256_castpi_ps(_mm256_cmpne_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE friend s32x8  operator+ (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_add_epi32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend s32x8  operator- (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_sub_epi32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend s32x8  operator* (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_mullo_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend s32x8  operator/ (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_div_epi32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend s32x8  operator% (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_rem_epi32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend s32x8  operator^ (s32 a, s32x8 b) { return {_mm256_xor_ps(_mm256_castpi_ps(_mm256_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend s32x8  operator& (s32 a, s32x8 b) { return {_mm256_and_ps(_mm256_castpi_ps(_mm256_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend s32x8  operator| (s32 a, s32x8 b) { return {_mm256_or_ps (_mm256_castpi_ps(_mm256_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend s32x8  operator<<(s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_slliv_epi32(a, b.m.pi))}; }
	FORCEINLINE	friend s32x8  operator>>(s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_sraiv_epi32(a, b.m.pi))}; }
	FORCEINLINE friend b32x8  operator< (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_cmplt_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator> (s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_cmpgt_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator<=(s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_cmple_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator>=(s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_cmpge_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator==(s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_cmpeq_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator!=(s32 a, s32x8 b) { return {_mm256_castpi_ps(_mm256_cmpne_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE        s32x8 &operator+= (s32x8 b)       { return m.pi = _mm256_add_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x8 &operator-= (s32x8 b)       { return m.pi = _mm256_sub_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x8 &operator*= (s32x8 b)       { return m.pi = _mm256_mullo_epi32(m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x8 &operator/= (s32x8 b)       { return m.pi = _mm256_div_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       s32x8 &operator%= (s32x8 b)       { return m.pi = _mm256_rem_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       s32x8 &operator^= (s32x8 b)       { return m.ps = _mm256_xor_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       s32x8 &operator&= (s32x8 b)       { return m.ps = _mm256_and_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       s32x8 &operator|= (s32x8 b)       { return m.ps = _mm256_or_ps      (m.ps, b.m.ps), *this; }
	FORCEINLINE	       s32x8 &operator<<=(s32x8 b)       { return m.pi = _mm256_sllv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE	       s32x8 &operator>>=(s32x8 b)       { return m.pi = _mm256_srav_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE        s32x8 &operator+= (s32   b)       { return m.pi = _mm256_add_epi32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE        s32x8 &operator-= (s32   b)       { return m.pi = _mm256_sub_epi32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE        s32x8 &operator*= (s32   b)       { return m.pi = _mm256_mullo_epi32(m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE        s32x8 &operator/= (s32   b)       { return m.pi = _mm256_div_epi32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE	       s32x8 &operator%= (s32   b)       { return m.pi = _mm256_rem_epi32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE	       s32x8 &operator^= (s32   b)       { return m.ps = _mm256_xor_ps     (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator&= (s32   b)       { return m.ps = _mm256_and_ps     (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator|= (s32   b)       { return m.ps = _mm256_or_ps      (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator<<=(s32   b)       { return m.pi = _mm256_slli_epi32(m.pi, b), *this; }
	FORCEINLINE	       s32x8 &operator>>=(s32   b)       { return m.pi = _mm256_srai_epi32(m.pi, b), *this; }
	FORCEINLINE        s32x8 &operator++ ()              { return m.pi = _mm256_add_epi32  (m.pi, _mm256_set1_epi32(1)), *this; }
	FORCEINLINE        s32x8 &operator-- ()              { return m.pi = _mm256_sub_epi32  (m.pi, _mm256_set1_epi32(1)), *this; }
	FORCEINLINE        s32x8  operator++ (int)           { auto result = *this; m.pi = _mm256_add_epi32  (m.pi, _mm256_set1_epi32(1)); return result; }
	FORCEINLINE        s32x8  operator-- (int)           { auto result = *this; m.pi = _mm256_sub_epi32  (m.pi, _mm256_set1_epi32(1)); return result; }
#elif ARCH_AVX
	FORCEINLINE        s32x8  operator-()          const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), lo.pi))        , _mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), hi.pi)))        }; }
	FORCEINLINE        s32x8  operator~()          const { return {_mm256_setr_m128(_mm_not_ps(lo.ps), _mm_not_ps(hi.ps))}; }
	FORCEINLINE	       s32x8  operator+ (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_add_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_add_epi32  (hi.pi, b.hi.pi)))               }; }
	FORCEINLINE	       s32x8  operator- (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, b.hi.pi)))               }; }
	FORCEINLINE	       s32x8  operator* (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_mullo_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, b.hi.pi)))               }; }
	FORCEINLINE	       s32x8  operator/ (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_div_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_div_epi32  (hi.pi, b.hi.pi)))               }; }
	FORCEINLINE	       s32x8  operator% (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_rem_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_rem_epi32  (hi.pi, b.hi.pi)))               }; }
	FORCEINLINE	       s32x8  operator^ (s32x8 b)  const { return {_mm256_setr_m128(_mm_xor_ps(lo.ps, b.lo.ps)                                   , _mm_xor_ps(hi.ps, b.hi.ps))                                   }; }
	FORCEINLINE	       s32x8  operator& (s32x8 b)  const { return {_mm256_setr_m128(_mm_and_ps(lo.ps, b.lo.ps)                                   , _mm_and_ps(hi.ps, b.hi.ps))                                   }; }
	FORCEINLINE	       s32x8  operator| (s32x8 b)  const { return {_mm256_setr_m128(_mm_or_ps (lo.ps, b.lo.ps)                                   , _mm_or_ps (hi.ps, b.hi.ps))                                   }; }
	FORCEINLINE	       s32x8  operator<<(s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sllv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_sllv_epi32(hi.pi, b.hi.pi)))                }; }
	FORCEINLINE	       s32x8  operator>>(s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_srlv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_srlv_epi32(hi.pi, b.hi.pi)))                }; }
	FORCEINLINE	       b32x8  operator< (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, b.hi.pi)))            }; }
	FORCEINLINE	       b32x8  operator> (s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, b.hi.pi)))            }; }
	FORCEINLINE	       b32x8  operator<=(s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmple_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmple_epi32(hi.pi, b.hi.pi)))            }; }
	FORCEINLINE	       b32x8  operator>=(s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, b.hi.pi)))            }; }
	FORCEINLINE	       b32x8  operator==(s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, b.hi.pi)))            }; }
	FORCEINLINE	       b32x8  operator!=(s32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, b.hi.pi)))            }; }
	FORCEINLINE	       s32x8  operator+ (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_add_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_add_epi32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       s32x8  operator- (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       s32x8  operator* (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_mullo_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       s32x8  operator/ (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_div_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_div_epi32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       s32x8  operator% (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_rem_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_rem_epi32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       s32x8  operator^ (s32   b)  const { return {_mm256_setr_m128(_mm_xor_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_xor_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          )}; }
	FORCEINLINE	       s32x8  operator& (s32   b)  const { return {_mm256_setr_m128(_mm_and_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_and_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          )}; }
	FORCEINLINE	       s32x8  operator| (s32   b)  const { return {_mm256_setr_m128(_mm_or_ps (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_or_ps (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          )}; }
	FORCEINLINE	       s32x8  operator<<(s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_slli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_slli_epi32(hi.pi, b))                      )}; }
	FORCEINLINE	       s32x8  operator>>(s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_srli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_srli_epi32(hi.pi, b))                      )}; }
	FORCEINLINE	       b32x8  operator< (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, _mm_set1_epi32(b))))}; }
	FORCEINLINE	       b32x8  operator> (s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, _mm_set1_epi32(b))))}; }
	FORCEINLINE	       b32x8  operator<=(s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmple_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmple_epi32(hi.pi, _mm_set1_epi32(b))))}; }
	FORCEINLINE	       b32x8  operator>=(s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, _mm_set1_epi32(b))))}; }
	FORCEINLINE	       b32x8  operator==(s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, _mm_set1_epi32(b))))}; }
	FORCEINLINE	       b32x8  operator!=(s32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, _mm_set1_epi32(b))))}; }
	FORCEINLINE friend s32x8  operator+ (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.hi.pi))  )}; }
	FORCEINLINE friend s32x8  operator- (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.hi.pi))  )}; }
	FORCEINLINE friend s32x8  operator* (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.hi.pi))  )}; }
	FORCEINLINE friend s32x8  operator/ (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_div_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_div_epi32  (_mm_set1_epi32(a), b.hi.pi))  )}; }
	FORCEINLINE	friend s32x8  operator% (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_rem_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_rem_epi32  (_mm_set1_epi32(a), b.hi.pi))  )}; }
	FORCEINLINE	friend s32x8  operator^ (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)       )}; }
	FORCEINLINE	friend s32x8  operator& (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)       )}; }
	FORCEINLINE	friend s32x8  operator| (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)       )}; }
	FORCEINLINE	friend s32x8  operator<<(s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_slliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_slliv_epi32(a, b.hi.pi))                  )}; }
	FORCEINLINE	friend s32x8  operator>>(s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_srliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_srliv_epi32(a, b.hi.pi))                  )}; }
	FORCEINLINE friend b32x8  operator< (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.hi.pi)))}; }
	FORCEINLINE friend b32x8  operator> (s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.hi.pi)))}; }
	FORCEINLINE friend b32x8  operator<=(s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.hi.pi)))}; }
	FORCEINLINE friend b32x8  operator>=(s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.hi.pi)))}; }
	FORCEINLINE friend b32x8  operator==(s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.hi.pi)))}; }
	FORCEINLINE friend b32x8  operator!=(s32 a, s32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.hi.pi)))}; }
	FORCEINLINE        s32x8 &operator+= (s32x8 b)       { return lo.pi = _mm_add_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_add_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator-= (s32x8 b)       { return lo.pi = _mm_sub_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_sub_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator*= (s32x8 b)       { return lo.pi = _mm_mullo_epi32(lo.pi, b.lo.pi)                         , hi.pi = _mm_mullo_epi32(hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator/= (s32x8 b)       { return lo.pi = _mm_div_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_div_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       s32x8 &operator%= (s32x8 b)       { return lo.pi = _mm_rem_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_rem_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       s32x8 &operator^= (s32x8 b)       { return lo.ps = _mm_xor_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_xor_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       s32x8 &operator&= (s32x8 b)       { return lo.ps = _mm_and_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_and_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       s32x8 &operator|= (s32x8 b)       { return lo.ps = _mm_or_ps      (lo.ps, b.lo.ps)                         , hi.ps = _mm_or_ps      (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       s32x8 &operator<<=(s32x8 b)       { return lo.pi = _mm_sllv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_sllv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       s32x8 &operator>>=(s32x8 b)       { return lo.pi = _mm_srlv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_srlv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator+= (s32   b)       { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        s32x8 &operator-= (s32   b)       { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        s32x8 &operator*= (s32   b)       { return lo.pi = _mm_mullo_epi32(lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_mullo_epi32(hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        s32x8 &operator/= (s32   b)       { return lo.pi = _mm_div_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_div_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       s32x8 &operator%= (s32   b)       { return lo.pi = _mm_rem_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_rem_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       s32x8 &operator^= (s32   b)       { return lo.ps = _mm_xor_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_xor_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator&= (s32   b)       { return lo.ps = _mm_and_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_and_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator|= (s32   b)       { return lo.ps = _mm_or_ps      (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_or_ps      (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator<<=(s32   b)       { return lo.pi = _mm_slli_epi32 (lo.pi, b)                               , hi.pi = _mm_slli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE	       s32x8 &operator>>=(s32   b)       { return lo.pi = _mm_srli_epi32 (lo.pi, b)                               , hi.pi = _mm_srli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE        s32x8 &operator++ ()              { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        s32x8 &operator-- ()              { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        s32x8  operator++ (int)           { auto result = *this; lo.pi = _mm_add_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_add_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
	FORCEINLINE        s32x8  operator-- (int)           { auto result = *this; lo.pi = _mm_sub_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_sub_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
#else
	FORCEINLINE        s32x8  operator-()          const { return {_mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), lo.pi))        , _mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), hi.pi))        }; }
	FORCEINLINE        s32x8  operator~()          const { return {_mm_not_ps(lo.ps), _mm_not_ps(hi.ps)}; }
	FORCEINLINE	       s32x8  operator+ (s32x8 b)  const { return {_mm_castpi_ps(_mm_add_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_add_epi32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       s32x8  operator- (s32x8 b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       s32x8  operator* (s32x8 b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       s32x8  operator/ (s32x8 b)  const { return {_mm_castpi_ps(_mm_div_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_div_epi32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       s32x8  operator% (s32x8 b)  const { return {_mm_castpi_ps(_mm_rem_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_rem_epi32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       s32x8  operator^ (s32x8 b)  const { return {_mm_xor_ps(lo.ps, b.lo.ps)                                   , _mm_xor_ps(hi.ps, b.hi.ps)                                   }; }
	FORCEINLINE	       s32x8  operator& (s32x8 b)  const { return {_mm_and_ps(lo.ps, b.lo.ps)                                   , _mm_and_ps(hi.ps, b.hi.ps)                                   }; }
	FORCEINLINE	       s32x8  operator| (s32x8 b)  const { return {_mm_or_ps (lo.ps, b.lo.ps)                                   , _mm_or_ps (hi.ps, b.hi.ps)                                   }; }
	FORCEINLINE	       s32x8  operator<<(s32x8 b)  const { return {_mm_castpi_ps(_mm_sllv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_sllv_epi32(hi.pi, b.hi.pi))                }; }
	FORCEINLINE	       s32x8  operator>>(s32x8 b)  const { return {_mm_castpi_ps(_mm_srlv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_srlv_epi32(hi.pi, b.hi.pi))                }; }
	FORCEINLINE	       b32x8  operator< (s32x8 b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator> (s32x8 b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator<=(s32x8 b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmple_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator>=(s32x8 b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator==(s32x8 b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator!=(s32x8 b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       s32x8  operator+ (s32   b)  const { return {_mm_castpi_ps(_mm_add_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_add_epi32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       s32x8  operator- (s32   b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       s32x8  operator* (s32   b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       s32x8  operator/ (s32   b)  const { return {_mm_castpi_ps(_mm_div_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_div_epi32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       s32x8  operator% (s32   b)  const { return {_mm_castpi_ps(_mm_rem_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_rem_epi32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       s32x8  operator^ (s32   b)  const { return {_mm_xor_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_xor_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          }; }
	FORCEINLINE	       s32x8  operator& (s32   b)  const { return {_mm_and_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_and_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          }; }
	FORCEINLINE	       s32x8  operator| (s32   b)  const { return {_mm_or_ps (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_or_ps (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          }; }
	FORCEINLINE	       s32x8  operator<<(s32   b)  const { return {_mm_castpi_ps(_mm_slli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_slli_epi32(hi.pi, b))                      }; }
	FORCEINLINE	       s32x8  operator>>(s32   b)  const { return {_mm_castpi_ps(_mm_srli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_srli_epi32(hi.pi, b))                      }; }
	FORCEINLINE	       b32x8  operator< (s32   b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator> (s32   b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator<=(s32   b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmple_epi32(hi.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator>=(s32   b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator==(s32   b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator!=(s32   b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, _mm_set1_epi32(b))), _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE friend s32x8  operator+ (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend s32x8  operator- (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend s32x8  operator* (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend s32x8  operator/ (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_div_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_div_epi32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE	friend s32x8  operator% (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_rem_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_rem_epi32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE	friend s32x8  operator^ (s32 a, s32x8 b) { return {_mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        }; }
	FORCEINLINE	friend s32x8  operator& (s32 a, s32x8 b) { return {_mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        }; }
	FORCEINLINE	friend s32x8  operator| (s32 a, s32x8 b) { return {_mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        }; }
	FORCEINLINE	friend s32x8  operator<<(s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_slliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_slliv_epi32(a, b.hi.pi))                   }; }
	FORCEINLINE	friend s32x8  operator>>(s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_srliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_srliv_epi32(a, b.hi.pi))                   }; }
	FORCEINLINE friend b32x8  operator< (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator> (s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator<=(s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator>=(s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator==(s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator!=(s32 a, s32x8 b) { return {_mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE        s32x8 &operator+= (s32x8 b)       { return lo.pi = _mm_add_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_add_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator-= (s32x8 b)       { return lo.pi = _mm_sub_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_sub_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator*= (s32x8 b)       { return lo.pi = _mm_mullo_epi32(lo.pi, b.lo.pi)                         , hi.pi = _mm_mullo_epi32(hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator/= (s32x8 b)       { return lo.pi = _mm_div_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_div_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       s32x8 &operator%= (s32x8 b)       { return lo.pi = _mm_rem_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_rem_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       s32x8 &operator^= (s32x8 b)       { return lo.ps = _mm_xor_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_xor_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       s32x8 &operator&= (s32x8 b)       { return lo.ps = _mm_and_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_and_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       s32x8 &operator|= (s32x8 b)       { return lo.ps = _mm_or_ps      (lo.ps, b.lo.ps)                         , hi.ps = _mm_or_ps      (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       s32x8 &operator<<=(s32x8 b)       { return lo.pi = _mm_sllv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_sllv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       s32x8 &operator>>=(s32x8 b)       { return lo.pi = _mm_srlv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_srlv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        s32x8 &operator+= (s32   b)       { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        s32x8 &operator-= (s32   b)       { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        s32x8 &operator*= (s32   b)       { return lo.pi = _mm_mullo_epi32(lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_mullo_epi32(hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        s32x8 &operator/= (s32   b)       { return lo.pi = _mm_div_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_div_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       s32x8 &operator%= (s32   b)       { return lo.pi = _mm_rem_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_rem_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       s32x8 &operator^= (s32   b)       { return lo.ps = _mm_xor_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_xor_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator&= (s32   b)       { return lo.ps = _mm_and_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_and_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator|= (s32   b)       { return lo.ps = _mm_or_ps      (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_or_ps      (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       s32x8 &operator<<=(s32   b)       { return lo.pi = _mm_slli_epi32 (lo.pi, b)                               , hi.pi = _mm_slli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE	       s32x8 &operator>>=(s32   b)       { return lo.pi = _mm_srli_epi32 (lo.pi, b)                               , hi.pi = _mm_srli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE        s32x8 &operator++ ()              { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        s32x8 &operator-- ()              { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        s32x8  operator++ (int)           { auto result = *this; lo.pi = _mm_add_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_add_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
	FORCEINLINE        s32x8  operator-- (int)           { auto result = *this; lo.pi = _mm_sub_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_sub_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
#endif
	s32 s[8];

	CVT(f32x8);
	CVT(u32x8);
	MEMFUNS_DATA(s32);
	INDEX_S(s32);
};

template<>
union u32x<4> {
	M16 m;
	u32 s[4];
	
	FORCEINLINE        u32x4  operator-()          const { return {_mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), m.pi))}; }
	FORCEINLINE        u32x4  operator~()          const { return {_mm_not_ps(m.ps)}; }
	FORCEINLINE	       u32x4  operator+ (u32x4 b)  const { return {_mm_castpi_ps(_mm_add_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator- (u32x4 b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator* (u32x4 b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator/ (u32x4 b)  const { return {_mm_castpi_ps(_mm_div_epu32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator% (u32x4 b)  const { return {_mm_castpi_ps(_mm_rem_epu32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator^ (u32x4 b)  const { return {_mm_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       u32x4  operator& (u32x4 b)  const { return {_mm_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       u32x4  operator| (u32x4 b)  const { return {_mm_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE	       u32x4  operator<<(u32x4 b)  const { return {_mm_castpi_ps(_mm_sllv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator>>(u32x4 b)  const { return {_mm_castpi_ps(_mm_srlv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator< (u32x4 b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator> (u32x4 b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator<=(u32x4 b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator>=(u32x4 b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator==(u32x4 b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x4  operator!=(u32x4 b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x4  operator+ (u32   b)  const { return {_mm_castpi_ps(_mm_add_epi32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator- (u32   b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator* (u32   b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator/ (u32   b)  const { return {_mm_castpi_ps(_mm_div_epu32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator% (u32   b)  const { return {_mm_castpi_ps(_mm_rem_epu32  (m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator^ (u32   b)  const { return {_mm_xor_ps(m.ps, _mm_castpi_ps(_mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator& (u32   b)  const { return {_mm_and_ps(m.ps, _mm_castpi_ps(_mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator| (u32   b)  const { return {_mm_or_ps (m.ps, _mm_castpi_ps(_mm_set1_epi32(b)))}; }
	FORCEINLINE	       u32x4  operator<<(u32   b)  const { return {_mm_castpi_ps(_mm_slli_epi32(m.pi, b))}; }
	FORCEINLINE	       u32x4  operator>>(u32   b)  const { return {_mm_castpi_ps(_mm_srli_epi32(m.pi, b))}; }
	FORCEINLINE	       b32x4  operator< (u32   b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator> (u32   b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator<=(u32   b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator>=(u32   b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator==(u32   b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE	       b32x4  operator!=(u32   b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(m.pi, _mm_set1_epi32(b)))}; }
	FORCEINLINE friend u32x4  operator+ (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend u32x4  operator- (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend u32x4  operator* (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend u32x4  operator/ (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_div_epu32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend u32x4  operator% (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_rem_epu32  (_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend u32x4  operator^ (u32 a, u32x4 b) { return {_mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend u32x4  operator& (u32 a, u32x4 b) { return {_mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend u32x4  operator| (u32 a, u32x4 b) { return {_mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend u32x4  operator<<(u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_slliv_epi32(a, b.m.pi))}; }
	FORCEINLINE	friend u32x4  operator>>(u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_srliv_epi32(a, b.m.pi))}; }
	FORCEINLINE friend b32x4  operator< (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator> (u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator<=(u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator>=(u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator==(u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x4  operator!=(u32 a, u32x4 b) { return {_mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE        u32x4 &operator+= (u32x4 b)       { return m.pi = _mm_add_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x4 &operator-= (u32x4 b)       { return m.pi = _mm_sub_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x4 &operator*= (u32x4 b)       { return m.pi = _mm_mullo_epi32(m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x4 &operator/= (u32x4 b)       { return m.pi = _mm_div_epu32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       u32x4 &operator%= (u32x4 b)       { return m.pi = _mm_rem_epu32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       u32x4 &operator^= (u32x4 b)       { return m.ps = _mm_xor_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       u32x4 &operator&= (u32x4 b)       { return m.ps = _mm_and_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       u32x4 &operator|= (u32x4 b)       { return m.ps = _mm_or_ps      (m.ps, b.m.ps), *this; }
	FORCEINLINE	       u32x4 &operator<<=(u32x4 b)       { return m.pi = _mm_sllv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE	       u32x4 &operator>>=(u32x4 b)       { return m.pi = _mm_srlv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x4 &operator+= (u32   b)       { return m.pi = _mm_add_epi32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE        u32x4 &operator-= (u32   b)       { return m.pi = _mm_sub_epi32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE        u32x4 &operator*= (u32   b)       { return m.pi = _mm_mullo_epi32(m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE        u32x4 &operator/= (u32   b)       { return m.pi = _mm_div_epu32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE	       u32x4 &operator%= (u32   b)       { return m.pi = _mm_rem_epu32  (m.pi, _mm_set1_epi32(b)), *this; }
	FORCEINLINE	       u32x4 &operator^= (u32   b)       { return m.ps = _mm_xor_ps     (m.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x4 &operator&= (u32   b)       { return m.ps = _mm_and_ps     (m.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x4 &operator|= (u32   b)       { return m.ps = _mm_or_ps      (m.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x4 &operator<<=(u32   b)       { return m.pi = _mm_slli_epi32(m.pi, b), *this; }
	FORCEINLINE	       u32x4 &operator>>=(u32   b)       { return m.pi = _mm_srli_epi32(m.pi, b), *this; }
	FORCEINLINE        u32x4 &operator++ ()              { return m.pi = _mm_add_epi32  (m.pi, _mm_set1_epi32(1)), *this; }
	FORCEINLINE        u32x4 &operator-- ()              { return m.pi = _mm_sub_epi32  (m.pi, _mm_set1_epi32(1)), *this; }
	FORCEINLINE        u32x4  operator++ (int)           { auto result = *this; m.pi = _mm_add_epi32  (m.pi, _mm_set1_epi32(1)); return result; }
	FORCEINLINE        u32x4  operator-- (int)           { auto result = *this; m.pi = _mm_sub_epi32  (m.pi, _mm_set1_epi32(1)); return result; }

	CVT(f32x4);
	CVT(s32x4);
	CVT(f64x4);
	MEMFUNS_DATA(u32);
	INDEX_S(u32);
};

#define BROADCAST(a) _mm256_set1_epi32((int)a)
#define NEGATE(a) _mm256_sub_epi32(_mm256_setzero_pi(), a)
#define FLIP(a) _mm256_castps_pi(_mm256_xor_ps(_mm256_castpi_ps(a), _mm256_setone_ps()))
#define ADD(a, b) _mm256_add_epi32(a, b)
#define SUB(a, b) _mm256_sub_epi32(a, b)
#define MUL(a, b) _mm256_mullo_epi32(a, b)
#define DIV(a, b) _mm256_div_epu32(a, b)
#define MOD(a, b) _mm256_rem_epu32(a, b)
#define XOR(a, b) _mm256_castps_pi(_mm256_xor_ps(_mm256_castpi_ps(a), _mm256_castpi_ps(b)))
#define AND(a, b) _mm256_castps_pi(_mm256_and_ps(_mm256_castpi_ps(a), _mm256_castpi_ps(b)))
#define  OR(a, b) _mm256_castps_pi(_mm256_or_ps (_mm256_castpi_ps(a), _mm256_castpi_ps(b)))
#define SHL_MM(a, b) _mm256_setr_epi32(									\
	(u32)_mm256_extract_epi32(a, 0) << (u32)_mm256_extract_epi32(b, 0),	\
	(u32)_mm256_extract_epi32(a, 1) << (u32)_mm256_extract_epi32(b, 1),	\
	(u32)_mm256_extract_epi32(a, 2) << (u32)_mm256_extract_epi32(b, 2),	\
	(u32)_mm256_extract_epi32(a, 3) << (u32)_mm256_extract_epi32(b, 3),	\
	(u32)_mm256_extract_epi32(a, 4) << (u32)_mm256_extract_epi32(b, 4),	\
	(u32)_mm256_extract_epi32(a, 5) << (u32)_mm256_extract_epi32(b, 5),	\
	(u32)_mm256_extract_epi32(a, 6) << (u32)_mm256_extract_epi32(b, 6),	\
	(u32)_mm256_extract_epi32(a, 7) << (u32)_mm256_extract_epi32(b, 7))
#define SHR_MM(a, b) _mm256_setr_epi32(									\
	(u32)_mm256_extract_epi32(a, 0) >> (u32)_mm256_extract_epi32(b, 0),	\
	(u32)_mm256_extract_epi32(a, 1) >> (u32)_mm256_extract_epi32(b, 1),	\
	(u32)_mm256_extract_epi32(a, 2) >> (u32)_mm256_extract_epi32(b, 2),	\
	(u32)_mm256_extract_epi32(a, 3) >> (u32)_mm256_extract_epi32(b, 3),	\
	(u32)_mm256_extract_epi32(a, 4) >> (u32)_mm256_extract_epi32(b, 4),	\
	(u32)_mm256_extract_epi32(a, 5) >> (u32)_mm256_extract_epi32(b, 5),	\
	(u32)_mm256_extract_epi32(a, 6) >> (u32)_mm256_extract_epi32(b, 6),	\
	(u32)_mm256_extract_epi32(a, 7) >> (u32)_mm256_extract_epi32(b, 7))
#define SHL_MS(a, b) _mm256_slli_epi32(a, _mm256_extract_epi32(b, 0))
#define SHR_MS(a, b) _mm256_srli_epi32(a, _mm256_extract_epi32(b, 0))
#define SHL_SM(a, b) SHL_MM(a, b)
#define SHR_SM(a, b) SHR_MM(a, b)
#define SHL(a, b, mode) SHL_##mode(a, b)
#define SHR(a, b, mode) SHR_##mode(a, b)
#define CMPLT(a, b) _mm256_cmpgt_epu32(b, a)
#define CMPGT(a, b) _mm256_cmpgt_epu32(a, b)
#define CMPLE(a, b) _mm256_not_epi32(_mm256_cmpgt_epu32(a, b))
#define CMPGE(a, b) _mm256_not_epi32(_mm256_cmpgt_epu32(b, a))
#define CMPEQ(a, b) _mm256_cmpeq_epi32(a, b)
#define CMPNE(a, b) _mm256_not_epi32(_mm256_cmpeq_epi32(a, b))

template<>
union u32x<8> {
#if ARCH_AVX
	M32 m;
#endif
	struct {
		M16 lo, hi;
	};
	u32 s[8];

#if ARCH_AVX2
	FORCEINLINE        u32x8  operator-()          const { return {_mm256_castpi_ps(_mm256_sub_epi32(_mm256_setzero_pi(), m.pi))}; }
	FORCEINLINE        u32x8  operator~()          const { return {_mm256_not_ps(m.ps)}; }
	FORCEINLINE	       u32x8  operator+ (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_add_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator- (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_sub_epi32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator* (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_mullo_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator/ (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_div_epu32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator% (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_rem_epu32  (m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator^ (u32x8 b)  const { return {_mm256_xor_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       u32x8  operator& (u32x8 b)  const { return {_mm256_and_ps(m.ps, b.m.ps)}; }
	FORCEINLINE	       u32x8  operator| (u32x8 b)  const { return {_mm256_or_ps (m.ps, b.m.ps)}; }
	FORCEINLINE	       u32x8  operator<<(u32x8 b)  const { return {_mm256_castpi_ps(_mm256_sllv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator>>(u32x8 b)  const { return {_mm256_castpi_ps(_mm256_srlv_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator< (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmplt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator> (u32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpgt_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator<=(u32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmple_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator>=(u32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpge_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator==(u32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpeq_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       b32x8  operator!=(u32x8 b)  const { return {_mm256_castpi_ps(_mm256_cmpne_epi32(m.pi, b.m.pi))}; }
	FORCEINLINE	       u32x8  operator+ (u32   b)  const { return {_mm256_castpi_ps(_mm256_add_epi32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator- (u32   b)  const { return {_mm256_castpi_ps(_mm256_sub_epi32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator* (u32   b)  const { return {_mm256_castpi_ps(_mm256_mullo_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator/ (u32   b)  const { return {_mm256_castpi_ps(_mm256_div_epu32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator% (u32   b)  const { return {_mm256_castpi_ps(_mm256_rem_epu32  (m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator^ (u32   b)  const { return {_mm256_xor_ps(m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator& (u32   b)  const { return {_mm256_and_ps(m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator| (u32   b)  const { return {_mm256_or_ps (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b)))}; }
	FORCEINLINE	       u32x8  operator<<(u32   b)  const { return {_mm256_castpi_ps(_mm256_slli_epi32(m.pi, b))}; }
	FORCEINLINE	       u32x8  operator>>(u32   b)  const { return {_mm256_castpi_ps(_mm256_srli_epi32(m.pi, b))}; }
	FORCEINLINE	       b32x8  operator< (u32   b)  const { return {_mm256_castpi_ps(_mm256_cmplt_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator> (u32   b)  const { return {_mm256_castpi_ps(_mm256_cmpgt_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator<=(u32   b)  const { return {_mm256_castpi_ps(_mm256_cmple_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator>=(u32   b)  const { return {_mm256_castpi_ps(_mm256_cmpge_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator==(u32   b)  const { return {_mm256_castpi_ps(_mm256_cmpeq_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE	       b32x8  operator!=(u32   b)  const { return {_mm256_castpi_ps(_mm256_cmpne_epi32(m.pi, _mm256_set1_epi32(b)))}; }
	FORCEINLINE friend u32x8  operator+ (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_add_epi32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend u32x8  operator- (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_sub_epi32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend u32x8  operator* (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_mullo_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend u32x8  operator/ (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_div_epu32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend u32x8  operator% (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_rem_epu32  (_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE	friend u32x8  operator^ (u32 a, u32x8 b) { return {_mm256_xor_ps(_mm256_castpi_ps(_mm256_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend u32x8  operator& (u32 a, u32x8 b) { return {_mm256_and_ps(_mm256_castpi_ps(_mm256_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend u32x8  operator| (u32 a, u32x8 b) { return {_mm256_or_ps (_mm256_castpi_ps(_mm256_set1_epi32(a)), b.m.ps)}; }
	FORCEINLINE	friend u32x8  operator<<(u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_slliv_epi32(a, b.m.pi))}; }
	FORCEINLINE	friend u32x8  operator>>(u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_srliv_epi32(a, b.m.pi))}; }
	FORCEINLINE friend b32x8  operator< (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_cmplt_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator> (u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_cmpgt_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator<=(u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_cmple_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator>=(u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_cmpge_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator==(u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_cmpeq_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE friend b32x8  operator!=(u32 a, u32x8 b) { return {_mm256_castpi_ps(_mm256_cmpne_epi32(_mm256_set1_epi32(a), b.m.pi))}; }
	FORCEINLINE        u32x8 &operator+=(u32x8 b)        { return m.pi = _mm256_add_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x8 &operator-=(u32x8 b)        { return m.pi = _mm256_sub_epi32  (m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x8 &operator*=(u32x8 b)        { return m.pi = _mm256_mullo_epi32(m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x8 &operator/=(u32x8 b)        { return m.pi = _mm256_div_epu32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       u32x8 &operator%=(u32x8 b)        { return m.pi = _mm256_rem_epu32  (m.pi, b.m.pi), *this; }
	FORCEINLINE	       u32x8 &operator^=(u32x8 b)        { return m.ps = _mm256_xor_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       u32x8 &operator&=(u32x8 b)        { return m.ps = _mm256_and_ps     (m.ps, b.m.ps), *this; }
	FORCEINLINE	       u32x8 &operator|=(u32x8 b)        { return m.ps = _mm256_or_ps      (m.ps, b.m.ps), *this; }
	FORCEINLINE	       u32x8 &operator<<=(u32x8 b)       { return m.pi = _mm256_sllv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE	       u32x8 &operator>>=(u32x8 b)       { return m.pi = _mm256_srlv_epi32 (m.pi, b.m.pi), *this; }
	FORCEINLINE        u32x8 &operator+=(u32   b)        { return m.pi = _mm256_add_epi32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE        u32x8 &operator-=(u32   b)        { return m.pi = _mm256_sub_epi32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE        u32x8 &operator*=(u32   b)        { return m.pi = _mm256_mullo_epi32(m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE        u32x8 &operator/=(u32   b)        { return m.pi = _mm256_div_epu32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE	       u32x8 &operator%=(u32   b)        { return m.pi = _mm256_rem_epu32  (m.pi, _mm256_set1_epi32(b)), *this; }
	FORCEINLINE	       u32x8 &operator^=(u32   b)        { return m.ps = _mm256_xor_ps     (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator&=(u32   b)        { return m.ps = _mm256_and_ps     (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator|=(u32   b)        { return m.ps = _mm256_or_ps      (m.ps, _mm256_castpi_ps(_mm256_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator<<=(u32   b)       { return m.pi = _mm256_slli_epi32(m.pi, b), *this; }
	FORCEINLINE	       u32x8 &operator>>=(u32   b)       { return m.pi = _mm256_srli_epi32(m.pi, b), *this; }
	FORCEINLINE        u32x8 &operator++ ()              { return m.pi = _mm256_add_epi32  (m.pi, _mm256_set1_epi32(1)), *this; }
	FORCEINLINE        u32x8 &operator-- ()              { return m.pi = _mm256_sub_epi32  (m.pi, _mm256_set1_epi32(1)), *this; }
	FORCEINLINE        u32x8  operator++ (int)           { auto result = *this; m.pi = _mm256_add_epi32  (m.pi, _mm256_set1_epi32(1)); return result; }
	FORCEINLINE        u32x8  operator-- (int)           { auto result = *this; m.pi = _mm256_sub_epi32  (m.pi, _mm256_set1_epi32(1)); return result; }
#elif ARCH_AVX
	FORCEINLINE        u32x8  operator-()          const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), lo.pi))        , _mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), hi.pi))        )}; }
	FORCEINLINE        u32x8  operator~()          const { return {_mm256_setr_m128(_mm_not_ps(lo.ps), _mm_not_ps(hi.ps))}; }
	FORCEINLINE	       u32x8  operator+ (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_add_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_add_epi32  (hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       u32x8  operator- (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       u32x8  operator* (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_mullo_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       u32x8  operator/ (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_div_epu32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_div_epu32  (hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       u32x8  operator% (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_rem_epu32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_rem_epu32  (hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       u32x8  operator^ (u32x8 b)  const { return {_mm256_setr_m128(_mm_xor_ps(lo.ps, b.lo.ps)                                   , _mm_xor_ps(hi.ps, b.hi.ps)                                   )}; }
	FORCEINLINE	       u32x8  operator& (u32x8 b)  const { return {_mm256_setr_m128(_mm_and_ps(lo.ps, b.lo.ps)                                   , _mm_and_ps(hi.ps, b.hi.ps)                                   )}; }
	FORCEINLINE	       u32x8  operator| (u32x8 b)  const { return {_mm256_setr_m128(_mm_or_ps (lo.ps, b.lo.ps)                                   , _mm_or_ps (hi.ps, b.hi.ps)                                   )}; }
	FORCEINLINE	       u32x8  operator<<(u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sllv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_sllv_epi32(hi.pi, b.hi.pi))                )}; }
	FORCEINLINE	       u32x8  operator>>(u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_srlv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_srlv_epi32(hi.pi, b.hi.pi))                )}; }
	FORCEINLINE	       b32x8  operator< (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       b32x8  operator> (u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       b32x8  operator<=(u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmple_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmple_epi32(hi.pi, b.hi.pi))				)}; }
	FORCEINLINE	       b32x8  operator>=(u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, b.hi.pi))				)}; }
	FORCEINLINE	       b32x8  operator==(u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, b.hi.pi))               )}; }
	FORCEINLINE	       b32x8  operator!=(u32x8 b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, b.hi.pi))				)}; }
	FORCEINLINE	       u32x8  operator+ (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_add_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_add_epi32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       u32x8  operator- (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       u32x8  operator* (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_mullo_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       u32x8  operator/ (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_div_epu32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_div_epu32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       u32x8  operator% (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_rem_epu32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_rem_epu32  (hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       u32x8  operator^ (u32   b)  const { return {_mm256_setr_m128(_mm_xor_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_xor_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          )}; }
	FORCEINLINE	       u32x8  operator& (u32   b)  const { return {_mm256_setr_m128(_mm_and_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_and_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          )}; }
	FORCEINLINE	       u32x8  operator| (u32   b)  const { return {_mm256_setr_m128(_mm_or_ps (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_or_ps (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          )}; }
	FORCEINLINE	       u32x8  operator<<(u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_slli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_slli_epi32(hi.pi, b))                      )}; }
	FORCEINLINE	       u32x8  operator>>(u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_srli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_srli_epi32(hi.pi, b))                      )}; }
	FORCEINLINE	       b32x8  operator< (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       b32x8  operator> (u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       b32x8  operator<=(u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmple_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmple_epi32(hi.pi, _mm_set1_epi32(b)))		)}; }
	FORCEINLINE	       b32x8  operator>=(u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, _mm_set1_epi32(b)))		)}; }
	FORCEINLINE	       b32x8  operator==(u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, _mm_set1_epi32(b)))     )}; }
	FORCEINLINE	       b32x8  operator!=(u32   b)  const { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, _mm_set1_epi32(b)))		)}; }
	FORCEINLINE friend u32x8  operator+ (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE friend u32x8  operator- (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE friend u32x8  operator* (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE friend u32x8  operator/ (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_div_epu32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_div_epu32  (_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE	friend u32x8  operator% (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_rem_epu32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_rem_epu32  (_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE	friend u32x8  operator^ (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        )}; }
	FORCEINLINE	friend u32x8  operator& (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        )}; }
	FORCEINLINE	friend u32x8  operator| (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        )}; }
	FORCEINLINE	friend u32x8  operator<<(u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_slliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_slliv_epi32(a, b.hi.pi))                   )}; }
	FORCEINLINE	friend u32x8  operator>>(u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_srliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_srliv_epi32(a, b.hi.pi))                   )}; }
	FORCEINLINE friend b32x8  operator< (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE friend b32x8  operator> (u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE friend b32x8  operator<=(u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.hi.pi))	)}; }
	FORCEINLINE friend b32x8  operator>=(u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.hi.pi))	)}; }
	FORCEINLINE friend b32x8  operator==(u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.hi.pi))   )}; }
	FORCEINLINE friend b32x8  operator!=(u32 a, u32x8 b) { return {_mm256_setr_m128(_mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.hi.pi))	)}; }
	FORCEINLINE        u32x8 &operator+= (u32x8 b)       { return lo.pi = _mm_add_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_add_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator-= (u32x8 b)       { return lo.pi = _mm_sub_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_sub_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator*= (u32x8 b)       { return lo.pi = _mm_mullo_epi32(lo.pi, b.lo.pi)                         , hi.pi = _mm_mullo_epi32(hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator/= (u32x8 b)       { return lo.pi = _mm_div_epu32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_div_epu32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       u32x8 &operator%= (u32x8 b)       { return lo.pi = _mm_rem_epu32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_rem_epu32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       u32x8 &operator^= (u32x8 b)       { return lo.ps = _mm_xor_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_xor_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       u32x8 &operator&= (u32x8 b)       { return lo.ps = _mm_and_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_and_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       u32x8 &operator|= (u32x8 b)       { return lo.ps = _mm_or_ps      (lo.ps, b.lo.ps)                         , hi.ps = _mm_or_ps      (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       u32x8 &operator<<=(u32x8 b)       { return lo.pi = _mm_sllv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_sllv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       u32x8 &operator>>=(u32x8 b)       { return lo.pi = _mm_srlv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_srlv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator+= (u32   b)       { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        u32x8 &operator-= (u32   b)       { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        u32x8 &operator*= (u32   b)       { return lo.pi = _mm_mullo_epi32(lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_mullo_epi32(hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        u32x8 &operator/= (u32   b)       { return lo.pi = _mm_div_epu32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_div_epu32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       u32x8 &operator%= (u32   b)       { return lo.pi = _mm_rem_epu32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_rem_epu32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       u32x8 &operator^= (u32   b)       { return lo.ps = _mm_xor_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_xor_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator&= (u32   b)       { return lo.ps = _mm_and_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_and_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator|= (u32   b)       { return lo.ps = _mm_or_ps      (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_or_ps      (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator<<=(u32   b)       { return lo.pi = _mm_slli_epi32 (lo.pi, b)                               , hi.pi = _mm_slli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE	       u32x8 &operator>>=(u32   b)       { return lo.pi = _mm_srli_epi32 (lo.pi, b)                               , hi.pi = _mm_srli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE        u32x8 &operator++ ()              { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        u32x8 &operator-- ()              { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        u32x8  operator++ (int)           { auto result = *this; lo.pi = _mm_add_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_add_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
	FORCEINLINE        u32x8  operator-- (int)           { auto result = *this; lo.pi = _mm_sub_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_sub_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
#else
	struct {
		M16 lo, hi;
	};
	FORCEINLINE        u32x8  operator-()          const { return {_mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), lo.pi))        , _mm_castpi_ps(_mm_sub_epi32(_mm_setzero_pi(), hi.pi))        }; }
	FORCEINLINE        u32x8  operator~()          const { return {_mm_not_ps(lo.ps), _mm_not_ps(hi.ps)}; }
	FORCEINLINE	       u32x8  operator+ (u32x8 b)  const { return {_mm_castpi_ps(_mm_add_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_add_epi32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       u32x8  operator- (u32x8 b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       u32x8  operator* (u32x8 b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       u32x8  operator/ (u32x8 b)  const { return {_mm_castpi_ps(_mm_div_epu32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_div_epu32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       u32x8  operator% (u32x8 b)  const { return {_mm_castpi_ps(_mm_rem_epu32  (lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_rem_epu32  (hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       u32x8  operator^ (u32x8 b)  const { return {_mm_xor_ps(lo.ps, b.lo.ps)                                   , _mm_xor_ps(hi.ps, b.hi.ps)                                   }; }
	FORCEINLINE	       u32x8  operator& (u32x8 b)  const { return {_mm_and_ps(lo.ps, b.lo.ps)                                   , _mm_and_ps(hi.ps, b.hi.ps)                                   }; }
	FORCEINLINE	       u32x8  operator| (u32x8 b)  const { return {_mm_or_ps (lo.ps, b.lo.ps)                                   , _mm_or_ps (hi.ps, b.hi.ps)                                   }; }
	FORCEINLINE	       u32x8  operator<<(u32x8 b)  const { return {_mm_castpi_ps(_mm_sllv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_sllv_epi32(hi.pi, b.hi.pi))                }; }
	FORCEINLINE	       u32x8  operator>>(u32x8 b)  const { return {_mm_castpi_ps(_mm_srlv_epi32(lo.pi, b.lo.pi))                , _mm_castpi_ps(_mm_srlv_epi32(hi.pi, b.hi.pi))                }; }
	FORCEINLINE	       b32x8  operator< (u32x8 b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       b32x8  operator> (u32x8 b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       b32x8  operator<=(u32x8 b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmple_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator>=(u32x8 b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       b32x8  operator==(u32x8 b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, b.hi.pi))               }; }
	FORCEINLINE	       b32x8  operator!=(u32x8 b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, b.lo.pi))               , _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, b.hi.pi))            }; }
	FORCEINLINE	       u32x8  operator+ (u32   b)  const { return {_mm_castpi_ps(_mm_add_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_add_epi32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       u32x8  operator- (u32   b)  const { return {_mm_castpi_ps(_mm_sub_epi32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_sub_epi32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       u32x8  operator* (u32   b)  const { return {_mm_castpi_ps(_mm_mullo_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_mullo_epi32(hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       u32x8  operator/ (u32   b)  const { return {_mm_castpi_ps(_mm_div_epu32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_div_epu32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       u32x8  operator% (u32   b)  const { return {_mm_castpi_ps(_mm_rem_epu32  (lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_rem_epu32  (hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       u32x8  operator^ (u32   b)  const { return {_mm_xor_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_xor_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          }; }
	FORCEINLINE	       u32x8  operator& (u32   b)  const { return {_mm_and_ps(lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_and_ps(hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          }; }
	FORCEINLINE	       u32x8  operator| (u32   b)  const { return {_mm_or_ps (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          , _mm_or_ps (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b)))          }; }
	FORCEINLINE	       u32x8  operator<<(u32   b)  const { return {_mm_castpi_ps(_mm_slli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_slli_epi32(hi.pi, b))                      }; }
	FORCEINLINE	       u32x8  operator>>(u32   b)  const { return {_mm_castpi_ps(_mm_srli_epi32(lo.pi, b))                      , _mm_castpi_ps(_mm_srli_epi32(hi.pi, b))                      }; }
	FORCEINLINE	       b32x8  operator< (u32   b)  const { return {_mm_castpi_ps(_mm_cmplt_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmplt_epi32(hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       b32x8  operator> (u32   b)  const { return {_mm_castpi_ps(_mm_cmpgt_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpgt_epi32(hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       b32x8  operator<=(u32   b)  const { return {_mm_castpi_ps(_mm_cmple_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmple_epi32(hi.pi, _mm_set1_epi32(b)))  }; }
	FORCEINLINE	       b32x8  operator>=(u32   b)  const { return {_mm_castpi_ps(_mm_cmpge_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpge_epi32(hi.pi, _mm_set1_epi32(b)))  }; }
	FORCEINLINE	       b32x8  operator==(u32   b)  const { return {_mm_castpi_ps(_mm_cmpeq_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpeq_epi32(hi.pi, _mm_set1_epi32(b)))     }; }
	FORCEINLINE	       b32x8  operator!=(u32   b)  const { return {_mm_castpi_ps(_mm_cmpne_epi32(lo.pi, _mm_set1_epi32(b)))     , _mm_castpi_ps(_mm_cmpne_epi32(hi.pi, _mm_set1_epi32(b)))  }; }
	FORCEINLINE friend u32x8  operator+ (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_add_epi32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend u32x8  operator- (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_sub_epi32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend u32x8  operator* (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_mullo_epi32(_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend u32x8  operator/ (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_div_epu32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_div_epu32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE	friend u32x8  operator% (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_rem_epu32  (_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_rem_epu32  (_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE	friend u32x8  operator^ (u32 a, u32x8 b) { return {_mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_xor_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        }; }
	FORCEINLINE	friend u32x8  operator& (u32 a, u32x8 b) { return {_mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_and_ps(_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        }; }
	FORCEINLINE	friend u32x8  operator| (u32 a, u32x8 b) { return {_mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.lo.ps)        , _mm_or_ps (_mm_castpi_ps(_mm_set1_epi32(a)), b.hi.ps)        }; }
	FORCEINLINE	friend u32x8  operator<<(u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_slliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_slliv_epi32(a, b.hi.pi))                   }; }
	FORCEINLINE	friend u32x8  operator>>(u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_srliv_epi32(a, b.lo.pi))                   , _mm_castpi_ps(_mm_srliv_epi32(a, b.hi.pi))                   }; }
	FORCEINLINE friend b32x8  operator< (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmplt_epi32(_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend b32x8  operator> (u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpgt_epi32(_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend b32x8  operator<=(u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmple_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator>=(u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpge_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE friend b32x8  operator==(u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpeq_epi32(_mm_set1_epi32(a), b.hi.pi))   }; }
	FORCEINLINE friend b32x8  operator!=(u32 a, u32x8 b) { return {_mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.lo.pi))   , _mm_castpi_ps(_mm_cmpne_epi32(_mm_set1_epi32(a), b.hi.pi))}; }
	FORCEINLINE        u32x8 &operator+= (u32x8 b)       { return lo.pi = _mm_add_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_add_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator-= (u32x8 b)       { return lo.pi = _mm_sub_epi32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_sub_epi32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator*= (u32x8 b)       { return lo.pi = _mm_mullo_epi32(lo.pi, b.lo.pi)                         , hi.pi = _mm_mullo_epi32(hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator/= (u32x8 b)       { return lo.pi = _mm_div_epu32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_div_epu32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       u32x8 &operator%= (u32x8 b)       { return lo.pi = _mm_rem_epu32  (lo.pi, b.lo.pi)                         , hi.pi = _mm_rem_epu32  (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       u32x8 &operator^= (u32x8 b)       { return lo.ps = _mm_xor_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_xor_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       u32x8 &operator&= (u32x8 b)       { return lo.ps = _mm_and_ps     (lo.ps, b.lo.ps)                         , hi.ps = _mm_and_ps     (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       u32x8 &operator|= (u32x8 b)       { return lo.ps = _mm_or_ps      (lo.ps, b.lo.ps)                         , hi.ps = _mm_or_ps      (hi.ps, b.hi.ps)                         , *this; }
	FORCEINLINE	       u32x8 &operator<<=(u32x8 b)       { return lo.pi = _mm_sllv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_sllv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE	       u32x8 &operator>>=(u32x8 b)       { return lo.pi = _mm_srlv_epi32 (lo.pi, b.lo.pi)                         , hi.pi = _mm_srlv_epi32 (hi.pi, b.hi.pi)                         , *this; }
	FORCEINLINE        u32x8 &operator+= (u32   b)       { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        u32x8 &operator-= (u32   b)       { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        u32x8 &operator*= (u32   b)       { return lo.pi = _mm_mullo_epi32(lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_mullo_epi32(hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE        u32x8 &operator/= (u32   b)       { return lo.pi = _mm_div_epu32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_div_epu32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       u32x8 &operator%= (u32   b)       { return lo.pi = _mm_rem_epu32  (lo.pi, _mm_set1_epi32(b))               , hi.pi = _mm_rem_epu32  (hi.pi, _mm_set1_epi32(b))               , *this; }
	FORCEINLINE	       u32x8 &operator^= (u32   b)       { return lo.ps = _mm_xor_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_xor_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator&= (u32   b)       { return lo.ps = _mm_and_ps     (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_and_ps     (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator|= (u32   b)       { return lo.ps = _mm_or_ps      (lo.ps, _mm_castpi_ps(_mm_set1_epi32(b))), hi.ps = _mm_or_ps      (hi.ps, _mm_castpi_ps(_mm_set1_epi32(b))), *this; }
	FORCEINLINE	       u32x8 &operator<<=(u32   b)       { return lo.pi = _mm_slli_epi32 (lo.pi, b)                               , hi.pi = _mm_slli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE	       u32x8 &operator>>=(u32   b)       { return lo.pi = _mm_srli_epi32 (lo.pi, b)                               , hi.pi = _mm_srli_epi32 (hi.pi, b)                               , *this; }
	FORCEINLINE        u32x8 &operator++ ()              { return lo.pi = _mm_add_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_add_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        u32x8 &operator-- ()              { return lo.pi = _mm_sub_epi32  (lo.pi, _mm_set1_epi32(1))               , hi.pi = _mm_sub_epi32  (hi.pi, _mm_set1_epi32(1))               , *this; }
	FORCEINLINE        u32x8  operator++ (int)           { auto result = *this; lo.pi = _mm_add_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_add_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
	FORCEINLINE        u32x8  operator-- (int)           { auto result = *this; lo.pi = _mm_sub_epi32(lo.pi, _mm_set1_epi32(1)); hi.pi = _mm_sub_epi32(hi.pi, _mm_set1_epi32(1)); return result; }
#endif

	CVT(f32x8);
	CVT(s32x8);
	MEMFUNS_DATA(u32);
	INDEX_S(u32);
};

FORCEINLINE b32x4 B32x4(b32 a, b32 b, b32 c, b32 d) { return S32x4(a, b, c, d) != 0; }
FORCEINLINE b32x8 B32x8(b32 a, b32 b, b32 c, b32 d, b32 e, b32 f, b32 g, b32 h) { return S32x8(a, b, c, d, e, f, g, h) != 0; }

// clang-format off
FORCEINLINE f32x4 F32x4(f32 v) { return {_mm_set1_ps(v)}; }
FORCEINLINE s32x4 S32x4(s32 v) { return {_mm_castpi_ps(_mm_set1_epi32(v))}; }
FORCEINLINE u32x4 U32x4(u32 v) { return {_mm_castpi_ps(_mm_set1_epi32((s32)v))}; }

FORCEINLINE f32x4 F32x4(f32 i, f32 j, f32 k, f32 l) { return {_mm_setr_ps(i, j, k, l)}; }
FORCEINLINE s32x4 S32x4(s32 i, s32 j, s32 k, s32 l) { return {_mm_castpi_ps(_mm_setr_epi32(i, j, k, l))}; }
FORCEINLINE u32x4 U32x4(u32 i, u32 j, u32 k, u32 l) { return {_mm_castpi_ps(_mm_setr_epi32((s32)i, (s32)j, (s32)k, (s32)l))}; }

FORCEINLINE f64x2 F64x2(f64 i, f64 j) { return {_mm_castpd_ps(_mm_setr_pd(i, j))}; }
#if ARCH_AVX
FORCEINLINE f32x8 F32x8(f32 v) { return {_mm256_set1_ps(v)}; }
FORCEINLINE s32x8 S32x8(s32 v) { return {_mm256_castpi_ps(_mm256_set1_epi32(v))}; }
FORCEINLINE u32x8 U32x8(u32 v) { return {_mm256_castpi_ps(_mm256_set1_epi32((s32)v))}; }

FORCEINLINE f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) { return {_mm256_setr_ps(a, b, c, d, e, f, g, h)}; }
FORCEINLINE s32x8 S32x8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h) { return {_mm256_castpi_ps(_mm256_setr_epi32(a, b, c, d, e, f, g, h))}; }
FORCEINLINE u32x8 U32x8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) { return {_mm256_castpi_ps(_mm256_setr_epi32((s32)a, (s32)b, (s32)c, (s32)d, (s32)e, (s32)f, (s32)g, (s32)h))}; }

FORCEINLINE f32x8 F32x8(f32x4 m0, f32x4 m1) { return {_mm256_setr_m128(m0.m.ps, m1.m.ps)}; }
FORCEINLINE s32x8 S32x8(s32x4 m0, s32x4 m1) { return {_mm256_setr_m128(m0.m.ps, m1.m.ps)}; }
FORCEINLINE u32x8 U32x8(u32x4 m0, u32x4 m1) { return {_mm256_setr_m128(m0.m.ps, m1.m.ps)}; }

FORCEINLINE f64x4 F64x4(f64 i, f64 j, f64 k, f64 l) { return {_mm256_castpd_ps(_mm256_setr_pd(i, j, k, l))}; }
#else
FORCEINLINE f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) { return {_mm_setr_ps(a, b, c, d), _mm_setr_ps(e, f, g, h)}; }
FORCEINLINE s32x8 S32x8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h) { return {_mm_castpi_ps(_mm_setr_epi32(a, b, c, d)), _mm_castpi_ps(_mm_setr_epi32(e, f, g, h))}; }
FORCEINLINE u32x8 U32x8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) { return {_mm_castpi_ps(_mm_setr_epi32((s32)a, (s32)b, (s32)c, (s32)d)), _mm_castpi_ps(_mm_setr_epi32((s32)e, (s32)f, (s32)g, (s32)h))}; }

FORCEINLINE f32x8 F32x8(f32x4 m0, f32x4 m1) { return {m0.m.ps, m1.m.ps}; }
FORCEINLINE s32x8 S32x8(s32x4 m0, s32x4 m1) { return {m0.m.ps, m1.m.ps}; }
FORCEINLINE u32x8 U32x8(u32x4 m0, u32x4 m1) { return {m0.m.ps, m1.m.ps}; }

FORCEINLINE f32x8 F32x8(f32 v) { return {_mm_set1_ps(v), _mm_set1_ps(v)}; }
FORCEINLINE s32x8 S32x8(s32 v) { return {_mm_castpi_ps(_mm_set1_epi32(v)), _mm_castpi_ps(_mm_set1_epi32(v))}; }
FORCEINLINE u32x8 U32x8(u32 v) { return {_mm_castpi_ps(_mm_set1_epi32((s32)v)), _mm_castpi_ps(_mm_set1_epi32((s32)v))}; }

FORCEINLINE f64x4 F64x4(f64 i, f64 j, f64 k, f64 l) { return {_mm_castpd_ps(_mm_setr_pd(i, j)), _mm_castpd_ps(_mm_setr_pd(k, l))}; }
#endif

#define X(f32, F32, p) \
template<> FORCEINLINE f32##x<p> F32##x<p> (f32 val){ return F32##x##p(val); } \

#define P(f32, F32) \
X(f32, F32, 4) \
X(f32, F32, 8) 

P(b32, B32)
P(f32, F32)
P(s32, S32)
P(u32, U32)

#undef P
#undef X

FORCEINLINE f32x4::operator s32x4() const { return {_mm_castpi_ps(_mm_cvtps_epi32(m.ps))}; }
FORCEINLINE f32x4::operator u32x4() const { return {_mm_castpi_ps(_mm_cvtps_epi32(m.ps))}; }
FORCEINLINE s32x4::operator f32x4() const { return {_mm_cvtepi32_ps(m.pi)}; }
FORCEINLINE s32x4::operator u32x4() const { return {m.ps}; }
FORCEINLINE u32x4::operator f32x4() const { return {_mm_setr_ps((f32)s[0], (f32)s[1], (f32)s[2], (f32)s[3])}; }
FORCEINLINE u32x4::operator s32x4() const { return {m.ps}; }
FORCEINLINE f64x4::operator u32x4() const { return {_mm_castpi_ps(_mm_setr_epi32((u32)s[0], (u32)s[1], (u32)s[2], (u32)s[3]))}; }

#if ARCH_AVX
FORCEINLINE f32x8::operator s32x8() const { return {_mm256_castpi_ps(_mm256_cvtps_epi32(m.ps))}; }
FORCEINLINE s32x8::operator f32x8() const { return {_mm256_cvtepi32_ps(m.pi)}; }
FORCEINLINE u32x4::operator f64x4() const { return {_mm256_castpd_ps(_mm256_setr_pd((f64)s[0], (f64)s[1], (f64)s[2], (f64)s[3]))}; }
FORCEINLINE f32x8::operator u32x8() const { return {_mm256_castpi_ps(_mm256_setr_epi32((u32)s[0], (u32)s[1], (u32)s[2], (u32)s[3], (u32)s[4], (u32)s[5], (u32)s[6], (u32)s[7]))}; }
FORCEINLINE s32x8::operator u32x8() const { return {m.ps}; }
FORCEINLINE u32x8::operator f32x8() const { return {_mm256_setr_ps((f32)s[0], (f32)s[1], (f32)s[2], (f32)s[3], (f32)s[4], (f32)s[5], (f32)s[6], (f32)s[7])}; }
FORCEINLINE u32x8::operator s32x8() const { return {m.ps}; }
#else
FORCEINLINE f32x8::operator s32x8() const { return {_mm_castpi_ps(_mm_cvtps_epi32(lo.ps)), _mm_castpi_ps(_mm_cvtps_epi32(hi.ps))}; }
FORCEINLINE s32x8::operator f32x8() const { return {_mm_cvtepi32_ps(lo.pi), _mm_cvtepi32_ps(hi.pi)}; }
FORCEINLINE u32x4::operator f64x4() const { return {_mm_castpd_ps(_mm_setr_pd((f64)s[0], (f64)s[1])), _mm_castpd_ps(_mm_setr_pd((f64)s[2], (f64)s[3]))}; }
FORCEINLINE f32x8::operator u32x8() const { return {_mm_castpi_ps(_mm_setr_epi32((u32)s[0], (u32)s[1], (u32)s[2], (u32)s[3])), _mm_castpi_ps(_mm_setr_epi32((u32)s[4], (u32)s[5], (u32)s[6], (u32)s[7]))}; }
FORCEINLINE s32x8::operator u32x8() const { return {lo.ps, hi.ps}; }
FORCEINLINE u32x8::operator f32x8() const { return {_mm_setr_ps((f32)s[0], (f32)s[1], (f32)s[2], (f32)s[3]), _mm_setr_ps((f32)s[4], (f32)s[5], (f32)s[6], (f32)s[7])}; }
FORCEINLINE u32x8::operator s32x8() const { return {lo.ps, hi.ps}; }
#endif

#if ARCH_AVX2
#define GATHER32x4(type) FORCEINLINE void gather(type &dst, void const *src, s32x4 offsets) { dst.m.ps = _mm_i32gather_ps((f32 const *)src, offsets.m.pi, 1); }
#define GATHER32x8(type) FORCEINLINE void gather(type &dst, void const *src, s32x8 offsets) { dst.m.ps = _mm256_i32gather_ps((f32 const *)src, offsets.m.pi, 1); }
#define GATHERMASK32x4(type) FORCEINLINE void gatherMask(type &dst, void const *src, s32x4 offsets, b32x4 mask, type def) { dst.m.ps = _mm_mask_i32gather_ps(def.m.ps, (f32*)src, offsets.m.pi, mask.m.ps, 1); }
#define GATHERMASK32x8(type) FORCEINLINE void gatherMask(type &dst, void const *src, s32x8 offsets, b32x8 mask, type def) { dst.m.ps = _mm256_mask_i32gather_ps(def.m.ps, (f32*)src, offsets.m.pi, mask.m.ps, 1); }
#else
#define GATHER32x4(type)												\
FORCEINLINE void gather(type &dst, void const *src, s32x4 offsets) {	\
	dst.m.ps = _mm_setr_ps(												\
		*(f32*)((u8 *)src + _mm_extract_epi32(offsets.m.pi, 0)),		\
		*(f32*)((u8 *)src + _mm_extract_epi32(offsets.m.pi, 1)),		\
		*(f32*)((u8 *)src + _mm_extract_epi32(offsets.m.pi, 2)),		\
		*(f32*)((u8 *)src + _mm_extract_epi32(offsets.m.pi, 3)));		\
}
#define GATHERMASK32x4(type)																											\
FORCEINLINE void gatherMask(type &dst, void const *src, s32x4 offsets, b32x4 mask, type def) {												\
	dst.m.ps = _mm_setr_ps(																												\
		_mm_extract_epi32(mask.m.pi, 0) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.m.pi, 0)) : _mm_extract_epi32(def.m.pi, 0),	\
		_mm_extract_epi32(mask.m.pi, 1) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.m.pi, 1)) : _mm_extract_epi32(def.m.pi, 1),	\
		_mm_extract_epi32(mask.m.pi, 2) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.m.pi, 2)) : _mm_extract_epi32(def.m.pi, 2),	\
		_mm_extract_epi32(mask.m.pi, 3) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.m.pi, 3)) : _mm_extract_epi32(def.m.pi, 3));	\
}
#if ARCH_AVX
#define GATHER32x8(type)												\
FORCEINLINE void gather(type &dst, void const *src, s32x8 offsets) {	\
	dst.m.ps = _mm256_setr_ps(											\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 0)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 1)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 2)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 3)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 4)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 5)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 6)),	\
		*(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 7)));	\
}
#define GATHERMASK32x8(type)																															\
FORCEINLINE void gatherMask(type &dst, void const *src, s32x8 offsets, b32x8 mask, type def) {																\
	f32 buf[8];																																			\
	_mm256_storeu_ps(buf, def.m.ps);																													\
	dst.m.ps = _mm256_setr_ps(																															\
		_mm256_extract_epi32(mask.m.pi, 0) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 0)) : buf[0],	\
		_mm256_extract_epi32(mask.m.pi, 1) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 1)) : buf[1],	\
		_mm256_extract_epi32(mask.m.pi, 2) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 2)) : buf[2],	\
		_mm256_extract_epi32(mask.m.pi, 3) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 3)) : buf[3],	\
		_mm256_extract_epi32(mask.m.pi, 4) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 4)) : buf[4],	\
		_mm256_extract_epi32(mask.m.pi, 5) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 5)) : buf[5],	\
		_mm256_extract_epi32(mask.m.pi, 6) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 6)) : buf[6],	\
		_mm256_extract_epi32(mask.m.pi, 7) ? *(f32*)((u8 *)(src) + _mm256_extract_epi32(offsets.m.pi, 7)) : buf[7]);\
}
#else
#define GATHER32x8(type)											\
FORCEINLINE void gather(type &dst, void const *src, s32x8 offsets) {\
	dst.lo.ps = _mm_setr_ps(										\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 0)),	\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 1)),	\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 2)),	\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 3)));\
	dst.hi.ps = _mm_setr_ps(										\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 0)),	\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 1)),	\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 2)),	\
		*(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 3)));\
}
#define GATHERMASK32x8(type)																												\
FORCEINLINE void gatherMask(type &dst, void const *src, s32x8 offsets, b32x8 mask, type def) {												\
	dst.lo.ps = _mm_setr_ps(																												\
		_mm_extract_epi32(mask.lo.pi, 0) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 0)) : _mm_extract_epi32(def.lo.pi, 0),	\
		_mm_extract_epi32(mask.lo.pi, 1) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 1)) : _mm_extract_epi32(def.lo.pi, 1),	\
		_mm_extract_epi32(mask.lo.pi, 2) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 2)) : _mm_extract_epi32(def.lo.pi, 2),	\
		_mm_extract_epi32(mask.lo.pi, 3) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.lo.pi, 3)) : _mm_extract_epi32(def.lo.pi, 3));	\
	dst.hi.ps = _mm_setr_ps(																												\
		_mm_extract_epi32(mask.hi.pi, 0) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 0)) : _mm_extract_epi32(def.hi.pi, 0),	\
		_mm_extract_epi32(mask.hi.pi, 1) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 1)) : _mm_extract_epi32(def.hi.pi, 1),	\
		_mm_extract_epi32(mask.hi.pi, 2) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 2)) : _mm_extract_epi32(def.hi.pi, 2),	\
		_mm_extract_epi32(mask.hi.pi, 3) ? *(f32*)((u8 *)(src) + _mm_extract_epi32(offsets.hi.pi, 3)) : _mm_extract_epi32(def.hi.pi, 3));	\
}
#endif
#endif


GATHER32x4(f32x4)
GATHER32x4(s32x4)
GATHER32x4(u32x4)
GATHER32x8(f32x8)
GATHER32x8(s32x8)
GATHER32x8(u32x8)

GATHERMASK32x4(f32x4)
GATHERMASK32x4(s32x4)
GATHERMASK32x4(u32x4)
GATHERMASK32x8(f32x8)
GATHERMASK32x8(s32x8)
GATHERMASK32x8(u32x8)

#undef GATHER32x4
#undef GATHER32x8

template <class T>
T load(void const *src) = delete;

template<> FORCEINLINE f32x4 load(void const *src) { return {_mm_loadu_ps((f32 const *)src)}; }
template<> FORCEINLINE s32x4 load(void const *src) { return {_mm_loadu_ps((f32 const *)src)}; }
template<> FORCEINLINE u32x4 load(void const *src) { return {_mm_loadu_ps((f32 const *)src)}; }
#if ARCH_AVX
template<> FORCEINLINE f32x8 load(void const *src) { return {_mm256_loadu_ps((f32 const *)src)}; }
template<> FORCEINLINE s32x8 load(void const *src) { return {_mm256_loadu_ps((f32 const *)src)}; }
template<> FORCEINLINE u32x8 load(void const *src) { return {_mm256_loadu_ps((f32 const *)src)}; }
#else
template<> FORCEINLINE f32x8 load(void const *src) { return {_mm_loadu_ps((f32 const *)src), _mm_loadu_ps((f32 *)src + 4)}; }
template<> FORCEINLINE s32x8 load(void const *src) { return {_mm_loadu_ps((f32 const *)src), _mm_loadu_ps((f32 *)src + 4)}; }
template<> FORCEINLINE u32x8 load(void const *src) { return {_mm_loadu_ps((f32 const *)src), _mm_loadu_ps((f32 *)src + 4)}; }
#endif

template <class T>
T loadMask(void const *src, b32x<widthOf<T>> mask) = delete;

#if ARCH_AVX
#define LOADMASK32x4(type) template<> FORCEINLINE type loadMask(void const *src, b32x<4> mask) { return {_mm_maskload_ps((f32 *)src, mask.m.pi)}; }
#define LOADMASK32x8(type) template<> FORCEINLINE type loadMask(void const *src, b32x<8> mask) { return {_mm256_maskload_ps((f32 *)src, mask.m.pi)}; }
#else
#define LOADMASK32x4(type)															\
template<> FORCEINLINE type loadMask(void const *src, b32x<4> mask) {				\
	return {_mm_setr_ps(_mm_extract_epi32(mask.m.pi, 0) ? ((f32 *)src)[0] : 0.0f,	\
						_mm_extract_epi32(mask.m.pi, 1) ? ((f32 *)src)[1] : 0.0f,	\
						_mm_extract_epi32(mask.m.pi, 2) ? ((f32 *)src)[2] : 0.0f,	\
						_mm_extract_epi32(mask.m.pi, 3) ? ((f32 *)src)[3] : 0.0f)};	\
}
#define LOADMASK32x8(type)															\
template<> FORCEINLINE type loadMask(void const *src, b32x<8> mask) {				\
	return {_mm_setr_ps(_mm_extract_epi32(mask.lo.pi, 0) ? ((f32 *)src)[0] : 0.0f,	\
						_mm_extract_epi32(mask.lo.pi, 1) ? ((f32 *)src)[1] : 0.0f,	\
						_mm_extract_epi32(mask.lo.pi, 2) ? ((f32 *)src)[2] : 0.0f,	\
						_mm_extract_epi32(mask.lo.pi, 3) ? ((f32 *)src)[3] : 0.0f),	\
			_mm_setr_ps(_mm_extract_epi32(mask.hi.pi, 0) ? ((f32 *)src)[4] : 0.0f,	\
						_mm_extract_epi32(mask.hi.pi, 1) ? ((f32 *)src)[5] : 0.0f,	\
						_mm_extract_epi32(mask.hi.pi, 2) ? ((f32 *)src)[6] : 0.0f,	\
						_mm_extract_epi32(mask.hi.pi, 3) ? ((f32 *)src)[7] : 0.0f)};\
}
#endif

LOADMASK32x4(f32x4)
LOADMASK32x4(s32x4)
LOADMASK32x4(u32x4)
LOADMASK32x8(f32x8)
LOADMASK32x8(s32x8)
LOADMASK32x8(u32x8)

#undef LOADMASK32x4
#undef LOADMASK32x8

#if ARCH_AVX
#define STOREMASK32x4(type) FORCEINLINE void storeMask(void *dst, type src, b32x<4> mask) { _mm_maskstore_ps((f32 *)dst, mask.m.pi, src.m.ps); }
#define STOREMASK32x8(type) FORCEINLINE void storeMask(void *dst, type src, b32x<8> mask) { _mm256_maskstore_ps((f32 *)dst, mask.m.pi, src.m.ps); }
#else
#define STOREMASK32x4(type)																\
FORCEINLINE void storeMask(void *dst, type src, b32x<4> mask) {							\
	if (_mm_extract_epi32(mask.m.pi, 0)) ((f32 *)dst)[0] = _mm_extract_ps(src.m.ps, 0);	\
	if (_mm_extract_epi32(mask.m.pi, 1)) ((f32 *)dst)[1] = _mm_extract_ps(src.m.ps, 1);	\
	if (_mm_extract_epi32(mask.m.pi, 2)) ((f32 *)dst)[2] = _mm_extract_ps(src.m.ps, 2);	\
	if (_mm_extract_epi32(mask.m.pi, 3)) ((f32 *)dst)[3] = _mm_extract_ps(src.m.ps, 3);	\
}
#define STOREMASK32x8(type)																	\
FORCEINLINE void storeMask(void *dst, type src, b32x<8> mask) {								\
	if (_mm_extract_epi32(mask.lo.pi, 0)) ((f32 *)dst)[0] = _mm_extract_ps(src.lo.ps, 0);	\
	if (_mm_extract_epi32(mask.lo.pi, 1)) ((f32 *)dst)[1] = _mm_extract_ps(src.lo.ps, 1);	\
	if (_mm_extract_epi32(mask.lo.pi, 2)) ((f32 *)dst)[2] = _mm_extract_ps(src.lo.ps, 2);	\
	if (_mm_extract_epi32(mask.lo.pi, 3)) ((f32 *)dst)[3] = _mm_extract_ps(src.lo.ps, 3);	\
	if (_mm_extract_epi32(mask.hi.pi, 0)) ((f32 *)dst)[4] = _mm_extract_ps(src.hi.ps, 0);	\
	if (_mm_extract_epi32(mask.hi.pi, 1)) ((f32 *)dst)[5] = _mm_extract_ps(src.hi.ps, 1);	\
	if (_mm_extract_epi32(mask.hi.pi, 2)) ((f32 *)dst)[6] = _mm_extract_ps(src.hi.ps, 2);	\
	if (_mm_extract_epi32(mask.hi.pi, 3)) ((f32 *)dst)[7] = _mm_extract_ps(src.hi.ps, 3);	\
}
#endif

STOREMASK32x4(f32x4)
STOREMASK32x4(s32x4)
STOREMASK32x4(u32x4)
STOREMASK32x8(f32x8)
STOREMASK32x8(s32x8)
STOREMASK32x8(u32x8)

#undef STOREMASK32x4
#undef STOREMASK32x8

template<u8 s0, u8 s1, u8 s2, u8 s3> FORCEINLINE f32x4 shuffle(f32x4 v) { return {_mm_shuffle_ps(v.m.ps, v.m.ps, _MM_SHUFFLE(s3, s2, s1, s0))}; }
template<u8 s0, u8 s1, u8 s2, u8 s3> FORCEINLINE s32x4 shuffle(s32x4 v) { return {_mm_shuffle_ps(v.m.ps, v.m.ps, _MM_SHUFFLE(s3, s2, s1, s0))}; }
template<u8 s0, u8 s1, u8 s2, u8 s3> FORCEINLINE u32x4 shuffle(u32x4 v) { return {_mm_shuffle_ps(v.m.ps, v.m.ps, _MM_SHUFFLE(s3, s2, s1, s0))}; }
//template<u8 s0, u8 s1, u8 s2, u8 s3, u8 s4, u8 s5, u8 s6, u8 s7> FORCEINLINE f32x8 shuffle(f32x8 v) { v.m = ASM::shuffle32(v.m, U32x8(s0,s1,s2,s3,s4,s5,s6,s7).m); return v; }
//template<u8 s0, u8 s1, u8 s2, u8 s3, u8 s4, u8 s5, u8 s6, u8 s7> FORCEINLINE s32x8 shuffle(s32x8 v) { v.m = ASM::shuffle32(v.m, U32x8(s0,s1,s2,s3,s4,s5,s6,s7).m); return v; }
//template<u8 s0, u8 s1, u8 s2, u8 s3, u8 s4, u8 s5, u8 s6, u8 s7> FORCEINLINE u32x8 shuffle(u32x8 v) { v.m = ASM::shuffle32(v.m, U32x8(s0,s1,s2,s3,s4,s5,s6,s7).m); return v; }

#if ARCH_AVX
#define SHUFFLE32x4(type) FORCEINLINE type shuffle(type v, s32x4 s) { return {_mm_permutevar_ps(v.m.ps, s.m.pi)}; }
#else
#define SHUFFLE32x4(type) \
FORCEINLINE type shuffle(type v, s32x4 s) {\
	__m128 idx = _mm_and_ps(s.m.ps, _mm_castpi_ps(_mm_set1_epi32(3)));\
	idx = _mm_castpi_ps(_mm_mullo_epi16(_mm_castps_pi(_mm_or_ps(idx, _mm_castpi_ps(_mm_slli_pi(_mm_castps_pi(idx), 2)))), _mm_set1_epi16(4)));\
	return {_mm_castpi_ps(_mm_shuffle_epi8(v.m.pi, _mm_add_epi8(_mm_castps_pi(_mm_or_ps(idx, _mm_castpi_ps(_mm_slli_pi(_mm_castps_pi(idx), 1)))), _mm_setr_epi8(0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3))))};\
}
#endif

#if ARCH_AVX2
#define SHUFFLE32x8(type) FORCEINLINE type shuffle(type v, s32x8 s) { return {_mm256_permutevar8x32_ps(v.m.ps, s.m.pi)}; }
#else
#define SHUFFLE32x8(type)
#endif

SHUFFLE32x4(f32x4)
SHUFFLE32x4(s32x4)
SHUFFLE32x4(u32x4)
SHUFFLE32x8(f32x8)
SHUFFLE32x8(s32x8)
SHUFFLE32x8(u32x8)

#undef SHUFFLE32x4
#undef SHUFFLE32x8

// clang-format on

#define UOP2(op, ty, con) \
	FORCEINLINE ty operator op() const { return {op x, op y}; }
#define UOP3(op, ty, con) \
	FORCEINLINE ty operator op() const { return {op x, op y, op z}; }
#define UOP4(op, ty, con) \
	FORCEINLINE ty operator op() const { return {op x, op y, op z, op w}; }
#define UOP4M(op, ty, con) \
	FORCEINLINE ty operator op() const { return con(op m); }

#define UOP2C(op, ty, con) FORCEINLINE constexpr UOP2(op, ty, con)
#define UOP3C(op, ty, con) FORCEINLINE constexpr UOP3(op, ty, con)
#define UOP4C(op, ty, con) FORCEINLINE constexpr UOP4(op, ty, con)

#define UOP2P(op, ty, con) UOP2(op, ty, con)
#define UOP3P(op, ty, con) UOP3(op, ty, con)
#define UOP4P(op, ty, con) UOP4(op, ty, con)

#define BOP2(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return {x op b.x, y op b.y}; }
#define BOP3(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return {x op b.x, y op b.y, z op b.z}; }
#define BOP4(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return {x op b.x, y op b.y, z op b.z, w op b.w}; }

#define BOP2P(op, ty, con) BOP2(op, ty, con)
#define BOP3P(op, ty, con) BOP3(op, ty, con)
#define BOP4P(op, ty, con) BOP4(op, ty, con)

#define BOP4M(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return con(m op b.m); }

#define BOP2C(op, ty, con) constexpr BOP2(op, ty, con)
#define BOP3C(op, ty, con) constexpr BOP3(op, ty, con)
#define BOP4C(op, ty, con) constexpr BOP4(op, ty, con)

#define BOP2CMP(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return {_mask<ty>(x op b.x), _mask<ty>(y op b.y)}; }
#define BOP3CMP(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return {_mask<ty>(x op b.x), _mask<ty>(y op b.y), _mask<ty>(z op b.z)}; }
#define BOP4CMP(op, ty, con)                                                                         \
	FORCEINLINE ty operator op(ty b) const {                                                                     \
		return {_mask<ty>(x op b.x), _mask<ty>(y op b.y), _mask<ty>(z op b.z), _mask<ty>(w op b.w)}; \
	}

#define BOP2PCMP(op, ty, con) BOP2P(op, ty, con) // mask operator op(ty b) const { return {x op b.x con y op b.y}; }
#define BOP3PCMP(op, ty, con) \
	BOP3P(op, ty, con) // mask operator op(ty b) const { return {x op b.x con y op b.y con z op b.z}; }
#define BOP4PCMP(op, ty, con) \
	BOP4P(op, ty, con) // mask operator op(ty b) const { return {x op b.x con y op b.y con z op b.z con w op b.w}; }

#define BOP4MCMP(op, ty, con) \
	FORCEINLINE ty operator op(ty b) const { return con(m op b.m); }

#define BOP2CCMP(op, ty, con) constexpr BOP2CMP(op, ty, con)
#define BOP3CCMP(op, ty, con) constexpr BOP3CMP(op, ty, con)
#define BOP4CCMP(op, ty, con) constexpr BOP4CMP(op, ty, con)

#define BOPSF32_(bop, uop, cmp, ty, con) uop(-, ty, con) bop(+, ty, con) bop(-, ty, con) bop(*, ty, con) bop(/, ty, con)
#define BOPSU32_(bop, uop, ty, con)                                                                                 \
	bop(+, ty, con) bop(-, ty, con) bop(*, ty, con) bop(/, ty, con) bop(%, ty, con) bop(^, ty, con) bop(|, ty, con) \
		bop(&, ty, con) bop(<<, ty, con) bop(>>, ty, con) uop(~, ty, con)
#define BOPSS32_(bop, uop, ty, con) BOPSU32_(bop, uop, ty, con) uop(-, ty, con)

#define BOPSF32(op, ty, con) BOPSF32_(CONCAT(B, op), CONCAT(U, op), CONCAT(CONCAT(B, op), CMP), ty, con)
#define BOPSS32(op, ty, con) BOPSS32_(CONCAT(B, op), CONCAT(U, op), ty, con)
#define BOPSU32(op, ty, con) BOPSU32_(CONCAT(B, op), CONCAT(U, op), ty, con)

#define VECCMP2(bool, v2) FORCEINLINE bool operator==(v2 b) const { return x == b.x && y == b.y; }
#define VECCMP3(bool, v3) FORCEINLINE bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; }
#define VECCMP4(bool, v4) FORCEINLINE bool operator==(v4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }

#define MEMBERS2(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                                                   \
		f32x4 x, y;                                                                                            \
	};                                                                                                         \
	fn(__VA_ARGS__) VECCMP2(bool, v2x4) CVT(v2s);                                                              \
	CVT(v2u);                                                                                                  \
	f32 s[(width)*2]

#define MEMBERS3(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                                                   \
		f32x4 x, y, z;                                                                                         \
	};                                                                                                         \
	v2x4 xy;                                                                                                   \
	struct {                                                                                                   \
		f32x4 _pad0;                                                                                           \
		v2x4 yz;                                                                                               \
	};                                                                                                         \
	fn(__VA_ARGS__) VECCMP3(bool, v3x4) CVT(v3s);                                                              \
	CVT(v3u);                                                                                                  \
	f32 s[(width)*3]

#define MEMBERS4(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                                                   \
		f32x4 x, y, z, w;                                                                                      \
	};                                                                                                         \
	struct {                                                                                                   \
		v2x4 xy;                                                                                               \
		v2x4 zw;                                                                                               \
	};                                                                                                         \
	v3x4 xyz;                                                                                                  \
	struct {                                                                                                   \
		f32x4 _pad0;                                                                                           \
		union {                                                                                                \
			v2x4 yz;                                                                                           \
			v3x4 yzw;                                                                                          \
		};                                                                                                     \
	};                                                                                                         \
	fn(__VA_ARGS__) VECCMP4(bool, v4x4) CVT(v4s);                                                              \
	CVT(v4u);                                                          \
	f32 s[(width)*4]

#define MEMBERS2S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS2(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	FORCEINLINE v2 operator[](umm i) const { return this[i]; }                                                      \
	FORCEINLINE v2 &operator[](umm i) { return this[i]; }
#define MEMBERS3S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS3(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	FORCEINLINE v3 operator[](umm i) const { return this[i]; }                                                      \
	FORCEINLINE v3 &operator[](umm i) { return this[i]; }
#define MEMBERS4S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS4(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	FORCEINLINE v4 operator[](umm i) const { return this[i]; }                                                      \
	FORCEINLINE v4 &operator[](umm i) { return this[i]; }

#define MEMBERS2P(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS2(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	f32x4 p[2];                                                                                                     \
	v2 u[width];                                                                                                    \
	FORCEINLINE v2 operator[](umm i) const { return u[i]; }                                                         \
	FORCEINLINE v2 &operator[](umm i) { return u[i]; }
#define MEMBERS3P(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS3(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	f32x4 p[3];                                                                                                     \
	v3 u[width];                                                                                                    \
	FORCEINLINE v3 operator[](umm i) const { return u[i]; }                                                         \
	FORCEINLINE v3 &operator[](umm i) { return u[i]; }
#define MEMBERS4P(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS4(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	f32x4 p[4];                                                                                                     \
	v4 u[width];                                                                                                    \
	FORCEINLINE v4 operator[](umm i) const { return u[i]; }                                                         \
	FORCEINLINE v4 &operator[](umm i) { return u[i]; }

// clang-format off
#define IMPL_v2f32x1() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v2f, f32, V2f) BOPSF32(OP2C, v2f, V2f)
#define IMPL_v3f32x1() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v3f, f32, V3f) BOPSF32(OP3C, v3f, V3f)
#define IMPL_v4f32x1() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v4f, f32, V4f) BOPSF32(OP4M, v4f, V4f) f32x4 m;
#define IMPL_v2s32x1() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v2s, s32, V2s) MEMFUNS_INT(v2s, s32, V2s) MEMFUNS_SHIFT_SCL(v2s, s32, V2s) BOPSS32(OP2C, v2s, V2s)
#define IMPL_v3s32x1() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v3s, s32, V3s) MEMFUNS_INT(v3s, s32, V3s) MEMFUNS_SHIFT_SCL(v3s, s32, V3s) BOPSS32(OP3C, v3s, V3s)
#define IMPL_v4s32x1() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v4s, s32, V4s) MEMFUNS_INT(v4s, s32, V4s) MEMFUNS_SHIFT_SCL_M(v4s, s32, V4s) BOPSS32(OP4M, v4s, V4s); s32x4 m;
#define IMPL_v2u32x1() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v2u, u32, V2u) MEMFUNS_INT(v2u, u32, V2u) MEMFUNS_SHIFT_SCL(v2u, u32, V2u) BOPSU32(OP2C, v2u, V2u)
#define IMPL_v3u32x1() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v3u, u32, V3u) MEMFUNS_INT(v3u, u32, V3u) MEMFUNS_SHIFT_SCL(v3u, u32, V3u) BOPSU32(OP3C, v3u, V3u)
#define IMPL_v4u32x1() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v4u, u32, V4u) MEMFUNS_INT(v4u, u32, V4u) MEMFUNS_SHIFT_SCL_M(v4u, u32, V4u) BOPSU32(OP4M, v4u, V4u); u32x4 m;

#define IMPL_v2f32x4() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v2fx4, f32, V2fx4) MEMFUNS_BASIC_SCL(v2fx4, f32x4, V2fx4) BOPSF32(OP2P, v2fx4, V2fx4)
#define IMPL_v3f32x4() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v3fx4, f32, V3fx4) MEMFUNS_BASIC_SCL(v3fx4, f32x4, V3fx4) BOPSF32(OP3P, v3fx4, V3fx4)
#define IMPL_v4f32x4() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v4fx4, f32, V4fx4) MEMFUNS_BASIC_SCL(v4fx4, f32x4, V4fx4) BOPSF32(OP4P, v4fx4, V4fx4)
#define IMPL_v2s32x4() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v2sx4, s32, V2sx4) MEMFUNS_INT(v2sx4, s32, V2sx4) BOPSS32(OP2P, v2sx4, V2sx4) MEMFUNS_SHIFT_SCL_2(v2sx4, s32)
#define IMPL_v3s32x4() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v3sx4, s32, V3sx4) MEMFUNS_INT(v3sx4, s32, V3sx4) BOPSS32(OP3P, v3sx4, V3sx4) MEMFUNS_SHIFT_SCL_3(v3sx4, s32)
#define IMPL_v4s32x4() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v4sx4, s32, V4sx4) MEMFUNS_INT(v4sx4, s32, V4sx4) BOPSS32(OP4P, v4sx4, V4sx4) MEMFUNS_SHIFT_SCL_4(v4sx4, s32)
#define IMPL_v2u32x4() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v2ux4, u32, V2ux4) MEMFUNS_INT(v2ux4, u32, V2ux4) BOPSU32(OP2P, v2ux4, V2ux4) MEMFUNS_SHIFT_SCL_2(v2ux4, u32)
#define IMPL_v3u32x4() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v3ux4, u32, V3ux4) MEMFUNS_INT(v3ux4, u32, V3ux4) BOPSU32(OP3P, v3ux4, V3ux4) MEMFUNS_SHIFT_SCL_3(v3ux4, u32)
#define IMPL_v4u32x4() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v4ux4, u32, V4ux4) MEMFUNS_INT(v4ux4, u32, V4ux4) BOPSU32(OP4P, v4ux4, V4ux4) MEMFUNS_SHIFT_SCL_4(v4ux4, u32)

#define IMPL_v2f32x8() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v2fx8, f32, V2fx8) MEMFUNS_BASIC_SCL(v2fx8, f32x8, V2fx8) BOPSF32(OP2P, v2fx8, V2fx8);
#define IMPL_v3f32x8() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v3fx8, f32, V3fx8) MEMFUNS_BASIC_SCL(v3fx8, f32x8, V3fx8) BOPSF32(OP3P, v3fx8, V3fx8);
#define IMPL_v4f32x8() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v4fx8, f32, V4fx8) MEMFUNS_BASIC_SCL(v4fx8, f32x8, V4fx8) BOPSF32(OP4P, v4fx8, V4fx8);
#define IMPL_v2s32x8() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v2sx8, s32, V2sx8) MEMFUNS_INT(v2sx8, s32, V2sx8)
#define IMPL_v3s32x8() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v3sx8, s32, V3sx8) MEMFUNS_INT(v3sx8, s32, V3sx8)
#define IMPL_v4s32x8() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v4sx8, s32, V4sx8) MEMFUNS_INT(v4sx8, s32, V4sx8)
#define IMPL_v2u32x8() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v2ux8, u32, V2ux8) MEMFUNS_INT(v2ux8, u32, V2ux8)
#define IMPL_v3u32x8() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v3ux8, u32, V3ux8) MEMFUNS_INT(v3ux8, u32, V3ux8)
#define IMPL_v4u32x8() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v4ux8, u32, V4ux8) MEMFUNS_INT(v4ux8, u32, V4ux8)

#define TYPES_f32x1(fn, packSize, dim) fn(packSize,  bool, f32, v2f, v3f, v4f,   f32,   v2f,   v3f,   v4f,   v2s,   v3s,   v4s,   v2u,   v3u,   v4u, IMPL_v##dim##f32x1)
#define TYPES_s32x1(fn, packSize, dim) fn(packSize,  bool, s32, v2s, v3s, v4s,   s32,   v2s,   v3s,   v4s,   v2f,   v3f,   v4f,   v2u,   v3u,   v4u, IMPL_v##dim##s32x1)
#define TYPES_u32x1(fn, packSize, dim) fn(packSize,  bool, u32, v2u, v3u, v4u,   u32,   v2u,   v3u,   v4u,   v2f,   v3f,   v4f,   v2s,   v3s,   v4s, IMPL_v##dim##u32x1)
#define TYPES_f32x4(fn, packSize, dim) fn(packSize, b32x4, f32, v2f, v3f, v4f, f32x4, v2fx4, v3fx4, v4fx4, v2sx4, v3sx4, v4sx4, v2ux4, v3ux4, v4ux4, IMPL_v##dim##f32x4)
#define TYPES_s32x4(fn, packSize, dim) fn(packSize, b32x4, s32, v2s, v3s, v4s, s32x4, v2sx4, v3sx4, v4sx4, v2fx4, v3fx4, v4fx4, v2ux4, v3ux4, v4ux4, IMPL_v##dim##s32x4)
#define TYPES_u32x4(fn, packSize, dim) fn(packSize, b32x4, u32, v2u, v3u, v4u, u32x4, v2ux4, v3ux4, v4ux4, v2fx4, v3fx4, v4fx4, v2sx4, v3sx4, v4sx4, IMPL_v##dim##u32x4)
#define TYPES_f32x8(fn, packSize, dim) fn(packSize, b32x8, f32, v2f, v3f, v4f, f32x8, v2fx8, v3fx8, v4fx8, v2sx8, v3sx8, v4sx8, v2ux8, v3ux8, v4ux8, IMPL_v##dim##f32x8)
#define TYPES_s32x8(fn, packSize, dim) fn(packSize, b32x8, s32, v2s, v3s, v4s, s32x8, v2sx8, v3sx8, v4sx8, v2fx8, v3fx8, v4fx8, v2ux8, v3ux8, v4ux8, IMPL_v##dim##s32x8)
#define TYPES_u32x8(fn, packSize, dim) fn(packSize, b32x8, u32, v2u, v3u, v4u, u32x8, v2ux8, v3ux8, v4ux8, v2fx8, v3fx8, v4fx8, v2sx8, v3sx8, v4sx8, IMPL_v##dim##u32x8)

#define VECIMPL(dim, SorP, packSize, type) TYPES_##type##x##packSize(MEMBERS##dim##SorP, packSize, dim)


template<> union v2fx<1> { VECIMPL(2, S, 1, f32); };
template<> union v3fx<1> { VECIMPL(3, S, 1, f32); };
template<> union v4fx<1> { VECIMPL(4, S, 1, f32); };

template<> union v2sx<1> { VECIMPL(2, S, 1, s32); };
template<> union v3sx<1> { VECIMPL(3, S, 1, s32); };
#if 1
template<> union v4sx<1> { VECIMPL(4, S, 1, s32); };
#else
#define MEMBERS4(f32, v2, v3) \
	struct {                  \
		f32 x, y, z, w;       \
	};                        \
	struct {                  \
		v2 xy;                \
		v2 zw;                \
	};                        \
	v3 xyz;                   \
	struct {                  \
		f32 _;                \
		union {               \
			v2 yz;            \
			v3 yzw;           \
		};                    \
	};
#define DATA(f32, count)		\
	f32 s[count];				\
	f32* data() { return s; }	\
	f32 const* data() const { return s; }
#define OP(op) \
	v4s operator op(v4s) const; \
	v4s operator op(s32) const;\
	v4s &operator op##=(v4s); \
	v4s &operator op##=(s32);
union v4s { 
	MEMBERS4(s32, v2s, v3s) DATA(s32, 4) CVT(v4f); CVT(v4u)
	s32x4 m; 
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&)
	bool operator==(v4s) const; 

	s32 operator[](umm i) const { return data()[i]; }
	s32& operator[](umm i) { return data()[i]; }
};
#undef OP
#define OP(op) \
	v4s v4s::operator op(v4s b) const { return {x op b.x, y op b.y, z op b.z, w op b.w}; } \
	v4s v4s::operator op(s32 b) const { return {x op b  , y op b  , z op b  , w op b  }; } \
	v4s &v4s::operator op##=(v4s b) { return x op##= b.x, y op##= b.y, z op##= b.z, w op##= b.w, *this; } \
	v4s &v4s::operator op##=(s32 b) { return x op##= b  , y op##= b  , z op##= b  , w op##= b  , *this; }
OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&)
#undef OP
bool v4s::operator==(v4s b) const { return allTrue(m == b.m); } 
#undef DATA
#undef MEMBERS4
#endif
template<> union v2ux<1> { VECIMPL(2, S, 1, u32); };
template<> union v3ux<1> { VECIMPL(3, S, 1, u32); };
template<> union v4ux<1> { VECIMPL(4, S, 1, u32); };

template<> union v2fx<4> { VECIMPL(2, P, 4, f32); };
template<> union v3fx<4> { VECIMPL(3, P, 4, f32); };
template<> union v4fx<4> { VECIMPL(4, P, 4, f32); };

template<> union v2sx<4> { VECIMPL(2, P, 4, s32); };
template<> union v3sx<4> { VECIMPL(3, P, 4, s32); };
template<> union v4sx<4> { VECIMPL(4, P, 4, s32); };

template<> union v2ux<4> { VECIMPL(2, P, 4, u32); };
template<> union v3ux<4> { VECIMPL(3, P, 4, u32); };
template<> union v4ux<4> { VECIMPL(4, P, 4, u32); };

template<> union v2fx<8> { VECIMPL(2, P, 8, f32); };
template<> union v3fx<8> { VECIMPL(3, P, 8, f32); };
template<> union v4fx<8> { VECIMPL(4, P, 8, f32); };
// clang-format on

template<>
union v2sx<8> {
	VECIMPL(2, P, 8, s32);
	BOPSS32(OP2P, v2sx8, V2sx8);
	FORCEINLINE v2sx8 operator<<(s32 b) const { return {x << b, y << b}; }
	FORCEINLINE v2sx8 operator>>(s32 b) const { return {x >> b, y >> b}; }
};
template<>
union v3sx<8> {
	VECIMPL(3, P, 8, s32);
	BOPSS32(OP3P, v3sx8, V3sx8);
	FORCEINLINE v3sx8 operator<<(s32 b) const { return {x << b, y << b, z << b}; }
	FORCEINLINE v3sx8 operator>>(s32 b) const { return {x >> b, y >> b, z >> b}; }
};
template<>
union v4sx<8> {
	VECIMPL(4, P, 8, s32);
	BOPSS32(OP4P, v4sx8, V4sx8);
	FORCEINLINE v4sx8 operator<<(s32 b) const { return {x << b, y << b, z << b, w << b}; }
	FORCEINLINE v4sx8 operator>>(s32 b) const { return {x >> b, y >> b, z >> b, w >> b}; }
};

template<>
union v2ux<8> {
	VECIMPL(2, P, 8, u32);
	BOPSU32(OP2P, v2ux8, V2ux8);
	FORCEINLINE v2ux8 operator<<(u32 b) const { return {x << b, y << b}; }
	FORCEINLINE v2ux8 operator>>(u32 b) const { return {x >> b, y >> b}; }
};
template<>
union v3ux<8> {
	VECIMPL(3, P, 8, u32);
	BOPSU32(OP3P, v3ux8, V3ux8);
	FORCEINLINE v3ux8 operator<<(u32 b) const { return {x << b, y << b, z << b}; }
	FORCEINLINE v3ux8 operator>>(u32 b) const { return {x >> b, y >> b, z >> b}; }
};
template<>
union v4ux<8> {
	VECIMPL(4, P, 8, u32);
	BOPSU32(OP4P, v4ux8, V4ux8);
	FORCEINLINE v4ux8 operator<<(u32 b) const { return {x << b, y << b, z << b, w << b}; }
	FORCEINLINE v4ux8 operator>>(u32 b) const { return {x >> b, y >> b, z >> b, w >> b}; }
};

union m3;
FORCEINLINE m3 transpose(m3 const&);

union m3 {
	struct {
		v3f i, j, k;
	};
	f32 s[9];
	FORCEINLINE v3f operator*(v3f b) const {
		v3f x = V3f(b.x) * i;
		v3f y = V3f(b.y) * j;
		v3f z = V3f(b.z) * k;
		return x + y + z;
	}
	FORCEINLINE m3 operator*(m3 b) const { return {*this * b.i, *this * b.j, *this * b.k}; }
	FORCEINLINE m3& operator*=(m3 b) { return *this = *this * b; }
	static FORCEINLINE m3 scaling(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0,
			0, y, 0,
			0, 0, z,
		};
		// clang-format on
	}
	static FORCEINLINE m3 scaling(v3f v) { return scaling(v.x, v.y, v.z); }
	static FORCEINLINE m3 scaling(v2f xy, f32 z) { return scaling(xy.x, xy.y, z); }
	static FORCEINLINE m3 scaling(f32 v) { return scaling(v, v, v); }
	static FORCEINLINE m3 translation(f32 x, f32 y) {
		// clang-format off
		return {
			1, 0, 0,
			0, 1, 0,
			x, y, 1,
		};
		// clang-format on
	}
	static FORCEINLINE m3 translation(v2f v) { return translation(v.x, v.y); }
	static FORCEINLINE m3 translation(f32 v) { return translation(v, v); }
	static FORCEINLINE m3 rotationX(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			1, 0, 0,
			0, c, s,
			0,-s, c,
		};
		// clang-format on
	}
	static FORCEINLINE m3 rotationY(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, 0, s,
			0, 1, 0,
		   -s, 0, c,
		};
		// clang-format on
	}
	static FORCEINLINE m3 rotationZ(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, s, 0,
		   -s, c, 0,
			0, 0, 1,
		};
		// clang-format on
	}
};

FORCEINLINE m3 transpose(m3 const& m) {
	return {
		m.i.x, m.j.x, m.k.x, m.i.y, m.j.y, m.k.y, m.i.z, m.j.z, m.k.z,
	};
}

union m4 {
	struct {
		v4f i, j, k, l;
	};
	__m128 m[4];
	f32 s[16];
	FORCEINLINE v4f operator*(v4f b) const {
		v4f x = V4f(b.x) * i;
		v4f y = V4f(b.y) * j;
		v4f z = V4f(b.z) * k;
		v4f w = V4f(b.w) * l;
		return x + y + z + w;
	}
	FORCEINLINE v3f operator*(v3f b) const {
		auto x = V4f(b.x) * i;
		auto y = V4f(b.y) * j;
		auto z = V4f(b.z) * k;
		auto r = x + y + z;
		v3f result;
		memcpy(&result, &r, 12);
		return result;
	}
	FORCEINLINE m4 operator*(m4 b) const { return {*this * b.i, *this * b.j, *this * b.k, *this * b.l}; }
	FORCEINLINE m4& operator*=(m4 b) { return *this = *this * b; }
	static FORCEINLINE m4 scaling(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static FORCEINLINE m4 identity() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }
	static FORCEINLINE m4 scaling(v3f v) { return scaling(v.x, v.y, v.z); }
	static FORCEINLINE m4 scaling(v2f xy, f32 z) { return scaling(xy.x, xy.y, z); }
	static FORCEINLINE m4 scaling(f32 v) { return scaling(v, v, v); }
	static FORCEINLINE m4 translation(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1,
		};
		// clang-format on
	}
	static FORCEINLINE m4 translation(v3f v) { return translation(v.x, v.y, v.z); }
	static FORCEINLINE m4 translation(v2f xy, f32 z) { return translation(xy.x, xy.y, z); }
	static FORCEINLINE m4 translation(f32 v) { return translation(v, v, v); }
	static FORCEINLINE m4 perspective(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 h	   = 1.0f / tanf(fov * 0.5f);
		f32 w	   = h / aspect;
		f32 fzdfmn = fz / (fz - nz);
		// clang-format off
		return {
			w, 0, 0, 0, 
			0, h, 0, 0, 
			0, 0, fzdfmn, 1, 
			0, 0, -fzdfmn * nz, 0
		};
		// clang-format on
	}
	static FORCEINLINE m4 rotationX(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			1, 0, 0, 0,
			0, c, s, 0,
			0,-s, c, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static FORCEINLINE m4 rotationY(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, 0, s, 0,
			0, 1, 0, 0,
		   -s, 0, c, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static FORCEINLINE m4 rotationZ(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, s, 0, 0,
		   -s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	// Roll, Pitch, Yaw (ZXY)
	static FORCEINLINE m4 rotationZXY(v3f v) {
		v3f sin;
		v3f cos;
		sincos(v, sin, cos);
		f32 a = sin.x;
		f32 c = sin.y;
		f32 e = sin.z;
		f32 b = cos.x;
		f32 d = cos.y;
		f32 f = cos.z;
		// clang-format off
		return {
			d * f - a * c * e,  b * e, c * f + a * d * e, 0,
			-a * c * f - d * e, b * f, a * d * f - c * e, 0,
			-b * c,             -a,    b * d,             0,
			0,                  0,     0,                 1,
		};
		// clang-format on
	}
	// Yaw, Pitch, Roll (YXZ)
	static FORCEINLINE m4 rotationYXZ(v3f v) {
		v3f sin;
		v3f cos;
		sincos(v, sin, cos);
		f32 a = sin.x;
		f32 c = sin.y;
		f32 e = sin.z;
		f32 b = cos.x;
		f32 d = cos.y;
		f32 f = cos.z;
		// clang-format off
		return {
			d * f + a * c * e,  -a * c * f + d * e, b * c, 0, 
			-b * e,             b * f,              a,     0,
			-c * f + a * d * e, -a * d * f - c * e, b * d, 0, 
			0,                  0,                  0,     1,
		};
		// clang-format on
	}
};

#undef CVT

#define V2(f32, v2f, V2f)                                     \
	FORCEINLINE constexpr v2f V2f(f32 x, f32 y) { return {x, y}; } \
	FORCEINLINE constexpr v2f V2f(f32 v) { return {v, v}; }
V2(f32, v2f, V2f);
V2(s32, v2s, V2s);
V2(u32, v2u, V2u);
#undef V2

#define V3(f32, v2f, v3f, V3f)                                          \
	FORCEINLINE constexpr v3f V3f(f32 x, f32 y, f32 z) { return {x, y, z}; } \
	FORCEINLINE constexpr v3f V3f(f32 v) { return {v, v, v}; }               \
	FORCEINLINE constexpr v3f V3f(v2f xy, f32 z) { return {xy.x, xy.y, z}; } \
	FORCEINLINE constexpr v3f V3f(f32 x, v2f yz) { return {x, yz.x, yz.y}; }
V3(f32, v2f, v3f, V3f);
V3(s32, v2s, v3s, V3s);
V3(u32, v2u, v3u, V3u);
#undef V3

#define V4(f32, v2f, v3f, v4f, V4f, f32x4)                                        \
	FORCEINLINE constexpr v4f V4f(f32 x, f32 y, f32 z, f32 w) { return {x, y, z, w}; } \
	FORCEINLINE constexpr v4f V4f(f32 v) { return {v, v, v, v}; }                      \
	FORCEINLINE constexpr v4f V4f(v2f xy, v2f zw) { return {xy.x, xy.y, zw.x, zw.y}; } \
	FORCEINLINE constexpr v4f V4f(v2f xy, f32 z, f32 w) { return {xy.x, xy.y, z, w}; } \
	FORCEINLINE constexpr v4f V4f(f32 x, f32 y, v2f zw) { return {x, y, zw.x, zw.y}; } \
	FORCEINLINE constexpr v4f V4f(v3f xyz, f32 w) { return {xyz.x, xyz.y, xyz.z, w}; } \
	FORCEINLINE constexpr v4f V4f(f32 x, v3f yzw) { return {x, yzw.x, yzw.y, yzw.z}; } \
	FORCEINLINE constexpr v4f V4f(f32x4 v) {                                           \
		v4f r{};                                                                  \
		return r.m = v, r;                                                        \
	}
V4(f32, v2f, v3f, v4f, V4f, f32x4);
V4(s32, v2s, v3s, v4s, V4s, s32x4);
V4(u32, v2u, v3u, v4u, V4u, u32x4);
#undef V4

// clang-format off
#define C(v4f, V4f) FORCEINLINE constexpr v4f V4f(v4f v) { return v; }
C(v2f, V2f) C(v2s, V2s) C(v2u, V2u);
C(v3f, V3f) C(v3s, V3s) C(v3u, V3u);
C(v4f, V4f) C(v4s, V4s) C(v4u, V4u);
C(v2fx4, V2fx4) C(v2sx4, V2sx4) C(v2ux4, V2ux4);
C(v3fx4, V3fx4) C(v3sx4, V3sx4) C(v3ux4, V3ux4);
C(v4fx4, V4fx4) C(v4sx4, V4sx4) C(v4ux4, V4ux4);
C(v2fx8, V2fx8) C(v2sx8, V2sx8) C(v2ux8, V2ux8);
C(v3fx8, V3fx8) C(v3sx8, V3sx8) C(v3ux8, V3ux8);
C(v4fx8, V4fx8) C(v4sx8, V4sx8) C(v4ux8, V4ux8);
#undef C
// clang-format on

#define V2x4(v2fx4, V2fx4, v2f, f32, f32x4, F32x4)                                            \
	FORCEINLINE v2fx4 V2fx4(f32 ix, f32 iy, f32 jx, f32 jy, f32 kx, f32 ky, f32 lx, f32 ly) { \
		return {                                                                              \
			F32x4(ix, jx, kx, lx),                                                            \
			F32x4(iy, jy, ky, ly),                                                            \
		};                                                                                    \
	}                                                                                         \
	FORCEINLINE v2fx4 V2fx4(v2f i, v2f j, v2f k, v2f l) {                                     \
		return {                                                                              \
			F32x4(i.x, j.x, k.x, l.x),                                                        \
			F32x4(i.y, j.y, k.y, l.y),                                                        \
		};                                                                                    \
	}                                                                                         \
	FORCEINLINE constexpr v2fx4 V2fx4(f32x4 x, f32x4 y) {                                     \
		v2fx4 r{};                                                                            \
		r.x = x;                                                                              \
		r.y = y;                                                                              \
		return r;                                                                             \
	}                                                                                         \
	FORCEINLINE constexpr v2fx4 V2fx4(M16 x, M16 y) {                                       \
		v2fx4 r{};                                                                            \
		r.x.m = x;                                                                            \
		r.y.m = y;                                                                            \
		return r;                                                                             \
	}                                                                                         \
	FORCEINLINE v2fx4 V2fx4(f32 x, f32 y) { return V2fx4(F32x4(x), F32x4(y)); }               \
	FORCEINLINE v2fx4 V2fx4(f32 v) { return V2fx4(F32x4(v), F32x4(v)); }                      \
	FORCEINLINE constexpr v2fx4 V2fx4(f32x4 v) { return V2fx4(v, v); }                        \
	FORCEINLINE v2fx4 V2fx4(v2f v) { return V2fx4(F32x4(v.x), F32x4(v.y)); }

V2x4(v2fx4, V2fx4, v2f, f32, f32x4, F32x4);
V2x4(v2sx4, V2sx4, v2s, s32, s32x4, S32x4);
V2x4(v2ux4, V2ux4, v2u, u32, u32x4, U32x4);
#undef V2x4

#define V3x4(v3fx4, V3fx4, v3f, f32, f32x4, F32x4)                                                                  \
	FORCEINLINE v3fx4 V3fx4(f32 ix, f32 iy, f32 iz, f32 jx, f32 jy, f32 jz, f32 kx, f32 ky, f32 kz, f32 lx, f32 ly, \
							f32 lz) {                                                                               \
		return {                                                                                                    \
			F32x4(ix, jx, kx, lx),                                                                                  \
			F32x4(iy, jy, ky, ly),                                                                                  \
			F32x4(iz, jz, kz, lz),                                                                                  \
		};                                                                                                          \
	}                                                                                                               \
	FORCEINLINE v3fx4 V3fx4(v3f i, v3f j, v3f k, v3f l) {                                                           \
		return {                                                                                                    \
			F32x4(i.x, j.x, k.x, l.x),                                                                              \
			F32x4(i.y, j.y, k.y, l.y),                                                                              \
			F32x4(i.z, j.z, k.z, l.z),                                                                              \
		};                                                                                                          \
	}                                                                                                               \
	FORCEINLINE constexpr v3fx4 V3fx4(f32x4 x, f32x4 y, f32x4 z) {                                                  \
		v3fx4 r{};                                                                                                  \
		r.x = x;                                                                                                    \
		r.y = y;                                                                                                    \
		r.z = z;                                                                                                    \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE constexpr v3fx4 V3fx4(M16 x, M16 y, M16 z) {                                                     \
		v3fx4 r{};                                                                                                  \
		r.x.m = x;                                                                                                  \
		r.y.m = y;                                                                                                  \
		r.z.m = z;                                                                                                  \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE v3fx4 V3fx4(f32 x, f32 y, f32 z) { return {F32x4(x), F32x4(y), F32x4(z)}; }                         \
	FORCEINLINE v3fx4 V3fx4(f32 v) { return V3fx4(v, v, v); }                                                       \
	FORCEINLINE constexpr v3fx4 V3fx4(f32x4 v) { return {v, v, v}; }                                                \
	FORCEINLINE v3fx4 V3fx4(v3f v) { return {F32x4(v.x), F32x4(v.y), F32x4(v.z)}; }

V3x4(v3fx4, V3fx4, v3f, f32, f32x4, F32x4);
V3x4(v3sx4, V3sx4, v3s, s32, s32x4, S32x4);
V3x4(v3ux4, V3ux4, v3u, u32, u32x4, U32x4);
#undef V3x4

#define V4x4(v4fx4, V4fx4, v4f, f32, f32x4, F32x4)                                                                  \
	FORCEINLINE v4fx4 V4fx4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, \
							f32 kw, f32 lx, f32 ly, f32 lz, f32 lw) {                                               \
		return {F32x4(ix, jx, kx, lx), F32x4(iy, jy, ky, ly), F32x4(iz, jz, kz, lz), F32x4(iw, jw, kw, lw)};        \
	}                                                                                                               \
	FORCEINLINE v4fx4 V4fx4(v4f i, v4f j, v4f k, v4f l) {                                                           \
		return {F32x4(i.x, j.x, k.x, l.x), F32x4(i.y, j.y, k.y, l.y), F32x4(i.z, j.z, k.z, l.z),                    \
				F32x4(i.w, j.w, k.w, l.w)};                                                                         \
	}                                                                                                               \
	FORCEINLINE constexpr v4fx4 V4fx4(f32x4 x, f32x4 y, f32x4 z, f32x4 w) {                                         \
		v4fx4 r{};                                                                                                  \
		r.x = x;                                                                                                    \
		r.y = y;                                                                                                    \
		r.z = z;                                                                                                    \
		r.w = w;                                                                                                    \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE constexpr v4fx4 V4fx4(M16 x, M16 y, M16 z, M16 w) {                                             \
		v4fx4 r{};                                                                                                  \
		r.x.m = x;                                                                                                  \
		r.y.m = y;                                                                                                  \
		r.z.m = z;                                                                                                  \
		r.w.m = w;                                                                                                  \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE v4fx4 V4fx4(f32 x, f32 y, f32 z, f32 w) { return {F32x4(x), F32x4(y), F32x4(z), F32x4(w)}; }        \
	FORCEINLINE v4fx4 V4fx4(f32 v) { return V4fx4(v, v, v, v); }                                                    \
	FORCEINLINE constexpr v4fx4 V4fx4(f32x4 v) { return {v, v, v, v}; }                                             \
	FORCEINLINE v4fx4 V4fx4(v4f v) { return {F32x4(v.x), F32x4(v.y), F32x4(v.z), F32x4(v.w)}; }

V4x4(v4fx4, V4fx4, v4f, f32, f32x4, F32x4);
V4x4(v4sx4, V4sx4, v4s, s32, s32x4, S32x4);
V4x4(v4ux4, V4ux4, v4u, u32, u32x4, U32x4);
#undef V4x4

#define V2x8(v2fx8, V2fx8, v2f, f32, f32x8, F32x8)                                                                  \
	FORCEINLINE constexpr v2fx8 V2fx8(f32x8 x, f32x8 y) {                                                           \
		v2fx8 r{};                                                                                                  \
		r.x = x;                                                                                                    \
		r.y = y;                                                                                                    \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE v2fx8 V2fx8(f32 x, f32 y) { return {F32x8(x), F32x8(y)}; }                                          \
	FORCEINLINE v2fx8 V2fx8(f32 ix, f32 iy, f32 jx, f32 jy, f32 kx, f32 ky, f32 lx, f32 ly, f32 ax, f32 ay, f32 bx, \
							f32 by, f32 cx, f32 cy, f32 dx, f32 dy) {                                               \
		return V2fx8(F32x8(ix, jx, kx, lx, ax, bx, cx, dx), F32x8(iy, jy, ky, ly, ay, by, cy, dy));                 \
	}                                                                                                               \
	FORCEINLINE v2fx8 V2fx8(v2f i, v2f j, v2f k, v2f l, v2f a, v2f b, v2f c, v2f d) {                               \
		return V2fx8(F32x8(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x), F32x8(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y)); \
	}                                                                                                               \
	FORCEINLINE v2fx8 V2fx8(f32 v) { return V2fx8(F32x8(v), F32x8(v)); }                                            \
	FORCEINLINE constexpr v2fx8 V2fx8(f32x8 v) { return V2fx8(v, v); }                                              \
	FORCEINLINE v2fx8 V2fx8(v2f v) { return V2fx8(v.x, v.y); }

V2x8(v2fx8, V2fx8, v2f, f32, f32x8, F32x8);
V2x8(v2sx8, V2sx8, v2s, s32, s32x8, S32x8);
V2x8(v2ux8, V2ux8, v2u, u32, u32x8, U32x8);
#undef V2x8

#define V3x8(v3fx8, V3fx8, v3f, f32, f32x8, F32x8, v3x4)                                                            \
	FORCEINLINE constexpr v3fx8 V3fx8(f32x8 x, f32x8 y, f32x8 z) {                                                  \
		v3fx8 r{};                                                                                                  \
		r.x = x;                                                                                                    \
		r.y = y;                                                                                                    \
		r.z = z;                                                                                                    \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE v3fx8 V3fx8(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz, f32 cx, f32 cy, f32 cz, f32 dx, f32 dy, \
							f32 dz, f32 ex, f32 ey, f32 ez, f32 fx, f32 fy, f32 fz, f32 gx, f32 gy, f32 gz, f32 hx, \
							f32 hy, f32 hz) {                                                                       \
		return V3fx8(F32x8(ax, bx, cx, dx, ex, fx, gx, hx), F32x8(ay, by, cy, dy, ey, fy, gy, hy),                  \
					 F32x8(az, bz, cz, dz, ez, fz, gz, hz));                                                        \
	}                                                                                                               \
	FORCEINLINE v3fx8 V3fx8(v3f i, v3f j, v3f k, v3f l, v3f a, v3f b, v3f c, v3f d) {                               \
		return V3fx8(F32x8(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x), F32x8(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y),  \
					 F32x8(i.z, j.z, k.z, l.z, a.z, b.z, c.z, d.z));                                                \
	}                                                                                                               \
	FORCEINLINE v3fx8 V3fx8(f32 x, f32 y, f32 z) { return V3fx8(F32x8(x), F32x8(y), F32x8(z)); }                                  \
	FORCEINLINE v3fx8 V3fx8(f32 v) { return V3fx8(v, v, v); }                                  \
	FORCEINLINE constexpr v3fx8 V3fx8(f32x8 v) { return V3fx8(v, v, v); }                                           \
	FORCEINLINE v3fx8 V3fx8(v3f v) { return V3fx8(F32x8(v.x), F32x8(v.y), F32x8(v.z)); }
V3x8(v3fx8, V3fx8, v3f, f32, f32x8, F32x8, v3fx4);
V3x8(v3sx8, V3sx8, v3s, s32, s32x8, S32x8, v3sx4);
V3x8(v3ux8, V3ux8, v3u, u32, u32x8, U32x8, v3ux4);
#undef V3x8

#define V4x8(v4fx8, V4fx8, v4f, f32, f32x8, F32x8)                                                                  \
	FORCEINLINE constexpr v4fx8 V4fx8(f32x8 x, f32x8 y, f32x8 z, f32x8 w) {                                         \
		v4fx8 r{};                                                                                                  \
		r.x = x;                                                                                                    \
		r.y = y;                                                                                                    \
		r.z = z;                                                                                                    \
		r.w = w;                                                                                                    \
		return r;                                                                                                   \
	}                                                                                                               \
	FORCEINLINE v4fx8 V4fx8(f32 ax, f32 ay, f32 az, f32 aw, f32 bx, f32 by, f32 bz, f32 bw, f32 cx, f32 cy, f32 cz, \
							f32 cw, f32 dx, f32 dy, f32 dz, f32 dw, f32 ex, f32 ey, f32 ez, f32 ew, f32 fx, f32 fy, \
							f32 fz, f32 fw, f32 gx, f32 gy, f32 gz, f32 gw, f32 hx, f32 hy, f32 hz, f32 hw) {       \
		return V4fx8(F32x8(ax, bx, cx, dx, ex, fx, gx, hx), F32x8(ay, by, cy, dy, ey, fy, gy, hy),                  \
					 F32x8(az, bz, cz, dz, ez, fz, gz, hz), F32x8(aw, bw, cw, dw, ew, fw, gw, hw));                 \
	}                                                                                                               \
	FORCEINLINE v4fx8 V4fx8(v4f i, v4f j, v4f k, v4f l, v4f a, v4f b, v4f c, v4f d) {                               \
		return V4fx8(F32x8(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x), F32x8(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y),  \
					 F32x8(i.z, j.z, k.z, l.z, a.z, b.z, c.z, d.z), F32x8(i.w, j.w, k.w, l.w, a.w, b.w, c.w, d.w)); \
	}                                                                                                               \
	FORCEINLINE v4fx8 V4fx8(f32 x, f32 y, f32 z, f32 w) { return V4fx8(F32x8(x), F32x8(y), F32x8(z), F32x8(w)); }                                  \
	FORCEINLINE v4fx8 V4fx8(f32 v) { return V4fx8(v, v, v, v); }                                  \
	FORCEINLINE constexpr v4fx8 V4fx8(f32x8 v) { return V4fx8(v, v, v, v); }                                        \
	FORCEINLINE v4fx8 V4fx8(v4f v) { return V4fx8(F32x8(v.x), F32x8(v.y), F32x8(v.z), F32x8(v.w)); }

V4x8(v4fx8, V4fx8, v4f, f32, f32x8, F32x8);
V4x8(v4sx8, V4sx8, v4s, s32, s32x8, S32x8);
V4x8(v4ux8, V4ux8, v4u, u32, u32x8, U32x8);
#undef V4x8

#define X(v2f, v3f, v4f, V2fx, V3fx, V4fx, p, f32) 														       \
template<> FORCEINLINE v2f##x<p> V2fx<p>(v2f v) { return V2fx##p(v); }										   \
template<> FORCEINLINE v3f##x<p> V3fx<p>(v3f v) { return V3fx##p(v); }										   \
template<> FORCEINLINE v4f##x<p> V4fx<p>(v4f v) { return V4fx##p(v); }										   \
template<> FORCEINLINE v2f##x<p> V2fx<p>(f32 v) { return V2fx##p(v); }										   \
template<> FORCEINLINE v3f##x<p> V3fx<p>(f32 v) { return V3fx##p(v); }										   \
template<> FORCEINLINE v4f##x<p> V4fx<p>(f32 v) { return V4fx##p(v); }										   \
template<> FORCEINLINE v2f##x<p> V2fx<p>(f32##x##p v) { return V2fx##p(v); }								   \
template<> FORCEINLINE v3f##x<p> V3fx<p>(f32##x##p v) { return V3fx##p(v); }								   \
template<> FORCEINLINE v4f##x<p> V4fx<p>(f32##x##p v) { return V4fx##p(v); }								   \
template<> FORCEINLINE v2f##x<p> V2fx<p>(f32 x, f32 y) { return V2fx##p(x, y); }							   \
template<> FORCEINLINE v3f##x<p> V3fx<p>(f32 x, f32 y, f32 z) { return V3fx##p(x, y, z); }					   \
template<> FORCEINLINE v4f##x<p> V4fx<p>(f32 x, f32 y, f32 z, f32 w) { return V4fx##p(x, y, z, w); }		   \
template<> FORCEINLINE v2f##x<p> V2fx<p>(f32##x##p x, f32##x##p y) { return V2fx##p(x, y); }				   \
template<> FORCEINLINE v3f##x<p> V3fx<p>(f32##x##p x, f32##x##p y, f32##x##p z) { return V3fx##p(x, y, z); }   \
template<> FORCEINLINE v4f##x<p> V4fx<p>(f32##x##p x, f32##x##p y, f32##x##p z, f32##x##p w) { return V4fx##p(x, y, z, w); }

#define P(v2f, v3f, v4f, V2fx, V3fx, V4fx, f32) \
X(v2f, v3f, v4f, V2fx, V3fx, V4fx, 4, f32)	    \
X(v2f, v3f, v4f, V2fx, V3fx, V4fx, 8, f32)

P(v2f, v3f, v4f, V2fx, V3fx, V4fx, f32)
P(v2s, v3s, v4s, V2sx, V3sx, V4sx, s32)
P(v2u, v3u, v4u, V2ux, V3ux, V4ux, u32)

#undef P
#undef X 

// clang-format off
#define CVT(v2f, v2s, V2s, s32, v2u, V2u, u32)                \
	FORCEINLINE v2f::operator v2s() const { return V2s((s32)x, (s32)y); } \
	FORCEINLINE v2f::operator v2u() const { return V2u((u32)x, (u32)y); }
CVT(v2f, v2s, V2s, s32, v2u, V2u, u32) CVT(v2fx4, v2sx4, V2sx4, s32x4, v2ux4, V2ux4, u32x4) CVT(v2fx8, v2sx8, V2sx8, s32x8, v2ux8, V2ux8, u32x8);
CVT(v2s, v2f, V2f, f32, v2u, V2u, u32) CVT(v2sx4, v2fx4, V2fx4, f32x4, v2ux4, V2ux4, u32x4) CVT(v2sx8, v2fx8, V2fx8, f32x8, v2ux8, V2ux8, u32x8);
CVT(v2u, v2f, V2f, f32, v2s, V2s, s32) CVT(v2ux4, v2fx4, V2fx4, f32x4, v2sx4, V2sx4, s32x4) CVT(v2ux8, v2fx8, V2fx8, f32x8, v2sx8, V2sx8, s32x8);
#undef CVT

#define CVT(v3f, v3s, V3s, s32, v3u, V3u, u32)                        \
	FORCEINLINE v3f::operator v3s() const { return V3s((s32)x, (s32)y, (s32)z); } \
	FORCEINLINE v3f::operator v3u() const { return V3u((u32)x, (u32)y, (u32)z); }
CVT(v3f, v3s, V3s, s32, v3u, V3u, u32) CVT(v3fx4, v3sx4, V3sx4, s32x4, v3ux4, V3ux4, u32x4) CVT(v3fx8, v3sx8, V3sx8, s32x8, v3ux8, V3ux8, u32x8);
CVT(v3s, v3f, V3f, f32, v3u, V3u, u32) CVT(v3sx4, v3fx4, V3fx4, f32x4, v3ux4, V3ux4, u32x4) CVT(v3sx8, v3fx8, V3fx8, f32x8, v3ux8, V3ux8, u32x8);
CVT(v3u, v3f, V3f, f32, v3s, V3s, s32) CVT(v3ux4, v3fx4, V3fx4, f32x4, v3sx4, V3sx4, s32x4) CVT(v3ux8, v3fx8, V3fx8, f32x8, v3sx8, V3sx8, s32x8);
#undef CVT

#define CVT(v4f, v4s, V4s, s32, v4u, V4u, u32)                                \
	FORCEINLINE v4f::operator v4s() const { return V4s((s32)x, (s32)y, (s32)z, (s32)w); } \
	FORCEINLINE v4f::operator v4u() const { return V4u((u32)x, (u32)y, (u32)z, (u32)w); }
CVT(v4f, v4s, V4s, s32, v4u, V4u, u32) CVT(v4fx4, v4sx4, V4sx4, s32x4, v4ux4, V4ux4, u32x4) CVT(v4fx8, v4sx8, V4sx8, s32x8, v4ux8, V4ux8, u32x8);
CVT(v4s, v4f, V4f, f32, v4u, V4u, u32) CVT(v4sx4, v4fx4, V4fx4, f32x4, v4ux4, V4ux4, u32x4) CVT(v4sx8, v4fx8, V4fx8, f32x8, v4ux8, V4ux8, u32x8);
CVT(v4u, v4f, V4f, f32, v4s, V4s, s32) CVT(v4ux4, v4fx4, V4fx4, f32x4, v4sx4, V4sx4, s32x4) CVT(v4ux8, v4fx8, V4fx8, f32x8, v4sx8, V4sx8, s32x8);
#undef CVT
// clang-format on

FORCEINLINE constexpr m4 M4(f32 v = 0.0f) { return m4{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; }
FORCEINLINE constexpr m4 M4(v4f i, v4f j, v4f k, v4f l) { return m4{i, j, k, l}; }
FORCEINLINE constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l) {
	m4 r{};
	r.m[0] = i;
	r.m[1] = j;
	r.m[2] = k;
	r.m[3] = l;
	return r;
}
FORCEINLINE constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw) {
	return {ix, iy, iz, iw, jx, jy, jz, jw, kx, ky, kz, kw, lx, ly, lz, lw};
}

FORCEINLINE m4 M4(m3 v) {
	return {
		V4f(v.i, 0),
		V4f(v.j, 0),
		V4f(v.k, 0),
		V4f(0, 0, 0, 1),
	};
}

namespace CE {

// clang-format off

#define F32x4(f32x4, F32x4, f32)					 \
FORCEINLINE constexpr f32x4 F32x4(f32 a, f32 b, f32 c, f32 d) {	 \
	f32x4 r{};										 \
	r[0] = a;										 \
	r[1] = b;										 \
	r[2] = c;										 \
	r[3] = d;										 \
	return r;										 \
}
F32x4(f32x4, F32x4, f32)
F32x4(s32x4, S32x4, s32)
F32x4(u32x4, U32x4, u32)
#undef F32x4

#define F32x8(f32x8, F32x8, f32)												\
FORCEINLINE constexpr f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {	\
	f32x8 r{};																	\
	r[0] = a;																	\
	r[1] = b;																	\
	r[2] = c;																	\
	r[3] = d;																	\
	r[4] = e;																	\
	r[5] = f;																	\
	r[6] = g;																	\
	r[7] = h;																	\
	return r;																	\
}
F32x8(f32x8, F32x8, f32)
F32x8(s32x8, S32x8, s32)
F32x8(u32x8, U32x8, u32)
#undef F32x8

#define V2x4(v2f, v2fx4, V2fx4, f32x4, F32x4)																								\
	FORCEINLINE constexpr v2fx4 V2fx4(f32x4 x, f32x4 y) { v2fx4 r{}; r.x = x; r.y = y; return r; }														\
	FORCEINLINE constexpr v2fx4 V2fx4(v2f a, v2f b, v2f c, v2f d) { return CE::V2fx4(CE::F32x4(a.x, b.x, c.x, d.x), CE::F32x4(a.y, b.y, c.y, d.y)); }	\
	FORCEINLINE constexpr v2fx4 V2fx4(v2f v) { return CE::V2fx4(v, v, v, v); }
V2x4(v2f, v2fx4, V2fx4, f32x4, F32x4)
V2x4(v2s, v2sx4, V2sx4, s32x4, S32x4)
V2x4(v2u, v2ux4, V2ux4, u32x4, U32x4)
#undef V2x4

#define V3x4(v3f, v3fx4, V3fx4, f32x4, F32x4)																																\
	FORCEINLINE constexpr v3fx4 V3fx4(f32x4 x, f32x4 y, f32x4 z) { v3fx4 r{}; r.x = x; r.y = y; r.z = z; return r; }																	\
	FORCEINLINE constexpr v3fx4 V3fx4(v3f a, v3f b, v3f c, v3f d) {  return CE::V3fx4(CE::F32x4(a.x, b.x, c.x, d.x), CE::F32x4(a.y, b.y, c.y, d.y), CE::F32x4(a.z, b.z, c.z, d.z)); }	\
	FORCEINLINE constexpr v3fx4 V3fx4(v3f v) { return CE::V3fx4(v, v, v, v); }
V3x4(v3f, v3fx4, V3fx4, f32x4, F32x4)
V3x4(v3s, v3sx4, V3sx4, s32x4, S32x4)
V3x4(v3u, v3ux4, V3ux4, u32x4, U32x4)
#undef V3x4

#define V4x4(v4f, v4fx4, V4fx4, f32x4, F32x4)																																							\
	FORCEINLINE constexpr v4fx4 V4fx4(f32x4 x, f32x4 y, f32x4 z, f32x4 w) { v4fx4 r{}; r.x = x; r.y = y; r.z = z; r.w = w; return r; }																				\
	FORCEINLINE constexpr v4fx4 V4fx4(v4f a, v4f b, v4f c, v4f d) { return CE::V4fx4(CE::F32x4(a.x, b.x, c.x, d.x), CE::F32x4(a.y, b.y, c.y, d.y), CE::F32x4(a.z, b.z, c.z, d.z), CE::F32x4(a.w, b.w, c.w, d.w)); }	\
	FORCEINLINE constexpr v4fx4 V4fx4(v4f v) { return CE::V4fx4(v, v, v, v); }
V4x4(v4f, v4fx4, V4fx4, f32x4, F32x4)
V4x4(v4s, v4sx4, V4sx4, s32x4, S32x4)
V4x4(v4u, v4ux4, V4ux4, u32x4, U32x4)
#undef V4x4

#define V2x8(v2f, v2fx8, V2fx8, f32x8, F32x8)																								\
	FORCEINLINE constexpr v2fx8 V2fx8(f32x8 x, f32x8 y) { v2fx8 r{}; r.x = x; r.y = y; return r; }														\
	FORCEINLINE constexpr v2fx8 V2fx8(v2f a, v2f b, v2f c, v2f d, v2f e, v2f f, v2f g, v2f h) { return CE::V2fx8(CE::F32x8(a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x), CE::F32x8(a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y)); }	\
	FORCEINLINE constexpr v2fx8 V2fx8(v2f v) { return CE::V2fx8(v, v, v, v, v, v, v, v); }
V2x8(v2f, v2fx8, V2fx8, f32x8, F32x8)
V2x8(v2s, v2sx8, V2sx8, s32x8, S32x8)
V2x8(v2u, v2ux8, V2ux8, u32x8, U32x8)
#undef V2x8

#define V3x8(v3f, v3fx8, V3fx8, f32x8, F32x8)																																\
	FORCEINLINE constexpr v3fx8 V3fx8(f32x8 x, f32x8 y, f32x8 z) { v3fx8 r{}; r.x = x; r.y = y; r.z = z; return r; }																	\
	FORCEINLINE constexpr v3fx8 V3fx8(v3f a, v3f b, v3f c, v3f d, v3f e, v3f f, v3f g, v3f h) {  return CE::V3fx8(CE::F32x8(a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x), CE::F32x8(a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y), CE::F32x8(a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z)); }	\
	FORCEINLINE constexpr v3fx8 V3fx8(v3f v) { return CE::V3fx8(v, v, v, v, v, v, v, v); }
V3x8(v3f, v3fx8, V3fx8, f32x8, F32x8)
V3x8(v3s, v3sx8, V3sx8, s32x8, S32x8)
V3x8(v3u, v3ux8, V3ux8, u32x8, U32x8)
#undef V3x8

#define V4x8(v4f, v4fx8, V4fx8, f32x8, F32x8)																																							\
	FORCEINLINE constexpr v4fx8 V4fx8(f32x8 x, f32x8 y, f32x8 z, f32x8 w) { v4fx8 r{}; r.x = x; r.y = y; r.z = z; r.w = w; return r; }																				\
	FORCEINLINE constexpr v4fx8 V4fx8(v4f a, v4f b, v4f c, v4f d, v4f e, v4f f, v4f g, v4f h) { return CE::V4fx8(CE::F32x8(a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x), CE::F32x8(a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y), CE::F32x8(a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z), CE::F32x8(a.w, b.w, c.w, d.w, e.w, f.w, g.w, h.w)); }	\
	FORCEINLINE constexpr v4fx8 V4fx8(v4f v) { return CE::V4fx8(v, v, v, v, v, v, v, v); }
V4x8(v4f, v4fx8, V4fx8, f32x8, F32x8)
V4x8(v4s, v4sx8, V4sx8, s32x8, S32x8)
V4x8(v4u, v4ux8, V4ux8, u32x8, U32x8)
#undef V4x8

// clang-format on

} // namespace CE

  // clang-format off
/*
template<> s32x4 cvt(f32x4 v) {  return S32x4(_mm_cvtps_epi32(v.m));  }
template<> f32x4 cvt(s32x4 v) {  return F32x4(_mm_cvtepi32_ps(v.m));  }

template<> s32x8 cvt(f32x8 v) {  return S32x8(_mm256_cvtps_epi32(v.m));  }
template<> f32x8 cvt(s32x8 v) {  return F32x8(_mm256_cvtepi32_ps(v.m));  }

#define CVT(u32x4, f32x4, F32x4, f32) template<> f32x4 cvt(u32x4 v) {  return F32x4((f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3]);  }
CVT(u32x4, f32x4, F32x4, f32);
CVT(f32x4, u32x4, U32x4, u32);
#undef CVT

#define CVT(u32x8, f32x8, F32x8, f32) template<> f32x8 cvt(u32x8 v) {  return F32x8((f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3], (f32)v[4], (f32)v[5], (f32)v[6], (f32)v[7]);  }
CVT(u32x8, f32x8, F32x8, f32);
CVT(f32x8, u32x8, U32x8, u32);
#undef CVT

#define CVT(u32x4, s32x4, S32x4) template<> constexpr s32x4 cvt(u32x4 v) {  return S32x4(v.m);  }
CVT(u32x4, s32x4, S32x4);
CVT(s32x4, u32x4, U32x4);
#undef CVT

#define CVT(u32x8, s32x8, S32x8) template<> constexpr s32x8 cvt(u32x8 v) {  return S32x8(v.m);  }
CVT(u32x8, s32x8, S32x8);
CVT(s32x8, u32x8, U32x8);
#undef CVT

#define CVT(v2f, v2s, s32) template<> constexpr v2s cvt(v2f v) { return {(s32)v.x, (s32)v.y}; }
CVT(v2f, v2s, s32) CVT(v2f, v2u, u32);
CVT(v2s, v2f, f32) CVT(v2s, v2u, u32);
CVT(v2u, v2f, f32) CVT(v2u, v2s, s32);
#undef CVT

#define CVT(v3f, v3s, s32) template<> constexpr v3s cvt(v3f v) { return {(s32)v.x, (s32)v.y, (s32)v.z}; }
CVT(v3f, v3s, s32) CVT(v3f, v3u, u32);
CVT(v3s, v3f, f32) CVT(v3s, v3u, u32);
CVT(v3u, v3f, f32) CVT(v3u, v3s, s32);
#undef CVT

#define CVT(v4f, v4s, s32) template<> constexpr v4s cvt(v4f v) { return {(s32)v.x, (s32)v.y, (s32)v.z, (s32)v.w}; }
CVT(v4f, v4u, u32) CVT(v4s, v4u, u32);
CVT(v4u, v4f, f32) CVT(v4u, v4s, s32);
#undef CVT

template<> v4s cvt(v4f v) { return V4s(cvt<s32x4>(v.m)); }
template<> v4f cvt(v4s v) { return V4f(cvt<f32x4>(v.m)); }

#define CVT(v2f, v2s, V2s, s32) template<> v2s cvt(v2f v) { return V2s(cvt<s32>(v.x), cvt<s32>(v.y)); }
CVT(v2fx4, v2sx4, V2sx4, s32x4) CVT(v2sx4, v2fx4, V2fx4, f32x4);
CVT(v2fx8, v2sx8, V2sx8, s32x8) CVT(v2sx8, v2fx8, V2fx8, f32x8);
#undef CVT

#define CVT(v3f, v3s, V3s, s32) template<> v3s cvt(v3f v) { return V3s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z)); }
CVT(v3fx4, v3sx4, V3sx4, s32x4) CVT(v3sx4, v3fx4, V3fx4, f32x4);
CVT(v3fx8, v3sx8, V3sx8, s32x8) CVT(v3sx8, v3fx8, V3fx8, f32x8);
#undef CVT

#define CVT(v4f, v4s, V4s, s32) template<> v4s cvt(v4f v) { return V4s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z), cvt<s32>(v.w)); }
CVT(v4fx4, v4sx4, V4sx4, s32x4) CVT(v4sx4, v4fx4, V4fx4, f32x4);
CVT(v4fx8, v4sx8, V4sx8, s32x8) CVT(v4sx8, v4fx8, V4fx8, f32x8);
#undef CVT

#define CVT(v2f, v2s, V2s, s32) template<> constexpr v2s cvt(v2f v) { return V2s(cvt<s32>(v.x), cvt<s32>(v.y)); }
CVT(v2fx4, v2ux4, V2ux4, u32x4) CVT(v2ux4, v2fx4, V2fx4, f32x4);
CVT(v2fx8, v2ux8, V2ux8, u32x8) CVT(v2ux8, v2fx8, V2fx8, f32x8);
CVT(v2sx4, v2ux4, V2ux4, u32x4) CVT(v2ux4, v2sx4, V2sx4, s32x4);
CVT(v2sx8, v2ux8, V2ux8, u32x8) CVT(v2ux8, v2sx8, V2sx8, s32x8);
#undef CVT

#define CVT(v3f, v3s, V3s, s32) template<> constexpr v3s cvt(v3f v) { return V3s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z)); }
CVT(v3fx4, v3ux4, V3ux4, u32x4) CVT(v3ux4, v3fx4, V3fx4, f32x4);
CVT(v3fx8, v3ux8, V3ux8, u32x8) CVT(v3ux8, v3fx8, V3fx8, f32x8);
CVT(v3sx4, v3ux4, V3ux4, u32x4) CVT(v3ux4, v3sx4, V3sx4, s32x4);
CVT(v3sx8, v3ux8, V3ux8, u32x8) CVT(v3ux8, v3sx8, V3sx8, s32x8);
#undef CVT

#define CVT(v4f, v4s, V4s, s32) template<> constexpr v4s cvt(v4f v) { return V4s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z), cvt<s32>(v.w)); }
CVT(v4fx4, v4ux4, V4ux4, u32x4) CVT(v4ux4, v4fx4, V4fx4, f32x4);
CVT(v4fx8, v4ux8, V4ux8, u32x8) CVT(v4ux8, v4fx8, V4fx8, f32x8);
CVT(v4sx4, v4ux4, V4ux4, u32x4) CVT(v4ux4, v4sx4, V4sx4, s32x4);
CVT(v4sx8, v4ux8, V4ux8, u32x8) CVT(v4ux8, v4sx8, V4sx8, s32x8);
#undef CVT
*/

namespace CE {

#define CVT(u32x4, f32x4, f32, F32x4) template<> constexpr f32x4 cvt(u32x4 v) {  return CE::F32x4((f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3]);  }
CVT(f32x4, s32x4, s32, S32x4) CVT(f32x4, u32x4, u32, U32x4);
CVT(s32x4, f32x4, f32, F32x4) CVT(s32x4, u32x4, u32, U32x4);
CVT(u32x4, f32x4, f32, F32x4) CVT(u32x4, s32x4, s32, S32x4);
#undef CVT

#define CVT(u32x8, f32x8, f32, F32x8) template<> constexpr f32x8 cvt(u32x8 v) {  return CE::F32x8((f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3], (f32)v[4], (f32)v[5], (f32)v[6], (f32)v[7]);  }
CVT(f32x8, s32x8, s32, S32x8) CVT(f32x8, u32x8, u32, U32x8);
CVT(s32x8, f32x8, f32, F32x8) CVT(s32x8, u32x8, u32, U32x8);
CVT(u32x8, f32x8, f32, F32x8) CVT(u32x8, s32x8, s32, S32x8);
#undef CVT

#define CVT(v2f, v2s, s32) template<> constexpr v2s cvt(v2f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y)}; }
CVT(v2f, v2s, s32) CVT(v2f, v2u, u32) CVT(v2fx4, v2sx4, s32x4) CVT(v2fx4, v2ux4, u32x4) CVT(v2fx8, v2sx8, s32x8) CVT(v2fx8, v2ux8, u32x8);
CVT(v2s, v2f, f32) CVT(v2s, v2u, u32) CVT(v2sx4, v2fx4, f32x4) CVT(v2sx4, v2ux4, u32x4) CVT(v2sx8, v2fx8, f32x8) CVT(v2sx8, v2ux8, u32x8);
CVT(v2u, v2f, f32) CVT(v2u, v2s, s32) CVT(v2ux4, v2fx4, f32x4) CVT(v2ux4, v2sx4, s32x4) CVT(v2ux8, v2fx8, f32x8) CVT(v2ux8, v2sx8, s32x8);
#undef CVT

#define CVT(v3f, v3s, s32) template<> constexpr v3s cvt(v3f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z)}; }
CVT(v3f, v3s, s32) CVT(v3f, v3u, u32) CVT(v3fx4, v3sx4, s32x4) CVT(v3fx4, v3ux4, u32x4) CVT(v3fx8, v3sx8, s32x8) CVT(v3fx8, v3ux8, u32x8);
CVT(v3s, v3f, f32) CVT(v3s, v3u, u32) CVT(v3sx4, v3fx4, f32x4) CVT(v3sx8, v3fx8, f32x8) CVT(v3sx4, v3ux4, u32x4) CVT(v3sx8, v3ux8, u32x8);
CVT(v3u, v3f, f32) CVT(v3u, v3s, s32) CVT(v3ux4, v3fx4, f32x4) CVT(v3ux8, v3fx8, f32x8) CVT(v3ux4, v3sx4, s32x4) CVT(v3ux8, v3sx8, s32x8);
#undef CVT

#define CVT(v4f, v4s, s32) template<> constexpr v4s cvt(v4f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z), CE::cvt<s32>(v.w)}; }
CVT(v4f, v4s, s32) CVT(v4f, v4u, u32) CVT(v4fx4, v4sx4, s32x4) CVT(v4fx4, v4ux4, u32x4) CVT(v4fx8, v4sx8, s32x8) CVT(v4fx8, v4ux8, u32x8);
CVT(v4s, v4f, f32) CVT(v4s, v4u, u32) CVT(v4sx4, v4fx4, f32x4) CVT(v4sx4, v4ux4, u32x4)	CVT(v4sx8, v4fx8, f32x8) CVT(v4sx8, v4ux8, u32x8);
CVT(v4u, v4f, f32) CVT(v4u, v4s, s32) CVT(v4ux4, v4fx4, f32x4) CVT(v4ux4, v4sx4, s32x4)	CVT(v4ux8, v4fx8, f32x8) CVT(v4ux8, v4sx8, s32x8);
#undef CVT
} // namespace CE

// clang-format on

#define SHUFFLE(a, s0, s1, b, s2, s3) _mm_shuffle_ps(a, b, _MM_SHUFFLE(s3, s2, s1, s0))

// clang-format off
FORCEINLINE v2f pack(v2f v) { return v; }
FORCEINLINE v3f pack(v3f v) { return v; }
FORCEINLINE v4f pack(v4f v) { return v; }
FORCEINLINE v2f unpack(v2f v) { return v; }
FORCEINLINE v3f unpack(v3f v) { return v; }
FORCEINLINE v4f unpack(v4f v) { return v; }
FORCEINLINE v2fx4 pack(v2fx4 v) { 
#if ARCH_AVX2
    *(__m256*)&v = _mm256_permutevar8x32_ps(*(__m256*)&v, _mm256_setr_epi32(0,2,4,6,1,3,5,7));
	return v;
#else
	v2fx4 r;
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 2];
		}
	}
	return r;
#endif
}
FORCEINLINE v3fx4 pack(v3fx4 v) { 
	v3fx4 r;
#if 0
	gather(*(f32x8*)&r, S32x8(0,3,6,9,1,4,7,10)*sizeof(f32), &v);
	gather(r.z, S32x4(2,5,8,11)*sizeof(f32), &v);
#else
	for (u32 i = 0; i < 3; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 3];
		}
	}
#endif
	return r;
}
FORCEINLINE v4fx4 pack(v4fx4 v) { 
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
FORCEINLINE v2fx8 pack(v2fx8 v) {
	v2fx8 r;
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 2];
		}
	}
	return r;
}
FORCEINLINE v3fx8 pack(v3fx8 v) { 
	v3fx8 r;
	for (u32 i = 0; i < 3; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 3];
		}
	}
	return r;
}
FORCEINLINE v4fx8 pack(v4fx8 v) { 
	v4fx8 r;
	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 4];
		}
	}
	return r;
}
FORCEINLINE v2fx4 unpack(v2fx4 v) { 
	v2fx4 r;
	r.x.m.ps = _mm_unpacklo_ps(v.x.m.ps, v.y.m.ps);
	r.y.m.ps = _mm_unpackhi_ps(v.x.m.ps, v.y.m.ps);
	return r;
}
FORCEINLINE v3fx4 unpack(v3fx4 v) { 
	auto xy = SHUFFLE(v.x.m.ps, 0, 2, v.y.m.ps, 0, 2);
	auto yz = SHUFFLE(v.y.m.ps, 1, 3, v.z.m.ps, 1, 3);
	auto xz = SHUFFLE(v.x.m.ps, 1, 3, v.z.m.ps, 0, 2);
	v.x.m.ps = SHUFFLE(xy, 0, 2, xz, 2, 0);
	v.y.m.ps = SHUFFLE(yz, 0, 2, xy, 1, 3);
	v.z.m.ps = SHUFFLE(xz, 3, 1, yz, 1, 3);
	return v;
}
FORCEINLINE v4fx4 unpack(v4fx4 v) {
	return pack(v);
}
FORCEINLINE v2fx8 unpack(v2fx8 v) { 
	v2fx8 r;
#if ARCH_AVX
	__m256 m02 = _mm256_unpacklo_ps(v.x.m.ps, v.y.m.ps);
	__m256 m13 = _mm256_unpackhi_ps(v.x.m.ps, v.y.m.ps);
	r.x.m.ps = _mm256_setr_m128(_mm256_extractf128_ps(m02, 0), _mm256_extractf128_ps(m13, 0));
	r.y.m.ps = _mm256_setr_m128(_mm256_extractf128_ps(m02, 1), _mm256_extractf128_ps(m13, 1));
#else
	for (u32 i = 0; i < 8; ++i){
		for (u32 j = 0; j < 2; ++j){
			r.s[i * 2 + j] = v.s[i + j * 8];
		}
	}
#endif
	return r;
}
FORCEINLINE v3fx8 unpack(v3fx8 v) {
#if ARCH_AVX
	auto xy = _mm256_shuffle_ps(v.x.m.ps, v.y.m.ps, MM_SHUFFLE(0, 2, 0, 2));
	auto yz = _mm256_shuffle_ps(v.y.m.ps, v.z.m.ps, MM_SHUFFLE(1, 3, 1, 3));
	auto xz = _mm256_shuffle_ps(v.x.m.ps, v.z.m.ps, MM_SHUFFLE(1, 3, 0, 2));
	auto rx = _mm256_shuffle_ps(xy, xz, MM_SHUFFLE(0, 2, 2, 0));
	auto ry = _mm256_shuffle_ps(yz, xy, MM_SHUFFLE(0, 2, 1, 3));
	auto rz = _mm256_shuffle_ps(xz, yz, MM_SHUFFLE(3, 1, 1, 3));
	v.x.m.ps = _mm256_permute2f128_ps(rx, ry, MM256_PERM128(0, 2));
	v.y.m.ps = _mm256_permute2f128_ps(rz, rx, MM256_PERM128(0, 3));
	v.z.m.ps = _mm256_permute2f128_ps(ry, rz, MM256_PERM128(1, 3));
	return v;
#else
	v3fx8 r;
	for (u32 i = 0; i < 8; ++i) {
		for (u32 j = 0; j < 3; ++j){
			r.s[i * 3 + j] = v.s[i + j * 8];
		}
	}
	return r;
#endif
}
FORCEINLINE v4fx8 unpack(v4fx8 v) {
	v4fx8 r;
	for (u32 i = 0; i < 8; ++i) {
		for (u32 j = 0; j < 4; ++j){
			r.s[i * 4 + j] = v.s[i + j * 8];
		}
	}
	return r;
}
// clang-format on

#define MINF32x4(a, b) {_mm_min_ps(a.m.ps, b.m.ps)}
#define MINS32x4(a, b) {_mm_castpi_ps(_mm_min_epi32(a.m.pi, b.m.pi))}
#define MINU32x4(a, b) {_mm_castpi_ps(_mm_min_epu32(a.m.pi, b.m.pi))}
#define MAXF32x4(a, b) {_mm_max_ps(a.m.ps, b.m.ps)}
#define MAXS32x4(a, b) {_mm_castpi_ps(_mm_max_epi32(a.m.pi, b.m.pi))}
#define MAXU32x4(a, b) {_mm_castpi_ps(_mm_max_epu32(a.m.pi, b.m.pi))}

#if ARCH_AVX
#define MINF32x8(a, b) {_mm256_min_ps(a.m.ps, b.m.ps)}
#define MAXF32x8(a, b) {_mm256_max_ps(a.m.ps, b.m.ps)}
#else
#define MINF32x8(a, b) {_mm_min_ps(a.lo.ps, b.lo.ps), _mm_min_ps(a.hi.ps, b.hi.ps)}
#define MAXF32x8(a, b) {_mm_max_ps(a.lo.ps, b.lo.ps), _mm_max_ps(a.hi.ps, b.hi.ps)}
#endif

#if ARCH_AVX2
#define MINS32x8(a, b) {_mm256_castpi_ps(_mm256_min_epi32(a.m.pi, b.m.pi))}
#define MINU32x8(a, b) {_mm256_castpi_ps(_mm256_min_epu32(a.m.pi, b.m.pi))}
#define MAXS32x8(a, b) {_mm256_castpi_ps(_mm256_max_epi32(a.m.pi, b.m.pi))}
#define MAXU32x8(a, b) {_mm256_castpi_ps(_mm256_max_epu32(a.m.pi, b.m.pi))}
#elif ARCH_AVX
#define MAXS32x8(a, b) {_mm256_setr_m128(_mm_castpi_ps(_mm_max_epi32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_max_epi32(a.hi.pi, b.hi.pi)))}
#define MAXU32x8(a, b) {_mm256_setr_m128(_mm_castpi_ps(_mm_max_epu32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_max_epu32(a.hi.pi, b.hi.pi)))}
#define MINS32x8(a, b) {_mm256_setr_m128(_mm_castpi_ps(_mm_min_epi32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_min_epi32(a.hi.pi, b.hi.pi)))}
#define MINU32x8(a, b) {_mm256_setr_m128(_mm_castpi_ps(_mm_min_epu32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_min_epu32(a.hi.pi, b.hi.pi)))}
#else
#define MAXS32x8(a, b) {_mm_castpi_ps(_mm_max_epi32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_max_epi32(a.hi.pi, b.hi.pi))}
#define MAXU32x8(a, b) {_mm_castpi_ps(_mm_max_epu32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_max_epu32(a.hi.pi, b.hi.pi))}
#define MINS32x8(a, b) {_mm_castpi_ps(_mm_min_epi32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_min_epi32(a.hi.pi, b.hi.pi))}
#define MINU32x8(a, b) {_mm_castpi_ps(_mm_min_epu32(a.lo.pi, b.lo.pi)), _mm_castpi_ps(_mm_min_epu32(a.hi.pi, b.hi.pi))}
#endif

// clang-format off
FORCEINLINE f32x4 min(f32x4 a, f32x4 b) { return MINF32x4(a, b); }
FORCEINLINE s32x4 min(s32x4 a, s32x4 b) { return MINS32x4(a, b); }
FORCEINLINE u32x4 min(u32x4 a, u32x4 b) { return MINU32x4(a, b); }
FORCEINLINE f32x8 min(f32x8 a, f32x8 b) { return MINF32x8(a, b); }
FORCEINLINE s32x8 min(s32x8 a, s32x8 b) { return MINS32x8(a, b); }
FORCEINLINE u32x8 min(u32x8 a, u32x8 b) { return MINU32x8(a, b); }

FORCEINLINE f32x4 max(f32x4 a, f32x4 b) { return MAXF32x4(a, b); }
FORCEINLINE s32x4 max(s32x4 a, s32x4 b) { return MAXS32x4(a, b); }
FORCEINLINE u32x4 max(u32x4 a, u32x4 b) { return MAXU32x4(a, b); }
FORCEINLINE f32x8 max(f32x8 a, f32x8 b) { return MAXF32x8(a, b); }
FORCEINLINE s32x8 max(s32x8 a, s32x8 b) { return MAXS32x8(a, b); }
FORCEINLINE u32x8 max(u32x8 a, u32x8 b) { return MAXU32x8(a, b); }

FORCEINLINE f32 min(f32x4 a) { 
	auto x2 = _mm_min_ps(a.m.ps, _mm_shuffle_ps(a.m.ps, a.m.ps, MM_SHUFFLE(2,3,0,0)));
	return _mm_cvtss_f32(_mm_min_ss(x2, _mm_shuffle_ps(x2, x2, MM_SHUFFLE(1,0,0,0))));
}
FORCEINLINE f32 max(f32x4 a) { 
	auto x2 = _mm_max_ps(a.m.ps, _mm_shuffle_ps(a.m.ps, a.m.ps, MM_SHUFFLE(2,3,0,0)));
	return _mm_cvtss_f32(_mm_max_ss(x2, _mm_shuffle_ps(x2, x2, MM_SHUFFLE(1,0,0,0))));
}

#if ARCH_AVX
FORCEINLINE f32 min(f32x8 a) { 
	auto x4 = _mm_min_ps(_mm256_extractf128_ps(a.m.ps, 0), _mm256_extractf128_ps(a.m.ps, 1));
	auto x2 = _mm_min_ps(x4, _mm_shuffle_ps(x4, x4, MM_SHUFFLE(2,3,0,0)));
	return _mm_cvtss_f32(_mm_min_ss(x2, _mm_shuffle_ps(x2, x2, MM_SHUFFLE(1,0,0,0))));
}
FORCEINLINE f32 max(f32x8 a) { 
	auto x4 = _mm_max_ps(_mm256_extractf128_ps(a.m.ps, 0), _mm256_extractf128_ps(a.m.ps, 1));
	auto x2 = _mm_max_ps(x4, _mm_shuffle_ps(x4, x4, MM_SHUFFLE(2,3,0,0)));
	return _mm_cvtss_f32(_mm_max_ss(x2, _mm_shuffle_ps(x2, x2, MM_SHUFFLE(1,0,0,0))));
}
#else
FORCEINLINE f32 min(f32x8 a) { 
	auto x4 = _mm_min_ps(a.lo.ps, a.hi.ps);
	auto x2 = _mm_min_ps(x4, _mm_shuffle_ps(x4, x4, MM_SHUFFLE(2,3,0,0)));
	return _mm_cvtss_f32(_mm_min_ss(x2, _mm_shuffle_ps(x2, x2, MM_SHUFFLE(1,0,0,0))));
}
FORCEINLINE f32 max(f32x8 a) { 
	auto x4 = _mm_max_ps(a.lo.ps, a.hi.ps);
	auto x2 = _mm_max_ps(x4, _mm_shuffle_ps(x4, x4, MM_SHUFFLE(2,3,0,0)));
	return _mm_cvtss_f32(_mm_max_ss(x2, _mm_shuffle_ps(x2, x2, MM_SHUFFLE(1,0,0,0))));
}
#endif

#undef MINF32x4
#undef MINS32x4
#undef MINU32x4
#undef MINF32x8
#undef MINS32x8
#undef MINU32x8
#undef MAXF32x4
#undef MAXS32x4
#undef MAXU32x4
#undef MAXF32x8
#undef MAXS32x8
#undef MAXU32x8

#define MINMAX(v2f, V2f)						  \
	FORCEINLINE auto min(v2f a, v2f b) { return V2f(min(a.x, b.x), min(a.y, b.y)); }\
	FORCEINLINE auto max(v2f a, v2f b) { return V2f(max(a.x, b.x), max(a.y, b.y)); }\
	FORCEINLINE void minmax(v2f a, v2f b, v2f& mn, v2f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
	}
MINMAX(v2f  , V2f  ) MINMAX(v2s  , V2s  ) MINMAX(v2u  , V2u  );
MINMAX(v2fx4, V2fx4) MINMAX(v2sx4, V2sx4) MINMAX(v2ux4, V2ux4);
MINMAX(v2fx8, V2fx8) MINMAX(v2sx8, V2sx8) MINMAX(v2ux8, V2ux8);
#undef MINMAX

#define MINMAX(v3f, V3f)						  \
	FORCEINLINE auto min(v3f a, v3f b) { return V3f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }\
	FORCEINLINE auto max(v3f a, v3f b) { return V3f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }\
	FORCEINLINE void minmax(v3f a, v3f b, v3f& mn, v3f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
	}
MINMAX(v3f  , V3f  ) MINMAX(v3s  , V3s  ) MINMAX(v3u  , V3u  );
MINMAX(v3fx4, V3fx4) MINMAX(v3sx4, V3sx4) MINMAX(v3ux4, V3ux4);
MINMAX(v3fx8, V3fx8) MINMAX(v3sx8, V3sx8) MINMAX(v3ux8, V3ux8);
#undef MINMAX

#define MINMAX(v4f, V4f)						  \
	FORCEINLINE auto min(v4f a, v4f b) { return V4f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w)); }\
	FORCEINLINE auto max(v4f a, v4f b) { return V4f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w)); }\
	FORCEINLINE void minmax(v4f a, v4f b, v4f& mn, v4f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
		minmax(a.w, b.w, mn.w, mx.w); 		  \
	}
MINMAX(v4f  , V4f  ) MINMAX(v4s  , V4s  ) MINMAX(v4u  , V4u  );
MINMAX(v4fx4, V4fx4) MINMAX(v4sx4, V4sx4) MINMAX(v4ux4, V4ux4);
MINMAX(v4fx8, V4fx8) MINMAX(v4sx8, V4sx8) MINMAX(v4ux8, V4ux8);
#undef MINMAX
// clang-format on

#define HALF(f32) FORCEINLINE f32 half(f32 v) { return v * 0.5f; }
HALF(f32) HALF(f32x4) HALF(f32x8);
HALF(v2f) HALF(v2fx4) HALF(v2fx8);
HALF(v3f) HALF(v3fx4) HALF(v3fx8);
HALF(v4f) HALF(v4fx4) HALF(v4fx8);
#undef HALF

#define HALF(s32) FORCEINLINE s32 half(s32 v) { return v / 2; }
HALF(s32) HALF(s32x4) HALF(s32x8);
HALF(v2s) HALF(v2sx4) HALF(v2sx8);
HALF(v3s) HALF(v3sx4) HALF(v3sx8);
HALF(v4s) HALF(v4sx4) HALF(v4sx8);
#undef HALF

#define HALF(u32) FORCEINLINE u32 half(u32 v) { return v >> 1; }
HALF(u32) HALF(u32x4) HALF(u32x8);
HALF(v2u) HALF(v2ux4) HALF(v2ux8);
HALF(v3u) HALF(v3ux4) HALF(v3ux8);
HALF(v4u) HALF(v4ux4) HALF(v4ux8);
#undef HALF

#define SELECT32x4(type) FORCEINLINE type select(b32x4 mask, type a, type b) { return {_mm_blendv_ps(b.m.ps, a.m.ps, mask.m.ps)}; }
#if ARCH_AVX
#define SELECT32x8(type) FORCEINLINE type select(b32x8 mask, type a, type b) { return {_mm256_blendv_ps(b.m.ps, a.m.ps, mask.m.ps)}; }
#else
#define SELECT32x8(type) FORCEINLINE type select(b32x8 mask, type a, type b) { return {_mm_blendv_ps(b.lo.ps, a.lo.ps, mask.lo.ps), _mm_blendv_ps(b.hi.ps, a.hi.ps, mask.hi.ps)}; }
#endif

SELECT32x4(f32x4)
SELECT32x4(s32x4)
SELECT32x4(u32x4)
SELECT32x8(f32x8)
SELECT32x8(s32x8)
SELECT32x8(u32x8)

#undef SELECT32x4
#undef SELECT32x8

FORCEINLINE v2fx4 select(b32x4 mask, v2fx4 a, v2fx4 b) { return V2fx4(select(mask, a.x, b.x), select(mask, a.y, b.y)); }
FORCEINLINE v2fx8 select(b32x8 mask, v2fx8 a, v2fx8 b) { return V2fx8(select(mask, a.x, b.x), select(mask, a.y, b.y)); }
FORCEINLINE v3fx4 select(b32x4 mask, v3fx4 a, v3fx4 b) { return V3fx4(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z)); }
FORCEINLINE v3fx8 select(b32x8 mask, v3fx8 a, v3fx8 b) { return V3fx8(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z)); }
FORCEINLINE v4fx4 select(b32x4 mask, v4fx4 a, v4fx4 b) { return V4fx4(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z), select(mask, a.w, b.w)); }
FORCEINLINE v4fx8 select(b32x8 mask, v4fx8 a, v4fx8 b) { return V4fx8(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z), select(mask, a.w, b.w)); }

FORCEINLINE f32x4 select(b32x4 mask, f32 a, f32x4 b) { return select(mask, F32x4(a), b); }
FORCEINLINE f32x8 select(b32x8 mask, f32 a, f32x8 b) { return select(mask, F32x8(a), b); }
FORCEINLINE s32x4 select(b32x4 mask, s32 a, s32x4 b) { return select(mask, S32x4(a), b); }
FORCEINLINE s32x8 select(b32x8 mask, s32 a, s32x8 b) { return select(mask, S32x8(a), b); }
FORCEINLINE u32x4 select(b32x4 mask, u32 a, u32x4 b) { return select(mask, U32x4(a), b); }
FORCEINLINE u32x8 select(b32x8 mask, u32 a, u32x8 b) { return select(mask, U32x8(a), b); }

FORCEINLINE f32x4 select(b32x4 mask, f32x4 a, f32 b) { return select(mask, a, F32x4(b)); }
FORCEINLINE f32x8 select(b32x8 mask, f32x8 a, f32 b) { return select(mask, a, F32x8(b)); }
FORCEINLINE s32x4 select(b32x4 mask, s32x4 a, s32 b) { return select(mask, a, S32x4(b)); }
FORCEINLINE s32x8 select(b32x8 mask, s32x8 a, s32 b) { return select(mask, a, S32x8(b)); }
FORCEINLINE u32x4 select(b32x4 mask, u32x4 a, u32 b) { return select(mask, a, U32x4(b)); }
FORCEINLINE u32x8 select(b32x8 mask, u32x8 a, u32 b) { return select(mask, a, U32x8(b)); }

FORCEINLINE f32x4 select(b32x4 mask, f32 a, f32 b) { return select(mask, F32x4(a), F32x4(b)); }
FORCEINLINE f32x8 select(b32x8 mask, f32 a, f32 b) { return select(mask, F32x8(a), F32x8(b)); }
FORCEINLINE s32x4 select(b32x4 mask, s32 a, s32 b) { return select(mask, S32x4(a), S32x4(b)); }
FORCEINLINE s32x8 select(b32x8 mask, s32 a, s32 b) { return select(mask, S32x8(a), S32x8(b)); }
FORCEINLINE u32x4 select(b32x4 mask, u32 a, u32 b) { return select(mask, U32x4(a), U32x4(b)); }
FORCEINLINE u32x8 select(b32x8 mask, u32 a, u32 b) { return select(mask, U32x8(a), U32x8(b)); }

#define FRAC(constexpr, f32, s32)	\
FORCEINLINE constexpr f32 frac(f32 v) {	\
	v = v - (f32)(s32)v;			\
	return select(v < 0, v + 1, v);	\
}
FRAC(constexpr, f32, s32)
FRAC(constexpr, f64, s64)
FRAC(, f32x4, s32x4)
FRAC(, f32x8, s32x8)
#undef FRAC

FORCEINLINE v4f frac(v4f v) { return V4f(frac(v.m)); }
FORCEINLINE v4fx4 frac(v4fx4 v) { return V4fx4(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }
FORCEINLINE v4fx8 frac(v4fx8 v) { return V4fx8(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }

FORCEINLINE v2f frac(v2f v) { return frac(V4f(v, 0, 0)).xy; }
FORCEINLINE v2fx4 frac(v2fx4 v) { return V2fx4(frac(v.x), frac(v.y)); }
FORCEINLINE v2fx8 frac(v2fx8 v) { return V2fx8(frac(v.x), frac(v.y)); }

FORCEINLINE v3f frac(v3f v) { return frac(V4f(v, 0)).xyz; }
FORCEINLINE v3fx4 frac(v3fx4 v) { return V3fx4(frac(v.x), frac(v.y), frac(v.z)); }
FORCEINLINE v3fx8 frac(v3fx8 v) { return V3fx8(frac(v.x), frac(v.y), frac(v.z)); }

FORCEINLINE constexpr s32 frac(s32 v, s32 s) {
	if (v < 0)
		return (v + 1) % s + s - 1;
	return v % s;
}
FORCEINLINE s32x4 frac(s32x4 v, s32x4 s) {
#if ARCH_AVX
	__m256d vd = _mm256_cvtepi32_pd(v.m.pi);
	__m256d sd = _mm256_cvtepi32_pd(s.m.pi);
	vd = _mm256_floor_pd(_mm256_div_pd(vd, sd));
	__m128i result = _mm256_cvtipd_epi32(vd);
#else
	__m128d vlo = _mm_cvtepi32_pd(v.m.pi);
	__m128d vhi = _mm_cvtepi32_pd(_mm_castpd_pi(_mm_shuffle_pd(v.m.pd, v.m.pd, 0b11)));
	__m128d slo = _mm_cvtepi32_pd(s.m.pi);
	__m128d shi = _mm_cvtepi32_pd(_mm_castpd_pi(_mm_shuffle_pd(s.m.pd, s.m.pd, 0b11)));
	vlo = _mm_floor_pd(_mm_div_pd(vlo, slo));
	vhi = _mm_floor_pd(_mm_div_pd(vhi, shi));
	__m128i result = _mm_cvtipd_epi32(vlo);
	result = _mm_insert_epi64(result, _mm_extract_epi64(_mm_cvtipd_epi32(vhi), 0), 1);
#endif
	return {_mm_castpi_ps(_mm_sub_epi32(v.m.pi, _mm_mullo_epi32(result, s.m.pi)))};
}
FORCEINLINE s32x4 frac(s32x4 v, s32 s) { return frac(v, {_mm_castpi_ps(_mm_set1_epi32(s))}); }

FORCEINLINE v2s frac(v2s v, v2s s) {
	M16 vm = {_mm_castpi_ps(_mm_setr_epi32(v.x, v.y, 0, 0))};
	M16 sm = {_mm_castpi_ps(_mm_setr_epi32(s.x, s.y, 0, 0))};
	__m128d vlo = _mm_cvtepi32_pd(vm.pi);
	__m128d slo = _mm_cvtepi32_pd(sm.pi);
	__m128i result = _mm_cvtipd_epi32(_mm_floor_pd(_mm_div_pd(vlo, slo)));
	result = _mm_sub_epi32(vm.pi, _mm_mullo_epi32(result, sm.pi));
	return *(v2s *)&result;
}
FORCEINLINE v3s frac(v3s v, v3s s) {
	return {
		frac(v.x, s.x),
		frac(v.y, s.y),
		frac(v.z, s.z),
	};
}
FORCEINLINE v4s frac(v4s v, v4s step) {
	return {
		frac(v.x, step.x),
		frac(v.y, step.y),
		frac(v.z, step.z),
		frac(v.w, step.w),
	};
}
FORCEINLINE v2s frac(v2s v, s32 s) { return frac(v, V2s(s)); }
FORCEINLINE v3s frac(v3s v, s32 s) { return frac(v, V3s(s)); }
FORCEINLINE v4s frac(v4s v, s32 s) { return frac(v, V4s(s)); }

FORCEINLINE f32 floor(f32 v) { return ::floorf(v); }
FORCEINLINE f64 floor(f64 v) { return ::floor(v); }
FORCEINLINE v2f floor(v2f v) {
	return {
		floor(v.x),
		floor(v.y),
	};
}
FORCEINLINE v3f floor(v3f v) {
	return {
		floor(v.x),
		floor(v.y),
		floor(v.z),
	};
}

FORCEINLINE f32x4 floor(f32x4 v) { return {_mm_round_ps(v.m.ps, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)}; }
FORCEINLINE f32x4  ceil(f32x4 v) { return {_mm_round_ps(v.m.ps, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)}; }
#if ARCH_AVX
FORCEINLINE f32x8 floor(f32x8 v) { return {_mm256_round_ps(v.m.ps, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)}; }
FORCEINLINE f32x8  ceil(f32x8 v) { return {_mm256_round_ps(v.m.ps, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)}; }
#else
FORCEINLINE f32x8 floor(f32x8 v) { return {_mm_round_ps(v.lo.ps, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC), _mm_round_ps(v.hi.ps, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)}; }
FORCEINLINE f32x8  ceil(f32x8 v) { return {_mm_round_ps(v.lo.ps, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC), _mm_round_ps(v.hi.ps, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)}; }
#endif

FORCEINLINE v4f floor(v4f v) { return V4f(floor(v.m)); }
FORCEINLINE constexpr s32 floor(s32 v, s32 s) {
	if (v < 0)
		v = (v + 1) / s - 1;
	else
		v /= s;
	return v * s;
}
FORCEINLINE s32x4 floor(s32x4 v, s32x4 s) {
	__m128i n = _mm_add_epi32(v.m.pi, _mm_set1_epi32(1));//   / s - 1;
#if ARCH_AVX
	__m256d vd = _mm256_cvtepi32_pd(v.m.pi);
	__m256d sd = _mm256_cvtepi32_pd(s.m.pi);

	__m256d nd = _mm256_cvtepi32_pd(n);
	n = _mm_sub_epi32(_mm256_cvtipd_epi32(_mm256_round_pd(_mm256_div_pd(nd, sd), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)), _mm_set1_epi32(1));

	__m128i p = _mm256_cvtipd_epi32(_mm256_div_pd(vd, sd));
#else
	__m128d vlo = _mm_cvtepi32_pd(v.m.pi);
	__m128d vhi = _mm_cvtepi32_pd(_mm_shuffle_epi32(v.m.pi, MM_SHUFFLE(2,3,0,0)));
	__m128d slo = _mm_cvtepi32_pd(s.m.pi);
	__m128d shi = _mm_cvtepi32_pd(_mm_shuffle_epi32(s.m.pi, MM_SHUFFLE(2,3,0,0)));

	__m128d ndlo = _mm_cvtepi32_pd(n);
	__m128d ndhi = _mm_cvtepi32_pd(_mm_shuffle_epi32(n, MM_SHUFFLE(2,3,0,0)));
	auto nlo = _mm_cvtipd_epi32(_mm_div_pd(ndlo, slo));
	auto nhi = _mm_cvtipd_epi32(_mm_div_pd(ndhi, shi));
	n = _mm_insert_epi64(nlo, _mm_extract_epi64(nhi, 0), 1);
	n = _mm_sub_epi32(n, _mm_set1_epi32(1));
	
	auto plo = _mm_cvtipd_epi32(_mm_div_pd(vlo, slo));
	auto phi = _mm_cvtipd_epi32(_mm_div_pd(vhi, shi));
	__m128i p = _mm_insert_epi64(plo, _mm_extract_epi64(phi, 0), 1);
#endif
	v.m.pi = _mm_mullo_epi32(_mm_blendv_epi32(p, n, _mm_cmplt_epi32(v.m.pi, _mm_set1_epi32(0))), s.m.pi);
	return v;
}
FORCEINLINE v2s floor(v2s v, v2s s) {
	auto result = floor(S32x4(v.x, v.y, 0, 0), S32x4(s.x, s.y, 0, 0));
	return *(v2s *)&result;
	return {
		floor(v.x, s.x),
		floor(v.y, s.y),
	};
}
FORCEINLINE v3s floor(v3s v, v3s s) {
	return {
		floor(v.x, s.x),
		floor(v.y, s.y),
		floor(v.z, s.z),
	};
}
FORCEINLINE v4s floor(v4s v, v4s s) {
#if 1
	// ~10 cy
	return {
		floor(v.x, s.x),
		floor(v.y, s.y),
		floor(v.z, s.z),
		floor(v.w, s.w),
	};
#else
	// ~37 cy
	s32x4 ms	 = S32x4((s32)step);
	s32x4 pos	 = v.m / ms;
	s32x4 neg	 = (v.m + 1) / ms - 1;
	__m128i mask = _mm_cmplt_epi32(v.m.m, _mm_setzero_pi());
	return V4s(S32x4(_mm_blendv_epi32(pos.m, neg.m, mask)) * (s32)step);
#endif
}

FORCEINLINE s32x4 floor(s32x4 v, s32 s) { return floor(v, S32x4(s)); }
FORCEINLINE v2s floor(v2s v, s32 s) { return floor(v, V2s(s)); }
FORCEINLINE v3s floor(v3s v, s32 s) { return floor(v, V3s(s)); }
FORCEINLINE v4s floor(v4s v, s32 s) { return floor(v, V4s(s)); }

FORCEINLINE s32 floorToInt(f32 v) { return (s32)floor(v); }
FORCEINLINE s64 floorToInt(f64 v) { return (s64)floor(v); }
FORCEINLINE v2s floorToInt(v2f v) {
	return {
		floorToInt(v.x),
		floorToInt(v.y),
	};
}
FORCEINLINE v3s floorToInt(v3f v) {
	return {
		floorToInt(v.x),
		floorToInt(v.y),
		floorToInt(v.z),
	};
}
FORCEINLINE s32x4 floorToInt(f32x4 v) { return (s32x4)floor(v); }
FORCEINLINE v4s floorToInt(v4f v) { return V4s(floorToInt(v.m)); }

FORCEINLINE f32 ceil(f32 v) { return ceilf(v); }
FORCEINLINE f64 ceil(f64 v) { return ::ceil(v); }

FORCEINLINE v2f ceil(v2f v) { return {ceil(v.x), ceil(v.y)}; }
FORCEINLINE v3f ceil(v3f v) { return {ceil(v.x), ceil(v.y), ceil(v.z)}; }
FORCEINLINE v4f ceil(v4f v) { return {ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)}; }

#define CEIL(t) \
	FORCEINLINE t ceil(t v, t s) { return floor(v + s - 1, s); }
constexpr CEIL(s32) CEIL(v2s) CEIL(v3s) CEIL(v4s);
#undef CEIL

#define CEIL(s32, f32) FORCEINLINE s32 ceilToInt(f32 v) { return (s32)ceil(v); }

CEIL(s32, f32)// CEIL(s32x4, f32x4) CEIL(s32x8, f32x8);
CEIL(s64, f64)// CEIL(s32x4, f32x4) CEIL(s32x8, f32x8);
CEIL(v2s, v2f)// CEIL(v2sx4, v2fx4) CEIL(v2sx4, v2fx4);
CEIL(v3s, v3f)// CEIL(v3sx4, v3fx4) CEIL(v3sx4, v3fx4);
CEIL(v4s, v4f)// CEIL(v4sx4, v4fx4) CEIL(v4sx4, v4fx4);

FORCEINLINE v2fx4 floor(v2fx4 v) { return V2fx4(floor(v.x), floor(v.y)); }
FORCEINLINE v2fx8 floor(v2fx8 v) { return V2fx8(floor(v.x), floor(v.y)); }
FORCEINLINE v3fx4 floor(v3fx4 v) { return V3fx4(floor(v.x), floor(v.y), floor(v.z)); }
FORCEINLINE v3fx8 floor(v3fx8 v) { return V3fx8(floor(v.x), floor(v.y), floor(v.z)); }
FORCEINLINE v4fx4 floor(v4fx4 v) { return V4fx4(floor(v.x), floor(v.y), floor(v.z), floor(v.w)); }
FORCEINLINE v4fx8 floor(v4fx8 v) { return V4fx8(floor(v.x), floor(v.y), floor(v.z), floor(v.w)); }
FORCEINLINE v2fx4 ceil(v2fx4 v) { return V2fx4(ceil(v.x), ceil(v.y)); }
FORCEINLINE v2fx8 ceil(v2fx8 v) { return V2fx8(ceil(v.x), ceil(v.y)); }
FORCEINLINE v3fx4 ceil(v3fx4 v) { return V3fx4(ceil(v.x), ceil(v.y), ceil(v.z)); }
FORCEINLINE v3fx8 ceil(v3fx8 v) { return V3fx8(ceil(v.x), ceil(v.y), ceil(v.z)); }
FORCEINLINE v4fx4 ceil(v4fx4 v) { return V4fx4(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)); }
FORCEINLINE v4fx8 ceil(v4fx8 v) { return V4fx8(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)); }

#define ROUND(f32) FORCEINLINE f32 round(f32 v) { return floor(v + 0.5f); }
ROUND(f32) ROUND(f32x4) ROUND(f32x8) ROUND(v2f) ROUND(v3f) ROUND(v4f);
#undef ROUND
FORCEINLINE v2fx4 round(v2fx4 v) { v.x = round(v.x); v.y = round(v.y); return v; }
FORCEINLINE v2fx8 round(v2fx8 v) { v.x = round(v.x); v.y = round(v.y); return v; }
FORCEINLINE v3fx4 round(v3fx4 v) { v.x = round(v.x); v.y = round(v.y); v.z = round(v.z); return v; }
FORCEINLINE v3fx8 round(v3fx8 v) { v.x = round(v.x); v.y = round(v.y); v.z = round(v.z); return v; }
FORCEINLINE v4fx4 round(v4fx4 v) { v.x = round(v.x); v.y = round(v.y); v.z = round(v.z); v.w = round(v.w); return v; }
FORCEINLINE v4fx8 round(v4fx8 v) { v.x = round(v.x); v.y = round(v.y); v.z = round(v.z); v.w = round(v.w); return v; }

FORCEINLINE s32 roundToInt(f32 v) { return (s32)lroundf(v); }
FORCEINLINE s64 roundToInt(f64 v) { return llround(v); }
FORCEINLINE v2s roundToInt(v2f v) { return {roundToInt(v.x), roundToInt(v.y)}; }
FORCEINLINE v3s roundToInt(v3f v) { return {roundToInt(v.x), roundToInt(v.y), roundToInt(v.z)}; }
FORCEINLINE v4s roundToInt(v4f v) { return (v4s)round(v); }
FORCEINLINE v2sx8 roundToInt(v2fx8 v) { return (v2sx8)round(v); }

FORCEINLINE f32 absolute(f32 v) { return *(u32*)&v &= 0x7FFFFFFF, v; }
FORCEINLINE u32 absolute(u32 v) { return v; }
FORCEINLINE s32 absolute(s32 v) { if (v < 0) v = -v; return v; }
FORCEINLINE f32x4 absolute(f32x4 v) { return {_mm_and_ps(v.m.ps, _mm_castpi_ps(_mm_set1_epi32(0x7FFFFFFF)))}; }
#if ARCH_AVX
FORCEINLINE f32x8 absolute(f32x8 v) { return {_mm256_and_ps(v.m.ps, _mm256_castpi_ps(_mm256_set1_epi32(0x7FFFFFFF)))}; }
#else
FORCEINLINE f32x8 absolute(f32x8 v) { return {_mm_and_ps(v.lo.ps, _mm_castpi_ps(_mm_set1_epi32(0x7FFFFFFF))), _mm_and_ps(v.hi.ps, _mm_castpi_ps(_mm_set1_epi32(0x7FFFFFFF)))}; }
#endif

FORCEINLINE v2f absolute(v2f v) { return {fabsf(v.x), fabsf(v.y)}; }
FORCEINLINE v3f absolute(v3f v) { return {fabsf(v.x), fabsf(v.y), fabsf(v.z)}; }
FORCEINLINE v4f absolute(v4f v) { return {fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w)}; }
FORCEINLINE v2fx4 absolute(v2fx4 v) { return V2fx4(absolute(v.x), absolute(v.y)); }
FORCEINLINE v2fx8 absolute(v2fx8 v) { return V2fx8(absolute(v.x), absolute(v.y)); }

FORCEINLINE v2s absolute(v2s a) { return {labs(a.x), labs(a.y)}; }
FORCEINLINE v3s absolute(v3s a) { return {labs(a.x), labs(a.y), labs(a.z)}; }
FORCEINLINE s32x4 absolute(s32x4 a) { return {_mm_castpi_ps(_mm_abs_epi32(a.m.pi))}; }
FORCEINLINE v4s absolute(v4s a) { return V4s(absolute(a.m)); }

FORCEINLINE f32 setSign(f32 dst, f32 src) {
    *(u32*)&dst &= 0x7FFFFFFF;
    *(u32*)&dst |= *(u32*)&src & 0x80000000;
    return dst;
}
FORCEINLINE f64 setSign(f64 dst, f64 src) {
    *(u64*)&dst &= 0x7FFFFFFFFFFFFFFF;
    *(u64*)&dst |= *(u64*)&src & 0x8000000000000000;
    return dst;
}
template<umm ps>
FORCEINLINE f32x<ps> setSign(f32x<ps> dst, f32x<ps> src) {
    *(u32x<ps>*)&dst &= U32x<ps>(0x7FFFFFFF);
    *(u32x<ps>*)&dst |= *(u32x<ps>*)&src & U32x<ps>(0x80000000);
    return dst;
}

FORCEINLINE f32 sign(f32 v) { return setSign(1.0f, v); }
FORCEINLINE v2f sign(v2f v) { v.x = setSign(1.0f, v.x); v.y = setSign(1.0f, v.y); return v; }
template<umm ps> FORCEINLINE f32x<ps> sign(f32x<ps> v) { return setSign(F32x<ps>(1.0f), v); }
template<umm ps> FORCEINLINE v2fx<ps> sign(v2fx<ps> v) { v.x = setSign(F32x<ps>(1.0f), v.x); v.y = setSign(F32x<ps>(1.0f), v.y); return v; }

#define TRUNC(f32) FORCEINLINE f32 trunc(f32 v) { return v > 0 ? floor(v) : ceil(v); }
TRUNC(f32)
TRUNC(f64)
#undef TRUNC

template<umm ps>
FORCEINLINE f32x<ps> trunc(f32x<ps> v) { return select(v > 0, floor(v), ceil(v)); }

#define MOD(f32) \
FORCEINLINE f32 modulo(f32 a, f32 b) { return setSign(a - trunc(a / b) * b, a); } \
FORCEINLINE f32 positiveModulo(f32 a, f32 b) { return frac(a / b) * b; }
MOD(f32)
MOD(f64)
#undef MOD

template<umm ps> FORCEINLINE f32x<ps> modulo(f32x<ps> a, f32x<ps> b) { return setSign(a - trunc(a * reciprocal(b)) * b, a); }
template<umm ps> FORCEINLINE f32x<ps> positiveModulo(f32x<ps> a, f32x<ps> b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE v2fx<ps> positiveModulo(v2fx<ps> a, f32x<ps> b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE v3fx<ps> positiveModulo(v3fx<ps> a, f32x<ps> b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE v4fx<ps> positiveModulo(v4fx<ps> a, f32x<ps> b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE f32x<ps> positiveModulo(f32x<ps> a, f32 b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE v2fx<ps> positiveModulo(v2fx<ps> a, f32 b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE v3fx<ps> positiveModulo(v3fx<ps> a, f32 b) { return frac(a * reciprocal(b)) * b; }
template<umm ps> FORCEINLINE v4fx<ps> positiveModulo(v4fx<ps> a, f32 b) { return frac(a * reciprocal(b)) * b; }

FORCEINLINE f32 sqrt(f32 v) { return sqrtf(v); }
FORCEINLINE v2f sqrt(v2f v) { return V2f(sqrtf(v.x), sqrtf(v.y)); }
FORCEINLINE v3f sqrt(v3f v) { return V3f(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z)); }
FORCEINLINE f32x4 reciprocal(f32x4 v) { return {_mm_rcp_ps(v.m.ps)}; }
FORCEINLINE f32x4 sqrt(f32x4 v) { return {_mm_sqrt_ps(v.m.ps)}; }
#if ARCH_AVX
FORCEINLINE f32x8 sqrt(f32x8 v) { return {_mm256_sqrt_ps(v.m.ps)}; }
FORCEINLINE f32x8 reciprocal(f32x8 v) { return {_mm256_rcp_ps(v.m.ps)}; }
#else
FORCEINLINE f32x8 sqrt(f32x8 v) { return {_mm_sqrt_ps(v.lo.ps), _mm_sqrt_ps(v.hi.ps)}; }
FORCEINLINE f32x8 reciprocal(f32x8 v) { return {_mm_rcp_ps(v.lo.ps), _mm_rcp_ps(v.hi.ps)}; }
#endif
FORCEINLINE v4f sqrt(v4f v) { return V4f(sqrt(v.m)); }

FORCEINLINE f32 reciprocal(f32 v) { return 1.0f / v; }

FORCEINLINE f32 sin(f32 v) { return ::sinf(v); }
FORCEINLINE f32 cos(f32 v) { return ::cosf(v); }
#if COMPILER_GCC
FORCEINLINE v2f sin(v2f v) { return V2f(sinf(v.x), sinf(v.y)); }
FORCEINLINE v3f sin(v3f v) { return V3f(sinf(v.x), sinf(v.y), sinf(v.z)); }
FORCEINLINE v4f sin(v4f v) { return V4f(sinf(v.x), sinf(v.y), sinf(v.z), sinf(v.w)); }
FORCEINLINE v2f cos(v2f v) { return V2f(cosf(v.x), cosf(v.y)); }
FORCEINLINE v3f cos(v3f v) { return V3f(cosf(v.x), cosf(v.y), cosf(v.z)); }
FORCEINLINE v4f cos(v4f v) { return V4f(cosf(v.x), cosf(v.y), cosf(v.z), cosf(v.w)); }
FORCEINLINE void sincos(v2f v, v2f& sinOut, v2f& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
FORCEINLINE void sincos(v3f v, v3f& sinOut, v3f& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
FORCEINLINE void sincos(v4f v, v4f& sinOut, v4f& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
#else
FORCEINLINE f32x4 sin(f32x4 v) { return {_mm_sin_ps(v.m.ps)}; }
FORCEINLINE f32x4 cos(f32x4 v) { return {_mm_cos_ps(v.m.ps)}; }
#if ARCH_AVX
FORCEINLINE f32x8 sin(f32x8 v) { return {_mm256_sin_ps(v.m.ps)}; }
FORCEINLINE f32x8 cos(f32x8 v) { return {_mm256_cos_ps(v.m.ps)}; }
#else
FORCEINLINE f32x8 sin(f32x8 v) { return {_mm_sin_ps(v.lo.ps), _mm_sin_ps(v.hi.ps)}; }
FORCEINLINE f32x8 cos(f32x8 v) { return {_mm_cos_ps(v.lo.ps), _mm_cos_ps(v.hi.ps)}; }
#endif
FORCEINLINE v4f sin(v4f v) { return V4f(sin(v.m)); }
FORCEINLINE v2f sin(v2f v) { return sin(V4f(v, 0, 0)).xy; }
FORCEINLINE v3f sin(v3f v) { return sin(V4f(v, 0)).xyz; }

#define SIN_BHASKARA(f32x4) 															\
FORCEINLINE f32x4 sinBhaskara(f32x4 v) {														\
	v = positiveModulo(v, pi * 2);														\
	auto mask = v >= pi;																\
	v = select(mask, v - pi, v);														\
	return (16*v*(pi-v))*reciprocal(5*pi*pi-4*v*(pi-v)) * select(mask, -1.0f, 1.0f);	\
}
SIN_BHASKARA(f32)
SIN_BHASKARA(f32x4)
SIN_BHASKARA(f32x8)
#undef SIN_BHASKARA

#define COS_BHASKARA(f32x4) FORCEINLINE f32x4 cosBhaskara(f32x4 v) { return sinBhaskara(v + pi*0.5f); }
COS_BHASKARA(f32)
COS_BHASKARA(f32x4)
COS_BHASKARA(f32x8)
#undef COS_BHASKARA

#define SINCOS_BHASKARA(v2fx4, V2fx4, f32x4) FORCEINLINE v2fx4 sincosBhaskara(f32x4 v) { return V2fx4(cosBhaskara(v), sinBhaskara(v)); }
SINCOS_BHASKARA(v2f  , V2f  , f32)
SINCOS_BHASKARA(v2fx4, V2fx4, f32x4)
SINCOS_BHASKARA(v2fx8, V2fx8, f32x8)
#undef SINCOS_BHASKARA

FORCEINLINE v4f cos(v4f v) { return V4f(cos(v.m)); }
FORCEINLINE v2f cos(v2f v) { return cos(V4f(v, 0, 0)).xy; }
FORCEINLINE v3f cos(v3f v) { return cos(V4f(v, 0)).xyz; }

FORCEINLINE void sincos(f32 v, f32& sinOut, f32& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
#define SINCOS(v2f, F32x4)				 \
FORCEINLINE void sincos(f32 v, v2f& result) { \
	result.y = F32x4(sin(v));			 \
	result.x = F32x4(cos(v));			 \
}										   
SINCOS(v2fx4, F32x4) SINCOS(v2fx8, F32x8)
#undef SINCOS

#define SINCOS(v2fx4, f32x4)				 \
FORCEINLINE void sincos(f32x4 v, v2fx4& result) { \
	result.y = sin(v);					     \
	result.x = cos(v);					     \
}
SINCOS(v2f, f32) SINCOS(v2fx4, f32x4) SINCOS(v2fx8, f32x8)
#undef SINCOS

FORCEINLINE void sincos(v2f v, v2f& sinOut, v2f& cosOut) {
	__m128 c, s = _mm_sincos_ps(&c, _mm_setr_ps(v.x, v.y, 0, 0));
	memcpy(&sinOut, &s, sizeof(sinOut));
	memcpy(&cosOut, &c, sizeof(cosOut));
}
FORCEINLINE void sincos(v3f v, v3f& sinOut, v3f& cosOut) {
	__m128 c, s = _mm_sincos_ps(&c, _mm_setr_ps(v.x, v.y, v.z, 0));
	memcpy(&sinOut, &s, sizeof(sinOut));
	memcpy(&cosOut, &c, sizeof(cosOut));
}
FORCEINLINE void sincos(f32x4 v, f32x4& sinOut, f32x4& cosOut) { sinOut.m.ps = _mm_sincos_ps(&cosOut.m.ps, v.m.ps); }
#if ARCH_AVX
FORCEINLINE void sincos(f32x8 v, f32x8& sinOut, f32x8& cosOut) { sinOut.m.ps = _mm256_sincos_ps(&cosOut.m.ps, v.m.ps); }
#else
FORCEINLINE void sincos(f32x8 v, f32x8& sinOut, f32x8& cosOut) { 
	sinOut.lo.ps = _mm_sincos_ps(&cosOut.lo.ps, v.lo.ps);
	sinOut.hi.ps = _mm_sincos_ps(&cosOut.hi.ps, v.hi.ps);
}
#endif
FORCEINLINE void sincos(v4f v, v4f& sinOut, v4f& cosOut) { sincos(v.m, sinOut.m, cosOut.m); }
#endif

FORCEINLINE v2f sincos(f32 v) { return {cos(v), sin(v)}; }
FORCEINLINE v2fx4 sincos(f32x4 v) { v2fx4 result; sincos(v, result.y, result.x); return result; }
FORCEINLINE v2fx8 sincos(f32x8 v) { v2fx8 result; sincos(v, result.y, result.x); return result; }

FORCEINLINE f32 atan2(f32 y, f32 x) { return ::atan2f(y, x); }
FORCEINLINE f32 atan2(v2f v) { return atan2(v.y, v.x); }

FORCEINLINE f32x4 atan2(f32x4 y, f32x4 x) { return {_mm_atan2_ps(y.m.ps, x.m.ps)}; }
FORCEINLINE f32x4 atan2(v2fx4 v) { return atan2(v.y, v.x); }

#if ARCH_AVX
FORCEINLINE f32x8 atan2(f32x8 y, f32x8 x) { return {_mm256_atan2_ps(y.m.ps, x.m.ps)}; }
#else
FORCEINLINE f32x8 atan2(f32x8 y, f32x8 x) { return {_mm_atan2_ps(y.lo.ps, x.lo.ps), _mm_atan2_ps(y.hi.ps, x.hi.ps)}; }
#endif
FORCEINLINE f32x8 atan2(v2fx8 v) { return atan2(v.y, v.x); }

#define ATAN2_APPROX(f32, v2f)										 \
f32 atan2Approx(f32 y, f32 x) {										 \
	auto ay = absolute(y) + 1e-10f;									 \
	auto mask = x < 0;												 \
	auto r = select(mask, (x + ay) / (ay - x), (x - ay) / (ay + x)); \
	auto angle = select(mask, pi * 0.75f, pi * 0.25f);				 \
	angle += (0.1963f * r * r - 0.9817f) * r;						 \
	return select(y < 0, -angle, angle);							 \
}																	 \
FORCEINLINE f32 atan2Approx(v2f v) { return atan2Approx(v.y, v.x); }
ATAN2_APPROX(f32, v2f)
ATAN2_APPROX(f32x4, v2fx4)
ATAN2_APPROX(f32x8, v2fx8)
#undef ATAN2_APPROX

FORCEINLINE u32 findLowestOneBit(u32 val) {
	unsigned long result;
	return _BitScanForward(&result, (unsigned long)val) ? (u32)result : ~0;
}
FORCEINLINE u32 findLowestOneBit(u64 val) {
	unsigned long result;
	return _BitScanForward64(&result, val) ? (u32)result : ~0;
}
FORCEINLINE u32 findHighestOneBit(u32 val) {
	unsigned long result;
	return _BitScanReverse(&result, (unsigned long)val) ? (u32)result : ~0;
}
FORCEINLINE u32 findHighestOneBit(u64 val) {
	unsigned long result;
	return _BitScanReverse64(&result, val) ? (u32)result : ~0;
}
FORCEINLINE u32 countBits(u32 v) { return (u32)_mm_popcnt_u32(v); }
FORCEINLINE u32 countBits(s32 v) { return countBits((u32)v); }
#if ARCH_AVX2
FORCEINLINE u32 countBits(u64 v) { return (u32)_mm_popcnt_u64(v); }
FORCEINLINE u32 countBits(s64 v) { return countBits((u64)v); }
#else
FORCEINLINE u32 countBits(u64 v) { return countBits((u32)v) + countBits((u32)(v >> 32)); }
FORCEINLINE u32 countBits(s64 v) { return countBits((u64)v); }
#endif
FORCEINLINE u32 fillBits(u32 v) {
	u32 r = 0;
	while (v--) {
		r = (r << 1) | 1;
	}
	return r;
}
FORCEINLINE f32 dot(f32 a, f32 b) { return a * b; }
FORCEINLINE f32 dot(f32x4 a, f32x4 b) { return {_mm_cvtss_f32(_mm_dp_ps(a.m.ps, b.m.ps, 0xFF))}; }
FORCEINLINE s32 dot(s32x4 a, s32x4 b) {
	a *= b;
	return a[0] + a[1] + a[2] + a[3];
}
FORCEINLINE u32 dot(u32x4 a, u32x4 b) {
	a *= b;
	return a[0] + a[1] + a[2] + a[3];
}

#define DOT(f32, v2f) FORCEINLINE f32 dot(v2f a, v2f b) { return a *= b, a.x + a.y; }
DOT(f32, v2f) DOT(f32x4, v2fx4) DOT(f32x8, v2fx8);
DOT(s32, v2s) DOT(s32x4, v2sx4) DOT(s32x8, v2sx8);
DOT(u32, v2u) DOT(u32x4, v2ux4) DOT(u32x8, v2ux8);
#undef DOT

#define DOT(f32, v3f) FORCEINLINE f32 dot(v3f a, v3f b) { return a *= b, a.x + a.y + a.z; }
DOT(f32, v3f) DOT(f32x4, v3fx4) DOT(f32x8, v3fx8);
DOT(s32, v3s) DOT(s32x4, v3sx4) DOT(s32x8, v3sx8);
DOT(u32, v3u) DOT(u32x4, v3ux4) DOT(u32x8, v3ux8);
#undef DOT

FORCEINLINE f32 dot(v4f a, v4f b) { return dot(a.m, b.m); }
FORCEINLINE s32 dot(v4s a, v4s b) { return dot(a.m, b.m); }
FORCEINLINE u32 dot(v4u a, v4u b) { return dot(a.m, b.m); }

#define DOT(f32, v4f)                 \
	FORCEINLINE f32 dot(v4f a, v4f b) {           \
		a *= b;                       \
		return a.x + a.y + a.z + a.w; \
	}
DOT(f32x4, v4fx4) DOT(f32x8, v4fx8);
DOT(s32x4, v4sx4) DOT(s32x8, v4sx8);
DOT(u32x4, v4ux4) DOT(u32x8, v4ux8);
#undef DOT

template <class T>
FORCEINLINE auto reflect(T v, T n) {
	return v - dot(v, n) * n * 2;
}

FORCEINLINE v2f cross(v2f a) { return {a.y, -a.x}; }
FORCEINLINE v2fx4 cross(v2fx4 a) { return {a.y, -a.x}; }
FORCEINLINE v2fx8 cross(v2fx8 a) { return {a.y, -a.x}; }
FORCEINLINE v2s cross(v2s a) { return {a.y, -a.x}; }
FORCEINLINE v2sx4 cross(v2sx4 a) { return {a.y, -a.x}; }
FORCEINLINE v2sx8 cross(v2sx8 a) { return {a.y, -a.x}; }
FORCEINLINE v3f cross(v3f a, v3f b) {
	// clang-format off
	return {
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z, 
		a.x * b.y - a.y * b.x
	};
	// clang-format on
}

FORCEINLINE f32 sum(v2f v) { return v.x + v.y; }

template <class T>
FORCEINLINE auto lengthSqr(T a) {
	return dot(a, a);
}
template <class T>
FORCEINLINE auto length(T a) {
	return sqrt(lengthSqr(a));
}
template <class T>
FORCEINLINE auto normalize(T a) {
	return a * (1.0f / length(a));
}
template <class T>
FORCEINLINE auto normalize(T a, T fallback) {
	auto lsq = lengthSqr(a);
	if (lsq == decltype(lsq){})
		return fallback;
	return a * (1.0f / sqrt(lsq));
}
template <class T>
FORCEINLINE auto distanceSqr(T a, T b) {
	return lengthSqr(a - b);
}
template <class T>
FORCEINLINE auto distance(T a, T b) {
	return sqrt(distanceSqr(a, b));
}
template <class T>
FORCEINLINE auto manhattan(T a, T b) {
	return sum(abs(a - b));
}
FORCEINLINE int maxDistance(v3s a, v3s b) {
	a = absolute(a - b);
	return max(max(a.x, a.y), a.z);
}

template <class T>
FORCEINLINE constexpr auto moveTowards(T a, T b, f32 t) {
	auto d = b - a;
	if (lengthSqr(d) > t * t) {
		return a + normalize(d) * t;
	} else {
		return b;
	}
}

template <class T>
FORCEINLINE constexpr auto moveAway(T a, T b, f32 t) {
	return a + normalize(a - b) * t;
}

FORCEINLINE f32 cos01(f32 t) { return 0.5f - cosf(t * pi) * 0.5f; }
/*
f32 noise(s32 x) {
	x = (x << 13) ^ x;
	return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}
f32 noise(v2s v) {
	auto n = v.x + v.y * 57;
	n	   = (n << 13) ^ n;
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483648.0f);
}
FORCEINLINE f32 cosNoise(f32 v) {
	auto fl = (s32)floor(v);
	return lerp(noise(fl), noise(fl + 1), cos01(frac(v)));
}
template <class Interp, class Sample>
FORCEINLINE f32 interpolate(v2f p, Interp&& interp, Sample&& sample) {
	auto x	= (s32)floor(p.x);
	auto y	= (s32)floor(p.y);
	auto fr = frac(p.x);
	return interp(interp(sample(x, y), sample(x + 1, y), fr), interp(sample(x, y + 1), sample(x + 1, y + 1), fr),
				  frac(p.y));
}
template <class Interp, class Sample>
FORCEINLINE f32 interpolate(v2s p, s32 s, Interp&& interp, Sample&& sample) {
	auto fl = floor(p, s);
	auto fr = frac(p.x, s) / f32(s);
	return interp(interp(sample(fl + v2s{0, 0}), sample(fl + v2s{1, 0}), fr),
				  interp(sample(fl + v2s{0, 1}), sample(fl + v2s{1, 1}), fr), frac(p.y, s) / f32(s));
}
*/
inline static constexpr v2f _randMul2{pi * 10, sqrt2 * 20};
inline static constexpr v3f _randMul3{pi * 10, sqrt2 * 20, sqrt3 * 15};
inline static constexpr v4f _randMul4{pi * 10, sqrt2 * 20, sqrt3 * 15, sqrt5 * 10};

FORCEINLINE u32 rotateLeft(u32 v, s32 shift = 1) { return _rotl(v, shift); }
FORCEINLINE u32 rotateRight(u32 v, s32 shift = 1) { return _rotr(v, shift); }
#if COMPILER_GCC
FORCEINLINE u8  rotateLeft (u8  v, s32 shift = 1) { return (v << shift) | (v >> ( 8 - shift)); }
FORCEINLINE u16 rotateLeft (u16 v, s32 shift = 1) { return (v << shift) | (v >> (16 - shift)); }
FORCEINLINE u64 rotateLeft (u64 v, s32 shift = 1) { return (v << shift) | (v >> (64 - shift)); }
FORCEINLINE u8  rotateRight(u8  v, s32 shift = 1) { return (v >> shift) | (v << ( 8 - shift)); }
FORCEINLINE u16 rotateRight(u16 v, s32 shift = 1) { return (v >> shift) | (v << (16 - shift)); }
FORCEINLINE u64 rotateRight(u64 v, s32 shift = 1) { return (v >> shift) | (v << (64 - shift)); }
#else
FORCEINLINE u8  rotateLeft (u8  v, s32 shift = 1) { return _rotl8(v, (u8)shift); }
FORCEINLINE u16 rotateLeft (u16 v, s32 shift = 1) { return _rotl16(v, (u8)shift); }
FORCEINLINE u64 rotateLeft (u64 v, s32 shift = 1) { return _rotl64(v, shift); }
FORCEINLINE u8  rotateRight(u8  v, s32 shift = 1) { return _rotr8(v, (u8)shift); }
FORCEINLINE u16 rotateRight(u16 v, s32 shift = 1) { return _rotr16(v, (u8)shift); }
FORCEINLINE u64 rotateRight(u64 v, s32 shift = 1) { return _rotr64(v, shift); }
#endif

#define ROT(u32x4) 								   \
FORCEINLINE u32x4 rotateLeft(u32x4 v, s32 shift = 1) {  \
	if (shift < 0) {							   \
		shift = absolute(shift) & 31;			   \
		return (v >> shift) | (v << (32 - shift)); \
	} else {									   \
		shift &= 31;							   \
		return (v << shift) | (v >> (32 - shift)); \
	}											   \
}												   \
FORCEINLINE u32x4 rotateRight(u32x4 v, s32 shift = 1) { \
	if (shift < 0) {							   \
		shift = absolute(shift) & 31;			   \
		return (v << shift) | (v >> (32 - shift)); \
	} else {									   \
		shift &= 31;							   \
		return (v >> shift) | (v << (32 - shift)); \
	}											   \
}
ROT(u32x4)
ROT(u32x8)
#undef ROT

#define RANDOMIZE(u32, s32) \
FORCEINLINE u32 randomize(u32 v) { return v * 0xEB84226F ^ 0x034FB5E7; } \
FORCEINLINE s32 randomize(s32 v) { return (s32)randomize((u32)v); }
RANDOMIZE(u32, s32)
RANDOMIZE(u32x4, s32x4)
RANDOMIZE(u32x8, s32x8)
#undef RANDOMIZE

#define RANDOMIZE(v2u)         \
	FORCEINLINE v2u randomize(v2u v) {     \
		v += 0x0C252DA0;       \
		v *= 0x034FB5E7;       \
		v ^= 0xF5605798;       \
		v.x = v.x * v.y + v.x; \
		v.y = v.x * v.y + v.x; \
		return v;              \
	}
RANDOMIZE(v2u) RANDOMIZE(v2ux4) RANDOMIZE(v2ux8);
#undef RANDOMIZE

#define RANDOMIZE(v3u, V3u)      \
	FORCEINLINE v3u randomize(v3u v) {       \
		v += 0x0C252DA0;         \
		v *= 0x034FB5E7;         \
		v ^= 0xF5605798;         \
		v *= V3u(v.z, v.x, v.y); \
		v *= V3u(v.z, v.x, v.y); \
		v *= V3u(v.z, v.x, v.y); \
		return v;                \
	}
RANDOMIZE(v3u, V3u) RANDOMIZE(v3ux4, V3ux4) RANDOMIZE(v3ux8, V3ux8);
#undef RANDOMIZE

#define RANDOMIZE(v4u, V4u)           \
	FORCEINLINE v4u randomize(v4u v) {            \
		v += 0x0C252DA0;              \
		v *= 0x034FB5E7;              \
		v ^= 0xF5605798;              \
		v *= V4u(v.w, v.x, v.y, v.z); \
		v *= V4u(v.w, v.x, v.y, v.z); \
		v *= V4u(v.w, v.x, v.y, v.z); \
		return v;                     \
	}
RANDOMIZE(v4u, V4u) RANDOMIZE(v4ux4, V4ux4) RANDOMIZE(v4ux8, V4ux8);
#undef RANDOMIZE

#define RANDOMIZE(v2s, v2u) \
	FORCEINLINE v2s randomize(v2s v) { return (v2s)randomize((v2u)v); }
RANDOMIZE(v2s, v2u) RANDOMIZE(v2sx4, v2ux4) RANDOMIZE(v2sx8, v2ux8);
RANDOMIZE(v3s, v3u) RANDOMIZE(v3sx4, v3ux4) RANDOMIZE(v3sx8, v3ux8);
RANDOMIZE(v4s, v4u) RANDOMIZE(v4sx4, v4ux4) RANDOMIZE(v4sx8, v4ux8);
#undef RANDOMIZE

FORCEINLINE u8 randomU8(u8 r) {
	r += 0x0C;
	r *= 0x61;
	r ^= 0xB2;
	r -= 0x80;
	r ^= 0xF5;
	r *= 0xA7;
	return rotateLeft(r, 4);
}
FORCEINLINE u32 randomU32(u32 r) {
	r += 0x0C252DA0;
	r *= 0x55555561;
	r ^= 0xB23E2387;
	r -= 0x8069BAC0;
	r ^= 0xF5605798;
	r *= 0xAAAAAABF;
	return (r << 16) | (r >> 16);
}
FORCEINLINE u32 randomU32(s32 in) { return randomU32((u32)in); }
FORCEINLINE u32 randomU32(v2s in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	return x ^ y;
}
FORCEINLINE u32 randomU32(v3s in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	auto z = randomU32(in.z);
	return x + y + z;
}
FORCEINLINE u32 randomU32(v4s v) {
	v = randomize(v);
	return u32(v.x) + u32(v.y) + u32(v.z) + u32(v.w);
}
FORCEINLINE u64 randomU64(v3s in) {
	auto x = (u64)randomU32(in.x);
	auto y = (u64)randomU32(in.y);
	auto z = (u64)randomU32(in.z);
	return x | (y << 32) + z | (x << 32) + y | (z << 32);
}

#define RANDOM(v2f, V2f)              \
	FORCEINLINE v2f random(v2f v) {               \
		v = frac(v * V2f(_randMul2)); \
		v += dot(v, v + pi * 4);      \
		v.x *= v.x;                   \
		v.y *= v.y;                   \
		return frac(v);               \
	}
RANDOM(v2f, V2f)
RANDOM(v2fx4, V2fx4)
RANDOM(v2fx8, V2fx8)
#undef RANDOM

#define RANDOM(v3f, V3f)                                   \
	FORCEINLINE v3f random(v3f v) {                                    \
		v = frac(v * V3f(_randMul3));                      \
		v += dot(v, v + pi * 4);                           \
		return frac(V3f(v.x * v.y, v.y * v.z, v.x * v.z)); \
	}
RANDOM(v3f, V3f)
RANDOM(v3fx4, V3fx4)
RANDOM(v3fx8, V3fx8)
#undef RANDOM

#define RANDOM(v4f, V4f)                          \
	FORCEINLINE v4f random(v4f v) {                           \
		v = frac((v + sqrt2) * V4f(_randMul4));   \
		v += dot(v, v + pi * 4);                  \
		return frac(v * V4f(v.y, v.z, v.w, v.x)); \
	}
RANDOM(v4f, V4f)
RANDOM(v4fx4, V4fx4)
RANDOM(v4fx8, V4fx8)
#undef RANDOM

// clang-format off
#define RANDOM(v2f, v2s, v2u) FORCEINLINE v2f random(v2s v) { return (v2f)(randomize((v2u)v) >> 8) * (1.0f / 16777216.0f); }
RANDOM(v2f, v2s, v2u) RANDOM(v2fx4, v2sx4, v2ux4) RANDOM(v2fx8, v2sx8, v2ux8)
RANDOM(v3f, v3s, v3u) RANDOM(v3fx4, v3sx4, v3ux4) RANDOM(v3fx8, v3sx8, v3ux8)
RANDOM(v4f, v4s, v4u) RANDOM(v4fx4, v4sx4, v4ux4) RANDOM(v4fx8, v4sx8, v4ux8)
#undef RANDOM

#define RANDOM(v2f, v2s) FORCEINLINE v2f operator()(v2s v) const { return random(v); }
struct Random {
	RANDOM(v2f, v2s) RANDOM(v2fx4, v2sx4) RANDOM(v2fx8, v2sx8)
	RANDOM(v3f, v3s) RANDOM(v3fx4, v3sx4) RANDOM(v3fx8, v3sx8)
	RANDOM(v4f, v4s) RANDOM(v4fx4, v4sx4) RANDOM(v4fx8, v4sx8)
	RANDOM(v2f, v2f) RANDOM(v2fx4, v2fx4) RANDOM(v2fx8, v2fx8)
	RANDOM(v3f, v3f) RANDOM(v3fx4, v3fx4) RANDOM(v3fx8, v3fx8)
	RANDOM(v4f, v4f) RANDOM(v4fx4, v4fx4) RANDOM(v4fx8, v4fx8)
};
#undef RANDOM

FORCEINLINE v3f hsvToRgb(f32 h, f32 s, f32 v) {
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
template<umm ps>
FORCEINLINE v3fx<ps> hsvToRgb(f32x<ps> h, f32x<ps> s, f32x<ps> v) {
	h = frac(h);
	f32x<ps> c = v * s;
	f32x<ps> x = c * (1 - abs(modulo(h * 6, F32x<ps>(2)) - 1));
	b32x<ps> cmp0 = h < ( 60 / 360.0f);
	b32x<ps> cmp1 = h < (120 / 360.0f);
	b32x<ps> cmp2 = h < (180 / 360.0f);
	b32x<ps> cmp3 = h < (240 / 360.0f);
	b32x<ps> cmp4 = h < (300 / 360.0f);
	b32x<ps> m0 =          cmp0;
	b32x<ps> m1 = !cmp0 && cmp1;
	b32x<ps> m2 = !cmp1 && cmp2;
	b32x<ps> m3 = !cmp2 && cmp3;
	b32x<ps> m4 = !cmp3 && cmp4;
	b32x<ps> m5 = !cmp4;
	v3fx<ps> m = V3fx<ps>(v - c);
	m = select(m0, m + V3fx<ps>(c, x, {}), m);
	m = select(m1, m + V3fx<ps>(x, c, {}), m);
	m = select(m2, m + V3fx<ps>({}, c, x), m);
	m = select(m3, m + V3fx<ps>({}, x, c), m);
	m = select(m4, m + V3fx<ps>(x, {}, c), m);
	m = select(m5, m + V3fx<ps>(c, {}, x), m);
	return m;
}

template<class T> inline static constexpr bool isVector = false;
template<umm w> inline static constexpr bool isVector<v2fx<w>> = true;
template<umm w> inline static constexpr bool isVector<v3fx<w>> = true;
template<umm w> inline static constexpr bool isVector<v4fx<w>> = true;
template<umm w> inline static constexpr bool isVector<v2sx<w>> = true;
template<umm w> inline static constexpr bool isVector<v3sx<w>> = true;
template<umm w> inline static constexpr bool isVector<v4sx<w>> = true;
template<umm w> inline static constexpr bool isVector<v2ux<w>> = true;
template<umm w> inline static constexpr bool isVector<v3ux<w>> = true;
template<umm w> inline static constexpr bool isVector<v4ux<w>> = true;
// clang-format on

template <umm width, class T>
FORCEINLINE auto widen(T v) = delete;

// clang-format off
template<> FORCEINLINE auto widen<1>(v2f v) { return v; }
template<> FORCEINLINE auto widen<1>(v3f v) { return v; }
template<> FORCEINLINE auto widen<1>(v4f v) { return v; }
template<> FORCEINLINE auto widen<1>(v2s v) { return v; }
template<> FORCEINLINE auto widen<1>(v3s v) { return v; }
template<> FORCEINLINE auto widen<1>(v4s v) { return v; }
template<> FORCEINLINE auto widen<1>(v2u v) { return v; }
template<> FORCEINLINE auto widen<1>(v3u v) { return v; }
template<> FORCEINLINE auto widen<1>(v4u v) { return v; }
template<> FORCEINLINE auto widen<4>(v2f v) { return V2fx4(v); }
template<> FORCEINLINE auto widen<4>(v3f v) { return V3fx4(v); }
template<> FORCEINLINE auto widen<4>(v4f v) { return V4fx4(v); }
template<> FORCEINLINE auto widen<4>(v2s v) { return V2sx4(v); }
template<> FORCEINLINE auto widen<4>(v3s v) { return V3sx4(v); }
template<> FORCEINLINE auto widen<4>(v4s v) { return V4sx4(v); }
template<> FORCEINLINE auto widen<4>(v2u v) { return V2ux4(v); }
template<> FORCEINLINE auto widen<4>(v3u v) { return V3ux4(v); }
template<> FORCEINLINE auto widen<4>(v4u v) { return V4ux4(v); }
template<> FORCEINLINE auto widen<8>(v2f v) { return V2fx8(v); }
template<> FORCEINLINE auto widen<8>(v3f v) { return V3fx8(v); }
template<> FORCEINLINE auto widen<8>(v4f v) { return V4fx8(v); }
template<> FORCEINLINE auto widen<8>(v2s v) { return V2sx8(v); }
template<> FORCEINLINE auto widen<8>(v3s v) { return V3sx8(v); }
template<> FORCEINLINE auto widen<8>(v4s v) { return V4sx8(v); }
template<> FORCEINLINE auto widen<8>(v2u v) { return V2ux8(v); }
template<> FORCEINLINE auto widen<8>(v3u v) { return V3ux8(v); }
template<> FORCEINLINE auto widen<8>(v4u v) { return V4ux8(v); }
// clang-format on

namespace CE {
#define WIDEN(ty, tyx4, conx4) \
	FORCEINLINE constexpr tyx4 widen(ty a, ty b, ty c, ty d) { return CE::conx4(a, b, c, d); }
WIDEN(v2f, v2fx4, V2fx4)
WIDEN(v2s, v2sx4, V2sx4)
WIDEN(v2u, v2ux4, V2ux4)
WIDEN(v3f, v3fx4, V3fx4)
WIDEN(v3s, v3sx4, V3sx4)
WIDEN(v3u, v3ux4, V3ux4)
WIDEN(v4f, v4fx4, V4fx4)
WIDEN(v4s, v4sx4, V4sx4)
WIDEN(v4u, v4ux4, V4ux4)
#undef WIDEN
#define WIDEN(ty, tyx8, conx8) \
	FORCEINLINE constexpr tyx8 widen(ty a, ty b, ty c, ty d, ty e, ty f, ty g, ty h) { return CE::conx8(a, b, c, d, e, f, g, h); }
WIDEN(v2f, v2fx8, V2fx8)
WIDEN(v2s, v2sx8, V2sx8)
WIDEN(v2u, v2ux8, V2ux8)
WIDEN(v3f, v3fx8, V3fx8)
WIDEN(v3s, v3sx8, V3sx8)
WIDEN(v3u, v3ux8, V3ux8)
WIDEN(v4f, v4fx8, V4fx8)
WIDEN(v4s, v4sx8, V4sx8)
WIDEN(v4u, v4ux8, V4ux8)
#undef WIDEN
} // namespace CE

template <class To, class T>
FORCEINLINE auto cvtScalar(T v) = delete;

// clang-format off
template<> FORCEINLINE auto cvtScalar<f32>(v2s v)   { return (v2f)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v3s v)   { return (v3f)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v4s v)   { return (v4f)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v2sx4 v) { return (v2fx4)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v3sx4 v) { return (v3fx4)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v4sx4 v) { return (v4fx4)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v2sx8 v) { return (v2fx8)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v3sx8 v) { return (v3fx8)v; }
template<> FORCEINLINE auto cvtScalar<f32>(v4sx8 v) { return (v4fx8)v; }
// clang-format on

template <class T, umm size>
struct WideType {};

// clang-format off
template<umm width> struct WideType<v2f, width> { using Type = v2fx<width>; };
template<umm width> struct WideType<v3f, width> { using Type = v3fx<width>; };
template<umm width> struct WideType<v4f, width> { using Type = v4fx<width>; };
template<umm width> struct WideType<v2s, width> { using Type = v2sx<width>; };
template<umm width> struct WideType<v3s, width> { using Type = v3sx<width>; };
template<umm width> struct WideType<v4s, width> { using Type = v4sx<width>; };
template<umm width> struct WideType<v2u, width> { using Type = v2ux<width>; };
template<umm width> struct WideType<v3u, width> { using Type = v3ux<width>; };
template<umm width> struct WideType<v4u, width> { using Type = v4ux<width>; };
// clang-format on

template <class From, class ToScalar>
struct Convert {};

// clang-format off
template<> struct Convert<v2f, s32> { using Type = v2s; };
template<> struct Convert<v3f, s32> { using Type = v3s; };
template<> struct Convert<v4f, s32> { using Type = v4s; };
template<> struct Convert<v2f, u32> { using Type = v2u; };
template<> struct Convert<v3f, u32> { using Type = v3u; };
template<> struct Convert<v4f, u32> { using Type = v4u; };
template<> struct Convert<v2s, f32> { using Type = v2f; };
template<> struct Convert<v3s, f32> { using Type = v3f; };
template<> struct Convert<v4s, f32> { using Type = v4f; };
template<> struct Convert<v2s, u32> { using Type = v2u; };
template<> struct Convert<v3s, u32> { using Type = v3u; };
template<> struct Convert<v4s, u32> { using Type = v4u; };
template<> struct Convert<v2u, f32> { using Type = v2f; };
template<> struct Convert<v3u, f32> { using Type = v3f; };
template<> struct Convert<v4u, f32> { using Type = v4f; };
template<> struct Convert<v2u, s32> { using Type = v2s; };
template<> struct Convert<v3u, s32> { using Type = v3s; };
template<> struct Convert<v4u, s32> { using Type = v4s; };
// clang-format on

namespace CE {

template <class ToScalar, class From, umm size>
FORCEINLINE constexpr auto cvtScalar(Array<From, size> const& arr) {
	using To = typename Convert<From, ToScalar>::Type;
	Array<To, size> result{};
	for (umm i = 0; i < size; ++i) {
		result[i] = CE::cvt<To>(arr[i]);
	}
	return result;
}

template <class T, umm count1, umm count4, umm count8>
struct WideArray {
	using T8 = typename WideType<T, 8>::Type;
	using T4 = typename WideType<T, 4>::Type;
	using T1 = T;
	T8 t8[count8]{};
	T4 t4[count4]{};
	T1 t1[count1]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count8 * 8 + count4 * 4 + count1]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[count8 * 8 + i * 4 + 0], arr[count8 * 8 + i * 4 + 1], arr[count8 * 8 + i * 4 + 2],
							  arr[count8 * 8 + i * 4 + 3]);
		}
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[count8 * 8 + count4 * 4 + i];
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count8 * 8 + count4 * 4 + count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
		for (auto const& t : t4)
			cb(t);
		for (auto const& t : t1)
			cb(t);
	}
};
template <class T, umm count1>
struct WideArray<T, count1, 0, 0> {
	using T1 = T;
	T1 t1[count1]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count1]) {
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[i];
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t1)
			cb(t);
	}
};
template <class T, umm count4>
struct WideArray<T, 0, count4, 0> {
	using T4 = typename WideType<T, 4>::Type;
	T4 t4[count4]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count4 * 4]) {
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[i * 4 + 0], arr[i * 4 + 1], arr[i * 4 + 2], arr[i * 4 + 3]);
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count4 * 4> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t4)
			cb(t);
	}
};
template <class T, umm count1, umm count4>
struct WideArray<T, count1, count4, 0> {
	using T4 = typename WideType<T, 4>::Type;
	using T1 = T;
	T4 t4[count4]{};
	T1 t1[count1]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count4 * 4 + count1]) {
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[i * 4 + 0], arr[i * 4 + 1], arr[i * 4 + 2], arr[i * 4 + 3]);
		}
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[count4 * 4 + i];
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count4 * 4 + count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t4)
			cb(t);
		for (auto const& t : t1)
			cb(t);
	}
};
template <class T, umm count8>
struct WideArray<T, 0, 0, count8> {
	using T8 = typename WideType<T, 8>::Type;
	T8 t8[count8]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count8 * 8]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count8 * 8> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
	}
};
template <class T, umm count1, umm count8>
struct WideArray<T, count1, 0, count8> {
	using T8 = typename WideType<T, 8>::Type;
	using T1 = T;
	T8 t8[count8]{};
	T1 t1[count1]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count8 * 8 + count1]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[count8 * 8 + i];
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count8 * 8 + count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
		for (auto const& t : t1)
			cb(t);
	}
};
template <class T, umm count4, umm count8>
struct WideArray<T, 0, count4, count8> {
	using T8 = typename WideType<T, 8>::Type;
	using T4 = typename WideType<T, 4>::Type;
	T8 t8[count8]{};
	T4 t4[count4]{};
	FORCEINLINE constexpr WideArray(T const (&arr)[count8 * 8 + count4 * 4]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[count8 * 8 + i * 4 + 0], arr[count8 * 8 + i * 4 + 1], arr[count8 * 8 + i * 4 + 2],
							  arr[count8 * 8 + i * 4 + 3]);
		}
	}
	FORCEINLINE constexpr WideArray(Array<T, count8 * 8 + count4 * 4> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	FORCEINLINE constexpr void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
		for (auto const& t : t4)
			cb(t);
	}
};
template <class T, umm size>
FORCEINLINE constexpr auto makeWide(Array<T, size> const& arr) {
	constexpr umm count8 = size / 8;
	constexpr umm count4 = (size - count8 * 8) / 4;
	constexpr umm count1 = size - count8 * 8 - count4 * 4;
	return WideArray<T, count1, count4, count8>(arr);
}
} // namespace CE

// for some reason compiler generates bad code for WideArray::forEach, so we don't use it by
// default. it's ~50-100 cycles faster if iterate raw arrays
#ifndef TL_USE_CONSTEXPR_WIDE
#define TL_USE_CONSTEXPR_WIDE 0
#endif
template <class T, class Random, umm count1, umm count4, umm count8>
FORCEINLINE f32 _voronoiF32(CE::WideArray<T, count1, count4, count8> const& wideOffsets, T v, Random random) {
	T tile		= floor(v);
	T rel		= v - tile;
	f32 minDist = 1000;
	wideOffsets.forEach([&](auto const& offset) {
		static constexpr auto offsetW = widthOf<decltype(offset)>;

		minDist = min(minDist, distanceSqr(widen<offsetW>(rel), random(widen<offsetW>(tile) + offset) + offset));
	});
	return minDist;
};
template <class T, class U, class Random, umm count1, umm count4, umm count8>
FORCEINLINE f32 _voronoiS32(CE::WideArray<T, count1, count4, count8> const& wideOffsets, T tile, U rel, Random random) {
	f32 minDist = 1000;
	wideOffsets.forEach([&](auto const& offset) {
		static constexpr auto width = widthOf<decltype(offset)>;

		minDist = min(minDist, distanceSqr(widen<width>(rel),
										   random(widen<width>(tile) + offset) * 0.5f + cvtScalar<float>(offset)));
	});
	return minDist;
};
// clang-format off
inline static constexpr Array<v2s, 9> voronoiOffsets2s { 
	-1,-1,
	-1, 0,
	-1, 1,
	 0,-1,
	 0, 0,
	 0, 1,
	 1,-1,
	 1, 0,
	 1, 1,
};
inline static constexpr Array<v3s, 27> voronoiOffsets3s { 
	-1,-1,-1, -1,-1, 0, -1,-1, 1,
	-1, 0,-1, -1, 0, 0, -1, 0, 1,
	-1, 1,-1, -1, 1, 0, -1, 1, 1,
	 0,-1,-1,  0,-1, 0,  0,-1, 1,
	 0, 0,-1,  0, 0, 0,  0, 0, 1,
	 0, 1,-1,  0, 1, 0,  0, 1, 1,
	 1,-1,-1,  1,-1, 0,  1,-1, 1,
	 1, 0,-1,  1, 0, 0,  1, 0, 1,
	 1, 1,-1,  1, 1, 0,  1, 1, 1,
};
inline static constexpr Array<v4s, 81> voronoiOffsets4s {
	-1,-1,-1,-1, -1,-1,-1, 0, -1,-1,-1, 1,  -1,-1, 0,-1, -1,-1, 0, 0, -1,-1, 0, 1,  -1,-1, 1,-1, -1,-1, 1, 0, -1,-1, 1, 1,
	-1, 0,-1,-1, -1, 0,-1, 0, -1, 0,-1, 1,  -1, 0, 0,-1, -1, 0, 0, 0, -1, 0, 0, 1,  -1, 0, 1,-1, -1, 0, 1, 0, -1, 0, 1, 1,
	-1, 1,-1,-1, -1, 1,-1, 0, -1, 1,-1, 1,  -1, 1, 0,-1, -1, 1, 0, 0, -1, 1, 0, 1,  -1, 1, 1,-1, -1, 1, 1, 0, -1, 1, 1, 1,
	 0,-1,-1,-1,  0,-1,-1, 0,  0,-1,-1, 1,   0,-1, 0,-1,  0,-1, 0, 0,  0,-1, 0, 1,   0,-1, 1,-1,  0,-1, 1, 0,  0,-1, 1, 1,
	 0, 0,-1,-1,  0, 0,-1, 0,  0, 0,-1, 1,   0, 0, 0,-1,  0, 0, 0, 0,  0, 0, 0, 1,   0, 0, 1,-1,  0, 0, 1, 0,  0, 0, 1, 1,
	 0, 1,-1,-1,  0, 1,-1, 0,  0, 1,-1, 1,   0, 1, 0,-1,  0, 1, 0, 0,  0, 1, 0, 1,   0, 1, 1,-1,  0, 1, 1, 0,  0, 1, 1, 1,
	 1,-1,-1,-1,  1,-1,-1, 0,  1,-1,-1, 1,   1,-1, 0,-1,  1,-1, 0, 0,  1,-1, 0, 1,   1,-1, 1,-1,  1,-1, 1, 0,  1,-1, 1, 1,
	 1, 0,-1,-1,  1, 0,-1, 0,  1, 0,-1, 1,   1, 0, 0,-1,  1, 0, 0, 0,  1, 0, 0, 1,   1, 0, 1,-1,  1, 0, 1, 0,  1, 0, 1, 1,
	 1, 1,-1,-1,  1, 1,-1, 0,  1, 1,-1, 1,   1, 1, 0,-1,  1, 1, 0, 0,  1, 1, 0, 1,   1, 1, 1,-1,  1, 1, 1, 0,  1, 1, 1, 1,
};
inline static constexpr auto voronoiOffsets2f = CE::cvtScalar<f32>(voronoiOffsets2s);
inline static constexpr auto voronoiOffsets3f = CE::cvtScalar<f32>(voronoiOffsets3s);
inline static constexpr auto voronoiOffsets4f = CE::cvtScalar<f32>(voronoiOffsets4s);
// clang-format on

template <class Random = Random>
FORCEINLINE f32 voronoi(v2f v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets2f);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * (1 / sqrt2);
}
template <class Random = Random>
FORCEINLINE f32 voronoi(v3f v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets3f);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * (1 / sqrt3);
}
template <class Random = Random>
FORCEINLINE f32 voronoi(v4f v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets4f);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * 0.5f;
}
template <class Random = Random>
FORCEINLINE f32 voronoi(v2s v, u32 cellSize, Random random = {}) {
	v2s flr							  = floor(v, cellSize);
	v2s tile						  = flr / (s32)cellSize;
	v2f rel							  = (v2f)(v - flr) / (f32)cellSize - 0.5f;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets2s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	f32x8 minDist8 = F32x8(9999);
	auto tile8	= V2sx8(tile);
	auto rel8	= V2fx8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist8 = min(minDist8, distanceSqr(rel8, random(tile8 + offset) * 0.5f + (v2fx8)offset));
	}
	f32 minDist = min(minDist8);
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) * 0.5f + (v2f)offset));
	}
#endif
	return sqrt(minDist) * (1 / sqrt2);
}
template <class Random = Random>
FORCEINLINE f32 voronoi(v3s v, u32 cellSize, Random random = {}) {
	v3s flr							  = floor(v, cellSize);
	v3s tile						  = flr / (s32)cellSize;
	v3f rel							  = (v3f)(v - flr) / (f32)cellSize - 0.5f;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets3s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	f32 minDist = 1000;
	v3sx8 tile8 = V3sx8(tile);
	v3fx8 rel8	= V3fx8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, distanceSqr(rel8, random(tile8 + offset) - 0.5f + (v3fx8)offset));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) - 0.5f + (v3f)offset));
	}
#endif
	return sqrt(minDist) * (1 / sqrt3);
}
template <class Random = Random>
FORCEINLINE f32 voronoi(v4s v, u32 cellSize, Random random = {}) {
	v4s flr							  = floor(v, cellSize);
	v4s tile						  = flr / (s32)cellSize;
	v4f rel							  = (v4f)(v - flr) / (f32)cellSize - 0.5f;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets4s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	f32 minDist = 1000;
	v4sx8 tile8 = V4sx8(tile);
	v4fx8 rel8	= V4fx8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, distanceSqr(rel8, random(tile8 + offset) * 0.5f + cvtScalar<float>(offset)));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) * 0.5f + (v4f)offset));
	}
#endif
	return sqrt(minDist) * 0.5f;
}

template<class T>
struct line {
	T a, b;
};

template <class T>
FORCEINLINE line<T> lineBeginEnd(T begin, T end) {
	return {begin, end};
}

template <class T>
FORCEINLINE line<T> lineBeginDir(T begin, T dir) {
	return {begin, begin + dir};
}

template<class T>
struct aabb {
	T min, max;
};

template <class T>
FORCEINLINE aabb<T> aabbMinMax(T min, T max) {
	return {min, max};
}
template <class T>
FORCEINLINE aabb<T> aabbMinDim(T min, T dim) {
	return {min, min + dim};
}
template <class T>
FORCEINLINE aabb<T> aabbCenterDim(T center, T dim) {
	dim = half(dim);
	return {center - dim, center + dim};
}
template <class T>
FORCEINLINE aabb<T> aabbCenterRadius(T center, T radius) {
	return {center - radius, center + radius};
}

#define IN_BOUNDS2(bool, v2fA, v2fB) 			\
	FORCEINLINE bool inBounds(v2fA v, aabb<v2fB> b) { \
		return 								    \
			(v.x >= b.min.x) && 			    \
			(v.y >= b.min.y) && 			    \
			(v.x < b.max.x) && 				    \
			(v.y < b.max.y); 				    \
	}
#define IN_BOUNDS3(bool, v3fA, v3fB) 			\
	FORCEINLINE bool inBounds(v3fA v, aabb<v3fB> b) { \
		return 								    \
			(v.x >= b.min.x) && 			    \
			(v.y >= b.min.y) &&				    \
			(v.z >= b.min.z) &&				    \
			(v.x < b.max.x) &&				    \
			(v.y < b.max.y) &&				    \
			(v.z < b.max.z);				    \
	}
IN_BOUNDS2(bool, v2f, v2f) IN_BOUNDS2(b32x4, v2fx4, v2f) IN_BOUNDS2(b32x8, v2fx8, v2f) IN_BOUNDS2(b32x4, v2f, v2fx4) IN_BOUNDS2(b32x8, v2f, v2fx8) IN_BOUNDS2(b32x4, v2fx4, v2fx4) IN_BOUNDS2(b32x8, v2fx8, v2fx8);
IN_BOUNDS2(bool, v2s, v2s) IN_BOUNDS2(b32x4, v2sx4, v2s) IN_BOUNDS2(b32x8, v2sx8, v2s) IN_BOUNDS2(b32x4, v2s, v2sx4) IN_BOUNDS2(b32x8, v2s, v2sx8) IN_BOUNDS2(b32x4, v2sx4, v2sx4) IN_BOUNDS2(b32x8, v2sx8, v2sx8);
IN_BOUNDS2(bool, v2u, v2u) IN_BOUNDS2(b32x4, v2ux4, v2u) IN_BOUNDS2(b32x8, v2ux8, v2u) IN_BOUNDS2(b32x4, v2u, v2ux4) IN_BOUNDS2(b32x8, v2u, v2ux8) IN_BOUNDS2(b32x4, v2ux4, v2ux4) IN_BOUNDS2(b32x8, v2ux8, v2ux8);
IN_BOUNDS3(bool, v3f, v3f) IN_BOUNDS3(b32x4, v3fx4, v3f) IN_BOUNDS3(b32x8, v3fx8, v3f) IN_BOUNDS3(b32x4, v3f, v3fx4) IN_BOUNDS3(b32x8, v3f, v3fx8) IN_BOUNDS3(b32x4, v3fx4, v3fx4) IN_BOUNDS3(b32x8, v3fx8, v3fx8);
IN_BOUNDS3(bool, v3s, v3s) IN_BOUNDS3(b32x4, v3sx4, v3s) IN_BOUNDS3(b32x8, v3sx8, v3s) IN_BOUNDS3(b32x4, v3s, v3sx4) IN_BOUNDS3(b32x8, v3s, v3sx8) IN_BOUNDS3(b32x4, v3sx4, v3sx4) IN_BOUNDS3(b32x8, v3sx8, v3sx8);
IN_BOUNDS3(bool, v3u, v3u) IN_BOUNDS3(b32x4, v3ux4, v3u) IN_BOUNDS3(b32x8, v3ux8, v3u) IN_BOUNDS3(b32x4, v3u, v3ux4) IN_BOUNDS3(b32x8, v3u, v3ux8) IN_BOUNDS3(b32x4, v3ux4, v3ux4) IN_BOUNDS3(b32x8, v3ux8, v3ux8);
#undef IN_BOUNDS2
#undef IN_BOUNDS3

#define INTERSECTS2(bool, v2f)						 \
	FORCEINLINE bool intersects(aabb<v2f> a, aabb<v2f> b) { \
		return 										 \
			(a.min.x < b.max.x) && 					 \
			(a.min.y < b.max.y) && 					 \
			(a.max.x > b.min.x) && 					 \
			(a.max.y > b.min.y); 					 \
	}
#define INTERSECTS3(bool, v3f)						 \
	FORCEINLINE bool intersects(aabb<v3f> a, aabb<v3f> b) { \
		return 										 \
			(a.min.x < b.max.x) && 					 \
			(a.min.y < b.max.y) && 					 \
			(a.min.z < b.max.z) && 					 \
			(a.max.x > b.min.x) && 					 \
			(a.max.y > b.min.y) && 					 \
			(a.max.z > b.min.z); 					 \
	}
INTERSECTS2(bool, v2f) INTERSECTS2(b32x4, v2fx4) INTERSECTS2(b32x8, v2fx8);
INTERSECTS2(bool, v2s) INTERSECTS2(b32x4, v2sx4) INTERSECTS2(b32x8, v2sx8);
INTERSECTS2(bool, v2u) INTERSECTS2(b32x4, v2ux4) INTERSECTS2(b32x8, v2ux8);
INTERSECTS3(bool, v3f) INTERSECTS3(b32x4, v3fx4) INTERSECTS3(b32x8, v3fx8);
INTERSECTS3(bool, v3s) INTERSECTS3(b32x4, v3sx4) INTERSECTS3(b32x8, v3sx8);
INTERSECTS3(bool, v3u) INTERSECTS3(b32x4, v3ux4) INTERSECTS3(b32x8, v3ux8);
#undef INTERSECTS2
#undef INTERSECTS3

FORCEINLINE bool intersects(line<v2f> line, aabb<v2f> aabb) {
	v2f dir = normalize(line.b - line.a);
	v2f rdir = 1.0f / dir;
	v2f vMin = (aabb.min - line.a) * rdir;
	v2f vMax = (aabb.max - line.a) * rdir;

	f32 tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y));
	f32 tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y));

	return tMax > 0 && tMin < tMax;
}
template <umm ps>
FORCEINLINE b32x<ps> intersects(line<v2fx<ps>> line, aabb<v2fx<ps>> aabb) {
	auto dir = normalize(line.b - line.a);
	auto rdir = 1.0f / dir;
	auto vMin = (aabb.min - line.a) * rdir;
	auto vMax = (aabb.max - line.a) * rdir;

	auto tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y));
	auto tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y));

	return tMax > 0 && tMin < tMax;
}

FORCEINLINE bool raycastLine(v2f a, v2f b, v2f c, v2f d, v2f& point, v2f& normal) {
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
template <umm ps>
FORCEINLINE b32x<ps> raycastLine(v2fx<ps> a, v2fx<ps> b, v2fx<ps> c, v2fx<ps> d, v2fx<ps>& point, v2fx<ps>& normal, bool backFace = true) {
	v2fx<ps> ba = b - a;
	v2fx<ps> dc = d - c;
	v2fx<ps> ac = a - c;

	v2fx<ps> s = v2fx<ps>{ba.x, dc.x} * ac.y - v2fx<ps>{ba.y, dc.y} * ac.x;
	s /= ba.x * dc.y - dc.x * ba.y;

	point  = a + (s.y * ba);
	normal = cross(normalize(c - d));

	auto backFaceMask = dot(b - a, normal) > 0;
	normal = select(backFaceMask, -normal, normal);

	auto result = (s.x >= 0) && (s.x <= 1) && (s.y >= 0) && (s.y <= 1);
	if(!backFace)
		result &= !backFaceMask;

	return result;
}
FORCEINLINE bool raycastRect(v2f a, v2f b, v2f tile, v2f size, v2f& point, v2f& normal) {
	v2f points[4];
	v2f normals[4];
	v2f const w = size;
	// clang-format off
	bool hits[]{
		raycastLine(a, b, tile + v2f{-w.x, w.y}, tile + v2f{ w.x, w.y}, points[0], normals[0]),
		raycastLine(a, b, tile + v2f{ w.x, w.y}, tile + v2f{ w.x,-w.y}, points[1], normals[1]),
		raycastLine(a, b, tile + v2f{ w.x,-w.y}, tile + v2f{-w.x,-w.y}, points[2], normals[2]),
		raycastLine(a, b, tile + v2f{-w.x,-w.y}, tile + v2f{-w.x, w.y}, points[3], normals[3]),
	};
	// clang-format on
	f32 minDist	 = FLT_MAX;
	int minIndex = -1;
	for (int i = 0; i < 4; ++i) {
		if (!hits[i])
			continue;
		f32 len = lengthSqr(a - points[i]);
		if (len < minDist) {
			minDist	 = len;
			minIndex = i;
		}
	}
	if (minIndex == -1) {
		return false;
	}
	point  = points[minIndex];
	normal = normals[minIndex];
	return true;
}
template <umm ps>
FORCEINLINE b32x<ps> raycastRect(v2fx<ps> a, v2fx<ps> b, v2fx<ps> tile, v2fx<ps> size, v2fx<ps>& point, v2fx<ps>& normal, bool backFace = true) {
	v2fx<ps> points[4];
	v2fx<ps> normals[4];
	v2fx<ps> const w = size;
	// clang-format off
	b32x<ps> hits[]{
		raycastLine(a, b, tile + v2fx<ps>{-w.x, w.y}, tile + v2fx<ps>{ w.x, w.y}, points[0], normals[0], backFace),
		raycastLine(a, b, tile + v2fx<ps>{ w.x, w.y}, tile + v2fx<ps>{ w.x,-w.y}, points[1], normals[1], backFace),
		raycastLine(a, b, tile + v2fx<ps>{ w.x,-w.y}, tile + v2fx<ps>{-w.x,-w.y}, points[2], normals[2], backFace),
		raycastLine(a, b, tile + v2fx<ps>{-w.x,-w.y}, tile + v2fx<ps>{-w.x, w.y}, points[3], normals[3], backFace),
	};
	// clang-format on
	f32x<ps> minDist  = F32x<ps>(FLT_MAX);
	s32x<ps> minIndex = S32x<ps>(-1);
	for (int i = 0; i < 4; ++i) {
		f32x<ps> len	  = lengthSqr(a - points[i]);
		b32x<ps> mask = hits[i] && len < minDist;
		minDist		  = select(mask, len, minDist);
		minIndex	  = select(mask, S32x<ps>(i), minIndex);
	}
	b32x<ps> masks[4];
	masks[0] = minIndex == -1;
	masks[1] = minIndex == 0;
	masks[2] = minIndex == 1;
	masks[3] = minIndex == 2;

	point  = select(masks[0], {},
			 select(masks[1], points[0], 
			 select(masks[2], points[1], 
			 select(masks[3], points[2], points[3]))));
	normal = select(masks[0], {},
			 select(masks[1], normals[0], select(masks[2], normals[1], select(masks[3], normals[2], normals[3]))));

	return !masks[0];
}
FORCEINLINE bool raycastAABB(v2f a, v2f b, v2f boxMin, v2f boxMax, v2f& point, v2f& normal) {
	v2f dir = normalize(b - a);
	v2f rdir = 1.0f / dir;
	v2f vMin = (boxMin - a) * rdir;
	v2f vMax = (boxMax - a) * rdir;

	f32 tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y));
	f32 tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y));

	if (tMax <= 0 || tMin >= tMax) {
		return false;
	}

	v2f boxR = (boxMax - boxMin) * 0.5f;
	v2f boxC = (boxMax + boxMin) * 0.5f;
	v2f aOff = dir * tMin;
	point = a + aOff;
	v2f localPoint = point - boxC;
	if (absolute(absolute(localPoint.x) - boxR.x) < 0.001f) {
		normal = {sign(localPoint.x), 0};
	} else {
		normal = {0, sign(localPoint.y)};
	}
	return true;
}
FORCEINLINE bool raycastAABB(v2f a, v2f b, aabb<v2f> box, v2f& point, v2f& normal) { return raycastAABB(a, b, box.min, box.max, point, normal); }
template <umm ps>
FORCEINLINE b32x<ps> raycastAABB(v2fx<ps> a, v2fx<ps> b, v2fx<ps> boxMin, v2fx<ps> boxMax, v2fx<ps>& point, v2fx<ps>& normal) {
	auto dir = normalize(b - a);
	auto rdir = 1.0f / dir;
	auto vMin = (boxMin - a) * rdir;
	auto vMax = (boxMax - a) * rdir;

	auto tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y));
	auto tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y));

	auto boxR = (boxMax - boxMin) * 0.5f;
	auto boxC = (boxMax + boxMin) * 0.5f;
	auto aOff = dir * tMin;
	point = a + aOff;
	auto localPoint = point - boxC;
	normal = select(absolute(absolute(localPoint.x) - boxR.x) < 0.001f, V2fx<ps>(sign(localPoint.x), {}), V2fx<ps>({}, sign(localPoint.y)));
	return tMax > 0 && tMin < tMax;
}
template <umm ps>
FORCEINLINE b32x<ps> raycastAABB(v2fx<ps> a, v2fx<ps> b, aabb<v2fx<ps>> box, v2fx<ps>& point, v2fx<ps>& normal) { return raycastAABB(a, b, box.min, box.max, point, normal); }
FORCEINLINE bool raycastPlane(v3f a, v3f b, v3f p1, v3f p2, v3f p3, v3f& point, v3f& normal) {
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
	float u	 = dot(dp1, p21);
	float v	 = dot(dp1, p31);

	if (lengthSqr(a - point) > lengthSqr(a - b) || dot(a - point, a - b) <= 0) {
		return false;
	}

	return u >= 0.0f && u <= dot(p21, p21) && v >= 0.0f && v <= dot(p31, p31);
}

FORCEINLINE bool raycastBlock(v3f a, v3f b, v3f blk, v3f blockDimensions, v3f &point, v3f &normal) {
	v3f points[6];
	v3f normals[6];
	bool results[6]{};
	f32 x = blockDimensions.x;
	f32 y = blockDimensions.y;
	f32 z = blockDimensions.z;
	// clang-format off
	results[0]	= raycastPlane(a, b, blk + v3f{ x, y, z}, blk + v3f{ x,-y, z}, blk + v3f{ x, y,-z}, points[0], normals[0]); //+x
	results[1]	= raycastPlane(a, b, blk + v3f{-x, y, z}, blk + v3f{-x, y,-z}, blk + v3f{-x,-y, z}, points[1], normals[1]); //-x
	results[2]	= raycastPlane(a, b, blk + v3f{ x, y, z}, blk + v3f{ x, y,-z}, blk + v3f{-x, y, z}, points[2], normals[2]); //+y
	results[3]	= raycastPlane(a, b, blk + v3f{ x,-y, z}, blk + v3f{-x,-y, z}, blk + v3f{ x,-y,-z}, points[3], normals[3]); //-y
	results[4]	= raycastPlane(a, b, blk + v3f{ x, y, z}, blk + v3f{-x, y, z}, blk + v3f{ x,-y, z}, points[4], normals[4]); //+z
	results[5]	= raycastPlane(a, b, blk + v3f{ x, y,-z}, blk + v3f{ x,-y,-z}, blk + v3f{-x, y,-z}, points[5], normals[5]); //-z
	// clang-format on
	s32 min		= -1;
	f32 minDist = FLT_MAX;
	for (s32 i = 0; i < 6; ++i) {
		if (results[i]) {
			auto lenSqr = lengthSqr(a - points[i]);
			if (lenSqr < minDist) {
				minDist = lenSqr;
				min = i;
			}
		}
	}
	if (min == -1)
		return false;
	point = points[min];
	normal = normals[min];
	return true;
}

template <class T, umm size>
FORCEINLINE constexpr T linearSample(const T (&arr)[size], float t) noexcept {
	f32 f = frac(t) * size;
	s32 a = (s32)f;
	s32 b = a + 1;
	if (b == size)
		b = 0;
	return lerp(arr[a], arr[b], frac(f));
}
struct FrustumPlanes {
	v4f planes[6];
	FORCEINLINE FrustumPlanes(m4 vp) {
		planes[0].x = vp.i.w + vp.i.x;
		planes[0].y = vp.j.w + vp.j.x;
		planes[0].z = vp.k.w + vp.k.x;
		planes[0].w = vp.l.w + vp.l.x;
		planes[1].x = vp.i.w - vp.i.x;
		planes[1].y = vp.j.w - vp.j.x;
		planes[1].z = vp.k.w - vp.k.x;
		planes[1].w = vp.l.w - vp.l.x;
		planes[2].x = vp.i.w - vp.i.y;
		planes[2].y = vp.j.w - vp.j.y;
		planes[2].z = vp.k.w - vp.k.y;
		planes[2].w = vp.l.w - vp.l.y;
		planes[3].x = vp.i.w + vp.i.y;
		planes[3].y = vp.j.w + vp.j.y;
		planes[3].z = vp.k.w + vp.k.y;
		planes[3].w = vp.l.w + vp.l.y;
		planes[5].x = vp.i.w - vp.i.z;
		planes[5].y = vp.j.w - vp.j.z;
		planes[5].z = vp.k.w - vp.k.z;
		planes[5].w = vp.l.w - vp.l.z;
		planes[4].x = vp.i.z;
		planes[4].y = vp.j.z;
		planes[4].z = vp.k.z;
		planes[4].w = vp.l.z;
		for (auto& p : planes) {
			p /= length(v3f{p.x, p.y, p.z});
		}
	}
	FORCEINLINE bool containsSphere(v3f position, float radius) const {
		for (auto p : planes) {
			if (dot(v3f{p.x, p.y, p.z}, position) + p.w + radius < 0) {
				return false;
			}
		}
		return true;
	}
};

namespace CE {

FORCEINLINE constexpr u32 countBits(u32 v) {
	u32 s = 0;
	u32 r = v;
	for (u32 i = 0; i < 32; ++i) {
		s += r & 1;
		r >>= 1;
	}
	return s;
}
FORCEINLINE constexpr v4s frac(v4s v, u32 step) {
	return {
		TL::frac(v.x, step),
		TL::frac(v.y, step),
		TL::frac(v.z, step),
		TL::frac(v.w, step),
	};
}

} // namespace CE

#define F32_16 F32x4
#define F32_32 F32x8
#define F32_64 F32x16
#define S32_16 S32x4
#define S32_32 S32x8
#define S32_64 S32x16
#define U32_16 U32x4
#define U32_32 U32x8
#define U32_64 U32x16
#define V2f_16 V2fx4
#define V2f_32 V2fx8
#define V2f_64 V2fx16
#define V3f_16 V3fx4
#define V3f_32 V3fx8
#define V3f_64 V3fx16
#define V4f_16 V4fx4
#define V4f_32 V4fx8
#define V4f_64 V4fx16

#define MAX_PACK_IMPL(p)								\
FORCEINLINE f32x<p/4> F32xm(f32 v = 0) { return F32_##p(v); } \
FORCEINLINE s32x<p/4> S32xm(s32 v = 0) { return S32_##p(v); } \
FORCEINLINE u32x<p/4> U32xm(u32 v = 0) { return U32_##p(v); } \
FORCEINLINE f32x<p/4> F32xm(M##p m) { return F32_##p(m); } \
FORCEINLINE s32x<p/4> S32xm(M##p m) { return S32_##p(m); } \
FORCEINLINE u32x<p/4> U32xm(M##p m) { return U32_##p(m); } \
FORCEINLINE v2fx<p/4> V2fxm(f32 v = 0) { return V2f_##p(v); } \
FORCEINLINE v3fx<p/4> V3fxm(f32 v = 0) { return V3f_##p(v); } \
FORCEINLINE v4fx<p/4> V4fxm(f32 v = 0) { return V4f_##p(v); } \
FORCEINLINE v2fx<p/4> V2fxm(v2f v) { return V2f_##p(v); } \
FORCEINLINE v3fx<p/4> V3fxm(v3f v) { return V3f_##p(v); } \
FORCEINLINE v4fx<p/4> V4fxm(v4f v) { return V4f_##p(v); } \
FORCEINLINE v2fx<p/4> V2fxm(f32 x, f32 y) { return V2f_##p(x, y); } \
FORCEINLINE v3fx<p/4> V3fxm(f32 x, f32 y, f32 z) { return V3f_##p(x, y, z); } \
FORCEINLINE v4fx<p/4> V4fxm(f32 x, f32 y, f32 z, f32 w) { return V4f_##p(x, y, z, w); } \
FORCEINLINE v2fx<p/4> V2fxm(f32xm x, f32xm y) { return V2f_##p(x, y); } \
FORCEINLINE v3fx<p/4> V3fxm(f32xm x, f32xm y, f32xm z) { return V3f_##p(x, y, z); } \
FORCEINLINE v4fx<p/4> V4fxm(f32xm x, f32xm y, f32xm z, f32xm w) { return V4f_##p(x, y, z, w); }

#ifndef TL_MATH_MAX_PACK
#if ARCH_AVX512F
#define TL_MATH_MAX_PACK 64
#elif ARCH_AVX
#define TL_MATH_MAX_PACK 32
#else
#define TL_MATH_MAX_PACK 16
#endif
#endif

static_assert(TL_MATH_MAX_PACK == 16 || TL_MATH_MAX_PACK == 32 ||TL_MATH_MAX_PACK == 64, "TL_MATH_MAX_PACK must be 16 (SSE), 32 (AVX) or 64 (AVX512)");

#define MAX_PACK_IMPL_(p) MAX_PACK_IMPL(p)

MAX_PACK_IMPL_(TL_MATH_MAX_PACK)

#undef MAX_PACK_IMPL_
#undef MAX_PACK_IMPL

template <class T>
inline static constexpr u32 simdElementCount = simdWidth / sizeof(T);

} // namespace TL

#undef MEMBERS2
#undef MEMBERS3
#undef MEMBERS4
#undef MEMFUNS_BASIC
#undef MEMFUNS_DATA
#undef MEMFUNS_INT
#undef MEMFUNS_SHIFT_SCL
#undef SHUFFLE

#undef MM_SHUFFLE
#undef MM256_PERM128

#if COMPILER_MSVC
#pragma warning(pop)
#endif

