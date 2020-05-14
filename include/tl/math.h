#pragma once
#include "common.h"
#include "asm.h"

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif
#include <float.h>
#include <math.h>
#include <memory.h>

#if COMPILER_MSVC
#pragma warning(disable : 5045) // spectre
#endif

namespace TL {

using M128 = ASM::XMM;
using M256 = ASM::YMM;
using M512 = ASM::ZMM;

#if ARCH_AVX512F
#define _simdWidth 64
#elif ARCH_AVX
#define _simdWidth 32
#else
#define _simdWidth 16
#endif

// simd register width in bytes
static constexpr u32 simdWidth = _simdWidth;

#undef _simdWidth

template<class T, u32 w, class UInt>
void _shuffleFallback(void *dst, void const *idx) {
	T src[w];
	memcpy(src, dst, sizeof(src));
	for(u32 i=0;i<w;++i) {
		((T *)dst)[i] = src[((UInt *)idx)[i]];
	}
}

#if ARCH_AVX
FORCEINLINE void _shuffle32x4(void *dst, void const *idx) { *(__m128*)dst = _mm_permutevar_ps(*(__m128*)dst, *(__m128i*)idx); }
#else
FORCEINLINE void _shuffle32x4(void *dst, void const *idx) { _shuffleFallback<u32, 4, u32>(dst, idx); }
#endif

#if ARCH_AVX2
FORCEINLINE void _gather32x4(void *dst, void const *memory, void const *offsets) { *(__m128*)dst = _mm_i32gather_ps((f32*)memory, *(__m128i*)offsets, 1); }
FORCEINLINE void _gather32x8(void *dst, void const *memory, void const *offsets) { *(__m256*)dst = _mm256_i32gather_ps((f32*)memory, *(__m256i*)offsets, 1); }
FORCEINLINE void _shuffle32x8(void *dst, void const *idx) { *(__m256*)dst = _mm256_permutevar_ps(*(__m256*)dst, *(__m256i*)idx); }
#else
FORCEINLINE void _gather32x4(void *_dst, void const *_memory, void const *_offsets) { 
	f32 *dst = (f32 *)_dst;
	u8 *memory = (u8*)_memory;
	s32 *offsets = (s32*)_offsets;
	_mm_storeu_ps(dst, _mm_setr_ps(*(f32*)(memory + offsets[0]), 
								   *(f32*)(memory + offsets[1]), 
								   *(f32*)(memory + offsets[2]), 
								   *(f32*)(memory + offsets[3]))); 
}
FORCEINLINE void _gather32x8(void *dst, void const *memory, void const *offsets) { 
	_gather32x4((u8*)dst +  0, memory, (u8*)offsets +  0);
	_gather32x4((u8*)dst + 16, memory, (u8*)offsets + 16);
}
FORCEINLINE void _shuffle32x8(void *dst, void const *idx) { _shuffleFallback<u32, 8, u32>(dst, idx); }
#endif

#if ARCH_AVX512F
FORCEINLINE void _gather32x16(void *dst, void const *memory, void const *offsets) { *(__m512*)dst = _mm512_i32gather_ps(*(__m512i*)offsets, (f32*)memory, sizeof(f32)); }
#else
FORCEINLINE void _gather32x16(void *dst, void const *memory, void const *offsets) { 
	_gather32x8((u8*)dst +  0, memory, (u8*)offsets +  0);
	_gather32x8((u8*)dst + 32, memory, (u8*)offsets + 32);
}
#endif

#define _mm_not_int(a)		 _mm_xor_si128(a, _mm_set1_epi32(~0))
#define _mm_cmpge_epu8(a, b) _mm_cmpeq_epi8(_mm_max_epu8(a, b), a)
#define _mm_cmple_epu8(a, b) _mm_cmpge_epu8(b, a)
#define _mm_cmpgt_epu8(a, b) _mm_not_int(_mm_cmple_epu8(a, b))
#define _mm_cmplt_epu8(a, b) _mm_cmpgt_epu8(b, a)

#define _mm_cmpge_epu16(a, b) _mm_cmpeq_epi16(_mm_max_epu16(a, b), a)
#define _mm_cmple_epu16(a, b) _mm_cmpge_epu16(b, a)
#define _mm_cmpgt_epu16(a, b) _mm_not_int(_mm_cmple_epu16(a, b))
#define _mm_cmplt_epu16(a, b) _mm_cmpgt_epu16(b, a)

#define _mm_cmpge_epu32(a, b) _mm_cmpeq_epi32(_mm_max_epu32(a, b), a)
#define _mm_cmple_epu32(a, b) _mm_cmpge_epu32(b, a)
#define _mm_cmpgt_epu32(a, b) _mm_not_int(_mm_cmple_epu32(a, b))
#define _mm_cmplt_epu32(a, b) _mm_cmpgt_epu32(b, a)

#define _mm_cmpeq_epu32(a, b) _mm_cmpeq_epi32(a, b)

#define _mm256_not_int(a) _mm256_xor_si256(a, _mm256_set1_epi32(~0))

#define _mm256_cmpge_epu32(a, b) _mm256_cmpeq_epi32(_mm256_max_epu32(a, b), a)
#define _mm256_cmple_epu32(a, b) _mm256_cmpge_epu32(b, a)
#define _mm256_cmpgt_epu32(a, b) _mm256_not_int(_mm256_cmple_epu32(a, b))
#define _mm256_cmplt_epu32(a, b) _mm256_cmpgt_epu32(b, a)

#define _mm256_cmpeq_epu32(a, b) _mm256_cmpeq_epi32(a, b)

#define _mm256_cmple_epi32(a, b) _mm256_cmpgt_epi32(b, a)
#define _mm256_cmpge_epi32(a, b) _mm256_or_si256(_mm256_cmpgt_epi32(a, b), _mm256_cmpeq_epi32(a, b))
#define _mm256_cmplt_epi32(a, b) _mm256_cmpge_epi32(b, a)

// clang-format off

constexpr f32 pi    = f32(3.1415926535897932384626433832795L);
constexpr f32 invPi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2 = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3 = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5 = f32(2.2360679774997896964091736687313L);

static const f32 _trueMask = []{ u32 v = ~0; return *(f32*)&v; }();
static const __m128 _trueMask128 = _mm_set1_ps(_trueMask);
static const __m256 _trueMask256 = _mm256_set1_ps(_trueMask);

template <class T> FORCEINLINE constexpr auto radians(T deg) { return deg * (T)(pi / 180.0L); }
template <class T> FORCEINLINE constexpr auto degrees(T rad) { return rad * (T)(180.0L / pi); }
template <class T, class U> FORCEINLINE constexpr auto min(T a, U b) { return a < b ? a : b; }
template <class T, class U> FORCEINLINE constexpr auto max(T a, U b) { return a > b ? a : b; }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto min(T a, U b, Rest... rest) { return min(min(a, b), rest...); }
template <class T, class U, class... Rest> FORCEINLINE constexpr auto max(T a, U b, Rest... rest) { return max(max(a, b), rest...); }
template <class T, class U, class V, class W> FORCEINLINE constexpr void minmax(T a, U b, V& mn, W& mx) { mn = min(a, b); mx = max(a, b); }
template <class T, class SN, class SX, class DN, class DX> FORCEINLINE constexpr auto map(T v, SN sn, SX sx, DN dn, DX dx) { return (v - sn) / (sx - sn) * (dx - dn) + dn; }
template <class T, class U, class V> FORCEINLINE constexpr auto clamp(T a, U mi, V ma) { return min(max(a, mi), ma); }
template <class T, class U> FORCEINLINE constexpr auto lerp(T a, U b, f32 t) { return a + (b - a) * t; }
template <class T> FORCEINLINE T select(bool mask, T a, T b) { return mask ? a : b; }
template <class T> FORCEINLINE constexpr auto pow2(T v) { return v * v; }
template <class T> FORCEINLINE constexpr auto pow3(T v) { return v * v * v; }
template <class T> FORCEINLINE constexpr auto pow4(T v) { return pow2(v * v); }
// clang-format on

namespace CE {

template <class To, class From>
FORCEINLINE constexpr To cvt(From v) {
	return (To)v;
}

} // namespace CE

// clang-format off
template<umm> union b32x; using b32x2 = b32x<2>; using b32x4 = b32x<4>; using b32x8 = b32x<8>; using b32x16 = b32x<16>;
template<umm> union f32x; using f32x2 = f32x<2>; using f32x4 = f32x<4>; using f32x8 = f32x<8>; using f32x16 = f32x<16>;
template<umm> union s32x; using s32x2 = s32x<2>; using s32x4 = s32x<4>; using s32x8 = s32x<8>; using s32x16 = s32x<16>;
template<umm> union u32x; using u32x2 = u32x<2>; using u32x4 = u32x<4>; using u32x8 = u32x<8>; using u32x16 = u32x<16>;
template<umm> union b64x; using b64x2 = b64x<2>; using b64x4 = b64x<4>; using b64x8 = b64x<8>; using b64x16 = b64x<16>;
template<umm> union f64x; using f64x2 = f64x<2>; using f64x4 = f64x<4>; using f64x8 = f64x<8>; using f64x16 = f64x<16>;

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

template<umm pack> using v2fx = _v2<f32, pack>; using v2fx2 = v2fx<2>; using v2fx4 = v2fx<4>; using v2fx8 = v2fx<8>; using v2fx16 = v2fx<16>;
template<umm pack> using v2sx = _v2<s32, pack>; using v2sx2 = v2sx<2>; using v2sx4 = v2sx<4>; using v2sx8 = v2sx<8>; using v2sx16 = v2sx<16>;
template<umm pack> using v2ux = _v2<u32, pack>; using v2ux2 = v2ux<2>; using v2ux4 = v2ux<4>; using v2ux8 = v2ux<8>; using v2ux16 = v2ux<16>;
template<umm pack> using v3fx = _v3<f32, pack>; using v3fx2 = v3fx<2>; using v3fx4 = v3fx<4>; using v3fx8 = v3fx<8>; using v3fx16 = v3fx<16>;
template<umm pack> using v3sx = _v3<s32, pack>; using v3sx2 = v3sx<2>; using v3sx4 = v3sx<4>; using v3sx8 = v3sx<8>; using v3sx16 = v3sx<16>;
template<umm pack> using v3ux = _v3<u32, pack>; using v3ux2 = v3ux<2>; using v3ux4 = v3ux<4>; using v3ux8 = v3ux<8>; using v3ux16 = v3ux<16>;
template<umm pack> using v4fx = _v4<f32, pack>; using v4fx2 = v4fx<2>; using v4fx4 = v4fx<4>; using v4fx8 = v4fx<8>; using v4fx16 = v4fx<16>;
template<umm pack> using v4sx = _v4<s32, pack>; using v4sx2 = v4sx<2>; using v4sx4 = v4sx<4>; using v4sx8 = v4sx<8>; using v4sx16 = v4sx<16>;
template<umm pack> using v4ux = _v4<u32, pack>; using v4ux2 = v4ux<2>; using v4ux4 = v4ux<4>; using v4ux8 = v4ux<8>; using v4ux16 = v4ux<16>;

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
template<umm pack> union v2fx; using v2fx2 = v2fx<2>; using v2fx4 = v2fx<4>; using v2fx8 = v2fx<8>; using v2fx16 = v2fx<16>;
template<umm pack> union v2sx; using v2sx2 = v2sx<2>; using v2sx4 = v2sx<4>; using v2sx8 = v2sx<8>; using v2sx16 = v2sx<16>;
template<umm pack> union v2ux; using v2ux2 = v2ux<2>; using v2ux4 = v2ux<4>; using v2ux8 = v2ux<8>; using v2ux16 = v2ux<16>;
template<umm pack> union v3fx; using v3fx2 = v3fx<2>; using v3fx4 = v3fx<4>; using v3fx8 = v3fx<8>; using v3fx16 = v3fx<16>;
template<umm pack> union v3sx; using v3sx2 = v3sx<2>; using v3sx4 = v3sx<4>; using v3sx8 = v3sx<8>; using v3sx16 = v3sx<16>;
template<umm pack> union v3ux; using v3ux2 = v3ux<2>; using v3ux4 = v3ux<4>; using v3ux8 = v3ux<8>; using v3ux16 = v3ux<16>;
template<umm pack> union v4fx; using v4fx2 = v4fx<2>; using v4fx4 = v4fx<4>; using v4fx8 = v4fx<8>; using v4fx16 = v4fx<16>;
template<umm pack> union v4sx; using v4sx2 = v4sx<2>; using v4sx4 = v4sx<4>; using v4sx8 = v4sx<8>; using v4sx16 = v4sx<16>;
template<umm pack> union v4ux; using v4ux2 = v4ux<2>; using v4ux4 = v4ux<4>; using v4ux8 = v4ux<8>; using v4ux16 = v4ux<16>;

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

union v2f;
union v2s;
union v2u;
union v3f;
union v3s;
union v3u;
union v4f;
union v4s;
union v4u;
#endif
// clang-format on

union m3;
union m4;

FORCEINLINE constexpr f32x8 F32x8(f32x4, f32x4);
FORCEINLINE constexpr s32x8 S32x8(s32x4, s32x4);
FORCEINLINE constexpr u32x8 U32x8(u32x4, u32x4);
FORCEINLINE constexpr f64x4 F64x4(f64x2, f64x2);

FORCEINLINE constexpr f32x16 F32x16(f32x8, f32x8);
FORCEINLINE constexpr s32x16 S32x16(s32x8, s32x8);
FORCEINLINE constexpr u32x16 U32x16(u32x8, u32x8);

template<umm width> FORCEINLINE f32x<width> F32x(f32 = 0);
template<umm width> FORCEINLINE s32x<width> S32x(s32 = 0);
template<umm width> FORCEINLINE u32x<width> U32x(u32 = 0);

FORCEINLINE constexpr f32x4 F32x4(M128);
FORCEINLINE constexpr s32x4 S32x4(M128);
FORCEINLINE constexpr u32x4 U32x4(M128);

FORCEINLINE constexpr f32x8 F32x8(M256);
FORCEINLINE constexpr s32x8 S32x8(M256);
FORCEINLINE constexpr u32x8 U32x8(M256);

FORCEINLINE constexpr f64x2 F64x2(M128);
//FORCEINLINE constexpr s64x2 S64x2(M128);
//FORCEINLINE constexpr u64x2 U64x2(M128);

FORCEINLINE constexpr f64x4 F64x4(M256);
//FORCEINLINE constexpr s64x4 S64x4(M256);
//FORCEINLINE constexpr u64x4 U64x4(M256);

FORCEINLINE f32x4 F32x4(f32 = 0);
FORCEINLINE s32x4 S32x4(s32 = 0);
FORCEINLINE u32x4 U32x4(u32 = 0);

FORCEINLINE f64x2 F64x2(f32 = 0);
FORCEINLINE f64x4 F64x4(f32 = 0);

FORCEINLINE f32x8 F32x8(f32 = 0);
FORCEINLINE s32x8 S32x8(s32 = 0);
FORCEINLINE u32x8 U32x8(u32 = 0);

FORCEINLINE f32x16 F32x16(f32 = 0);
FORCEINLINE s32x16 S32x16(s32 = 0);
FORCEINLINE u32x16 U32x16(u32 = 0);

FORCEINLINE f32x4 F32x4(f32, f32, f32, f32);
FORCEINLINE s32x4 S32x4(s32, s32, s32, s32);
FORCEINLINE u32x4 U32x4(u32, u32, u32, u32);

FORCEINLINE f32x8 F32x8(f32, f32, f32, f32, f32, f32, f32, f32);
FORCEINLINE s32x8 S32x8(s32, s32, s32, s32, s32, s32, s32, s32);
FORCEINLINE u32x8 U32x8(u32, u32, u32, u32, u32, u32, u32, u32);

FORCEINLINE f32x16 F32x16(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
FORCEINLINE s32x16 S32x16(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
FORCEINLINE u32x16 U32x16(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

FORCEINLINE constexpr v2f V2f(f32 = 0);
FORCEINLINE constexpr v2s V2s(s32 = 0);
FORCEINLINE constexpr v2u V2u(u32 = 0);

FORCEINLINE constexpr v3f V3f(f32 = 0);
FORCEINLINE constexpr v3s V3s(s32 = 0);
FORCEINLINE constexpr v3u V3u(u32 = 0);

FORCEINLINE constexpr v4f V4f(f32 = 0);
FORCEINLINE constexpr v4s V4s(s32 = 0);
FORCEINLINE constexpr v4u V4u(u32 = 0);

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

#define MOP(op, vec) \
	FORCEINLINE vec& operator op##=(vec b) { return *this = *this op b; }
#define MOPS(op, vec, scl, con) \
	FORCEINLINE vec& operator op##=(scl b) { return *this op## = con(b); }
#define OPS(op, vec, scl, con) \
	FORCEINLINE vec operator op(scl b) const { return *this op con(b); }
#define SOP(op, vec, scl, con) \
	FORCEINLINE friend vec operator op(scl a, vec b) { return con(a) op b; }

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

#define FALLBACK_UN_OP(ret, op, con, _128)			FORCEINLINE ret operator op(       ) const { return con(op _128[0], op _128[1]); }
#define FALLBACK_BIN_OP(ret, op, con, _128)			FORCEINLINE ret operator op(ret b  ) const { return con(   _128[0] op b._128[0], _128[1] op b._128[1]); }
#define FALLBACK_BIN_OP_S(ret, op, scl, con, _128)	FORCEINLINE ret operator op(scl b  ) const { return con(   _128[0] op b        , _128[1] op b); }
#define FALLBACK_CMP_OP(ret, op, param, _128)		FORCEINLINE ret operator op(param b) const { return    {   _128[0] op b._128[0], _128[1] op b._128[1]}; }

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
	FALLBACK_BIN_OP(u32x8, +, U32x8, _128)                    \
	FALLBACK_BIN_OP(u32x8, -, U32x8, _128)                    \
	FALLBACK_BIN_OP(u32x8, /, U32x8, _128)                    \
	FALLBACK_BIN_OP(u32x8, %, U32x8, _128)                    \
	FALLBACK_BIN_OP(u32x8, ^, U32x8, _128)                    \
	FALLBACK_BIN_OP(u32x8, |, U32x8, _128)                    \
	FALLBACK_BIN_OP(u32x8, &, U32x8, _128)                    \
	FALLBACK_BIN_OP_S(u32x8, <<, u32, U32x8, _128)            \
	FALLBACK_BIN_OP_S(u32x8, >>, u32, U32x8, _128)            \
	FALLBACK_CMP_OP(b32x8, <, u32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, >, u32x8, _128)                 \
	FALLBACK_CMP_OP(b32x8, <=, u32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, >=, u32x8, _128)                \
	FALLBACK_CMP_OP(b32x8, ==, u32x8, _128)

#define MASK_IMPL(b32, b32x4, M128, width)								\
	b32 s[width];														\
	M128 m;																\
	FORCEINLINE b32x4() = default;										\
	FORCEINLINE b32x4(M128 m) : m(m) {}									\
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

template<>
union b32x<4> {
	MASK_IMPL(b32, b32x4, M128, 4)
	FORCEINLINE operator u8() const { return compressMask32(m); }
};

template<>
union b32x<8> {
	struct {
		b32x4 l;
		b32x4 h;
	};
	FORCEINLINE b32x8(b32x4 l, b32x4 h) : l(l), h(h) {}
	MASK_IMPL(b32, b32x8, M256, 8)
	FORCEINLINE operator u8() const { return compressMask32(m); }
};
template<>
union b32x<16> {
	struct {
		b32x8 l;
		b32x8 h;
	};
	FORCEINLINE b32x16(b32x8 l, b32x8 h) : l(l), h(h) {}
	MASK_IMPL(b32, b32x16, M512, 16)
	FORCEINLINE operator u16() const { return compressMask32(m); }
};

template<>
union b64x<2> {
	MASK_IMPL(b64, b64x2, M128, 2)
	FORCEINLINE operator u8() const { return compressMask64(m); }
};

template<>
union b64x<4> {
	struct {
		b64x2 l;
		b64x2 h;
	};
	FORCEINLINE b64x4(b64x2 l, b64x2 h) : l(l), h(h) {}
	MASK_IMPL(b64, b64x4, M256, 4)
	FORCEINLINE operator u8() const { return compressMask64(m); }
};

#undef MASK_IMPL

FORCEINLINE b32x4 B32x4(bool v) { b32x4 m; memset(&m, v ? ~0 : 0, sizeof(m)); return m; }
FORCEINLINE b32x8 B32x8(bool v) { b32x8 m; memset(&m, v ? ~0 : 0, sizeof(m)); return m; }

FORCEINLINE b32x4 andNot(b32x4 a, b32x4 b) { return andNot(a.m, b.m); }
FORCEINLINE b32x8 andNot(b32x8 a, b32x8 b) { return andNot(a.m, b.m); }

FORCEINLINE bool allTrue (b32x4 v) { return (u32)v == 0xF; }
FORCEINLINE bool anyTrue (b32x4 v) { return (u32)v != 0;   }
FORCEINLINE bool allFalse(b32x4 v) { return (u32)v == 0;   }
FORCEINLINE bool anyFalse(b32x4 v) { return (u32)v != 0xF; }
FORCEINLINE bool allTrue (b32x8 v) { return (u32)v == 0xFF; }
FORCEINLINE bool anyTrue (b32x8 v) { return (u32)v != 0;    }
FORCEINLINE bool allFalse(b32x8 v) { return (u32)v == 0;    }
FORCEINLINE bool anyFalse(b32x8 v) { return (u32)v != 0xFF; }

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
union f32x<4> {
	f32 s[4];
	M128 m;
	FORCEINLINE f32x4 operator-() const { return F32x4(_mm_xor_ps(m, _mm_set1_ps(-0.0f))); }
	FORCEINLINE f32x4 operator+(f32x4 b) const { return F32x4(_mm_add_ps(m, b.m)); }
	FORCEINLINE f32x4 operator-(f32x4 b) const { return F32x4(_mm_sub_ps(m, b.m)); }
	FORCEINLINE f32x4 operator*(f32x4 b) const { return F32x4(_mm_mul_ps(m, b.m)); }
	FORCEINLINE f32x4 operator/(f32x4 b) const { return F32x4(_mm_div_ps(m, b.m)); }
	FORCEINLINE b32x4 operator<(f32x4 b) const { return  _mm_cmp_ps(m, b.m, _CMP_LT_OQ); }
	FORCEINLINE b32x4 operator>(f32x4 b) const { return  _mm_cmp_ps(m, b.m, _CMP_GT_OQ); }
	FORCEINLINE b32x4 operator<=(f32x4 b) const { return _mm_cmp_ps(m, b.m, _CMP_LE_OQ); }
	FORCEINLINE b32x4 operator>=(f32x4 b) const { return _mm_cmp_ps(m, b.m, _CMP_GE_OQ); }
	FORCEINLINE b32x4 operator==(f32x4 b) const { return _mm_cmp_ps(m, b.m, _CMP_EQ_OQ); }
	FORCEINLINE b32x4 operator!=(f32x4 b) const { return _mm_cmp_ps(m, b.m, _CMP_NEQ_OQ); }
	CVT(s32x4);
	CVT(u32x4);
	SCLX_CMP(b32x4, f32x4, f32, F32x4)
	MEMFUNS_DATA(f32);
	INDEX_S(f32);
	MEMFUNS_BASIC(f32x4, f32, F32x4)
};

#define OP(op) \
	FORCEINLINE s32x4 operator op(s32 b) const { return s32x4(*this) op##= b; } 
template<>
union s32x<4> {
	s32 s[4];
	M128 m;
	FORCEINLINE s32x4 operator-() const { return S32x4(_mm_sub_epi32(_mm_setzero_si128(), m)); }
	FORCEINLINE s32x4 operator~() const { return *this ^ (~0); }
	FORCEINLINE s32x4 operator+(s32x4 b) const { return S32x4(  _mm_add_epi32(m, b.m)); }
	FORCEINLINE s32x4 operator-(s32x4 b) const { return S32x4(  _mm_sub_epi32(m, b.m)); }
	FORCEINLINE s32x4 operator*(s32x4 b) const { return S32x4(_mm_mullo_epi32(m, b.m)); }
	FORCEINLINE s32x4 operator^(s32x4 b) const { return S32x4(  _mm_xor_si128(m, b.m)); }
	FORCEINLINE s32x4 operator|(s32x4 b) const { return S32x4(   _mm_or_si128(m, b.m)); }
	FORCEINLINE s32x4 operator&(s32x4 b) const { return S32x4(  _mm_and_si128(m, b.m)); }
	FORCEINLINE s32x4 operator<<(s32x4 b) const { return S32x4(s[0] << b.s[0], s[1] << b.s[1], s[2] << b.s[2], s[3] << b.s[3]); }
	FORCEINLINE s32x4 operator>>(s32x4 b) const { return S32x4(s[0] >> b.s[0], s[1] >> b.s[1], s[2] >> b.s[2], s[3] >> b.s[3]); }
	FORCEINLINE s32x4 operator/(s32x4 b) const { return S32x4(s[0] / b.s[0], s[1] / b.s[1], s[2] / b.s[2], s[3] / b.s[3]); }
	FORCEINLINE s32x4 operator%(s32x4 b) const { return S32x4(s[0] % b.s[0], s[1] % b.s[1], s[2] % b.s[2], s[3] % b.s[3]); }
	FORCEINLINE s32x4 operator<<(s32 b) const { return S32x4(_mm_slli_epi32(m, b)); }
	FORCEINLINE s32x4 operator>>(s32 b) const { return S32x4(_mm_srai_epi32(m, b)); }
#if 0
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&)
	FORCEINLINE s32x4& operator+=(s32x4 b) { return *this = *this + b; }
	FORCEINLINE s32x4& operator-=(s32x4 b) { return *this = *this - b; }
	FORCEINLINE s32x4& operator*=(s32x4 b) { return *this = *this * b; }
	FORCEINLINE s32x4& operator/=(s32x4 b) { return s[0] /= b.s[0], s[1] /= b.s[1], s[2] /= b.s[2], s[3] /= b.s[3], *this; }
	FORCEINLINE s32x4& operator%=(s32x4 b) { return s[0] %= b.s[0], s[1] %= b.s[1], s[2] %= b.s[2], s[3] %= b.s[3], *this; }
	FORCEINLINE s32x4& operator^=(s32x4 b) { return *this = *this ^ b; }
	FORCEINLINE s32x4& operator|=(s32x4 b) { return *this = *this | b; }
	FORCEINLINE s32x4& operator&=(s32x4 b) { return *this = *this & b; }
	FORCEINLINE s32x4& operator+=(s32 b) { return *this += S32x4(b); }
	FORCEINLINE s32x4& operator-=(s32 b) { return *this -= S32x4(b); }
	FORCEINLINE s32x4& operator*=(s32 b) { return *this *= S32x4(b); }
	FORCEINLINE s32x4& operator/=(s32 b) { return s[0] /= b, s[1] /= b, s[2] /= b, s[3] /= b, *this; }
	FORCEINLINE s32x4& operator%=(s32 b) { return s[0] %= b, s[1] %= b, s[2] %= b, s[3] %= b, *this; }
	FORCEINLINE s32x4& operator^=(s32 b) { return *this ^= S32x4(b); }
	FORCEINLINE s32x4& operator|=(s32 b) { return *this |= S32x4(b); }
	FORCEINLINE s32x4& operator&=(s32 b) { return *this &= S32x4(b); }
#endif
	FORCEINLINE b32x4 operator<(s32x4 b) const { return  _mm_cmplt_epi32(m, b.m); }
	FORCEINLINE b32x4 operator>(s32x4 b) const { return  _mm_cmpgt_epi32(m, b.m); }
	FORCEINLINE b32x4 operator<=(s32x4 b) const { return _mm_cmpgt_epi32(b.m, m); }
	FORCEINLINE b32x4 operator>=(s32x4 b) const { return _mm_cmplt_epi32(b.m, m); }
	FORCEINLINE b32x4 operator==(s32x4 b) const { return _mm_cmpeq_epi32(m, b.m); }
	FORCEINLINE b32x4 operator!=(s32x4 b) const { return !(*this == b); }
	CVT(f32x4);
	CVT(u32x4);
	SCLX_CMP(b32x4, s32x4, s32, S32x4)
	MEMFUNS_BASIC(s32x4, s32, S32x4);
	MEMFUNS_INT(s32x4, s32, S32x4);
	MEMFUNS_DATA(s32);
	INDEX_S(s32);
};
template<>
union u32x<4> {
	u32 s[4];
	M128 m;
	FORCEINLINE u32x4 operator~() const { return *this ^ (~0u); }
	FORCEINLINE u32x4 operator+(u32x4 b) const { return U32x4(_mm_add_epi32(m, b.m)); }
	FORCEINLINE u32x4 operator-(u32x4 b) const { return U32x4(_mm_sub_epi32(m, b.m)); }
	FORCEINLINE u32x4 operator*(u32x4 b) const { return {s[0] * b.s[0], s[1] * b.s[1], s[2] * b.s[2], s[3] * b.s[3]}; }
#if COMPILER_GCC
	FORCEINLINE u32x4 operator/(u32x4 b) const { return {s[0] / b.s[0], s[1] / b.s[1], s[2] / b.s[2], s[3] / b.s[3]}; }
	FORCEINLINE u32x4 operator%(u32x4 b) const { return {s[0] % b.s[0], s[1] % b.s[1], s[2] % b.s[2], s[3] % b.s[3]}; }
#else
	FORCEINLINE u32x4 operator/(u32x4 b) const { return U32x4(_mm_div_epu32(m, b.m)); }
	FORCEINLINE u32x4 operator%(u32x4 b) const { return U32x4(_mm_rem_epu32(m, b.m)); }
#endif
	FORCEINLINE u32x4 operator^(u32x4 b) const { return U32x4(_mm_xor_si128(m, b.m)); }
	FORCEINLINE u32x4 operator|(u32x4 b) const { return U32x4(_mm_or_si128 (m, b.m)); }
	FORCEINLINE u32x4 operator&(u32x4 b) const { return U32x4(_mm_and_si128(m, b.m)); }
	FORCEINLINE u32x4 operator<<(u32x4 b) const { return U32x4(s[0] << b.s[0], s[1] << b.s[1], s[2] << b.s[2], s[3] << b.s[3]); }
	FORCEINLINE u32x4 operator>>(u32x4 b) const { return U32x4(s[0] >> b.s[0], s[1] >> b.s[1], s[2] >> b.s[2], s[3] >> b.s[3]); }
	FORCEINLINE u32x4 operator<<(u32 b) const { return U32x4(_mm_slli_epi32(m, (s32)b)); }
	FORCEINLINE u32x4 operator>>(u32 b) const { return U32x4(_mm_srli_epi32(m, (s32)b)); }
	FORCEINLINE b32x4 operator<(u32x4 b) const { return  _mm_cmplt_epu32(m, b.m); }
	FORCEINLINE b32x4 operator>(u32x4 b) const { return  _mm_cmpgt_epu32(m, b.m); }
	FORCEINLINE b32x4 operator<=(u32x4 b) const { return _mm_cmple_epu32(m, b.m); }
	FORCEINLINE b32x4 operator>=(u32x4 b) const { return _mm_cmpge_epu32(m, b.m); }
	FORCEINLINE b32x4 operator==(u32x4 b) const { return _mm_cmpeq_epu32(m, b.m); }
	FORCEINLINE b32x4 operator!=(u32x4 b) const { return !(*this == b); }
	CVT(f32x4);
	CVT(s32x4);
	CVT(f64x4);

	SCLX_CMP(b32x4, u32x4, u32, U32x4)
	MEMFUNS_BASIC(u32x4, u32, U32x4);
	MEMFUNS_INT(u32x4, u32, U32x4);
	MEMFUNS_DATA(u32);
	INDEX_S(u32);
};

template <>
union f32x<8> {
	f32 s[8];
	f32x4 _128[2];
	M256 m;
#if ARCH_AVX
	FORCEINLINE f32x8 operator-() const { return F32x8(_mm256_xor_ps(m, _mm256_set1_ps(-0.0f))); }
	FORCEINLINE f32x8 operator+(f32x8 b) const { return F32x8(_mm256_add_ps(m, b.m)); }
	FORCEINLINE f32x8 operator-(f32x8 b) const { return F32x8(_mm256_sub_ps(m, b.m)); }
	FORCEINLINE f32x8 operator*(f32x8 b) const { return F32x8(_mm256_mul_ps(m, b.m)); }
	FORCEINLINE f32x8 operator/(f32x8 b) const { return F32x8(_mm256_div_ps(m, b.m)); }
	FORCEINLINE b32x8 operator<(f32x8 b) const { return       _mm256_cmp_ps(m, b.m, _CMP_LT_OQ); }
	FORCEINLINE b32x8 operator>(f32x8 b) const { return       _mm256_cmp_ps(m, b.m, _CMP_GT_OQ); }
	FORCEINLINE b32x8 operator<=(f32x8 b) const { return      _mm256_cmp_ps(m, b.m, _CMP_LE_OQ); }
	FORCEINLINE b32x8 operator>=(f32x8 b) const { return      _mm256_cmp_ps(m, b.m, _CMP_GE_OQ); }
	FORCEINLINE b32x8 operator==(f32x8 b) const { return      _mm256_cmp_ps(m, b.m, _CMP_EQ_OQ); }
	FORCEINLINE b32x8 operator!=(f32x8 b) const { return      _mm256_cmp_ps(m, b.m, _CMP_NEQ_OQ); }
#else
	FALLBACK_F32X_OPERATORS(f32x8, F32x8, b32x8, _128)
#endif
	CVT(s32x8);
	CVT(u32x8);
	SCLX_CMP(b32x8, f32x8, f32, F32x8)
	MEMFUNS_DATA(f32);
	INDEX_S(f32);
	MEMFUNS_BASIC(f32x8, f32, F32x8)
};

template<>
union s32x<8> {
	s32 s[8];
	s32x4 _128[2];
	M256 m;
#if ARCH_AVX2
	FORCEINLINE s32x8 operator-() const { return S32x8(_mm256_sub_epi32(_mm256_setzero_si256(), m)); }
	FORCEINLINE s32x8 operator+(s32x8 b) const { return S32x8(_mm256_add_epi32  (m, b.m)); }
	FORCEINLINE s32x8 operator-(s32x8 b) const { return S32x8(_mm256_sub_epi32  (m, b.m)); }
	FORCEINLINE s32x8 operator*(s32x8 b) const { return S32x8(_mm256_mullo_epi32(m, b.m)); }
#if COMPILER_GCC
	FALLBACK_BIN_OP(s32x8, /, S32x8, _128)
	FALLBACK_BIN_OP(s32x8, %, S32x8, _128)
#else
	FORCEINLINE s32x8 operator/(s32x8 b) const { return S32x8(_mm256_div_epi32(m, b.m)); }
	FORCEINLINE s32x8 operator%(s32x8 b) const { return S32x8(_mm256_rem_epi32(m, b.m)); }
#endif
	FORCEINLINE s32x8 operator^(s32x8 b) const { return S32x8(_mm256_xor_si256(m, b.m)); }
	FORCEINLINE s32x8 operator|(s32x8 b) const { return S32x8( _mm256_or_si256(m, b.m)); }
	FORCEINLINE s32x8 operator&(s32x8 b) const { return S32x8(_mm256_and_si256(m, b.m)); }
	FORCEINLINE s32x8 operator<<(s32 b) const { return S32x8(_mm256_slli_epi32(m, b)); }
	FORCEINLINE s32x8 operator>>(s32 b) const { return S32x8(_mm256_srai_epi32(m, b)); }
	FORCEINLINE b32x8 operator<(s32x8 b) const { return     _mm256_cmplt_epi32(m, b.m); }
	FORCEINLINE b32x8 operator>(s32x8 b) const { return     _mm256_cmpgt_epi32(m, b.m); }
	FORCEINLINE b32x8 operator<=(s32x8 b) const { return    _mm256_cmpgt_epi32(b.m, m); }
	FORCEINLINE b32x8 operator>=(s32x8 b) const { return    _mm256_cmplt_epi32(b.m, m); }
	FORCEINLINE b32x8 operator==(s32x8 b) const { return    _mm256_cmpeq_epi32(m, b.m); }
#else
	FALLBACK_S32X_OPERATORS(s32x8, S32x8, b32x8, _128)
#endif
	FORCEINLINE s32x8 operator~() const { return *this ^ (~0); }
	FALLBACK_BIN_OP(s32x8, <<, S32x8, _128)
	FALLBACK_BIN_OP(s32x8, >>, S32x8, _128)
	b32x8 operator!=(s32x8 b) const { return !(*this == b); }
	CVT(f32x8);
	CVT(u32x8);
	SCLX_CMP(b32x8, s32x8, s32, S32x8)
	MEMFUNS_BASIC(s32x8, s32, S32x8);
	MEMFUNS_INT(s32x8, s32, S32x8);
	MEMFUNS_DATA(s32);
	INDEX_S(s32);
};
template<>
union u32x<8> {
	u32 s[8];
	u32x4 _128[2];
	M256 m;
#if ARCH_AVX2
	FORCEINLINE u32x8 operator+(u32x8 b) const { return U32x8(_mm256_add_epi32(m, b.m)); }
	FORCEINLINE u32x8 operator-(u32x8 b) const { return U32x8(_mm256_sub_epi32(m, b.m)); }
#if COMPILER_GCC
	FALLBACK_BIN_OP(u32x8, /, U32x8, _128)
	FALLBACK_BIN_OP(u32x8, %, U32x8, _128)
#else
	FORCEINLINE u32x8 operator/(u32x8 b) const { return U32x8(_mm256_div_epu32(m, b.m)); }
	FORCEINLINE u32x8 operator%(u32x8 b) const { return U32x8(_mm256_rem_epu32(m, b.m)); }
#endif
	FORCEINLINE u32x8 operator^(u32x8 b) const { return U32x8(_mm256_xor_si256(m, b.m)); }
	FORCEINLINE u32x8 operator|(u32x8 b) const { return U32x8(_mm256_or_si256 (m, b.m)); }
	FORCEINLINE u32x8 operator&(u32x8 b) const { return U32x8(_mm256_and_si256(m, b.m)); }
	FORCEINLINE u32x8 operator<<(u32 b) const { return U32x8(_mm256_slli_epi32(m, (s32)b)); }
	FORCEINLINE u32x8 operator>>(u32 b) const { return U32x8(_mm256_srli_epi32(m, (s32)b)); }
	FORCEINLINE b32x8 operator<(u32x8 b) const { return _mm256_cmplt_epu32    (m, b.m); }
	FORCEINLINE b32x8 operator>(u32x8 b) const { return _mm256_cmpgt_epu32    (m, b.m); }
	FORCEINLINE b32x8 operator<=(u32x8 b) const { return _mm256_cmple_epu32   (m, b.m); }
	FORCEINLINE b32x8 operator>=(u32x8 b) const { return _mm256_cmpge_epu32   (m, b.m); }
	FORCEINLINE b32x8 operator==(u32x8 b) const { return _mm256_cmpeq_epu32   (m, b.m); }
#else
	FALLBACK_U32X_OPERATORS(u32x8, U32x8, b32x8, _128)
#endif
	FORCEINLINE u32x8 operator~() const { return *this ^ (~0u); }
	FALLBACK_BIN_OP(u32x8, * , U32x8, _128)
	FALLBACK_BIN_OP(u32x8, <<, U32x8, _128)
	FALLBACK_BIN_OP(u32x8, >>, U32x8, _128)
	FORCEINLINE b32x8 operator!=(u32x8 b) const { return !(*this == b); }
	CVT(f32x8);
	CVT(s32x8);
	SCLX_CMP(b32x8, u32x8, u32, U32x8)
	MEMFUNS_BASIC(u32x8, u32, U32x8);
	MEMFUNS_INT(u32x8, u32, U32x8);
	MEMFUNS_DATA(u32);
	INDEX_S(u32);
};
template<>
union f32x<16> {
	f32 s[16];
	f32x4 _128[4];
	f32x8 _256[4];
	M512 m;
#if ARCH_AVX512F
	FORCEINLINE f32x16 operator-() const { return F32x16(_mm512_sub_ps(_mm512_setzero_ps(), m)); }
	FORCEINLINE f32x16 operator+(f32x16 b) const { return F32x16(_mm512_add_ps(m, b.m)); }
	FORCEINLINE f32x16 operator-(f32x16 b) const { return F32x16(_mm512_sub_ps(m, b.m)); }
	FORCEINLINE f32x16 operator*(f32x16 b) const { return F32x16(_mm512_mul_ps(m, b.m)); }
	FORCEINLINE f32x16 operator/(f32x16 b) const { return F32x16(_mm512_div_ps(m, b.m)); }
	FORCEINLINE b32x16 operator<(f32x16 b) const { return _mm512_cmp_ps(m, b.m, _CMP_LT_OQ); }
	FORCEINLINE b32x16 operator>(f32x16 b) const { return _mm512_cmp_ps(m, b.m, _CMP_GT_OQ); }
	FORCEINLINE b32x16 operator<=(f32x16 b) const { return _mm512_cmp_ps(m, b.m, _CMP_LE_OQ); }
	FORCEINLINE b32x16 operator>=(f32x16 b) const { return _mm512_cmp_ps(m, b.m, _CMP_GE_OQ); }
	FORCEINLINE b32x16 operator==(f32x16 b) const { return _mm512_cmp_ps(m, b.m, _CMP_EQ_OQ); }
	FORCEINLINE b32x16 operator!=(f32x16 b) const { return _mm512_cmp_ps(m, b.m, _CMP_NEQ_OQ); }
#else
	FALLBACK_F32X_OPERATORS(f32x16, F32x16, b32x16, _256)
#endif
	CVT(s32x16);
	CVT(u32x16);
	SCLX_CMP(b32x16, f32x16, f32, F32x16)
	MEMFUNS_DATA(f32);
	INDEX_S(f32);
	MEMFUNS_BASIC(f32x16, f32, F32x16)
};
template<>
union s32x<16> {
	s32 s[16];
	s32x8 _256[2];
	__m512i m;
#if ARCH_AVX512F
	FORCEINLINE s32x16 operator-() const { return S32x16(_mm512_sub_epi32(_mm512_setzero_si512(), m)); }
	FORCEINLINE s32x16 operator+(s32x16 b) const { return S32x16(_mm512_add_epi32(m, b.m)); }
	FORCEINLINE s32x16 operator-(s32x16 b) const { return S32x16(_mm512_sub_epi32(m, b.m)); }
	FORCEINLINE s32x16 operator*(s32x16 b) const { return S32x16(_mm512_mullo_epi32(m, b.m)); }
#if COMPILER_GCC
	FALLBACK_BIN_OP(s32x16, /, S32x16, _256)
	FALLBACK_BIN_OP(s32x16, %, S32x16, _256)
#else
	FORCEINLINE s32x16 operator/(s32x16 b) const { return S32x16(_mm512_div_epi32(m, b.m)); }
	FORCEINLINE s32x16 operator%(s32x16 b) const { return S32x16(_mm512_rem_epi32(m, b.m)); }
#endif
	FORCEINLINE s32x16 operator^(s32x16 b) const { return S32x16(_mm512_xor_si512(m, b.m)); }
	FORCEINLINE s32x16 operator|(s32x16 b) const { return S32x16(_mm512_or_si512(m, b.m)); }
	FORCEINLINE s32x16 operator&(s32x16 b) const { return S32x16(_mm512_and_si512(m, b.m)); }
	FORCEINLINE s32x16 operator<<(s32 b) const { return S32x16(_mm512_slli_epi32(m, b)); }
	FORCEINLINE s32x16 operator>>(s32 b) const { return S32x16(_mm512_srai_epi32(m, b)); }
	FORCEINLINE b32x16 operator<(s32x16 b) const { return _mm512_cmplt_epi32(m, b.m); }
	FORCEINLINE b32x16 operator>(s32x16 b) const { return _mm512_cmpgt_epi32(m, b.m); }
	FORCEINLINE b32x16 operator<=(s32x16 b) const { return _mm512_cmpgt_epi32(b.m, m); }
	FORCEINLINE b32x16 operator>=(s32x16 b) const { return _mm512_cmplt_epi32(b.m, m); }
	FORCEINLINE b32x16 operator==(s32x16 b) const { return _mm512_cmpeq_epi32(m, b.m); }
#else
	FALLBACK_S32X_OPERATORS(s32x16, S32x16, b32x16, _256)
#endif
	s32x16 operator~() const { return *this ^ (~0); }
	FALLBACK_BIN_OP(s32x16, <<, S32x16, _256)
	FALLBACK_BIN_OP(s32x16, >>, S32x16, _256)
	b32x16 operator!=(s32x16 b) const { return !(*this == b); }
	CVT(f32x16);
	CVT(u32x16);
	SCLX_CMP(b32x16, s32x16, s32, S32x16)
	MEMFUNS_BASIC(s32x16, s32, S32x16);
	MEMFUNS_INT(s32x16, s32, S32x16);
	MEMFUNS_DATA(s32);
	INDEX_S(s32);
};
template<>
union u32x<16> {
	u32 s[16];
	u32x8 _256[2];
	__m512i m;
#if ARCH_AVX512F
	FORCEINLINE u32x16 operator+(u32x16 b) const { return U32x16(_mm512_add_epi32(m, b.m)); }
	FORCEINLINE u32x16 operator-(u32x16 b) const { return U32x16(_mm512_sub_epi32(m, b.m)); }
#if COMPILER_GCC
	FALLBACK_BIN_OP(u32x16, /, U32x16, _256)
	FALLBACK_BIN_OP(u32x16, %, U32x16, _256)
#else
	FORCEINLINE u32x16 operator/(u32x16 b) const { return U32x16(_mm512_div_epu32(m, b.m)); }
	FORCEINLINE u32x16 operator%(u32x16 b) const { return U32x16(_mm512_rem_epu32(m, b.m)); }
#endif
	FORCEINLINE u32x16 operator^(u32x16 b) const { return U32x16(_mm512_xor_si512(m, b.m)); }
	FORCEINLINE u32x16 operator|(u32x16 b) const { return U32x16(_mm512_or_si512(m, b.m)); }
	FORCEINLINE u32x16 operator&(u32x16 b) const { return U32x16(_mm512_and_si512(m, b.m)); }
	FORCEINLINE u32x16 operator<<(u32 b) const { return U32x16(_mm512_slli_epi32(m, (s32)b)); }
	FORCEINLINE u32x16 operator>>(u32 b) const { return U32x16(_mm512_srli_epi32(m, (s32)b)); }
	FORCEINLINE b32x16 operator<(u32x16 b) const { return _mm512_cmplt_epu32(m, b.m); }
	FORCEINLINE b32x16 operator>(u32x16 b) const { return _mm512_cmpgt_epu32(m, b.m); }
	FORCEINLINE b32x16 operator<=(u32x16 b) const { return _mm512_cmple_epu32(m, b.m); }
	FORCEINLINE b32x16 operator>=(u32x16 b) const { return _mm512_cmpge_epu32(m, b.m); }
	FORCEINLINE b32x16 operator==(u32x16 b) const { return _mm512_cmpeq_epu32(m, b.m); }
#else
	FALLBACK_U32X_OPERATORS(u32x16, U32x16, b32x16, _256)
#endif
	FORCEINLINE u32x16 operator~() const { return *this ^ (~0u); }
	FALLBACK_BIN_OP(u32x16, * , U32x16, _256)
	FALLBACK_BIN_OP(u32x16, <<, U32x16, _256)
	FALLBACK_BIN_OP(u32x16, >>, U32x16, _256)
	FORCEINLINE b32x16 operator!=(u32x16 b) const { return !(*this == b); }
	CVT(f32x16);
	CVT(s32x16);
	SCLX_CMP(b32x16, u32x16, u32, U32x16)
	MEMFUNS_BASIC(u32x16, u32, U32x16);
	MEMFUNS_INT(u32x16, u32, U32x16);
	MEMFUNS_DATA(u32);
	INDEX_S(u32);
};

template <>
union f64x<2> {
	f64 s[2];
	M128 p;
	FORCEINLINE f64x2 operator-() const { return F64x2(_mm_xor_pd(p, _mm_set1_pd(-0.0))); }
	FORCEINLINE f64x2 operator+(f64x2 b) const { return F64x2(_mm_add_pd(p, b.p)); }
	FORCEINLINE f64x2 operator-(f64x2 b) const { return F64x2(_mm_sub_pd(p, b.p)); }
	FORCEINLINE f64x2 operator*(f64x2 b) const { return F64x2(_mm_mul_pd(p, b.p)); }
	FORCEINLINE f64x2 operator/(f64x2 b) const { return F64x2(_mm_div_pd(p, b.p)); }
	FORCEINLINE b64x2 operator<(f64x2 b) const { return _mm_cmp_pd(p, b.p, _CMP_LT_OQ); }
	FORCEINLINE b64x2 operator>(f64x2 b) const { return _mm_cmp_pd(p, b.p, _CMP_GT_OQ); }
	FORCEINLINE b64x2 operator<=(f64x2 b) const { return _mm_cmp_pd(p, b.p, _CMP_LE_OQ); }
	FORCEINLINE b64x2 operator>=(f64x2 b) const { return _mm_cmp_pd(p, b.p, _CMP_GE_OQ); }
	FORCEINLINE b64x2 operator==(f64x2 b) const { return _mm_cmp_pd(p, b.p, _CMP_EQ_OQ); }
	FORCEINLINE b64x2 operator!=(f64x2 b) const { return _mm_cmp_pd(p, b.p, _CMP_NEQ_OQ); }
	//CVT(s64x2);
	//CVT(u64x2);
	SCLX_CMP(b64x2, f64x2, f64, F64x2)
	MEMFUNS_DATA(f64);
	INDEX_S(f64);
	MEMFUNS_BASIC(f64x2, f64, F64x2)
};
template <>
union f64x<4> {
	f64 s[4];
	f64x2 _128[2];
	M256 p;
#if ARCH_AVX
	FORCEINLINE f64x4 operator-() const { return F64x4(_mm256_xor_pd(p, _mm256_set1_pd(-0.0))); }
	FORCEINLINE f64x4 operator+(f64x4 b) const { return F64x4(_mm256_add_pd(p, b.p)); }
	FORCEINLINE f64x4 operator-(f64x4 b) const { return F64x4(_mm256_sub_pd(p, b.p)); }
	FORCEINLINE f64x4 operator*(f64x4 b) const { return F64x4(_mm256_mul_pd(p, b.p)); }
	FORCEINLINE f64x4 operator/(f64x4 b) const { return F64x4(_mm256_div_pd(p, b.p)); }
	FORCEINLINE b64x4 operator<(f64x4 b) const { return _mm256_cmp_pd(p, b.p, _CMP_LT_OQ); }
	FORCEINLINE b64x4 operator>(f64x4 b) const { return _mm256_cmp_pd(p, b.p, _CMP_GT_OQ); }
	FORCEINLINE b64x4 operator<=(f64x4 b) const { return _mm256_cmp_pd(p, b.p, _CMP_LE_OQ); }
	FORCEINLINE b64x4 operator>=(f64x4 b) const { return _mm256_cmp_pd(p, b.p, _CMP_GE_OQ); }
	FORCEINLINE b64x4 operator==(f64x4 b) const { return _mm256_cmp_pd(p, b.p, _CMP_EQ_OQ); }
	FORCEINLINE b64x4 operator!=(f64x4 b) const { return _mm256_cmp_pd(p, b.p, _CMP_NEQ_OQ); }
#else
	FALLBACK_F32X_OPERATORS(f64x4, F64x4, b64x4, _128)
#endif
	CVT(u32x4);
	//CVT(s64x4);
	//CVT(u64x4);
	SCLX_CMP(b64x4, f64x4, f64, F64x4)
	MEMFUNS_DATA(f64);
	INDEX_S(f64);
	MEMFUNS_BASIC(f64x4, f64, F64x4)
};


FORCEINLINE b32x4 B32x4(b32 a, b32 b, b32 c, b32 d) { return S32x4(a, b, c, d) != 0; }
FORCEINLINE b32x8 B32x8(b32 a, b32 b, b32 c, b32 d, b32 e, b32 f, b32 g, b32 h) { return S32x8(a, b, c, d, e, f, g, h) != 0; }

// clang-format off
FORCEINLINE constexpr f32x4 F32x4(M128 m) { f32x4 r{}; r.m = m; return r; }
FORCEINLINE constexpr s32x4 S32x4(M128 m) { s32x4 r{}; r.m = m; return r; }
FORCEINLINE constexpr u32x4 U32x4(M128 m) { u32x4 r{}; r.m = m; return r; }

FORCEINLINE f32x4 F32x4(f32 v) { return F32x4(_mm_set1_ps(v)); }
FORCEINLINE s32x4 S32x4(s32 v) { return S32x4(_mm_set1_epi32(v)); }
FORCEINLINE u32x4 U32x4(u32 v) { return U32x4(_mm_set1_epi32((s32)v)); }

FORCEINLINE f32x4 F32x4(f32 i, f32 j, f32 k, f32 l) { return F32x4(_mm_setr_ps(i, j, k, l)); }
FORCEINLINE s32x4 S32x4(s32 i, s32 j, s32 k, s32 l) { return S32x4(_mm_setr_epi32(i, j, k, l)); }
FORCEINLINE u32x4 U32x4(u32 i, u32 j, u32 k, u32 l) { return U32x4(_mm_setr_epi32((s32)i, (s32)j, (s32)k, (s32)l)); }

FORCEINLINE f64x2 F64x2(f64 i, f64 j) { return F64x2(_mm_setr_pd(i, j)); }
#if ARCH_AVX
FORCEINLINE f64x4 F64x4(f64 i, f64 j, f64 k, f64 l) { return F64x4(_mm256_setr_pd(i, j, k, l)); }

FORCEINLINE f32x8 F32x8(f32 v) { return F32x8(_mm256_set1_ps(v)); }
FORCEINLINE s32x8 S32x8(s32 v) { return S32x8(_mm256_set1_epi32(v)); }
FORCEINLINE u32x8 U32x8(u32 v) { return U32x8(_mm256_set1_epi32((s32)v)); }

FORCEINLINE f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) { return F32x8(_mm256_setr_ps(a, b, c, d, e, f, g, h)); }
FORCEINLINE s32x8 S32x8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h) { return S32x8(_mm256_setr_epi32(a, b, c, d, e, f, g, h)); }
FORCEINLINE u32x8 U32x8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) { return U32x8(_mm256_setr_epi32((s32)a, (s32)b, (s32)c, (s32)d, (s32)e, (s32)f, (s32)g, (s32)h)); }

#else
FORCEINLINE f64x4 F64x4(f64 i, f64 j, f64 k, f64 l) { return F64x4(F64x2(i, j), F64x2(k, l)); }
FORCEINLINE f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) { return F32x8(F32x4(a, b, c, d), F32x4(e, f, g, h)); }
FORCEINLINE s32x8 S32x8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h) { return S32x8(S32x4(a, b, c, d), S32x4(e, f, g, h)); }
FORCEINLINE u32x8 U32x8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) { return U32x8(U32x4(a, b, c, d), U32x4(e, f, g, h)); }

