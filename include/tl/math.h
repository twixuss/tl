#pragma once
#include "common.h"

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif
#include <float.h>
#include <math.h>
#include <memory.h>

#if COMPILER_GCC
#include <immintrin.h>
#elif COMPILER_MSVC
#include <intrin.h>
#endif

//#if !ARCH_AVX
//#define __m256
//#define __m256i
//#endif

#if COMPILER_MSVC
#pragma warning(pop)
#pragma warning(push)
#pragma warning(disable : 4201 /* unnamed struct */                              \
				4626		   /* assignment operator was implicitly deleted  */ \
				4820		   /* struct padding  */                             \
				5045 /* spectre */)
#endif

namespace TL {

FORCEINLINE __m128i _mm_blendv_epi32(__m128i a, __m128i b, __m128i mask) {
	__m128 r = _mm_blendv_ps(*(__m128*)&a, *(__m128*)&b, *(__m128*)&mask);
	return *(__m128i*)&r;
}
FORCEINLINE __m256 _mm256_cmplt_ps(__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_LT_OQ); }

// clang-format off

constexpr f32 pi    = f32(3.1415926535897932384626433832795L);
constexpr f32 invPi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2 = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3 = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5 = f32(2.2360679774997896964091736687313L);

// clang-format on

template <class T>
constexpr auto deg2rad(T deg) {
	return deg * (1.0L / 360.0L * pi * 2);
}
template <class T>
constexpr auto rad2deg(T rad) {
	return rad * (invPi / 2 * 360.0f);
}

template <class T, class U>
constexpr auto min(T a, U b) {
	return a < b ? a : b;
}
template <class T, class U, class... Rest>
constexpr auto min(T a, U b, Rest... rest) {
	return min(min(a, b), rest...);
}
template <class T, class U>
constexpr auto max(T a, U b) {
	return a > b ? a : b;
}
template <class T, class U, class... Rest>
constexpr auto max(T a, U b, Rest... rest) {
	return max(max(a, b), rest...);
}
template <class T, class U, class V, class W>
constexpr void minmax(T a, U b, V& mn, W& mx) {
	if (a < b) {
		mn = a;
		mx = b;
	} else {
		mn = b;
		mx = a;
	}
}
template <class T, class SN, class SX, class DN, class DX>
constexpr auto map(T v, SN sn, SX sx, DN dn, DX dx) {
	return (v - sn) / (sx - sn) * (dx - dn) + dn;
}
template <class T, class U, class V>
constexpr auto clamp(T a, U mi, V ma) {
	return min(max(a, mi), ma);
}
template <class T>
constexpr auto lerp(T a, T b, f32 t) {
	return a + (b - a) * t;
}
template <class A, class B>
auto select(bool mask, A a, B b) {
	return mask ? a : b;
}
template <class T>
constexpr auto moveTowards(T a, T b, f32 t) {
	return select(a < b, min(a + t, b), max(a - t, b));
}
template <class T>
constexpr auto pow2(T v) {
	return v * v;
}
template <class T>
constexpr auto pow3(T v) {
	return v * v * v;
}
template <class T>
constexpr auto pow4(T v) {
	return pow2(v * v);
}

template <class To, class From>
To cvt(From v) {
	return (To)v;
}

namespace CE {

template <class To, class From>
constexpr To cvt(From v) {
	return (To)v;
}

} // namespace CE

union f32x4;
union f32x8;

union s32x4;
union s32x8;

union u32x4;
union u32x8;

union v2;
union v2s;
union v2u;

union v2x4;
union v2sx4;
union v2ux4;

union v2x8;
union v2sx8;
union v2ux8;

union v3;
union v3s;
union v3u;

union v3x4;
union v3sx4;
union v3ux4;

union v3x8;
union v3sx8;
union v3ux8;

union v4;
union v4s;
union v4u;

union v4x4;
union v4sx4;
union v4ux4;

union v4x8;
union v4sx8;
union v4ux8;

union m4;

constexpr f32x4 F32x4(__m128);
constexpr s32x4 S32x4(__m128i);
constexpr u32x4 U32x4(__m128i);

constexpr f32x8 F32x8(__m256);
constexpr s32x8 S32x8(__m256i);
constexpr u32x8 U32x8(__m256i);

inline f32x4 F32x4(f32);
inline s32x4 S32x4(s32);
inline u32x4 U32x4(u32);

inline f32x8 F32x8(f32);
inline s32x8 S32x8(s32);
inline u32x8 U32x8(u32);

inline s32x4 S32x4(s32, s32, s32, s32);
inline u32x4 U32x4(u32, u32, u32, u32);

inline s32x8 S32x8(s32, s32, s32, s32, s32, s32, s32, s32);
inline u32x8 U32x8(u32, u32, u32, u32, u32, u32, u32, u32);

constexpr v2 V2(f32 = 0);
constexpr v2s V2s(s32 = 0);
constexpr v2u V2u(u32 = 0);

constexpr v3 V3(f32 = 0);
constexpr v3s V3s(s32 = 0);
constexpr v3u V3u(u32 = 0);

constexpr v4 V4(f32 = 0);
constexpr v4s V4s(s32 = 0);
constexpr v4u V4u(u32 = 0);

constexpr v2 V2(f32 x, f32 y);
constexpr v2s V2s(s32 x, s32 y);
constexpr v2u V2u(u32 x, u32 y);

constexpr v3 V3(f32 x, f32 y, f32 z);
constexpr v3s V3s(s32 x, s32 y, s32 z);
constexpr v3u V3u(u32 x, u32 y, u32 z);

constexpr v4 V4(f32x4);
constexpr v4s V4s(s32x4);
constexpr v4u V4u(u32x4);

inline v2x4 V2x4(f32);
inline v2sx4 V2sx4(s32);
inline v2ux4 V2ux4(u32);

constexpr v2x4 V2x4(f32x4);
constexpr v2sx4 V2sx4(s32x4);
constexpr v2ux4 V2ux4(u32x4);

constexpr v3x4 V3x4(f32x4);
constexpr v3sx4 V3sx4(s32x4);
constexpr v3ux4 V3ux4(u32x4);

constexpr v3x4 V3x4(__m128 x, __m128 y, __m128 z);
constexpr v3x4 V3x4(v4 x, v4 y, v4 z);
inline v3x4 V3x4(f32 v);

constexpr v3sx4 V3sx4(__m128i x, __m128i y, __m128i z);
constexpr v3sx4 V3sx4(v4s x, v4s y, v4s z);
inline v3sx4 V3sx4(s32 v);

constexpr v3ux4 V3ux4(__m128i x, __m128i y, __m128i z);
constexpr v3ux4 V3ux4(v4u x, v4u y, v4u z);
inline v3ux4 V3ux4(u32 v);

inline v2x8 V2x8(f32 v);
inline v2sx8 V2sx8(s32 v);
inline v2ux8 V2ux8(u32 v);

constexpr v2x8 V2x8(f32x8 v);
constexpr v2sx8 V2sx8(s32x8 v);
constexpr v2ux8 V2ux8(u32x8 v);

inline v3x8 V3x8(f32 v);
inline v3sx8 V3sx8(s32 v);
inline v3ux8 V3ux8(u32 v);

constexpr v3x8 V3x8(f32x8 v);
constexpr v3sx8 V3sx8(s32x8 v);
constexpr v3ux8 V3ux8(u32x8 v);

v4x4 V4x4(f32);
v4sx4 V4sx4(s32);
v4ux4 V4ux4(u32);

constexpr v4x4 V4x4(f32x4);
constexpr v4sx4 V4sx4(s32x4);
constexpr v4ux4 V4ux4(u32x4);

v4x8 V4x8(f32);
v4sx8 V4sx8(s32);
v4ux8 V4ux8(u32);

constexpr v4x8 V4x8(f32x8);
constexpr v4sx8 V4sx8(s32x8);
constexpr v4ux8 V4ux8(u32x8);

constexpr m4 M4();
constexpr m4 M4(v4 i, v4 j, v4 k, v4 l);
constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l);
constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw);

inline v2 sin(v2 v);
inline v3 sin(v3 v);
inline v4 sin(v4 v);
inline v2 cos(v2 v);
inline v3 cos(v3 v);
inline v4 cos(v4 v);
inline void sincos(v2 v, v2& sinOut, v2& cosOut);
inline void sincos(v3 v, v3& sinOut, v3& cosOut);
inline void sincos(v4 v, v4& sinOut, v4& cosOut);

#define MEMBERS2(width, scl, v1) \
	struct {                     \
		v1 x, y;                 \
	};                           \
	scl v[(width)*2]

#define MEMBERS3(width, scl, v1, v2) \
	struct {                         \
		v1 x, y, z;                  \
	};                               \
	v2 xy;                           \
	struct {                         \
		v1 _pad0;                    \
		v2 yz;                       \
	};                               \
	scl v[(width)*3]

#define MEMBERS4(width, scl, v1, v2, v3) \
	struct {                             \
		v1 x, y, z, w;                   \
	};                                   \
	v3 xyz;                              \
	struct {                             \
		v1 _pad0;                        \
		union {                          \
			v2 yz;                       \
			v3 yzw;                      \
		};                               \
	};                                   \
	struct {                             \
		v2 xy;                           \
		v2 zw;                           \
	};                                   \
	scl v[(width)*4]

#define MEMFUNS_BASIC_SCL(vec, scl, con)                      \
	friend vec operator+(scl a, vec b) { return con(a) + b; } \
	friend vec operator-(scl a, vec b) { return con(a) - b; } \
	friend vec operator*(scl a, vec b) { return con(a) * b; } \
	friend vec operator/(scl a, vec b) { return con(a) / b; } \
	vec operator+(scl b) const { return *this + con(b); }     \
	vec operator-(scl b) const { return *this - con(b); }     \
	vec operator*(scl b) const { return *this * con(b); }     \
	vec operator/(scl b) const { return *this / con(b); }     \
	vec& operator+=(scl b) { return *this += con(b); }        \
	vec& operator-=(scl b) { return *this -= con(b); }        \
	vec& operator*=(scl b) { return *this *= con(b); }        \
	vec& operator/=(scl b) { return *this /= con(b); }

#define MEMFUNS_BASIC(vec, scl, con)                       \
	vec& operator+=(vec b) { return *this = *this + b; }   \
	vec& operator-=(vec b) { return *this = *this - b; }   \
	vec& operator*=(vec b) { return *this = *this * b; }   \
	vec& operator/=(vec b) { return *this = *this / b; }   \
	bool operator!=(vec b) const { return !(*this == b); } \
	MEMFUNS_BASIC_SCL(vec, scl, con)

#define MEMFUNS_DATA(scl)                                  \
	constexpr scl* data() { return v; }                    \
	constexpr scl const* data() const { return v; }        \
	constexpr scl& operator[](size_t _i) { return v[_i]; } \
	constexpr scl operator[](size_t _i) const { return v[_i]; }

#define MEMFUNS_INT(vec, scl, con)                              \
	vec& operator%=(vec b) { return *this = *this % b; }        \
	vec& operator^=(vec b) { return *this = *this ^ b; }        \
	vec& operator|=(vec b) { return *this = *this | b; }        \
	vec& operator&=(vec b) { return *this = *this & b; }        \
	vec& operator<<=(vec b) { return *this = *this << b; }      \
	vec& operator>>=(vec b) { return *this = *this >> b; }      \
	vec operator%(scl b) const { return *this % con(b); }       \
	vec operator^(scl b) const { return *this ^ con(b); }       \
	vec operator|(scl b) const { return *this | con(b); }       \
	vec operator&(scl b) const { return *this & con(b); }       \
	vec& operator%=(scl b) { return *this %= con(b); }          \
	vec& operator^=(scl b) { return *this ^= con(b); }          \
	vec& operator|=(scl b) { return *this |= con(b); }          \
	vec& operator&=(scl b) { return *this &= con(b); }          \
	vec& operator<<=(scl b) { return *this = *this << b; }      \
	vec& operator>>=(scl b) { return *this = *this >> b; }      \
	friend vec operator%(scl a, vec b) { return con(a) % b; }   \
	friend vec operator^(scl a, vec b) { return con(a) ^ b; }   \
	friend vec operator|(scl a, vec b) { return con(a) | b; }   \
	friend vec operator&(scl a, vec b) { return con(a) & b; }   \
	friend vec operator<<(scl a, vec b) { return con(a) << b; } \
	friend vec operator>>(scl a, vec b) { return con(a) >> b; }

#define MEMFUNS_SHIFT_SCL(vec, scl, con)                    \
	vec operator<<(scl b) const { return *this << con(b); } \
	vec operator>>(scl b) const { return *this >> con(b); }

#define MEMFUNS_SHIFT_SCL_M(vec, scl, con)              \
	vec operator<<(scl b) const { return con(m << b); } \
	vec operator>>(scl b) const { return con(m >> b); }

#define MEMFUNS_SHIFT_SCL_2(vec, scl)                        \
	vec operator<<(scl b) const { return {x << b, y << b}; } \
	vec operator>>(scl b) const { return {x >> b, y >> b}; }

#define MEMFUNS_SHIFT_SCL_3(vec, scl)                                \
	vec operator<<(scl b) const { return {x << b, y << b, z << b}; } \
	vec operator>>(scl b) const { return {x >> b, y >> b, z >> b}; }

#define MEMFUNS_SHIFT_SCL_4(vec, scl)                                        \
	vec operator<<(scl b) const { return {x << b, y << b, z << b, w << b}; } \
	vec operator>>(scl b) const { return {x >> b, y >> b, z >> b, w >> b}; }

union f32x4 {
	f32 v[4];
	struct {
		f32 i, j, k, l;
	};
	__m128 m;
	f32x4 operator-() const { return F32x4(_mm_sub_ps(_mm_setzero_ps(), m)); }
	f32x4 operator+(f32x4 b) const { return F32x4(_mm_add_ps(m, b.m)); }
	f32x4 operator-(f32x4 b) const { return F32x4(_mm_sub_ps(m, b.m)); }
	f32x4 operator*(f32x4 b) const { return F32x4(_mm_mul_ps(m, b.m)); }
	f32x4 operator/(f32x4 b) const { return F32x4(_mm_div_ps(m, b.m)); }
	bool operator==(f32x4 b) const { return i == b.i && j == b.j && k == b.k && l == b.l; }
	f32x4 operator<(f32x4 b) const { return F32x4(_mm_cmp_ps(m, b.m, _CMP_LT_OQ)); }
	f32x4 operator>(f32x4 b) const { return F32x4(_mm_cmp_ps(m, b.m, _CMP_GT_OQ)); }
	f32x4 operator<=(f32x4 b) const { return F32x4(_mm_cmp_ps(m, b.m, _CMP_LE_OQ)); }
	f32x4 operator>=(f32x4 b) const { return F32x4(_mm_cmp_ps(m, b.m, _CMP_GE_OQ)); }
	MEMFUNS_DATA(f32);
	MEMFUNS_BASIC(f32x4, f32, F32x4)
};

constexpr f32x4 F32x4(__m128 m) {
	f32x4 r{};
	r.m = m;
	return r;
}
inline f32x4 F32x4(f32 v) { return F32x4(_mm_set1_ps(v)); }
inline f32x4 F32x4(f32 i, f32 j, f32 k, f32 l) { return F32x4(_mm_setr_ps(i, j, k, l)); }

