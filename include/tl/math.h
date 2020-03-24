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
#pragma warning(disable : 4201 4626)
#endif

namespace TL {

#define PI			 3.141592653f
#define DEG2RAD(deg) ((deg) / 360.0f * PI * 2)
#define RAD2DEG(rad) ((rad) / PI / 2 * 360.0f)
#define SQRT2		 1.414213538f
#define SQRT3		 1.732050776f
#define SQRT5		 2.23606801f

template <class T, class U>
constexpr auto min(T a, U b) {
	return a < b ? a : b;
}
template<class T, class U, class... Rest>
constexpr auto min(T a, U b, Rest... rest) {
	return min(min(a, b), rest...);
}
template <class T, class U>
constexpr auto max(T a, U b) {
	return a > b ? a : b;
}
template<class T, class U, class... Rest>
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
template <class T>
constexpr auto moveTowards(T a, T b, f32 t) {
	if (a < b) {
		return min(a + t, b);
	} else {
		return max(a - t, b);
	}
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

union f32x4;
union f32x8;
union s32x4;
union s32x8;
union u32x4;
union u32x8;
union v2;
union v3;
union v4;
union v2s;
union v3s;
union v4s;
union v2u;
union v3u;
union v4u;
union v4x2;
union v4sx2;
union v4ux2;
union v3x4;
union v3sx4;
union v3ux4;
union v2x8;
union v2sx8;
union v2ux8;
union v3x8;
union v3sx8;
union v3ux8;
union m4;

constexpr f32x4 F32x4(__m128);
constexpr s32x4 S32x4(__m128i);
constexpr u32x4 U32x4(__m128i);

constexpr f32x8 F32x8(__m256);
constexpr s32x8 S32x8(__m256i);
constexpr u32x8 U32x8(__m256i);

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

inline v4 V4(f32 = 0);
inline v4s V4s(s32 = 0);
inline v4u V4u(u32 = 0);

constexpr v2 V2(f32 x, f32 y);
constexpr v2s V2s(s32 x, s32 y);
constexpr v2u V2u(u32 x, u32 y);

constexpr v3 V3(f32 x, f32 y, f32 z);
constexpr v3s V3s(s32 x, s32 y, s32 z);
constexpr v3u V3u(u32 x, u32 y, u32 z);

inline v4 V4(f32 x, f32 y, f32 z, f32 w);
inline v4s V4s(s32 x, s32 y, s32 z, s32 w);
inline v4u V4u(u32 x, u32 y, u32 z, u32 w);

constexpr v4 V4(f32x4);
constexpr v4s V4s(s32x4);
constexpr v4u V4u(u32x4);

constexpr v4x2 V4x2(f32x8);
constexpr v4sx2 V4sx2(s32x8);
constexpr v4ux2 V4ux2(u32x8);

constexpr v4x2 V4x2(f32 v);
constexpr v4x2 V4x2(v4 a, v4 b);

constexpr v4sx2 V4sx2(s32 v);
constexpr v4sx2 V4sx2(v4s a, v4s b);

constexpr v4ux2 V4ux2(u32 v);
constexpr v4ux2 V4ux2(v4u a, v4u b);

constexpr v3x4 V3x4(__m128 x, __m128 y, __m128 z);
constexpr v3x4 V3x4(v4 x, v4 y, v4 z);
inline v3x4 V3x4(f32 v);

constexpr v3sx4 V3sx4(__m128i x, __m128i y, __m128i z);
constexpr v3sx4 V3sx4(v4s x, v4s y, v4s z);
inline v3sx4 V3sx4(s32 v);

constexpr v3ux4 V3ux4(__m128i x, __m128i y, __m128i z);
constexpr v3ux4 V3ux4(v4u x, v4u y, v4u z);
inline v3ux4 V3ux4(u32 v);

inline v3x8 V3x8(f32 v);
inline v3sx8 V3sx8(s32 v);
inline v3ux8 V3ux8(u32 v);

inline v2x8 V2x8(f32 v);
inline v2sx8 V2sx8(s32 v);
inline v2ux8 V2ux8(u32 v);

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

#define MEMBERS2(scl) \
	struct {          \
		scl x, y;     \
	};                \
	scl v[2]

#define MEMBERS3(scl, v2) \
	struct {              \
		scl x, y, z;      \
	};                    \
	v2 xy;                \
	struct {              \
		scl _pad0;        \
		v2 yz;            \
	};                    \
	scl v[3]

#define MEMBERS4(scl, v2, v3) \
	struct {                  \
		scl x, y, z, w;       \
	};                        \
	v3 xyz;                   \
	struct {                  \
		scl _pad0;            \
		union {               \
			v2 yz;            \
			v3 yzw;           \
		};                    \
	};                        \
	struct {                  \
		v2 xy;                \
		v2 zw;                \
	};                        \
	scl v[4]

#define MEMFUNS_BASIC(vec, scl, con)                          \
	vec& operator+=(vec b) { return *this = *this + b; }      \
	vec& operator-=(vec b) { return *this = *this - b; }      \
	vec& operator*=(vec b) { return *this = *this * b; }      \
	vec& operator/=(vec b) { return *this = *this / b; }      \
	bool operator!=(vec b) const { return !(*this == b); }    \
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

#define MEMFUNS_DATA(scl)     \
	scl* data() { return v; } \
	scl const* data() const { return v; } \
	scl& operator[](size_t i) { return v[i]; } \
	scl operator[](size_t i) const { return v[i]; }

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
	MEMFUNS_DATA(f32);
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
	MEMFUNS_DATA(f32);
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
	s32x4 operator-() const { return S32x4(_mm_sub_epi32(_mm_set1_epi32(0), m)); }
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

union u32x8 {
	u32 v[8];
	__m256i m;
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
	u32x8 operator<<(u32 b) const { return U32x8(_mm256_slli_epi32(m, b)); }
	u32x8 operator>>(u32 b) const { return U32x8(_mm256_srli_epi32(m, b)); }
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
};
constexpr u32x8 U32x8(__m256i m) {
	u32x8 r{};
	r.m = m;
	return r;
}
inline u32x8 U32x8(u32 v) { return U32x8(_mm256_set1_epi32(v)); }
inline u32x8 U32x8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) {
	return U32x8(_mm256_setr_epi32(a, b, c, d, e, f, g, h));
}
inline u32x8 U32x8(u32 a, u32 b, u32 c, u32 d) { return U32x8(_mm256_setr_epi32(a, b, c, d, a, b, c, d)); }

union u32x4 {
	u32 v[4];
	struct {
		u32 i, j, k, l;
	};
	__m128i m;
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
};
constexpr u32x4 U32x4(__m128i m) {
	u32x4 r{};
	r.m = m;
	return r;
}
inline u32x4 U32x4(u32 i, u32 j, u32 k, u32 l) { return U32x4(_mm_setr_epi32((u32)i, (u32)j, (u32)k, (u32)l)); }
inline u32x4 U32x4(u32 v) { return U32x4(_mm_set1_epi32(v)); }

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

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_DATA(scl)
#define OP(op) \
	constexpr v2 operator op(v2 b) const { return {x op b.x, y op b.y}; }
union v2 {
	MEMBERS2(f32);
	OP(+) OP(-) OP(*) OP(/);
	MEMFUNS(v2, f32, V2);
	constexpr v2 operator-() const { return {-x, -y}; }
	constexpr bool operator==(v2 b) const { return x == b.x && y == b.y; }
};
#undef OP

#define OP(op) \
	constexpr v3 operator op(v3 b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3 {
	MEMBERS3(f32, v2);
	OP(+) OP(-) OP(*) OP(/);
	MEMFUNS(v3, f32, V3);
	v3 operator-() const { return {-x, -y, -z}; }
	bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; }
};
#undef OP

#define OP(op) \
	constexpr v4 operator op(v4 b) const { return V4(m op b.m); }
union v4 {
	MEMBERS4(f32, v2, v3);
	OP(+) OP(-) OP(*) OP(/);
	MEMFUNS(v4, f32, V4);
	f32x4 m;
	v4 operator-() const { return V4(-m); }
	bool operator==(v4 b) const { return m == b.m; }
};
#undef MEMFUNS
#undef OP

#define MEMFUNS(vec, scl, con) \
	MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL(vec, scl, con)
#define OP(op) \
	constexpr v2s operator op(v2s b) const { return {x op b.x, y op b.y}; }
union v2s {
	MEMBERS2(s32);
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	MEMFUNS(v2s, s32, V2s);
	v2s operator-() const { return {-x, -y}; }
	bool operator==(v2s b) const { return x == b.x && y == b.y; }
	explicit operator v2() const { return {(f32)x, (f32)y}; }
};
#undef OP

#define OP(op) \
	constexpr v3s operator op(v3s b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3s {
	MEMBERS3(s32, v2s);
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	MEMFUNS(v3s, s32, V3s);
	v3s operator-() const { return {-x, -y, -z}; }
	bool operator==(v3s b) const { return x == b.x && y == b.y && z == b.z; }
};
#undef OP

#define OP(op) \
	constexpr v2u operator op(v2u b) const { return {x op b.x, y op b.y}; }
union v2u {
	MEMBERS2(u32);
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	MEMFUNS(v2u, u32, V2u);
	bool operator==(v2u b) const { return x == b.x && y == b.y; }
	explicit operator v2() const { return {(f32)x, (f32)y}; }
	explicit operator v2s() const { return {(s32)x, (s32)y}; }
};
#undef OP

#define OP(op) \
	constexpr v3u operator op(v3u b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3u {
	MEMBERS3(u32, v2u);
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	MEMFUNS(v3u, u32, V3u);
	bool operator==(v3u b) const { return x == b.x && y == b.y && z == b.z; }
};
#undef OP
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
#define OP(op) \
	v4s operator op(v4s b) const { return V4s(m op b.m); }
union v4s {
	MEMBERS4(s32, v2s, v3s);
	OP(+) OP(-) OP(*) OP(^) OP(|) OP(&) OP(<<) OP(>>) OP(/) OP(%);
	MEMFUNS(v4s, s32, V4s);
	s32x4 m;
	v4s operator-() const { return V4s(-m); }
	v4s operator<<(s32 b) const { return V4s(m << b); }
	v4s operator>>(s32 b) const { return V4s(m >> b); }
	bool operator==(v4s b) const { return m == b.m; }
};
#undef OP

#define OP(op) \
	v4u operator op(v4u b) const { return V4u(m op b.m); }
union v4u {
	MEMBERS4(u32, v2u, v3u);
	OP(+) OP(-) OP(*) OP(^) OP(|) OP(&) OP(<<) OP(>>) OP(/) OP(%);
	MEMFUNS(v4u, u32, V4u);
	u32x4 m;
	v4u operator<<(u32 b) const { return V4u(m << b); }
	v4u operator>>(u32 b) const { return V4u(m >> b); }
	bool operator==(v4u b) const { return m == b.m; }
}; // namespace TL
#undef OP
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)
#define OP(op) \
	v4x2 operator op(v4x2 b) const { return V4x2(m op b.m); }
union v4x2 {
	struct {
		v4 i, j;
	};
	f32x8 m;
	v4x2 operator-() const { return V4x2(-m); }
	OP(+) OP(-) OP(*) OP(/);
	bool operator==(v4x2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4x2, f32, V4x2);
};
#undef OP
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
#define OP(op) \
	v4sx2 operator op(v4sx2 b) const { return V4sx2(m op b.m); }
union v4sx2 {
	struct {
		v4s i, j;
	};
	s32x8 m;
	v4sx2 operator-() const { return V4sx2(-m); }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	v4sx2 operator<<(s32 b) const { return V4sx2(m << b); }
	v4sx2 operator>>(s32 b) const { return V4sx2(m << b); }
	bool operator==(v4sx2 b) const { return m == b.m; }
	MEMFUNS(v4sx2, s32, V4sx2);
};
#undef OP
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
#define OP(op) \
	v4ux2 operator op(v4ux2 b) const { return V4ux2(m op b.m); }
union v4ux2 {
	struct {
		v4u i, j;
	};
	u32x8 m;
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	v4ux2 operator<<(u32 b) const { return V4ux2(m << b); }
	v4ux2 operator>>(u32 b) const { return V4ux2(m << b); }
	bool operator==(v4ux2 b) const { return m == b.m; }
	MEMFUNS(v4ux2, u32, V4ux2);
};
#undef OP
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)
union v3x4 {
	struct {
		f32x4 x, y, z;
	};
	v3x4 operator-() const { return {-x, -y, -z}; }
	v3x4 operator+(v3x4 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3x4 operator-(v3x4 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3x4 operator*(v3x4 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3x4 operator/(v3x4 b) const { return {x / b.x, y / b.y, z / b.z}; }
	bool operator==(v3x4 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3x4, f32, V3x4);
};

union v3x8 {
	struct {
		f32x8 x, y, z;
	};
	v3x8 operator-() const { return {-x, -y, -z}; }
	v3x8 operator+(v3x8 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3x8 operator-(v3x8 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3x8 operator*(v3x8 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3x8 operator/(v3x8 b) const { return {x / b.x, y / b.y, z / b.z}; }
	bool operator==(v3x8 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3x8, f32, V3x8);
};

union v2x8 {
	struct {
		f32x8 x, y;
	};
	v2x8 operator+(v2x8 b) const { return {x + b.x, y + b.y}; }
	v2x8 operator-(v2x8 b) const { return {x - b.x, y - b.y}; }
	v2x8 operator*(v2x8 b) const { return {x * b.x, y * b.y}; }
	v2x8 operator/(v2x8 b) const { return {x / b.x, y / b.y}; }
	bool operator==(v2x8 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2x8, f32, V2x8);
};
#undef MEMFUNS

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL(vec, scl, con)
#define OP(op) \
	v3sx4 operator op(v3sx4 b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3sx4 {
	struct {
		s32x4 x, y, z;
	};
	v3sx4 operator-() const { return {-x, -y, -z}; }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	bool operator==(v3sx4 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3sx4, s32, V3sx4);
};
#undef OP

#define OP(op) \
	v3sx8 operator op(v3sx8 b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3sx8 {
	struct {
		s32x8 x, y, z;
	};
	v3sx8 operator-() const { return {-x, -y, -z}; }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	bool operator==(v3sx8 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3sx8, s32, V3sx8);
};
#undef OP

#define OP(op) \
	v3ux4 operator op(v3ux4 b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3ux4 {
	struct {
		u32x4 x, y, z;
	};
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	bool operator==(v3ux4 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3ux4, u32, V3ux4);
};
#undef OP

#define OP(op) \
	v3ux8 operator op(v3ux8 b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3ux8 {
	struct {
		u32x8 x, y, z;
	};
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	bool operator==(v3ux8 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3ux8, u32, V3ux8);
};
#undef OP

union v2sx8 {
	struct {
		s32x8 x, y;
	};
	v2sx8 operator+(v2sx8 b) const { return {x + b.x, y + b.y}; }
	v2sx8 operator-(v2sx8 b) const { return {x - b.x, y - b.y}; }
	v2sx8 operator*(v2sx8 b) const { return {x * b.x, y * b.y}; }
	v2sx8 operator/(v2sx8 b) const { return {x / b.x, y / b.y}; }
	v2sx8 operator%(v2sx8 b) const { return {x % b.x, y % b.y}; }
	v2sx8 operator^(v2sx8 b) const { return {x ^ b.x, y ^ b.y}; }
	v2sx8 operator|(v2sx8 b) const { return {x | b.x, y | b.y}; }
	v2sx8 operator&(v2sx8 b) const { return {x & b.x, y & b.y}; }
	v2sx8 operator<<(v2sx8 b) const { return {x << b.x, y << b.y}; }
	v2sx8 operator>>(v2sx8 b) const { return {x >> b.x, y >> b.y}; }
	bool operator==(v2sx8 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2sx8, s32, V2sx8);
};
union v2ux8 {
	struct {
		u32x8 x, y;
	};
	v2ux8 operator+(v2ux8 b) const { return {x + b.x, y + b.y}; }
	v2ux8 operator-(v2ux8 b) const { return {x - b.x, y - b.y}; }
	v2ux8 operator*(v2ux8 b) const { return {x * b.x, y * b.y}; }
	v2ux8 operator/(v2ux8 b) const { return {x / b.x, y / b.y}; }
	v2ux8 operator%(v2ux8 b) const { return {x % b.x, y % b.y}; }
	v2ux8 operator^(v2ux8 b) const { return {x ^ b.x, y ^ b.y}; }
	v2ux8 operator|(v2ux8 b) const { return {x | b.x, y | b.y}; }
	v2ux8 operator&(v2ux8 b) const { return {x & b.x, y & b.y}; }
	v2ux8 operator<<(v2ux8 b) const { return {x << b.x, y << b.y}; }
	v2ux8 operator>>(v2ux8 b) const { return {x >> b.x, y >> b.y}; }
	bool operator==(v2ux8 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2ux8, u32, V2ux8);
};

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

union alignas(64) m4 {
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

template <class T>
struct m128 {};
template <class T>
struct m256 {};

template <>
struct m128<v3> {
	using Type = v3x4;
};
template <>
struct m256<v3> {
	using Type = v3x8;
};

#undef MEMFUNS

#define CON(scl, vec2, con)                             \
	constexpr vec2 con(scl x, scl y) { return {x, y}; } \
	constexpr vec2 con(scl v) { return {v, v}; }
CON(f32, v2, V2);
CON(s32, v2s, V2s);
CON(u32, v2u, V2u);

constexpr v2 V2(v2s v) { return {(f32)v.x, (f32)v.y}; }
constexpr v2 V2(v2u v) { return {(f32)v.x, (f32)v.y}; }
constexpr v2s V2s(v2 v) { return {(s32)v.x, (s32)v.y}; }
constexpr v2s V2s(v2u v) { return {(s32)v.x, (s32)v.y}; }
constexpr v2u V2u(v2 v) { return {(u32)v.x, (u32)v.y}; }
constexpr v2u V2u(v2s v) { return {(u32)v.x, (u32)v.y}; }

#undef CON
#define CON(scl, vec2, vec3, con)                                  \
	constexpr vec3 con(scl x, scl y, scl z) { return {x, y, z}; }  \
	constexpr vec3 con(scl v) { return {v, v, v}; }                \
	constexpr vec3 con(vec2 xy, scl z) { return {xy.x, xy.y, z}; } \
	constexpr vec3 con(scl x, vec2 yz) { return {x, yz.x, yz.y}; }

CON(f32, v2, v3, V3);
CON(s32, v2s, v3s, V3s);
CON(u32, v2u, v3u, V3u);
#undef CON

constexpr v3 V3(v3s v) { return V3((f32)v.x, (f32)v.y, (f32)v.z); }
constexpr v3 V3(v3u v) { return V3((f32)v.x, (f32)v.y, (f32)v.z); }
constexpr v3s V3s(v3 v) { return V3s((s32)v.x, (s32)v.y, (s32)v.z); }
constexpr v3s V3s(v3u v) { return V3s((s32)v.x, (s32)v.y, (s32)v.z); }
constexpr v3u V3u(v3 v) { return V3u((u32)v.x, (u32)v.y, (u32)v.z); }
constexpr v3u V3u(v3s v) { return V3u((u32)v.x, (u32)v.y, (u32)v.z); }

#define CON(scl, vec2, vec3, vec4, con, x4)                                \
	inline vec4 con(scl x, scl y, scl z, scl w) { return {x, y, z, w}; }   \
	inline vec4 con(scl v) { return {v, v, v, v}; }                        \
	inline vec4 con(vec2 xy, vec2 zw) { return {xy.x, xy.y, zw.x, zw.y}; } \
	inline vec4 con(vec2 xy, scl z, scl w) { return {xy.x, xy.y, z, w}; }  \
	inline vec4 con(scl x, scl y, vec2 zw) { return {x, y, zw.x, zw.y}; }  \
	inline vec4 con(vec3 xyz, scl w) { return {xyz.x, xyz.y, xyz.z, w}; }  \
	inline vec4 con(scl x, vec3 yzw) { return {x, yzw.x, yzw.y, yzw.z}; }  \
	constexpr vec4 con(x4 v) {                                             \
		vec4 r{};                                                          \
		r.m = v;                                                           \
		return r;                                                          \
	}

CON(f32, v2, v3, v4, V4, f32x4);
CON(s32, v2s, v3s, v4s, V4s, s32x4);
CON(u32, v2u, v3u, v4u, V4u, u32x4);
#undef CON

constexpr v4 V4(__m128 m) {
	v4 r{};
	r.m.m = m;
	return r;
}
constexpr v4s V4s(__m128i m) {
	v4s r{};
	r.m.m = m;
	return r;
}
constexpr v4u V4u(__m128i m) {
	v4u r{};
	r.m.m = m;
	return r;
}

inline v4 V4(v4s v) { return V4(F32x4(v.m)); }
inline v4 V4(v4u v) { return V4(F32x4(v.m)); }
inline v4s V4s(v4 v) { return V4s(S32x4(v.m)); }
inline v4s V4s(v4u v) { return V4s(S32x4(v.m)); }
inline v4u V4u(v4 v) { return V4u(U32x4(v.m)); }
inline v4u V4u(v4s v) { return V4u(U32x4(v.m)); }

#define V4X2(scl, v4, v4x2, V4, V4x2, sclx8, Sclx8, v4x2o1, v4x2o2)                     \
	constexpr v4x2 V4x2(scl v) { return V4x2(Sclx8(v)); }                               \
	constexpr v4x2 V4x2(scl a, scl b, scl c, scl d, scl e, scl f, scl g, scl h) {       \
		return V4x2(Sclx8(a, b, c, d, e, f, g, h));                                     \
	}                                                                                   \
	constexpr v4x2 V4x2(scl a, scl b, scl c, scl d) { return V4x2(Sclx8(a, b, c, d)); } \
	constexpr v4x2 V4x2(sclx8 v) {                                                      \
		v4x2 r{};                                                                       \
		r.m = v;                                                                        \
		return r;                                                                       \
	}                                                                                   \
	constexpr v4x2 V4x2(v4x2o1 v) { return V4x2(V4(v.i), V4(v.j)); }                    \
	constexpr v4x2 V4x2(v4x2o2 v) { return V4x2(V4(v.i), V4(v.j)); }                    \
	constexpr v4x2 V4x2(v4 a) { return {a, a}; }                                        \
	constexpr v4x2 V4x2(v4 a, v4 b) { return {a, b}; }

V4X2(f32, v4, v4x2, V4, V4x2, f32x8, F32x8, v4sx2, v4ux2)
V4X2(s32, v4s, v4sx2, V4s, V4sx2, s32x8, S32x8, v4x2, v4ux2)
V4X2(u32, v4u, v4ux2, V4u, V4ux2, u32x8, U32x8, v4x2, v4sx2)
#undef V4X2

#define V2X8(v2x8, V2x8, v2, V4x2, f32, f32x8, F32x8, v4x2)                                                          \
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
	inline v2x8 V2x8(v2 v) { return V2x8(F32x8(v.x), F32x8(v.y)); }

V2X8(v2x8, V2x8, v2, V4x2, f32, f32x8, F32x8, v4x2);
V2X8(v2sx8, V2sx8, v2s, V4sx2, s32, s32x8, S32x8, v4sx2);
V2X8(v2ux8, V2ux8, v2u, V4ux2, u32, u32x8, U32x8, v4ux2);
#undef V2X8

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

inline v3x8 V3x8(f32x8 x, f32x8 y, f32x8 z) { return {x, y, z}; }
inline v3x8 V3x8(f32 v) { return {F32x8(v), F32x8(v), F32x8(v)}; }
inline v3x8 V3x8(v3 v) { return {F32x8(v.x), F32x8(v.y), F32x8(v.z)}; }
inline v3sx8 V3sx8(s32x8 x, s32x8 y, s32x8 z) { return {x, y, z}; }
inline v3sx8 V3sx8(s32 v) { return {S32x8(v), S32x8(v), S32x8(v)}; }
inline v3sx8 V3sx8(v3s v) { return {S32x8(v.x), S32x8(v.y), S32x8(v.z)}; }
inline v3ux8 V3ux8(u32x8 x, u32x8 y, u32x8 z) { return {x, y, z}; }
inline v3ux8 V3ux8(u32 v) { return {U32x8(v), U32x8(v), U32x8(v)}; }
inline v3ux8 V3ux8(v3u v) { return {U32x8(v.x), U32x8(v.y), U32x8(v.z)}; }

inline v3x8 V3x8(v3sx8 v) { return {F32x8(v.x), F32x8(v.y), F32x8(v.z)}; }
inline v3x8 V3x8(v3ux8 v) { return {F32x8(v.x), F32x8(v.y), F32x8(v.z)}; }
inline v3sx8 V3sx8(v3x8 v) { return {S32x8(v.x), S32x8(v.y), S32x8(v.z)}; }
inline v3sx8 V3sx8(v3ux8 v) { return {S32x8(v.x), S32x8(v.y), S32x8(v.z)}; }
inline v3ux8 V3ux8(v3x8 v) { return {U32x8(v.x), U32x8(v.y), U32x8(v.z)}; }
inline v3ux8 V3ux8(v3sx8 v) { return {U32x8(v.x), U32x8(v.y), U32x8(v.z)}; }

inline v2x8  V2x8 (v2sx8 v) { return {F32x8(v.x), F32x8(v.y)}; }
inline v2x8  V2x8 (v2ux8 v) { return {F32x8(v.x), F32x8(v.y)}; }
inline v2sx8 V2sx8(v2x8  v) { return {S32x8(v.x), S32x8(v.y)}; }
inline v2sx8 V2sx8(v2ux8 v) { return {S32x8(v.x), S32x8(v.y)}; }
inline v2ux8 V2ux8(v2x8  v) { return {U32x8(v.x), U32x8(v.y)}; }
inline v2ux8 V2ux8(v2sx8 v) { return {U32x8(v.x), U32x8(v.y)}; }

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

constexpr v3x4 V3x4(v3 v) { return {v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z}; }
constexpr v3sx4 V3sx4(v3s v) { return {v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z}; }

constexpr v3sx4 V3sx4(s32 ix, s32 iy, s32 iz, s32 jx, s32 jy, s32 jz, s32 kx, s32 ky, s32 kz, s32 lx, s32 ly, s32 lz) {
	return {ix, jx, kx, lx, iy, jy, ky, ly, iz, jz, kz, lz};
}

constexpr v3sx4 V3sx4(v3s a, v3s b, v3s c, v3s d) {
	return {
		a.x, b.x, c.x, d.x, a.y, b.y, c.y, d.y, a.z, b.z, c.z, d.z,
	};
}

constexpr v3x8 V3x8(v3 v) {
	return {v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y,
			v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z, v.z, v.z, v.z, v.z};
}
constexpr v3sx8 V3sx8(v3s v) {
	return {v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y,
			v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z, v.z, v.z, v.z, v.z};
}

constexpr v3sx8 V3sx8(s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2, s32 x3, s32 y3, s32 z3,
					  s32 x4, s32 y4, s32 z4, s32 x5, s32 y5, s32 z5, s32 x6, s32 y6, s32 z6, s32 x7, s32 y7, s32 z7) {
	return {x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3, y4, y5, y6, y7, z0, z1, z2, z3, z4, z5, z6, z7};
}

constexpr v3sx8 V3sx8(v3s a, v3s b, v3s c, v3s d, v3s e, v3s f, v3s g, v3s h) {
	return {
		a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x, a.y, b.y, c.y, d.y,
		e.y, f.y, g.y, h.y, a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z,
	};
}

constexpr v2sx8 V2sx8(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 x4, s32 y4, s32 x5, s32 y5,
					  s32 x6, s32 y6, s32 x7, s32 y7) {
	return {x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3, y4, y5, y6, y7};
}

constexpr v4sx2 V4sx2(v4s a, v4s b) { return {a, b}; }

} // namespace CE

f32 min(f32x8 v) {
	return min(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

// clang-format off
template<> auto min(f32x4 a, f32x4 b) { return F32x4(_mm_min_ps(a.m, b.m)); }
template<> auto min(s32x4 a, s32x4 b) { return S32x4(_mm_min_epi32(a.m, b.m)); }
template<> auto min(u32x4 a, u32x4 b) { return U32x4(_mm_min_epu32(a.m, b.m)); }
template<> auto min(v2 a, v2 b) { return V2(min(a.x, b.x), min(a.y, b.y)); }
template<> auto min(v3 a, v3 b) { return V3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
template<> auto min(v4 a, v4 b) { return V4(min(a.m, b.m)); }
template<> auto min(v2s a, v2s b) { return V2s(min(a.x, b.x), min(a.y, b.y)); }
template<> auto min(v3s a, v3s b) { return V3s(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
template<> auto min(v4s a, v4s b) { return V4s(min(a.m, b.m)); }
template<> auto min(v2u a, v2u b) { return V2u(min(a.x, b.x), min(a.y, b.y)); }
template<> auto min(v3u a, v3u b) { return V3u(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
template<> auto min(v4u a, v4u b) { return V4u(min(a.m, b.m)); }
template<> auto min(f32 a, v2 b) { return min(a, min(b.x, b.y)); }
template<> auto min(f32 a, v4 b) { return min(a, min(min(b.x, b.y), min(b.z, b.w))); }
template<> auto min(f32 a, v4x2 b) { return min(a, min(b.i, b.j)); } 
template<> auto min(f32 a, f32x8 b) { return min(a, min(b)); } 

template<> auto max(f32x4 a, f32x4 b) { return F32x4(_mm_max_ps(a.m, b.m)); }
template<> auto max(s32x4 a, s32x4 b) { return S32x4(_mm_max_epi32(a.m, b.m)); }
template<> auto max(u32x4 a, u32x4 b) { return U32x4(_mm_max_epu32(a.m, b.m)); }
template<> auto max(v2 a, v2 b) { return V2(max(a.x, b.x), max(a.y, b.y)); }
template<> auto max(v3 a, v3 b) { return V3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
template<> auto max(v4 a, v4 b) { return V4(max(a.m, b.m)); }
template<> auto max(v2s a, v2s b) { return V2s(max(a.x, b.x), max(a.y, b.y)); }
template<> auto max(v3s a, v3s b) { return V3s(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
template<> auto max(v4s a, v4s b) { return V4s(max(a.m, b.m)); }
template<> auto max(v2u a, v2u b) { return V2u(max(a.x, b.x), max(a.y, b.y)); }
template<> auto max(v3u a, v3u b) { return V3u(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
template<> auto max(v4u a, v4u b) { return V4u(max(a.m, b.m)); }
template<> auto max(f32 a, v2 b) { return max(a, max(b.x, b.y)); }
template<> auto max(f32 a, v4 b) { return max(a, max(max(b.x, b.y), max(b.z, b.w))); }
template<> auto max(f32 a, v4x2 b) { return max(a, max(b.i, b.j)); }
#if ARCH_AVX
template<> auto min(f32x8 a, f32x8 b) { return F32x8(_mm256_min_ps(a.m, b.m)); }
template<> auto max(f32x8 a, f32x8 b) { return F32x8(_mm256_max_ps(a.m, b.m)); }
#else
template<> auto min(f32x8 a, f32x8 b) { return F32x8(min(a.im, b.im), min(a.jm, b.jm)); }
template<> auto max(f32x8 a, f32x8 b) { return F32x8(max(a.im, b.im), max(a.jm, b.jm)); }
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

template <>
constexpr auto moveTowards(v2 a, v2 b, f32 t) {
	return v2{
		moveTowards(a.x, b.x, t),
		moveTowards(a.y, b.y, t),
	};
}

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

constexpr f32 aspectRatio(v2 v) { return v.x / v.y; }
constexpr f32 aspectRatio(v2s v) { return aspectRatio(V2(v)); }
constexpr f32 aspectRatio(v2u v) { return aspectRatio(V2(v)); }

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
v2 frac(v2 v) {
	return {
		frac(v.x),
		frac(v.y),
	};
}
v3 frac(v3 v) {
	return {
		frac(v.x),
		frac(v.y),
		frac(v.z),
	};
}
f32x4 frac(f32x4 v) {
	v.m = _mm_sub_ps(v.m, _mm_cvtepi32_ps(_mm_cvtps_epi32(v.m)));
	v.m = _mm_add_ps(v.m, _mm_and_ps(_mm_cmplt_ps(v.m, _mm_setzero_ps()), _mm_set1_ps(1.0f)));
	return v;
}
v4 frac(v4 v) { return V4(frac(v.m)); }
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
#if 1
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
		floor(v.w, step),
	};
#else
	__m128i ms = _mm_set1_epi32((s32)step);
	__m128i one = _mm_set1_epi32(1);
	__m128i pos = _mm_div_epi32(v.m, ms);
	__m128i neg = _mm_sub_epi32(_mm_div_epi32(_mm_add_epi32(v.m, one), ms), one);
	__m128i mask = _mm_cmplt_epi32(v.m, _mm_setzero_si128());
	return V4s(_mm_mullo_epi32(_mm_or_si128(_mm_and_si128(mask, neg), _mm_andnot_si128(mask, pos)),
							   _mm_set1_epi32((s32)step)));
#endif
}
f32 dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
f32 dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
f32 dot(f32x4 a, f32x4 b) {
	f32 result;
	_mm_store_ss(&result, _mm_dp_ps(a.m, b.m, 0xFF));
	return result;
}
f32 dot(v4 a, v4 b) { return dot(a.m, b.m); }
f32x4 dot(v3x4 a, v3x4 b) {
	a *= b;
	return a.x + a.y + a.z;
}
f32x8 dot(v3x8 a, v3x8 b) {
	a *= b;
	return a.x + a.y + a.z;
}
f32x8 dot(v2x8 a, v2x8 b) {
	a *= b;
	return a.x + a.y;
}
s32 dot(v2s a, v2s b) { return a.x * b.x + a.y * b.y; }
s32 dot(v3s a, v3s b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
s32 dot(v4s a, v4s b) {
	a *= b;
	return a.x + a.y + a.z + a.w;
}
#if ARCH_AVX
v2 dot(v4x2 a, v4x2 b) {
	auto dp = _mm256_dp_ps(a.m.m, b.m.m, 0xF1);
	return V2(dp.m256_f32[0], dp.m256_f32[4]);
}
#else
v2 dot(v4x2 a, v4x2 b) { return V2(dot(a.i, b.i), dot(a.j, b.j)); }
#endif
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

f32 sign(f32 v) { return v > 0 ? 1 : v < 0 ? -1 : 0; }

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
inline f32 cos01(f32 t) { return 0.5f - cosf(t * PI) * 0.5f; }
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
inline static constexpr v2 _randMul2{PI, SQRT2 * 2};
inline static constexpr v3 _randMul3{PI, SQRT2 * 2, SQRT3 * 3};
inline static constexpr v4 _randMul4{PI, SQRT2 * 2, SQRT3 * 3, SQRT5};

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
u32 randomize(u32 v) { return rotateLeft(((v + 0x0C252DA0) * 0x034FB5E7) ^ 0xF5605798, 16); }
s32 randomize(s32 v) { return randomize((u32)v); }
v2s randomize(v2s v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v.x = v.x * v.y + v.x;
	v.y = v.x * v.y + v.x;
	return v;
}
v2sx8 randomize(v2sx8 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v.x = v.x * v.y + v.x;
	v.y = v.x * v.y + v.x;
	return v;
}
v3s randomize(v3s v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v *= V3s(v.z, v.x, v.y);
	v *= V3s(v.z, v.x, v.y);
	v *= V3s(v.z, v.x, v.y);
	return v;
}
__forceinline v3sx4 randomize(v3sx4 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v *= V3sx4(v.z, v.x, v.y);
	v *= V3sx4(v.z, v.x, v.y);
	v *= V3sx4(v.z, v.x, v.y);
	return v;
}
v3sx8 randomize(v3sx8 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v *= V3sx8(v.z, v.x, v.y);
	v *= V3sx8(v.z, v.x, v.y);
	v *= V3sx8(v.z, v.x, v.y);
	return v;
}
v4s randomize(v4s v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v ^= V4s(V4u(v) >> 31);
	v *= V4s(_mm_shuffle_epi32(v.m.m, _MM_SHUFFLE(2, 1, 0, 3)));
	v *= V4s(_mm_shuffle_epi32(v.m.m, _MM_SHUFFLE(2, 1, 0, 3)));
	v *= V4s(_mm_shuffle_epi32(v.m.m, _MM_SHUFFLE(2, 1, 0, 3)));
	return v;
}
#if ARCH_AVX
v4sx2 randomize(v4sx2 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v ^= V4sx2(V4ux2(v) >> 31);
	v *= V4sx2(S32x8(_mm256_shuffle_epi32(v.m.m, _MM_SHUFFLE(2, 1, 0, 3))));
	v *= V4sx2(S32x8(_mm256_shuffle_epi32(v.m.m, _MM_SHUFFLE(2, 1, 0, 3))));
	v *= V4sx2(S32x8(_mm256_shuffle_epi32(v.m.m, _MM_SHUFFLE(2, 1, 0, 3))));
	return v;
}
#else
v4sx2 randomize(v4sx2 v) { return V4sx2(randomize(v.i), randomize(v.j)); }
#endif

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
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	auto z = randomU32(in.z);
	return (u64)x | ((u64)y << 32) + (u64)z | ((u64)x << 32) + (u64)y | ((u64)z << 32);
}
v2 random01(v2 p) {
	v2 a = frac((p + SQRT2) * _randMul2);
	a += dot(a, a + PI * 4);
	return frac(V2(a.x * a.y, a.x + a.y));
}
/*
v3 random01(v3 p) {
	v3 a = frac((p + SQRT2) * _randMul3);
	a += dot(a, a + PI * 4);
	return frac(V3(a.x * a.y, a.y * a.z, a.x * a.z));
}
*/
v4 random01(v4 p) {
	v4 a = frac((p + SQRT2) * _randMul4);
	a += dot(a, a + PI * 4);
	return frac(a * V4(_mm_shuffle_ps(a.m.m, a.m.m, _MM_SHUFFLE(1, 2, 3, 0))));
}
v3 random01(v3 p) {
	v4 a = random01(V4(p, 0));
	memcpy(&p, &a, sizeof(p));
	return p;
}
/*
v2 random01(v2 p) {
	v4 a = random01(V4(p, 0, 0));
	memcpy(&p, &a, sizeof(p));
	return p;
}
v4 random01x2(v4 p) {
	p	 = frac((p + SQRT2) * V4(_randMul2, _randMul2));
	v4 a = p + V4(PI * 4);
	v2 d = V2(dot(p.xy, a.xy), dot(p.zw, a.zw));
	p += V4(d.x, d.x, d.y, d.y);
	return V4(frac(V2(p.x * p.y, p.x + p.y)),
			  frac(V2(p.z * p.w, p.z + p.w)));
}
*/
v2 random01(v2s v) {
	auto x = randomU32(v);
	auto y = randomU32(u32(x + 2454940283));
	return V2(V2u(x, y) / 256) / 16777215.f + 0.5f;
}
v3 random01(v3s v) {
	auto x = randomU32(v);
	auto y = randomU32(u32(x + 2454940283));
	auto z = randomU32(x ^ y);
	return V3(V3u(x, y, z) / 256) / 16777215.f;
}
v2 randomN(v2s v) { return V2(randomize(v) / 256) * (1.0f / 8388608.f); }
v3 randomN(v3s v) { return V3(randomize(v) / 256) * (1.0f / 8388608.f); }
v4 randomN(v4s v) { return V4(randomize(v) / 256) * (1.0f / 8388608.f); }
v4x2 randomN(v4sx2 v) { return V4x2(randomize(v) / 256) * (1.0f / 8388608.f); }
v3x4 randomN(v3sx4 v) { return V3x4(randomize(v) / 256) * (1.0f / 8388608.f); }
v2x8 randomN(v2sx8 v) { return V2x8(randomize(v) / 256) * (1.0f / 8388608.f); }
v3x8 randomN(v3sx8 v) { return V3x8(randomize(v) / 256) * (1.0f / 8388608.f); }

#define MSVC_BUG_FIXED 0
#if MSVC_BUG_FIXED

template <size_t i, size_t size, v3s const (&arr)[size], class Cb>
constexpr void wideForEachIter(Cb cb) {
	if constexpr (i + 8 <= size) {
		constexpr auto x	   = arr[0].x;
		constexpr v3s const* c = arr + i;
		constexpr v3sx8 v	   = {
			 c[0].x, c[1].x, c[2].x, c[3].x, c[4].x, c[5].x, c[6].x, c[7].x, c[0].y, c[1].y, c[2].y, c[3].y,
			 c[4].y, c[5].y, c[6].y, c[7].y, c[0].z, c[1].z, c[2].z, c[3].z, c[4].z, c[5].z, c[6].z, c[7].z,
		 };
		cb(v);
		wideForEachIter<i + 8, size, arr>(cb);
	} else if constexpr (i + 4 <= size) {
		constexpr v3s const* c = arr + i;
		constexpr v3sx4 v	   = {
			 c[0].x, c[1].x, c[2].x, c[3].x, c[0].y, c[1].y, c[2].y, c[3].y, c[0].z, c[1].z, c[2].z, c[3].z,
		 };
		cb(v);
		wideForEachIter<i - 4, size, arr>(cb);
	} else if constexpr (i + 1 <= size) {
		cb(arr[i]);
		wideForEachIter<i + 1, size, arr>(cb);
	}
}

template <size_t size, v3s const (&arr)[size], class Cb>
constexpr void wideForEach(Cb cb) {
	wideForEachIter<0, size, arr>(cb);
	/*
	size_t i	   = 0;
	for (; i + 8 < size; i += 8) {
		constexpr v3s const* c = arr + i;
		v3sx8 v			   = {
			  c[0].x, c[1].x, c[2].x, c[3].x, c[4].x, c[5].x, c[6].x, c[7].x, c[0].y, c[1].y, c[2].y, c[3].y,
			  c[4].y, c[5].y, c[6].y, c[7].y, c[0].z, c[1].z, c[2].z, c[3].z, c[4].z, c[5].z, c[6].z, c[7].z,
		  };
		cb(v);
	}
	for (; i + 4 < size; i += 4) {
		v3s const* c = arr + i;
		v3sx4 v	 = {
			c[0].x, c[1].x, c[2].x, c[3].x, c[0].y, c[1].y, c[2].y, c[3].y, c[0].z, c[1].z, c[2].z, c[3].z,
		};
		cb(v);
	}
	for (; i + 1 < size; ++i) {
		cb(arr[i]);
	}
	*/
}
#endif

template <class T>
constexpr size_t noWidth() {
	static_assert(false, "no width");
	return -1;
}

template <class T>
constexpr size_t widthOf = noWidth<T>();
template <>
constexpr size_t widthOf<v3s> = 1;
template <>
constexpr size_t widthOf<v3sx4> = 4;
template <>
constexpr size_t widthOf<v3sx8> = 8;

template <size_t width, class T>
inline constexpr auto widen(T v) = delete;
template <>
inline constexpr auto widen<1>(v3 v) {
	return v;
}
template <>
inline constexpr auto widen<1>(v3s v) {
	return v;
}
template <>
inline constexpr auto widen<4>(v3 v) {
	return CE::V3x4(v);
}
template <>
inline constexpr auto widen<4>(v3x4 v) {
	return v;
}
template <>
inline constexpr auto widen<4>(v3s v) {
	return CE::V3sx4(v);
}
template <>
inline constexpr auto widen<4>(v3sx4 v) {
	return v;
}
template <>
inline constexpr auto widen<8>(v3 v) {
	return CE::V3x8(v);
}
template <>
inline constexpr auto widen<8>(v3x8 v) {
	return v;
}
template <>
inline constexpr auto widen<8>(v3s v) {
	return CE::V3sx8(v);
}
template <>
inline constexpr auto widen<8>(v3sx8 v) {
	return v;
}

namespace CE {
inline constexpr v3sx4 widen(v3s a, v3s b, v3s c, v3s d) { return CE::V3sx4(a, b, c, d); }
inline constexpr v3sx8 widen(v3s a, v3s b, v3s c, v3s d, v3s e, v3s f, v3s g, v3s h) {
	return CE::V3sx8(a, b, c, d, e, f, g, h);
}
inline constexpr v4sx2 widen(v4s a, v4s b) { return CE::V4sx2(a, b); }
} // namespace CE

template <class To, class T>
auto wideCast(T v) = delete;

template <>
inline auto wideCast<float, v3s>(v3s v) {
	return V3(v);
}
template <>
inline auto wideCast<float, v3sx4>(v3sx4 v) {
	return V3x4(v);
}
template <>
inline auto wideCast<float, v3sx8>(v3sx8 v) {
	return V3x8(v);
}
template <>
inline auto wideCast<float>(v4sx2 v) {
	return V4x2(v);
}

template <class T, size_t size>
struct WideType {};

template <>
struct WideType<v3s, 4> {
	using Type = v3sx4;
};
template <>
struct WideType<v3s, 8> {
	using Type = v3sx8;
};
template <>
struct WideType<v4s, 4> {
	using Type = v4s;
};
template <>
struct WideType<v4s, 8> {
	using Type = v4sx2;
};

namespace CE {
namespace Widen {
template <class T, size_t count1, size_t count4, size_t count8>
struct WideHolder {
	using T1 = T;
	using T4 = typename WideType<T, 4>::Type;
	using T8 = typename WideType<T, 8>::Type;
};
template <class T, size_t count1, size_t count4>
struct WideHolder<T, count1, count4, 0> {};
template <class T, size_t count1, size_t count8>
struct WideHolder<T, count1, 0, count8> {
	using T1 = T;
	using T8 = typename WideType<T, 8>::Type;
	T8 t8[count8 * sizeof(T) * 8 / sizeof(T8)]{};
	T1 t1[count1]{};
	constexpr WideHolder() = delete;
	inline constexpr WideHolder(T const (&arr)[count8 * 8 + count1]) {
		for (size_t i = 0; i < _countof(t8); ++i) {
			if constexpr (sizeof(T8) == sizeof(T) * 8) {
				t8[i] = CE::widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
								  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
			} else if constexpr (sizeof(T8) == sizeof(T) * 2) {
				t8[i] = CE::widen(arr[i * 2 + 0], arr[i * 2 + 1]);
			} else if constexpr (sizeof(T8) == sizeof(T)) {
				t8[i] = arr[i];
			} else {
				static_assert(false);
			}
		}
		for (size_t i = 0; i < count1; ++i) {
			t1[i] = arr[count8 * 8 + i];
		}
	}
	template <class Cb>
	inline constexpr void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
		for (auto const& t : t1)
			cb(t);
	}
};
// template<class T>
// struct WideHolder<T,0,0,0> {
//	static_assert(false, "bad wide holder");
//};
} // namespace Widen
template <class T, size_t size>
inline constexpr auto prepareForSIMD(T const (&arr)[size]) {
	using namespace Widen;
	constexpr size_t count8 = size / 8;
	constexpr size_t count4 = (size - count8 * 8) / 4;
	constexpr size_t count1 = size - count8 * 8 - count4 * 4;
	return WideHolder<T, count1, count4, count8>(arr);
}
} // namespace CE
namespace Widen {
template <class T, size_t count1, size_t count4, size_t count8>
struct WideHolder {
	using T1 = T;
	using T4 = typename WideType<T, 4>::Type;
	using T8 = typename WideType<T, 8>::Type;
};
template <class T, size_t count1, size_t count4>
struct WideHolder<T, count1, count4, 0> {};
template <class T, size_t count1, size_t count8>
struct WideHolder<T, count1, 0, count8> {
	using T1 = T;
	using T8 = typename WideType<T, 8>::Type;
	T8 t8[count8 * sizeof(T) * 8 / sizeof(T8)]{};
	T1 t1[count1]{};
	WideHolder() = delete;
	inline WideHolder(T const (&arr)[count8 * 8 + count1]) {
		if constexpr (std::is_same_v<T8, v4sx2>) {
			memcpy(t8, arr, sizeof(t8));
		} else {
			for (size_t i = 0; i < _countof(t8); ++i) {
				if constexpr (sizeof(T8) == sizeof(T) * 8) {
					t8[i] = widen(arr[i * 8 + 0], arr[i * 8 + 1], arr[i * 8 + 2], arr[i * 8 + 3], arr[i * 8 + 4],
								  arr[i * 8 + 5], arr[i * 8 + 6], arr[i * 8 + 7]);
				} else if constexpr (sizeof(T8) == sizeof(T) * 2) {
					t8[i] = widen(arr[i * 2 + 0], arr[i * 2 + 1]);
				} else {
					static_assert(false);
				}
			}
		}
		for (size_t i = 0; i < count1; ++i) {
			t1[i] = arr[count8 * 8 + i];
		}
	}
	template <class Cb>
	inline void forEach(Cb cb) const {
		for (auto const& t : t8)
			cb(t);
		for (auto const& t : t1)
			cb(t);
	}
};
// template<class T>
// struct WideHolder<T,0,0,0> {
//	static_assert(false, "bad wide holder");
//};
} // namespace Widen
template <class T, size_t size>
inline constexpr auto prepareForSIMD(T const (&arr)[size]) {
	using namespace Widen;
	constexpr size_t count8 = size / 8;
	constexpr size_t count4 = (size - count8 * 8) / 4;
	constexpr size_t count1 = size - count8 * 8 - count4 * 4;
	return WideHolder<T, count1, count4, count8>(arr);
}
inline f32 voronoi(v2 v) {
	v2 rel		= frac(v) - 0.5f;
	v2 tile		= floor(v);
	f32 minDist = 1000;
	for (f32 x = -1; x <= 1; ++x) {
		for (f32 y = -1; y <= 1; ++y) {
			v2 off	= V2(x, y);
			minDist = min(minDist, lengthSqr(rel - random01(tile + off) + off - 0.5f));
		}
	}
	return sqrt(minDist) * (1 / SQRT2);
}
inline f32 voronoi(v3 v) {
	v3 rel		= frac(v) - 0.5f;
	v3 tile		= floor(v);
	f32 minDist = 1000;
	for (f32 x = -1; x <= 1; ++x) {
		for (f32 y = -1; y <= 1; ++y) {
			for (f32 z = -1; z <= 1; ++z) {
				v3 off	= V3(x, y, z);
				minDist = min(minDist, lengthSqr(rel - random01(tile + off) + off - 0.5f));
			}
		}
	}
	return sqrt(minDist) * (1 / SQRT3);
}
inline f32 voronoi(v4 v) {
	v4 rel		= frac(v) - 0.5f;
	v4 tile		= floor(v);
	f32 minDist = 1000;
	for (f32 x = -1; x <= 1; ++x) {
		for (f32 y = -1; y <= 1; ++y) {
			for (f32 z = -1; z <= 1; ++z) {
				for (f32 w = -1; w <= 1; ++w) {
					v4 off	= V4(x, y, z, w);
					minDist = min(minDist, lengthSqr(rel - random01(tile + off) + off - 0.5f));
				}
			}
		}
	}
	return sqrt(minDist) * (1 / SQRT3);
}
/*
inline v2 voronoi(v2 a, v2 b) {
	v4 ab	   = V4(a, b);
	v4 rel	   = frac(ab) - 0.5f;
	v4 tile	   = floor(ab);
	v2 minDist = V2(1000.0f);
	for (f32 x = -1; x <= 1; ++x) {
		for (f32 y = -1; y <= 1; ++y) {
			v4 off	= V4(x, y, x, y);
			minDist = min(minDist, lengthSqrx2(rel - random01x2(tile + off) + off - 0.5f));
		}
	}
	return sqrt(minDist) * (1 / SQRT2);
}
*/
inline f32 voronoi(v2s v, u32 cellSize) {
	v2s flr		= floor(v, cellSize);
	v2s tile	= flr / (s32)cellSize;
	v2 rel		= V2(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist = 1000;
	auto tile8	= V2sx8(tile);
	auto rel8	= V2x8(rel);

	static constexpr v2sx8 offset8 = CE::V2sx8(-1, -1, -1, 0, -1, 1, 0, -1, 0, 0, 0, 1, 1, -1, 1, 0);
	auto target					   = randomN(tile8 + offset8) * 0.5f + V2x8(offset8) - rel8;
	f32x8 dp					   = dot(target, target);
	minDist						   = min(minDist, dp);
	minDist						   = min(minDist, lengthSqr(rel - randomN(tile + 1) * 0.5f + 1));
	return sqrt(minDist) * (1 / SQRT2);
}
inline f32 voronoi(v3s v, u32 cellSize) {
	v3s flr		= floor(v, cellSize);
	v3s tile	= flr / (s32)cellSize;
	v3 rel		= V3(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist = 1000;
	v3sx8 tile8 = V3sx8(tile);
	v3x8 rel8	= V3x8(rel);
#if 0
	// clang-format off
	static constexpr v3s offsets[]{ 
		{-1,-1,-1}, {-1,-1, 0}, {-1,-1, 1},
		{-1, 0,-1}, {-1, 0, 0}, {-1, 0, 1},
		{-1, 1,-1}, {-1, 1, 0}, {-1, 1, 1},
		{ 0,-1,-1}, { 0,-1, 0}, { 0,-1, 1},
		{ 0, 0,-1}, { 0, 0, 0}, { 0, 0, 1},
		{ 0, 1,-1}, { 0, 1, 0}, { 0, 1, 1},
		{ 1,-1,-1}, { 1,-1, 0}, { 1,-1, 1},
		{ 1, 0,-1}, { 1, 0, 0}, { 1, 0, 1},
		{ 1, 1,-1}, { 1, 1, 0}, { 1, 1, 1},
	};
	// clang-format on
	static constexpr auto wideOffsets = CE::prepareForSIMD(offsets);
#if 1
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, lengthSqr(randomN(tile8 + offset) * 0.5f + V3x8(offset) - rel8));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, lengthSqr(randomN(tile + offset) * 0.5f + V3(offset) - rel));
	}
#else
	wideOffsets.forEach([&](auto const& offset) {
		static constexpr auto offsetW = widthOf<std::decay_t<decltype(offset)>>;
		// static_assert(offsetW == 8);
		// static_assert(std::is_same_v<decltype(makeWideAs<offsetW>(rel)), v3x8>);
		minDist = min(minDist, lengthSqr(randomN(widen<offsetW>(tile) + offset) * 0.5f + wideCast<float>(offset) -
									   widen<offsetW>(rel)));
	});
#endif
#else
	// clang-format off
	static constexpr v3sx8 offsets8[] {
		CE::V3sx8(-1,-1,-1,-1,-1, 0,-1,-1, 1,-1, 0,-1,-1, 0, 0,-1, 0, 1,-1, 1,-1,-1, 1, 0),
		CE::V3sx8(-1, 1, 1, 0,-1,-1, 0,-1, 0, 0,-1, 1, 0, 0,-1, 0, 0, 0, 0, 0, 1, 0, 1,-1),
		CE::V3sx8( 0, 1, 0, 0, 1, 1, 1,-1,-1, 1,-1, 0, 1,-1, 1, 1, 0,-1, 1, 0, 0, 1, 0, 1),
	};
	static constexpr v3s offsets[]{ 
		{1, 1,-1}, 
		{1, 1, 0}, 
		{1, 1, 1}
	};
	// clang-format on
	for (auto offset : offsets8) {
		minDist = min(minDist, lengthSqr(randomN(tile8 + offset) * 0.5f + V3x8(offset) - rel8));
	}
	for (auto offset : offsets) {
		minDist = min(minDist, lengthSqr(randomN(tile + offset) * 0.5f + V3(offset) - rel));
	}
#endif
	return sqrt(minDist) * (1 / SQRT3);
}
inline f32 voronoi(v4s v, u32 cellSize) {
	v4s flr		= floor(v, cellSize);
	v4s tile	= flr / (s32)cellSize;
	v4 rel		= V4(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist = 1000;
	v4sx2 tile8 = V4sx2(tile);
	v4x2 rel8	= V4x2(rel);
#if 1
	// clang-format off
	v4s offsets[] {
		{-1,-1,-1,-1},{-1,-1,-1, 0},{-1,-1,-1, 1},
		{-1,-1, 0,-1},{-1,-1, 0, 0},{-1,-1, 0, 1},
		{-1,-1, 1,-1},{-1,-1, 1, 0},{-1,-1, 1, 1},
		{-1, 0,-1,-1},{-1, 0,-1, 0},{-1, 0,-1, 1},
		{-1, 0, 0,-1},{-1, 0, 0, 0},{-1, 0, 0, 1},
		{-1, 0, 1,-1},{-1, 0, 1, 0},{-1, 0, 1, 1},
		{-1, 1,-1,-1},{-1, 1,-1, 0},{-1, 1,-1, 1},
		{-1, 1, 0,-1},{-1, 1, 0, 0},{-1, 1, 0, 1},
		{-1, 1, 1,-1},{-1, 1, 1, 0},{-1, 1, 1, 1},
		{ 0,-1,-1,-1},{ 0,-1,-1, 0},{ 0,-1,-1, 1},
		{ 0,-1, 0,-1},{ 0,-1, 0, 0},{ 0,-1, 0, 1},
		{ 0,-1, 1,-1},{ 0,-1, 1, 0},{ 0,-1, 1, 1},
		{ 0, 0,-1,-1},{ 0, 0,-1, 0},{ 0, 0,-1, 1},
		{ 0, 0, 0,-1},{ 0, 0, 0, 0},{ 0, 0, 0, 1},
		{ 0, 0, 1,-1},{ 0, 0, 1, 0},{ 0, 0, 1, 1},
		{ 0, 1,-1,-1},{ 0, 1,-1, 0},{ 0, 1,-1, 1},
		{ 0, 1, 0,-1},{ 0, 1, 0, 0},{ 0, 1, 0, 1},
		{ 0, 1, 1,-1},{ 0, 1, 1, 0},{ 0, 1, 1, 1},
		{ 1,-1,-1,-1},{ 1,-1,-1, 0},{ 1,-1,-1, 1},
		{ 1,-1, 0,-1},{ 1,-1, 0, 0},{ 1,-1, 0, 1},
		{ 1,-1, 1,-1},{ 1,-1, 1, 0},{ 1,-1, 1, 1},
		{ 1, 0,-1,-1},{ 1, 0,-1, 0},{ 1, 0,-1, 1},
		{ 1, 0, 0,-1},{ 1, 0, 0, 0},{ 1, 0, 0, 1},
		{ 1, 0, 1,-1},{ 1, 0, 1, 0},{ 1, 0, 1, 1},
		{ 1, 1,-1,-1},{ 1, 1,-1, 0},{ 1, 1,-1, 1},
		{ 1, 1, 0,-1},{ 1, 1, 0, 0},{ 1, 1, 0, 1},
		{ 1, 1, 1,-1},{ 1, 1, 1, 0},{ 1, 1, 1, 1}
	};
	// clang-format on
	auto wideOffsets = prepareForSIMD(offsets);
	for (auto offset : wideOffsets.t8) {
		minDist = min(minDist, lengthSqr(randomN(tile8 + offset) * 0.5f + wideCast<float>(offset) - rel8));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, lengthSqr(randomN(tile + offset) * 0.5f + V4(offset) - rel));
	}
#else
	// clang-format off
	static constexpr v4sx2 offsets256[] {
		{-1,-1,-1,-1,-1,-1,-1, 0},
		{-1,-1,-1, 1,-1,-1, 0,-1},
		{-1,-1, 0, 0,-1,-1, 0, 1},
		{-1,-1, 1,-1,-1,-1, 1, 0},
		{-1,-1, 1, 1,-1, 0,-1,-1},
		{-1, 0,-1, 0,-1, 0,-1, 1},
		{-1, 0, 0,-1,-1, 0, 0, 0},
		{-1, 0, 0, 1,-1, 0, 1,-1},
		{-1, 0, 1, 0,-1, 0, 1, 1},
		{-1, 1,-1,-1,-1, 1,-1, 0},
		{-1, 1,-1, 1,-1, 1, 0,-1},
		{-1, 1, 0, 0,-1, 1, 0, 1},
		{-1, 1, 1,-1,-1, 1, 1, 0},
		{-1, 1, 1, 1, 0,-1,-1,-1},
		{ 0,-1,-1, 0, 0,-1,-1, 1},
		{ 0,-1, 0,-1, 0,-1, 0, 0},
		{ 0,-1, 0, 1, 0,-1, 1,-1},
		{ 0,-1, 1, 0, 0,-1, 1, 1},
		{ 0, 0,-1,-1, 0, 0,-1, 0},
		{ 0, 0,-1, 1, 0, 0, 0,-1},
		{ 0, 0, 0, 0, 0, 0, 0, 1},
		{ 0, 0, 1,-1, 0, 0, 1, 0},
		{ 0, 0, 1, 1, 0, 1,-1,-1},
		{ 0, 1,-1, 0, 0, 1,-1, 1},
		{ 0, 1, 0,-1, 0, 1, 0, 0},
		{ 0, 1, 0, 1, 0, 1, 1,-1},
		{ 0, 1, 1, 0, 0, 1, 1, 1},
		{ 1,-1,-1,-1, 1,-1,-1, 0},
		{ 1,-1,-1, 1, 1,-1, 0,-1},
		{ 1,-1, 0, 0, 1,-1, 0, 1},
		{ 1,-1, 1,-1, 1,-1, 1, 0},
		{ 1,-1, 1, 1, 1, 0,-1,-1},
		{ 1, 0,-1, 0, 1, 0,-1, 1},
		{ 1, 0, 0,-1, 1, 0, 0, 0},
		{ 1, 0, 0, 1, 1, 0, 1,-1},
		{ 1, 0, 1, 0, 1, 0, 1, 1},
		{ 1, 1,-1,-1, 1, 1,-1, 0},
		{ 1, 1,-1, 1, 1, 1, 0,-1},
		{ 1, 1, 0, 0, 1, 1, 0, 1},
		{ 1, 1, 1,-1, 1, 1, 1, 0},
	};
	// clang-format on
	for (auto offset : offsets256) {
		v4x2 target = randomN(tile8 + offset) * 0.5f + V4x2(offset) - rel8;
		v2 dp = dot(target, target);
		minDist = min(minDist, dp.x);
		minDist = min(minDist, dp.y);
	}
	minDist = min(minDist, lengthSqr(rel - randomN(tile + 1) * 0.5f + 1));
#endif
	return sqrt(minDist) * (1 / SQRT3);
}
/*
struct Hit {
	v3 p, n;
};
inline bool raycastPlane(v3 a, v3 b, v3 s1, v3 s2, v3 s3, Hit& hit) {
	// 1.
	auto dS21 = s2 - s1;
	auto dS31 = s3 - s1;
	hit.n	  = normalize(cross(dS21, dS31));

	// 2.
	auto dR = a - b;

	f32 ndotdR = dot(hit.n, dR);

	if (ndotdR <= 1e-5f) { // Choose your tolerance
		return false;
	}

	f32 t = dot(-hit.n, a - s1) / ndotdR;
	hit.p = a + dR * t;

	// 3.
	auto dMS1 = hit.p - s1;
	float u	  = dot(dMS1, dS21);
	float v	  = dot(dMS1, dS31);

	if (lengthSqr(a - hit.p) > lengthSqr(a - b) || dot(a - hit.p, a - b) <= 0) {
		return false;
	}

	// 4.
	return (u >= 0.0f && u <= dot(dS21, dS21) && v >= 0.0f && v <= dot(dS31, dS31));
}
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