FORCEINLINE f32x8 F32x8(f32 v) { return F32x8(F32x4(v), F32x4(v)); }
FORCEINLINE s32x8 S32x8(s32 v) { return S32x8(S32x4(v), S32x4(v)); }
FORCEINLINE u32x8 U32x8(u32 v) { return U32x8(U32x4(v), U32x4(v)); }
#endif

FORCEINLINE constexpr f32x8 F32x8(M256 m) { f32x8 r{}; r.m = m; return r; }
FORCEINLINE constexpr s32x8 S32x8(M256 m) { s32x8 r{}; r.m = m; return r; }
FORCEINLINE constexpr u32x8 U32x8(M256 m) { u32x8 r{}; r.m = m; return r; }

FORCEINLINE constexpr f32x8 F32x8(f32x4 m0, f32x4 m1) { f32x8 r{}; r._128[0] = m0; r._128[1] = m1; return r; }
FORCEINLINE constexpr s32x8 S32x8(s32x4 m0, s32x4 m1) { s32x8 r{}; r._128[0] = m0; r._128[1] = m1; return r; }
FORCEINLINE constexpr u32x8 U32x8(u32x4 m0, u32x4 m1) { u32x8 r{}; r._128[0] = m0; r._128[1] = m1; return r; }

FORCEINLINE f32x8 F32x8(f32 a, f32 b, f32 c, f32 d) { return F32x8(a, b, c, d, a, b, c, d); }
FORCEINLINE s32x8 S32x8(s32 a, s32 b, s32 c, s32 d) { return S32x8(a, b, c, d, a, b, c, d); }
FORCEINLINE u32x8 U32x8(u32 a, u32 b, u32 c, u32 d) { return U32x8(a, b, c, d, a, b, c, d); }