union f32x8 {
	f32 v[8];
	__m256 m;
	f32x8 operator-() const { return F32x8(_mm256_sub_ps(_mm256_setzero_ps(), m)); }
	f32x8 operator+(f32x8 b) const { return F32x8(_mm256_add_ps(m, b.m)); }
	f32x8 operator-(f32x8 b) const { return F32x8(_mm256_sub_ps(m, b.m)); }
	f32x8 operator*(f32x8 b) const { return F32x8(_mm256_mul_ps(m, b.m)); }
	f32x8 operator/(f32x8 b) const { return F32x8(_mm256_div_ps(m, b.m)); }
	bool operator==(f32x8 b) const {
		return v[0] == b.v[0] && v[1] == b.v[1] && v[2] == b.v[2] && v[3] == b.v[3] && v[4] == b.v[4] &&
			   v[5] == b.v[5] && v[6] == b.v[6] && v[7] == b.v[7];
	}
	f32x8 operator<(f32x8 b) const { return F32x8(_mm256_cmp_ps(m, b.m, _CMP_LT_OQ)); }
	f32x8 operator>(f32x8 b) const { return F32x8(_mm256_cmp_ps(m, b.m, _CMP_GT_OQ)); }
	f32x8 operator<=(f32x8 b) const { return F32x8(_mm256_cmp_ps(m, b.m, _CMP_LE_OQ)); }
	f32x8 operator>=(f32x8 b) const { return F32x8(_mm256_cmp_ps(m, b.m, _CMP_GE_OQ)); }
	MEMFUNS_DATA(f32);
	MEMFUNS_BASIC(f32x8, f32, F32x8)
};
constexpr f32x8 F32x8(__m256 m) {
	f32x8 r{};
	r.m = m;
	return r;
}
inline f32x8 F32x8(f32 v) { return F32x8(_mm256_set1_ps(v)); }
inline f32x8 F32x8(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
	return F32x8(_mm256_setr_ps(a, b, c, d, e, f, g, h));
}
inline f32x8 F32x8(f32 a, f32 b, f32 c, f32 d) { return F32x8(_mm256_setr_ps(a, b, c, d, a, b, c, d)); }

union s32x4 {
	s32 v[4];
	struct {
		s32 i, j, k, l;
	};
	__m128i m;
	s32x4 operator-() const { return S32x4(_mm_sub_epi32(_mm_setzero_si128(), m)); }
	s32x4 operator~() const { return *this ^ (~0); }
	s32x4 operator+(s32x4 b) const { return S32x4(_mm_add_epi32(m, b.m)); }
	s32x4 operator-(s32x4 b) const { return S32x4(_mm_sub_epi32(m, b.m)); }
	s32x4 operator*(s32x4 b) const { return S32x4(_mm_mullo_epi32(m, b.m)); }
	s32x4 operator^(s32x4 b) const { return S32x4(_mm_xor_si128(m, b.m)); }
	s32x4 operator|(s32x4 b) const { return S32x4(_mm_or_si128(m, b.m)); }
	s32x4 operator&(s32x4 b) const { return S32x4(_mm_and_si128(m, b.m)); }
	s32x4 operator<<(s32x4 b) const { return S32x4(i << b.i, j << b.j, k << b.k, l << b.l); }
	s32x4 operator>>(s32x4 b) const { return S32x4(i >> b.i, j >> b.j, k >> b.k, l >> b.l); }
#if COMPILER_GCC
	s32x4 operator/(s32x4 b) const { return {i / b.i, j / b.j, k / b.k, l / b.l}; }
	s32x4 operator%(s32x4 b) const { return {i % b.i, j % b.j, k % b.k, l % b.l}; }
#else
	s32x4 operator/(s32x4 b) const { return S32x4(_mm_div_epi32(m, b.m)); }
	s32x4 operator%(s32x4 b) const { return S32x4(_mm_rem_epi32(m, b.m)); }
#endif
	s32x4 operator<<(s32 b) const { return S32x4(_mm_slli_epi32(m, b)); }
	s32x4 operator>>(s32 b) const { return S32x4(_mm_srai_epi32(m, b)); }
	bool operator==(s32x4 b) const { return i == b.i && j == b.j && k == b.k && l == b.l; }
	MEMFUNS_BASIC(s32x4, s32, S32x4);
	MEMFUNS_INT(s32x4, s32, S32x4);
	MEMFUNS_DATA(s32);
};
constexpr s32x4 S32x4(__m128i m) {
	s32x4 r{};
	r.m = m;
	return r;
}
inline s32x4 S32x4(s32 v) { return S32x4(_mm_set1_epi32(v)); }
inline s32x4 S32x4(s32 i, s32 j, s32 k, s32 l) { return S32x4(_mm_setr_epi32(i, j, k, l)); }

union s32x8 {
	s32 v[8];
	__m256i m;
	s32x8 operator-() const { return S32x8(_mm256_sub_epi32(_mm256_setzero_si256(), m)); }
	s32x8 operator~() const { return *this ^ (~0); }
	s32x8 operator+(s32x8 b) const { return S32x8(_mm256_add_epi32(m, b.m)); }
	s32x8 operator-(s32x8 b) const { return S32x8(_mm256_sub_epi32(m, b.m)); }
	s32x8 operator*(s32x8 b) const { return S32x8(_mm256_mullo_epi32(m, b.m)); }
	s32x8 operator/(s32x8 b) const { return S32x8(_mm256_div_epi32(m, b.m)); }
	s32x8 operator%(s32x8 b) const { return S32x8(_mm256_rem_epi32(m, b.m)); }
	s32x8 operator^(s32x8 b) const { return S32x8(_mm256_xor_si256(m, b.m)); }
	s32x8 operator|(s32x8 b) const { return S32x8(_mm256_or_si256(m, b.m)); }
	s32x8 operator&(s32x8 b) const { return S32x8(_mm256_and_si256(m, b.m)); }
	s32x8 operator<<(s32 b) const { return S32x8(_mm256_slli_epi32(m, b)); }
	s32x8 operator>>(s32 b) const { return S32x8(_mm256_srai_epi32(m, b)); }
	s32x8 operator<<(s32x8 b) const {
		return S32x8(v[0] << b.v[0], v[1] << b.v[1], v[2] << b.v[2], v[3] << b.v[3], v[4] << b.v[4], v[5] << b.v[5],
					 v[6] << b.v[6], v[7] << b.v[7]);
	}
	s32x8 operator>>(s32x8 b) const {
		return S32x8(v[0] >> b.v[0], v[1] >> b.v[1], v[2] >> b.v[2], v[3] >> b.v[3], v[4] >> b.v[4], v[5] >> b.v[5],
					 v[6] >> b.v[6], v[7] >> b.v[7]);
	}
	bool operator==(s32x8 b) const {
		return v[0] == b.v[0] && v[1] == b.v[1] && v[2] == b.v[2] && v[3] == b.v[3] && v[4] == b.v[4] &&
			   v[5] == b.v[5] && v[6] == b.v[6] && v[7] == b.v[7];
	}
	MEMFUNS_BASIC(s32x8, s32, S32x8);
	MEMFUNS_INT(s32x8, s32, S32x8);
	MEMFUNS_DATA(s32);
};
constexpr s32x8 S32x8(__m256i m) {
	s32x8 r{};
	r.m = m;
	return r;
}
inline s32x8 S32x8(s32 v) { return S32x8(_mm256_set1_epi32(v)); }
inline s32x8 S32x8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h) {
	return S32x8(_mm256_setr_epi32(a, b, c, d, e, f, g, h));
}
inline s32x8 S32x8(s32 a, s32 b, s32 c, s32 d) { return S32x8(_mm256_setr_epi32(a, b, c, d, a, b, c, d)); }

union u32x4 {
	u32 v[4];
	struct {
		u32 i, j, k, l;
	};
	__m128i m;
	u32x4 operator~() const { return *this ^ (~0u); }
	u32x4 operator+(u32x4 b) const { return U32x4(_mm_add_epi32(m, b.m)); }
	u32x4 operator-(u32x4 b) const { return U32x4(_mm_sub_epi32(m, b.m)); }
	u32x4 operator*(u32x4 b) const { return {i * b.i, j * b.j, k * b.k, l * b.l}; }
#if COMPILER_GCC
	u32x4 operator/(u32x4 b) const { return {i / b.i, j / b.j, k / b.k, l / b.l}; }
	u32x4 operator%(u32x4 b) const { return {i % b.i, j % b.j, k % b.k, l % b.l}; }
#else
	u32x4 operator/(u32x4 b) const { return U32x4(_mm_div_epu32(m, b.m)); }
	u32x4 operator%(u32x4 b) const { return U32x4(_mm_rem_epu32(m, b.m)); }
#endif
	u32x4 operator^(u32x4 b) const { return U32x4(_mm_xor_si128(m, b.m)); }
	u32x4 operator|(u32x4 b) const { return U32x4(_mm_or_si128(m, b.m)); }
	u32x4 operator&(u32x4 b) const { return U32x4(_mm_and_si128(m, b.m)); }
	u32x4 operator<<(u32x4 b) const { return U32x4(i << b.i, j << b.j, k << b.k, l << b.l); }
	u32x4 operator>>(u32x4 b) const { return U32x4(i >> b.i, j >> b.j, k >> b.k, l >> b.l); }
	u32x4 operator<<(u32 b) const { return U32x4(_mm_slli_epi32(m, (s32)b)); }
	u32x4 operator>>(u32 b) const { return U32x4(_mm_srli_epi32(m, (s32)b)); }
	bool operator==(u32x4 b) const { return i == b.i && j == b.j && k == b.k && l == b.l; }
	MEMFUNS_BASIC(u32x4, u32, U32x4);
	MEMFUNS_INT(u32x4, u32, U32x4);
	MEMFUNS_DATA(u32);
};
constexpr u32x4 U32x4(__m128i m) {
	u32x4 r{};
	r.m = m;
	return r;
}
inline u32x4 U32x4(u32 i, u32 j, u32 k, u32 l) { return U32x4(_mm_setr_epi32((s32)i, (s32)j, (s32)k, (s32)l)); }
inline u32x4 U32x4(u32 v) { return U32x4(_mm_set1_epi32((s32)v)); }

union u32x8 {
	u32 v[8];
	__m256i m;
	u32x8 operator~() const { return *this ^ (~0u); }
	u32x8 operator+(u32x8 b) const { return U32x8(_mm256_add_epi32(m, b.m)); }
	u32x8 operator-(u32x8 b) const { return U32x8(_mm256_sub_epi32(m, b.m)); }
	u32x8 operator*(u32x8 b) const {
		return U32x8(v[0] * b.v[0], v[1] * b.v[1], v[2] * b.v[2], v[3] * b.v[3], v[4] * b.v[4], v[5] * b.v[5],
					 v[6] * b.v[6], v[7] * b.v[7]);
	}
	u32x8 operator/(u32x8 b) const { return U32x8(_mm256_div_epu32(m, b.m)); }
	u32x8 operator%(u32x8 b) const { return U32x8(_mm256_rem_epu32(m, b.m)); }
	u32x8 operator^(u32x8 b) const { return U32x8(_mm256_xor_si256(m, b.m)); }
	u32x8 operator|(u32x8 b) const { return U32x8(_mm256_or_si256(m, b.m)); }
	u32x8 operator&(u32x8 b) const { return U32x8(_mm256_and_si256(m, b.m)); }
	u32x8 operator<<(u32 b) const { return U32x8(_mm256_slli_epi32(m, (s32)b)); }
	u32x8 operator>>(u32 b) const { return U32x8(_mm256_srli_epi32(m, (s32)b)); }
	u32x8 operator<<(u32x8 b) const {
		return U32x8(v[0] << b.v[0], v[1] << b.v[1], v[2] << b.v[2], v[3] << b.v[3], v[4] << b.v[4], v[5] << b.v[5],
					 v[6] << b.v[6], v[7] << b.v[7]);
	}
	u32x8 operator>>(u32x8 b) const {
		return U32x8(v[0] >> b.v[0], v[1] >> b.v[1], v[2] >> b.v[2], v[3] >> b.v[3], v[4] >> b.v[4], v[5] >> b.v[5],
					 v[6] >> b.v[6], v[7] >> b.v[7]);
	}
	bool operator==(u32x8 b) const {
		return v[0] == b.v[0] && v[1] == b.v[1] && v[2] == b.v[2] && v[3] == b.v[3] && v[4] == b.v[4] &&
			   v[5] == b.v[5] && v[6] == b.v[6] && v[7] == b.v[7];
	}
	MEMFUNS_BASIC(u32x8, u32, U32x8);
	MEMFUNS_INT(u32x8, u32, U32x8);
	MEMFUNS_DATA(u32);
};
constexpr u32x8 U32x8(__m256i m) {
	u32x8 r{};
	r.m = m;
	return r;
}
inline u32x8 U32x8(u32 v) { return U32x8(_mm256_set1_epi32((s32)v)); }
inline u32x8 U32x8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) {
	return U32x8(_mm256_setr_epi32((s32)a, (s32)b, (s32)c, (s32)d, (s32)e, (s32)f, (s32)g, (s32)h));
}
inline u32x8 U32x8(u32 a, u32 b, u32 c, u32 d) {
	return U32x8(_mm256_setr_epi32((s32)a, (s32)b, (s32)c, (s32)d, (s32)a, (s32)b, (s32)c, (s32)d));
}

inline f32x4 F32x4(s32x4 v) { return F32x4(_mm_cvtepi32_ps(v.m)); }
inline f32x4 F32x4(u32x4 v) { return F32x4((f32)v.i, (f32)v.j, (f32)v.k, (f32)v.l); }
inline s32x4 S32x4(f32x4 v) { return S32x4(_mm_cvtps_epi32(v.m)); }
inline s32x4 S32x4(u32x4 v) { return S32x4(v.m); }
inline u32x4 U32x4(f32x4 v) { return U32x4((u32)v.i, (u32)v.j, (u32)v.k, (u32)v.l); }
inline u32x4 U32x4(s32x4 v) { return U32x4(v.m); }

inline f32x8 F32x8(s32x8 v) { return F32x8(_mm256_cvtepi32_ps(v.m)); }
inline f32x8 F32x8(u32x8 v) {
	return F32x8((f32)v.v[0], (f32)v.v[1], (f32)v.v[2], (f32)v.v[3], (f32)v.v[4], (f32)v.v[5], (f32)v.v[6],
				 (f32)v.v[7]);
}
inline s32x8 S32x8(f32x8 v) { return S32x8(_mm256_cvtps_epi32(v.m)); }
inline s32x8 S32x8(u32x8 v) { return S32x8(v.m); }
inline u32x8 U32x8(f32x8 v) {
	return U32x8((u32)v.v[0], (u32)v.v[1], (u32)v.v[2], (u32)v.v[3], (u32)v.v[4], (u32)v.v[5], (u32)v.v[6],
				 (u32)v.v[7]);
}
inline u32x8 U32x8(s32x8 v) { return U32x8(v.m); }

constexpr f32 _maskF32(bool v) {
	u32 r = v ? ~0u : 0u;
	return *(f32*)&r;
}

#define UOP2(op, ty, con) \
	ty operator op() const { return {op x, op y}; }
#define UOP3(op, ty, con) \
	ty operator op() const { return {op x, op y, op z}; }
#define UOP4(op, ty, con) \
	ty operator op() const { return {op x, op y, op z, op w}; }
#define UOP4M(op, ty, con) \
	ty operator op() const { return con(op m); }

#define UOP2C(op, ty, con) constexpr UOP2(op, ty, con)
#define UOP3C(op, ty, con) constexpr UOP3(op, ty, con)
#define UOP4C(op, ty, con) constexpr UOP4(op, ty, con)

#define UOP2P(op, ty, con) UOP2(op, ty, con)
#define UOP3P(op, ty, con) UOP3(op, ty, con)
#define UOP4P(op, ty, con) UOP4(op, ty, con)

