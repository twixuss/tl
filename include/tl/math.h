#pragma once

#include "common.h"
#if COMPILER_MSVC
#pragma warning(push, 0)
#endif
#include <float.h>
#include <intrin.h>
#include <math.h>
#include <memory.h>

#if COMPILER_GCC
#include <immintrin.h>
#endif
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
template <class T, class U>
constexpr auto max(T a, U b) {
	return a > b ? a : b;
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

union v2;
union v3;
union v4;
union v2i;
union v3i;
union v4i;
union v2u;
union v3u;
union v4u;
union v4_x2;
union v4i_x2;
union v4u_x2;
union v3_x4;
union v3i_x4;
union v3u_x4;
union v2_x8;
union v2i_x8;
union v2u_x8;
union v3_x8;
union v3i_x8;
union v3u_x8;
union m4;

constexpr v2 V2(f32 v = 0.f);
constexpr v2i V2i(i32 v = 0);
constexpr v2u V2u(u32 v = 0);

constexpr v3 V3(f32 v = 0.f);
constexpr v3i V3i(i32 v = 0);
constexpr v3u V3u(u32 v = 0);

inline v4 V4(f32 v = 0.f);
inline v4i V4i(i32 v = 0);
inline v4u V4u(u32 v = 0);

constexpr v2 V2(f32 x, f32 y);
constexpr v2i V2i(i32 x, i32 y);
constexpr v2u V2u(u32 x, u32 y);

constexpr v3 V3(f32 x, f32 y, f32 z);
constexpr v3i V3i(i32 x, i32 y, i32 z);
constexpr v3u V3u(u32 x, u32 y, u32 z);

inline v4 V4(f32 x, f32 y, f32 z, f32 w);
inline v4i V4i(i32 x, i32 y, i32 z, i32 w);
inline v4u V4u(u32 x, u32 y, u32 z, u32 w);

constexpr v4 V4(__m128);
constexpr v4i V4i(__m128i);
constexpr v4u V4u(__m128i);

constexpr v4_x2 V4_x2(__m256 m);
constexpr v4_x2 V4_x2(f32 v);
constexpr v4_x2 V4_x2(v4 a, v4 b);

constexpr v4i_x2 V4i_x2(__m256i m);
constexpr v4i_x2 V4i_x2(i32 v);
constexpr v4i_x2 V4i_x2(v4i a, v4i b);

constexpr v4u_x2 V4u_x2(__m256i m);
constexpr v4u_x2 V4u_x2(u32 v);
constexpr v4u_x2 V4u_x2(v4u a, v4u b);

constexpr v3_x4 V3_x4(__m128 x, __m128 y, __m128 z);
constexpr v3_x4 V3_x4(v4 x, v4 y, v4 z);
inline v3_x4 V3_x4(f32 v);

constexpr v3i_x4 V3i_x4(__m128i x, __m128i y, __m128i z);
constexpr v3i_x4 V3i_x4(v4i x, v4i y, v4i z);
inline v3i_x4 V3i_x4(i32 v);

constexpr v3u_x4 V3u_x4(__m128i x, __m128i y, __m128i z);
constexpr v3u_x4 V3u_x4(v4u x, v4u y, v4u z);
inline v3u_x4 V3u_x4(u32 v);

inline v3_x8 V3_x8(f32 v);
inline v3i_x8 V3i_x8(i32 v);
inline v3u_x8 V3u_x8(u32 v);

inline v2_x8 V2_x8(f32 v);
inline v2i_x8 V2i_x8(i32 v);
inline v2u_x8 V2u_x8(u32 v);

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
	const scl* data() const { return v; }

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

#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_DATA(scl)
#define BINOP(op) \
	constexpr v2 operator op(v2 b) const { return {x op b.x, y op b.y}; }
union v2 {
	MEMBERS2(f32);
	constexpr v2 operator-() const { return {-x, -y}; }
	BINOP(+) BINOP(-) BINOP(*) BINOP(/);
	constexpr bool operator==(v2 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2, f32, V2);
};
#undef BINOP
#define BINOP(op) \
	constexpr v3 operator op(v3 b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3 {
	MEMBERS3(f32, v2);
	v3 operator-() const { return {-x, -y, -z}; }
	BINOP(+) BINOP(-) BINOP(*) BINOP(/);
	bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3, f32, V3);
};
union v4 {
	MEMBERS4(f32, v2, v3);
	__m128 m;
	v4 operator-() const { return V4(_mm_sub_ps(_mm_set1_ps(0), m)); }
	v4 operator+(v4 b) const { return V4(_mm_add_ps(m, b.m)); }
	v4 operator-(v4 b) const { return V4(_mm_sub_ps(m, b.m)); }
	v4 operator*(v4 b) const { return V4(_mm_mul_ps(m, b.m)); }
	v4 operator/(v4 b) const { return V4(_mm_div_ps(m, b.m)); }
	bool operator==(v4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
	MEMFUNS(v4, f32, V4);
};
#undef MEMFUNS
#define MEMFUNS(vec, scl, con) \
	MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL(vec, scl, con)
#undef BINOP
#define BINOP(op) \
	constexpr v2i operator op(v2i b) const { return {x op b.x, y op b.y}; }
union v2i {
	MEMBERS2(i32);
	v2i operator-() const { return {-x, -y}; }
	BINOP(+) BINOP(-) BINOP(*) BINOP(/) BINOP(%) BINOP(^) BINOP(|) BINOP(&) BINOP(<<) BINOP(>>);
	bool operator==(v2i b) const { return x == b.x && y == b.y; }
	explicit operator v2() const { return {(f32)x, (f32)y}; }
	MEMFUNS(v2i, i32, V2i);
};
#undef BINOP
#define BINOP(op) \
	constexpr v3i operator op(v3i b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3i {
	MEMBERS3(i32, v2i);
	v3i operator-() const { return {-x, -y, -z}; }
	BINOP(+) BINOP(-) BINOP(*) BINOP(/) BINOP(%) BINOP(^) BINOP(|) BINOP(&) BINOP(<<) BINOP(>>);
	bool operator==(v3i b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3i, i32, V3i);
};
#undef BINOP
#define BINOP(op) \
	constexpr v2u operator op(v2u b) const { return {x op b.x, y op b.y}; }
union v2u {
	MEMBERS2(u32);
	BINOP(+) BINOP(-) BINOP(*) BINOP(/) BINOP(%) BINOP(^) BINOP(|) BINOP(&) BINOP(<<) BINOP(>>);
	bool operator==(v2u b) const { return x == b.x && y == b.y; }
	explicit operator v2() const { return {(f32)x, (f32)y}; }
	explicit operator v2i() const { return {(i32)x, (i32)y}; }
	MEMFUNS(v2u, u32, V2u);
};
#undef BINOP
#define BINOP(op) \
	constexpr v3u operator op(v3u b) const { return {x op b.x, y op b.y, z op b.z}; }
union v3u {
	MEMBERS3(u32, v2u);
	BINOP(+) BINOP(-) BINOP(*) BINOP(/) BINOP(%) BINOP(^) BINOP(|) BINOP(&) BINOP(<<) BINOP(>>);
	bool operator==(v3u b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3u, u32, V3u);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_DATA(scl) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)

union v4i {
	MEMBERS4(i32, v2i, v3i);
	__m128i m;
	v4i operator-() const { return V4i(_mm_sub_epi32(_mm_set1_epi32(0), m)); }
	v4i operator+(v4i b) const { return V4i(_mm_add_epi32(m, b.m)); }
	v4i operator-(v4i b) const { return V4i(_mm_sub_epi32(m, b.m)); }
	v4i operator*(v4i b) const { return V4i(_mm_mullo_epi32(m, b.m)); }
	v4i operator^(v4i b) const { return V4i(_mm_xor_si128(m, b.m)); }
	v4i operator|(v4i b) const { return V4i(_mm_or_si128(m, b.m)); }
	v4i operator&(v4i b) const { return V4i(_mm_and_si128(m, b.m)); }
	v4i operator<<(v4i b) const { return V4i(x << b.x, y << b.y, z << b.z, w << b.w); }
	v4i operator>>(v4i b) const { return V4i(x >> b.x, y >> b.y, z >> b.z, w >> b.w); }
#if COMPILER_GCC
	v4i operator/(v4i b) const { return {x / b.x, y / b.y, z / b.z, w / b.w}; }
	v4i operator%(v4i b) const { return {x % b.x, y % b.y, z % b.z, w % b.w}; }
#else
	v4i operator/(v4i b) const { return V4i(_mm_div_epi32(m, b.m)); }
	v4i operator%(v4i b) const { return V4i(_mm_rem_epi32(m, b.m)); }
#endif
	v4i operator<<(i32 b) const { return V4i(_mm_slli_epi32(m, b)); }
	v4i operator>>(i32 b) const { return V4i(_mm_srai_epi32(m, b)); }
	bool operator==(v4i b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
	MEMFUNS(v4i, i32, V4i);
};
union v4u {
	MEMBERS4(u32, v2u, v3u);
	__m128i m;
	v4u operator+(v4u b) const { return V4u(_mm_add_epi32(m, b.m)); }
	v4u operator-(v4u b) const { return V4u(_mm_sub_epi32(m, b.m)); }
	v4u operator*(v4u b) const { return {x * b.x, y * b.y, z * b.z, w * b.w}; }
#if COMPILER_GCC
	v4u operator/(v4u b) const { return {x / b.x, y / b.y, z / b.z, w / b.w}; }
	v4u operator%(v4u b) const { return {x % b.x, y % b.y, z % b.z, w % b.w}; }
#else
	v4u operator/(v4u b) const { return V4u(_mm_div_epu32(m, b.m)); }
	v4u operator%(v4u b) const { return V4u(_mm_rem_epu32(m, b.m)); }
#endif
	v4u operator^(v4u b) const { return V4u(_mm_xor_si128(m, b.m)); }
	v4u operator|(v4u b) const { return V4u(_mm_or_si128(m, b.m)); }
	v4u operator&(v4u b) const { return V4u(_mm_and_si128(m, b.m)); }
	v4u operator<<(v4u b) const { return V4u(x << b.x, y << b.y, z << b.z, w << b.w); }
	v4u operator>>(v4u b) const { return V4u(x >> b.x, y >> b.y, z >> b.z, w >> b.w); }
	v4u operator<<(u32 b) const { return V4u(_mm_slli_epi32(m, i32(b))); }
	v4u operator>>(u32 b) const { return V4u(_mm_srli_epi32(m, i32(b))); }
	bool operator==(v4u b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
	MEMFUNS(v4u, u32, V4u);
}; // namespace TL

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)
union v4_x2 {
	struct {
		v4 i, j;
	};
	__m256 m;
	v4_x2 operator-() const { return V4_x2(_mm256_sub_ps(_mm256_set1_ps(0), m)); }
	v4_x2 operator+(v4_x2 b) const { return V4_x2(_mm256_add_ps(m, b.m)); }
	v4_x2 operator-(v4_x2 b) const { return V4_x2(_mm256_sub_ps(m, b.m)); }
	v4_x2 operator*(v4_x2 b) const { return V4_x2(_mm256_mul_ps(m, b.m)); }
	v4_x2 operator/(v4_x2 b) const { return V4_x2(_mm256_div_ps(m, b.m)); }
	bool operator==(v4_x2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4_x2, f32, V4_x2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v4i_x2 {
	struct {
		v4i i, j;
	};
	__m256i m;
	v4i_x2 operator-() const { return V4i_x2(_mm256_sub_epi32(_mm256_set1_epi32(0), m)); }
	v4i_x2 operator+(v4i_x2 b) const { return V4i_x2(_mm256_add_epi32(m, b.m)); }
	v4i_x2 operator-(v4i_x2 b) const { return V4i_x2(_mm256_sub_epi32(m, b.m)); }
	v4i_x2 operator*(v4i_x2 b) const { return V4i_x2(_mm256_mullo_epi32(m, b.m)); }
	v4i_x2 operator^(v4i_x2 b) const { return V4i_x2(_mm256_xor_si256(m, b.m)); }
	v4i_x2 operator|(v4i_x2 b) const { return V4i_x2(_mm256_or_si256(m, b.m)); }
	v4i_x2 operator&(v4i_x2 b) const { return V4i_x2(_mm256_and_si256(m, b.m)); }
	v4i_x2 operator<<(v4i_x2 b) const { return V4i_x2(i << b.i, j << b.j); }
	v4i_x2 operator>>(v4i_x2 b) const { return V4i_x2(i >> b.i, j >> b.j); }
#if COMPILER_GCC
	v4i_x2 operator/(v4i_x2 b) const { return V4i_x2(i / b.i, j / b.j); }
	v4i_x2 operator%(v4i_x2 b) const { return V4i_x2(i % b.i, j % b.j); }
#else
	v4i_x2 operator/(v4i_x2 b) const { return V4i_x2(_mm256_div_epi32(m, b.m)); }
	v4i_x2 operator%(v4i_x2 b) const { return V4i_x2(_mm256_rem_epi32(m, b.m)); }
#endif
	v4i_x2 operator<<(i32 b) const { return V4i_x2(_mm256_slli_epi32(m, i32(b))); }
	v4i_x2 operator>>(i32 b) const { return V4i_x2(_mm256_srai_epi32(m, i32(b))); }
	bool operator==(v4i_x2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4i_x2, i32, V4i_x2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v4u_x2 {
	struct {
		v4u i, j;
	};
	__m256i m;
	v4u_x2 operator+(v4u_x2 b) const { return V4u_x2(_mm256_add_epi32(m, b.m)); }
	v4u_x2 operator-(v4u_x2 b) const { return V4u_x2(_mm256_sub_epi32(m, b.m)); }
	v4u_x2 operator*(v4u_x2 b) const { return V4u_x2(i * b.i, j * b.j); }
	v4u_x2 operator^(v4u_x2 b) const { return V4u_x2(_mm256_xor_si256(m, b.m)); }
	v4u_x2 operator|(v4u_x2 b) const { return V4u_x2(_mm256_or_si256(m, b.m)); }
	v4u_x2 operator&(v4u_x2 b) const { return V4u_x2(_mm256_and_si256(m, b.m)); }
	v4u_x2 operator<<(v4u_x2 b) const { return V4u_x2(i << b.i, j << b.j); }
	v4u_x2 operator>>(v4u_x2 b) const { return V4u_x2(i >> b.i, j >> b.j); }
#if COMPILER_GCC
	v4u_x2 operator/(v4u_x2 b) const { return V4u_x2(i / b.i, j / b.j); }
	v4u_x2 operator%(v4u_x2 b) const { return V4u_x2(i % b.i, j % b.j); }
#else
	v4u_x2 operator/(v4u_x2 b) const { return V4u_x2(_mm256_div_epi32(m, b.m)); }
	v4u_x2 operator%(v4u_x2 b) const { return V4u_x2(_mm256_rem_epi32(m, b.m)); }
#endif
	v4u_x2 operator<<(u32 b) const { return V4u_x2(_mm256_slli_epi32(m, i32(b))); }
	v4u_x2 operator>>(u32 b) const { return V4u_x2(_mm256_srli_epi32(m, i32(b))); }
	bool operator==(v4u_x2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4u_x2, u32, V4u_x2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)

union v3_x4 {
	struct {
		v4 x, y, z;
	};
	v3_x4 operator-() const { return {-x, -y, -z}; }
	v3_x4 operator+(v3_x4 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3_x4 operator-(v3_x4 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3_x4 operator*(v3_x4 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3_x4 operator/(v3_x4 b) const { return {x / b.x, y / b.y, z / b.z}; }
	bool operator==(v3_x4 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3_x4, f32, V3_x4);
};

union v3_x8 {
	struct {
		v4_x2 x, y, z;
	};
	v3_x8 operator-() const { return {-x, -y, -z}; }
	v3_x8 operator+(v3_x8 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3_x8 operator-(v3_x8 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3_x8 operator*(v3_x8 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3_x8 operator/(v3_x8 b) const { return {x / b.x, y / b.y, z / b.z}; }
	bool operator==(v3_x8 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3_x8, f32, V3_x8);
};

union v2_x8 {
	struct {
		v4_x2 x, y;
	};
	v2_x8 operator+(v2_x8 b) const { return {x + b.x, y + b.y}; }
	v2_x8 operator-(v2_x8 b) const { return {x - b.x, y - b.y}; }
	v2_x8 operator*(v2_x8 b) const { return {x * b.x, y * b.y}; }
	v2_x8 operator/(v2_x8 b) const { return {x / b.x, y / b.y}; }
	bool operator==(v2_x8 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2_x8, f32, V2_x8);
};
#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con) MEMFUNS_SHIFT_SCL(vec, scl, con)

union v3i_x4 {
	struct {
		v4i x, y, z;
	};
	v3i_x4 operator-() const { return {-x, -y, -z}; }
	v3i_x4 operator+(v3i_x4 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3i_x4 operator-(v3i_x4 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3i_x4 operator*(v3i_x4 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3i_x4 operator/(v3i_x4 b) const { return {x / b.x, y / b.y, z / b.z}; }
	v3i_x4 operator%(v3i_x4 b) const { return {x % b.x, y % b.y, z % b.z}; }
	v3i_x4 operator^(v3i_x4 b) const { return {x ^ b.x, y ^ b.y, z ^ b.z}; }
	v3i_x4 operator|(v3i_x4 b) const { return {x | b.x, y | b.y, z | b.z}; }
	v3i_x4 operator&(v3i_x4 b) const { return {x & b.x, y & b.y, z & b.z}; }
	v3i_x4 operator<<(v3i_x4 b) const { return {x << b.x, y << b.y, z << b.z}; }
	v3i_x4 operator>>(v3i_x4 b) const { return {x >> b.x, y >> b.y, z >> b.z}; }
	bool operator==(v3i_x4 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3i_x4, i32, V3i_x4);
};
union v3i_x8 {
	struct {
		v4i_x2 x, y, z;
	};
	v3i_x8 operator-() const { return {-x, -y, -z}; }
	v3i_x8 operator+(v3i_x8 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3i_x8 operator-(v3i_x8 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3i_x8 operator*(v3i_x8 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3i_x8 operator/(v3i_x8 b) const { return {x / b.x, y / b.y, z / b.z}; }
	v3i_x8 operator%(v3i_x8 b) const { return {x % b.x, y % b.y, z % b.z}; }
	v3i_x8 operator^(v3i_x8 b) const { return {x ^ b.x, y ^ b.y, z ^ b.z}; }
	v3i_x8 operator|(v3i_x8 b) const { return {x | b.x, y | b.y, z | b.z}; }
	v3i_x8 operator&(v3i_x8 b) const { return {x & b.x, y & b.y, z & b.z}; }
	v3i_x8 operator<<(v3i_x8 b) const { return {x << b.x, y << b.y, z << b.z}; }
	v3i_x8 operator>>(v3i_x8 b) const { return {x >> b.x, y >> b.y, z >> b.z}; }
	bool operator==(v3i_x8 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3i_x8, i32, V3i_x8);
};
union v3u_x4 {
	struct {
		v4u x, y, z;
	};
	v3u_x4 operator+(v3u_x4 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3u_x4 operator-(v3u_x4 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3u_x4 operator*(v3u_x4 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3u_x4 operator/(v3u_x4 b) const { return {x / b.x, y / b.y, z / b.z}; }
	v3u_x4 operator%(v3u_x4 b) const { return {x % b.x, y % b.y, z % b.z}; }
	v3u_x4 operator^(v3u_x4 b) const { return {x ^ b.x, y ^ b.y, z ^ b.z}; }
	v3u_x4 operator|(v3u_x4 b) const { return {x | b.x, y | b.y, z | b.z}; }
	v3u_x4 operator&(v3u_x4 b) const { return {x & b.x, y & b.y, z & b.z}; }
	v3u_x4 operator<<(v3u_x4 b) const { return {x << b.x, y << b.y, z << b.z}; }
	v3u_x4 operator>>(v3u_x4 b) const { return {x >> b.x, y >> b.y, z >> b.z}; }
	bool operator==(v3u_x4 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3u_x4, u32, V3u_x4);
};
union v3u_x8 {
	struct {
		v4u_x2 x, y, z;
	};
	v3u_x8 operator+(v3u_x8 b) const { return {x + b.x, y + b.y, z + b.z}; }
	v3u_x8 operator-(v3u_x8 b) const { return {x - b.x, y - b.y, z - b.z}; }
	v3u_x8 operator*(v3u_x8 b) const { return {x * b.x, y * b.y, z * b.z}; }
	v3u_x8 operator/(v3u_x8 b) const { return {x / b.x, y / b.y, z / b.z}; }
	v3u_x8 operator%(v3u_x8 b) const { return {x % b.x, y % b.y, z % b.z}; }
	v3u_x8 operator^(v3u_x8 b) const { return {x ^ b.x, y ^ b.y, z ^ b.z}; }
	v3u_x8 operator|(v3u_x8 b) const { return {x | b.x, y | b.y, z | b.z}; }
	v3u_x8 operator&(v3u_x8 b) const { return {x & b.x, y & b.y, z & b.z}; }
	v3u_x8 operator<<(v3u_x8 b) const { return {x << b.x, y << b.y, z << b.z}; }
	v3u_x8 operator>>(v3u_x8 b) const { return {x >> b.x, y >> b.y, z >> b.z}; }
	bool operator==(v3u_x8 b) const { return x == b.x && y == b.y && z == b.z; }
	MEMFUNS(v3u_x8, u32, V3u_x8);
};
union v2i_x8 {
	struct {
		v4i_x2 x, y;
	};
	v2i_x8 operator+(v2i_x8 b) const { return {x + b.x, y + b.y}; }
	v2i_x8 operator-(v2i_x8 b) const { return {x - b.x, y - b.y}; }
	v2i_x8 operator*(v2i_x8 b) const { return {x * b.x, y * b.y}; }
	v2i_x8 operator/(v2i_x8 b) const { return {x / b.x, y / b.y}; }
	v2i_x8 operator%(v2i_x8 b) const { return {x % b.x, y % b.y}; }
	v2i_x8 operator^(v2i_x8 b) const { return {x ^ b.x, y ^ b.y}; }
	v2i_x8 operator|(v2i_x8 b) const { return {x | b.x, y | b.y}; }
	v2i_x8 operator&(v2i_x8 b) const { return {x & b.x, y & b.y}; }
	v2i_x8 operator<<(v2i_x8 b) const { return {x << b.x, y << b.y}; }
	v2i_x8 operator>>(v2i_x8 b) const { return {x >> b.x, y >> b.y}; }
	bool operator==(v2i_x8 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2i_x8, i32, V2i_x8);
};
union v2u_x8 {
	struct {
		v4u_x2 x, y;
	};
	v2u_x8 operator+(v2u_x8 b) const { return {x + b.x, y + b.y}; }
	v2u_x8 operator-(v2u_x8 b) const { return {x - b.x, y - b.y}; }
	v2u_x8 operator*(v2u_x8 b) const { return {x * b.x, y * b.y}; }
	v2u_x8 operator/(v2u_x8 b) const { return {x / b.x, y / b.y}; }
	v2u_x8 operator%(v2u_x8 b) const { return {x % b.x, y % b.y}; }
	v2u_x8 operator^(v2u_x8 b) const { return {x ^ b.x, y ^ b.y}; }
	v2u_x8 operator|(v2u_x8 b) const { return {x | b.x, y | b.y}; }
	v2u_x8 operator&(v2u_x8 b) const { return {x & b.x, y & b.y}; }
	v2u_x8 operator<<(v2u_x8 b) const { return {x << b.x, y << b.y}; }
	v2u_x8 operator>>(v2u_x8 b) const { return {x >> b.x, y >> b.y}; }
	bool operator==(v2u_x8 b) const { return x == b.x && y == b.y; }
	MEMFUNS(v2u_x8, u32, V2u_x8);
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
		__m128 x = _mm_mul_ps(_mm_set1_ps(b.x), i.m);
		__m128 y = _mm_mul_ps(_mm_set1_ps(b.y), j.m);
		__m128 z = _mm_mul_ps(_mm_set1_ps(b.z), k.m);
		__m128 r = _mm_add_ps(_mm_add_ps(x, y), z);
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
	using Type = v3_x4;
};
template <>
struct m256<v3> {
	using Type = v3_x8;
};

#undef MEMFUNS

#define CON(scl, vec2, con)                             \
	constexpr vec2 con(scl x, scl y) { return {x, y}; } \
	constexpr vec2 con(scl v) { return {v, v}; }
CON(f32, v2, V2);
CON(i32, v2i, V2i);
CON(u32, v2u, V2u);

constexpr v2 V2(v2i v) { return {(f32)v.x, (f32)v.y}; }
constexpr v2 V2(v2u v) { return {(f32)v.x, (f32)v.y}; }
constexpr v2i V2i(v2 v) { return {(i32)v.x, (i32)v.y}; }
constexpr v2i V2i(v2u v) { return {(i32)v.x, (i32)v.y}; }
constexpr v2u V2u(v2 v) { return {(u32)v.x, (u32)v.y}; }
constexpr v2u V2u(v2i v) { return {(u32)v.x, (u32)v.y}; }

#undef CON
#define CON(scl, vec2, vec3, con)                                  \
	constexpr vec3 con(scl x, scl y, scl z) { return {x, y, z}; }  \
	constexpr vec3 con(scl v) { return {v, v, v}; }                \
	constexpr vec3 con(vec2 xy, scl z) { return {xy.x, xy.y, z}; } \
	constexpr vec3 con(scl x, vec2 yz) { return {x, yz.x, yz.y}; }

CON(f32, v2, v3, V3);
CON(i32, v2i, v3i, V3i);
CON(u32, v2u, v3u, V3u);

constexpr v3 V3(v3i v) { return V3((f32)v.x, (f32)v.y, (f32)v.z); }
constexpr v3 V3(v3u v) { return V3((f32)v.x, (f32)v.y, (f32)v.z); }
constexpr v3i V3i(v3 v) { return V3i((i32)v.x, (i32)v.y, (i32)v.z); }
constexpr v3i V3i(v3u v) { return V3i((i32)v.x, (i32)v.y, (i32)v.z); }
constexpr v3u V3u(v3 v) { return V3u((u32)v.x, (u32)v.y, (u32)v.z); }
constexpr v3u V3u(v3i v) { return V3u((u32)v.x, (u32)v.y, (u32)v.z); }

#undef CON
#define CON(scl, vec2, vec3, vec4, con)                                    \
	inline vec4 con(scl x, scl y, scl z, scl w) { return {x, y, z, w}; }   \
	inline vec4 con(scl v) { return {v, v, v, v}; }                        \
	inline vec4 con(vec2 xy, vec2 zw) { return {xy.x, xy.y, zw.x, zw.y}; } \
	inline vec4 con(vec2 xy, scl z, scl w) { return {xy.x, xy.y, z, w}; }  \
	inline vec4 con(scl x, scl y, vec2 zw) { return {x, y, zw.x, zw.y}; }  \
	inline vec4 con(vec3 xyz, scl w) { return {xyz.x, xyz.y, xyz.z, w}; }  \
	inline vec4 con(scl x, vec3 yzw) { return {x, yzw.x, yzw.y, yzw.z}; }

CON(f32, v2, v3, v4, V4);
CON(i32, v2i, v3i, v4i, V4i);
CON(u32, v2u, v3u, v4u, V4u);

constexpr v4 V4(__m128 m) {
	v4 r{};
	r.m = m;
	return r;
}
constexpr v4i V4i(__m128i m) {
	v4i r{};
	r.m = m;
	return r;
}
constexpr v4u V4u(__m128i m) {
	v4u r{};
	r.m = m;
	return r;
}

inline v4 V4(v4i v) { return V4(_mm_cvtepi32_ps(v.m)); }
inline v4 V4(v4u v) { return V4((f32)v.x, (f32)v.y, (f32)v.z, (f32)v.w); }
inline v4i V4i(v4 v) { return V4i(_mm_cvtps_epi32(v.m)); }
constexpr v4i V4i(v4u v) { return V4i(v.m); }
inline v4u V4u(v4 v) { return V4u((u32)v.x, (u32)v.y, (u32)v.z, (u32)v.w); }
constexpr v4u V4u(v4i v) { return V4u(v.m); }

#define v4x2(type, con, vec4, m256)                       \
	constexpr type con(vec4 a) { return {a, a}; }         \
	constexpr type con(vec4 a, vec4 b) { return {a, b}; } \
	constexpr type con(m256 m) {                          \
		type r{};                                         \
		r.m = m;                                          \
		return r;                                         \
	}

v4x2(v4_x2, V4_x2, v4, __m256);
v4x2(v4i_x2, V4i_x2, v4i, __m256i);
v4x2(v4u_x2, V4u_x2, v4u, __m256i);
#undef v4x2

constexpr v4_x2 V4_x2(f32 v) { return V4_x2(_mm256_set1_ps(v)); }
constexpr v4_x2 V4_x2(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
	return V4_x2(_mm256_setr_ps(a, b, c, d, e, f, g, h));
}
constexpr v4_x2 V4_x2(v4i_x2 v) { return V4_x2(_mm256_cvtepi32_ps(v.m)); }
constexpr v4_x2 V4_x2(v4u_x2 v) { return V4_x2(_mm256_cvtepi32_ps(v.m)); }

constexpr v4i_x2 V4i_x2(i32 v) { return V4i_x2(_mm256_set1_epi32(v)); }
constexpr v4i_x2 V4i_x2(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 h) {
	return V4i_x2(_mm256_setr_epi32(a, b, c, d, e, f, g, h));
}
constexpr v4i_x2 V4i_x2(v4_x2 v) { return V4i_x2(_mm256_cvtps_epi32(v.m)); }
constexpr v4i_x2 V4i_x2(v4u_x2 v) { return V4i_x2(v.m); }

constexpr v4u_x2 V4u_x2(u32 v) { return V4u_x2(_mm256_set1_epi32(i32(v))); }
constexpr v4u_x2 V4u_x2(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) {
	return V4u_x2(_mm256_setr_epi32((i32)a, (i32)b, (i32)c, (i32)d, (i32)e, (i32)f, (i32)g, (i32)h));
}
constexpr v4u_x2 V4u_x2(v4_x2 v) { return V4u_x2(_mm256_cvtps_epi32(v.m)); }
constexpr v4u_x2 V4u_x2(v4i_x2 v) { return V4u_x2(v.m); }

#define v2x8(vec, con, vec2, v4x2con, scl, v4x2)                                                                   \
	constexpr vec con(v4x2 x, v4x2 y) { return {x, y}; }                                                           \
	inline vec con(scl ix, scl iy, scl jx, scl jy, scl kx, scl ky, scl lx, scl ly, scl ax, scl ay, scl bx, scl by, \
				   scl cx, scl cy, scl dx, scl dy) {                                                               \
		return {                                                                                                   \
			v4x2con(ix, jx, kx, lx, ax, bx, cx, dx),                                                               \
			v4x2con(iy, jy, ky, ly, ay, by, cy, dy),                                                               \
		};                                                                                                         \
	}                                                                                                              \
	inline vec con(vec2 i, vec2 j, vec2 k, vec2 l, vec2 a, vec2 b, vec2 c, vec2 d) {                               \
		return {                                                                                                   \
			v4x2con(i.x, j.x, k.x, l.x, a.x, b.x, c.x, d.x),                                                       \
			v4x2con(i.y, j.y, k.y, l.y, a.y, b.y, c.y, d.y),                                                       \
		};                                                                                                         \
	}                                                                                                              \
	inline vec con(scl v) { return {v4x2con(v), v4x2con(v)}; }                                                     \
	constexpr vec con(v4x2 v) { return {v, v}; }                                                                   \
	inline vec con(vec2 v) { return {v4x2con(v.x), v4x2con(v.y)}; }

v2x8(v2_x8, V2_x8, v2, V4_x2, f32, v4_x2);
v2x8(v2i_x8, V2i_x8, v2i, V4i_x2, i32, v4i_x2);
v2x8(v2u_x8, V2u_x8, v2u, V4u_x2, u32, v4u_x2);

#define v3x4(x4, con4, vec3, vec4, vec4con, scl, m128)                                                               \
	constexpr x4 con4(m128 x, m128 y, m128 z) {                                                                      \
		x4 r{};                                                                                                      \
		r.x.m = x;                                                                                                   \
		r.y.m = y;                                                                                                   \
		r.z.m = z;                                                                                                   \
		return r;                                                                                                    \
	}                                                                                                                \
	inline x4 con4(scl ix, scl iy, scl iz, scl jx, scl jy, scl jz, scl kx, scl ky, scl kz, scl lx, scl ly, scl lz) { \
		return {                                                                                                     \
			vec4con(ix, jx, kx, lx),                                                                                 \
			vec4con(iy, jy, ky, ly),                                                                                 \
			vec4con(iz, jz, kz, lz),                                                                                 \
		};                                                                                                           \
	}                                                                                                                \
	inline x4 con4(vec3 i, vec3 j, vec3 k, vec3 l) {                                                                 \
		return {                                                                                                     \
			vec4con(i.x, j.x, k.x, l.x),                                                                             \
			vec4con(i.y, j.y, k.y, l.y),                                                                             \
			vec4con(i.z, j.z, k.z, l.z),                                                                             \
		};                                                                                                           \
	}                                                                                                                \
	constexpr x4 con4(vec4 x, vec4 y, vec4 z) { return {x, y, z}; }                                                  \
	inline x4 con4(scl v) { return {vec4con(v), vec4con(v), vec4con(v)}; }                                           \
	constexpr x4 con4(vec4 v) { return {v, v, v}; }                                                                  \
	inline x4 con4(vec3 v) { return {vec4con(v.x), vec4con(v.y), vec4con(v.z)}; }

v3x4(v3_x4, V3_x4, v3, v4, V4, f32, __m128);
v3x4(v3i_x4, V3i_x4, v3i, v4i, V4i, i32, __m128i);
v3x4(v3u_x4, V3u_x4, v3u, v4u, V4u, u32, __m128i);

inline v3_x4 V3_x4(v3i_x4 v) { return V3_x4(V4(v.x), V4(v.y), V4(v.z)); }
inline v3_x4 V3_x4(v3u_x4 v) { return V3_x4(V4(v.x), V4(v.y), V4(v.z)); }

inline v3i_x4 V3i_x4(v3_x4 v) { return V3i_x4(V4i(v.x), V4i(v.y), V4i(v.z)); }
inline v3i_x4 V3i_x4(v3u_x4 v) { return V3i_x4(V4i(v.x), V4i(v.y), V4i(v.z)); }

inline v3u_x4 V3u_x4(v3_x4 v) { return V3u_x4(V4u(v.x), V4u(v.y), V4u(v.z)); }
inline v3u_x4 V3u_x4(v3i_x4 v) { return V3u_x4(V4u(v.x), V4u(v.y), V4u(v.z)); }

inline v3_x8 V3_x8(v4_x2 x, v4_x2 y, v4_x2 z) { return {x, y, z}; }
inline v3_x8 V3_x8(f32 v) { return {V4_x2(v), V4_x2(v), V4_x2(v)}; }
inline v3_x8 V3_x8(v3 v) { return V3_x8(V4_x2(v.x), V4_x2(v.y), V4_x2(v.z)); }
inline v3i_x8 V3i_x8(v4i_x2 x, v4i_x2 y, v4i_x2 z) { return {x, y, z}; }
inline v3i_x8 V3i_x8(i32 v) { return {V4i_x2(v), V4i_x2(v), V4i_x2(v)}; }
inline v3i_x8 V3i_x8(v3i v) { return V3i_x8(V4i_x2(v.x), V4i_x2(v.y), V4i_x2(v.z)); }
inline v3u_x8 V3u_x8(v4u_x2 x, v4u_x2 y, v4u_x2 z) { return {x, y, z}; }
inline v3u_x8 V3u_x8(u32 v) { return {V4u_x2(v), V4u_x2(v), V4u_x2(v)}; }
inline v3u_x8 V3u_x8(v3u v) { return V3u_x8(V4u_x2(v.x), V4u_x2(v.y), V4u_x2(v.z)); }

inline v3_x8 V3_x8(v3i_x8 v) { return V3_x8(V4_x2(v.x), V4_x2(v.y), V4_x2(v.z)); }
inline v3_x8 V3_x8(v3u_x8 v) { return V3_x8(V4_x2(v.x), V4_x2(v.y), V4_x2(v.z)); }
inline v3i_x8 V3i_x8(v3_x8 v) { return V3i_x8(V4i_x2(v.x), V4i_x2(v.y), V4i_x2(v.z)); }
inline v3i_x8 V3i_x8(v3u_x8 v) { return V3i_x8(V4i_x2(v.x), V4i_x2(v.y), V4i_x2(v.z)); }
inline v3u_x8 V3u_x8(v3_x8 v) { return V3u_x8(V4u_x2(v.x), V4u_x2(v.y), V4u_x2(v.z)); }
inline v3u_x8 V3u_x8(v3i_x8 v) { return V3u_x8(V4u_x2(v.x), V4u_x2(v.y), V4u_x2(v.z)); }

inline v2_x8 V2_x8(v2i_x8 v) { return V2_x8(V4_x2(v.x), V4_x2(v.y)); }
inline v2_x8 V2_x8(v2u_x8 v) { return V2_x8(V4_x2(v.x), V4_x2(v.y)); }
inline v2i_x8 V2i_x8(v2_x8 v) { return V2i_x8(V4i_x2(v.x), V4i_x2(v.y)); }
inline v2i_x8 V2i_x8(v2u_x8 v) { return V2i_x8(V4i_x2(v.x), V4i_x2(v.y)); }
inline v2u_x8 V2u_x8(v2_x8 v) { return V2u_x8(V4u_x2(v.x), V4u_x2(v.y)); }
inline v2u_x8 V2u_x8(v2i_x8 v) { return V2u_x8(V4u_x2(v.x), V4u_x2(v.y)); }

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
		V4(0,0,0,1),
	};
}

namespace CE {

constexpr v3_x4 V3_x4(v3 v) { return {v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z}; }
constexpr v3i_x4 V3i_x4(v3i v) { return {v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z}; }

constexpr v3i_x4 V3i_x4(i32 ix, i32 iy, i32 iz, i32 jx, i32 jy, i32 jz, i32 kx, i32 ky, i32 kz, i32 lx, i32 ly,
						i32 lz) {
	return {ix, jx, kx, lx, iy, jy, ky, ly, iz, jz, kz, lz};
}

constexpr v3i_x4 V3i_x4(v3i a, v3i b, v3i c, v3i d) {
	return {
		a.x, b.x, c.x, d.x, a.y, b.y, c.y, d.y, a.z, b.z, c.z, d.z,
	};
}

constexpr v3_x8 V3_x8(v3 v) {
	return {v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y,
			v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z, v.z, v.z, v.z, v.z};
}
constexpr v3i_x8 V3i_x8(v3i v) {
	return {v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.x, v.y, v.y, v.y, v.y,
			v.y, v.y, v.y, v.y, v.z, v.z, v.z, v.z, v.z, v.z, v.z, v.z};
}

constexpr v3i_x8 V3i_x8(i32 x0, i32 y0, i32 z0, i32 x1, i32 y1, i32 z1, i32 x2, i32 y2, i32 z2, i32 x3, i32 y3, i32 z3,
						i32 x4, i32 y4, i32 z4, i32 x5, i32 y5, i32 z5, i32 x6, i32 y6, i32 z6, i32 x7, i32 y7,
						i32 z7) {
	return {x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3, y4, y5, y6, y7, z0, z1, z2, z3, z4, z5, z6, z7};
}

constexpr v3i_x8 V3i_x8(v3i a, v3i b, v3i c, v3i d, v3i e, v3i f, v3i g, v3i h) {
	return {
		a.x, b.x, c.x, d.x, e.x, f.x, g.x, h.x, a.y, b.y, c.y, d.y,
		e.y, f.y, g.y, h.y, a.z, b.z, c.z, d.z, e.z, f.z, g.z, h.z,
	};
}

constexpr v2i_x8 V2i_x8(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, i32 x4, i32 y4, i32 x5, i32 y5,
						i32 x6, i32 y6, i32 x7, i32 y7) {
	return {x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3, y4, y5, y6, y7};
}

constexpr v4i_x2 V4i_x2(v4i a, v4i b) { return {a, b}; }

} // namespace CE

// clang-format off
template<> auto min(v2 a, v2 b) { return V2(min(a.x, b.x), min(a.y, b.y)); }
template<> auto min(v3 a, v3 b) { return V3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
template<> auto min(v4 a, v4 b) { return V4(_mm_min_ps(a.m, b.m)); }
template<> auto min(v4_x2 a, v4_x2 b) { return V4_x2(_mm256_min_ps(a.m, b.m)); }
template<> auto min(f32 a, v2 b) { return min(a, min(b.x, b.y)); }
template<> auto min(f32 a, v4 b) { return min(a, min(min(b.x, b.y), min(b.z, b.w))); }
template<> auto min(f32 a, v4_x2 b) { return min(a, min(b.i, b.j)); }
template<> auto min(v2i a, v2i b) { return V2i(min(a.x, b.x), min(a.y, b.y)); }
template<> auto min(v3i a, v3i b) { return V3i(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
template<> auto min(v4i a, v4i b) { return V4i(_mm_min_epi32(a.m, b.m)); }
template<> auto min(v2u a, v2u b) { return V2u(min(a.x, b.x), min(a.y, b.y)); }
template<> auto min(v3u a, v3u b) { return V3u(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
template<> auto min(v4u a, v4u b) { return V4u(_mm_min_epu32(a.m, b.m)); }

template<> auto max(v2 a, v2 b) { return V2(max(a.x, b.x), max(a.y, b.y)); }
template<> auto max(v3 a, v3 b) { return V3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
template<> auto max(v4 a, v4 b) { return V4(_mm_max_ps(a.m, b.m)); }
template<> auto max(v4_x2 a, v4_x2 b) { return V4_x2(_mm256_max_ps(a.m, b.m)); }
template<> auto max(f32 a, v2 b) { return max(a, max(b.x, b.y)); }
template<> auto max(f32 a, v4 b) { return max(a, max(max(b.x, b.y), max(b.z, b.w))); }
template<> auto max(f32 a, v4_x2 b) { return max(a, max(b.i, b.j)); }
template<> auto max(v2i a, v2i b) { return V2i(max(a.x, b.x), max(a.y, b.y)); }
template<> auto max(v3i a, v3i b) { return V3i(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
template<> auto max(v4i a, v4i b) { return V4i(_mm_max_epi32(a.m, b.m)); }
template<> auto max(v2u a, v2u b) { return V2u(max(a.x, b.x), max(a.y, b.y)); }
template<> auto max(v3u a, v3u b) { return V3u(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
template<> auto max(v4u a, v4u b) { return V4u(_mm_max_epu32(a.m, b.m)); }

void minmax(v2 a, v2 b, v2& mn, v2& mx) {
	minmax(a.x, b.x, mn.x, mx.x);
	minmax(a.y, b.y, mn.y, mx.y); 
}
void minmax(v2i a, v2i b, v2i& mn, v2i& mx) {
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
v4 sqrt(v4 v) {
	v.m = _mm_sqrt_ps(v.m);
	return v;
}
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
inline v2 sin(v2 v) {
	auto m = _mm_sin_ps(_mm_setr_ps(v.x, v.y, 0, 0));
	v2 result;
	memcpy(&result, &m, sizeof(result));
	return result;
}
inline v3 sin(v3 v) {
	auto m = _mm_sin_ps(_mm_setr_ps(v.x, v.y, v.z, 0));
	v3 result;
	memcpy(&result, &m, sizeof(result));
	return result;
}
inline v4 sin(v4 v) { return V4(_mm_sin_ps(v.m)); }
inline v2 cos(v2 v) {
	auto m = _mm_cos_ps(_mm_setr_ps(v.x, v.y, 0, 0));
	v2 result;
	memcpy(&result, &m, sizeof(result));
	return result;
}
inline v3 cos(v3 v) {
	auto m = _mm_cos_ps(_mm_setr_ps(v.x, v.y, v.z, 0));
	v3 result;
	memcpy(&result, &m, sizeof(result));
	return result;
}
inline v4 cos(v4 v) { return V4(_mm_cos_ps(v.m)); }
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
inline void sincos(v4 v, v4& sinOut, v4& cosOut) { sinOut.m = _mm_sincos_ps(&cosOut.m, v.m); }
#endif

constexpr f32 aspectRatio(v2 v) { return v.x / v.y; }
constexpr f32 aspectRatio(v2i v) { return aspectRatio(V2(v)); }
constexpr f32 aspectRatio(v2u v) { return aspectRatio(V2(v)); }

u32 countBits(u32 v) { return (u32)_mm_popcnt_u32(v); }
u32 countBits(i32 v) { return countBits((u32)v); }
#if !ARCH_X86
u32 countBits(u64 v) { return (u32)_mm_popcnt_u64(v); }
u32 countBits(i64 v) { return countBits((u64)v); }
#endif
u32 fillBits(u32 v) {
	u32 r = 0;
	while (v--) {
		r = (r << 1) | 1;
	}
	return r;
}
inline f32 round(f32 v) {
	return roundf(v);
}
inline v4 round(v4 v) { return V4(_mm_round_ps(v.m, _MM_FROUND_NINT)); }
inline i32 roundInt(f32 v) {
	if (v < 0)
		return int(v - 0.5f);
	return int(v + 0.5f);
}
inline v2i roundInt(v2 v) { return {roundInt(v.x), roundInt(v.y)}; }
inline v3i roundInt(v3 v) { return {roundInt(v.x), roundInt(v.y), roundInt(v.z)}; }
inline v4i roundInt(v4 v) { return V4i(round(v)); }
constexpr f32 frac(f32 v) {
	auto r = v - (i64)v;
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
v4 frac(v4 v) {
	v.m = _mm_sub_ps(v.m, _mm_cvtepi32_ps(_mm_cvtps_epi32(v.m)));
	v.m = _mm_add_ps(v.m, _mm_and_ps(_mm_cmplt_ps(v.m, _mm_setzero_ps()), _mm_set1_ps(1.0f)));
	return v;
}
constexpr i32 frac(i32 v, u32 s) {
	if (v < 0)
		return (v + 1) % i32(s) + i32(s) - 1;
	return v % i32(s);
}
v2i frac(v2i v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
	};
}
v3i frac(v3i v, u32 s) {
	return {
		frac(v.x, s),
		frac(v.y, s),
		frac(v.z, s),
	};
}
v4i frac(v4i v, u32 s) {
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
	if (i32 bits = countBits(s); bits == 1) {
		unsigned long idx;
		_BitScanForward(&idx, (u32)s);
		u32 rmask = fillBits(idx);
		__m128i mrmask = _mm_set1_epi32((i32)rmask);
		pos = _mm_and_si128(v.m, mrmask);
		neg = _mm_sub_epi32(_mm_add_epi32(_mm_and_si128(_mm_add_epi32(v.m, one), mrmask), ms), one);
	} else {
		pos = _mm_rem_epi32(v.m, ms);
		neg = _mm_sub_epi32(_mm_add_epi32(_mm_rem_epi32(_mm_add_epi32(v.m, one), ms), ms), one);
	}
	return V4i(_mm_or_si128(_mm_and_si128(mask, neg), _mm_andnot_si128(mask, pos)));
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
v4 floor(v4 v) {
	v.m = _mm_floor_ps(v.m);
	return v;
}
i32 floorInt(f32 v) { return (i32)floorf(v); }
v2i floorInt(v2 v) {
	return {
		floorInt(v.x),
		floorInt(v.y),
	};
}
v3i floorInt(v3 v) {
	return {
		floorInt(v.x),
		floorInt(v.y),
		floorInt(v.z),
	};
}
v4i floorInt(v4 v) { return V4i(_mm_cvtps_epi32(_mm_floor_ps(v.m))); }
constexpr i32 floor(i32 v, u32 s) {
	if (v < 0)
		v = (v + 1) / i32(s) - 1;
	else
		v /= i32(s);
	return v * i32(s);
}
v2i floor(v2i v, u32 step) {
	return {
		floor(v.x, step),
		floor(v.y, step),
	};
}
v3i floor(v3i v, u32 step) {
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
	};
}
v4i floor(v4i v, u32 step) {
#if 1
	return {
		floor(v.x, step),
		floor(v.y, step),
		floor(v.z, step),
		floor(v.w, step),
	};
#else
	__m128i ms = _mm_set1_epi32((i32)step);
	__m128i one = _mm_set1_epi32(1);
	__m128i pos = _mm_div_epi32(v.m, ms);
	__m128i neg = _mm_sub_epi32(_mm_div_epi32(_mm_add_epi32(v.m, one), ms), one);
	__m128i mask = _mm_cmplt_epi32(v.m, _mm_setzero_si128());
	return V4i(_mm_mullo_epi32(_mm_or_si128(_mm_and_si128(mask, neg), _mm_andnot_si128(mask, pos)),
							   _mm_set1_epi32((i32)step)));
#endif
}
f32 dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
f32 dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
f32 dot(v4 a, v4 b) {
	f32 result;
	_mm_store_ss(&result, _mm_dp_ps(a.m, b.m, 0xFF));
	return result;
}
v2 dot(v4_x2 a, v4_x2 b) {
	auto dp = _mm256_dp_ps(a.m, b.m, 0xF1);
	return V2(dp.m256_f32[0], dp.m256_f32[4]);
}
v4 dot(v3_x4 a, v3_x4 b) {
	a *= b;
	return a.x + a.y + a.z;
}
v4_x2 dot(v3_x8 a, v3_x8 b) {
	a *= b;
	return a.x + a.y + a.z;
}
v4_x2 dot(v2_x8 a, v2_x8 b) {
	a *= b;
	return a.x + a.y;
}
i32 dot(v2i a, v2i b) { return a.x * b.x + a.y * b.y; }
i32 dot(v3i a, v3i b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
i32 dot(v4i a, v4i b) {
	a *= b;
	return a.x + a.y + a.z + a.w;
}
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

v2 abs(v2 v) { return {fabsf(v.x),fabsf(v.y)};  }
v3 abs(v3 v) { return {fabsf(v.x),fabsf(v.y),fabsf(v.z)};  }
v4 abs(v4 v) { return {fabsf(v.x),fabsf(v.y),fabsf(v.z),fabsf(v.w)};  }
v2i abs(v2i a) { return {labs(a.x), labs(a.y)}; }
v3i abs(v3i a) { return {labs(a.x), labs(a.y), labs(a.z)}; }
v4i abs(v4i a) { return V4i(_mm_abs_epi32(a.m)); }

f32 sign(f32 v) { return v > 0 ? 1 : v < 0 ? -1 : 0; }

f32 sum(v2 v) {
	return v.x + v.y;
}

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
template<class T>
auto manhattan(T a, T b) {
	return sum(abs(a - b));
}
int maxDistance(v3i a, v3i b) {
	a = abs(a - b);
	return max(max(a.x, a.y), a.z);
}
inline f32 cos01(f32 t) { return 0.5f - cosf(t * PI) * 0.5f; }
/*
f32 noise(i32 x) {
	x = (x << 13) ^ x;
	return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}
f32 noise(v2i v) {
	auto n = v.x + v.y * 57;
	n	   = (n << 13) ^ n;
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483648.0f);
}
inline f32 cosNoise(f32 v) {
	auto fl = (i32)floor(v);
	return lerp(noise(fl), noise(fl + 1), cos01(frac(v)));
}
template <class Interp, class Sample>
inline f32 interpolate(v2 p, Interp&& interp, Sample&& sample) {
	auto x	= (i32)floor(p.x);
	auto y	= (i32)floor(p.y);
	auto fr = frac(p.x);
	return interp(interp(sample(x, y), sample(x + 1, y), fr), interp(sample(x, y + 1), sample(x + 1, y + 1), fr),
				  frac(p.y));
}
template <class Interp, class Sample>
inline f32 interpolate(v2i p, i32 s, Interp&& interp, Sample&& sample) {
	auto fl = floor(p, s);
	auto fr = frac(p.x, s) / f32(s);
	return interp(interp(sample(fl + v2i{0, 0}), sample(fl + v2i{1, 0}), fr),
				  interp(sample(fl + v2i{0, 1}), sample(fl + v2i{1, 1}), fr), frac(p.y, s) / f32(s));
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
i32 randomize(i32 v) { return randomize((u32)v); }
v2i randomize(v2i v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v.x = v.x * v.y + v.x;
	v.y = v.x * v.y + v.x;
	return v;
}
v2i_x8 randomize(v2i_x8 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v.x = v.x * v.y + v.x;
	v.y = v.x * v.y + v.x;
	return v;
}
v3i randomize(v3i v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v *= V3i(v.z, v.x, v.y);
	v *= V3i(v.z, v.x, v.y);
	v *= V3i(v.z, v.x, v.y);
	return v;
}
__forceinline v3i_x4 randomize(v3i_x4 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v *= V3i_x4(v.z, v.x, v.y);
	v *= V3i_x4(v.z, v.x, v.y);
	v *= V3i_x4(v.z, v.x, v.y);
	return v;
}
v3i_x8 randomize(v3i_x8 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v *= V3i_x8(v.z, v.x, v.y);
	v *= V3i_x8(v.z, v.x, v.y);
	v *= V3i_x8(v.z, v.x, v.y);
	return v;
}
v4i randomize(v4i v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v ^= V4i(V4u(v) >> 31);
	v *= V4i(_mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 1, 0, 3)));
	v *= V4i(_mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 1, 0, 3)));
	v *= V4i(_mm_shuffle_epi32(v.m, _MM_SHUFFLE(2, 1, 0, 3)));
	return v;
}
v4i_x2 randomize(v4i_x2 v) {
	v += 0x0C252DA0;
	v *= 0x034FB5E7;
	v ^= 0xF5605798;
	v ^= V4i_x2(V4u_x2(v) >> 31);
	v *= V4i_x2(_mm256_shuffle_epi32(v.m, _MM_SHUFFLE(2, 1, 0, 3)));
	v *= V4i_x2(_mm256_shuffle_epi32(v.m, _MM_SHUFFLE(2, 1, 0, 3)));
	v *= V4i_x2(_mm256_shuffle_epi32(v.m, _MM_SHUFFLE(2, 1, 0, 3)));
	return v;
}

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
u32 randomU32(i32 in) { return randomU32((u32)in); }
u32 randomU32(v2i in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	return x ^ y;
}
u32 randomU32(v3i in) {
	auto x = randomU32(in.x);
	auto y = randomU32(in.y);
	auto z = randomU32(in.z);
	return x + y + z;
}
u32 randomU32(v4i v) {
	v = randomize(v);
	return u32(v.x) + u32(v.y) + u32(v.z) + u32(v.w);
}
u64 randomU64(v3i in) {
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
	return frac(a * V4(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(1, 2, 3, 0))));
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
v4 random01_x2(v4 p) {
	p	 = frac((p + SQRT2) * V4(_randMul2, _randMul2));
	v4 a = p + V4(PI * 4);
	v2 d = V2(dot(p.xy, a.xy), dot(p.zw, a.zw));
	p += V4(d.x, d.x, d.y, d.y);
	return V4(frac(V2(p.x * p.y, p.x + p.y)),
			  frac(V2(p.z * p.w, p.z + p.w)));
}
*/
v2 random01(v2i v) {
	auto x = randomU32(v);
	auto y = randomU32(u32(x + 2454940283));
	return V2(V2u(x, y) / 256) / 16777215.f + 0.5f;
}
v3 random01(v3i v) {
	auto x = randomU32(v);
	auto y = randomU32(u32(x + 2454940283));
	auto z = randomU32(x ^ y);
	return V3(V3u(x, y, z) / 256) / 16777215.f;
}
v2 randomN(v2i v) { return V2(randomize(v) / 256) * (1.0f / 8388608.f); }
v3 randomN(v3i v) { return V3(randomize(v) / 256) * (1.0f / 8388608.f); }
v4 randomN(v4i v) { return V4(randomize(v) / 256) * (1.0f / 8388608.f); }
v4_x2 randomN(v4i_x2 v) { return V4_x2(randomize(v) / 256) * (1.0f / 8388608.f); }
v3_x4 randomN(v3i_x4 v) { return V3_x4(randomize(v) / 256) * (1.0f / 8388608.f); }
v2_x8 randomN(v2i_x8 v) { return V2_x8(randomize(v) / 256) * (1.0f / 8388608.f); }
v3_x8 randomN(v3i_x8 v) { return V3_x8(randomize(v) / 256) * (1.0f / 8388608.f); }

#define MSVC_BUG_FIXED 0
#if MSVC_BUG_FIXED

template <size_t i, size_t size, v3i const (&arr)[size], class Cb>
constexpr void wideForEachIter(Cb cb) {
	if constexpr (i + 8 <= size) {
		constexpr auto x	   = arr[0].x;
		constexpr v3i const* c = arr + i;
		constexpr v3i_x8 v	   = {
			c[0].x, c[1].x, c[2].x, c[3].x, c[4].x, c[5].x, c[6].x, c[7].x, c[0].y, c[1].y, c[2].y, c[3].y,
			c[4].y, c[5].y, c[6].y, c[7].y, c[0].z, c[1].z, c[2].z, c[3].z, c[4].z, c[5].z, c[6].z, c[7].z,
		};
		cb(v);
		wideForEachIter<i + 8, size, arr>(cb);
	} else if constexpr (i + 4 <= size) {
		constexpr v3i const* c = arr + i;
		constexpr v3i_x4 v	   = {
			c[0].x, c[1].x, c[2].x, c[3].x, c[0].y, c[1].y, c[2].y, c[3].y, c[0].z, c[1].z, c[2].z, c[3].z,
		};
		cb(v);
		wideForEachIter<i - 4, size, arr>(cb);
	} else if constexpr (i + 1 <= size) {
		cb(arr[i]);
		wideForEachIter<i + 1, size, arr>(cb);
	}
}

template <size_t size, v3i const (&arr)[size], class Cb>
constexpr void wideForEach(Cb cb) {
	wideForEachIter<0, size, arr>(cb);
	/*
	size_t i	   = 0;
	for (; i + 8 < size; i += 8) {
		constexpr v3i const* c = arr + i;
		v3i_x8 v			   = {
			  c[0].x, c[1].x, c[2].x, c[3].x, c[4].x, c[5].x, c[6].x, c[7].x, c[0].y, c[1].y, c[2].y, c[3].y,
			  c[4].y, c[5].y, c[6].y, c[7].y, c[0].z, c[1].z, c[2].z, c[3].z, c[4].z, c[5].z, c[6].z, c[7].z,
		  };
		cb(v);
	}
	for (; i + 4 < size; i += 4) {
		v3i const* c = arr + i;
		v3i_x4 v	 = {
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
constexpr size_t widthOf<v3i> = 1;
template <>
constexpr size_t widthOf<v3i_x4> = 4;
template <>
constexpr size_t widthOf<v3i_x8> = 8;

template <size_t width, class T>
inline constexpr auto widen(T v) = delete;
template <>
inline constexpr auto widen<1>(v3 v) {
	return v;
}
template <>
inline constexpr auto widen<1>(v3i v) {
	return v;
}
template <>
inline constexpr auto widen<4>(v3 v) {
	return CE::V3_x4(v);
}
template <>
inline constexpr auto widen<4>(v3_x4 v) {
	return v;
}
template <>
inline constexpr auto widen<4>(v3i v) {
	return CE::V3i_x4(v);
}
template <>
inline constexpr auto widen<4>(v3i_x4 v) {
	return v;
}
template <>
inline constexpr auto widen<8>(v3 v) {
	return CE::V3_x8(v);
}
template <>
inline constexpr auto widen<8>(v3_x8 v) {
	return v;
}
template <>
inline constexpr auto widen<8>(v3i v) {
	return CE::V3i_x8(v);
}
template <>
inline constexpr auto widen<8>(v3i_x8 v) {
	return v;
}

namespace CE {
inline constexpr v3i_x4 widen(v3i a, v3i b, v3i c, v3i d) { return CE::V3i_x4(a, b, c, d); }
inline constexpr v3i_x8 widen(v3i a, v3i b, v3i c, v3i d, v3i e, v3i f, v3i g, v3i h) {
	return CE::V3i_x8(a, b, c, d, e, f, g, h);
}
inline constexpr v4i_x2 widen(v4i a, v4i b) { return CE::V4i_x2(a, b); }
} // namespace CE

template <class To, class T>
auto wideCast(T v) = delete;

template <>
inline auto wideCast<float, v3i>(v3i v) {
	return V3(v);
}
template <>
inline auto wideCast<float, v3i_x4>(v3i_x4 v) {
	return V3_x4(v);
}
template <>
inline auto wideCast<float, v3i_x8>(v3i_x8 v) {
	return V3_x8(v);
}
template <>
inline auto wideCast<float>(v4i_x2 v) {
	return V4_x2(v);
}

template <class T, size_t size>
struct WideType {};

template <>
struct WideType<v3i, 4> {
	using Type = v3i_x4;
};
template <>
struct WideType<v3i, 8> {
	using Type = v3i_x8;
};
template <>
struct WideType<v4i, 4> {
	using Type = v4i;
};
template <>
struct WideType<v4i, 8> {
	using Type = v4i_x2;
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
		if constexpr (std::is_same_v<T8, v4i_x2>) {
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
			minDist = min(minDist, lengthSqr_x2(rel - random01_x2(tile + off) + off - 0.5f));
		}
	}
	return sqrt(minDist) * (1 / SQRT2);
}
*/
inline f32 voronoi(v2i v, u32 cellSize) {
	v2i flr		= floor(v, cellSize);
	v2i tile	= flr / (i32)cellSize;
	v2 rel		= V2(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist = 1000;
	auto tile8	= V2i_x8(tile);
	auto rel8	= V2_x8(rel);

	static constexpr v2i_x8 offset8 = CE::V2i_x8(-1, -1, -1, 0, -1, 1, 0, -1, 0, 0, 0, 1, 1, -1, 1, 0);
	auto target						= randomN(tile8 + offset8) * 0.5f + V2_x8(offset8) - rel8;
	v4_x2 dp						= dot(target, target);
	minDist							= min(minDist, dp);
	minDist							= min(minDist, lengthSqr(rel - randomN(tile + 1) * 0.5f + 1));
	return sqrt(minDist) * (1 / SQRT2);
}
inline f32 voronoi(v3i v, u32 cellSize) {
	v3i flr		 = floor(v, cellSize);
	v3i tile	 = flr / (i32)cellSize;
	v3 rel		 = V3(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist	 = 1000;
	v3i_x8 tile8 = V3i_x8(tile);
	v3_x8 rel8	 = V3_x8(rel);
#if 0
	// clang-format off
	static constexpr v3i offsets[]{ 
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
		minDist = min(minDist, lengthSqr(randomN(tile8 + offset) * 0.5f + V3_x8(offset) - rel8));
	}
	for (auto offset : wideOffsets.t1) {
		minDist = min(minDist, lengthSqr(randomN(tile + offset) * 0.5f + V3(offset) - rel));
	}
#else
	wideOffsets.forEach([&](auto const& offset) {
		static constexpr auto offsetW = widthOf<std::decay_t<decltype(offset)>>;
		// static_assert(offsetW == 8);
		// static_assert(std::is_same_v<decltype(makeWideAs<offsetW>(rel)), v3_x8>);
		minDist = min(minDist, lengthSqr(randomN(widen<offsetW>(tile) + offset) * 0.5f + wideCast<float>(offset) -
									   widen<offsetW>(rel)));
	});
#endif
#else
	// clang-format off
	static constexpr v3i_x8 offsets8[] {
		CE::V3i_x8(-1,-1,-1,-1,-1, 0,-1,-1, 1,-1, 0,-1,-1, 0, 0,-1, 0, 1,-1, 1,-1,-1, 1, 0),
		CE::V3i_x8(-1, 1, 1, 0,-1,-1, 0,-1, 0, 0,-1, 1, 0, 0,-1, 0, 0, 0, 0, 0, 1, 0, 1,-1),
		CE::V3i_x8( 0, 1, 0, 0, 1, 1, 1,-1,-1, 1,-1, 0, 1,-1, 1, 1, 0,-1, 1, 0, 0, 1, 0, 1),
	};
	static constexpr v3i offsets[]{ 
		{1, 1,-1}, 
		{1, 1, 0}, 
		{1, 1, 1}
	};
	// clang-format on
	for (auto offset : offsets8) {
		minDist = min(minDist, lengthSqr(randomN(tile8 + offset) * 0.5f + V3_x8(offset) - rel8));
	}
	for (auto offset : offsets) {
		minDist = min(minDist, lengthSqr(randomN(tile + offset) * 0.5f + V3(offset) - rel));
	}
#endif
	return sqrt(minDist) * (1 / SQRT3);
}
inline f32 voronoi(v4i v, u32 cellSize) {
	v4i flr		 = floor(v, cellSize);
	v4i tile	 = flr / (i32)cellSize;
	v4 rel		 = V4(v - flr) / (f32)cellSize - 0.5f;
	f32 minDist	 = 1000;
	v4i_x2 tile8 = V4i_x2(tile);
	v4_x2 rel8	 = V4_x2(rel);
#if 1
	// clang-format off
	v4i offsets[] {
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
	static constexpr v4i_x2 offsets256[] {
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
		v4_x2 target = randomN(tile8 + offset) * 0.5f + V4_x2(offset) - rel8;
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
	i32 min		= -1;
	f32 minDist = FLT_MAX;
	for (i32 i = 0; i < 6; ++i) {
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
	i32 a = (i32)f;
	i32 b = a + 1;
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
constexpr v4i frac(v4i v, u32 step) {
	return {
		TL::frac(v.x, step),
		TL::frac(v.y, step),
		TL::frac(v.z, step),
		TL::frac(v.w, step),
	};
}

} // namespace CE

} // namespace TL

#if COMPILER_MSVC
#pragma warning(pop)
#endif