FORCEINLINE constexpr f32x16 F32x16(f32x8 m0, f32x8 m1) { f32x16 r{}; r._256[0] = m0; r._256[1] = m1; return r; }
FORCEINLINE constexpr s32x16 S32x16(s32x8 m0, s32x8 m1) { s32x16 r{}; r._256[0] = m0; r._256[1] = m1; return r; }
FORCEINLINE constexpr u32x16 U32x16(u32x8 m0, u32x8 m1) { u32x16 r{}; r._256[0] = m0; r._256[1] = m1; return r; }

FORCEINLINE f32x16 F32x16(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h,f32 i,f32 j,f32 k,f32 l,f32 m,f32 n,f32 o,f32 p) { return F32x16(F32x8(a, b, c, d, e, f, g, h), F32x8(i,j,k,l,m,n,o,p)); }
FORCEINLINE s32x16 S32x16(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h,s32 i,s32 j,s32 k,s32 l,s32 m,s32 n,s32 o,s32 p) { return S32x16(S32x8(a, b, c, d, e, f, g, h), S32x8(i,j,k,l,m,n,o,p)); }
FORCEINLINE u32x16 U32x16(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h,u32 i,u32 j,u32 k,u32 l,u32 m,u32 n,u32 o,u32 p) { return U32x16(U32x8(a, b, c, d, e, f, g, h), U32x8(i,j,k,l,m,n,o,p)); }