#define BOP2(op, ty, con) \
	ty operator op(ty b) const { return {x op b.x, y op b.y}; }
#define BOP3(op, ty, con) \
	ty operator op(ty b) const { return {x op b.x, y op b.y, z op b.z}; }
#define BOP4(op, ty, con) \
	ty operator op(ty b) const { return {x op b.x, y op b.y, z op b.z, w op b.w}; }

#define BOP2P(op, ty, con) BOP2(op, ty, con)
#define BOP3P(op, ty, con) BOP3(op, ty, con)
#define BOP4P(op, ty, con) BOP4(op, ty, con)

#define BOP4M(op, ty, con) \
	ty operator op(ty b) const { return con(m op b.m); }

#define BOP2CMP(op, ty, con) \
	ty operator op(ty b) const { return {_maskF32(x op b.x), _maskF32(y op b.y)}; }
#define BOP3CMP(op, ty, con) \
	ty operator op(ty b) const { return {_maskF32(x op b.x), _maskF32(y op b.y), _maskF32(z op b.z)}; }
#define BOP4CMP(op, ty, con)                                                                     \
	ty operator op(ty b) const {                                                                 \
		return {_maskF32(x op b.x), _maskF32(y op b.y), _maskF32(z op b.z), _maskF32(w op b.w)}; \
	}

#define BOP2PCMP BOP2P
#define BOP3PCMP BOP2P
#define BOP4PCMP BOP2P

#define BOP4MCMP(op, ty, con) \
	ty operator op(ty b) const { return con(m op b.m); }

#define BOP2C(op, ty, con) constexpr BOP2(op, ty, con)
#define BOP3C(op, ty, con) constexpr BOP3(op, ty, con)
#define BOP4C(op, ty, con) constexpr BOP4(op, ty, con)

#define BOP2CCMP(op, ty, con) constexpr BOP2CMP(op, ty, con)
#define BOP3CCMP(op, ty, con) constexpr BOP3CMP(op, ty, con)
#define BOP4CCMP(op, ty, con) constexpr BOP4CMP(op, ty, con)

#define BOPSF32_(bop, uop, cmp, ty, con)                                                                            \
	uop(-, ty, con) bop(+, ty, con) bop(-, ty, con) bop(*, ty, con) bop(/, ty, con) cmp(<, ty, con) cmp(>, ty, con) \
		cmp(<=, ty, con) cmp(>=, ty, con)
#define BOPSU32_(bop, uop, ty, con)                                                                                 \
	bop(+, ty, con) bop(-, ty, con) bop(*, ty, con) bop(/, ty, con) bop(%, ty, con) bop(^, ty, con) bop(|, ty, con) \
		bop(&, ty, con) bop(<<, ty, con) bop(>>, ty, con) uop(~, ty, con)
#define BOPSS32_(bop, uop, ty, con) BOPSU32_(bop, uop, ty, con) uop(-, ty, con)

#define BOPSF32(op, ty, con) BOPSF32_(CONCAT(B, op), CONCAT(U, op), CONCAT(CONCAT(B, op), CMP), ty, con)
#define BOPSS32(op, ty, con) BOPSS32_(CONCAT(B, op), CONCAT(U, op), ty, con)
#define BOPSU32(op, ty, con) BOPSU32_(CONCAT(B, op), CONCAT(U, op), ty, con)