FORCEINLINE f32x16 F32x16(f32 v) { return F32x16(F32x8(v), F32x8(v)); }
FORCEINLINE s32x16 S32x16(s32 v) { return S32x16(S32x8(v), S32x8(v)); }
FORCEINLINE u32x16 U32x16(u32 v) { return U32x16(U32x8(v), U32x8(v)); }

template<> FORCEINLINE f32x<4>  F32x<4> (f32 val){ return F32x4(val); }
template<> FORCEINLINE s32x<4>  S32x<4> (s32 val){ return S32x4(val); }
template<> FORCEINLINE u32x<4>  U32x<4> (u32 val){ return U32x4(val); }
template<> FORCEINLINE f32x<8>  F32x<8> (f32 val){ return F32x8(val); }
template<> FORCEINLINE s32x<8>  S32x<8> (s32 val){ return S32x8(val); }
template<> FORCEINLINE u32x<8>  U32x<8> (u32 val){ return U32x8(val); }
template<> FORCEINLINE f32x<16> F32x<16>(f32 val){ return F32x16(val); }
template<> FORCEINLINE s32x<16> S32x<16>(s32 val){ return S32x16(val); }
template<> FORCEINLINE u32x<16> U32x<16>(u32 val){ return U32x16(val); }

FORCEINLINE f32x4::operator s32x4() const { return S32x4(_mm_cvtps_epi32(m)); }
FORCEINLINE f32x4::operator u32x4() const { return U32x4((u32)s[0], (u32)s[1], (u32)s[2], (u32)s[3]); }
FORCEINLINE s32x4::operator f32x4() const { return F32x4(_mm_cvtepi32_ps(m)); }
FORCEINLINE s32x4::operator u32x4() const { return *(u32x4*)this; }
FORCEINLINE u32x4::operator f32x4() const { return F32x4((f32)s[0], (f32)s[1], (f32)s[2], (f32)s[3]); }
FORCEINLINE u32x4::operator s32x4() const { return *(s32x4*)this; }
FORCEINLINE u32x4::operator f64x4() const { return F64x4((f64)s[0], (f64)s[1], (f64)s[2], (f64)s[3]); }
FORCEINLINE f64x4::operator u32x4() const { return U32x4((u32)s[0], (u32)s[1], (u32)s[2], (u32)s[3]); }

#if ARCH_AVX
FORCEINLINE f32x8::operator s32x8() const { return S32x8(_mm256_cvtps_epi32(m)); }
FORCEINLINE s32x8::operator f32x8() const { return F32x8(_mm256_cvtepi32_ps(m)); }
#else
FORCEINLINE f32x8::operator s32x8() const { return S32x8((s32x4)_128[0], (s32x4)_128[1]); }
FORCEINLINE s32x8::operator f32x8() const { return F32x8((f32x4)_128[0], (f32x4)_128[1]); }
#endif

FORCEINLINE f32x8::operator u32x8() const { return U32x8((u32)s[0], (u32)s[1], (u32)s[2], (u32)s[3], (u32)s[4], (u32)s[5], (u32)s[6], (u32)s[7]); }
FORCEINLINE s32x8::operator u32x8() const { return *(u32x8*)this; }
FORCEINLINE u32x8::operator f32x8() const { return F32x8((f32)s[0], (f32)s[1], (f32)s[2], (f32)s[3], (f32)s[4], (f32)s[5], (f32)s[6], (f32)s[7]); }
FORCEINLINE u32x8::operator s32x8() const { return *(s32x8*)this; }

FORCEINLINE void gather(f32x4& v, s32x4 offsets, void const* memory) { _gather32x4(&v, memory, &offsets); }
FORCEINLINE void gather(s32x4& v, s32x4 offsets, void const* memory) { _gather32x4(&v, memory, &offsets); }
FORCEINLINE void gather(u32x4& v, s32x4 offsets, void const* memory) { _gather32x4(&v, memory, &offsets); }

FORCEINLINE s32x4 makeGatherOffsets(s32 structSize, s32 memberOffset, s32 elementOffset = 0) {
	return (S32x4(0, 1, 2, 3) + elementOffset) * structSize + memberOffset;
}

#define GATHER_STRUCT_OFFSETS(struct, member, ...) makeGatherOffsets(sizeof(struct), offsetof(struct, member), __VA_ARGS__)

FORCEINLINE void gather(f32x8& v, s32x8 offsets, void const* memory) { _gather32x8(&v, memory, &offsets); }
FORCEINLINE void gather(s32x8& v, s32x8 offsets, void const* memory) { _gather32x8(&v, memory, &offsets); }
FORCEINLINE void gather(u32x8& v, s32x8 offsets, void const* memory) { _gather32x8(&v, memory, &offsets); }

FORCEINLINE void gather(f32x16& v, s32x16 offsets, void const* memory) { _gather32x16(&v, memory, &offsets); }
FORCEINLINE void gather(s32x16& v, s32x16 offsets, void const* memory) { _gather32x16(&v, memory, &offsets); }
FORCEINLINE void gather(u32x16& v, s32x16 offsets, void const* memory) { _gather32x16(&v, memory, &offsets); }

template<u8 s0, u8 s1, u8 s2, u8 s3> FORCEINLINE f32x4 shuffle(f32x4 v) { return v.ps = _mm_shuffle_ps(v.ps, v.ps, _MM_SHUFFLE(s3, s2, s1, s0)), v; }
template<u8 s0, u8 s1, u8 s2, u8 s3> FORCEINLINE s32x4 shuffle(s32x4 v) { return v.ps = _mm_shuffle_ps(v.ps, v.ps, _MM_SHUFFLE(s3, s2, s1, s0)), v; }
template<u8 s0, u8 s1, u8 s2, u8 s3> FORCEINLINE u32x4 shuffle(u32x4 v) { return v.ps = _mm_shuffle_ps(v.ps, v.ps, _MM_SHUFFLE(s3, s2, s1, s0)), v; }
FORCEINLINE f32x4 shuffle(f32x4 v, s32x4 s) { return _shuffle32x4(&v, &s), v; }
FORCEINLINE f32x8 shuffle(f32x8 v, s32x8 s) { return _shuffle32x8(&v, &s), v; }
template<u8 s0, u8 s1, u8 s2, u8 s3, u8 s4, u8 s5, u8 s6, u8 s7> FORCEINLINE f32x8 shuffle(f32x8 v) { return v.ps = _mm256_permutevar8x32_ps(v.ps, _mm256_setr_epi32(s0,s1,s2,s3,s4,s5,s6,s7)), v; }
template<u8 s0, u8 s1, u8 s2, u8 s3, u8 s4, u8 s5, u8 s6, u8 s7> FORCEINLINE s32x8 shuffle(s32x8 v) { return v.ps = _mm256_permutevar8x32_ps(v.ps, _mm256_setr_epi32(s0,s1,s2,s3,s4,s5,s6,s7)), v; }
template<u8 s0, u8 s1, u8 s2, u8 s3, u8 s4, u8 s5, u8 s6, u8 s7> FORCEINLINE u32x8 shuffle(u32x8 v) { return v.ps = _mm256_permutevar8x32_ps(v.ps, _mm256_setr_epi32(s0,s1,s2,s3,s4,s5,s6,s7)), v; }

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

#define VECCMP2(bool, v2) \
	FORCEINLINE bool operator==(v2 b) const { return x == b.x && y == b.y; }
#define VECCMP3(bool, v3) \
	FORCEINLINE bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; }
#define VECCMP4(bool, v4) \
	FORCEINLINE bool operator==(v4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }

#define MEMBERS2S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                                                    \
		f32x4 x, y;                                                                                             \
	};                                                                                                          \
	fn(__VA_ARGS__) VECCMP2(bool, v2x4) CVT(v2s); CVT(v2u);                                                          \
	f32 s[(width)*2]

#define MEMBERS3S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                                                    \
		f32x4 x, y, z;                                                                                          \
	};                                                                                                          \
	v2x4 xy;                                                                                                    \
	struct {                                                                                                    \
		f32x4 _pad0;                                                                                            \
		v2x4 yz;                                                                                                \
	};                                                                                                          \
	fn(__VA_ARGS__) VECCMP3(bool, v3x4) CVT(v3s); CVT(v3u);                                                          \
	f32 s[(width)*3]

#define MEMBERS4S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                                                    \
		f32x4 x, y, z, w;                                                                                       \
	};                                                                                                          \
	struct {                                                                                                    \
		v2x4 xy;                                                                                                \
		v2x4 zw;                                                                                                \
	};                                                                                                          \
	v3x4 xyz;                                                                                                   \
	struct {                                                                                                    \
		f32x4 _pad0;                                                                                            \
		union {                                                                                                 \
			v2x4 yz;                                                                                            \
			v3x4 yzw;                                                                                           \
		};                                                                                                      \
	};                                                                                                          \
	fn(__VA_ARGS__) VECCMP4(bool, v4x4) CVT(v4s); CVT(v4u);                                                          \
	f32 s[(width)*4]

#define MEMBERS2P(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)      \
	MEMBERS2S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	f32x4 p[2];                                                                                                      \
	v2 u[width];                                                                                                     \
	FORCEINLINE v2 operator[](umm i) const { return u[i]; }                                                                   \
	FORCEINLINE v2& operator[](umm i) { return u[i]; }
#define MEMBERS3P(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)      \
	MEMBERS3S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	f32x4 p[3];                                                                                                      \
	v3 u[width];                                                                                                     \
	FORCEINLINE v3 operator[](umm i) const { return u[i]; }                                                                   \
	FORCEINLINE v3& operator[](umm i) { return u[i]; }
#define MEMBERS4P(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)      \
	MEMBERS4S(width, bool, f32, v2, v3, v4, f32x4, v2x4, v3x4, v4x4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	f32x4 p[4];                                                                                                      \
	v4 u[width];                                                                                                     \
	FORCEINLINE v4 operator[](umm i) const { return u[i]; }                                                                   \
	FORCEINLINE v4& operator[](umm i) { return u[i]; }

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

#define TYPES_f32x1(fn, packSize, dim) fn(packSize,     bool, f32, v2f, v3f, v4f,   f32,   v2f,   v3f,   v4f,   v2s,   v3s,   v4s,   v2u,   v3u,   v4u, IMPL_v##dim##f32x1)
#define TYPES_s32x1(fn, packSize, dim) fn(packSize,     bool, s32, v2s, v3s, v4s,   s32,   v2s,   v3s,   v4s,   v2f,   v3f,   v4f,   v2u,   v3u,   v4u, IMPL_v##dim##s32x1)
#define TYPES_u32x1(fn, packSize, dim) fn(packSize,     bool, u32, v2u, v3u, v4u,   u32,   v2u,   v3u,   v4u,   v2f,   v3f,   v4f,   v2s,   v3s,   v4s, IMPL_v##dim##u32x1)
#define TYPES_f32x4(fn, packSize, dim) fn(packSize, b32x4, f32, v2f, v3f, v4f, f32x4, v2fx4, v3fx4, v4fx4, v2sx4, v3sx4, v4sx4, v2ux4, v3ux4, v4ux4, IMPL_v##dim##f32x4)
#define TYPES_s32x4(fn, packSize, dim) fn(packSize, b32x4, s32, v2s, v3s, v4s, s32x4, v2sx4, v3sx4, v4sx4, v2fx4, v3fx4, v4fx4, v2ux4, v3ux4, v4ux4, IMPL_v##dim##s32x4)
#define TYPES_u32x4(fn, packSize, dim) fn(packSize, b32x4, u32, v2u, v3u, v4u, u32x4, v2ux4, v3ux4, v4ux4, v2fx4, v3fx4, v4fx4, v2sx4, v3sx4, v4sx4, IMPL_v##dim##u32x4)
#define TYPES_f32x8(fn, packSize, dim) fn(packSize, b32x8, f32, v2f, v3f, v4f, f32x8, v2fx8, v3fx8, v4fx8, v2sx8, v3sx8, v4sx8, v2ux8, v3ux8, v4ux8, IMPL_v##dim##f32x8)
#define TYPES_s32x8(fn, packSize, dim) fn(packSize, b32x8, s32, v2s, v3s, v4s, s32x8, v2sx8, v3sx8, v4sx8, v2fx8, v3fx8, v4fx8, v2ux8, v3ux8, v4ux8, IMPL_v##dim##s32x8)
#define TYPES_u32x8(fn, packSize, dim) fn(packSize, b32x8, u32, v2u, v3u, v4u, u32x8, v2ux8, v3ux8, v4ux8, v2fx8, v3fx8, v4fx8, v2sx8, v3sx8, v4sx8, IMPL_v##dim##u32x8)

#define VECIMPL(dim, SorP, packSize, type) TYPES_##type##x##packSize(MEMBERS##dim##SorP, packSize, dim)


union v2f { VECIMPL(2, S, 1, f32); };
union v3f { VECIMPL(3, S, 1, f32); };
union v4f { VECIMPL(4, S, 1, f32); };

union v2s { VECIMPL(2, S, 1, s32); };
union v3s { VECIMPL(3, S, 1, s32); };
#if 1
union v4s { VECIMPL(4, S, 1, s32); };
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
union v2u { VECIMPL(2, S, 1, u32); };
union v3u { VECIMPL(3, S, 1, u32); };
union v4u { VECIMPL(4, S, 1, u32); };

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
	inline v3f operator*(v3f b) const {
		v3f x = V3f(b.x) * i;
		v3f y = V3f(b.y) * j;
		v3f z = V3f(b.z) * k;
		return x + y + z;
	}
	inline m3 operator*(m3 b) const { return {*this * b.i, *this * b.j, *this * b.k}; }
	inline m3& operator*=(m3 b) { return *this = *this * b; }
	static inline m3 scaling(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0,
			0, y, 0,
			0, 0, z,
		};
		// clang-format on
	}
	static inline m3 scaling(v3f v) { return scaling(v.x, v.y, v.z); }
	static inline m3 scaling(v2f xy, f32 z) { return scaling(xy.x, xy.y, z); }
	static inline m3 scaling(f32 v) { return scaling(v, v, v); }
	static inline m3 translation(f32 x, f32 y) {
		// clang-format off
		return {
			1, 0, 0,
			0, 1, 0,
			x, y, 1,
		};
		// clang-format on
	}
	static inline m3 translation(v2f v) { return translation(v.x, v.y); }
	static inline m3 translation(f32 v) { return translation(v, v); }
	static inline m3 rotationX(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			1, 0, 0,
			0, c, s,
			0,-s, c,
		};
		// clang-format on
	}
	static inline m3 rotationY(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, 0, s,
			0, 1, 0,
		   -s, 0, c,
		};
		// clang-format on
	}
	static inline m3 rotationZ(f32 a) {
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

inline m3 transpose(m3 const& m) {
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
	inline v4f operator*(v4f b) const {
		v4f x = V4f(b.x) * i;
		v4f y = V4f(b.y) * j;
		v4f z = V4f(b.z) * k;
		v4f w = V4f(b.w) * l;
		return x + y + z + w;
	}
	inline v3f operator*(v3f b) const {
		auto x = V4f(b.x) * i;
		auto y = V4f(b.y) * j;
		auto z = V4f(b.z) * k;
		auto r = x + y + z;
		v3f result;
		memcpy(&result, &r, 12);
		return result;
	}
	inline m4 operator*(m4 b) const { return {*this * b.i, *this * b.j, *this * b.k, *this * b.l}; }
	inline m4& operator*=(m4 b) { return *this = *this * b; }
	static inline m4 scaling(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static inline m4 identity() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }
	static inline m4 scaling(v3f v) { return scaling(v.x, v.y, v.z); }
	static inline m4 scaling(v2f xy, f32 z) { return scaling(xy.x, xy.y, z); }
	static inline m4 scaling(f32 v) { return scaling(v, v, v); }
	static inline m4 translation(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1,
		};
		// clang-format on
	}
	static inline m4 translation(v3f v) { return translation(v.x, v.y, v.z); }
	static inline m4 translation(v2f xy, f32 z) { return translation(xy.x, xy.y, z); }
	static inline m4 translation(f32 v) { return translation(v, v, v); }
	static inline m4 perspective(f32 aspect, f32 fov, f32 nz, f32 fz) {
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
	static inline m4 rotationX(f32 a) {
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
	static inline m4 rotationY(f32 a) {
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
	static inline m4 rotationZ(f32 a) {
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
	static inline m4 rotationZXY(v3f v) {
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
	static inline m4 rotationYXZ(v3f v) {
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
	FORCEINLINE constexpr v3fx4 V3fx4(f32x4 x, f32x4 y, f32x4 z) { return {x, y, z}; }                              \
	FORCEINLINE v3fx4 V3fx4(f32 v) { return {F32x4(v), F32x4(v), F32x4(v)}; }                                       \
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
	FORCEINLINE constexpr v4fx4 V4fx4(f32x4 x, f32x4 y, f32x4 z, f32x4 w) { return {x, y, z, w}; }                  \
	FORCEINLINE v4fx4 V4fx4(f32 v) { return {F32x4(v), F32x4(v), F32x4(v), F32x4(v)}; }                             \
	FORCEINLINE constexpr v4fx4 V4fx4(f32x4 v) { return {v, v, v, v}; }                                             \
	FORCEINLINE v4fx4 V4fx4(v4f v) { return {F32x4(v.x), F32x4(v.y), F32x4(v.z), F32x4(v.w)}; }

V4x4(v4fx4, V4fx4, v4f, f32, f32x4, F32x4);
V4x4(v4sx4, V4sx4, v4s, s32, s32x4, S32x4);
V4x4(v4ux4, V4ux4, v4u, u32, u32x4, U32x4);
#undef V4x4

#define V2x8(v2fx8, V2fx8, v2f, f32, f32x8, F32x8)                                                                  \
	FORCEINLINE constexpr v2fx8 V2fx8(f32x8 x, f32x8 y) { return {x, y}; }                                          \
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
	FORCEINLINE constexpr v3fx8 V3fx8(f32x8 x, f32x8 y, f32x8 z) { return {x, y, z}; }                              \
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
	FORCEINLINE v3fx8 V3fx8(f32 v) { return V3fx8(F32x8(v), F32x8(v), F32x8(v)); }                                  \
	FORCEINLINE constexpr v3fx8 V3fx8(f32x8 v) { return V3fx8(v, v, v); }                                           \
	FORCEINLINE v3fx8 V3fx8(v3f v) { return V3fx8(F32x8(v.x), F32x8(v.y), F32x8(v.z)); }
V3x8(v3fx8, V3fx8, v3f, f32, f32x8, F32x8, v3fx4);
V3x8(v3sx8, V3sx8, v3s, s32, s32x8, S32x8, v3sx4);
V3x8(v3ux8, V3ux8, v3u, u32, u32x8, U32x8, v3ux4);
#undef V3x8

#define V4x8(v4fx8, V4fx8, v4f, f32, f32x8, F32x8)                                                                  \
	FORCEINLINE constexpr v4fx8 V4fx8(f32x8 x, f32x8 y, f32x8 z, f32x8 w) { return {x, y, z, w}; }                  \
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
	FORCEINLINE v4fx8 V4fx8(f32 v) { return V4fx8(F32x8(v), F32x8(v), F32x8(v), F32x8(v)); }                        \
	FORCEINLINE constexpr v4fx8 V4fx8(f32x8 v) { return V4fx8(v, v, v, v); }                                        \
	FORCEINLINE v4fx8 V4fx8(v4f v) { return V4fx8(F32x8(v.x), F32x8(v.y), F32x8(v.z), F32x8(v.w)); }

V4x8(v4fx8, V4fx8, v4f, f32, f32x8, F32x8);
V4x8(v4sx8, V4sx8, v4s, s32, s32x8, S32x8);
V4x8(v4ux8, V4ux8, v4u, u32, u32x8, U32x8);
#undef V4x8

// clang-format off
#define CVT(v2f, v2s, V2s, s32, v2u, V2u, u32)                \
	inline v2f::operator v2s() const { return V2s((s32)x, (s32)y); } \
	inline v2f::operator v2u() const { return V2u((u32)x, (u32)y); }
CVT(v2f, v2s, V2s, s32, v2u, V2u, u32) CVT(v2fx4, v2sx4, V2sx4, s32x4, v2ux4, V2ux4, u32x4) CVT(v2fx8, v2sx8, V2sx8, s32x8, v2ux8, V2ux8, u32x8);
CVT(v2s, v2f, V2f, f32, v2u, V2u, u32) CVT(v2sx4, v2fx4, V2fx4, f32x4, v2ux4, V2ux4, u32x4) CVT(v2sx8, v2fx8, V2fx8, f32x8, v2ux8, V2ux8, u32x8);
CVT(v2u, v2f, V2f, f32, v2s, V2s, s32) CVT(v2ux4, v2fx4, V2fx4, f32x4, v2sx4, V2sx4, s32x4) CVT(v2ux8, v2fx8, V2fx8, f32x8, v2sx8, V2sx8, s32x8);
#undef CVT

#define CVT(v3f, v3s, V3s, s32, v3u, V3u, u32)                        \
	inline v3f::operator v3s() const { return V3s((s32)x, (s32)y, (s32)z); } \
	inline v3f::operator v3u() const { return V3u((u32)x, (u32)y, (u32)z); }
CVT(v3f, v3s, V3s, s32, v3u, V3u, u32) CVT(v3fx4, v3sx4, V3sx4, s32x4, v3ux4, V3ux4, u32x4) CVT(v3fx8, v3sx8, V3sx8, s32x8, v3ux8, V3ux8, u32x8);
CVT(v3s, v3f, V3f, f32, v3u, V3u, u32) CVT(v3sx4, v3fx4, V3fx4, f32x4, v3ux4, V3ux4, u32x4) CVT(v3sx8, v3fx8, V3fx8, f32x8, v3ux8, V3ux8, u32x8);
CVT(v3u, v3f, V3f, f32, v3s, V3s, s32) CVT(v3ux4, v3fx4, V3fx4, f32x4, v3sx4, V3sx4, s32x4) CVT(v3ux8, v3fx8, V3fx8, f32x8, v3sx8, V3sx8, s32x8);
#undef CVT

#define CVT(v4f, v4s, V4s, s32, v4u, V4u, u32)                                \
	inline v4f::operator v4s() const { return V4s((s32)x, (s32)y, (s32)z, (s32)w); } \
	inline v4f::operator v4u() const { return V4u((u32)x, (u32)y, (u32)z, (u32)w); }
CVT(v4f, v4s, V4s, s32, v4u, V4u, u32) CVT(v4fx4, v4sx4, V4sx4, s32x4, v4ux4, V4ux4, u32x4) CVT(v4fx8, v4sx8, V4sx8, s32x8, v4ux8, V4ux8, u32x8);
CVT(v4s, v4f, V4f, f32, v4u, V4u, u32) CVT(v4sx4, v4fx4, V4fx4, f32x4, v4ux4, V4ux4, u32x4) CVT(v4sx8, v4fx8, V4fx8, f32x8, v4ux8, V4ux8, u32x8);
CVT(v4u, v4f, V4f, f32, v4s, V4s, s32) CVT(v4ux4, v4fx4, V4fx4, f32x4, v4sx4, V4sx4, s32x4) CVT(v4ux8, v4fx8, V4fx8, f32x8, v4sx8, V4sx8, s32x8);
#undef CVT
// clang-format on

inline constexpr m4 M4(f32 v = 0.0f) { return m4{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; }
inline constexpr m4 M4(v4f i, v4f j, v4f k, v4f l) { return m4{i, j, k, l}; }
inline constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l) {
	m4 r{};
	r.m[0] = i;
	r.m[1] = j;
	r.m[2] = k;
	r.m[3] = l;
	return r;
}
inline constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw) {
	return {ix, iy, iz, iw, jx, jy, jz, jw, kx, ky, kz, kw, lx, ly, lz, lw};
}

inline m4 M4(m3 v) {
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
inline constexpr f32x4 F32x4(f32 a, f32 b, f32 c, f32 d) {	 \
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
inline constexpr f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {	\
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
	inline constexpr v2fx4 V2fx4(f32x4 x, f32x4 y) { v2fx4 r{}; r.x = x; r.y = y; return r; }														\
	inline constexpr v2fx4 V2fx4(v2f a, v2f b, v2f c, v2f d) { return CE::V2fx4(CE::F32x4(a.x, b.x, c.x, d.x), CE::F32x4(a.y, b.y, c.y, d.y)); }	\
	inline constexpr v2fx4 V2fx4(v2f v) { return CE::V2fx4(v, v, v, v); }
V2x4(v2f, v2fx4, V2fx4, f32x4, F32x4)
V2x4(v2s, v2sx4, V2sx4, s32x4, S32x4)
V2x4(v2u, v2ux4, V2ux4, u32x4, U32x4)
#undef V2x4

#define V3x4(v3f, v3fx4, V3fx4, f32x4, F32x4)																																\
	inline constexpr v3fx4 V3fx4(f32x4 x, f32x4 y, f32x4 z) { v3fx4 r{}; r.x = x; r.y = y; r.z = z; return r; }																	\
	inline constexpr v3fx4 V3fx4(v3f a, v3f b, v3f c, v3f d) {  return CE::V3fx4(CE::F32x4(a.x, b.x, c.x, d.x), CE::F32x4(a.y, b.y, c.y, d.y), CE::F32x4(a.z, b.z, c.z, d.z)); }	\
	inline constexpr v3fx4 V3fx4(v3f v) { return CE::V3fx4(v, v, v, v); }
V3x4(v3f, v3fx4, V3fx4, f32x4, F32x4)
V3x4(v3s, v3sx4, V3sx4, s32x4, S32x4)
V3x4(v3u, v3ux4, V3ux4, u32x4, U32x4)
#undef V3x4

#define V4x4(v4f, v4fx4, V4fx4, f32x4, F32x4)																																							\
	inline constexpr v4fx4 V4fx4(f32x4 x, f32x4 y, f32x4 z, f32x4 w) { v4fx4 r{}; r.x = x; r.y = y; r.z = z; r.w = w; return r; }																				\
	inline constexpr v4fx4 V4fx4(v4f a, v4f b, v4f c, v4f d) { return CE::V4fx4(CE::F32x4(a.x, b.x, c.x, d.x), CE::F32x4(a.y, b.y, c.y, d.y), CE::F32x4(a.z, b.z, c.z, d.z), CE::F32x4(a.w, b.w, c.w, d.w)); }	\
	inline constexpr v4fx4 V4fx4(v4f v) { return CE::V4fx4(v, v, v, v); }
V4x4(v4f, v4fx4, V4fx4, f32x4, F32x4)
V4x4(v4s, v4sx4, V4sx4, s32x4, S32x4)
V4x4(v4u, v4ux4, V4ux4, u32x4, U32x4)
#undef V4x4

#define V2x8(v2f, v2fx8, V2fx8, f32x8, F32x8)																								\
	inline constexpr v2fx8 V2fx8(f32x8 x, f32x8 y) { v2fx8 r{}; r.x = x; r.y = y; return r; }														\
	inline constexpr v2fx8 V2fx8(v2f a, v2f b, v2f c, v2f d, v2f e, v2f f, v2f g, v2f h) { return CE::V2fx8(CE::F32x8(a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x), CE::F32x8(a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y)); }	\
	inline constexpr v2fx8 V2fx8(v2f v) { return CE::V2fx8(v, v, v, v, v, v, v, v); }
V2x8(v2f, v2fx8, V2fx8, f32x8, F32x8)
V2x8(v2s, v2sx8, V2sx8, s32x8, S32x8)
V2x8(v2u, v2ux8, V2ux8, u32x8, U32x8)
#undef V2x8

#define V3x8(v3f, v3fx8, V3fx8, f32x8, F32x8)																																\
	inline constexpr v3fx8 V3fx8(f32x8 x, f32x8 y, f32x8 z) { v3fx8 r{}; r.x = x; r.y = y; r.z = z; return r; }																	\
	inline constexpr v3fx8 V3fx8(v3f a, v3f b, v3f c, v3f d, v3f e, v3f f, v3f g, v3f h) {  return CE::V3fx8(CE::F32x8(a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x), CE::F32x8(a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y), CE::F32x8(a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z)); }	\
	inline constexpr v3fx8 V3fx8(v3f v) { return CE::V3fx8(v, v, v, v, v, v, v, v); }
V3x8(v3f, v3fx8, V3fx8, f32x8, F32x8)
V3x8(v3s, v3sx8, V3sx8, s32x8, S32x8)
V3x8(v3u, v3ux8, V3ux8, u32x8, U32x8)
#undef V3x8

#define V4x8(v4f, v4fx8, V4fx8, f32x8, F32x8)																																							\
	inline constexpr v4fx8 V4fx8(f32x8 x, f32x8 y, f32x8 z, f32x8 w) { v4fx8 r{}; r.x = x; r.y = y; r.z = z; r.w = w; return r; }																				\
	inline constexpr v4fx8 V4fx8(v4f a, v4f b, v4f c, v4f d, v4f e, v4f f, v4f g, v4f h) { return CE::V4fx8(CE::F32x8(a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x), CE::F32x8(a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y), CE::F32x8(a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z), CE::F32x8(a.w, b.w, c.w, d.w, e.w, f.w, g.w, h.w)); }	\
	inline constexpr v4fx8 V4fx8(v4f v) { return CE::V4fx8(v, v, v, v, v, v, v, v); }
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

#define CVT(u32x4, f32x4, f32, F32x4) template<> inline constexpr f32x4 cvt(u32x4 v) {  return CE::F32x4((f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3]);  }
CVT(f32x4, s32x4, s32, S32x4) CVT(f32x4, u32x4, u32, U32x4);
CVT(s32x4, f32x4, f32, F32x4) CVT(s32x4, u32x4, u32, U32x4);
CVT(u32x4, f32x4, f32, F32x4) CVT(u32x4, s32x4, s32, S32x4);
#undef CVT

#define CVT(u32x8, f32x8, f32) template<> inline constexpr f32x8 cvt(u32x8 v) {  return {(f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3], (f32)v[4], (f32)v[5], (f32)v[6], (f32)v[7]};  }
CVT(f32x8, s32x8, s32) CVT(f32x8, u32x8, u32);
CVT(s32x8, f32x8, f32) CVT(s32x8, u32x8, u32);
CVT(u32x8, f32x8, f32) CVT(u32x8, s32x8, s32);
#undef CVT

#define CVT(v2f, v2s, s32) template<> inline constexpr v2s cvt(v2f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y)}; }
CVT(v2f, v2s, s32) CVT(v2f, v2u, u32) CVT(v2fx4, v2sx4, s32x4) CVT(v2fx4, v2ux4, u32x4) CVT(v2fx8, v2sx8, s32x8) CVT(v2fx8, v2ux8, u32x8);
CVT(v2s, v2f, f32) CVT(v2s, v2u, u32) CVT(v2sx4, v2fx4, f32x4) CVT(v2sx4, v2ux4, u32x4) CVT(v2sx8, v2fx8, f32x8) CVT(v2sx8, v2ux8, u32x8);
CVT(v2u, v2f, f32) CVT(v2u, v2s, s32) CVT(v2ux4, v2fx4, f32x4) CVT(v2ux4, v2sx4, s32x4) CVT(v2ux8, v2fx8, f32x8) CVT(v2ux8, v2sx8, s32x8);
#undef CVT

#define CVT(v3f, v3s, s32) template<> inline constexpr v3s cvt(v3f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z)}; }
CVT(v3f, v3s, s32) CVT(v3f, v3u, u32) CVT(v3fx4, v3sx4, s32x4) CVT(v3fx4, v3ux4, u32x4) CVT(v3fx8, v3sx8, s32x8) CVT(v3fx8, v3ux8, u32x8);
CVT(v3s, v3f, f32) CVT(v3s, v3u, u32) CVT(v3sx4, v3fx4, f32x4) CVT(v3sx8, v3fx8, f32x8) CVT(v3sx4, v3ux4, u32x4) CVT(v3sx8, v3ux8, u32x8);
CVT(v3u, v3f, f32) CVT(v3u, v3s, s32) CVT(v3ux4, v3fx4, f32x4) CVT(v3ux8, v3fx8, f32x8) CVT(v3ux4, v3sx4, s32x4) CVT(v3ux8, v3sx8, s32x8);
#undef CVT

#define CVT(v4f, v4s, s32) template<> inline constexpr v4s cvt(v4f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z), CE::cvt<s32>(v.w)}; }
CVT(v4f, v4s, s32) CVT(v4f, v4u, u32) CVT(v4fx4, v4sx4, s32x4) CVT(v4fx4, v4ux4, u32x4) CVT(v4fx8, v4sx8, s32x8) CVT(v4fx8, v4ux8, u32x8);
CVT(v4s, v4f, f32) CVT(v4s, v4u, u32) CVT(v4sx4, v4fx4, f32x4) CVT(v4sx4, v4ux4, u32x4)	CVT(v4sx8, v4fx8, f32x8) CVT(v4sx8, v4ux8, u32x8);
CVT(v4u, v4f, f32) CVT(v4u, v4s, s32) CVT(v4ux4, v4fx4, f32x4) CVT(v4ux4, v4sx4, s32x4)	CVT(v4ux8, v4fx8, f32x8) CVT(v4ux8, v4sx8, s32x8);
#undef CVT
} // namespace CE
// clang-format on

#define SHUFFLE(a, s0, s1, b, s2, s3) F32x4(_mm_shuffle_ps(a.m, b.m, _MM_SHUFFLE(s3, s2, s1, s0)))

// clang-format off
inline v2fx4 pack(v2fx4 v) { 
#if ARCH_AVX2
    *(__m256*)&v = _mm256_permutevar8x32_ps(*(__m256*)&v, _mm256_setr_epi32(0,2,4,6,1,3,5,7));
	return v;
#else
	v2fx4 r;
    r.s[0] = v.s[0]; 
    r.s[1] = v.s[2];	 
    r.s[2] = v.s[4]; 
    r.s[3] = v.s[6];	
    r.s[4] = v.s[1]; 
    r.s[5] = v.s[3];
    r.s[6] = v.s[5]; 
    r.s[7] = v.s[7];
	return r;
#endif
}
inline v3fx4 pack(v3fx4 v) { 
	v3fx4 r;
#if 0
	gather(*(f32x8*)&r, S32x8(0,3,6,9,1,4,7,10)*sizeof(f32), &v);
	gather(r.z, S32x4(2,5,8,11)*sizeof(f32), &v);
#else
    r.s[0]  = v.s[0]; 
    r.s[1]  = v.s[3];	 
    r.s[2]  = v.s[6];
    r.s[3]  = v.s[9]; 
    r.s[4]  = v.s[1];	
    r.s[5]  = v.s[4];
    r.s[6]  = v.s[7]; 
    r.s[7]  = v.s[10];
    r.s[8]  = v.s[2];
    r.s[9]  = v.s[5]; 
    r.s[10] = v.s[8];
    r.s[11] = v.s[11];
#endif
	return r;
}
inline v4fx4 pack(v4fx4 v) { 
	v4fx4 r;
#if ARCH_AVX2
	// msvc can't do this
	using namespace ASM;
	
	YMM &v0 = YMMWORD_PTR(v);
	YMM &v1 = YMMWORD_PTR(v) + 32;
	YMM &r0 = YMMWORD_PTR(r);
	YMM &r1 = YMMWORD_PTR(r) + 32;
	YMM idx = _mm256_setr_epi32(0,4,1,5,2,6,3,7);

    vunpcklps	(r1, v0, v1);
    vunpckhps	(r0, v0, v1);
    vpermd		(v1, idx, r1);
    vpermd		(v0, idx, r0);
    vmovaps		(r0, v1);
    vmovaps		(r1, v0);

#else
    r.s[0]  = v.s[0]; 
    r.s[1]  = v.s[4];	 
    r.s[2]  = v.s[8];
    r.s[3]  = v.s[12];
    r.s[4]  = v.s[1]; 
    r.s[5]  = v.s[5];	
    r.s[6]  = v.s[9];
    r.s[7]  = v.s[13];
    r.s[8]  = v.s[2]; 
    r.s[9]  = v.s[6];
    r.s[10] = v.s[10];
    r.s[11] = v.s[14];
    r.s[12] = v.s[3]; 
    r.s[13] = v.s[7];
    r.s[14] = v.s[11];
    r.s[15] = v.s[15];
#endif
	return r;
}
inline v2fx8 pack(v2fx8 v) {
	v2fx8 r;
	gather(*(f32x16*)&r, S32x16(0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15)*sizeof(f32), &v);
	return r;
}
inline v3fx8 pack(v3fx8 v) { 
	v3fx8 r;
	gather(*(f32x16*)&r, S32x16(0,3,6, 9,12,15,18,21,1,4,7,10,13,16,19,22)*sizeof(f32), &v);
	gather(r.z, S32x8(2,5,8,11,14,17,20,23)*sizeof(f32), &v);
	return r;
}
inline v4fx8 pack(v4fx8 v) { 
	v4fx8 r;
	gather(*(f32x16*)&r.x, S32x16(0,4, 8,12,16,20,24,28,1,5, 9,13,17,21,25,29)*sizeof(f32), &v);
	gather(*(f32x16*)&r.z, S32x16(2,6,10,14,18,22,26,30,3,7,11,15,19,23,27,31)*sizeof(f32), &v);
	return r;
}
inline v2fx4 unpack(v2fx4 v) { 
	auto x = F32x4(_mm_unpacklo_ps(v.x.m, v.y.m));
	auto y = F32x4(_mm_unpackhi_ps(v.x.m, v.y.m));
	return V2fx4(x, y);
}
inline v3fx4 unpack(v3fx4 v) { 
#if 0
	auto xy = SHUFFLE(v.x, 0, 2, v.y, 0, 2);
	auto yz = SHUFFLE(v.y, 1, 3, v.z, 1, 3);
	auto xz = SHUFFLE(v.x, 1, 3, v.z, 0, 2);
	v.x = SHUFFLE(xy, 0, 2, xz, 2, 0);
	v.y = SHUFFLE(yz, 0, 2, xy, 1, 3);
	v.z = SHUFFLE(xz, 3, 1, yz, 1, 3);
	return v;
#else
	v3fx4 r;
    r.s[0]  = v.s[0]; 
    r.s[1]  = v.s[4];	 
    r.s[2]  = v.s[8];
    r.s[3]  = v.s[1]; 
    r.s[4]  = v.s[5];	
    r.s[5]  = v.s[9];
    r.s[6]  = v.s[2]; 
    r.s[7]  = v.s[6];
    r.s[8]  = v.s[10];
    r.s[9]  = v.s[3]; 
    r.s[10] = v.s[7];
    r.s[11] = v.s[11];
	return r;
#endif
}
inline v4fx4 unpack(v4fx4 v) {
	return pack(v);
}
inline v2fx8 unpack(v2fx8 v) { 
#if ARCH_AVX
	auto m02 = F32x8(_mm256_unpacklo_ps(v.x.m, v.y.m));
	auto m13 = F32x8(_mm256_unpackhi_ps(v.x.m, v.y.m));
#else
	auto m02 = F32x8(F32x4(_mm_unpacklo_ps(v.x._128[0].m, v.y._128[0].m)), F32x4(_mm_unpacklo_ps(v.x._128[1].m, v.y._128[1].m)));
	auto m13 = F32x8(F32x4(_mm_unpackhi_ps(v.x._128[0].m, v.y._128[0].m)), F32x4(_mm_unpackhi_ps(v.x._128[1].m, v.y._128[1].m)));
#endif
#if COMPILER_GCC || !ARCH_AVX
	v.x._128[0] = m02._128[0];
	v.x._128[1] = m13._128[0];
	v.y._128[0] = m02._128[1];
	v.y._128[1] = m13._128[1];
#else
	_mm256_storeu2_m128(v.y.data(), v.x.data(), m02.m);
	_mm256_storeu2_m128(v.y.data() + 4, v.x.data() + 4, m13.m);
#endif
	return v;
}
inline v3fx8 unpack(v3fx8 v) { 
	v3fx4 &a = ((v3fx4*)&v)[0];
	v3fx4 &b = ((v3fx4*)&v)[1];
	std::swap(a.y, a.z);
	std::swap(a.z, b.y);
	std::swap(b.x, b.y);
	a = unpack(a);
	b = unpack(b);
	return v;
}
inline v4fx8 unpack(v4fx8 v) {
	return {
		v.s[0], v.s[8],	 v.s[16], v.s[24],
		v.s[1], v.s[9],	 v.s[17], v.s[25],
		v.s[2], v.s[10], v.s[18], v.s[26],
		v.s[3], v.s[11], v.s[19], v.s[27],
		v.s[4], v.s[12], v.s[20], v.s[28],
		v.s[5],	v.s[13], v.s[21], v.s[29],
		v.s[6],	v.s[14], v.s[22], v.s[30],
		v.s[7],	v.s[15], v.s[23], v.s[31],
	}; 
}
// clang-format on

inline f32 min(f32x8 v) { return min(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }

// clang-format off
inline f32x4 min(f32x4 a, f32x4 b) { return F32x4(_mm_min_ps(a.m, b.m)); }
inline s32x4 min(s32x4 a, s32x4 b) { return S32x4(_mm_min_epi32(a.m, b.m)); }
inline u32x4 min(u32x4 a, u32x4 b) { return U32x4(_mm_min_epu32(a.m, b.m)); }
#if ARCH_AVX
inline f32x8 min(f32x8 a, f32x8 b) { return F32x8(_mm256_min_ps(a.m, b.m)); }
inline s32x8 min(s32x8 a, s32x8 b) { return S32x8(_mm256_min_epi32(a.m, b.m)); }
inline u32x8 min(u32x8 a, u32x8 b) { return U32x8(_mm256_min_epu32(a.m, b.m)); }
#else
inline f32x8 min(f32x8 a, f32x8 b) { return F32x8(min(a._128[0], b._128[0]), min(a._128[1], b._128[1])); }
inline s32x8 min(s32x8 a, s32x8 b) { return S32x8(min(a._128[0], b._128[0]), min(a._128[1], b._128[1])); }
inline u32x8 min(u32x8 a, u32x8 b) { return U32x8(min(a._128[0], b._128[0]), min(a._128[1], b._128[1])); }
#endif
inline f32 min(f32 a, f32x8 b) { return min(a, min(b)); } 

inline f32x4 max(f32x4 a, f32x4 b) { return F32x4(_mm_max_ps(a.m, b.m)); }
inline s32x4 max(s32x4 a, s32x4 b) { return S32x4(_mm_max_epi32(a.m, b.m)); }
inline u32x4 max(u32x4 a, u32x4 b) { return U32x4(_mm_max_epu32(a.m, b.m)); }
#if ARCH_AVX
inline f32x8 max(f32x8 a, f32x8 b) { return F32x8(_mm256_max_ps(a.m, b.m)); }
inline s32x8 max(s32x8 a, s32x8 b) { return S32x8(_mm256_max_epi32(a.m, b.m)); }
inline u32x8 max(u32x8 a, u32x8 b) { return U32x8(_mm256_max_epu32(a.m, b.m)); }
#else
inline f32x8 max(f32x8 a, f32x8 b) { return F32x8(max(a._128[0], b._128[0]), max(a._128[1], b._128[1])); }
inline s32x8 max(s32x8 a, s32x8 b) { return S32x8(max(a._128[0], b._128[0]), max(a._128[1], b._128[1])); }
inline u32x8 max(u32x8 a, u32x8 b) { return U32x8(max(a._128[0], b._128[0]), max(a._128[1], b._128[1])); }
#endif

#define MINMAX(v2f, V2f)						  \
	inline auto min(v2f a, v2f b) { return V2f(min(a.x, b.x), min(a.y, b.y)); }\
	inline auto max(v2f a, v2f b) { return V2f(max(a.x, b.x), max(a.y, b.y)); }\
	inline void minmax(v2f a, v2f b, v2f& mn, v2f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
	}
MINMAX(v2f  , V2f  ) MINMAX(v2s  , V2s  ) MINMAX(v2u  , V2u  );
MINMAX(v2fx4, V2fx4) MINMAX(v2sx4, V2sx4) MINMAX(v2ux4, V2ux4);
MINMAX(v2fx8, V2fx8) MINMAX(v2sx8, V2sx8) MINMAX(v2ux8, V2ux8);
#undef MINMAX

#define MINMAX(v3f, V3f)						  \
	inline auto min(v3f a, v3f b) { return V3f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }\
	inline auto max(v3f a, v3f b) { return V3f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }\
	inline void minmax(v3f a, v3f b, v3f& mn, v3f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
	}
MINMAX(v3f  , V3f  ) MINMAX(v3s  , V3s  ) MINMAX(v3u  , V3u  );
MINMAX(v3fx4, V3fx4) MINMAX(v3sx4, V3sx4) MINMAX(v3ux4, V3ux4);
MINMAX(v3fx8, V3fx8) MINMAX(v3sx8, V3sx8) MINMAX(v3ux8, V3ux8);
#undef MINMAX

#define MINMAX(v4f, V4f)						  \
	inline auto min(v4f a, v4f b) { return V4f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w)); }\
	inline auto max(v4f a, v4f b) { return V4f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w)); }\
	inline void minmax(v4f a, v4f b, v4f& mn, v4f& mx) { \
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

#define IN_BOUNDS(bool, v2f) \
	inline bool inBounds(v2f v, v2f min, v2f max) { return (min.x < v.x) && (v.x < max.x) && (min.y < v.y) && (v.y < max.y); }
IN_BOUNDS(bool, v2f) IN_BOUNDS(b32x4, v2fx4) IN_BOUNDS(b32x8, v2fx8);
IN_BOUNDS(bool, v2s) IN_BOUNDS(b32x4, v2sx4) IN_BOUNDS(b32x8, v2sx8);
IN_BOUNDS(bool, v2u) IN_BOUNDS(b32x4, v2ux4) IN_BOUNDS(b32x8, v2ux8);
#undef IN_BOUNDS

inline constexpr bool _isTrueMask(f32 v) { return *(u32*)&v == ~0u; }

inline constexpr f32 select(bool mask, f32 a, f32 b) { return mask ? a : b; }
inline f32x4 select(b32x4 mask, f32x4 a, f32x4 b) { return F32x4(select(mask.m, a.m, b.m)); }
inline s32x4 select(b32x4 mask, s32x4 a, s32x4 b) { return S32x4(select(mask.m, a.m, b.m)); }
inline v2fx4 select(b32x4 mask, v2fx4 a, v2fx4 b) { return V2fx4(select(mask, a.x, b.x), select(mask, a.y, b.y)); }
inline v3fx4 select(b32x4 mask, v3fx4 a, v3fx4 b) {
	return V3fx4(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z));
}
inline v4fx4 select(b32x4 mask, v4fx4 a, v4fx4 b) {
	return V4fx4(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z), select(mask, a.w, b.w));
}
inline f32x8 select(b32x8 mask, f32x8 a, f32x8 b) { return F32x8(select(mask.m, a.m, b.m)); }
inline s32x8 select(b32x8 mask, s32x8 a, s32x8 b) { return S32x8(select(mask.m, a.m, b.m)); }
inline v2fx8 select(b32x8 mask, v2fx8 a, v2fx8 b) { return V2fx8(select(mask, a.x, b.x), select(mask, a.y, b.y)); }
inline v3fx8 select(b32x8 mask, v3fx8 a, v3fx8 b) {
	return V3fx8(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z));
}
inline v4fx8 select(b32x8 mask, v4fx8 a, v4fx8 b) {
	return V4fx8(select(mask, a.x, b.x), select(mask, a.y, b.y), select(mask, a.z, b.z), select(mask, a.w, b.w));
}

inline f32 sqrt(f32 v) { return sqrtf(v); }
inline v2f sqrt(v2f v) { return V2f(sqrtf(v.x), sqrtf(v.y)); }
inline v3f sqrt(v3f v) { return V3f(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z)); }
inline f32x4 sqrt(f32x4 v) { return F32x4(_mm_sqrt_ps(v.m)); }
#if ARCH_AVX
inline f32x8 sqrt(f32x8 v) { return F32x8(_mm256_sqrt_ps(v.m)); }
#else
inline f32x8 sqrt(f32x8 v) { return F32x8(sqrt(v._128[0]), sqrt(v._128[1])); }
#endif
inline v4f sqrt(v4f v) { return V4f(sqrt(v.m)); }
#if COMPILER_GCC
inline v2f sin(v2f v) { return V2f(sinf(v.x), sinf(v.y)); }
inline v3f sin(v3f v) { return V3f(sinf(v.x), sinf(v.y), sinf(v.z)); }
inline v4f sin(v4f v) { return V4f(sinf(v.x), sinf(v.y), sinf(v.z), sinf(v.w)); }
inline v2f cos(v2f v) { return V2f(cosf(v.x), cosf(v.y)); }
inline v3f cos(v3f v) { return V3f(cosf(v.x), cosf(v.y), cosf(v.z)); }
inline v4f cos(v4f v) { return V4f(cosf(v.x), cosf(v.y), cosf(v.z), cosf(v.w)); }
inline void sincos(v2f v, v2f& sinOut, v2f& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
inline void sincos(v3f v, v3f& sinOut, v3f& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
inline void sincos(v4f v, v4f& sinOut, v4f& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
#else
inline f32x4 sin(f32x4 v) { return F32x4(_mm_sin_ps(v.m)); }
inline v4f sin(v4f v) { return V4f(sin(v.m)); }
inline v2f sin(v2f v) { return sin(V4f(v, 0, 0)).xy; }
inline v3f sin(v3f v) { return sin(V4f(v, 0)).xyz; }
inline f32x4 cos(f32x4 v) { return F32x4(_mm_cos_ps(v.m)); }
inline v4f cos(v4f v) { return V4f(cos(v.m)); }
inline v2f cos(v2f v) { return cos(V4f(v, 0, 0)).xy; }
inline v3f cos(v3f v) { return cos(V4f(v, 0)).xyz; }

inline void sincos(f32 v, f32& sinOut, f32& cosOut) {
	sinOut = sinf(v);
	cosOut = cosf(v);
}
inline void sincos(v2f v, v2f& sinOut, v2f& cosOut) {
	__m128 c, s = _mm_sincos_ps(&c, _mm_setr_ps(v.x, v.y, 0, 0));
	memcpy(&sinOut, &s, sizeof(sinOut));
	memcpy(&cosOut, &c, sizeof(cosOut));
}
inline void sincos(v3f v, v3f& sinOut, v3f& cosOut) {
	__m128 c, s = _mm_sincos_ps(&c, _mm_setr_ps(v.x, v.y, v.z, 0));
	memcpy(&sinOut, &s, sizeof(sinOut));
	memcpy(&cosOut, &c, sizeof(cosOut));
}
inline void sincos(f32x4 v, f32x4& sinOut, f32x4& cosOut) { sinOut.m = _mm_sincos_ps(&cosOut.m.ps, v.m); }
#if ARCH_AVX
inline void sincos(f32x8 v, f32x8& sinOut, f32x8& cosOut) { sinOut.m = _mm256_sincos_ps(&cosOut.m.ps, v.m); }
#else
inline void sincos(f32x8 v, f32x8& sinOut, f32x8& cosOut) { 
	sincos(v._128[0], sinOut._128[0], cosOut._128[0]); 
	sincos(v._128[1], sinOut._128[1], cosOut._128[1]); 
}
#endif
inline void sincos(v4f v, v4f& sinOut, v4f& cosOut) { sincos(v.m, sinOut.m, cosOut.m); }
#endif

inline f32 atan2(v2f v) { return atan2f(v.y, v.x); }

inline u32 findLowestBit(u32 val) {
	unsigned long result;
	return _BitScanForward(&result, (unsigned long)val) ? (u32)result : ~0;
}
inline u32 findLowestBit(u64 val) {
	unsigned long result;
	return _BitScanForward64(&result, val) ? (u32)result : ~0;
}
inline u32 findHighestBit(u32 val) {
	unsigned long result;
	return _BitScanReverse(&result, (unsigned long)val) ? (u32)result : ~0;
}
inline u32 findHighestBit(u64 val) {
	unsigned long result;
	return _BitScanReverse64(&result, val) ? (u32)result : ~0;
}
inline u32 countBits(u32 v) { return (u32)_mm_popcnt_u32(v); }
inline u32 countBits(s32 v) { return countBits((u32)v); }
#if ARCH_AVX2
inline u32 countBits(u64 v) { return (u32)_mm_popcnt_u64(v); }
inline u32 countBits(s64 v) { return countBits((u64)v); }
#else
inline u32 countBits(u64 v) { return countBits((u32)v) + countBits((u32)(v >> 32)); }
inline u32 countBits(s64 v) { return countBits((u64)v); }
#endif
inline u32 fillBits(u32 v) {
	u32 r = 0;
	while (v--) {
		r = (r << 1) | 1;
	}
	return r;
}
#define FRAC(constexpr, f32, s32)	\
inline constexpr f32 frac(f32 v) {	\
	v = v - (f32)(s32)v;			\
	return select(v < 0, v + 1, v);	\
}
FRAC(constexpr, f32, s32)
FRAC(, f32x4, s32x4)
FRAC(, f32x8, s32x8)
#undef FRAC

inline v4f frac(v4f v) { return V4f(frac(v.m)); }
inline v4fx4 frac(v4fx4 v) { return V4fx4(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }
inline v4fx8 frac(v4fx8 v) { return V4fx8(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }

inline v2f frac(v2f v) { return frac(V4f(v, 0, 0)).xy; }
inline v2fx4 frac(v2fx4 v) { return V2fx4(frac(v.x), frac(v.y)); }
inline v2fx8 frac(v2fx8 v) { return V2fx8(frac(v.x), frac(v.y)); }

inline v3f frac(v3f v) { return frac(V4f(v, 0)).xyz; }
inline v3fx4 frac(v3fx4 v) { return V3fx4(frac(v.x), frac(v.y), frac(v.z)); }
inline v3fx8 frac(v3fx8 v) { return V3fx8(frac(v.x), frac(v.y), frac(v.z)); }

inline constexpr s32 frac(s32 v, u32 s) {
	if (v < 0)
		return (v + 1) % (s32)s + (s32)s - 1;
	return v % (s32)s;
}
inline v2s frac(v2s v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
	};
}
inline v3s frac(v3s v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
		frac(v.z, s),
	};
}
inline v4s frac(v4s v, u32 step) {
#if 0
	return {
		frac(v.x, s),
		frac(v.y, s),
		frac(v.z, s),
		frac(v.w, s),
	};
#else
	// NOTE: this version is ~2 cycles slower than just 4 fracs (20 -> 22), 
	// but it is 2x faster if step is a power of 2
	if (u32 bits = countBits(step); bits == 1) {
		s32x4 mrmask = S32x4((s32)fillBits(findLowestBit(step)));
		s32x4 one = S32x4(1);
		return V4s(select(v.m < 0, ((v.m + one) & mrmask) + S32x4((s32)step) - one, v.m & mrmask));
	} else {
		return {
			frac(v.x, step),
			frac(v.y, step),
			frac(v.z, step),
			frac(v.w, step),
		};
	}
#endif
}
inline f32 floor(f32 v) { return floorf(v); }
inline v2f floor(v2f v) {
	return {
		floorf(v.x),
		floorf(v.y),
	};
}
inline v3f floor(v3f v) {
	return {
		floorf(v.x),
		floorf(v.y),
		floorf(v.z),
	};
}
inline f32x4 floor(f32x4 v) { return F32x4(_mm_round_ps(v.m, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)); }
#if ARCH_AVX
inline f32x8 floor(f32x8 v) { return F32x8(_mm256_round_ps(v.m, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)); }
#else
inline f32x8 floor(f32x8 v) { return F32x8(floor(v._128[0]), floor(v._128[1])); }
#endif
inline v4f floor(v4f v) { return V4f(floor(v.m)); }
inline s32 floorToInt(f32 v) { return (s32)floorf(v); }
inline v2s floorToInt(v2f v) {
	return {
		floorToInt(v.x),
		floorToInt(v.y),
	};
}
inline v3s floorToInt(v3f v) {
	return {
		floorToInt(v.x),
		floorToInt(v.y),
		floorToInt(v.z),
	};
}
inline s32x4 floorToInt(f32x4 v) { return S32x4(_mm_cvtps_epi32(_mm_floor_ps(v.m))); }
inline v4s floorToInt(v4f v) { return V4s(floorToInt(v.m)); }
inline constexpr s32 floor(s32 v, u32 s) {
	if (v < 0)
		v = (v + 1) / (s32)s - 1;
	else
		v /= (s32)s;
	return v * (s32)s;
}
inline v2s floor(v2s v, u32 step) {
	return {
		floor(v.x, step),
		floor(v.y, step),
	};
}
inline v3s floor(v3s v, u32 step) {
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
	};
}
inline v4s floor(v4s v, u32 step) {
#if 1
	// ~10 cy
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
		floor(v.w, step),
	};
#else
	// ~37 cy
	s32x4 ms	 = S32x4((s32)step);
	s32x4 pos	 = v.m / ms;
	s32x4 neg	 = (v.m + 1) / ms - 1;
	__m128i mask = _mm_cmplt_epi32(v.m.m, _mm_setzero_si128());
	return V4s(S32x4(_mm_blendv_epi32(pos.m, neg.m, mask)) * (s32)step);
#endif
}

#define CEIL(t) \
	t ceil(t v, u32 s) { return floor(v + ((s32)s - 1), s); }
inline constexpr CEIL(s32) CEIL(v2s) CEIL(v3s) CEIL(v4s);
#undef CEIL

#define ROUND(f32) \
	inline f32 round(f32 v) { return floor(v + 0.5f); }
ROUND(f32) ROUND(f32x4) ROUND(f32x8) ROUND(v2f) ROUND(v3f) ROUND(v4f);
#undef ROUND
inline v2fx8 round(v2fx8 v) { return V2fx8(round(v.x), round(v.y)); }
inline s32 roundToInt(f32 v) { return floorToInt(v + 0.5f); }
inline v2s roundToInt(v2f v) { return {roundToInt(v.x), roundToInt(v.y)}; }
inline v3s roundToInt(v3f v) { return {roundToInt(v.x), roundToInt(v.y), roundToInt(v.z)}; }
inline v4s roundToInt(v4f v) { return (v4s)round(v); }
inline v2sx8 roundToInt(v2fx8 v) { return (v2sx8)round(v); }
inline f32 dot(f32x4 a, f32x4 b) {
	f32 result;
	_mm_store_ss(&result, _mm_dp_ps(a.m, b.m, 0xFF));
	return result;
}
inline s32 dot(s32x4 a, s32x4 b) {
	a *= b;
	return a[0] + a[1] + a[2] + a[3];
}
inline u32 dot(u32x4 a, u32x4 b) {
	a *= b;
	return a[0] + a[1] + a[2] + a[3];
}
inline f32 dot(f32 a, f32 b) { return a * b; }
#define DOT(f32, v2f)       \
	inline f32 dot(v2f a, v2f b) { \
		a *= b;             \
		return a.x + a.y;   \
	}
DOT(f32, v2f) DOT(f32x4, v2fx4) DOT(f32x8, v2fx8);
DOT(s32, v2s) DOT(s32x4, v2sx4) DOT(s32x8, v2sx8);
DOT(u32, v2u) DOT(u32x4, v2ux4) DOT(u32x8, v2ux8);
#undef DOT

#define DOT(f32, v3f)           \
	inline f32 dot(v3f a, v3f b) {     \
		a *= b;                 \
		return a.x + a.y + a.z; \
	}
DOT(f32, v3f) DOT(f32x4, v3fx4) DOT(f32x8, v3fx8);
DOT(s32, v3s) DOT(s32x4, v3sx4) DOT(s32x8, v3sx8);
DOT(u32, v3u) DOT(u32x4, v3ux4) DOT(u32x8, v3ux8);
#undef DOT

inline f32 dot(v4f a, v4f b) { return dot(a.m, b.m); }
inline s32 dot(v4s a, v4s b) { return dot(a.m, b.m); }
inline u32 dot(v4u a, v4u b) { return dot(a.m, b.m); }

#define DOT(f32, v4f)                 \
	inline f32 dot(v4f a, v4f b) {           \
		a *= b;                       \
		return a.x + a.y + a.z + a.w; \
	}
DOT(f32x4, v4fx4) DOT(f32x8, v4fx8);
DOT(s32x4, v4sx4) DOT(s32x8, v4sx8);
DOT(u32x4, v4ux4) DOT(u32x8, v4ux8);
#undef DOT

template <class T>
inline auto reflect(T v, T n) {
	return v - dot(v, n) * n * 2;
}

inline v2f cross(v2f a) { return {a.y, -a.x}; }
inline v2fx4 cross(v2fx4 a) { return {a.y, -a.x}; }
inline v2fx8 cross(v2fx8 a) { return {a.y, -a.x}; }
inline v2s cross(v2s a) { return {a.y, -a.x}; }
inline v2sx4 cross(v2sx4 a) { return {a.y, -a.x}; }
inline v2sx8 cross(v2sx8 a) { return {a.y, -a.x}; }
inline v3f cross(v3f a, v3f b) {
	// clang-format off
	return {
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z, 
		a.x * b.y - a.y * b.x
	};
	// clang-format on
}

inline f32x4 abs(f32x4 v) { return F32x4(v.m & (M128)_mm_set1_epi32(0x7FFFFFFF)); }
#if ARCH_AVX
inline f32x8 abs(f32x8 v) { return F32x8(v.m & (M256)_mm256_set1_epi32(0x7FFFFFFF)); }
#else
inline f32x8 abs(f32x8 v) { return F32x8(abs(v._128[0]), abs(v._128[1])); }
#endif

inline v2f abs(v2f v) { return {fabsf(v.x), fabsf(v.y)}; }
inline v3f abs(v3f v) { return {fabsf(v.x), fabsf(v.y), fabsf(v.z)}; }
inline v4f abs(v4f v) { return {fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w)}; }
inline v2fx4 abs(v2fx4 v) { return V2fx4(abs(v.x), abs(v.y)); }
inline v2fx8 abs(v2fx8 v) { return V2fx8(abs(v.x), abs(v.y)); }

inline v2s abs(v2s a) { return {labs(a.x), labs(a.y)}; }
inline v3s abs(v3s a) { return {labs(a.x), labs(a.y), labs(a.z)}; }
inline s32x4 abs(s32x4 a) { return S32x4(_mm_abs_epi32(a.m)); }
inline v4s abs(v4s a) { return V4s(abs(a.m)); }

inline f32 sign(f32 v) { return v > 0.0f ? 1.0f : v < 0.0f ? -1.0f : 0.0f; }

inline f32 sum(v2f v) { return v.x + v.y; }

template <class T>
inline auto lengthSqr(T a) {
	return dot(a, a);
}
template <class T>
inline auto length(T a) {
	return sqrt(lengthSqr(a));
}
template <class T>
inline auto normalize(T a) {
	return a / length(a);
}
template <class T>
inline auto normalize(T a, T fallback) {
	auto lsq = lengthSqr(a);
	if (lsq == decltype(lsq){})
		return fallback;
	return a / sqrt(lsq);
}
template <class T>
inline auto distanceSqr(T a, T b) {
	return lengthSqr(a - b);
}
template <class T>
inline auto distance(T a, T b) {
	return sqrt(distanceSqr(a, b));
}
template <class T>
inline auto manhattan(T a, T b) {
	return sum(abs(a - b));
}
inline int maxDistance(v3s a, v3s b) {
	a = abs(a - b);
	return max(max(a.x, a.y), a.z);
}

template <class T>
inline constexpr auto moveTowards(T a, T b, f32 t) {
	auto d = b - a;
	if (lengthSqr(d) > t * t) {
		return a + normalize(d) * t;
	} else {
		return b;
	}
}

template <class T>
inline constexpr auto moveAway(T a, T b, f32 t) {
	return a + normalize(a - b) * t;
}

inline f32 cos01(f32 t) { return 0.5f - cosf(t * pi) * 0.5f; }
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
inline f32 cosNoise(f32 v) {
	auto fl = (s32)floor(v);
	return lerp(noise(fl), noise(fl + 1), cos01(frac(v)));
}
template <class Interp, class Sample>
inline f32 interpolate(v2f p, Interp&& interp, Sample&& sample) {
	auto x	= (s32)floor(p.x);
	auto y	= (s32)floor(p.y);
	auto fr = frac(p.x);
	return interp(interp(sample(x, y), sample(x + 1, y), fr), interp(sample(x, y + 1), sample(x + 1, y + 1), fr),
				  frac(p.y));
}
template <class Interp, class Sample>
inline f32 interpolate(v2s p, s32 s, Interp&& interp, Sample&& sample) {
	auto fl = floor(p, s);
	auto fr = frac(p.x, s) / f32(s);
	return interp(interp(sample(fl + v2s{0, 0}), sample(fl + v2s{1, 0}), fr),
				  interp(sample(fl + v2s{0, 1}), sample(fl + v2s{1, 1}), fr), frac(p.y, s) / f32(s));
}
*/
inline static constexpr v2f _randMul2{pi * 10, sqrt2 * 20};
inline static constexpr v3f _randMul3{pi * 10, sqrt2 * 20, sqrt3 * 15};
inline static constexpr v4f _randMul4{pi * 10, sqrt2 * 20, sqrt3 * 15, sqrt5 * 10};

inline u32 rotateLeft(u32 v, int shift) { return _rotl(v, shift); }
inline u32 rotateRight(u32 v, int shift) { return _rotr(v, shift); }
#if COMPILER_GCC
inline u8 rotateLeft(u8 v, int shift) { return (v << shift) | (v >> (8 - shift)); }
inline u16 rotateLeft(u16 v, int shift) { return (v << shift) | (v >> (16 - shift)); }
inline u64 rotateLeft(u64 v, int shift) { return (v << shift) | (v >> (64 - shift)); }
inline u8 rotateRight(u8 v, int shift) { return (v >> shift) | (v << (8 - shift)); }
inline u16 rotateRight(u16 v, int shift) { return (v >> shift) | (v << (16 - shift)); }
inline u64 rotateRight(u64 v, int shift) { return (v >> shift) | (v << (64 - shift)); }
#else
inline u8 rotateLeft(u8 v, int shift) { return _rotl8(v, (u8)shift); }
inline u16 rotateLeft(u16 v, int shift) { return _rotl16(v, (u8)shift); }
inline u64 rotateLeft(u64 v, int shift) { return _rotl64(v, shift); }
inline u8 rotateRight(u8 v, int shift) { return _rotr8(v, (u8)shift); }
inline u16 rotateRight(u16 v, int shift) { return _rotr16(v, (u8)shift); }
inline u64 rotateRight(u64 v, int shift) { return _rotr64(v, shift); }
#endif

#define RANDOMIZE(u32) inline u32 randomize(u32 v) { return v * 0xEB84226F ^ 0x034FB5E7; }
RANDOMIZE(u32) RANDOMIZE(u32x4) RANDOMIZE(u32x8)
#undef RANDOMIZE
#define RANDOMIZE(u32, s32) s32 randomize(s32 v) { return (s32)randomize((u32)v); }
RANDOMIZE(u32, s32) RANDOMIZE(u32x4, s32x4) RANDOMIZE(u32x8, s32x8)
#undef RANDOMIZE

#define RANDOMIZE(v2u)         \
	inline v2u randomize(v2u v) {     \
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
	inline v3u randomize(v3u v) {       \
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
	inline v4u randomize(v4u v) {            \
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
	inline v2s randomize(v2s v) { return (v2s)randomize((v2u)v); }
RANDOMIZE(v2s, v2u) RANDOMIZE(v2sx4, v2ux4) RANDOMIZE(v2sx8, v2ux8);
RANDOMIZE(v3s, v3u) RANDOMIZE(v3sx4, v3ux4) RANDOMIZE(v3sx8, v3ux8);
RANDOMIZE(v4s, v4u) RANDOMIZE(v4sx4, v4ux4) RANDOMIZE(v4sx8, v4ux8);
#undef RANDOMIZE

inline u8 randomU8(u8 r) {
	r += 0x0C;
	r *= 0x61;
	r ^= 0xB2;
	r -= 0x80;
	r ^= 0xF5;
	r *= 0xA7;
	return rotateLeft(r, 4);
}
inline u32 randomU32(u32 r) {
	r += 0x0C252DA0;
	r *= 0x55555561;
	r ^= 0xB23E2387;
	r -= 0x8069BAC0;
	r ^= 0xF5605798;
	r *= 0xAAAAAABF;
	return (r << 16) | (r >> 16);
}
inline u32 randomU32(s32 in) { return randomU32((u32)in); }
inline u32 randomU32(v2s in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	return x ^ y;
}
inline u32 randomU32(v3s in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	auto z = randomU32(in.z);
	return x + y + z;
}
inline u32 randomU32(v4s v) {
	v = randomize(v);
	return u32(v.x) + u32(v.y) + u32(v.z) + u32(v.w);
}
inline u64 randomU64(v3s in) {
	auto x = (u64)randomU32(in.x);
	auto y = (u64)randomU32(in.y);
	auto z = (u64)randomU32(in.z);
	return x | (y << 32) + z | (x << 32) + y | (z << 32);
}

#define RANDOM(v2f, V2f)              \
	inline v2f random(v2f v) {               \
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
	inline v3f random(v3f v) {                                    \
		v = frac(v * V3f(_randMul3));                      \
		v += dot(v, v + pi * 4);                           \
		return frac(V3f(v.x * v.y, v.y * v.z, v.x * v.z)); \
	}
RANDOM(v3f, V3f)
RANDOM(v3fx4, V3fx4)
RANDOM(v3fx8, V3fx8)
#undef RANDOM

#define RANDOM(v4f, V4f)                          \
	inline v4f random(v4f v) {                           \
		v = frac((v + sqrt2) * V4f(_randMul4));   \
		v += dot(v, v + pi * 4);                  \
		return frac(v * V4f(v.y, v.z, v.w, v.x)); \
	}
RANDOM(v4f, V4f)
RANDOM(v4fx4, V4fx4)
RANDOM(v4fx8, V4fx8)
#undef RANDOM

// clang-format off
#define RANDOM(v2f, v2s, v2u) v2f random(v2s v) { return (v2f)(randomize((v2u)v) >> 8) * (1.0f / 16777216.0f); }
RANDOM(v2f, v2s, v2u) RANDOM(v2fx4, v2sx4, v2ux4) RANDOM(v2fx8, v2sx8, v2ux8)
RANDOM(v3f, v3s, v3u) RANDOM(v3fx4, v3sx4, v3ux4) RANDOM(v3fx8, v3sx8, v3ux8)
RANDOM(v4f, v4s, v4u) RANDOM(v4fx4, v4sx4, v4ux4) RANDOM(v4fx8, v4sx8, v4ux8)
#undef RANDOM

#define RANDOM(v2f, v2s) inline v2f operator()(v2s v) const { return random(v); }
struct Random {
	RANDOM(v2f, v2s) RANDOM(v2fx4, v2sx4) RANDOM(v2fx8, v2sx8)
	RANDOM(v3f, v3s) RANDOM(v3fx4, v3sx4) RANDOM(v3fx8, v3sx8)
	RANDOM(v4f, v4s) RANDOM(v4fx4, v4sx4) RANDOM(v4fx8, v4sx8)
	RANDOM(v2f, v2f) RANDOM(v2fx4, v2fx4) RANDOM(v2fx8, v2fx8)
	RANDOM(v3f, v3f) RANDOM(v3fx4, v3fx4) RANDOM(v3fx8, v3fx8)
	RANDOM(v4f, v4f) RANDOM(v4fx4, v4fx4) RANDOM(v4fx8, v4fx8)
};
#undef RANDOM

template<class T> constexpr bool isVector = false;
template<> constexpr bool isVector<v2f  > = true;
template<> constexpr bool isVector<v3f  > = true;
template<> constexpr bool isVector<v4f  > = true;
template<> constexpr bool isVector<v2s  > = true;
template<> constexpr bool isVector<v3s  > = true;
template<> constexpr bool isVector<v4s  > = true;
template<> constexpr bool isVector<v2u  > = true;
template<> constexpr bool isVector<v3u  > = true;
template<> constexpr bool isVector<v4u  > = true;
template<umm w> constexpr bool isVector<v2fx<w>> = true;
template<umm w> constexpr bool isVector<v3fx<w>> = true;
template<umm w> constexpr bool isVector<v4fx<w>> = true;
template<umm w> constexpr bool isVector<v2sx<w>> = true;
template<umm w> constexpr bool isVector<v3sx<w>> = true;
template<umm w> constexpr bool isVector<v4sx<w>> = true;
template<umm w> constexpr bool isVector<v2ux<w>> = true;
template<umm w> constexpr bool isVector<v3ux<w>> = true;
template<umm w> constexpr bool isVector<v4ux<w>> = true;
// clang-format on

template <class T>
struct RemoveQualifiers {
	using Type = T;
};
template <class T>
struct RemoveQualifiers<const T> {
	using Type = typename RemoveQualifiers<T>::Type;
};
template <class T>
struct RemoveQualifiers<volatile T> {
	using Type = typename RemoveQualifiers<T>::Type;
};
template <class T>
struct RemoveQualifiers<T*> {
	using Type = typename RemoveQualifiers<T>::Type;
};
template <class T>
struct RemoveQualifiers<T&> {
	using Type = typename RemoveQualifiers<T>::Type;
};

template <class T>
constexpr umm _widthOf = 0;
// clang-format off
template<umm width> constexpr umm _widthOf<f32x<width>> = width;
template<umm width> constexpr umm _widthOf<s32x<width>> = width;
template<umm width> constexpr umm _widthOf<u32x<width>> = width;
template<umm width> constexpr umm _widthOf<v2fx<width>> = width;
template<umm width> constexpr umm _widthOf<v2sx<width>> = width;
template<umm width> constexpr umm _widthOf<v2ux<width>> = width;
template<umm width> constexpr umm _widthOf<v3fx<width>> = width;
template<umm width> constexpr umm _widthOf<v3sx<width>> = width;
template<umm width> constexpr umm _widthOf<v3ux<width>> = width;
template<umm width> constexpr umm _widthOf<v4fx<width>> = width;
template<umm width> constexpr umm _widthOf<v4sx<width>> = width;
template<umm width> constexpr umm _widthOf<v4ux<width>> = width;
// clang-format on

template <class T>
constexpr umm widthOf = _widthOf<RemoveQualifiers<T>::Type>;

template <umm width, class T>
inline auto widen(T v) = delete;

// clang-format off
template<> inline auto widen<1>(v2f v) { return v; }
template<> inline auto widen<1>(v3f v) { return v; }
template<> inline auto widen<1>(v4f v) { return v; }
template<> inline auto widen<1>(v2s v) { return v; }
template<> inline auto widen<1>(v3s v) { return v; }
template<> inline auto widen<1>(v4s v) { return v; }
template<> inline auto widen<1>(v2u v) { return v; }
template<> inline auto widen<1>(v3u v) { return v; }
template<> inline auto widen<1>(v4u v) { return v; }
template<> inline auto widen<4>(v2f v) { return V2fx4(v); }
template<> inline auto widen<4>(v3f v) { return V3fx4(v); }
template<> inline auto widen<4>(v4f v) { return V4fx4(v); }
template<> inline auto widen<4>(v2s v) { return V2sx4(v); }
template<> inline auto widen<4>(v3s v) { return V3sx4(v); }
template<> inline auto widen<4>(v4s v) { return V4sx4(v); }
template<> inline auto widen<4>(v2u v) { return V2ux4(v); }
template<> inline auto widen<4>(v3u v) { return V3ux4(v); }
template<> inline auto widen<4>(v4u v) { return V4ux4(v); }
template<> inline auto widen<8>(v2f v) { return V2fx8(v); }
template<> inline auto widen<8>(v3f v) { return V3fx8(v); }
template<> inline auto widen<8>(v4f v) { return V4fx8(v); }
template<> inline auto widen<8>(v2s v) { return V2sx8(v); }
template<> inline auto widen<8>(v3s v) { return V3sx8(v); }
template<> inline auto widen<8>(v4s v) { return V4sx8(v); }
template<> inline auto widen<8>(v2u v) { return V2ux8(v); }
template<> inline auto widen<8>(v3u v) { return V3ux8(v); }
template<> inline auto widen<8>(v4u v) { return V4ux8(v); }
// clang-format on

namespace CE {
#define WIDEN(ty, tyx4, conx4) \
	inline constexpr tyx4 widen(ty a, ty b, ty c, ty d) { return CE::conx4(a, b, c, d); }
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
	inline constexpr tyx8 widen(ty a, ty b, ty c, ty d, ty e, ty f, ty g, ty h) { return CE::conx8(a, b, c, d, e, f, g, h); }
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
inline auto cvtScalar(T v) = delete;

// clang-format off
template<> inline auto cvtScalar<f32>(v2s v)   { return (v2f)v; }
template<> inline auto cvtScalar<f32>(v3s v)   { return (v3f)v; }
template<> inline auto cvtScalar<f32>(v4s v)   { return (v4f)v; }
template<> inline auto cvtScalar<f32>(v2sx4 v) { return (v2fx4)v; }
template<> inline auto cvtScalar<f32>(v3sx4 v) { return (v3fx4)v; }
template<> inline auto cvtScalar<f32>(v4sx4 v) { return (v4fx4)v; }
template<> inline auto cvtScalar<f32>(v2sx8 v) { return (v2fx8)v; }
template<> inline auto cvtScalar<f32>(v3sx8 v) { return (v3fx8)v; }
template<> inline auto cvtScalar<f32>(v4sx8 v) { return (v4fx8)v; }
// clang-format on

template <class T, umm size>
struct Array {
	inline constexpr T& operator[](umm i) { return v[i]; }
	inline constexpr T const& operator[](umm i) const { return v[i]; }
	T v[size];
};

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
inline constexpr auto cvtScalar(Array<From, size> const& arr) {
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
	inline constexpr WideArray(T const (&arr)[count8 * 8 + count4 * 4 + count1]) {
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
	inline constexpr WideArray(Array<T, count8 * 8 + count4 * 4 + count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
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
	inline constexpr WideArray(T const (&arr)[count1]) {
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[i];
		}
	}
	inline constexpr WideArray(Array<T, count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
		for (auto const& t : t1)
			cb(t);
	}
};
template <class T, umm count4>
struct WideArray<T, 0, count4, 0> {
	using T4 = typename WideType<T, 4>::Type;
	T4 t4[count4]{};
	inline constexpr WideArray(T const (&arr)[count4 * 4]) {
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[i * 4 + 0], arr[i * 4 + 1], arr[i * 4 + 2], arr[i * 4 + 3]);
		}
	}
	inline constexpr WideArray(Array<T, count4 * 4> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
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
	inline constexpr WideArray(T const (&arr)[count4 * 4 + count1]) {
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[i * 4 + 0], arr[i * 4 + 1], arr[i * 4 + 2], arr[i * 4 + 3]);
		}
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[count4 * 4 + i];
		}
	}
	inline constexpr WideArray(Array<T, count4 * 4 + count1> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
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
	inline constexpr WideArray(T const (&arr)[count8 * 8]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
	}
	inline constexpr WideArray(Array<T, count8 * 8> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
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
	inline constexpr WideArray(T const (&arr)[count8 * 8 + count1]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (umm i = 0; i < count1; ++i) {
			t1[i] = arr[count8 * 8 + i];
		}
	}
	inline constexpr WideArray(Array<T, count8 * 8 + count1> const& arr) : WideArray(arr.v) {}
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
	inline constexpr WideArray(T const (&arr)[count8 * 8 + count4 * 4]) {
		for (umm i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (umm i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[count8 * 8 + i * 4 + 0], arr[count8 * 8 + i * 4 + 1], arr[count8 * 8 + i * 4 + 2],
							  arr[count8 * 8 + i * 4 + 3]);
		}
	}
	inline constexpr WideArray(Array<T, count8 * 8 + count4 * 4> const& arr) : WideArray(arr.v) {}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
		for (auto const& t : t4)
			cb(t);
	}
};
template <class T, umm size>
inline constexpr auto makeWide(Array<T, size> const& arr) {
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
static constexpr Array<v2s, 9> voronoiOffsets2s { 
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
static constexpr Array<v3s, 27> voronoiOffsets3s { 
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
static constexpr Array<v4s, 81> voronoiOffsets4s {
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
static constexpr auto voronoiOffsets2f = CE::cvtScalar<f32>(voronoiOffsets2s);
static constexpr auto voronoiOffsets3f = CE::cvtScalar<f32>(voronoiOffsets3s);
static constexpr auto voronoiOffsets4f = CE::cvtScalar<f32>(voronoiOffsets4s);
// clang-format on

template <class Random = Random>
inline f32 voronoi(v2f v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets2f);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * (1 / sqrt2);
}
template <class Random = Random>
inline f32 voronoi(v3f v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets3f);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * (1 / sqrt3);
}
template <class Random = Random>
inline f32 voronoi(v4f v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets4f);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * 0.5f;
}
template <class Random = Random>
inline f32 voronoi(v2s v, u32 cellSize, Random random = {}) {
	v2s flr							  = floor(v, cellSize);
	v2s tile						  = flr / (s32)cellSize;
	v2f rel							  = (v2f)(v - flr) / (f32)cellSize - 0.5f;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets2s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	f32 minDist = 1000;
	auto tile8	= V2sx8(tile);
	auto rel8	= V2fx8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, distanceSqr(rel8, random(tile8 + offset) * 0.5f + (v2fx8)offset));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) * 0.5f + (v2f)offset));
	}
#endif
	return sqrt(minDist) * (1 / sqrt2);
}
template <class Random = Random>
inline f32 voronoi(v3s v, u32 cellSize, Random random = {}) {
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
inline f32 voronoi(v4s v, u32 cellSize, Random random = {}) {
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

inline bool raycastLine(v2f a, v2f b, v2f c, v2f d, v2f& point, v2f& normal) {
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
inline b32x<ps> raycastLine(v2fx<ps> a, v2fx<ps> b, v2fx<ps> c, v2fx<ps> d, v2fx<ps>& point, v2fx<ps>& normal, bool backFace = true) {
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
inline bool raycastRect(v2f a, v2f b, v2f tile, v2f size, v2f& point, v2f& normal) {
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
inline b32x<ps> raycastRect(v2fx<ps> a, v2fx<ps> b, v2fx<ps> tile, v2fx<ps> size, v2fx<ps>& point, v2fx<ps>& normal, bool backFace = true) {
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
inline bool raycastPlane(v3f a, v3f b, v3f p1, v3f p2, v3f p3, v3f& point, v3f& normal) {
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

inline bool raycastBlock(v3f a, v3f b, v3f blk, v3f blockDimensions, v3f &point, v3f &normal) {
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
inline constexpr T linearSample(const T (&arr)[size], float t) noexcept {
	f32 f = frac(t) * size;
	s32 a = (s32)f;
	s32 b = a + 1;
	if (b == size)
		b = 0;
	return lerp(arr[a], arr[b], frac(f));
}
struct FrustumPlanes {
	v4f planes[6];
	inline FrustumPlanes(m4 vp) {
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
	inline bool containsSphere(v3f position, float radius) const {
		for (auto p : planes) {
			if (dot(v3f{p.x, p.y, p.z}, position) + p.w + radius < 0) {
				return false;
			}
		}
		return true;
	}
};

namespace CE {

inline constexpr u32 countBits(u32 v) {
	u32 s = 0;
	u32 r = v;
	for (u32 i = 0; i < 32; ++i) {
		s += r & 1;
		r >>= 1;
	}
	return s;
}
inline constexpr v4s frac(v4s v, u32 step) {
	return {
		TL::frac(v.x, step),
		TL::frac(v.y, step),
		TL::frac(v.z, step),
		TL::frac(v.w, step),
	};
}

} // namespace CE

#define MAX_PACK_IMPL(p)								\
FORCEINLINE f32x##p F32xm(f32 v) { return F32x##p(v); } \
FORCEINLINE s32x##p S32xm(s32 v) { return S32x##p(v); } \
FORCEINLINE u32x##p U32xm(u32 v) { return U32x##p(v); } \
FORCEINLINE v2fx##p V2fxm(f32 v) { return V2fx##p(v); } \
FORCEINLINE v2fx##p V2fxm(v2f v) { return V2fx##p(v); } \
FORCEINLINE v3fx##p V3fxm(f32 v) { return V3fx##p(v); } \
FORCEINLINE v3fx##p V3fxm(v3f v) { return V3fx##p(v); } \
FORCEINLINE v4fx##p V4fxm(f32 v) { return V4fx##p(v); } \
FORCEINLINE v4fx##p V4fxm(v4f v) { return V4fx##p(v); } 

#ifndef TL_MATH_MAX_PACK
#if ARCH_AVX512F
#define TL_MATH_MAX_PACK 16
#elif ARCH_AVX
#define TL_MATH_MAX_PACK 8
#else
#define TL_MATH_MAX_PACK 4
#endif
#endif

static_assert(TL_MATH_MAX_PACK == 4 || TL_MATH_MAX_PACK == 8 ||TL_MATH_MAX_PACK == 16, "TL_MATH_MAX_PACK must be 4 (SSE), 8 (AVX) or 16 (AVX512)");

MAX_PACK_IMPL(TL_MATH_MAX_PACK)

#undef MAX_PACK_IMPL

template <class T>
static constexpr u32 simdElementCount = simdWidth / sizeof(T);

} // namespace TL

#undef MEMBERS2
#undef MEMBERS3
#undef MEMBERS4
#undef MEMFUNS_BASIC
#undef MEMFUNS_DATA
#undef MEMFUNS_INT
#undef MEMFUNS_SHIFT_SCL
#undef SHUFFLE

#if COMPILER_MSVC
#pragma warning(pop)
#endif