#define CVT(v2s, v2u)              \
	explicit operator v2s() const; \
	explicit operator v2u() const

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_DATA(scl)
union v2 {
	MEMBERS2(1, f32, f32);
	BOPSF32(OP2C, v2, V2);
	MEMFUNS(v2, f32, V2);
	CVT(v2s, v2u);
	constexpr bool operator==(v2 b) const { return x == b.x && y == b.y; }
};
union v3 {
	MEMBERS3(1, f32, f32, v2);
	BOPSF32(OP3C, v3, V3);
	MEMFUNS(v3, f32, V3);
	CVT(v3s, v3u);
	bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4 {
	MEMBERS4(1, f32, f32, v2, v3);
	BOPSF32(OP4M, v4, V4);
	MEMFUNS(v4, f32, V4);
	CVT(v4s, v4u);
	f32x4 m;
	bool operator==(v4 b) const { return m == b.m; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) \
	MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL(vec, scl, con)
union v2s {
	MEMBERS2(1, s32, s32);
	BOPSS32(OP2C, v2s, V2s);
	MEMFUNS(v2s, s32, V2s);
	CVT(v2, v2u);
	bool operator==(v2s b) const { return x == b.x && y == b.y; }
};
union v3s {
	MEMBERS3(1, s32, s32, v2s);
	BOPSS32(OP3C, v3s, V3s);
	MEMFUNS(v3s, s32, V3s);
	CVT(v3, v3u);
	bool operator==(v3s b) const { return x == b.x && y == b.y && z == b.z; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) \
	MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL_M(vec, scl, con)
union v4s {
	MEMBERS4(1, s32, s32, v2s, v3s);
	BOPSS32(OP4M, v4s, V4s);
	MEMFUNS(v4s, s32, V4s);
	CVT(v4, v4u);
	s32x4 m;
	bool operator==(v4s b) const { return m == b.m; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) \
	MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL(vec, scl, con)
union v2u {
	MEMBERS2(1, u32, u32);
	BOPSU32(OP2C, v2u, V2u);
	MEMFUNS(v2u, u32, V2u);
	CVT(v2, v2s);
	bool operator==(v2u b) const { return x == b.x && y == b.y; }
};
union v3u {
	MEMBERS3(1, u32, u32, v2u);
	BOPSU32(OP3C, v3u, V3u);
	MEMFUNS(v3u, u32, V3u);
	CVT(v3, v3s);
	bool operator==(v3u b) const { return x == b.x && y == b.y && z == b.z; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) \
	MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL_M(vec, scl, con)
union v4u {
	MEMBERS4(1, u32, u32, v2u, v3u);
	BOPSU32(OP4M, v4u, V4u);
	MEMFUNS(v4u, u32, V4u);
	CVT(v4, v4s);
	u32x4 m;
	bool operator==(v4u b) const { return m == b.m; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con, f32x4) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_BASIC_SCL(vec, f32x4, con)
union v2x4 {
	MEMBERS2(4, f32, f32x4);
	BOPSF32(OP2P, v2x4, V2x4);
	MEMFUNS(v2x4, f32, V2x4, f32x4);
	bool operator==(v2x4 b) const { return x == b.x && y == b.y; }
};
union v3x4 {
	MEMBERS3(4, f32, f32x4, v2x4);
	BOPSF32(OP3P, v3x4, V3x4);
	MEMFUNS(v3x4, f32, V3x4, f32x4);
	bool operator==(v3x4 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4x4 {
	MEMBERS4(4, f32, f32x4, v2x4, v3x4);
	BOPSF32(OP4P, v4x4, V4x4);
	MEMFUNS(v4x4, f32, V4x4, f32x4);
	bool operator==(v4x4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v2sx4 {
	MEMBERS2(4, s32, s32x4);
	BOPSS32(OP2P, v2sx4, V2sx4);
	MEMFUNS(v2sx4, s32, V2sx4);
	MEMFUNS_SHIFT_SCL_2(v2sx4, s32)
	bool operator==(v2sx4 b) const { return x == b.x && y == b.y; }
};
union v3sx4 {
	MEMBERS3(4, s32, s32x4, v2sx4);
	BOPSS32(OP3P, v3sx4, V3sx4);
	MEMFUNS(v3sx4, s32, V3sx4);
	MEMFUNS_SHIFT_SCL_3(v3sx4, s32)
	bool operator==(v3sx4 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4sx4 {
	MEMBERS4(4, s32, s32x4, v2sx4, v3sx4);
	BOPSS32(OP4P, v4sx4, V4sx4);
	MEMFUNS(v4sx4, s32, V4sx4);
	MEMFUNS_SHIFT_SCL_4(v4sx4, s32)
	bool operator==(v4sx4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v2ux4 {
	MEMBERS2(4, u32, u32x4);
	BOPSU32(OP2P, v2ux4, V2ux4);
	MEMFUNS(v2ux4, u32, V2ux4);
	MEMFUNS_SHIFT_SCL_2(v2ux4, u32)
	bool operator==(v2ux4 b) const { return x == b.x && y == b.y; }
};
union v3ux4 {
	MEMBERS3(4, u32, u32x4, v2ux4);
	BOPSU32(OP3P, v3ux4, V3ux4);
	MEMFUNS(v3ux4, u32, V3ux4);
	MEMFUNS_SHIFT_SCL_3(v3ux4, u32)
	bool operator==(v3ux4 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4ux4 {
	MEMBERS4(4, u32, u32x4, v2ux4, v3ux4);
	BOPSU32(OP4P, v4ux4, V4ux4);
	MEMFUNS(v4ux4, u32, V4ux4);
	MEMFUNS_SHIFT_SCL_4(v4ux4, u32)
	bool operator==(v4ux4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)
union v2x8 {
	MEMBERS2(8, f32, f32x8);
	BOPSF32(OP2P, v2x8, V2x8);
	MEMFUNS(v2x8, f32, V2x8);
	MEMFUNS_BASIC_SCL(v2x8, f32x8, V2x8)
	bool operator==(v2x8 b) const { return x == b.x && y == b.y; }
};
union v3x8 {
	MEMBERS3(8, f32, f32x8, v2x8);
	BOPSF32(OP3P, v3x8, V3x8);
	MEMFUNS(v3x8, f32, V3x8);
	MEMFUNS_BASIC_SCL(v3x8, f32x8, V3x8)
	bool operator==(v3x8 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4x8 {
	MEMBERS4(8, f32, f32x8, v2x8, v3x8);
	BOPSF32(OP4P, v4x8, V4x8);
	MEMFUNS(v4x8, f32, V4x8);
	MEMFUNS_BASIC_SCL(v4x8, f32x8, V4x8)
	bool operator==(v4x8 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v2sx8 {
	MEMBERS2(8, s32, s32x8);
	BOPSS32(OP2P, v2sx8, V2sx8);
	MEMFUNS(v2sx8, s32, V2sx8);
	v2sx8 operator<<(s32 b) const { return {x << b, y << b}; }
	v2sx8 operator>>(s32 b) const { return {x >> b, y >> b}; }
	bool operator==(v2sx8 b) const { return x == b.x && y == b.y; }
};
union v3sx8 {
	MEMBERS3(8, s32, s32x8, v2sx8);
	BOPSS32(OP3P, v3sx8, V3sx8);
	MEMFUNS(v3sx8, s32, V3sx8);
	v3sx8 operator<<(s32 b) const { return {x << b, y << b, z << b}; }
	v3sx8 operator>>(s32 b) const { return {x >> b, y >> b, z >> b}; }
	bool operator==(v3sx8 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4sx8 {
	MEMBERS4(8, s32, s32x8, v2sx8, v3sx8);
	BOPSS32(OP4P, v4sx8, V4sx8);
	MEMFUNS(v4sx8, s32, V4sx8);
	v4sx8 operator<<(s32 b) const { return {x << b, y << b, z << b, w << b}; }
	v4sx8 operator>>(s32 b) const { return {x >> b, y >> b, z >> b, w >> b}; }
	bool operator==(v4sx8 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v2ux8 {
	MEMBERS2(8, u32, u32x8);
	BOPSU32(OP2P, v2ux8, V2ux8);
	MEMFUNS(v2ux8, u32, V2ux8);
	v2ux8 operator<<(u32 b) const { return {x << b, y << b}; }
	v2ux8 operator>>(u32 b) const { return {x >> b, y >> b}; }
	bool operator==(v2ux8 b) const { return x == b.x && y == b.y; }
};
union v3ux8 {
	MEMBERS3(8, u32, u32x8, v2ux8);
	BOPSU32(OP3P, v3ux8, V3ux8);
	MEMFUNS(v3ux8, u32, V3ux8);
	v3ux8 operator<<(u32 b) const { return {x << b, y << b, z << b}; }
	v3ux8 operator>>(u32 b) const { return {x >> b, y >> b, z >> b}; }
	bool operator==(v3ux8 b) const { return x == b.x && y == b.y && z == b.z; }
};
union v4ux8 {
	MEMBERS4(8, u32, u32x8, v2ux8, v3ux8);
	BOPSU32(OP4P, v4ux8, V4ux8);
	MEMFUNS(v4ux8, u32, V4ux8);
	v4ux8 operator<<(u32 b) const { return {x << b, y << b, z << b, w << b}; }
	v4ux8 operator>>(u32 b) const { return {x >> b, y >> b, z >> b, w >> b}; }
	bool operator==(v4ux8 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
};
#undef MEMFUNS

union m3;
m3 transpose(m3 const&);

union m3 {
	struct {
		v3 i, j, k;
	};
	f32 v[9];
	v3 operator*(v3 b) const {
		v3 x = V3(b.x) * i;
		v3 y = V3(b.y) * j;
		v3 z = V3(b.z) * k;
		return x + y + z;
	}
	m3 operator*(m3 b) const { return {*this * b.i, *this * b.j, *this * b.k}; }
	m3& operator*=(m3 b) { return *this = *this * b; }
	static m3 scaling(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0,
			0, y, 0,
			0, 0, z,
		};
		// clang-format on
	}
	static m3 scaling(v3 v) { return scaling(v.x, v.y, v.z); }
	static m3 scaling(v2 xy, f32 z) { return scaling(xy.x, xy.y, z); }
	static m3 scaling(f32 v) { return scaling(v, v, v); }
	static m3 translation(f32 x, f32 y) {
		// clang-format off
		return {
			1, 0, 0,
			0, 1, 0,
			x, y, 1,
		};
		// clang-format on
	}
	static m3 translation(v2 v) { return translation(v.x, v.y); }
	static m3 translation(f32 v) { return translation(v, v); }
	static m3 rotationX(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			1, 0, 0,
			0, c, s,
			0,-s, c,
		};
		// clang-format on
	}
	static m3 rotationY(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, 0, s,
			0, 1, 0,
		   -s, 0, c,
		};
		// clang-format on
	}
	static m3 rotationZ(f32 a) {
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

m3 transpose(m3 const& m) {
	return {
		m.i.x, m.j.x, m.k.x, m.i.y, m.j.y, m.k.y, m.i.z, m.j.z, m.k.z,
	};
}

union m4 {
	struct {
		v4 i, j, k, l;
	};
	__m128 m[4];
	f32 v[16];
	v4 operator*(v4 b) const {
		v4 x = V4(b.x) * i;
		v4 y = V4(b.y) * j;
		v4 z = V4(b.z) * k;
		v4 w = V4(b.w) * l;
		return x + y + z + w;
	}
	v3 operator*(v3 b) const {
		auto x = V4(b.x) * i;
		auto y = V4(b.y) * j;
		auto z = V4(b.z) * k;
		auto r = x + y + z;
		v3 result;
		memcpy(&result, &r, 12);
		return result;
	}
	m4 operator*(m4 b) const { return {*this * b.i, *this * b.j, *this * b.k, *this * b.l}; }
	m4& operator*=(m4 b) { return *this = *this * b; }
	static m4 scaling(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static m4 identity() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }
	static m4 scaling(v3 v) { return scaling(v.x, v.y, v.z); }
	static m4 scaling(v2 xy, f32 z) { return scaling(xy.x, xy.y, z); }
	static m4 scaling(f32 v) { return scaling(v, v, v); }
	static m4 translation(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1,
		};
		// clang-format on
	}
	static m4 translation(v3 v) { return translation(v.x, v.y, v.z); }
	static m4 translation(v2 xy, f32 z) { return translation(xy.x, xy.y, z); }
	static m4 translation(f32 v) { return translation(v, v, v); }
	static m4 perspective(f32 aspect, f32 fov, f32 nz, f32 fz) {
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
	static m4 rotationX(f32 a) {
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
	static m4 rotationY(f32 a) {
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
	static m4 rotationZ(f32 a) {
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
	static m4 rotationZXY(v3 v) {
		v3 sin;
		v3 cos;
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
	static m4 rotationYXZ(v3 v) {
		v3 sin;
		v3 cos;
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
#undef MEMFUNS

#define CVT(v2, v2s, s32, v2u, u32)                       \
	v2::operator v2s() const { return {(s32)x, (s32)y}; } \
	v2::operator v2u() const { return {(u32)x, (u32)y}; }
CVT(v2, v2s, s32, v2u, u32);
CVT(v2s, v2, f32, v2u, u32);
CVT(v2u, v2, f32, v2s, s32);
#undef CVT

#define CVT(v3, v3s, s32, v3u, u32)                               \
	v3::operator v3s() const { return {(s32)x, (s32)y, (s32)z}; } \
	v3::operator v3u() const { return {(u32)x, (u32)y, (u32)z}; }
CVT(v3, v3s, s32, v3u, u32);
CVT(v3s, v3, f32, v3u, u32);
CVT(v3u, v3, f32, v3s, s32);
#undef CVT

#define CVT(v4, v4s, s32, v4u, u32)                                       \
	v4::operator v4s() const { return {(s32)x, (s32)y, (s32)z, (s32)w}; } \
	v4::operator v4u() const { return {(u32)x, (u32)y, (u32)z, (u32)w}; }
CVT(v4, v4s, s32, v4u, u32);
CVT(v4s, v4, f32, v4u, u32);
CVT(v4u, v4, f32, v4s, s32);
#undef CVT

#define V2(f32, v2, V2, v2s, v2u)                           \
	constexpr v2 V2(f32 x, f32 y) { return {x, y}; }        \
	constexpr v2 V2(f32 v) { return {v, v}; }               \
	constexpr v2 V2(v2s v) { return {(f32)v.x, (f32)v.y}; } \
	constexpr v2 V2(v2u v) { return {(f32)v.x, (f32)v.y}; }
V2(f32, v2, V2, v2s, v2u);
V2(s32, v2s, V2s, v2, v2u);
V2(u32, v2u, V2u, v2, v2s);
#undef V2

#define V3(f32, v2, v3, V3, v3s, v3u)                                   \
	constexpr v3 V3(f32 x, f32 y, f32 z) { return {x, y, z}; }          \
	constexpr v3 V3(f32 v) { return {v, v, v}; }                        \
	constexpr v3 V3(v2 xy, f32 z) { return {xy.x, xy.y, z}; }           \
	constexpr v3 V3(f32 x, v2 yz) { return {x, yz.x, yz.y}; }           \
	constexpr v3 V3(v3s v) { return V3((f32)v.x, (f32)v.y, (f32)v.z); } \
	constexpr v3 V3(v3u v) { return V3((f32)v.x, (f32)v.y, (f32)v.z); }

V3(f32, v2, v3, V3, v3s, v3u);
V3(s32, v2s, v3s, V3s, v3, v3u);
V3(u32, v2u, v3u, V3u, v3, v3s);
#undef V3

#define V4(f32, v2, v3, v4, V4, v4x4, __m128, v4s, v4u, F32x4)           \
	constexpr v4 V4(f32 x, f32 y, f32 z, f32 w) { return {x, y, z, w}; } \
	constexpr v4 V4(f32 v) { return {v, v, v, v}; }                      \
	constexpr v4 V4(v2 xy, v2 zw) { return {xy.x, xy.y, zw.x, zw.y}; }   \
	constexpr v4 V4(v2 xy, f32 z, f32 w) { return {xy.x, xy.y, z, w}; }  \
	constexpr v4 V4(f32 x, f32 y, v2 zw) { return {x, y, zw.x, zw.y}; }  \
	constexpr v4 V4(v3 xyz, f32 w) { return {xyz.x, xyz.y, xyz.z, w}; }  \
	constexpr v4 V4(f32 x, v3 yzw) { return {x, yzw.x, yzw.y, yzw.z}; }  \
	constexpr v4 V4(v4x4 v) {                                            \
		v4 r{};                                                          \
		r.m = v;                                                         \
		return r;                                                        \
	}                                                                    \
	constexpr v4 V4(__m128 m) {                                          \
		v4 r{};                                                          \
		r.m.m = m;                                                       \
		return r;                                                        \
	}                                                                    \
	inline v4 V4(v4s v) { return V4(F32x4(v.m)); }                       \
	inline v4 V4(v4u v) { return V4(F32x4(v.m)); }

V4(f32, v2, v3, v4, V4, f32x4, __m128, v4s, v4u, F32x4);
V4(s32, v2s, v3s, v4s, V4s, s32x4, __m128i, v4, v4u, S32x4);
V4(u32, v2u, v3u, v4u, V4u, u32x4, __m128i, v4, v4s, U32x4);
#undef V4

// clang-format off
#define C(v4, V4) constexpr v4 V4(v4 v) { return v; } template<> constexpr v4 cvt<v4, v4>(v4 v) { return v; }
C(v2, V2) C(v2s, V2s) C(v2u, V2u)
C(v3, V3) C(v3s, V3s) C(v3u, V3u)
C(v4, V4) C(v4s, V4s) C(v4u, V4u)
C(v2x4, V2x4) C(v2sx4, V2sx4) C(v2ux4, V2ux4)
C(v3x4, V3x4) C(v3sx4, V3sx4) C(v3ux4, V3ux4)
C(v4x4, V4x4) C(v4sx4, V4sx4) C(v4ux4, V4ux4)
C(v2x8, V2x8) C(v2sx8, V2sx8) C(v2ux8, V2ux8)
C(v3x8, V3x8) C(v3sx8, V3sx8) C(v3ux8, V3ux8)
C(v4x8, V4x8) C(v4sx8, V4sx8) C(v4ux8, V4ux8)
#undef C
// clang-format on

#define V2X4(v2x4, V2x4, v2, f32, f32x4, F32x4, v2sx4, v2ux4)                          \
	inline v2x4 V2x4(f32 ix, f32 iy, f32 jx, f32 jy, f32 kx, f32 ky, f32 lx, f32 ly) { \
		return {                                                                       \
			F32x4(ix, jx, kx, lx),                                                     \
			F32x4(iy, jy, ky, ly),                                                     \
		};                                                                             \
	}                                                                                  \
	inline v2x4 V2x4(v2 i, v2 j, v2 k, v2 l) {                                         \
		return {                                                                       \
			F32x4(i.x, j.x, k.x, l.x),                                                 \
			F32x4(i.y, j.y, k.y, l.y),                                                 \
		};                                                                             \
	}                                                                                  \
	constexpr v2x4 V2x4(f32x4 x, f32x4 y) { return {x, y}; }                           \
	inline v2x4 V2x4(f32 v) { return {F32x4(v), F32x4(v)}; }                           \
	constexpr v2x4 V2x4(f32x4 v) { return {v, v}; }                                    \
	inline v2x4 V2x4(v2 v) { return {F32x4(v.x), F32x4(v.y)}; }                        \
	inline v2x4 V2x4(v2sx4 v) { return V2x4(F32x4(v.x), F32x4(v.y)); }                 \
	inline v2x4 V2x4(v2ux4 v) { return V2x4(F32x4(v.x), F32x4(v.y)); }

				V2X4(v2x4, V2x4, v2, f32, f32x4, F32x4, v2sx4, v2ux4);
V2X4(v2sx4, V2sx4, v2s, s32, s32x4, S32x4, v2x4, v2ux4);
V2X4(v2ux4, V2ux4, v2u, u32, u32x4, U32x4, v2x4, v2sx4);
#undef V2X4

#define V3X4(v3x4, V3x4, v3, f32, f32x4, F32x4, v3sx4, v3ux4)                                                          \
	inline v3x4 V3x4(f32 ix, f32 iy, f32 iz, f32 jx, f32 jy, f32 jz, f32 kx, f32 ky, f32 kz, f32 lx, f32 ly, f32 lz) { \
		return {                                                                                                       \
			F32x4(ix, jx, kx, lx),                                                                                     \
			F32x4(iy, jy, ky, ly),                                                                                     \
			F32x4(iz, jz, kz, lz),                                                                                     \
		};                                                                                                             \
	}                                                                                                                  \
	inline v3x4 V3x4(v3 i, v3 j, v3 k, v3 l) {                                                                         \
		return {                                                                                                       \
			F32x4(i.x, j.x, k.x, l.x),                                                                                 \
			F32x4(i.y, j.y, k.y, l.y),                                                                                 \
			F32x4(i.z, j.z, k.z, l.z),                                                                                 \
		};                                                                                                             \
	}                                                                                                                  \
	constexpr v3x4 V3x4(f32x4 x, f32x4 y, f32x4 z) { return {x, y, z}; }                                               \
	inline v3x4 V3x4(f32 v) { return {F32x4(v), F32x4(v), F32x4(v)}; }                                                 \
	constexpr v3x4 V3x4(f32x4 v) { return {v, v, v}; }                                                                 \
	inline v3x4 V3x4(v3 v) { return {F32x4(v.x), F32x4(v.y), F32x4(v.z)}; }                                            \
	inline v3x4 V3x4(v3sx4 v) { return V3x4(F32x4(v.x), F32x4(v.y), F32x4(v.z)); }                                     \
	inline v3x4 V3x4(v3ux4 v) { return V3x4(F32x4(v.x), F32x4(v.y), F32x4(v.z)); }

V3X4(v3x4, V3x4, v3, f32, f32x4, F32x4, v3sx4, v3ux4);
V3X4(v3sx4, V3sx4, v3s, s32, s32x4, S32x4, v3x4, v3ux4);
V3X4(v3ux4, V3ux4, v3u, u32, u32x4, U32x4, v3x4, v3sx4);
#undef V3X4

#define V4X4(v4x4, V4x4, v4, f32, f32x4, F32x4, v4sx4, v4ux4)                                                        \
	inline v4x4 V4x4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, \
					 f32 lx, f32 ly, f32 lz, f32 lw) {                                                               \
		return {F32x4(ix, jx, kx, lx), F32x4(iy, jy, ky, ly), F32x4(iz, jz, kz, lz), F32x4(iw, jw, kw, lw)};         \
	}                                                                                                                \
	inline v4x4 V4x4(v4 i, v4 j, v4 k, v4 l) {                                                                       \
		return {F32x4(i.x, j.x, k.x, l.x), F32x4(i.y, j.y, k.y, l.y), F32x4(i.z, j.z, k.z, l.z),                     \
				F32x4(i.w, j.w, k.w, l.w)};                                                                          \
	}                                                                                                                \
	constexpr v4x4 V4x4(f32x4 x, f32x4 y, f32x4 z, f32x4 w) { return {x, y, z, w}; }                                 \
	inline v4x4 V4x4(f32 v) { return {F32x4(v), F32x4(v), F32x4(v), F32x4(v)}; }                                     \
	constexpr v4x4 V4x4(f32x4 v) { return {v, v, v, v}; }                                                            \
	inline v4x4 V4x4(v4 v) { return {F32x4(v.x), F32x4(v.y), F32x4(v.z), F32x4(v.w)}; }                              \
	inline v4x4 V4x4(v4sx4 v) { return V4x4(F32x4(v.x), F32x4(v.y), F32x4(v.z), F32x4(v.w)); }                       \
	inline v4x4 V4x4(v4ux4 v) { return V4x4(F32x4(v.x), F32x4(v.y), F32x4(v.z), F32x4(v.w)); }

V4X4(v4x4, V4x4, v4, f32, f32x4, F32x4, v4sx4, v4ux4);
V4X4(v4sx4, V4sx4, v4s, s32, s32x4, S32x4, v4x4, v4ux4);
V4X4(v4ux4, V4ux4, v4u, u32, u32x4, U32x4, v4x4, v4sx4);
#undef V4X4

#define V2X8(v2x8, V2x8, v2, V4x2, f32, f32x8, F32x8, v2sx8, v2ux8)                                                  \
	constexpr v2x8 V2x8(f32x8 x, f32x8 y) { return {x, y}; }                                                         \
	inline v2x8 V2x8(f32 ix, f32 iy, f32 jx, f32 jy, f32 kx, f32 ky, f32 lx, f32 ly, f32 ax, f32 ay, f32 bx, f32 by, \
					 f32 cx, f32 cy, f32 dx, f32 dy) {                                                               \
		return V2x8(F32x8(ix, jx, kx, lx, ax, bx, cx, dx), F32x8(iy, jy, ky, ly, ay, by, cy, dy));                   \
	}                                                                                                                \
	inline v2x8 V2x8(v2 i, v2 j, v2 k, v2 l, v2 a, v2 b, v2 c, v2 d) {                                               \
		return V2x8(F32x8(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x), F32x8(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y));   \
	}                                                                                                                \
	inline v2x8 V2x8(f32 v) { return V2x8(F32x8(v), F32x8(v)); }                                                     \
	constexpr v2x8 V2x8(f32x8 v) { return V2x8(v, v); }                                                              \
	inline v2x8 V2x8(v2 v) { return V2x8(F32x8(v.x), F32x8(v.y)); }                                                  \
	inline v2x8 V2x8(v2sx8 v) { return V2x8(F32x8(v.x), F32x8(v.y)); }                                               \
	inline v2x8 V2x8(v2ux8 v) { return V2x8(F32x8(v.x), F32x8(v.y)); }

V2X8(v2x8, V2x8, v2, V4x2, f32, f32x8, F32x8, v2sx8, v2ux8);
V2X8(v2sx8, V2sx8, v2s, V4sx2, s32, s32x8, S32x8, v2x8, v2ux8);
V2X8(v2ux8, V2ux8, v2u, V4ux2, u32, u32x8, U32x8, v2x8, v2sx8);
#undef V2X8

#define V3X8(v3x8, V3x8, v3, V3x2, f32, f32x8, F32x8, v3sx8, v3ux8)                                                    \
	constexpr v3x8 V3x8(f32x8 x, f32x8 y, f32x8 z) { return {x, y, z}; }                                               \
	inline v3x8 V3x8(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz, f32 cx, f32 cy, f32 cz, f32 dx, f32 dy, f32 dz,   \
					 f32 ex, f32 ey, f32 ez, f32 fx, f32 fy, f32 fz, f32 gx, f32 gy, f32 gz, f32 hx, f32 hy, f32 hz) { \
		return V3x8(F32x8(ax, bx, cx, dx, ex, fx, gx, hx), F32x8(ay, by, cy, dy, ey, fy, gy, hy),                      \
					F32x8(az, bz, cz, dz, ez, fz, gz, hz));                                                            \
	}                                                                                                                  \
	inline v3x8 V3x8(v3 i, v3 j, v3 k, v3 l, v3 a, v3 b, v3 c, v3 d) {                                                 \
		return V3x8(F32x8(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x), F32x8(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y),      \
					F32x8(i.z, j.z, k.z, l.z, a.z, b.z, c.z, d.z));                                                    \
	}                                                                                                                  \
	inline v3x8 V3x8(f32 v) { return V3x8(F32x8(v), F32x8(v), F32x8(v)); }                                             \
	constexpr v3x8 V3x8(f32x8 v) { return V3x8(v, v, v); }                                                             \
	inline v3x8 V3x8(v3 v) { return V3x8(F32x8(v.x), F32x8(v.y), F32x8(v.z)); }                                        \
	inline v3x8 V3x8(v3sx8 v) { return V3x8(F32x8(v.x), F32x8(v.y), F32x8(v.z)); }                                     \
	inline v3x8 V3x8(v3ux8 v) { return V3x8(F32x8(v.x), F32x8(v.y), F32x8(v.z)); }

V3X8(v3x8, V3x8, v3, V3x2, f32, f32x8, F32x8, v3sx8, v3ux8);
V3X8(v3sx8, V3sx8, v3s, V3sx2, s32, s32x8, S32x8, v3x8, v3ux8);
V3X8(v3ux8, V3ux8, v3u, V3ux2, u32, u32x8, U32x8, v3x8, v3sx8);
#undef V3X8

#define V4X8(v4x8, V4x8, v4, V4x2, f32, f32x8, F32x8, v4sx8, v4ux8)                                                  \
	constexpr v4x8 V4x8(f32x8 x, f32x8 y, f32x8 z, f32x8 w) { return {x, y, z, w}; }                                 \
	inline v4x8 V4x8(f32 ax, f32 ay, f32 az, f32 aw, f32 bx, f32 by, f32 bz, f32 bw, f32 cx, f32 cy, f32 cz, f32 cw, \
					 f32 dx, f32 dy, f32 dz, f32 dw, f32 ex, f32 ey, f32 ez, f32 ew, f32 fx, f32 fy, f32 fz, f32 fw, \
					 f32 gx, f32 gy, f32 gz, f32 gw, f32 hx, f32 hy, f32 hz, f32 hw) {                               \
		return V4x8(F32x8(ax, bx, cx, dx, ex, fx, gx, hx), F32x8(ay, by, cy, dy, ey, fy, gy, hy),                    \
					F32x8(az, bz, cz, dz, ez, fz, gz, hz), F32x8(aw, bw, cw, dw, ew, fw, gw, hw));                   \
	}                                                                                                                \
	inline v4x8 V4x8(v4 i, v4 j, v4 k, v4 l, v4 a, v4 b, v4 c, v4 d) {                                               \
		return V4x8(F32x8(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x), F32x8(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y),    \
					F32x8(i.z, j.z, k.z, l.z, a.z, b.z, c.z, d.z), F32x8(i.w, j.w, k.w, l.w, a.w, b.w, c.w, d.w));   \
	}                                                                                                                \
	inline v4x8 V4x8(f32 v) { return V4x8(F32x8(v), F32x8(v), F32x8(v), F32x8(v)); }                                 \
	constexpr v4x8 V4x8(f32x8 v) { return V4x8(v, v, v, v); }                                                        \
	inline v4x8 V4x8(v4 v) { return V4x8(F32x8(v.x), F32x8(v.y), F32x8(v.z), F32x8(v.w)); }                          \
	inline v4x8 V4x8(v4sx8 v) { return V4x8(F32x8(v.x), F32x8(v.y), F32x8(v.z), F32x8(v.w)); }                       \
	inline v4x8 V4x8(v4ux8 v) { return V4x8(F32x8(v.x), F32x8(v.y), F32x8(v.z), F32x8(v.w)); }

V4X8(v4x8, V4x8, v4, V4x2, f32, f32x8, F32x8, v4sx8, v4ux8);
V4X8(v4sx8, V4sx8, v4s, V4sx2, s32, s32x8, S32x8, v4x8, v4ux8);
V4X8(v4ux8, V4ux8, v4u, V4ux2, u32, u32x8, U32x8, v4x8, v4sx8);
#undef V4X8

constexpr m4 M4(f32 v = 0.0f) { return m4{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; }
constexpr m4 M4(v4 i, v4 j, v4 k, v4 l) { return m4{i, j, k, l}; }
constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l) {
	m4 r{};
	r.m[0] = i;
	r.m[1] = j;
	r.m[2] = k;
	r.m[3] = l;
	return r;
}
constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw) {
	return {ix, iy, iz, iw, jx, jy, jz, jw, kx, ky, kz, kw, lx, ly, lz, lw};
}

m4 M4(m3 v) {
	return {
		V4(v.i, 0),
		V4(v.j, 0),
		V4(v.k, 0),
		V4(0, 0, 0, 1),
	};
}

namespace CE {
#define V2x4(v2, v2x4, V2x4)                                                                         \
	constexpr v2x4 V2x4(v2 a, v2 b, v2 c, v2 d) { return {a.x, b.x, c.x, d.x, a.y, b.y, c.y, d.y}; } \
	constexpr v2x4 V2x4(v2 v) { return CE::V2x4(v, v, v, v); }
V2x4(v2, v2x4, V2x4) V2x4(v2s, v2sx4, V2sx4) V2x4(v2u, v2ux4, V2ux4);
#undef V2x4
#define V3x4(v3, v3x4, V3x4)                                                 \
	constexpr v3x4 V3x4(v3 a, v3 b, v3 c, v3 d) {                            \
		return {a.x, b.x, c.x, d.x, a.y, b.y, c.y, d.y, a.z, b.z, c.z, d.z}; \
	}                                                                        \
	constexpr v3x4 V3x4(v3 v) { return CE::V3x4(v, v, v, v); }
V3x4(v3, v3x4, V3x4) V3x4(v3s, v3sx4, V3sx4) V3x4(v3u, v3ux4, V3ux4);
#undef V3x4
#define V4x4(v4, v4x4, V4x4)                                                                     \
	constexpr v4x4 V4x4(v4 a, v4 b, v4 c, v4 d) {                                                \
		return {a.x, b.x, c.x, d.x, a.y, b.y, c.y, d.y, a.z, b.z, c.z, d.z, a.w, b.w, c.w, d.w}; \
	}                                                                                            \
	constexpr v4x4 V4x4(v4 v) { return CE::V4x4(v, v, v, v); }
V4x4(v4, v4x4, V4x4) V4x4(v4s, v4sx4, V4sx4) V4x4(v4u, v4ux4, V4ux4);
#undef V4x4
#define V2x8(v2, v2x8, V2x8)                                                                     \
	constexpr v2x8 V2x8(v2 a, v2 b, v2 c, v2 d, v2 e, v2 f, v2 g, v2 h) {                        \
		return {a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x, a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y}; \
	}                                                                                            \
	constexpr v2x8 V2x8(v2 v) { return CE::V2x8(v, v, v, v, v, v, v, v); }
V2x8(v2, v2x8, V2x8) V2x8(v2s, v2sx8, V2sx8) V2x8(v2u, v2ux8, V2ux8);
#undef V2x8
#define V3x8(v3, v3x8, V3x8)                                              \
	constexpr v3x8 V3x8(v3 a, v3 b, v3 c, v3 d, v3 e, v3 f, v3 g, v3 h) { \
		return {                                                          \
			a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x, a.y, b.y, c.y, d.y,   \
			e.y, f.y, g.y, h.y, a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z,   \
		};                                                                \
	}                                                                     \
	constexpr v3x8 V3x8(v3 v) { return CE::V3x8(v, v, v, v, v, v, v, v); }
V3x8(v3, v3x8, V3x8) V3x8(v3s, v3sx8, V3sx8) V3x8(v3u, v3ux8, V3ux8);
#undef V3x8
#define V4x8(v4, v4x8, V4x8)                                                                     \
	constexpr v4x8 V4x8(v4 a, v4 b, v4 c, v4 d, v4 e, v4 f, v4 g, v4 h) {                        \
		return {a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x, a.y, b.y, c.y, d.y, e.y, f.y, g.y, h.y,  \
				a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z, a.w, b.w, c.w, d.w, e.w, f.w, g.w, h.w}; \
	}                                                                                            \
	constexpr v4x8 V4x8(v4 v) { return CE::V4x8(v, v, v, v, v, v, v, v); }
V4x8(v4, v4x8, V4x8) V4x8(v4s, v4sx8, V4sx8) V4x8(v4u, v4ux8, V4ux8);
#undef V4x8
} // namespace CE
  // clang-format off

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

#define CVT(v2, v2s, s32) template<> constexpr v2s cvt(v2 v) { return {(s32)v.x, (s32)v.y}; }
CVT(v2, v2s, s32) CVT(v2, v2u, u32);
CVT(v2s, v2, f32) CVT(v2s, v2u, u32);
CVT(v2u, v2, f32) CVT(v2u, v2s, s32);
#undef CVT

#define CVT(v3, v3s, s32) template<> constexpr v3s cvt(v3 v) { return {(s32)v.x, (s32)v.y, (s32)v.z}; }
CVT(v3, v3s, s32) CVT(v3, v3u, u32);
CVT(v3s, v3, f32) CVT(v3s, v3u, u32);
CVT(v3u, v3, f32) CVT(v3u, v3s, s32);
#undef CVT

#define CVT(v4, v4s, s32) template<> constexpr v4s cvt(v4 v) { return {(s32)v.x, (s32)v.y, (s32)v.z, (s32)v.w}; }
CVT(v4, v4u, u32) CVT(v4s, v4u, u32);
CVT(v4u, v4, f32) CVT(v4u, v4s, s32);
#undef CVT

template<> v4s cvt(v4 v) { return V4s(cvt<s32x4>(v.m)); }
template<> v4 cvt(v4s v) { return V4( cvt<f32x4>(v.m)); }

#define CVT(v2, v2s, V2s, s32) template<> v2s cvt(v2 v) { return V2s(cvt<s32>(v.x), cvt<s32>(v.y)); }
CVT(v2x4, v2sx4, V2sx4, s32x4) CVT(v2sx4, v2x4, V2x4, f32x4);
CVT(v2x8, v2sx8, V2sx8, s32x8) CVT(v2sx8, v2x8, V2x8, f32x8);
#undef CVT

#define CVT(v3, v3s, V3s, s32) template<> v3s cvt(v3 v) { return V3s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z)); }
CVT(v3x4, v3sx4, V3sx4, s32x4) CVT(v3sx4, v3x4, V3x4, f32x4);
CVT(v3x8, v3sx8, V3sx8, s32x8) CVT(v3sx8, v3x8, V3x8, f32x8);
#undef CVT

#define CVT(v4, v4s, V4s, s32) template<> v4s cvt(v4 v) { return V4s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z), cvt<s32>(v.w)); }
CVT(v4x4, v4sx4, V4sx4, s32x4) CVT(v4sx4, v4x4, V4x4, f32x4);
CVT(v4x8, v4sx8, V4sx8, s32x8) CVT(v4sx8, v4x8, V4x8, f32x8);
#undef CVT

#define CVT(v2, v2s, V2s, s32) template<> constexpr v2s cvt(v2 v) { return V2s(cvt<s32>(v.x), cvt<s32>(v.y)); }
CVT(v2x4, v2ux4, V2ux4, u32x4) CVT(v2ux4, v2x4, V2x4, f32x4);
CVT(v2x8, v2ux8, V2ux8, u32x8) CVT(v2ux8, v2x8, V2x8, f32x8);
CVT(v2sx4, v2ux4, V2ux4, u32x4) CVT(v2ux4, v2sx4, V2sx4, s32x4);
CVT(v2sx8, v2ux8, V2ux8, u32x8) CVT(v2ux8, v2sx8, V2sx8, s32x8);
#undef CVT

#define CVT(v3, v3s, V3s, s32) template<> constexpr v3s cvt(v3 v) { return V3s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z)); }
CVT(v3x4, v3ux4, V3ux4, u32x4) CVT(v3ux4, v3x4, V3x4, f32x4);
CVT(v3x8, v3ux8, V3ux8, u32x8) CVT(v3ux8, v3x8, V3x8, f32x8);
CVT(v3sx4, v3ux4, V3ux4, u32x4) CVT(v3ux4, v3sx4, V3sx4, s32x4);
CVT(v3sx8, v3ux8, V3ux8, u32x8) CVT(v3ux8, v3sx8, V3sx8, s32x8);
#undef CVT

#define CVT(v4, v4s, V4s, s32) template<> constexpr v4s cvt(v4 v) { return V4s(cvt<s32>(v.x), cvt<s32>(v.y), cvt<s32>(v.z), cvt<s32>(v.w)); }
CVT(v4x4, v4ux4, V4ux4, u32x4) CVT(v4ux4, v4x4, V4x4, f32x4);
CVT(v4x8, v4ux8, V4ux8, u32x8) CVT(v4ux8, v4x8, V4x8, f32x8);
CVT(v4sx4, v4ux4, V4ux4, u32x4) CVT(v4ux4, v4sx4, V4sx4, s32x4);
CVT(v4sx8, v4ux8, V4ux8, u32x8) CVT(v4ux8, v4sx8, V4sx8, s32x8);
#undef CVT

namespace CE {

#define CVT(u32x4, f32x4, f32) template<> constexpr f32x4 cvt(u32x4 v) {  return {(f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3]};  }
CVT(f32x4, s32x4, s32) CVT(f32x4, u32x4, u32);
CVT(s32x4, f32x4, f32) CVT(s32x4, u32x4, u32);
CVT(u32x4, f32x4, f32) CVT(u32x4, s32x4, s32);
#undef CVT

#define CVT(u32x8, f32x8, f32) template<> constexpr f32x8 cvt(u32x8 v) {  return {(f32)v[0], (f32)v[1], (f32)v[2], (f32)v[3], (f32)v[4], (f32)v[5], (f32)v[6], (f32)v[7]};  }
CVT(f32x8, s32x8, s32) CVT(f32x8, u32x8, u32);
CVT(s32x8, f32x8, f32) CVT(s32x8, u32x8, u32);
CVT(u32x8, f32x8, f32) CVT(u32x8, s32x8, s32);
#undef CVT

#define CVT(v2, v2s, s32) template<> constexpr v2s cvt(v2 v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y)}; }
CVT(v2, v2s, s32) CVT(v2, v2u, u32);
CVT(v2s, v2, f32) CVT(v2s, v2u, u32);
CVT(v2u, v2, f32) CVT(v2u, v2s, s32);
CVT(v2x4, v2sx4, s32x4) CVT(v2x4, v2ux4, u32x4);
CVT(v2x8, v2sx8, s32x8)	CVT(v2x8, v2ux8, u32x8);
CVT(v2sx4, v2x4, f32x4) CVT(v2sx4, v2ux4, u32x4);
CVT(v2sx8, v2x8, f32x8) CVT(v2sx8, v2ux8, u32x8);
CVT(v2ux4, v2x4, f32x4) CVT(v2ux4, v2sx4, s32x4);
CVT(v2ux8, v2x8, f32x8) CVT(v2ux8, v2sx8, s32x8);
#undef CVT

#define CVT(v3, v3s, s32) template<> constexpr v3s cvt(v3 v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z)}; }
CVT(v3, v3s, s32) CVT(v3, v3u, u32);
CVT(v3s, v3, f32) CVT(v3s, v3u, u32);
CVT(v3u, v3, f32) CVT(v3u, v3s, s32);
CVT(v3x4, v3sx4, s32x4) CVT(v3x8, v3sx8, s32x8);
CVT(v3x4, v3ux4, u32x4) CVT(v3x8, v3ux8, u32x8);
CVT(v3sx4, v3x4, f32x4) CVT(v3sx4, v3ux4, u32x4);
CVT(v3sx8, v3x8, f32x8) CVT(v3sx8, v3ux8, u32x8);
CVT(v3ux4, v3x4, f32x4) CVT(v3ux4, v3sx4, s32x4);
CVT(v3ux8, v3x8, f32x8) CVT(v3ux8, v3sx8, s32x8);
#undef CVT

#define CVT(v4, v4s, s32) template<> constexpr v4s cvt(v4 v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z), CE::cvt<s32>(v.w)}; }
CVT(v4, v4s, s32) CVT(v4, v4u, u32);
CVT(v4s, v4, f32) CVT(v4s, v4u, u32);
CVT(v4u, v4, f32) CVT(v4u, v4s, s32);
CVT(v4x4, v4sx4, s32x4) CVT(v4x8, v4sx8, s32x8);
CVT(v4x4, v4ux4, u32x4) CVT(v4x8, v4ux8, u32x8);
CVT(v4sx4, v4x4, f32x4) CVT(v4sx4, v4ux4, u32x4);
CVT(v4sx8, v4x8, f32x8) CVT(v4sx8, v4ux8, u32x8);
CVT(v4ux4, v4x4, f32x4) CVT(v4ux4, v4sx4, s32x4);
CVT(v4ux8, v4x8, f32x8) CVT(v4ux8, v4sx8, s32x8);
#undef CVT
} // namespace CE
// clang-format on

f32 min(f32x8 v) { return min(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }

// clang-format off
auto min(f32x4 a, f32x4 b) { return F32x4(_mm_min_ps(a.m, b.m)); }
auto min(s32x4 a, s32x4 b) { return S32x4(_mm_min_epi32(a.m, b.m)); }
auto min(u32x4 a, u32x4 b) { return U32x4(_mm_min_epu32(a.m, b.m)); }
auto min(v2 a, v2 b) { return V2(min(a.x, b.x), min(a.y, b.y)); }
auto min(v3 a, v3 b) { return V3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
auto min(v4 a, v4 b) { return V4(min(a.m, b.m)); }
auto min(v2s a, v2s b) { return V2s(min(a.x, b.x), min(a.y, b.y)); }
auto min(v3s a, v3s b) { return V3s(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
auto min(v4s a, v4s b) { return V4s(min(a.m, b.m)); }
auto min(v2u a, v2u b) { return V2u(min(a.x, b.x), min(a.y, b.y)); }
auto min(v3u a, v3u b) { return V3u(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
auto min(v4u a, v4u b) { return V4u(min(a.m, b.m)); }
auto min(f32 a, v2 b) { return min(a, min(b.x, b.y)); }
auto min(f32 a, v4 b) { return min(a, min(min(b.x, b.y), min(b.z, b.w))); }
auto min(f32 a, f32x8 b) { return min(a, min(b)); } 

auto max(f32x4 a, f32x4 b) { return F32x4(_mm_max_ps(a.m, b.m)); }
auto max(s32x4 a, s32x4 b) { return S32x4(_mm_max_epi32(a.m, b.m)); }
auto max(u32x4 a, u32x4 b) { return U32x4(_mm_max_epu32(a.m, b.m)); }
auto max(v2 a, v2 b) { return V2(max(a.x, b.x), max(a.y, b.y)); }
auto max(v3 a, v3 b) { return V3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
auto max(v4 a, v4 b) { return V4(max(a.m, b.m)); }
auto max(v2s a, v2s b) { return V2s(max(a.x, b.x), max(a.y, b.y)); }
auto max(v3s a, v3s b) { return V3s(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
auto max(v4s a, v4s b) { return V4s(max(a.m, b.m)); }
auto max(v2u a, v2u b) { return V2u(max(a.x, b.x), max(a.y, b.y)); }
auto max(v3u a, v3u b) { return V3u(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
auto max(v4u a, v4u b) { return V4u(max(a.m, b.m)); }
auto max(f32 a, v2 b) { return max(a, max(b.x, b.y)); }
auto max(f32 a, v4 b) { return max(a, max(max(b.x, b.y), max(b.z, b.w))); }
#if ARCH_AVX
auto min(f32x8 a, f32x8 b) { return F32x8(_mm256_min_ps(a.m, b.m)); }
auto max(f32x8 a, f32x8 b) { return F32x8(_mm256_max_ps(a.m, b.m)); }
#else
auto min(f32x8 a, f32x8 b) { return F32x8(min(a.im, b.im), min(a.jm, b.jm)); }
auto max(f32x8 a, f32x8 b) { return F32x8(max(a.im, b.im), max(a.jm, b.jm)); }
#endif

void minmax(v2 a, v2 b, v2& mn, v2& mx) {
	minmax(a.x, b.x, mn.x, mx.x);
	minmax(a.y, b.y, mn.y, mx.y); 
}
void minmax(v2s a, v2s b, v2s& mn, v2s& mx) {
	minmax(a.x, b.x, mn.x, mx.x);
	minmax(a.y, b.y, mn.y, mx.y); 
}
void minmax(v2u a, v2u b, v2u& mn, v2u& mx) {
	minmax(a.x, b.x, mn.x, mx.x);
	minmax(a.y, b.y, mn.y, mx.y); 
}
// clang-format on

constexpr bool _isTrueMask(f32 v) { return *(u32*)&v == ~0u; }

f32 select(f32 mask, f32 a, f32 b) { return _isTrueMask(mask) ? a : b; }

f32x4 select(f32x4 mask, f32x4 a, f32x4 b) { return F32x4(_mm_blendv_ps(b.m, a.m, mask.m)); }
v2 select(v2 mask, v2 a, v2 b) { return {select(mask.x, a.x, b.x), select(mask.y, a.y, b.y)}; }
v3 select(v3 mask, v3 a, v3 b) {
	return {select(mask.x, a.x, b.x), select(mask.y, a.y, b.y), select(mask.z, a.z, b.z)};
}
v4 select(v4 mask, v4 a, v4 b) { return V4(select(mask.m, a.m, b.m)); }

v2 moveTowards(v2 a, v2 b, f32 t) { return select(a < b, min(a + t, b), max(a - t, b)); }
v3 moveTowards(v3 a, v3 b, f32 t) {
	return {
		moveTowards(a.x, b.x, t),
		moveTowards(a.y, b.y, t),
		moveTowards(a.z, b.z, t),
	};
}
v4 moveTowards(v4 a, v4 b, f32 t) { return select(a < b, min(a + t, b), max(a - t, b)); }

f32 sqrt(f32 v) { return sqrtf(v); }
v2 sqrt(v2 v) { return V2(sqrtf(v.x), sqrtf(v.y)); }
v3 sqrt(v3 v) { return V3(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z)); }
f32x4 sqrt(f32x4 v) {
	v.m = _mm_sqrt_ps(v.m);
	return v;
}
v4 sqrt(v4 v) { return V4(sqrt(v.m)); }
#if COMPILER_GCC
inline v2 sin(v2 v) { return V2(sinf(v.x), sinf(v.y)); }
inline v3 sin(v3 v) { return V3(sinf(v.x), sinf(v.y), sinf(v.z)); }
inline v4 sin(v4 v) { return V4(sinf(v.x), sinf(v.y), sinf(v.z), sinf(v.w)); }
inline v2 cos(v2 v) { return V2(cosf(v.x), cosf(v.y)); }
inline v3 cos(v3 v) { return V3(cosf(v.x), cosf(v.y), cosf(v.z)); }
inline v4 cos(v4 v) { return V4(cosf(v.x), cosf(v.y), cosf(v.z), cosf(v.w)); }
inline void sincos(v2 v, v2& sinOut, v2& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
inline void sincos(v3 v, v3& sinOut, v3& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
inline void sincos(v4 v, v4& sinOut, v4& cosOut) {
	sinOut = sin(v);
	cosOut = cos(v);
}
#else
inline f32x4 sin(f32x4 v) { return F32x4(_mm_sin_ps(v.m)); }
inline v4 sin(v4 v) { return V4(sin(v.m)); }
inline v2 sin(v2 v) { return sin(V4(v, 0, 0)).xy; }
inline v3 sin(v3 v) { return sin(V4(v, 0)).xyz; }
inline f32x4 cos(f32x4 v) { return F32x4(_mm_cos_ps(v.m)); }
inline v4 cos(v4 v) { return V4(cos(v.m)); }
inline v2 cos(v2 v) { return cos(V4(v, 0, 0)).xy; }
inline v3 cos(v3 v) { return cos(V4(v, 0)).xyz; }

inline void sincos(v2 v, v2& sinOut, v2& cosOut) {
	__m128 c, s = _mm_sincos_ps(&c, _mm_setr_ps(v.x, v.y, 0, 0));
	memcpy(&sinOut, &s, sizeof(sinOut));
	memcpy(&cosOut, &c, sizeof(cosOut));
}
inline void sincos(v3 v, v3& sinOut, v3& cosOut) {
	__m128 c, s = _mm_sincos_ps(&c, _mm_setr_ps(v.x, v.y, v.z, 0));
	memcpy(&sinOut, &s, sizeof(sinOut));
	memcpy(&cosOut, &c, sizeof(cosOut));
}
inline void sincos(f32x4 v, f32x4& sinOut, f32x4& cosOut) { sinOut.m = _mm_sincos_ps(&cosOut.m, v.m); }
inline void sincos(v4 v, v4& sinOut, v4& cosOut) { sincos(v.m, sinOut.m, cosOut.m); }
#endif

u32 countBits(u32 v) { return (u32)_mm_popcnt_u32(v); }
u32 countBits(s32 v) { return countBits((u32)v); }
#if ARCH_AVX2
u32 countBits(u64 v) { return (u32)_mm_popcnt_u64(v); }
u32 countBits(s64 v) { return countBits((u64)v); }
#endif
u32 fillBits(u32 v) {
	u32 r = 0;
	while (v--) {
		r = (r << 1) | 1;
	}
	return r;
}
inline f32 round(f32 v) { return roundf(v); }
inline f32x4 round(f32x4 v) { return F32x4(_mm_round_ps(v.m, _MM_FROUND_NINT)); }
inline v4 round(v4 v) { return V4(round(v.m)); }
inline s32 roundInt(f32 v) {
	if (v < 0)
		return int(v - 0.5f);
	return int(v + 0.5f);
}
inline v2s roundInt(v2 v) { return {roundInt(v.x), roundInt(v.y)}; }
inline v3s roundInt(v3 v) { return {roundInt(v.x), roundInt(v.y), roundInt(v.z)}; }
inline v4s roundInt(v4 v) { return V4s(round(v)); }
constexpr f32 frac(f32 v) {
	auto r = v - (s64)v;
	if (r < 0)
		++r;
	return r;
}
f32x4 frac(f32x4 v) {
	v.m = _mm_sub_ps(v.m, _mm_cvtepi32_ps(_mm_cvtps_epi32(v.m)));
	v.m = _mm_add_ps(v.m, _mm_and_ps(_mm_cmplt_ps(v.m, _mm_setzero_ps()), _mm_set1_ps(1.0f)));
	return v;
}
f32x8 frac(f32x8 v) {
	v.m = _mm256_sub_ps(v.m, _mm256_cvtepi32_ps(_mm256_cvtps_epi32(v.m)));
	v.m = _mm256_add_ps(v.m, _mm256_and_ps(_mm256_cmplt_ps(v.m, _mm256_setzero_ps()), _mm256_set1_ps(1.0f)));
	return v;
}
v4 frac(v4 v) { return V4(frac(v.m)); }
v4x4 frac(v4x4 v) { return V4x4(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }
v4x8 frac(v4x8 v) { return V4x8(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }

v2 frac(v2 v) { return frac(V4(v, 0, 0)).xy; }
v2x4 frac(v2x4 v) { return V2x4(frac(v.x), frac(v.y)); }
v2x8 frac(v2x8 v) { return V2x8(frac(v.x), frac(v.y)); }

v3 frac(v3 v) { return frac(V4(v, 0)).xyz; }
v3x4 frac(v3x4 v) { return V3x4(frac(v.x), frac(v.y), frac(v.z)); }
v3x8 frac(v3x8 v) { return V3x8(frac(v.x), frac(v.y), frac(v.z)); }

constexpr s32 frac(s32 v, u32 s) {
	if (v < 0)
		return (v + 1) % (s32)s + (s32)s - 1;
	return v % (s32)s;
}
v2s frac(v2s v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
	};
}
v3s frac(v3s v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
		frac(v.z, s),
	};
}
v4s frac(v4s v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
		frac(v.z, s),
		frac(v.w, s),
	};
#if 0
	__m128i ms	 = _mm_set1_epi32(s);
	__m128i one	 = _mm_set1_epi32(1);
	__m128i mask = _mm_cmplt_epi32(v.m, _mm_setzero_si128());
	__m128i pos, neg;
	if (s32 bits = countBits(s); bits == 1) {
		unsigned long idx;
		_BitScanForward(&idx, (u32)s);
		u32 rmask = fillBits(idx);
		__m128i mrmask = _mm_set1_epi32((s32)rmask);
		pos = _mm_and_si128(v.m, mrmask);
		neg = _mm_sub_epi32(_mm_add_epi32(_mm_and_si128(_mm_add_epi32(v.m, one), mrmask), ms), one);
	} else {
		pos = _mm_rem_epi32(v.m, ms);
		neg = _mm_sub_epi32(_mm_add_epi32(_mm_rem_epi32(_mm_add_epi32(v.m, one), ms), ms), one);
	}
	return V4s(_mm_or_si128(_mm_and_si128(mask, neg), _mm_andnot_si128(mask, pos)));
#endif
}
f32 floor(f32 v) { return floorf(v); }
v2 floor(v2 v) {
	return {
		floorf(v.x),
		floorf(v.y),
	};
}
v3 floor(v3 v) {
	return {
		floorf(v.x),
		floorf(v.y),
		floorf(v.z),
	};
}
f32x4 floor(f32x4 v) { return F32x4(_mm_floor_ps(v.m)); }
v4 floor(v4 v) { return V4(floor(v.m)); }
s32 floorInt(f32 v) { return (s32)floorf(v); }
v2s floorInt(v2 v) {
	return {
		floorInt(v.x),
		floorInt(v.y),
	};
}
v3s floorInt(v3 v) {
	return {
		floorInt(v.x),
		floorInt(v.y),
		floorInt(v.z),
	};
}
s32x4 floorInt(f32x4 v) { return S32x4(_mm_cvtps_epi32(_mm_floor_ps(v.m))); }
v4s floorInt(v4 v) { return V4s(floorInt(v.m)); }
constexpr s32 floor(s32 v, u32 s) {
	if (v < 0)
		v = (v + 1) / (s32)s - 1;
	else
		v /= (s32)s;
	return v * (s32)s;
}
v2s floor(v2s v, u32 step) {
	return {
		floor(v.x, step),
		floor(v.y, step),
	};
}
v3s floor(v3s v, u32 step) {
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
	};
}
v4s floor(v4s v, u32 step) {
#if 0
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
		floor(v.w, step),
	};
#else
	s32x4 ms = S32x4((s32)step);
	s32x4 pos = v.m / ms;
	s32x4 neg = (v.m + 1) / ms - 1;
	__m128i mask = _mm_cmplt_epi32(v.m.m, _mm_setzero_si128());
	return V4s(S32x4(_mm_blendv_epi32(pos.m, neg.m, mask)) * (s32)step);
#endif
}
f32 dot(f32x4 a, f32x4 b) {
	f32 result;
	_mm_store_ss(&result, _mm_dp_ps(a.m, b.m, 0xFF));
	return result;
}
#define DOT(f32, v2)      \
	f32 dot(v2 a, v2 b) { \
		a *= b;           \
		return a.x + a.y; \
	}
DOT(f32, v2)
DOT(f32x4, v2x4)
DOT(f32x8, v2x8)
#undef DOT

#define DOT(f32, v3)            \
	f32 dot(v3 a, v3 b) {       \
		a *= b;                 \
		return a.x + a.y + a.z; \
	}
DOT(f32, v3)
DOT(f32x4, v3x4)
DOT(f32x8, v3x8)
#undef DOT

f32 dot(v4 a, v4 b) { return dot(a.m, b.m); }

#define DOT(f32, v4)                  \
	f32 dot(v4 a, v4 b) {             \
		a *= b;                       \
		return a.x + a.y + a.z + a.w; \
	}
DOT(f32x4, v4x4)
DOT(f32x8, v4x8)
#undef DOT

v2 cross(v2 a) { return {a.y, -a.x}; }
v3 cross(v3 a, v3 b) {
	// clang-format off
	return {
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z, 
		a.x * b.y - a.y * b.x
	};
	// clang-format on
}

v2 abs(v2 v) { return {fabsf(v.x), fabsf(v.y)}; }
v3 abs(v3 v) { return {fabsf(v.x), fabsf(v.y), fabsf(v.z)}; }
v4 abs(v4 v) { return {fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w)}; }
v2s abs(v2s a) { return {labs(a.x), labs(a.y)}; }
v3s abs(v3s a) { return {labs(a.x), labs(a.y), labs(a.z)}; }
s32x4 abs(s32x4 a) { return S32x4(_mm_abs_epi32(a.m)); }
v4s abs(v4s a) { return V4s(abs(a.m)); }

f32 sign(f32 v) { return v > 0.0f ? 1.0f : v < 0.0f ? -1.0f : 0.0f; }

f32 sum(v2 v) { return v.x + v.y; }

template <class T>
auto lengthSqr(T a) {
	return dot(a, a);
}
template <class T>
auto length(T a) {
	return sqrt(lengthSqr(a));
}
template <class T>
auto normalize(T a) {
	return a / length(a);
}
template <class T>
auto normalize(T a, T fallback) {
	auto lsq = lengthSqr(a);
	if (lsq == decltype(lsq){})
		return fallback;
	return a / sqrt(lsq);
}
template <class T>
auto distanceSqr(T a, T b) {
	return lengthSqr(a - b);
}
template <class T>
auto distance(T a, T b) {
	return sqrt(distanceSqr(a, b));
}
template <class T>
auto manhattan(T a, T b) {
	return sum(abs(a - b));
}
int maxDistance(v3s a, v3s b) {
	a = abs(a - b);
	return max(max(a.x, a.y), a.z);
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
inline f32 interpolate(v2 p, Interp&& interp, Sample&& sample) {
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
inline static constexpr v2 _randMul2{pi * 10, sqrt2 * 20};
inline static constexpr v3 _randMul3{pi * 10, sqrt2 * 20, sqrt3 * 15};
inline static constexpr v4 _randMul4{pi * 10, sqrt2 * 20, sqrt3 * 15, sqrt5 * 10};

u32 rotateLeft(u32 v, int shift) { return _rotl(v, shift); }
u32 rotateRight(u32 v, int shift) { return _rotr(v, shift); }
#if COMPILER_GCC
u8 rotateLeft(u8 v, int shift) { return (v << shift) | (v >> (8 - shift)); }
u16 rotateLeft(u16 v, int shift) { return (v << shift) | (v >> (16 - shift)); }
u64 rotateLeft(u64 v, int shift) { return (v << shift) | (v >> (64 - shift)); }
u8 rotateRight(u8 v, int shift) { return (v >> shift) | (v << (8 - shift)); }
u16 rotateRight(u16 v, int shift) { return (v >> shift) | (v << (16 - shift)); }
u64 rotateRight(u64 v, int shift) { return (v >> shift) | (v << (64 - shift)); }
#else
u8 rotateLeft(u8 v, int shift) { return _rotl8(v, (u8)shift); }
u16 rotateLeft(u16 v, int shift) { return _rotl16(v, (u8)shift); }
u64 rotateLeft(u64 v, int shift) { return _rotl64(v, shift); }
u8 rotateRight(u8 v, int shift) { return _rotr8(v, (u8)shift); }
u16 rotateRight(u16 v, int shift) { return _rotr16(v, (u8)shift); }
u64 rotateRight(u64 v, int shift) { return _rotr64(v, shift); }
#endif
u32 randomize(u32 v) { 
	return v * 0xEB84226F ^ 0x034FB5E7;
	//return rotateLeft(((v + 0x0C252DA0) * 0x034FB5E7) ^ 0xF5605798, 16); 
}
s32 randomize(s32 v) { return (s32)randomize((u32)v); }
#define RANDOMIZE(v2u)         \
	v2u randomize(v2u v) {     \
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
	v3u randomize(v3u v) {       \
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
	v4u randomize(v4u v) {            \
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
	v2s randomize(v2s v) { return cvt<v2s>(randomize(cvt<v2u>(v))); }
RANDOMIZE(v2s, v2u) RANDOMIZE(v2sx4, v2ux4) RANDOMIZE(v2sx8, v2ux8);
RANDOMIZE(v3s, v3u) RANDOMIZE(v3sx4, v3ux4) RANDOMIZE(v3sx8, v3ux8);
RANDOMIZE(v4s, v4u) RANDOMIZE(v4sx4, v4ux4) RANDOMIZE(v4sx8, v4ux8);
#undef RANDOMIZE

u8 randomU8(u8 r) {
	r += 0x0C;
	r *= 0x61;
	r ^= 0xB2;
	r -= 0x80;
	r ^= 0xF5;
	r *= 0xA7;
	return rotateLeft(r, 4);
}
u32 randomU32(u32 r) {
	r += 0x0C252DA0;
	r *= 0x55555561;
	r ^= 0xB23E2387;
	r -= 0x8069BAC0;
	r ^= 0xF5605798;
	r *= 0xAAAAAABF;
	return (r << 16) | (r >> 16);
}
u32 randomU32(s32 in) { return randomU32((u32)in); }
u32 randomU32(v2s in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	return x ^ y;
}
u32 randomU32(v3s in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	auto z = randomU32(in.z);
	return x + y + z;
}
u32 randomU32(v4s v) {
	v = randomize(v);
	return u32(v.x) + u32(v.y) + u32(v.z) + u32(v.w);
}
u64 randomU64(v3s in) {
	auto x = (u64)randomU32(in.x);
	auto y = (u64)randomU32(in.y);
	auto z = (u64)randomU32(in.z);
	return x | (y << 32) + z | (x << 32) + y | (z << 32);
}
#define RANDOM(v2, V2)               \
	v2 random(v2 v) {                \
		v = frac(v * V2(_randMul2)); \
		v += dot(v, v + pi * 4);     \
		v.x *= v.x;                  \
		v.y *= v.y;                  \
		return frac(v);              \
	}
RANDOM(v2, V2)
RANDOM(v2x4, V2x4)
RANDOM(v2x8, V2x8)
#undef RANDOM
#define RANDOM(v3, V3)                                    \
	v3 random(v3 v) {                                     \
		v = frac(v * V3(_randMul3));                      \
		v += dot(v, v + pi * 4);                          \
		return frac(V3(v.x * v.y, v.y * v.z, v.x * v.z)); \
	}
RANDOM(v3, V3)
RANDOM(v3x4, V3x4)
RANDOM(v3x8, V3x8)
#undef RANDOM
#define RANDOM(v4, V4)                           \
	v4 random(v4 v) {                            \
		v = frac((v + sqrt2) * V4(_randMul4));   \
		v += dot(v, v + pi * 4);                 \
		return frac(v * V4(v.y, v.z, v.w, v.x)); \
	}
RANDOM(v4, V4)
RANDOM(v4x4, V4x4)
RANDOM(v4x8, V4x8)
#undef RANDOM

// clang-format off
#define RANDOM(v2, v2s, V2, V2u) v2 random(v2s v) { return V2(randomize(V2u(v)) >> 8) * (1.0f / 16777216.0f); }
RANDOM(v2, v2s, V2, V2u) RANDOM(v2x4, v2sx4, V2x4, V2ux4) RANDOM(v2x8, v2sx8, V2x8, V2ux8)
RANDOM(v3, v3s, V3, V3u) RANDOM(v3x4, v3sx4, V3x4, V3ux4) RANDOM(v3x8, v3sx8, V3x8, V3ux8)
RANDOM(v4, v4s, V4, V4u) RANDOM(v4x4, v4sx4, V4x4, V4ux4) RANDOM(v4x8, v4sx8, V4x8, V4ux8)
#undef RANDOM

#define RANDOM(v2, v2s) v2 operator()(v2s v) const { return random(v); }
struct Random {
	RANDOM(v2, v2s) RANDOM(v2x4, v2sx4) RANDOM(v2x8, v2sx8)
	RANDOM(v3, v3s) RANDOM(v3x4, v3sx4) RANDOM(v3x8, v3sx8)
	RANDOM(v4, v4s) RANDOM(v4x4, v4sx4) RANDOM(v4x8, v4sx8)
	RANDOM(v2, v2) RANDOM(v2x4, v2x4) RANDOM(v2x8, v2x8)
	RANDOM(v3, v3) RANDOM(v3x4, v3x4) RANDOM(v3x8, v3x8)
	RANDOM(v4, v4) RANDOM(v4x4, v4x4) RANDOM(v4x8, v4x8)
};
#undef RANDOM

template<class T> constexpr bool isVector = false;
template<> constexpr bool isVector<v2   > = true;
template<> constexpr bool isVector<v3   > = true;
template<> constexpr bool isVector<v4   > = true;
template<> constexpr bool isVector<v2s  > = true;
template<> constexpr bool isVector<v3s  > = true;
template<> constexpr bool isVector<v4s  > = true;
template<> constexpr bool isVector<v2u  > = true;
template<> constexpr bool isVector<v3u  > = true;
template<> constexpr bool isVector<v4u  > = true;
template<> constexpr bool isVector<v2x4 > = true;
template<> constexpr bool isVector<v3x4 > = true;
template<> constexpr bool isVector<v4x4 > = true;
template<> constexpr bool isVector<v2sx4> = true;
template<> constexpr bool isVector<v3sx4> = true;
template<> constexpr bool isVector<v4sx4> = true;
template<> constexpr bool isVector<v2ux4> = true;
template<> constexpr bool isVector<v3ux4> = true;
template<> constexpr bool isVector<v4ux4> = true;
template<> constexpr bool isVector<v2x8 > = true;
template<> constexpr bool isVector<v3x8 > = true;
template<> constexpr bool isVector<v4x8 > = true;
template<> constexpr bool isVector<v2sx8> = true;
template<> constexpr bool isVector<v3sx8> = true;
template<> constexpr bool isVector<v4sx8> = true;
template<> constexpr bool isVector<v2ux8> = true;
template<> constexpr bool isVector<v3ux8> = true;
template<> constexpr bool isVector<v4ux8> = true;
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
constexpr size_t _widthOf = [] {}(); // invalid width for invalid types
// clang-format off
template<> constexpr size_t _widthOf<v2>    = 1;
template<> constexpr size_t _widthOf<v3>    = 1;
template<> constexpr size_t _widthOf<v4>    = 1;
template<> constexpr size_t _widthOf<v2s>   = 1;
template<> constexpr size_t _widthOf<v3s>   = 1;
template<> constexpr size_t _widthOf<v4s>   = 1;
template<> constexpr size_t _widthOf<v2u>   = 1;
template<> constexpr size_t _widthOf<v3u>   = 1;
template<> constexpr size_t _widthOf<v4u>   = 1;
template<> constexpr size_t _widthOf<v2x4>  = 4;
template<> constexpr size_t _widthOf<v3x4>  = 4;
template<> constexpr size_t _widthOf<v4x4>  = 4;
template<> constexpr size_t _widthOf<v2sx4> = 4;
template<> constexpr size_t _widthOf<v3sx4> = 4;
template<> constexpr size_t _widthOf<v4sx4> = 4;
template<> constexpr size_t _widthOf<v2ux4> = 4;
template<> constexpr size_t _widthOf<v3ux4> = 4;
template<> constexpr size_t _widthOf<v4ux4> = 4;
template<> constexpr size_t _widthOf<v2x8>  = 8;
template<> constexpr size_t _widthOf<v3x8>  = 8;
template<> constexpr size_t _widthOf<v4x8>  = 8;
template<> constexpr size_t _widthOf<v2sx8> = 8;
template<> constexpr size_t _widthOf<v3sx8> = 8;
template<> constexpr size_t _widthOf<v4sx8> = 8;
template<> constexpr size_t _widthOf<v2ux8> = 8;
template<> constexpr size_t _widthOf<v3ux8> = 8;
template<> constexpr size_t _widthOf<v4ux8> = 8;
// clang-format on

template <class T>
constexpr size_t widthOf = _widthOf<RemoveQualifiers<T>::Type>;

template <size_t width, class T>
auto widen(T v) = delete;

// clang-format off
template<> auto widen<1>(v2    v) { return v; }
template<> auto widen<1>(v3    v) { return v; }
template<> auto widen<1>(v4    v) { return v; }
template<> auto widen<1>(v2s   v) { return v; }
template<> auto widen<1>(v3s   v) { return v; }
template<> auto widen<1>(v4s   v) { return v; }
template<> auto widen<1>(v2u   v) { return v; }
template<> auto widen<1>(v3u   v) { return v; }
template<> auto widen<1>(v4u   v) { return v; }
template<> auto widen<4>(v2    v) { return  V2x4(v); }
template<> auto widen<4>(v3    v) { return  V3x4(v); }
template<> auto widen<4>(v4    v) { return  V4x4(v); }
template<> auto widen<4>(v2s   v) { return V2sx4(v); }
template<> auto widen<4>(v3s   v) { return V3sx4(v); }
template<> auto widen<4>(v4s   v) { return V4sx4(v); }
template<> auto widen<4>(v2u   v) { return V2ux4(v); }
template<> auto widen<4>(v3u   v) { return V3ux4(v); }
template<> auto widen<4>(v4u   v) { return V4ux4(v); }
template<> auto widen<8>(v2    v) { return  V2x8(v); }
template<> auto widen<8>(v3    v) { return  V3x8(v); }
template<> auto widen<8>(v4    v) { return  V4x8(v); }
template<> auto widen<8>(v2s   v) { return V2sx8(v); }
template<> auto widen<8>(v3s   v) { return V3sx8(v); }
template<> auto widen<8>(v4s   v) { return V4sx8(v); }
template<> auto widen<8>(v2u   v) { return V2ux8(v); }
template<> auto widen<8>(v3u   v) { return V3ux8(v); }
template<> auto widen<8>(v4u   v) { return V4ux8(v); }
// clang-format on

namespace CE {
#define WIDEN(ty, tyx4, conx4) \
	constexpr tyx4 widen(ty a, ty b, ty c, ty d) { return CE::conx4(a, b, c, d); }
WIDEN(v2, v2x4, V2x4)
WIDEN(v2s, v2sx4, V2sx4)
WIDEN(v2u, v2ux4, V2ux4)
WIDEN(v3, v3x4, V3x4)
WIDEN(v3s, v3sx4, V3sx4)
WIDEN(v3u, v3ux4, V3ux4)
WIDEN(v4, v4x4, V4x4)
WIDEN(v4s, v4sx4, V4sx4)
WIDEN(v4u, v4ux4, V4ux4)
#undef WIDEN
#define WIDEN(ty, tyx8, conx8) \
	constexpr tyx8 widen(ty a, ty b, ty c, ty d, ty e, ty f, ty g, ty h) { return CE::conx8(a, b, c, d, e, f, g, h); }
WIDEN(v2, v2x8, V2x8)
WIDEN(v2s, v2sx8, V2sx8)
WIDEN(v2u, v2ux8, V2ux8)
WIDEN(v3, v3x8, V3x8)
WIDEN(v3s, v3sx8, V3sx8)
WIDEN(v3u, v3ux8, V3ux8)
WIDEN(v4, v4x8, V4x8)
WIDEN(v4s, v4sx8, V4sx8)
WIDEN(v4u, v4ux8, V4ux8)
#undef WIDEN
} // namespace CE

template <class To, class T>
auto wideCast(T v) = delete;

// clang-format off
template<> inline auto wideCast<f32>(v2s v) { return V2(v); }
template<> inline auto wideCast<f32>(v2sx4 v) { return V2x4(v); }
template<> inline auto wideCast<f32>(v2sx8 v) { return V2x8(v); }
template<> inline auto wideCast<f32>(v3s v) { return V3(v); }
template<> inline auto wideCast<f32>(v3sx4 v) { return V3x4(v); }
template<> inline auto wideCast<f32>(v3sx8 v) { return V3x8(v); }
template<> inline auto wideCast<f32>(v4s v) { return V4(v); }
template<> inline auto wideCast<f32>(v4sx4 v) { return V4x4(v); }
template<> inline auto wideCast<f32>(v4sx8 v) { return V4x8(v); }
// clang-format on

template <class T, size_t size>
struct Array {
	constexpr T& operator[](size_t i) { return v[i]; }
	constexpr T const& operator[](size_t i) const { return v[i]; }
	T v[size];
};

template <class T, size_t size>
struct WideType {};

// clang-format off
template<> struct WideType<v2, 4> { using Type = v2x4; };
template<> struct WideType<v3, 4> { using Type = v3x4; };
template<> struct WideType<v4, 4> { using Type = v4x4; };
template<> struct WideType<v2s, 4> { using Type = v2sx4; };
template<> struct WideType<v3s, 4> { using Type = v3sx4; };
template<> struct WideType<v4s, 4> { using Type = v4sx4; };
template<> struct WideType<v2u, 4> { using Type = v2ux4; };
template<> struct WideType<v3u, 4> { using Type = v3ux4; };
template<> struct WideType<v4u, 4> { using Type = v4ux4; };
template<> struct WideType<v2, 8> { using Type = v2x8; };
template<> struct WideType<v3, 8> { using Type = v3x8; };
template<> struct WideType<v4, 8> { using Type = v4x8; };
template<> struct WideType<v2s, 8> { using Type = v2sx8; };
template<> struct WideType<v3s, 8> { using Type = v3sx8; };
template<> struct WideType<v4s, 8> { using Type = v4sx8; };
template<> struct WideType<v2u, 8> { using Type = v2ux8; };
template<> struct WideType<v3u, 8> { using Type = v3ux8; };
template<> struct WideType<v4u, 8> { using Type = v4ux8; };
// clang-format on

template <class From, class ToScalar>
struct Convert {};

// clang-format off
template<> struct Convert<v2, s32> { using Type = v2s; };
template<> struct Convert<v3, s32> { using Type = v3s; };
template<> struct Convert<v4, s32> { using Type = v4s; };
template<> struct Convert<v2, u32> { using Type = v2u; };
template<> struct Convert<v3, u32> { using Type = v3u; };
template<> struct Convert<v4, u32> { using Type = v4u; };
template<> struct Convert<v2s, f32> { using Type = v2; };
template<> struct Convert<v3s, f32> { using Type = v3; };
template<> struct Convert<v4s, f32> { using Type = v4; };
template<> struct Convert<v2s, u32> { using Type = v2u; };
template<> struct Convert<v3s, u32> { using Type = v3u; };
template<> struct Convert<v4s, u32> { using Type = v4u; };
template<> struct Convert<v2u, f32> { using Type = v2; };
template<> struct Convert<v3u, f32> { using Type = v3; };
template<> struct Convert<v4u, f32> { using Type = v4; };
template<> struct Convert<v2u, s32> { using Type = v2s; };
template<> struct Convert<v3u, s32> { using Type = v3s; };
template<> struct Convert<v4u, s32> { using Type = v4s; };
// clang-format on

namespace CE {
template <class ToScalar, class From, size_t size>
constexpr auto wideCast(Array<From, size> const& arr) {
	using To = typename Convert<From, ToScalar>::Type;
	Array<To, size> result{};
	for (size_t i = 0; i < size; ++i) {
		result[i] = CE::cvt<To>(arr[i]);
	}
	return result;
}

template <class T, size_t count1, size_t count4, size_t count8>
struct WideArray {
	using T8 = typename WideType<T, 8>::Type;
	using T4 = typename WideType<T, 4>::Type;
	using T1 = T;
	T8 t8[count8]{};
	T4 t4[count4]{};
	T1 t1[count1]{};
	inline constexpr WideArray(T const (&arr)[count8 * 8 + count4 * 4 + count1]) {
		for (size_t i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (size_t i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[count8 * 8 + i * 4 + 0], arr[count8 * 8 + i * 4 + 1], arr[count8 * 8 + i * 4 + 2],
							  arr[count8 * 8 + i * 4 + 3]);
		}
		for (size_t i = 0; i < count1; ++i) {
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
template <class T, size_t count1>
struct WideArray<T, count1, 0, 0> {
	using T1 = T;
	T1 t1[count1]{};
	inline constexpr WideArray(T const (&arr)[count1]) {
		for (size_t i = 0; i < count1; ++i) {
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
template <class T, size_t count4>
struct WideArray<T, 0, count4, 0> {
	using T4 = typename WideType<T, 4>::Type;
	T4 t4[count4]{};
	inline constexpr WideArray(T const (&arr)[count4 * 4]) {
		for (size_t i = 0; i < count4; ++i) {
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
template <class T, size_t count1, size_t count4>
struct WideArray<T, count1, count4, 0> {
	using T4 = typename WideType<T, 4>::Type;
	using T1 = T;
	T4 t4[count4]{};
	T1 t1[count1]{};
	inline constexpr WideArray(T const (&arr)[count4 * 4 + count1]) {
		for (size_t i = 0; i < count4; ++i) {
			t4[i] = CE::widen(arr[i * 4 + 0], arr[i * 4 + 1], arr[i * 4 + 2], arr[i * 4 + 3]);
		}
		for (size_t i = 0; i < count1; ++i) {
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
template <class T, size_t count8>
struct WideArray<T, 0, 0, count8> {
	using T8 = typename WideType<T, 8>::Type;
	T8 t8[count8]{};
	inline constexpr WideArray(T const (&arr)[count8 * 8]) {
		for (size_t i = 0; i < count8; ++i) {
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
template <class T, size_t count1, size_t count8>
struct WideArray<T, count1, 0, count8> {
	using T8 = typename WideType<T, 8>::Type;
	using T1 = T;
	T8 t8[count8]{};
	T1 t1[count1]{};
	inline constexpr WideArray(T const (&arr)[count8 * 8 + count1]) {
		for (size_t i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (size_t i = 0; i < count1; ++i) {
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
template <class T, size_t count4, size_t count8>
struct WideArray<T, 0, count4, count8> {
	using T8 = typename WideType<T, 8>::Type;
	using T4 = typename WideType<T, 4>::Type;
	T8 t8[count8]{};
	T4 t4[count4]{};
	inline constexpr WideArray(T const (&arr)[count8 * 8 + count4 * 4]) {
		for (size_t i = 0; i < count8; ++i) {
			t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
							  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
		}
		for (size_t i = 0; i < count4; ++i) {
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
template <class T, size_t size>
inline constexpr auto makeWide(Array<T, size> const& arr) {
	constexpr size_t count8 = size / 8;
	constexpr size_t count4 = (size - count8 * 8) / 4;
	constexpr size_t count1 = size - count8 * 8 - count4 * 4;
	return WideArray<T, count1, count4, count8>(arr);
}
} // namespace CE

// for some reason compiler generates bad code for WideArray::forEach, so we don't use it by
// default. it's ~50-100 cycles faster if iterate raw arrays
#ifndef TL_USE_CONSTEXPR_WIDE
#define TL_USE_CONSTEXPR_WIDE 0
#endif
template <class T, class Random, size_t count1, size_t count4, size_t count8>
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
template <class T, class U, class Random, size_t count1, size_t count4, size_t count8>
FORCEINLINE f32 _voronoiS32(CE::WideArray<T, count1, count4, count8> const& wideOffsets, T tile, U rel, Random random) {
	f32 minDist = 1000;
	wideOffsets.forEach([&](auto const& offset) {
		static constexpr auto width = widthOf<decltype(offset)>;

		minDist = min(minDist, distanceSqr(widen<width>(rel),
										   random(widen<width>(tile) + offset) * 0.5f + wideCast<float>(offset)));
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
static constexpr auto voronoiOffsets2 = CE::wideCast<f32>(voronoiOffsets2s);
static constexpr auto voronoiOffsets3 = CE::wideCast<f32>(voronoiOffsets3s);
static constexpr auto voronoiOffsets4 = CE::wideCast<f32>(voronoiOffsets4s);
// clang-format on

template <class Random = Random>
inline f32 voronoi(v2 v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets2);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * (1 / sqrt2);
}
template <class Random = Random>
inline f32 voronoi(v3 v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets3);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * (1 / sqrt3);
}
template <class Random = Random>
inline f32 voronoi(v4 v, Random random = {}) {
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets4);
	return sqrt(_voronoiF32(wideOffsets, v, random)) * 0.5f;
}
template <class Random = Random>
inline f32 voronoi(v2s v, u32 cellSize, Random random = {}) {
	v2s flr							  = floor(v, cellSize);
	v2s tile						  = flr / (s32)cellSize;
	v2 rel							  = V2(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist						  = 1000;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets2s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	auto tile8 = V2sx8(tile);
	auto rel8 = V2x8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, distanceSqr(rel8, random(tile8 + offset) * 0.5f + V2x8(offset)));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) * 0.5f + V2(offset)));
	}
#endif
	return sqrt(minDist) * (1 / sqrt2);
}
template <class Random = Random>
inline f32 voronoi(v3s v, u32 cellSize, Random random = {}) {
	v3s flr							  = floor(v, cellSize);
	v3s tile						  = flr / (s32)cellSize;
	v3 rel							  = V3(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist						  = 1000;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets3s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	v3sx8 tile8 = V3sx8(tile);
	v3x8 rel8 = V3x8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, distanceSqr(rel8, random(tile8 + offset) * 0.5f + V3x8(offset)));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) * 0.5f + V3(offset)));
	}
#endif
	return sqrt(minDist) * (1 / sqrt3);
}
template <class Random = Random>
inline f32 voronoi(v4s v, u32 cellSize, Random random = {}) {
	v4s flr							  = floor(v, cellSize);
	v4s tile						  = flr / (s32)cellSize;
	v4 rel							  = V4(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist						  = 1000;
	static constexpr auto wideOffsets = CE::makeWide(voronoiOffsets4s);
#if TL_USE_CONSTEXPR_WIDE
	minDist = _voronoiS32(wideOffsets, tile, rel, random);
#else
	v4sx8 tile8 = V4sx8(tile);
	v4x8 rel8 = V4x8(rel);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, distanceSqr(rel8, random(tile8 + offset) * 0.5f + wideCast<float>(offset)));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, distanceSqr(rel, random(tile + offset) * 0.5f + V4(offset)));
	}
#endif
	return sqrt(minDist) * 0.5f;
}

bool raycastLine(v2 a, v2 b, v2 c, v2 d, v2& point, v2& normal) {
	v2 ba = b - a;
	v2 dc = d - c;
	v2 ac = a - c;

	v2 s = v2{ba.x, dc.x} * ac.y - v2{ba.y, dc.y} * ac.x;
	s /= ba.x * dc.y - dc.x * ba.y;

	if (s.x >= 0 && s.x <= 1 && s.y >= 0 && s.y <= 1) {
		point  = a + (s.y * ba);
		normal = c - d;
		normal = cross(normal);
		if (dot(b - a, normal) > 0)
			normal *= -1;
		return true;
	}
	return false;
}
bool raycastRect(v2 a, v2 b, v2 tile, v2 size, v2& point, v2& normal) {
	v2 points[4];
	v2 normals[4];
	v2 const w = size;
	// clang-format off
	bool hits[]{
		raycastLine(a, b, tile + v2{-w.x, w.y}, tile + v2{ w.x, w.y}, points[0], normals[0]),
		raycastLine(a, b, tile + v2{ w.x, w.y}, tile + v2{ w.x,-w.y}, points[1], normals[1]),
		raycastLine(a, b, tile + v2{ w.x,-w.y}, tile + v2{-w.x,-w.y}, points[2], normals[2]),
		raycastLine(a, b, tile + v2{-w.x,-w.y}, tile + v2{-w.x, w.y}, points[3], normals[3]),
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
bool raycastPlane(v3 a, v3 b, v3 p1, v3 p2, v3 p3, v3& point, v3& normal) {
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
/*
inline bool raycastBlock(v3 a, v3 b, v3 blk, Hit& hit, v3 blockDimensions) {
	Hit hits[6];
	bool results[6]{};
	f32 x		= blockDimensions.x;
	f32 y		= blockDimensions.y;
	f32 z		= blockDimensions.z;
	results[0]	= raycastPlane(a, b, blk + v3{x, y, z}, blk + v3{x, -y, z}, blk + v3{x, y, -z},
							   hits[0]); //+x
	results[1]	= raycastPlane(a, b, blk + v3{-x, y, z}, blk + v3{-x, y, -z}, blk + v3{-x, -y, z},
							   hits[1]); //-x
	results[2]	= raycastPlane(a, b, blk + v3{x, y, z}, blk + v3{x, y, -z}, blk + v3{-x, y, z},
							   hits[2]); //+y
	results[3]	= raycastPlane(a, b, blk + v3{x, -y, z}, blk + v3{-x, -y, z}, blk + v3{x, -y, -z},
							   hits[3]); //-y
	results[4]	= raycastPlane(a, b, blk + v3{x, y, z}, blk + v3{-x, y, z}, blk + v3{x, -y, z},
							   hits[4]); //+z
	results[5]	= raycastPlane(a, b, blk + v3{x, y, -z}, blk + v3{x, -y, -z}, blk + v3{-x, y, -z},
							   hits[5]); //-z
	s32 min		= -1;
	f32 minDist = FLT_MAX;
	for (s32 i = 0; i < 6; ++i) {
		if (results[i]) {
			auto lenSqr = lengthSqr(a - hits[i].p);
			if (lenSqr < minDist) {
				minDist = lenSqr;
				min		= i;
			}
		}
	}
	if (min == -1)
		return false;
	hit = hits[min];
	return true;
}
*/
template <class T, size_t size>
constexpr T linearSample(const T (&arr)[size], float t) noexcept {
	f32 f = frac(t) * size;
	s32 a = (s32)f;
	s32 b = a + 1;
	if (b == size)
		b = 0;
	return lerp(arr[a], arr[b], frac(f));
}
struct FrustumPlanes {
	v4 planes[6];
	FrustumPlanes(m4 vp) {
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
			p /= length(v3{p.x, p.y, p.z});
		}
	}
	bool containsSphere(v3 position, float radius) const {
		for (auto p : planes) {
			if (dot(v3{p.x, p.y, p.z}, position) + p.w + radius < 0) {
				return false;
			}
		}
		return true;
	}
};

namespace CE {

constexpr u32 countBits(u32 v) {
	u32 s = 0;
	u32 r = v;
	for (u32 i = 0; i < 32; ++i) {
		s += r & 1;
		r >>= 1;
	}
	return s;
}
constexpr v4s frac(v4s v, u32 step) {
	return {
		TL::frac(v.x, step),
		TL::frac(v.y, step),
		TL::frac(v.z, step),
		TL::frac(v.w, step),
	};
}

} // namespace CE

} // namespace TL

#undef MEMBERS2
#undef MEMBERS3
#undef MEMBERS4
#undef MEMFUNS_BASIC
#undef MEMFUNS_DATA
#undef MEMFUNS_INT
#undef MEMFUNS_SHIFT_SCL

#if COMPILER_MSVC
#pragma warning(pop)
#endif
