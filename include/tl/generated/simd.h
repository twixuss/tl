#pragma once
#include <tl/common.h>

namespace tl {
namespace simd_typed {

union u8x16;
union u16x8;
union u32x4;
union u64x2;
union s8x16;
union s16x8;
union s32x4;
union s64x2;
union f32x4;
union f64x2;

#if ARCH_AVX
union u8x32;
union u16x16;
union u32x8;
union u64x4;
union s8x32;
union s16x16;
union s32x8;
union s64x4;
union f32x8;
union f64x4;

#endif

#if ARCH_AVX512
union u8x64;
union u16x32;
union u32x16;
union u64x8;
union s8x64;
union s16x32;
union s32x16;
union s64x8;
union f32x16;
union f64x8;

#endif
union u8x16 {
	__m128i m;
	u8 s[16];
	forceinline u8 operator[](umm i) const { return s[i]; }
	forceinline u8 &operator[](umm i) { return s[i]; }
	forceinline friend u8x16 operator+(u8x16 a) { return a; }
	forceinline friend u8x16 operator-(u8x16 a) { return u8x16{} - a; }
	forceinline friend u8x16 operator~(u8x16 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u8x16 operator+(u8x16 a, u8x16 b) { return {_mm_add_epi8(a.m, b.m)}; }
	forceinline friend u8x16 operator+(u8x16 a, u8 b) { return {_mm_add_epi8(a.m, _mm_set1_epi8(b))}; }
	forceinline friend u8x16 operator+(u8 a, u8x16 b) { return {_mm_add_epi8(_mm_set1_epi8(a), b.m)}; }
	forceinline friend u8x16 &operator+=(u8x16 &a, u8x16 b) { return a=a+b; }
	forceinline friend u8x16 &operator+=(u8x16 &a, u8 b) { return a=a+b; }
	forceinline friend u8x16 operator-(u8x16 a, u8x16 b) { return {_mm_sub_epi8(a.m, b.m)}; }
	forceinline friend u8x16 operator-(u8x16 a, u8 b) { return {_mm_sub_epi8(a.m, _mm_set1_epi8(b))}; }
	forceinline friend u8x16 operator-(u8 a, u8x16 b) { return {_mm_sub_epi8(_mm_set1_epi8(a), b.m)}; }
	forceinline friend u8x16 &operator-=(u8x16 &a, u8x16 b) { return a=a-b; }
	forceinline friend u8x16 &operator-=(u8x16 &a, u8 b) { return a=a-b; }
	forceinline friend u8x16 operator*(u8x16 a, u8x16 b) {((u8*)&a)[0] *= ((u8*)&b)[0]; ((u8*)&a)[1] *= ((u8*)&b)[1]; ((u8*)&a)[2] *= ((u8*)&b)[2]; ((u8*)&a)[3] *= ((u8*)&b)[3]; ((u8*)&a)[4] *= ((u8*)&b)[4]; ((u8*)&a)[5] *= ((u8*)&b)[5]; ((u8*)&a)[6] *= ((u8*)&b)[6]; ((u8*)&a)[7] *= ((u8*)&b)[7]; ((u8*)&a)[8] *= ((u8*)&b)[8]; ((u8*)&a)[9] *= ((u8*)&b)[9]; ((u8*)&a)[10] *= ((u8*)&b)[10]; ((u8*)&a)[11] *= ((u8*)&b)[11]; ((u8*)&a)[12] *= ((u8*)&b)[12]; ((u8*)&a)[13] *= ((u8*)&b)[13]; ((u8*)&a)[14] *= ((u8*)&b)[14]; ((u8*)&a)[15] *= ((u8*)&b)[15]; return a; }
	forceinline friend u8x16 operator*(u8x16 a, u8 b) {((u8*)&a)[0] *= b; ((u8*)&a)[1] *= b; ((u8*)&a)[2] *= b; ((u8*)&a)[3] *= b; ((u8*)&a)[4] *= b; ((u8*)&a)[5] *= b; ((u8*)&a)[6] *= b; ((u8*)&a)[7] *= b; ((u8*)&a)[8] *= b; ((u8*)&a)[9] *= b; ((u8*)&a)[10] *= b; ((u8*)&a)[11] *= b; ((u8*)&a)[12] *= b; ((u8*)&a)[13] *= b; ((u8*)&a)[14] *= b; ((u8*)&a)[15] *= b; return a; }
	forceinline friend u8x16 operator*(u8 a, u8x16 b) {((u8*)&b)[0] *= a; ((u8*)&b)[1] *= a; ((u8*)&b)[2] *= a; ((u8*)&b)[3] *= a; ((u8*)&b)[4] *= a; ((u8*)&b)[5] *= a; ((u8*)&b)[6] *= a; ((u8*)&b)[7] *= a; ((u8*)&b)[8] *= a; ((u8*)&b)[9] *= a; ((u8*)&b)[10] *= a; ((u8*)&b)[11] *= a; ((u8*)&b)[12] *= a; ((u8*)&b)[13] *= a; ((u8*)&b)[14] *= a; ((u8*)&b)[15] *= a; return b; }
	forceinline friend u8x16 &operator*=(u8x16 &a, u8x16 b) { return a=a*b; }
	forceinline friend u8x16 &operator*=(u8x16 &a, u8 b) { return a=a*b; }
	forceinline friend u8x16 operator/(u8x16 a, u8x16 b) { return {_mm_div_epu8(a.m, b.m)}; }
	forceinline friend u8x16 operator/(u8x16 a, u8 b) { return {_mm_div_epu8(a.m, _mm_set1_epi8(b))}; }
	forceinline friend u8x16 operator/(u8 a, u8x16 b) { return {_mm_div_epu8(_mm_set1_epi8(a), b.m)}; }
	forceinline friend u8x16 &operator/=(u8x16 &a, u8x16 b) { return a=a/b; }
	forceinline friend u8x16 &operator/=(u8x16 &a, u8 b) { return a=a/b; }
	forceinline friend u8x16 operator^(u8x16 a, u8x16 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u8x16 operator^(u8x16 a, u8 b) { return {_mm_xor_si128(a.m, _mm_set1_epi8(b))}; }
	forceinline friend u8x16 operator^(u8 a, u8x16 b) { return {_mm_xor_si128(_mm_set1_epi8(a), b.m)}; }
	forceinline friend u8x16 &operator^=(u8x16 &a, u8x16 b) { return a=a^b; }
	forceinline friend u8x16 &operator^=(u8x16 &a, u8 b) { return a=a^b; }
	forceinline friend u8x16 operator&(u8x16 a, u8x16 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u8x16 operator&(u8x16 a, u8 b) { return {_mm_and_si128(a.m, _mm_set1_epi8(b))}; }
	forceinline friend u8x16 operator&(u8 a, u8x16 b) { return {_mm_and_si128(_mm_set1_epi8(a), b.m)}; }
	forceinline friend u8x16 &operator&=(u8x16 &a, u8x16 b) { return a=a&b; }
	forceinline friend u8x16 &operator&=(u8x16 &a, u8 b) { return a=a&b; }
	forceinline friend u8x16 operator|(u8x16 a, u8x16 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u8x16 operator|(u8x16 a, u8 b) { return {_mm_or_si128(a.m, _mm_set1_epi8(b))}; }
	forceinline friend u8x16 operator|(u8 a, u8x16 b) { return {_mm_or_si128(_mm_set1_epi8(a), b.m)}; }
	forceinline friend u8x16 &operator|=(u8x16 &a, u8x16 b) { return a=a|b; }
	forceinline friend u8x16 &operator|=(u8x16 &a, u8 b) { return a=a|b; }
	forceinline operator s8x16() const;
};

forceinline u8x16 U8x16(u8 a) { return {_mm_set1_epi8(a)}; }
forceinline u8x16 U8x16(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15) { return {_mm_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }


union u16x8 {
	__m128i m;
	u16 s[8];
	forceinline u16 operator[](umm i) const { return s[i]; }
	forceinline u16 &operator[](umm i) { return s[i]; }
	forceinline friend u16x8 operator+(u16x8 a) { return a; }
	forceinline friend u16x8 operator-(u16x8 a) { return u16x8{} - a; }
	forceinline friend u16x8 operator~(u16x8 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u16x8 operator+(u16x8 a, u16x8 b) { return {_mm_add_epi16(a.m, b.m)}; }
	forceinline friend u16x8 operator+(u16x8 a, u16 b) { return {_mm_add_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator+(u16 a, u16x8 b) { return {_mm_add_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator+=(u16x8 &a, u16x8 b) { return a=a+b; }
	forceinline friend u16x8 &operator+=(u16x8 &a, u16 b) { return a=a+b; }
	forceinline friend u16x8 operator-(u16x8 a, u16x8 b) { return {_mm_sub_epi16(a.m, b.m)}; }
	forceinline friend u16x8 operator-(u16x8 a, u16 b) { return {_mm_sub_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator-(u16 a, u16x8 b) { return {_mm_sub_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator-=(u16x8 &a, u16x8 b) { return a=a-b; }
	forceinline friend u16x8 &operator-=(u16x8 &a, u16 b) { return a=a-b; }
	forceinline friend u16x8 operator*(u16x8 a, u16x8 b) { return {_mm_mullo_epi16(a.m, b.m)}; }
	forceinline friend u16x8 operator*(u16x8 a, u16 b) { return {_mm_mullo_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator*(u16 a, u16x8 b) { return {_mm_mullo_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator*=(u16x8 &a, u16x8 b) { return a=a*b; }
	forceinline friend u16x8 &operator*=(u16x8 &a, u16 b) { return a=a*b; }
	forceinline friend u16x8 operator/(u16x8 a, u16x8 b) { return {_mm_div_epu16(a.m, b.m)}; }
	forceinline friend u16x8 operator/(u16x8 a, u16 b) { return {_mm_div_epu16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator/(u16 a, u16x8 b) { return {_mm_div_epu16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator/=(u16x8 &a, u16x8 b) { return a=a/b; }
	forceinline friend u16x8 &operator/=(u16x8 &a, u16 b) { return a=a/b; }
	forceinline friend u16x8 operator^(u16x8 a, u16x8 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u16x8 operator^(u16x8 a, u16 b) { return {_mm_xor_si128(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator^(u16 a, u16x8 b) { return {_mm_xor_si128(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator^=(u16x8 &a, u16x8 b) { return a=a^b; }
	forceinline friend u16x8 &operator^=(u16x8 &a, u16 b) { return a=a^b; }
	forceinline friend u16x8 operator&(u16x8 a, u16x8 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u16x8 operator&(u16x8 a, u16 b) { return {_mm_and_si128(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator&(u16 a, u16x8 b) { return {_mm_and_si128(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator&=(u16x8 &a, u16x8 b) { return a=a&b; }
	forceinline friend u16x8 &operator&=(u16x8 &a, u16 b) { return a=a&b; }
	forceinline friend u16x8 operator|(u16x8 a, u16x8 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u16x8 operator|(u16x8 a, u16 b) { return {_mm_or_si128(a.m, _mm_set1_epi16(b))}; }
	forceinline friend u16x8 operator|(u16 a, u16x8 b) { return {_mm_or_si128(_mm_set1_epi16(a), b.m)}; }
	forceinline friend u16x8 &operator|=(u16x8 &a, u16x8 b) { return a=a|b; }
	forceinline friend u16x8 &operator|=(u16x8 &a, u16 b) { return a=a|b; }
	forceinline friend u16x8 operator<<(u16x8 a, u16 b) { return {_mm_slli_epi16(a.m, b)}; }
	forceinline friend u16x8 operator>>(u16x8 a, u16 b) { return {_mm_srli_epi16(a.m, b)}; }
	forceinline operator s16x8() const;
};

forceinline u16x8 U16x8(u16 a) { return {_mm_set1_epi16(a)}; }
forceinline u16x8 U16x8(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7) { return {_mm_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7)}; }


union u32x4 {
	__m128i m;
	u32 s[4];
	forceinline u32 operator[](umm i) const { return s[i]; }
	forceinline u32 &operator[](umm i) { return s[i]; }
	forceinline friend u32x4 operator+(u32x4 a) { return a; }
	forceinline friend u32x4 operator-(u32x4 a) { return u32x4{} - a; }
	forceinline friend u32x4 operator~(u32x4 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u32x4 operator+(u32x4 a, u32x4 b) { return {_mm_add_epi32(a.m, b.m)}; }
	forceinline friend u32x4 operator+(u32x4 a, u32 b) { return {_mm_add_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator+(u32 a, u32x4 b) { return {_mm_add_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator+=(u32x4 &a, u32x4 b) { return a=a+b; }
	forceinline friend u32x4 &operator+=(u32x4 &a, u32 b) { return a=a+b; }
	forceinline friend u32x4 operator-(u32x4 a, u32x4 b) { return {_mm_sub_epi32(a.m, b.m)}; }
	forceinline friend u32x4 operator-(u32x4 a, u32 b) { return {_mm_sub_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator-(u32 a, u32x4 b) { return {_mm_sub_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator-=(u32x4 &a, u32x4 b) { return a=a-b; }
	forceinline friend u32x4 &operator-=(u32x4 &a, u32 b) { return a=a-b; }
	forceinline friend u32x4 operator*(u32x4 a, u32x4 b) { return {_mm_mullo_epi32(a.m, b.m)}; }
	forceinline friend u32x4 operator*(u32x4 a, u32 b) { return {_mm_mullo_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator*(u32 a, u32x4 b) { return {_mm_mullo_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator*=(u32x4 &a, u32x4 b) { return a=a*b; }
	forceinline friend u32x4 &operator*=(u32x4 &a, u32 b) { return a=a*b; }
	forceinline friend u32x4 operator/(u32x4 a, u32x4 b) { return {_mm_div_epu32(a.m, b.m)}; }
	forceinline friend u32x4 operator/(u32x4 a, u32 b) { return {_mm_div_epu32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator/(u32 a, u32x4 b) { return {_mm_div_epu32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator/=(u32x4 &a, u32x4 b) { return a=a/b; }
	forceinline friend u32x4 &operator/=(u32x4 &a, u32 b) { return a=a/b; }
	forceinline friend u32x4 operator^(u32x4 a, u32x4 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u32x4 operator^(u32x4 a, u32 b) { return {_mm_xor_si128(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator^(u32 a, u32x4 b) { return {_mm_xor_si128(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator^=(u32x4 &a, u32x4 b) { return a=a^b; }
	forceinline friend u32x4 &operator^=(u32x4 &a, u32 b) { return a=a^b; }
	forceinline friend u32x4 operator&(u32x4 a, u32x4 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u32x4 operator&(u32x4 a, u32 b) { return {_mm_and_si128(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator&(u32 a, u32x4 b) { return {_mm_and_si128(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator&=(u32x4 &a, u32x4 b) { return a=a&b; }
	forceinline friend u32x4 &operator&=(u32x4 &a, u32 b) { return a=a&b; }
	forceinline friend u32x4 operator|(u32x4 a, u32x4 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u32x4 operator|(u32x4 a, u32 b) { return {_mm_or_si128(a.m, _mm_set1_epi32(b))}; }
	forceinline friend u32x4 operator|(u32 a, u32x4 b) { return {_mm_or_si128(_mm_set1_epi32(a), b.m)}; }
	forceinline friend u32x4 &operator|=(u32x4 &a, u32x4 b) { return a=a|b; }
	forceinline friend u32x4 &operator|=(u32x4 &a, u32 b) { return a=a|b; }
	forceinline friend u32x4 operator<<(u32x4 a, u32 b) { return {_mm_slli_epi32(a.m, b)}; }
	forceinline friend u32x4 operator>>(u32x4 a, u32 b) { return {_mm_srli_epi32(a.m, b)}; }
	forceinline operator s32x4() const;
	forceinline operator f32x4() const;
};

forceinline u32x4 U32x4(u32 a) { return {_mm_set1_epi32(a)}; }
forceinline u32x4 U32x4(u32 _0,u32 _1,u32 _2,u32 _3) { return {_mm_setr_epi32(_0,_1,_2,_3)}; }


union u64x2 {
	__m128i m;
	u64 s[2];
	forceinline u64 operator[](umm i) const { return s[i]; }
	forceinline u64 &operator[](umm i) { return s[i]; }
	forceinline friend u64x2 operator+(u64x2 a) { return a; }
	forceinline friend u64x2 operator-(u64x2 a) { return u64x2{} - a; }
	forceinline friend u64x2 operator~(u64x2 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u64x2 operator+(u64x2 a, u64x2 b) { return {_mm_add_epi64(a.m, b.m)}; }
	forceinline friend u64x2 operator+(u64x2 a, u64 b) { return {_mm_add_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator+(u64 a, u64x2 b) { return {_mm_add_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator+=(u64x2 &a, u64x2 b) { return a=a+b; }
	forceinline friend u64x2 &operator+=(u64x2 &a, u64 b) { return a=a+b; }
	forceinline friend u64x2 operator-(u64x2 a, u64x2 b) { return {_mm_sub_epi64(a.m, b.m)}; }
	forceinline friend u64x2 operator-(u64x2 a, u64 b) { return {_mm_sub_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator-(u64 a, u64x2 b) { return {_mm_sub_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator-=(u64x2 &a, u64x2 b) { return a=a-b; }
	forceinline friend u64x2 &operator-=(u64x2 &a, u64 b) { return a=a-b; }
	forceinline friend u64x2 operator*(u64x2 a, u64x2 b) { return {_mm_mullo_epi64(a.m, b.m)}; }
	forceinline friend u64x2 operator*(u64x2 a, u64 b) { return {_mm_mullo_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator*(u64 a, u64x2 b) { return {_mm_mullo_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator*=(u64x2 &a, u64x2 b) { return a=a*b; }
	forceinline friend u64x2 &operator*=(u64x2 &a, u64 b) { return a=a*b; }
	forceinline friend u64x2 operator/(u64x2 a, u64x2 b) { return {_mm_div_epu64(a.m, b.m)}; }
	forceinline friend u64x2 operator/(u64x2 a, u64 b) { return {_mm_div_epu64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator/(u64 a, u64x2 b) { return {_mm_div_epu64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator/=(u64x2 &a, u64x2 b) { return a=a/b; }
	forceinline friend u64x2 &operator/=(u64x2 &a, u64 b) { return a=a/b; }
	forceinline friend u64x2 operator^(u64x2 a, u64x2 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u64x2 operator^(u64x2 a, u64 b) { return {_mm_xor_si128(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator^(u64 a, u64x2 b) { return {_mm_xor_si128(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator^=(u64x2 &a, u64x2 b) { return a=a^b; }
	forceinline friend u64x2 &operator^=(u64x2 &a, u64 b) { return a=a^b; }
	forceinline friend u64x2 operator&(u64x2 a, u64x2 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u64x2 operator&(u64x2 a, u64 b) { return {_mm_and_si128(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator&(u64 a, u64x2 b) { return {_mm_and_si128(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator&=(u64x2 &a, u64x2 b) { return a=a&b; }
	forceinline friend u64x2 &operator&=(u64x2 &a, u64 b) { return a=a&b; }
	forceinline friend u64x2 operator|(u64x2 a, u64x2 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u64x2 operator|(u64x2 a, u64 b) { return {_mm_or_si128(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend u64x2 operator|(u64 a, u64x2 b) { return {_mm_or_si128(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend u64x2 &operator|=(u64x2 &a, u64x2 b) { return a=a|b; }
	forceinline friend u64x2 &operator|=(u64x2 &a, u64 b) { return a=a|b; }
	forceinline friend u64x2 operator<<(u64x2 a, u64 b) { return {_mm_slli_epi64(a.m, b)}; }
	forceinline friend u64x2 operator>>(u64x2 a, u64 b) { return {_mm_srli_epi64(a.m, b)}; }
	forceinline operator s64x2() const;
	forceinline operator f64x2() const;
};

forceinline u64x2 U64x2(u64 a) { return {_mm_set1_epi64x(a)}; }
forceinline u64x2 U64x2(u64 _0,u64 _1) { return {_mm_setr_epi64x(_0,_1)}; }


union s8x16 {
	__m128i m;
	s8 s[16];
	forceinline s8 operator[](umm i) const { return s[i]; }
	forceinline s8 &operator[](umm i) { return s[i]; }
	forceinline friend s8x16 operator+(s8x16 a) { return a; }
	forceinline friend s8x16 operator-(s8x16 a) { return s8x16{} - a; }
	forceinline friend s8x16 operator~(s8x16 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s8x16 operator+(s8x16 a, s8x16 b) { return {_mm_add_epi8(a.m, b.m)}; }
	forceinline friend s8x16 operator+(s8x16 a, s8 b) { return {_mm_add_epi8(a.m, _mm_set1_epi8(b))}; }
	forceinline friend s8x16 operator+(s8 a, s8x16 b) { return {_mm_add_epi8(_mm_set1_epi8(a), b.m)}; }
	forceinline friend s8x16 &operator+=(s8x16 &a, s8x16 b) { return a=a+b; }
	forceinline friend s8x16 &operator+=(s8x16 &a, s8 b) { return a=a+b; }
	forceinline friend s8x16 operator-(s8x16 a, s8x16 b) { return {_mm_sub_epi8(a.m, b.m)}; }
	forceinline friend s8x16 operator-(s8x16 a, s8 b) { return {_mm_sub_epi8(a.m, _mm_set1_epi8(b))}; }
	forceinline friend s8x16 operator-(s8 a, s8x16 b) { return {_mm_sub_epi8(_mm_set1_epi8(a), b.m)}; }
	forceinline friend s8x16 &operator-=(s8x16 &a, s8x16 b) { return a=a-b; }
	forceinline friend s8x16 &operator-=(s8x16 &a, s8 b) { return a=a-b; }
	forceinline friend s8x16 operator*(s8x16 a, s8x16 b) {((s8*)&a)[0] *= ((s8*)&b)[0]; ((s8*)&a)[1] *= ((s8*)&b)[1]; ((s8*)&a)[2] *= ((s8*)&b)[2]; ((s8*)&a)[3] *= ((s8*)&b)[3]; ((s8*)&a)[4] *= ((s8*)&b)[4]; ((s8*)&a)[5] *= ((s8*)&b)[5]; ((s8*)&a)[6] *= ((s8*)&b)[6]; ((s8*)&a)[7] *= ((s8*)&b)[7]; ((s8*)&a)[8] *= ((s8*)&b)[8]; ((s8*)&a)[9] *= ((s8*)&b)[9]; ((s8*)&a)[10] *= ((s8*)&b)[10]; ((s8*)&a)[11] *= ((s8*)&b)[11]; ((s8*)&a)[12] *= ((s8*)&b)[12]; ((s8*)&a)[13] *= ((s8*)&b)[13]; ((s8*)&a)[14] *= ((s8*)&b)[14]; ((s8*)&a)[15] *= ((s8*)&b)[15]; return a; }
	forceinline friend s8x16 operator*(s8x16 a, s8 b) {((s8*)&a)[0] *= b; ((s8*)&a)[1] *= b; ((s8*)&a)[2] *= b; ((s8*)&a)[3] *= b; ((s8*)&a)[4] *= b; ((s8*)&a)[5] *= b; ((s8*)&a)[6] *= b; ((s8*)&a)[7] *= b; ((s8*)&a)[8] *= b; ((s8*)&a)[9] *= b; ((s8*)&a)[10] *= b; ((s8*)&a)[11] *= b; ((s8*)&a)[12] *= b; ((s8*)&a)[13] *= b; ((s8*)&a)[14] *= b; ((s8*)&a)[15] *= b; return a; }
	forceinline friend s8x16 operator*(s8 a, s8x16 b) {((s8*)&b)[0] *= a; ((s8*)&b)[1] *= a; ((s8*)&b)[2] *= a; ((s8*)&b)[3] *= a; ((s8*)&b)[4] *= a; ((s8*)&b)[5] *= a; ((s8*)&b)[6] *= a; ((s8*)&b)[7] *= a; ((s8*)&b)[8] *= a; ((s8*)&b)[9] *= a; ((s8*)&b)[10] *= a; ((s8*)&b)[11] *= a; ((s8*)&b)[12] *= a; ((s8*)&b)[13] *= a; ((s8*)&b)[14] *= a; ((s8*)&b)[15] *= a; return b; }
	forceinline friend s8x16 &operator*=(s8x16 &a, s8x16 b) { return a=a*b; }
	forceinline friend s8x16 &operator*=(s8x16 &a, s8 b) { return a=a*b; }
	forceinline friend s8x16 operator/(s8x16 a, s8x16 b) { return {_mm_div_epi8(a.m, b.m)}; }
	forceinline friend s8x16 operator/(s8x16 a, s8 b) { return {_mm_div_epi8(a.m, _mm_set1_epi8(b))}; }
	forceinline friend s8x16 operator/(s8 a, s8x16 b) { return {_mm_div_epi8(_mm_set1_epi8(a), b.m)}; }
	forceinline friend s8x16 &operator/=(s8x16 &a, s8x16 b) { return a=a/b; }
	forceinline friend s8x16 &operator/=(s8x16 &a, s8 b) { return a=a/b; }
	forceinline friend s8x16 operator^(s8x16 a, s8x16 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s8x16 operator^(s8x16 a, s8 b) { return {_mm_xor_si128(a.m, _mm_set1_epi8(b))}; }
	forceinline friend s8x16 operator^(s8 a, s8x16 b) { return {_mm_xor_si128(_mm_set1_epi8(a), b.m)}; }
	forceinline friend s8x16 &operator^=(s8x16 &a, s8x16 b) { return a=a^b; }
	forceinline friend s8x16 &operator^=(s8x16 &a, s8 b) { return a=a^b; }
	forceinline friend s8x16 operator&(s8x16 a, s8x16 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s8x16 operator&(s8x16 a, s8 b) { return {_mm_and_si128(a.m, _mm_set1_epi8(b))}; }
	forceinline friend s8x16 operator&(s8 a, s8x16 b) { return {_mm_and_si128(_mm_set1_epi8(a), b.m)}; }
	forceinline friend s8x16 &operator&=(s8x16 &a, s8x16 b) { return a=a&b; }
	forceinline friend s8x16 &operator&=(s8x16 &a, s8 b) { return a=a&b; }
	forceinline friend s8x16 operator|(s8x16 a, s8x16 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s8x16 operator|(s8x16 a, s8 b) { return {_mm_or_si128(a.m, _mm_set1_epi8(b))}; }
	forceinline friend s8x16 operator|(s8 a, s8x16 b) { return {_mm_or_si128(_mm_set1_epi8(a), b.m)}; }
	forceinline friend s8x16 &operator|=(s8x16 &a, s8x16 b) { return a=a|b; }
	forceinline friend s8x16 &operator|=(s8x16 &a, s8 b) { return a=a|b; }
	forceinline operator u8x16() const;
};

forceinline s8x16 S8x16(s8 a) { return {_mm_set1_epi8(a)}; }
forceinline s8x16 S8x16(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15) { return {_mm_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }


union s16x8 {
	__m128i m;
	s16 s[8];
	forceinline s16 operator[](umm i) const { return s[i]; }
	forceinline s16 &operator[](umm i) { return s[i]; }
	forceinline friend s16x8 operator+(s16x8 a) { return a; }
	forceinline friend s16x8 operator-(s16x8 a) { return s16x8{} - a; }
	forceinline friend s16x8 operator~(s16x8 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s16x8 operator+(s16x8 a, s16x8 b) { return {_mm_add_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator+(s16x8 a, s16 b) { return {_mm_add_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator+(s16 a, s16x8 b) { return {_mm_add_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator+=(s16x8 &a, s16x8 b) { return a=a+b; }
	forceinline friend s16x8 &operator+=(s16x8 &a, s16 b) { return a=a+b; }
	forceinline friend s16x8 operator-(s16x8 a, s16x8 b) { return {_mm_sub_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator-(s16x8 a, s16 b) { return {_mm_sub_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator-(s16 a, s16x8 b) { return {_mm_sub_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator-=(s16x8 &a, s16x8 b) { return a=a-b; }
	forceinline friend s16x8 &operator-=(s16x8 &a, s16 b) { return a=a-b; }
	forceinline friend s16x8 operator*(s16x8 a, s16x8 b) { return {_mm_mullo_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator*(s16x8 a, s16 b) { return {_mm_mullo_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator*(s16 a, s16x8 b) { return {_mm_mullo_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator*=(s16x8 &a, s16x8 b) { return a=a*b; }
	forceinline friend s16x8 &operator*=(s16x8 &a, s16 b) { return a=a*b; }
	forceinline friend s16x8 operator/(s16x8 a, s16x8 b) { return {_mm_div_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator/(s16x8 a, s16 b) { return {_mm_div_epi16(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator/(s16 a, s16x8 b) { return {_mm_div_epi16(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator/=(s16x8 &a, s16x8 b) { return a=a/b; }
	forceinline friend s16x8 &operator/=(s16x8 &a, s16 b) { return a=a/b; }
	forceinline friend s16x8 operator^(s16x8 a, s16x8 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s16x8 operator^(s16x8 a, s16 b) { return {_mm_xor_si128(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator^(s16 a, s16x8 b) { return {_mm_xor_si128(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator^=(s16x8 &a, s16x8 b) { return a=a^b; }
	forceinline friend s16x8 &operator^=(s16x8 &a, s16 b) { return a=a^b; }
	forceinline friend s16x8 operator&(s16x8 a, s16x8 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s16x8 operator&(s16x8 a, s16 b) { return {_mm_and_si128(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator&(s16 a, s16x8 b) { return {_mm_and_si128(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator&=(s16x8 &a, s16x8 b) { return a=a&b; }
	forceinline friend s16x8 &operator&=(s16x8 &a, s16 b) { return a=a&b; }
	forceinline friend s16x8 operator|(s16x8 a, s16x8 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s16x8 operator|(s16x8 a, s16 b) { return {_mm_or_si128(a.m, _mm_set1_epi16(b))}; }
	forceinline friend s16x8 operator|(s16 a, s16x8 b) { return {_mm_or_si128(_mm_set1_epi16(a), b.m)}; }
	forceinline friend s16x8 &operator|=(s16x8 &a, s16x8 b) { return a=a|b; }
	forceinline friend s16x8 &operator|=(s16x8 &a, s16 b) { return a=a|b; }
	forceinline friend s16x8 operator<<(s16x8 a, s16 b) { return {_mm_slli_epi16(a.m, b)}; }
	forceinline friend s16x8 operator>>(s16x8 a, s16 b) { return {_mm_srai_epi16(a.m, b)}; }
	forceinline operator u16x8() const;
};

forceinline s16x8 S16x8(s16 a) { return {_mm_set1_epi16(a)}; }
forceinline s16x8 S16x8(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7) { return {_mm_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7)}; }


union s32x4 {
	__m128i m;
	s32 s[4];
	forceinline s32 operator[](umm i) const { return s[i]; }
	forceinline s32 &operator[](umm i) { return s[i]; }
	forceinline friend s32x4 operator+(s32x4 a) { return a; }
	forceinline friend s32x4 operator-(s32x4 a) { return s32x4{} - a; }
	forceinline friend s32x4 operator~(s32x4 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s32x4 operator+(s32x4 a, s32x4 b) { return {_mm_add_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator+(s32x4 a, s32 b) { return {_mm_add_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator+(s32 a, s32x4 b) { return {_mm_add_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator+=(s32x4 &a, s32x4 b) { return a=a+b; }
	forceinline friend s32x4 &operator+=(s32x4 &a, s32 b) { return a=a+b; }
	forceinline friend s32x4 operator-(s32x4 a, s32x4 b) { return {_mm_sub_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator-(s32x4 a, s32 b) { return {_mm_sub_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator-(s32 a, s32x4 b) { return {_mm_sub_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator-=(s32x4 &a, s32x4 b) { return a=a-b; }
	forceinline friend s32x4 &operator-=(s32x4 &a, s32 b) { return a=a-b; }
	forceinline friend s32x4 operator*(s32x4 a, s32x4 b) { return {_mm_mullo_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator*(s32x4 a, s32 b) { return {_mm_mullo_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator*(s32 a, s32x4 b) { return {_mm_mullo_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator*=(s32x4 &a, s32x4 b) { return a=a*b; }
	forceinline friend s32x4 &operator*=(s32x4 &a, s32 b) { return a=a*b; }
	forceinline friend s32x4 operator/(s32x4 a, s32x4 b) { return {_mm_div_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator/(s32x4 a, s32 b) { return {_mm_div_epi32(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator/(s32 a, s32x4 b) { return {_mm_div_epi32(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator/=(s32x4 &a, s32x4 b) { return a=a/b; }
	forceinline friend s32x4 &operator/=(s32x4 &a, s32 b) { return a=a/b; }
	forceinline friend s32x4 operator^(s32x4 a, s32x4 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s32x4 operator^(s32x4 a, s32 b) { return {_mm_xor_si128(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator^(s32 a, s32x4 b) { return {_mm_xor_si128(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator^=(s32x4 &a, s32x4 b) { return a=a^b; }
	forceinline friend s32x4 &operator^=(s32x4 &a, s32 b) { return a=a^b; }
	forceinline friend s32x4 operator&(s32x4 a, s32x4 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s32x4 operator&(s32x4 a, s32 b) { return {_mm_and_si128(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator&(s32 a, s32x4 b) { return {_mm_and_si128(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator&=(s32x4 &a, s32x4 b) { return a=a&b; }
	forceinline friend s32x4 &operator&=(s32x4 &a, s32 b) { return a=a&b; }
	forceinline friend s32x4 operator|(s32x4 a, s32x4 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s32x4 operator|(s32x4 a, s32 b) { return {_mm_or_si128(a.m, _mm_set1_epi32(b))}; }
	forceinline friend s32x4 operator|(s32 a, s32x4 b) { return {_mm_or_si128(_mm_set1_epi32(a), b.m)}; }
	forceinline friend s32x4 &operator|=(s32x4 &a, s32x4 b) { return a=a|b; }
	forceinline friend s32x4 &operator|=(s32x4 &a, s32 b) { return a=a|b; }
	forceinline friend s32x4 operator<<(s32x4 a, s32 b) { return {_mm_slli_epi32(a.m, b)}; }
	forceinline friend s32x4 operator>>(s32x4 a, s32 b) { return {_mm_srai_epi32(a.m, b)}; }
	forceinline operator u32x4() const;
	forceinline operator f32x4() const;
};

forceinline s32x4 S32x4(s32 a) { return {_mm_set1_epi32(a)}; }
forceinline s32x4 S32x4(s32 _0,s32 _1,s32 _2,s32 _3) { return {_mm_setr_epi32(_0,_1,_2,_3)}; }


union s64x2 {
	__m128i m;
	s64 s[2];
	forceinline s64 operator[](umm i) const { return s[i]; }
	forceinline s64 &operator[](umm i) { return s[i]; }
	forceinline friend s64x2 operator+(s64x2 a) { return a; }
	forceinline friend s64x2 operator-(s64x2 a) { return s64x2{} - a; }
	forceinline friend s64x2 operator~(s64x2 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s64x2 operator+(s64x2 a, s64x2 b) { return {_mm_add_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator+(s64x2 a, s64 b) { return {_mm_add_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator+(s64 a, s64x2 b) { return {_mm_add_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator+=(s64x2 &a, s64x2 b) { return a=a+b; }
	forceinline friend s64x2 &operator+=(s64x2 &a, s64 b) { return a=a+b; }
	forceinline friend s64x2 operator-(s64x2 a, s64x2 b) { return {_mm_sub_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator-(s64x2 a, s64 b) { return {_mm_sub_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator-(s64 a, s64x2 b) { return {_mm_sub_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator-=(s64x2 &a, s64x2 b) { return a=a-b; }
	forceinline friend s64x2 &operator-=(s64x2 &a, s64 b) { return a=a-b; }
	forceinline friend s64x2 operator*(s64x2 a, s64x2 b) { return {_mm_mullo_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator*(s64x2 a, s64 b) { return {_mm_mullo_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator*(s64 a, s64x2 b) { return {_mm_mullo_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator*=(s64x2 &a, s64x2 b) { return a=a*b; }
	forceinline friend s64x2 &operator*=(s64x2 &a, s64 b) { return a=a*b; }
	forceinline friend s64x2 operator/(s64x2 a, s64x2 b) { return {_mm_div_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator/(s64x2 a, s64 b) { return {_mm_div_epi64(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator/(s64 a, s64x2 b) { return {_mm_div_epi64(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator/=(s64x2 &a, s64x2 b) { return a=a/b; }
	forceinline friend s64x2 &operator/=(s64x2 &a, s64 b) { return a=a/b; }
	forceinline friend s64x2 operator^(s64x2 a, s64x2 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s64x2 operator^(s64x2 a, s64 b) { return {_mm_xor_si128(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator^(s64 a, s64x2 b) { return {_mm_xor_si128(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator^=(s64x2 &a, s64x2 b) { return a=a^b; }
	forceinline friend s64x2 &operator^=(s64x2 &a, s64 b) { return a=a^b; }
	forceinline friend s64x2 operator&(s64x2 a, s64x2 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s64x2 operator&(s64x2 a, s64 b) { return {_mm_and_si128(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator&(s64 a, s64x2 b) { return {_mm_and_si128(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator&=(s64x2 &a, s64x2 b) { return a=a&b; }
	forceinline friend s64x2 &operator&=(s64x2 &a, s64 b) { return a=a&b; }
	forceinline friend s64x2 operator|(s64x2 a, s64x2 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s64x2 operator|(s64x2 a, s64 b) { return {_mm_or_si128(a.m, _mm_set1_epi64x(b))}; }
	forceinline friend s64x2 operator|(s64 a, s64x2 b) { return {_mm_or_si128(_mm_set1_epi64x(a), b.m)}; }
	forceinline friend s64x2 &operator|=(s64x2 &a, s64x2 b) { return a=a|b; }
	forceinline friend s64x2 &operator|=(s64x2 &a, s64 b) { return a=a|b; }
	forceinline friend s64x2 operator<<(s64x2 a, s64 b) { return {_mm_slli_epi64(a.m, b)}; }
	forceinline friend s64x2 operator>>(s64x2 a, s64 b) { return {_mm_srai_epi64(a.m, b)}; }
	forceinline operator u64x2() const;
	forceinline operator f64x2() const;
};

forceinline s64x2 S64x2(s64 a) { return {_mm_set1_epi64x(a)}; }
forceinline s64x2 S64x2(s64 _0,s64 _1) { return {_mm_setr_epi64x(_0,_1)}; }


union f32x4 {
	__m128 m;
	f32 s[4];
	forceinline f32 operator[](umm i) const { return s[i]; }
	forceinline f32 &operator[](umm i) { return s[i]; }
	forceinline friend f32x4 operator+(f32x4 a) { return a; }
	forceinline friend f32x4 operator-(f32x4 a) { return f32x4{} - a; }
	forceinline friend f32x4 operator+(f32x4 a, f32x4 b) { return {_mm_add_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator+(f32x4 a, f32 b) { return {_mm_add_ps(a.m, _mm_set1_ps(b))}; }
	forceinline friend f32x4 operator+(f32 a, f32x4 b) { return {_mm_add_ps(_mm_set1_ps(a), b.m)}; }
	forceinline friend f32x4 &operator+=(f32x4 &a, f32x4 b) { return a=a+b; }
	forceinline friend f32x4 &operator+=(f32x4 &a, f32 b) { return a=a+b; }
	forceinline friend f32x4 operator-(f32x4 a, f32x4 b) { return {_mm_sub_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator-(f32x4 a, f32 b) { return {_mm_sub_ps(a.m, _mm_set1_ps(b))}; }
	forceinline friend f32x4 operator-(f32 a, f32x4 b) { return {_mm_sub_ps(_mm_set1_ps(a), b.m)}; }
	forceinline friend f32x4 &operator-=(f32x4 &a, f32x4 b) { return a=a-b; }
	forceinline friend f32x4 &operator-=(f32x4 &a, f32 b) { return a=a-b; }
	forceinline friend f32x4 operator*(f32x4 a, f32x4 b) { return {_mm_mul_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator*(f32x4 a, f32 b) { return {_mm_mul_ps(a.m, _mm_set1_ps(b))}; }
	forceinline friend f32x4 operator*(f32 a, f32x4 b) { return {_mm_mul_ps(_mm_set1_ps(a), b.m)}; }
	forceinline friend f32x4 &operator*=(f32x4 &a, f32x4 b) { return a=a*b; }
	forceinline friend f32x4 &operator*=(f32x4 &a, f32 b) { return a=a*b; }
	forceinline friend f32x4 operator/(f32x4 a, f32x4 b) { return {_mm_div_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator/(f32x4 a, f32 b) { return {_mm_div_ps(a.m, _mm_set1_ps(b))}; }
	forceinline friend f32x4 operator/(f32 a, f32x4 b) { return {_mm_div_ps(_mm_set1_ps(a), b.m)}; }
	forceinline friend f32x4 &operator/=(f32x4 &a, f32x4 b) { return a=a/b; }
	forceinline friend f32x4 &operator/=(f32x4 &a, f32 b) { return a=a/b; }
	forceinline operator u32x4() const;
	forceinline operator s32x4() const;
};

forceinline f32x4 F32x4(f32 a) { return {_mm_set1_ps(a)}; }
forceinline f32x4 F32x4(f32 _0,f32 _1,f32 _2,f32 _3) { return {_mm_setr_ps(_0,_1,_2,_3)}; }

forceinline f32x4 fmadd(f32x4 a, f32x4 b, f32x4 c) { return {_mm_fmadd_ps(a.m, b.m, c.m)}; }

union f64x2 {
	__m128d m;
	f64 s[2];
	forceinline f64 operator[](umm i) const { return s[i]; }
	forceinline f64 &operator[](umm i) { return s[i]; }
	forceinline friend f64x2 operator+(f64x2 a) { return a; }
	forceinline friend f64x2 operator-(f64x2 a) { return f64x2{} - a; }
	forceinline friend f64x2 operator+(f64x2 a, f64x2 b) { return {_mm_add_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator+(f64x2 a, f64 b) { return {_mm_add_pd(a.m, _mm_set1_pd(b))}; }
	forceinline friend f64x2 operator+(f64 a, f64x2 b) { return {_mm_add_pd(_mm_set1_pd(a), b.m)}; }
	forceinline friend f64x2 &operator+=(f64x2 &a, f64x2 b) { return a=a+b; }
	forceinline friend f64x2 &operator+=(f64x2 &a, f64 b) { return a=a+b; }
	forceinline friend f64x2 operator-(f64x2 a, f64x2 b) { return {_mm_sub_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator-(f64x2 a, f64 b) { return {_mm_sub_pd(a.m, _mm_set1_pd(b))}; }
	forceinline friend f64x2 operator-(f64 a, f64x2 b) { return {_mm_sub_pd(_mm_set1_pd(a), b.m)}; }
	forceinline friend f64x2 &operator-=(f64x2 &a, f64x2 b) { return a=a-b; }
	forceinline friend f64x2 &operator-=(f64x2 &a, f64 b) { return a=a-b; }
	forceinline friend f64x2 operator*(f64x2 a, f64x2 b) { return {_mm_mul_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator*(f64x2 a, f64 b) { return {_mm_mul_pd(a.m, _mm_set1_pd(b))}; }
	forceinline friend f64x2 operator*(f64 a, f64x2 b) { return {_mm_mul_pd(_mm_set1_pd(a), b.m)}; }
	forceinline friend f64x2 &operator*=(f64x2 &a, f64x2 b) { return a=a*b; }
	forceinline friend f64x2 &operator*=(f64x2 &a, f64 b) { return a=a*b; }
	forceinline friend f64x2 operator/(f64x2 a, f64x2 b) { return {_mm_div_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator/(f64x2 a, f64 b) { return {_mm_div_pd(a.m, _mm_set1_pd(b))}; }
	forceinline friend f64x2 operator/(f64 a, f64x2 b) { return {_mm_div_pd(_mm_set1_pd(a), b.m)}; }
	forceinline friend f64x2 &operator/=(f64x2 &a, f64x2 b) { return a=a/b; }
	forceinline friend f64x2 &operator/=(f64x2 &a, f64 b) { return a=a/b; }
	forceinline operator u64x2() const;
	forceinline operator s64x2() const;
};

forceinline f64x2 F64x2(f64 a) { return {_mm_set1_pd(a)}; }
forceinline f64x2 F64x2(f64 _0,f64 _1) { return {_mm_setr_pd(_0,_1)}; }

forceinline f64x2 fmadd(f64x2 a, f64x2 b, f64x2 c) { return {_mm_fmadd_pd(a.m, b.m, c.m)}; }

forceinline u8x16::operator s8x16() const { return {m}; }
forceinline u16x8::operator s16x8() const { return {m}; }
forceinline u32x4::operator s32x4() const { return {m}; }
forceinline u32x4::operator f32x4() const {
#if ARCH_AVX512
	return {_mm_cvtepu32_ps(m)}; }
#else
	f32x4 r;
	r[0] = (f32)s[0];
	r[1] = (f32)s[1];
	r[2] = (f32)s[2];
	r[3] = (f32)s[3];
	return r;
#endif
}
forceinline u64x2::operator s64x2() const { return {m}; }
forceinline u64x2::operator f64x2() const {
#if ARCH_AVX512
	return {_mm_cvtepu64_pd(m)}; }
#else
	f64x2 r;
	r[0] = (f64)s[0];
	r[1] = (f64)s[1];
	return r;
#endif
}
forceinline s8x16::operator u8x16() const { return {m}; }
forceinline s16x8::operator u16x8() const { return {m}; }
forceinline s32x4::operator u32x4() const { return {m}; }
forceinline s32x4::operator f32x4() const { return {_mm_cvtepi32_ps(m)}; }
forceinline s64x2::operator u64x2() const { return {m}; }
forceinline s64x2::operator f64x2() const { return {_mm_cvtepi64_pd(m)}; }
forceinline f32x4::operator u32x4() const { return {_mm_cvtps_epu32(m)}; }
forceinline f32x4::operator s32x4() const { return {_mm_cvtps_epi32(m)}; }
forceinline f64x2::operator u64x2() const { return {_mm_cvtpd_epu64(m)}; }
forceinline f64x2::operator s64x2() const { return {_mm_cvtpd_epi64(m)}; }

#if ARCH_AVX
union u8x32 {
	__m256i m;
	u8x16 x16[2];
	u8 s[32];
	forceinline u8 operator[](umm i) const { return s[i]; }
	forceinline u8 &operator[](umm i) { return s[i]; }
	forceinline friend u8x32 operator+(u8x32 a) { return a; }
	forceinline friend u8x32 operator-(u8x32 a) { return u8x32{} - a; }
	forceinline friend u8x32 operator~(u8x32 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend u8x32 operator+(u8x32 a, u8x32 b) { return {_mm256_add_epi8(a.m, b.m)}; }
	forceinline friend u8x32 operator+(u8x32 a, u8 b) { return {_mm256_add_epi8(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend u8x32 operator+(u8 a, u8x32 b) { return {_mm256_add_epi8(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend u8x32 &operator+=(u8x32 &a, u8x32 b) { return a=a+b; }
	forceinline friend u8x32 &operator+=(u8x32 &a, u8 b) { return a=a+b; }
	forceinline friend u8x32 operator-(u8x32 a, u8x32 b) { return {_mm256_sub_epi8(a.m, b.m)}; }
	forceinline friend u8x32 operator-(u8x32 a, u8 b) { return {_mm256_sub_epi8(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend u8x32 operator-(u8 a, u8x32 b) { return {_mm256_sub_epi8(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend u8x32 &operator-=(u8x32 &a, u8x32 b) { return a=a-b; }
	forceinline friend u8x32 &operator-=(u8x32 &a, u8 b) { return a=a-b; }
	forceinline friend u8x32 operator*(u8x32 a, u8x32 b) {((u8*)&a)[0] *= ((u8*)&b)[0]; ((u8*)&a)[1] *= ((u8*)&b)[1]; ((u8*)&a)[2] *= ((u8*)&b)[2]; ((u8*)&a)[3] *= ((u8*)&b)[3]; ((u8*)&a)[4] *= ((u8*)&b)[4]; ((u8*)&a)[5] *= ((u8*)&b)[5]; ((u8*)&a)[6] *= ((u8*)&b)[6]; ((u8*)&a)[7] *= ((u8*)&b)[7]; ((u8*)&a)[8] *= ((u8*)&b)[8]; ((u8*)&a)[9] *= ((u8*)&b)[9]; ((u8*)&a)[10] *= ((u8*)&b)[10]; ((u8*)&a)[11] *= ((u8*)&b)[11]; ((u8*)&a)[12] *= ((u8*)&b)[12]; ((u8*)&a)[13] *= ((u8*)&b)[13]; ((u8*)&a)[14] *= ((u8*)&b)[14]; ((u8*)&a)[15] *= ((u8*)&b)[15]; ((u8*)&a)[16] *= ((u8*)&b)[16]; ((u8*)&a)[17] *= ((u8*)&b)[17]; ((u8*)&a)[18] *= ((u8*)&b)[18]; ((u8*)&a)[19] *= ((u8*)&b)[19]; ((u8*)&a)[20] *= ((u8*)&b)[20]; ((u8*)&a)[21] *= ((u8*)&b)[21]; ((u8*)&a)[22] *= ((u8*)&b)[22]; ((u8*)&a)[23] *= ((u8*)&b)[23]; ((u8*)&a)[24] *= ((u8*)&b)[24]; ((u8*)&a)[25] *= ((u8*)&b)[25]; ((u8*)&a)[26] *= ((u8*)&b)[26]; ((u8*)&a)[27] *= ((u8*)&b)[27]; ((u8*)&a)[28] *= ((u8*)&b)[28]; ((u8*)&a)[29] *= ((u8*)&b)[29]; ((u8*)&a)[30] *= ((u8*)&b)[30]; ((u8*)&a)[31] *= ((u8*)&b)[31]; return a; }
	forceinline friend u8x32 operator*(u8x32 a, u8 b) {((u8*)&a)[0] *= b; ((u8*)&a)[1] *= b; ((u8*)&a)[2] *= b; ((u8*)&a)[3] *= b; ((u8*)&a)[4] *= b; ((u8*)&a)[5] *= b; ((u8*)&a)[6] *= b; ((u8*)&a)[7] *= b; ((u8*)&a)[8] *= b; ((u8*)&a)[9] *= b; ((u8*)&a)[10] *= b; ((u8*)&a)[11] *= b; ((u8*)&a)[12] *= b; ((u8*)&a)[13] *= b; ((u8*)&a)[14] *= b; ((u8*)&a)[15] *= b; ((u8*)&a)[16] *= b; ((u8*)&a)[17] *= b; ((u8*)&a)[18] *= b; ((u8*)&a)[19] *= b; ((u8*)&a)[20] *= b; ((u8*)&a)[21] *= b; ((u8*)&a)[22] *= b; ((u8*)&a)[23] *= b; ((u8*)&a)[24] *= b; ((u8*)&a)[25] *= b; ((u8*)&a)[26] *= b; ((u8*)&a)[27] *= b; ((u8*)&a)[28] *= b; ((u8*)&a)[29] *= b; ((u8*)&a)[30] *= b; ((u8*)&a)[31] *= b; return a; }
	forceinline friend u8x32 operator*(u8 a, u8x32 b) {((u8*)&b)[0] *= a; ((u8*)&b)[1] *= a; ((u8*)&b)[2] *= a; ((u8*)&b)[3] *= a; ((u8*)&b)[4] *= a; ((u8*)&b)[5] *= a; ((u8*)&b)[6] *= a; ((u8*)&b)[7] *= a; ((u8*)&b)[8] *= a; ((u8*)&b)[9] *= a; ((u8*)&b)[10] *= a; ((u8*)&b)[11] *= a; ((u8*)&b)[12] *= a; ((u8*)&b)[13] *= a; ((u8*)&b)[14] *= a; ((u8*)&b)[15] *= a; ((u8*)&b)[16] *= a; ((u8*)&b)[17] *= a; ((u8*)&b)[18] *= a; ((u8*)&b)[19] *= a; ((u8*)&b)[20] *= a; ((u8*)&b)[21] *= a; ((u8*)&b)[22] *= a; ((u8*)&b)[23] *= a; ((u8*)&b)[24] *= a; ((u8*)&b)[25] *= a; ((u8*)&b)[26] *= a; ((u8*)&b)[27] *= a; ((u8*)&b)[28] *= a; ((u8*)&b)[29] *= a; ((u8*)&b)[30] *= a; ((u8*)&b)[31] *= a; return b; }
	forceinline friend u8x32 &operator*=(u8x32 &a, u8x32 b) { return a=a*b; }
	forceinline friend u8x32 &operator*=(u8x32 &a, u8 b) { return a=a*b; }
	forceinline friend u8x32 operator/(u8x32 a, u8x32 b) { return {_mm256_div_epu8(a.m, b.m)}; }
	forceinline friend u8x32 operator/(u8x32 a, u8 b) { return {_mm256_div_epu8(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend u8x32 operator/(u8 a, u8x32 b) { return {_mm256_div_epu8(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend u8x32 &operator/=(u8x32 &a, u8x32 b) { return a=a/b; }
	forceinline friend u8x32 &operator/=(u8x32 &a, u8 b) { return a=a/b; }
	forceinline friend u8x32 operator^(u8x32 a, u8x32 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend u8x32 operator^(u8x32 a, u8 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend u8x32 operator^(u8 a, u8x32 b) { return {_mm256_xor_si256(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend u8x32 &operator^=(u8x32 &a, u8x32 b) { return a=a^b; }
	forceinline friend u8x32 &operator^=(u8x32 &a, u8 b) { return a=a^b; }
	forceinline friend u8x32 operator&(u8x32 a, u8x32 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend u8x32 operator&(u8x32 a, u8 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend u8x32 operator&(u8 a, u8x32 b) { return {_mm256_and_si256(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend u8x32 &operator&=(u8x32 &a, u8x32 b) { return a=a&b; }
	forceinline friend u8x32 &operator&=(u8x32 &a, u8 b) { return a=a&b; }
	forceinline friend u8x32 operator|(u8x32 a, u8x32 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend u8x32 operator|(u8x32 a, u8 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend u8x32 operator|(u8 a, u8x32 b) { return {_mm256_or_si256(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend u8x32 &operator|=(u8x32 &a, u8x32 b) { return a=a|b; }
	forceinline friend u8x32 &operator|=(u8x32 &a, u8 b) { return a=a|b; }
	forceinline operator s8x32() const;
};

forceinline u8x32 U8x32(u8 a) { return {_mm256_set1_epi8(a)}; }
forceinline u8x32 U8x32(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15,u8 _16,u8 _17,u8 _18,u8 _19,u8 _20,u8 _21,u8 _22,u8 _23,u8 _24,u8 _25,u8 _26,u8 _27,u8 _28,u8 _29,u8 _30,u8 _31) { return {_mm256_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }


union u16x16 {
	__m256i m;
	u16x8 x8[2];
	u16 s[16];
	forceinline u16 operator[](umm i) const { return s[i]; }
	forceinline u16 &operator[](umm i) { return s[i]; }
	forceinline friend u16x16 operator+(u16x16 a) { return a; }
	forceinline friend u16x16 operator-(u16x16 a) { return u16x16{} - a; }
	forceinline friend u16x16 operator~(u16x16 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend u16x16 operator+(u16x16 a, u16x16 b) { return {_mm256_add_epi16(a.m, b.m)}; }
	forceinline friend u16x16 operator+(u16x16 a, u16 b) { return {_mm256_add_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator+(u16 a, u16x16 b) { return {_mm256_add_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator+=(u16x16 &a, u16x16 b) { return a=a+b; }
	forceinline friend u16x16 &operator+=(u16x16 &a, u16 b) { return a=a+b; }
	forceinline friend u16x16 operator-(u16x16 a, u16x16 b) { return {_mm256_sub_epi16(a.m, b.m)}; }
	forceinline friend u16x16 operator-(u16x16 a, u16 b) { return {_mm256_sub_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator-(u16 a, u16x16 b) { return {_mm256_sub_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator-=(u16x16 &a, u16x16 b) { return a=a-b; }
	forceinline friend u16x16 &operator-=(u16x16 &a, u16 b) { return a=a-b; }
	forceinline friend u16x16 operator*(u16x16 a, u16x16 b) { return {_mm256_mullo_epi16(a.m, b.m)}; }
	forceinline friend u16x16 operator*(u16x16 a, u16 b) { return {_mm256_mullo_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator*(u16 a, u16x16 b) { return {_mm256_mullo_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator*=(u16x16 &a, u16x16 b) { return a=a*b; }
	forceinline friend u16x16 &operator*=(u16x16 &a, u16 b) { return a=a*b; }
	forceinline friend u16x16 operator/(u16x16 a, u16x16 b) { return {_mm256_div_epu16(a.m, b.m)}; }
	forceinline friend u16x16 operator/(u16x16 a, u16 b) { return {_mm256_div_epu16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator/(u16 a, u16x16 b) { return {_mm256_div_epu16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator/=(u16x16 &a, u16x16 b) { return a=a/b; }
	forceinline friend u16x16 &operator/=(u16x16 &a, u16 b) { return a=a/b; }
	forceinline friend u16x16 operator^(u16x16 a, u16x16 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend u16x16 operator^(u16x16 a, u16 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator^(u16 a, u16x16 b) { return {_mm256_xor_si256(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator^=(u16x16 &a, u16x16 b) { return a=a^b; }
	forceinline friend u16x16 &operator^=(u16x16 &a, u16 b) { return a=a^b; }
	forceinline friend u16x16 operator&(u16x16 a, u16x16 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend u16x16 operator&(u16x16 a, u16 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator&(u16 a, u16x16 b) { return {_mm256_and_si256(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator&=(u16x16 &a, u16x16 b) { return a=a&b; }
	forceinline friend u16x16 &operator&=(u16x16 &a, u16 b) { return a=a&b; }
	forceinline friend u16x16 operator|(u16x16 a, u16x16 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend u16x16 operator|(u16x16 a, u16 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend u16x16 operator|(u16 a, u16x16 b) { return {_mm256_or_si256(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend u16x16 &operator|=(u16x16 &a, u16x16 b) { return a=a|b; }
	forceinline friend u16x16 &operator|=(u16x16 &a, u16 b) { return a=a|b; }
	forceinline friend u16x16 operator<<(u16x16 a, u16 b) { return {_mm256_slli_epi16(a.m, b)}; }
	forceinline friend u16x16 operator>>(u16x16 a, u16 b) { return {_mm256_srli_epi16(a.m, b)}; }
	forceinline operator s16x16() const;
};

forceinline u16x16 U16x16(u16 a) { return {_mm256_set1_epi16(a)}; }
forceinline u16x16 U16x16(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7,u16 _8,u16 _9,u16 _10,u16 _11,u16 _12,u16 _13,u16 _14,u16 _15) { return {_mm256_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }


union u32x8 {
	__m256i m;
	u32x4 x4[2];
	u32 s[8];
	forceinline u32 operator[](umm i) const { return s[i]; }
	forceinline u32 &operator[](umm i) { return s[i]; }
	forceinline friend u32x8 operator+(u32x8 a) { return a; }
	forceinline friend u32x8 operator-(u32x8 a) { return u32x8{} - a; }
	forceinline friend u32x8 operator~(u32x8 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend u32x8 operator+(u32x8 a, u32x8 b) { return {_mm256_add_epi32(a.m, b.m)}; }
	forceinline friend u32x8 operator+(u32x8 a, u32 b) { return {_mm256_add_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator+(u32 a, u32x8 b) { return {_mm256_add_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator+=(u32x8 &a, u32x8 b) { return a=a+b; }
	forceinline friend u32x8 &operator+=(u32x8 &a, u32 b) { return a=a+b; }
	forceinline friend u32x8 operator-(u32x8 a, u32x8 b) { return {_mm256_sub_epi32(a.m, b.m)}; }
	forceinline friend u32x8 operator-(u32x8 a, u32 b) { return {_mm256_sub_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator-(u32 a, u32x8 b) { return {_mm256_sub_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator-=(u32x8 &a, u32x8 b) { return a=a-b; }
	forceinline friend u32x8 &operator-=(u32x8 &a, u32 b) { return a=a-b; }
	forceinline friend u32x8 operator*(u32x8 a, u32x8 b) { return {_mm256_mullo_epi32(a.m, b.m)}; }
	forceinline friend u32x8 operator*(u32x8 a, u32 b) { return {_mm256_mullo_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator*(u32 a, u32x8 b) { return {_mm256_mullo_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator*=(u32x8 &a, u32x8 b) { return a=a*b; }
	forceinline friend u32x8 &operator*=(u32x8 &a, u32 b) { return a=a*b; }
	forceinline friend u32x8 operator/(u32x8 a, u32x8 b) { return {_mm256_div_epu32(a.m, b.m)}; }
	forceinline friend u32x8 operator/(u32x8 a, u32 b) { return {_mm256_div_epu32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator/(u32 a, u32x8 b) { return {_mm256_div_epu32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator/=(u32x8 &a, u32x8 b) { return a=a/b; }
	forceinline friend u32x8 &operator/=(u32x8 &a, u32 b) { return a=a/b; }
	forceinline friend u32x8 operator^(u32x8 a, u32x8 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend u32x8 operator^(u32x8 a, u32 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator^(u32 a, u32x8 b) { return {_mm256_xor_si256(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator^=(u32x8 &a, u32x8 b) { return a=a^b; }
	forceinline friend u32x8 &operator^=(u32x8 &a, u32 b) { return a=a^b; }
	forceinline friend u32x8 operator&(u32x8 a, u32x8 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend u32x8 operator&(u32x8 a, u32 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator&(u32 a, u32x8 b) { return {_mm256_and_si256(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator&=(u32x8 &a, u32x8 b) { return a=a&b; }
	forceinline friend u32x8 &operator&=(u32x8 &a, u32 b) { return a=a&b; }
	forceinline friend u32x8 operator|(u32x8 a, u32x8 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend u32x8 operator|(u32x8 a, u32 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend u32x8 operator|(u32 a, u32x8 b) { return {_mm256_or_si256(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend u32x8 &operator|=(u32x8 &a, u32x8 b) { return a=a|b; }
	forceinline friend u32x8 &operator|=(u32x8 &a, u32 b) { return a=a|b; }
	forceinline friend u32x8 operator<<(u32x8 a, u32 b) { return {_mm256_slli_epi32(a.m, b)}; }
	forceinline friend u32x8 operator>>(u32x8 a, u32 b) { return {_mm256_srli_epi32(a.m, b)}; }
	forceinline operator s32x8() const;
	forceinline operator f32x8() const;
};

forceinline u32x8 U32x8(u32 a) { return {_mm256_set1_epi32(a)}; }
forceinline u32x8 U32x8(u32 _0,u32 _1,u32 _2,u32 _3,u32 _4,u32 _5,u32 _6,u32 _7) { return {_mm256_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7)}; }


union u64x4 {
	__m256i m;
	u64x2 x2[2];
	u64 s[4];
	forceinline u64 operator[](umm i) const { return s[i]; }
	forceinline u64 &operator[](umm i) { return s[i]; }
	forceinline friend u64x4 operator+(u64x4 a) { return a; }
	forceinline friend u64x4 operator-(u64x4 a) { return u64x4{} - a; }
	forceinline friend u64x4 operator~(u64x4 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend u64x4 operator+(u64x4 a, u64x4 b) { return {_mm256_add_epi64(a.m, b.m)}; }
	forceinline friend u64x4 operator+(u64x4 a, u64 b) { return {_mm256_add_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator+(u64 a, u64x4 b) { return {_mm256_add_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator+=(u64x4 &a, u64x4 b) { return a=a+b; }
	forceinline friend u64x4 &operator+=(u64x4 &a, u64 b) { return a=a+b; }
	forceinline friend u64x4 operator-(u64x4 a, u64x4 b) { return {_mm256_sub_epi64(a.m, b.m)}; }
	forceinline friend u64x4 operator-(u64x4 a, u64 b) { return {_mm256_sub_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator-(u64 a, u64x4 b) { return {_mm256_sub_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator-=(u64x4 &a, u64x4 b) { return a=a-b; }
	forceinline friend u64x4 &operator-=(u64x4 &a, u64 b) { return a=a-b; }
	forceinline friend u64x4 operator*(u64x4 a, u64x4 b) { return {_mm256_mullo_epi64(a.m, b.m)}; }
	forceinline friend u64x4 operator*(u64x4 a, u64 b) { return {_mm256_mullo_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator*(u64 a, u64x4 b) { return {_mm256_mullo_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator*=(u64x4 &a, u64x4 b) { return a=a*b; }
	forceinline friend u64x4 &operator*=(u64x4 &a, u64 b) { return a=a*b; }
	forceinline friend u64x4 operator/(u64x4 a, u64x4 b) { return {_mm256_div_epu64(a.m, b.m)}; }
	forceinline friend u64x4 operator/(u64x4 a, u64 b) { return {_mm256_div_epu64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator/(u64 a, u64x4 b) { return {_mm256_div_epu64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator/=(u64x4 &a, u64x4 b) { return a=a/b; }
	forceinline friend u64x4 &operator/=(u64x4 &a, u64 b) { return a=a/b; }
	forceinline friend u64x4 operator^(u64x4 a, u64x4 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend u64x4 operator^(u64x4 a, u64 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator^(u64 a, u64x4 b) { return {_mm256_xor_si256(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator^=(u64x4 &a, u64x4 b) { return a=a^b; }
	forceinline friend u64x4 &operator^=(u64x4 &a, u64 b) { return a=a^b; }
	forceinline friend u64x4 operator&(u64x4 a, u64x4 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend u64x4 operator&(u64x4 a, u64 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator&(u64 a, u64x4 b) { return {_mm256_and_si256(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator&=(u64x4 &a, u64x4 b) { return a=a&b; }
	forceinline friend u64x4 &operator&=(u64x4 &a, u64 b) { return a=a&b; }
	forceinline friend u64x4 operator|(u64x4 a, u64x4 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend u64x4 operator|(u64x4 a, u64 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend u64x4 operator|(u64 a, u64x4 b) { return {_mm256_or_si256(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend u64x4 &operator|=(u64x4 &a, u64x4 b) { return a=a|b; }
	forceinline friend u64x4 &operator|=(u64x4 &a, u64 b) { return a=a|b; }
	forceinline friend u64x4 operator<<(u64x4 a, u64 b) { return {_mm256_slli_epi64(a.m, b)}; }
	forceinline friend u64x4 operator>>(u64x4 a, u64 b) { return {_mm256_srli_epi64(a.m, b)}; }
	forceinline operator s64x4() const;
	forceinline operator f64x4() const;
};

forceinline u64x4 U64x4(u64 a) { return {_mm256_set1_epi64x(a)}; }
forceinline u64x4 U64x4(u64 _0,u64 _1,u64 _2,u64 _3) { return {_mm256_setr_epi64x(_0,_1,_2,_3)}; }


union s8x32 {
	__m256i m;
	s8x16 x16[2];
	s8 s[32];
	forceinline s8 operator[](umm i) const { return s[i]; }
	forceinline s8 &operator[](umm i) { return s[i]; }
	forceinline friend s8x32 operator+(s8x32 a) { return a; }
	forceinline friend s8x32 operator-(s8x32 a) { return s8x32{} - a; }
	forceinline friend s8x32 operator~(s8x32 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend s8x32 operator+(s8x32 a, s8x32 b) { return {_mm256_add_epi8(a.m, b.m)}; }
	forceinline friend s8x32 operator+(s8x32 a, s8 b) { return {_mm256_add_epi8(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend s8x32 operator+(s8 a, s8x32 b) { return {_mm256_add_epi8(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend s8x32 &operator+=(s8x32 &a, s8x32 b) { return a=a+b; }
	forceinline friend s8x32 &operator+=(s8x32 &a, s8 b) { return a=a+b; }
	forceinline friend s8x32 operator-(s8x32 a, s8x32 b) { return {_mm256_sub_epi8(a.m, b.m)}; }
	forceinline friend s8x32 operator-(s8x32 a, s8 b) { return {_mm256_sub_epi8(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend s8x32 operator-(s8 a, s8x32 b) { return {_mm256_sub_epi8(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend s8x32 &operator-=(s8x32 &a, s8x32 b) { return a=a-b; }
	forceinline friend s8x32 &operator-=(s8x32 &a, s8 b) { return a=a-b; }
	forceinline friend s8x32 operator*(s8x32 a, s8x32 b) {((s8*)&a)[0] *= ((s8*)&b)[0]; ((s8*)&a)[1] *= ((s8*)&b)[1]; ((s8*)&a)[2] *= ((s8*)&b)[2]; ((s8*)&a)[3] *= ((s8*)&b)[3]; ((s8*)&a)[4] *= ((s8*)&b)[4]; ((s8*)&a)[5] *= ((s8*)&b)[5]; ((s8*)&a)[6] *= ((s8*)&b)[6]; ((s8*)&a)[7] *= ((s8*)&b)[7]; ((s8*)&a)[8] *= ((s8*)&b)[8]; ((s8*)&a)[9] *= ((s8*)&b)[9]; ((s8*)&a)[10] *= ((s8*)&b)[10]; ((s8*)&a)[11] *= ((s8*)&b)[11]; ((s8*)&a)[12] *= ((s8*)&b)[12]; ((s8*)&a)[13] *= ((s8*)&b)[13]; ((s8*)&a)[14] *= ((s8*)&b)[14]; ((s8*)&a)[15] *= ((s8*)&b)[15]; ((s8*)&a)[16] *= ((s8*)&b)[16]; ((s8*)&a)[17] *= ((s8*)&b)[17]; ((s8*)&a)[18] *= ((s8*)&b)[18]; ((s8*)&a)[19] *= ((s8*)&b)[19]; ((s8*)&a)[20] *= ((s8*)&b)[20]; ((s8*)&a)[21] *= ((s8*)&b)[21]; ((s8*)&a)[22] *= ((s8*)&b)[22]; ((s8*)&a)[23] *= ((s8*)&b)[23]; ((s8*)&a)[24] *= ((s8*)&b)[24]; ((s8*)&a)[25] *= ((s8*)&b)[25]; ((s8*)&a)[26] *= ((s8*)&b)[26]; ((s8*)&a)[27] *= ((s8*)&b)[27]; ((s8*)&a)[28] *= ((s8*)&b)[28]; ((s8*)&a)[29] *= ((s8*)&b)[29]; ((s8*)&a)[30] *= ((s8*)&b)[30]; ((s8*)&a)[31] *= ((s8*)&b)[31]; return a; }
	forceinline friend s8x32 operator*(s8x32 a, s8 b) {((s8*)&a)[0] *= b; ((s8*)&a)[1] *= b; ((s8*)&a)[2] *= b; ((s8*)&a)[3] *= b; ((s8*)&a)[4] *= b; ((s8*)&a)[5] *= b; ((s8*)&a)[6] *= b; ((s8*)&a)[7] *= b; ((s8*)&a)[8] *= b; ((s8*)&a)[9] *= b; ((s8*)&a)[10] *= b; ((s8*)&a)[11] *= b; ((s8*)&a)[12] *= b; ((s8*)&a)[13] *= b; ((s8*)&a)[14] *= b; ((s8*)&a)[15] *= b; ((s8*)&a)[16] *= b; ((s8*)&a)[17] *= b; ((s8*)&a)[18] *= b; ((s8*)&a)[19] *= b; ((s8*)&a)[20] *= b; ((s8*)&a)[21] *= b; ((s8*)&a)[22] *= b; ((s8*)&a)[23] *= b; ((s8*)&a)[24] *= b; ((s8*)&a)[25] *= b; ((s8*)&a)[26] *= b; ((s8*)&a)[27] *= b; ((s8*)&a)[28] *= b; ((s8*)&a)[29] *= b; ((s8*)&a)[30] *= b; ((s8*)&a)[31] *= b; return a; }
	forceinline friend s8x32 operator*(s8 a, s8x32 b) {((s8*)&b)[0] *= a; ((s8*)&b)[1] *= a; ((s8*)&b)[2] *= a; ((s8*)&b)[3] *= a; ((s8*)&b)[4] *= a; ((s8*)&b)[5] *= a; ((s8*)&b)[6] *= a; ((s8*)&b)[7] *= a; ((s8*)&b)[8] *= a; ((s8*)&b)[9] *= a; ((s8*)&b)[10] *= a; ((s8*)&b)[11] *= a; ((s8*)&b)[12] *= a; ((s8*)&b)[13] *= a; ((s8*)&b)[14] *= a; ((s8*)&b)[15] *= a; ((s8*)&b)[16] *= a; ((s8*)&b)[17] *= a; ((s8*)&b)[18] *= a; ((s8*)&b)[19] *= a; ((s8*)&b)[20] *= a; ((s8*)&b)[21] *= a; ((s8*)&b)[22] *= a; ((s8*)&b)[23] *= a; ((s8*)&b)[24] *= a; ((s8*)&b)[25] *= a; ((s8*)&b)[26] *= a; ((s8*)&b)[27] *= a; ((s8*)&b)[28] *= a; ((s8*)&b)[29] *= a; ((s8*)&b)[30] *= a; ((s8*)&b)[31] *= a; return b; }
	forceinline friend s8x32 &operator*=(s8x32 &a, s8x32 b) { return a=a*b; }
	forceinline friend s8x32 &operator*=(s8x32 &a, s8 b) { return a=a*b; }
	forceinline friend s8x32 operator/(s8x32 a, s8x32 b) { return {_mm256_div_epi8(a.m, b.m)}; }
	forceinline friend s8x32 operator/(s8x32 a, s8 b) { return {_mm256_div_epi8(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend s8x32 operator/(s8 a, s8x32 b) { return {_mm256_div_epi8(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend s8x32 &operator/=(s8x32 &a, s8x32 b) { return a=a/b; }
	forceinline friend s8x32 &operator/=(s8x32 &a, s8 b) { return a=a/b; }
	forceinline friend s8x32 operator^(s8x32 a, s8x32 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend s8x32 operator^(s8x32 a, s8 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend s8x32 operator^(s8 a, s8x32 b) { return {_mm256_xor_si256(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend s8x32 &operator^=(s8x32 &a, s8x32 b) { return a=a^b; }
	forceinline friend s8x32 &operator^=(s8x32 &a, s8 b) { return a=a^b; }
	forceinline friend s8x32 operator&(s8x32 a, s8x32 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend s8x32 operator&(s8x32 a, s8 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend s8x32 operator&(s8 a, s8x32 b) { return {_mm256_and_si256(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend s8x32 &operator&=(s8x32 &a, s8x32 b) { return a=a&b; }
	forceinline friend s8x32 &operator&=(s8x32 &a, s8 b) { return a=a&b; }
	forceinline friend s8x32 operator|(s8x32 a, s8x32 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend s8x32 operator|(s8x32 a, s8 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi8(b))}; }
	forceinline friend s8x32 operator|(s8 a, s8x32 b) { return {_mm256_or_si256(_mm256_set1_epi8(a), b.m)}; }
	forceinline friend s8x32 &operator|=(s8x32 &a, s8x32 b) { return a=a|b; }
	forceinline friend s8x32 &operator|=(s8x32 &a, s8 b) { return a=a|b; }
	forceinline operator u8x32() const;
};

forceinline s8x32 S8x32(s8 a) { return {_mm256_set1_epi8(a)}; }
forceinline s8x32 S8x32(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15,s8 _16,s8 _17,s8 _18,s8 _19,s8 _20,s8 _21,s8 _22,s8 _23,s8 _24,s8 _25,s8 _26,s8 _27,s8 _28,s8 _29,s8 _30,s8 _31) { return {_mm256_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }


union s16x16 {
	__m256i m;
	s16x8 x8[2];
	s16 s[16];
	forceinline s16 operator[](umm i) const { return s[i]; }
	forceinline s16 &operator[](umm i) { return s[i]; }
	forceinline friend s16x16 operator+(s16x16 a) { return a; }
	forceinline friend s16x16 operator-(s16x16 a) { return s16x16{} - a; }
	forceinline friend s16x16 operator~(s16x16 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend s16x16 operator+(s16x16 a, s16x16 b) { return {_mm256_add_epi16(a.m, b.m)}; }
	forceinline friend s16x16 operator+(s16x16 a, s16 b) { return {_mm256_add_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator+(s16 a, s16x16 b) { return {_mm256_add_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator+=(s16x16 &a, s16x16 b) { return a=a+b; }
	forceinline friend s16x16 &operator+=(s16x16 &a, s16 b) { return a=a+b; }
	forceinline friend s16x16 operator-(s16x16 a, s16x16 b) { return {_mm256_sub_epi16(a.m, b.m)}; }
	forceinline friend s16x16 operator-(s16x16 a, s16 b) { return {_mm256_sub_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator-(s16 a, s16x16 b) { return {_mm256_sub_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator-=(s16x16 &a, s16x16 b) { return a=a-b; }
	forceinline friend s16x16 &operator-=(s16x16 &a, s16 b) { return a=a-b; }
	forceinline friend s16x16 operator*(s16x16 a, s16x16 b) { return {_mm256_mullo_epi16(a.m, b.m)}; }
	forceinline friend s16x16 operator*(s16x16 a, s16 b) { return {_mm256_mullo_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator*(s16 a, s16x16 b) { return {_mm256_mullo_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator*=(s16x16 &a, s16x16 b) { return a=a*b; }
	forceinline friend s16x16 &operator*=(s16x16 &a, s16 b) { return a=a*b; }
	forceinline friend s16x16 operator/(s16x16 a, s16x16 b) { return {_mm256_div_epi16(a.m, b.m)}; }
	forceinline friend s16x16 operator/(s16x16 a, s16 b) { return {_mm256_div_epi16(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator/(s16 a, s16x16 b) { return {_mm256_div_epi16(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator/=(s16x16 &a, s16x16 b) { return a=a/b; }
	forceinline friend s16x16 &operator/=(s16x16 &a, s16 b) { return a=a/b; }
	forceinline friend s16x16 operator^(s16x16 a, s16x16 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend s16x16 operator^(s16x16 a, s16 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator^(s16 a, s16x16 b) { return {_mm256_xor_si256(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator^=(s16x16 &a, s16x16 b) { return a=a^b; }
	forceinline friend s16x16 &operator^=(s16x16 &a, s16 b) { return a=a^b; }
	forceinline friend s16x16 operator&(s16x16 a, s16x16 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend s16x16 operator&(s16x16 a, s16 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator&(s16 a, s16x16 b) { return {_mm256_and_si256(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator&=(s16x16 &a, s16x16 b) { return a=a&b; }
	forceinline friend s16x16 &operator&=(s16x16 &a, s16 b) { return a=a&b; }
	forceinline friend s16x16 operator|(s16x16 a, s16x16 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend s16x16 operator|(s16x16 a, s16 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi16(b))}; }
	forceinline friend s16x16 operator|(s16 a, s16x16 b) { return {_mm256_or_si256(_mm256_set1_epi16(a), b.m)}; }
	forceinline friend s16x16 &operator|=(s16x16 &a, s16x16 b) { return a=a|b; }
	forceinline friend s16x16 &operator|=(s16x16 &a, s16 b) { return a=a|b; }
	forceinline friend s16x16 operator<<(s16x16 a, s16 b) { return {_mm256_slli_epi16(a.m, b)}; }
	forceinline friend s16x16 operator>>(s16x16 a, s16 b) { return {_mm256_srai_epi16(a.m, b)}; }
	forceinline operator u16x16() const;
};

forceinline s16x16 S16x16(s16 a) { return {_mm256_set1_epi16(a)}; }
forceinline s16x16 S16x16(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7,s16 _8,s16 _9,s16 _10,s16 _11,s16 _12,s16 _13,s16 _14,s16 _15) { return {_mm256_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }


union s32x8 {
	__m256i m;
	s32x4 x4[2];
	s32 s[8];
	forceinline s32 operator[](umm i) const { return s[i]; }
	forceinline s32 &operator[](umm i) { return s[i]; }
	forceinline friend s32x8 operator+(s32x8 a) { return a; }
	forceinline friend s32x8 operator-(s32x8 a) { return s32x8{} - a; }
	forceinline friend s32x8 operator~(s32x8 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend s32x8 operator+(s32x8 a, s32x8 b) { return {_mm256_add_epi32(a.m, b.m)}; }
	forceinline friend s32x8 operator+(s32x8 a, s32 b) { return {_mm256_add_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator+(s32 a, s32x8 b) { return {_mm256_add_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator+=(s32x8 &a, s32x8 b) { return a=a+b; }
	forceinline friend s32x8 &operator+=(s32x8 &a, s32 b) { return a=a+b; }
	forceinline friend s32x8 operator-(s32x8 a, s32x8 b) { return {_mm256_sub_epi32(a.m, b.m)}; }
	forceinline friend s32x8 operator-(s32x8 a, s32 b) { return {_mm256_sub_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator-(s32 a, s32x8 b) { return {_mm256_sub_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator-=(s32x8 &a, s32x8 b) { return a=a-b; }
	forceinline friend s32x8 &operator-=(s32x8 &a, s32 b) { return a=a-b; }
	forceinline friend s32x8 operator*(s32x8 a, s32x8 b) { return {_mm256_mullo_epi32(a.m, b.m)}; }
	forceinline friend s32x8 operator*(s32x8 a, s32 b) { return {_mm256_mullo_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator*(s32 a, s32x8 b) { return {_mm256_mullo_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator*=(s32x8 &a, s32x8 b) { return a=a*b; }
	forceinline friend s32x8 &operator*=(s32x8 &a, s32 b) { return a=a*b; }
	forceinline friend s32x8 operator/(s32x8 a, s32x8 b) { return {_mm256_div_epi32(a.m, b.m)}; }
	forceinline friend s32x8 operator/(s32x8 a, s32 b) { return {_mm256_div_epi32(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator/(s32 a, s32x8 b) { return {_mm256_div_epi32(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator/=(s32x8 &a, s32x8 b) { return a=a/b; }
	forceinline friend s32x8 &operator/=(s32x8 &a, s32 b) { return a=a/b; }
	forceinline friend s32x8 operator^(s32x8 a, s32x8 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend s32x8 operator^(s32x8 a, s32 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator^(s32 a, s32x8 b) { return {_mm256_xor_si256(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator^=(s32x8 &a, s32x8 b) { return a=a^b; }
	forceinline friend s32x8 &operator^=(s32x8 &a, s32 b) { return a=a^b; }
	forceinline friend s32x8 operator&(s32x8 a, s32x8 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend s32x8 operator&(s32x8 a, s32 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator&(s32 a, s32x8 b) { return {_mm256_and_si256(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator&=(s32x8 &a, s32x8 b) { return a=a&b; }
	forceinline friend s32x8 &operator&=(s32x8 &a, s32 b) { return a=a&b; }
	forceinline friend s32x8 operator|(s32x8 a, s32x8 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend s32x8 operator|(s32x8 a, s32 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi32(b))}; }
	forceinline friend s32x8 operator|(s32 a, s32x8 b) { return {_mm256_or_si256(_mm256_set1_epi32(a), b.m)}; }
	forceinline friend s32x8 &operator|=(s32x8 &a, s32x8 b) { return a=a|b; }
	forceinline friend s32x8 &operator|=(s32x8 &a, s32 b) { return a=a|b; }
	forceinline friend s32x8 operator<<(s32x8 a, s32 b) { return {_mm256_slli_epi32(a.m, b)}; }
	forceinline friend s32x8 operator>>(s32x8 a, s32 b) { return {_mm256_srai_epi32(a.m, b)}; }
	forceinline operator u32x8() const;
	forceinline operator f32x8() const;
};

forceinline s32x8 S32x8(s32 a) { return {_mm256_set1_epi32(a)}; }
forceinline s32x8 S32x8(s32 _0,s32 _1,s32 _2,s32 _3,s32 _4,s32 _5,s32 _6,s32 _7) { return {_mm256_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7)}; }


union s64x4 {
	__m256i m;
	s64x2 x2[2];
	s64 s[4];
	forceinline s64 operator[](umm i) const { return s[i]; }
	forceinline s64 &operator[](umm i) { return s[i]; }
	forceinline friend s64x4 operator+(s64x4 a) { return a; }
	forceinline friend s64x4 operator-(s64x4 a) { return s64x4{} - a; }
	forceinline friend s64x4 operator~(s64x4 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
	forceinline friend s64x4 operator+(s64x4 a, s64x4 b) { return {_mm256_add_epi64(a.m, b.m)}; }
	forceinline friend s64x4 operator+(s64x4 a, s64 b) { return {_mm256_add_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator+(s64 a, s64x4 b) { return {_mm256_add_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator+=(s64x4 &a, s64x4 b) { return a=a+b; }
	forceinline friend s64x4 &operator+=(s64x4 &a, s64 b) { return a=a+b; }
	forceinline friend s64x4 operator-(s64x4 a, s64x4 b) { return {_mm256_sub_epi64(a.m, b.m)}; }
	forceinline friend s64x4 operator-(s64x4 a, s64 b) { return {_mm256_sub_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator-(s64 a, s64x4 b) { return {_mm256_sub_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator-=(s64x4 &a, s64x4 b) { return a=a-b; }
	forceinline friend s64x4 &operator-=(s64x4 &a, s64 b) { return a=a-b; }
	forceinline friend s64x4 operator*(s64x4 a, s64x4 b) { return {_mm256_mullo_epi64(a.m, b.m)}; }
	forceinline friend s64x4 operator*(s64x4 a, s64 b) { return {_mm256_mullo_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator*(s64 a, s64x4 b) { return {_mm256_mullo_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator*=(s64x4 &a, s64x4 b) { return a=a*b; }
	forceinline friend s64x4 &operator*=(s64x4 &a, s64 b) { return a=a*b; }
	forceinline friend s64x4 operator/(s64x4 a, s64x4 b) { return {_mm256_div_epi64(a.m, b.m)}; }
	forceinline friend s64x4 operator/(s64x4 a, s64 b) { return {_mm256_div_epi64(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator/(s64 a, s64x4 b) { return {_mm256_div_epi64(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator/=(s64x4 &a, s64x4 b) { return a=a/b; }
	forceinline friend s64x4 &operator/=(s64x4 &a, s64 b) { return a=a/b; }
	forceinline friend s64x4 operator^(s64x4 a, s64x4 b) { return {_mm256_xor_si256(a.m, b.m)}; }
	forceinline friend s64x4 operator^(s64x4 a, s64 b) { return {_mm256_xor_si256(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator^(s64 a, s64x4 b) { return {_mm256_xor_si256(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator^=(s64x4 &a, s64x4 b) { return a=a^b; }
	forceinline friend s64x4 &operator^=(s64x4 &a, s64 b) { return a=a^b; }
	forceinline friend s64x4 operator&(s64x4 a, s64x4 b) { return {_mm256_and_si256(a.m, b.m)}; }
	forceinline friend s64x4 operator&(s64x4 a, s64 b) { return {_mm256_and_si256(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator&(s64 a, s64x4 b) { return {_mm256_and_si256(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator&=(s64x4 &a, s64x4 b) { return a=a&b; }
	forceinline friend s64x4 &operator&=(s64x4 &a, s64 b) { return a=a&b; }
	forceinline friend s64x4 operator|(s64x4 a, s64x4 b) { return {_mm256_or_si256(a.m, b.m)}; }
	forceinline friend s64x4 operator|(s64x4 a, s64 b) { return {_mm256_or_si256(a.m, _mm256_set1_epi64x(b))}; }
	forceinline friend s64x4 operator|(s64 a, s64x4 b) { return {_mm256_or_si256(_mm256_set1_epi64x(a), b.m)}; }
	forceinline friend s64x4 &operator|=(s64x4 &a, s64x4 b) { return a=a|b; }
	forceinline friend s64x4 &operator|=(s64x4 &a, s64 b) { return a=a|b; }
	forceinline friend s64x4 operator<<(s64x4 a, s64 b) { return {_mm256_slli_epi64(a.m, b)}; }
	forceinline friend s64x4 operator>>(s64x4 a, s64 b) { return {_mm256_srai_epi64(a.m, b)}; }
	forceinline operator u64x4() const;
	forceinline operator f64x4() const;
};

forceinline s64x4 S64x4(s64 a) { return {_mm256_set1_epi64x(a)}; }
forceinline s64x4 S64x4(s64 _0,s64 _1,s64 _2,s64 _3) { return {_mm256_setr_epi64x(_0,_1,_2,_3)}; }


union f32x8 {
	__m256 m;
	f32x4 x4[2];
	f32 s[8];
	forceinline f32 operator[](umm i) const { return s[i]; }
	forceinline f32 &operator[](umm i) { return s[i]; }
	forceinline friend f32x8 operator+(f32x8 a) { return a; }
	forceinline friend f32x8 operator-(f32x8 a) { return f32x8{} - a; }
	forceinline friend f32x8 operator+(f32x8 a, f32x8 b) { return {_mm256_add_ps(a.m, b.m)}; }
	forceinline friend f32x8 operator+(f32x8 a, f32 b) { return {_mm256_add_ps(a.m, _mm256_set1_ps(b))}; }
	forceinline friend f32x8 operator+(f32 a, f32x8 b) { return {_mm256_add_ps(_mm256_set1_ps(a), b.m)}; }
	forceinline friend f32x8 &operator+=(f32x8 &a, f32x8 b) { return a=a+b; }
	forceinline friend f32x8 &operator+=(f32x8 &a, f32 b) { return a=a+b; }
	forceinline friend f32x8 operator-(f32x8 a, f32x8 b) { return {_mm256_sub_ps(a.m, b.m)}; }
	forceinline friend f32x8 operator-(f32x8 a, f32 b) { return {_mm256_sub_ps(a.m, _mm256_set1_ps(b))}; }
	forceinline friend f32x8 operator-(f32 a, f32x8 b) { return {_mm256_sub_ps(_mm256_set1_ps(a), b.m)}; }
	forceinline friend f32x8 &operator-=(f32x8 &a, f32x8 b) { return a=a-b; }
	forceinline friend f32x8 &operator-=(f32x8 &a, f32 b) { return a=a-b; }
	forceinline friend f32x8 operator*(f32x8 a, f32x8 b) { return {_mm256_mul_ps(a.m, b.m)}; }
	forceinline friend f32x8 operator*(f32x8 a, f32 b) { return {_mm256_mul_ps(a.m, _mm256_set1_ps(b))}; }
	forceinline friend f32x8 operator*(f32 a, f32x8 b) { return {_mm256_mul_ps(_mm256_set1_ps(a), b.m)}; }
	forceinline friend f32x8 &operator*=(f32x8 &a, f32x8 b) { return a=a*b; }
	forceinline friend f32x8 &operator*=(f32x8 &a, f32 b) { return a=a*b; }
	forceinline friend f32x8 operator/(f32x8 a, f32x8 b) { return {_mm256_div_ps(a.m, b.m)}; }
	forceinline friend f32x8 operator/(f32x8 a, f32 b) { return {_mm256_div_ps(a.m, _mm256_set1_ps(b))}; }
	forceinline friend f32x8 operator/(f32 a, f32x8 b) { return {_mm256_div_ps(_mm256_set1_ps(a), b.m)}; }
	forceinline friend f32x8 &operator/=(f32x8 &a, f32x8 b) { return a=a/b; }
	forceinline friend f32x8 &operator/=(f32x8 &a, f32 b) { return a=a/b; }
	forceinline operator u32x8() const;
	forceinline operator s32x8() const;
};

forceinline f32x8 F32x8(f32 a) { return {_mm256_set1_ps(a)}; }
forceinline f32x8 F32x8(f32 _0,f32 _1,f32 _2,f32 _3,f32 _4,f32 _5,f32 _6,f32 _7) { return {_mm256_setr_ps(_0,_1,_2,_3,_4,_5,_6,_7)}; }

forceinline f32x8 fmadd(f32x8 a, f32x8 b, f32x8 c) { return {_mm256_fmadd_ps(a.m, b.m, c.m)}; }

union f64x4 {
	__m256d m;
	f64x2 x2[2];
	f64 s[4];
	forceinline f64 operator[](umm i) const { return s[i]; }
	forceinline f64 &operator[](umm i) { return s[i]; }
	forceinline friend f64x4 operator+(f64x4 a) { return a; }
	forceinline friend f64x4 operator-(f64x4 a) { return f64x4{} - a; }
	forceinline friend f64x4 operator+(f64x4 a, f64x4 b) { return {_mm256_add_pd(a.m, b.m)}; }
	forceinline friend f64x4 operator+(f64x4 a, f64 b) { return {_mm256_add_pd(a.m, _mm256_set1_pd(b))}; }
	forceinline friend f64x4 operator+(f64 a, f64x4 b) { return {_mm256_add_pd(_mm256_set1_pd(a), b.m)}; }
	forceinline friend f64x4 &operator+=(f64x4 &a, f64x4 b) { return a=a+b; }
	forceinline friend f64x4 &operator+=(f64x4 &a, f64 b) { return a=a+b; }
	forceinline friend f64x4 operator-(f64x4 a, f64x4 b) { return {_mm256_sub_pd(a.m, b.m)}; }
	forceinline friend f64x4 operator-(f64x4 a, f64 b) { return {_mm256_sub_pd(a.m, _mm256_set1_pd(b))}; }
	forceinline friend f64x4 operator-(f64 a, f64x4 b) { return {_mm256_sub_pd(_mm256_set1_pd(a), b.m)}; }
	forceinline friend f64x4 &operator-=(f64x4 &a, f64x4 b) { return a=a-b; }
	forceinline friend f64x4 &operator-=(f64x4 &a, f64 b) { return a=a-b; }
	forceinline friend f64x4 operator*(f64x4 a, f64x4 b) { return {_mm256_mul_pd(a.m, b.m)}; }
	forceinline friend f64x4 operator*(f64x4 a, f64 b) { return {_mm256_mul_pd(a.m, _mm256_set1_pd(b))}; }
	forceinline friend f64x4 operator*(f64 a, f64x4 b) { return {_mm256_mul_pd(_mm256_set1_pd(a), b.m)}; }
	forceinline friend f64x4 &operator*=(f64x4 &a, f64x4 b) { return a=a*b; }
	forceinline friend f64x4 &operator*=(f64x4 &a, f64 b) { return a=a*b; }
	forceinline friend f64x4 operator/(f64x4 a, f64x4 b) { return {_mm256_div_pd(a.m, b.m)}; }
	forceinline friend f64x4 operator/(f64x4 a, f64 b) { return {_mm256_div_pd(a.m, _mm256_set1_pd(b))}; }
	forceinline friend f64x4 operator/(f64 a, f64x4 b) { return {_mm256_div_pd(_mm256_set1_pd(a), b.m)}; }
	forceinline friend f64x4 &operator/=(f64x4 &a, f64x4 b) { return a=a/b; }
	forceinline friend f64x4 &operator/=(f64x4 &a, f64 b) { return a=a/b; }
	forceinline operator u64x4() const;
	forceinline operator s64x4() const;
};

forceinline f64x4 F64x4(f64 a) { return {_mm256_set1_pd(a)}; }
forceinline f64x4 F64x4(f64 _0,f64 _1,f64 _2,f64 _3) { return {_mm256_setr_pd(_0,_1,_2,_3)}; }

forceinline f64x4 fmadd(f64x4 a, f64x4 b, f64x4 c) { return {_mm256_fmadd_pd(a.m, b.m, c.m)}; }

forceinline u8x32::operator s8x32() const { return {m}; }
forceinline u16x16::operator s16x16() const { return {m}; }
forceinline u32x8::operator s32x8() const { return {m}; }
forceinline u32x8::operator f32x8() const {
#if ARCH_AVX512
	return {_mm256_cvtepu32_ps(m)}; }
#else
	f32x8 r;
	r[0] = (f32)s[0];
	r[1] = (f32)s[1];
	r[2] = (f32)s[2];
	r[3] = (f32)s[3];
	r[4] = (f32)s[4];
	r[5] = (f32)s[5];
	r[6] = (f32)s[6];
	r[7] = (f32)s[7];
	return r;
#endif
}
forceinline u64x4::operator s64x4() const { return {m}; }
forceinline u64x4::operator f64x4() const {
#if ARCH_AVX512
	return {_mm256_cvtepu64_pd(m)}; }
#else
	f64x4 r;
	r[0] = (f64)s[0];
	r[1] = (f64)s[1];
	r[2] = (f64)s[2];
	r[3] = (f64)s[3];
	return r;
#endif
}
forceinline s8x32::operator u8x32() const { return {m}; }
forceinline s16x16::operator u16x16() const { return {m}; }
forceinline s32x8::operator u32x8() const { return {m}; }
forceinline s32x8::operator f32x8() const { return {_mm256_cvtepi32_ps(m)}; }
forceinline s64x4::operator u64x4() const { return {m}; }
forceinline s64x4::operator f64x4() const { return {_mm256_cvtepi64_pd(m)}; }
forceinline f32x8::operator u32x8() const { return {_mm256_cvtps_epu32(m)}; }
forceinline f32x8::operator s32x8() const { return {_mm256_cvtps_epi32(m)}; }
forceinline f64x4::operator u64x4() const { return {_mm256_cvtpd_epu64(m)}; }
forceinline f64x4::operator s64x4() const { return {_mm256_cvtpd_epi64(m)}; }

#endif

#if ARCH_AVX512
union u8x64 {
	__m512i m;
	u8x32 x32[2];
	u8x16 x16[4];
	u8 s[64];
	forceinline u8 operator[](umm i) const { return s[i]; }
	forceinline u8 &operator[](umm i) { return s[i]; }
	forceinline friend u8x64 operator+(u8x64 a) { return a; }
	forceinline friend u8x64 operator-(u8x64 a) { return u8x64{} - a; }
	forceinline friend u8x64 operator~(u8x64 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend u8x64 operator+(u8x64 a, u8x64 b) { return {_mm512_add_epi8(a.m, b.m)}; }
	forceinline friend u8x64 operator+(u8x64 a, u8 b) { return {_mm512_add_epi8(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend u8x64 operator+(u8 a, u8x64 b) { return {_mm512_add_epi8(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend u8x64 &operator+=(u8x64 &a, u8x64 b) { return a=a+b; }
	forceinline friend u8x64 &operator+=(u8x64 &a, u8 b) { return a=a+b; }
	forceinline friend u8x64 operator-(u8x64 a, u8x64 b) { return {_mm512_sub_epi8(a.m, b.m)}; }
	forceinline friend u8x64 operator-(u8x64 a, u8 b) { return {_mm512_sub_epi8(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend u8x64 operator-(u8 a, u8x64 b) { return {_mm512_sub_epi8(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend u8x64 &operator-=(u8x64 &a, u8x64 b) { return a=a-b; }
	forceinline friend u8x64 &operator-=(u8x64 &a, u8 b) { return a=a-b; }
	forceinline friend u8x64 operator*(u8x64 a, u8x64 b) {((u8*)&a)[0] *= ((u8*)&b)[0]; ((u8*)&a)[1] *= ((u8*)&b)[1]; ((u8*)&a)[2] *= ((u8*)&b)[2]; ((u8*)&a)[3] *= ((u8*)&b)[3]; ((u8*)&a)[4] *= ((u8*)&b)[4]; ((u8*)&a)[5] *= ((u8*)&b)[5]; ((u8*)&a)[6] *= ((u8*)&b)[6]; ((u8*)&a)[7] *= ((u8*)&b)[7]; ((u8*)&a)[8] *= ((u8*)&b)[8]; ((u8*)&a)[9] *= ((u8*)&b)[9]; ((u8*)&a)[10] *= ((u8*)&b)[10]; ((u8*)&a)[11] *= ((u8*)&b)[11]; ((u8*)&a)[12] *= ((u8*)&b)[12]; ((u8*)&a)[13] *= ((u8*)&b)[13]; ((u8*)&a)[14] *= ((u8*)&b)[14]; ((u8*)&a)[15] *= ((u8*)&b)[15]; ((u8*)&a)[16] *= ((u8*)&b)[16]; ((u8*)&a)[17] *= ((u8*)&b)[17]; ((u8*)&a)[18] *= ((u8*)&b)[18]; ((u8*)&a)[19] *= ((u8*)&b)[19]; ((u8*)&a)[20] *= ((u8*)&b)[20]; ((u8*)&a)[21] *= ((u8*)&b)[21]; ((u8*)&a)[22] *= ((u8*)&b)[22]; ((u8*)&a)[23] *= ((u8*)&b)[23]; ((u8*)&a)[24] *= ((u8*)&b)[24]; ((u8*)&a)[25] *= ((u8*)&b)[25]; ((u8*)&a)[26] *= ((u8*)&b)[26]; ((u8*)&a)[27] *= ((u8*)&b)[27]; ((u8*)&a)[28] *= ((u8*)&b)[28]; ((u8*)&a)[29] *= ((u8*)&b)[29]; ((u8*)&a)[30] *= ((u8*)&b)[30]; ((u8*)&a)[31] *= ((u8*)&b)[31]; ((u8*)&a)[32] *= ((u8*)&b)[32]; ((u8*)&a)[33] *= ((u8*)&b)[33]; ((u8*)&a)[34] *= ((u8*)&b)[34]; ((u8*)&a)[35] *= ((u8*)&b)[35]; ((u8*)&a)[36] *= ((u8*)&b)[36]; ((u8*)&a)[37] *= ((u8*)&b)[37]; ((u8*)&a)[38] *= ((u8*)&b)[38]; ((u8*)&a)[39] *= ((u8*)&b)[39]; ((u8*)&a)[40] *= ((u8*)&b)[40]; ((u8*)&a)[41] *= ((u8*)&b)[41]; ((u8*)&a)[42] *= ((u8*)&b)[42]; ((u8*)&a)[43] *= ((u8*)&b)[43]; ((u8*)&a)[44] *= ((u8*)&b)[44]; ((u8*)&a)[45] *= ((u8*)&b)[45]; ((u8*)&a)[46] *= ((u8*)&b)[46]; ((u8*)&a)[47] *= ((u8*)&b)[47]; ((u8*)&a)[48] *= ((u8*)&b)[48]; ((u8*)&a)[49] *= ((u8*)&b)[49]; ((u8*)&a)[50] *= ((u8*)&b)[50]; ((u8*)&a)[51] *= ((u8*)&b)[51]; ((u8*)&a)[52] *= ((u8*)&b)[52]; ((u8*)&a)[53] *= ((u8*)&b)[53]; ((u8*)&a)[54] *= ((u8*)&b)[54]; ((u8*)&a)[55] *= ((u8*)&b)[55]; ((u8*)&a)[56] *= ((u8*)&b)[56]; ((u8*)&a)[57] *= ((u8*)&b)[57]; ((u8*)&a)[58] *= ((u8*)&b)[58]; ((u8*)&a)[59] *= ((u8*)&b)[59]; ((u8*)&a)[60] *= ((u8*)&b)[60]; ((u8*)&a)[61] *= ((u8*)&b)[61]; ((u8*)&a)[62] *= ((u8*)&b)[62]; ((u8*)&a)[63] *= ((u8*)&b)[63]; return a; }
	forceinline friend u8x64 operator*(u8x64 a, u8 b) {((u8*)&a)[0] *= b; ((u8*)&a)[1] *= b; ((u8*)&a)[2] *= b; ((u8*)&a)[3] *= b; ((u8*)&a)[4] *= b; ((u8*)&a)[5] *= b; ((u8*)&a)[6] *= b; ((u8*)&a)[7] *= b; ((u8*)&a)[8] *= b; ((u8*)&a)[9] *= b; ((u8*)&a)[10] *= b; ((u8*)&a)[11] *= b; ((u8*)&a)[12] *= b; ((u8*)&a)[13] *= b; ((u8*)&a)[14] *= b; ((u8*)&a)[15] *= b; ((u8*)&a)[16] *= b; ((u8*)&a)[17] *= b; ((u8*)&a)[18] *= b; ((u8*)&a)[19] *= b; ((u8*)&a)[20] *= b; ((u8*)&a)[21] *= b; ((u8*)&a)[22] *= b; ((u8*)&a)[23] *= b; ((u8*)&a)[24] *= b; ((u8*)&a)[25] *= b; ((u8*)&a)[26] *= b; ((u8*)&a)[27] *= b; ((u8*)&a)[28] *= b; ((u8*)&a)[29] *= b; ((u8*)&a)[30] *= b; ((u8*)&a)[31] *= b; ((u8*)&a)[32] *= b; ((u8*)&a)[33] *= b; ((u8*)&a)[34] *= b; ((u8*)&a)[35] *= b; ((u8*)&a)[36] *= b; ((u8*)&a)[37] *= b; ((u8*)&a)[38] *= b; ((u8*)&a)[39] *= b; ((u8*)&a)[40] *= b; ((u8*)&a)[41] *= b; ((u8*)&a)[42] *= b; ((u8*)&a)[43] *= b; ((u8*)&a)[44] *= b; ((u8*)&a)[45] *= b; ((u8*)&a)[46] *= b; ((u8*)&a)[47] *= b; ((u8*)&a)[48] *= b; ((u8*)&a)[49] *= b; ((u8*)&a)[50] *= b; ((u8*)&a)[51] *= b; ((u8*)&a)[52] *= b; ((u8*)&a)[53] *= b; ((u8*)&a)[54] *= b; ((u8*)&a)[55] *= b; ((u8*)&a)[56] *= b; ((u8*)&a)[57] *= b; ((u8*)&a)[58] *= b; ((u8*)&a)[59] *= b; ((u8*)&a)[60] *= b; ((u8*)&a)[61] *= b; ((u8*)&a)[62] *= b; ((u8*)&a)[63] *= b; return a; }
	forceinline friend u8x64 operator*(u8 a, u8x64 b) {((u8*)&b)[0] *= a; ((u8*)&b)[1] *= a; ((u8*)&b)[2] *= a; ((u8*)&b)[3] *= a; ((u8*)&b)[4] *= a; ((u8*)&b)[5] *= a; ((u8*)&b)[6] *= a; ((u8*)&b)[7] *= a; ((u8*)&b)[8] *= a; ((u8*)&b)[9] *= a; ((u8*)&b)[10] *= a; ((u8*)&b)[11] *= a; ((u8*)&b)[12] *= a; ((u8*)&b)[13] *= a; ((u8*)&b)[14] *= a; ((u8*)&b)[15] *= a; ((u8*)&b)[16] *= a; ((u8*)&b)[17] *= a; ((u8*)&b)[18] *= a; ((u8*)&b)[19] *= a; ((u8*)&b)[20] *= a; ((u8*)&b)[21] *= a; ((u8*)&b)[22] *= a; ((u8*)&b)[23] *= a; ((u8*)&b)[24] *= a; ((u8*)&b)[25] *= a; ((u8*)&b)[26] *= a; ((u8*)&b)[27] *= a; ((u8*)&b)[28] *= a; ((u8*)&b)[29] *= a; ((u8*)&b)[30] *= a; ((u8*)&b)[31] *= a; ((u8*)&b)[32] *= a; ((u8*)&b)[33] *= a; ((u8*)&b)[34] *= a; ((u8*)&b)[35] *= a; ((u8*)&b)[36] *= a; ((u8*)&b)[37] *= a; ((u8*)&b)[38] *= a; ((u8*)&b)[39] *= a; ((u8*)&b)[40] *= a; ((u8*)&b)[41] *= a; ((u8*)&b)[42] *= a; ((u8*)&b)[43] *= a; ((u8*)&b)[44] *= a; ((u8*)&b)[45] *= a; ((u8*)&b)[46] *= a; ((u8*)&b)[47] *= a; ((u8*)&b)[48] *= a; ((u8*)&b)[49] *= a; ((u8*)&b)[50] *= a; ((u8*)&b)[51] *= a; ((u8*)&b)[52] *= a; ((u8*)&b)[53] *= a; ((u8*)&b)[54] *= a; ((u8*)&b)[55] *= a; ((u8*)&b)[56] *= a; ((u8*)&b)[57] *= a; ((u8*)&b)[58] *= a; ((u8*)&b)[59] *= a; ((u8*)&b)[60] *= a; ((u8*)&b)[61] *= a; ((u8*)&b)[62] *= a; ((u8*)&b)[63] *= a; return b; }
	forceinline friend u8x64 &operator*=(u8x64 &a, u8x64 b) { return a=a*b; }
	forceinline friend u8x64 &operator*=(u8x64 &a, u8 b) { return a=a*b; }
	forceinline friend u8x64 operator/(u8x64 a, u8x64 b) { return {_mm512_div_epu8(a.m, b.m)}; }
	forceinline friend u8x64 operator/(u8x64 a, u8 b) { return {_mm512_div_epu8(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend u8x64 operator/(u8 a, u8x64 b) { return {_mm512_div_epu8(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend u8x64 &operator/=(u8x64 &a, u8x64 b) { return a=a/b; }
	forceinline friend u8x64 &operator/=(u8x64 &a, u8 b) { return a=a/b; }
	forceinline friend u8x64 operator^(u8x64 a, u8x64 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend u8x64 operator^(u8x64 a, u8 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend u8x64 operator^(u8 a, u8x64 b) { return {_mm512_xor_si512(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend u8x64 &operator^=(u8x64 &a, u8x64 b) { return a=a^b; }
	forceinline friend u8x64 &operator^=(u8x64 &a, u8 b) { return a=a^b; }
	forceinline friend u8x64 operator&(u8x64 a, u8x64 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend u8x64 operator&(u8x64 a, u8 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend u8x64 operator&(u8 a, u8x64 b) { return {_mm512_and_si512(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend u8x64 &operator&=(u8x64 &a, u8x64 b) { return a=a&b; }
	forceinline friend u8x64 &operator&=(u8x64 &a, u8 b) { return a=a&b; }
	forceinline friend u8x64 operator|(u8x64 a, u8x64 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend u8x64 operator|(u8x64 a, u8 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend u8x64 operator|(u8 a, u8x64 b) { return {_mm512_or_si512(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend u8x64 &operator|=(u8x64 &a, u8x64 b) { return a=a|b; }
	forceinline friend u8x64 &operator|=(u8x64 &a, u8 b) { return a=a|b; }
	forceinline operator s8x64() const;
};

forceinline u8x64 U8x64(u8 a) { return {_mm512_set1_epi8(a)}; }
forceinline u8x64 U8x64(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15,u8 _16,u8 _17,u8 _18,u8 _19,u8 _20,u8 _21,u8 _22,u8 _23,u8 _24,u8 _25,u8 _26,u8 _27,u8 _28,u8 _29,u8 _30,u8 _31,u8 _32,u8 _33,u8 _34,u8 _35,u8 _36,u8 _37,u8 _38,u8 _39,u8 _40,u8 _41,u8 _42,u8 _43,u8 _44,u8 _45,u8 _46,u8 _47,u8 _48,u8 _49,u8 _50,u8 _51,u8 _52,u8 _53,u8 _54,u8 _55,u8 _56,u8 _57,u8 _58,u8 _59,u8 _60,u8 _61,u8 _62,u8 _63) { return {_mm512_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63)}; }


union u16x32 {
	__m512i m;
	u16x16 x16[2];
	u16x8 x8[4];
	u16 s[32];
	forceinline u16 operator[](umm i) const { return s[i]; }
	forceinline u16 &operator[](umm i) { return s[i]; }
	forceinline friend u16x32 operator+(u16x32 a) { return a; }
	forceinline friend u16x32 operator-(u16x32 a) { return u16x32{} - a; }
	forceinline friend u16x32 operator~(u16x32 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend u16x32 operator+(u16x32 a, u16x32 b) { return {_mm512_add_epi16(a.m, b.m)}; }
	forceinline friend u16x32 operator+(u16x32 a, u16 b) { return {_mm512_add_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator+(u16 a, u16x32 b) { return {_mm512_add_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator+=(u16x32 &a, u16x32 b) { return a=a+b; }
	forceinline friend u16x32 &operator+=(u16x32 &a, u16 b) { return a=a+b; }
	forceinline friend u16x32 operator-(u16x32 a, u16x32 b) { return {_mm512_sub_epi16(a.m, b.m)}; }
	forceinline friend u16x32 operator-(u16x32 a, u16 b) { return {_mm512_sub_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator-(u16 a, u16x32 b) { return {_mm512_sub_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator-=(u16x32 &a, u16x32 b) { return a=a-b; }
	forceinline friend u16x32 &operator-=(u16x32 &a, u16 b) { return a=a-b; }
	forceinline friend u16x32 operator*(u16x32 a, u16x32 b) { return {_mm512_mullo_epi16(a.m, b.m)}; }
	forceinline friend u16x32 operator*(u16x32 a, u16 b) { return {_mm512_mullo_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator*(u16 a, u16x32 b) { return {_mm512_mullo_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator*=(u16x32 &a, u16x32 b) { return a=a*b; }
	forceinline friend u16x32 &operator*=(u16x32 &a, u16 b) { return a=a*b; }
	forceinline friend u16x32 operator/(u16x32 a, u16x32 b) { return {_mm512_div_epu16(a.m, b.m)}; }
	forceinline friend u16x32 operator/(u16x32 a, u16 b) { return {_mm512_div_epu16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator/(u16 a, u16x32 b) { return {_mm512_div_epu16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator/=(u16x32 &a, u16x32 b) { return a=a/b; }
	forceinline friend u16x32 &operator/=(u16x32 &a, u16 b) { return a=a/b; }
	forceinline friend u16x32 operator^(u16x32 a, u16x32 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend u16x32 operator^(u16x32 a, u16 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator^(u16 a, u16x32 b) { return {_mm512_xor_si512(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator^=(u16x32 &a, u16x32 b) { return a=a^b; }
	forceinline friend u16x32 &operator^=(u16x32 &a, u16 b) { return a=a^b; }
	forceinline friend u16x32 operator&(u16x32 a, u16x32 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend u16x32 operator&(u16x32 a, u16 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator&(u16 a, u16x32 b) { return {_mm512_and_si512(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator&=(u16x32 &a, u16x32 b) { return a=a&b; }
	forceinline friend u16x32 &operator&=(u16x32 &a, u16 b) { return a=a&b; }
	forceinline friend u16x32 operator|(u16x32 a, u16x32 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend u16x32 operator|(u16x32 a, u16 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend u16x32 operator|(u16 a, u16x32 b) { return {_mm512_or_si512(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend u16x32 &operator|=(u16x32 &a, u16x32 b) { return a=a|b; }
	forceinline friend u16x32 &operator|=(u16x32 &a, u16 b) { return a=a|b; }
	forceinline friend u16x32 operator<<(u16x32 a, u16 b) { return {_mm512_slli_epi16(a.m, b)}; }
	forceinline friend u16x32 operator>>(u16x32 a, u16 b) { return {_mm512_srli_epi16(a.m, b)}; }
	forceinline operator s16x32() const;
};

forceinline u16x32 U16x32(u16 a) { return {_mm512_set1_epi16(a)}; }
forceinline u16x32 U16x32(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7,u16 _8,u16 _9,u16 _10,u16 _11,u16 _12,u16 _13,u16 _14,u16 _15,u16 _16,u16 _17,u16 _18,u16 _19,u16 _20,u16 _21,u16 _22,u16 _23,u16 _24,u16 _25,u16 _26,u16 _27,u16 _28,u16 _29,u16 _30,u16 _31) { return {_mm512_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }


union u32x16 {
	__m512i m;
	u32x8 x8[2];
	u32x4 x4[4];
	u32 s[16];
	forceinline u32 operator[](umm i) const { return s[i]; }
	forceinline u32 &operator[](umm i) { return s[i]; }
	forceinline friend u32x16 operator+(u32x16 a) { return a; }
	forceinline friend u32x16 operator-(u32x16 a) { return u32x16{} - a; }
	forceinline friend u32x16 operator~(u32x16 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend u32x16 operator+(u32x16 a, u32x16 b) { return {_mm512_add_epi32(a.m, b.m)}; }
	forceinline friend u32x16 operator+(u32x16 a, u32 b) { return {_mm512_add_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator+(u32 a, u32x16 b) { return {_mm512_add_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator+=(u32x16 &a, u32x16 b) { return a=a+b; }
	forceinline friend u32x16 &operator+=(u32x16 &a, u32 b) { return a=a+b; }
	forceinline friend u32x16 operator-(u32x16 a, u32x16 b) { return {_mm512_sub_epi32(a.m, b.m)}; }
	forceinline friend u32x16 operator-(u32x16 a, u32 b) { return {_mm512_sub_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator-(u32 a, u32x16 b) { return {_mm512_sub_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator-=(u32x16 &a, u32x16 b) { return a=a-b; }
	forceinline friend u32x16 &operator-=(u32x16 &a, u32 b) { return a=a-b; }
	forceinline friend u32x16 operator*(u32x16 a, u32x16 b) { return {_mm512_mullo_epi32(a.m, b.m)}; }
	forceinline friend u32x16 operator*(u32x16 a, u32 b) { return {_mm512_mullo_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator*(u32 a, u32x16 b) { return {_mm512_mullo_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator*=(u32x16 &a, u32x16 b) { return a=a*b; }
	forceinline friend u32x16 &operator*=(u32x16 &a, u32 b) { return a=a*b; }
	forceinline friend u32x16 operator/(u32x16 a, u32x16 b) { return {_mm512_div_epu32(a.m, b.m)}; }
	forceinline friend u32x16 operator/(u32x16 a, u32 b) { return {_mm512_div_epu32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator/(u32 a, u32x16 b) { return {_mm512_div_epu32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator/=(u32x16 &a, u32x16 b) { return a=a/b; }
	forceinline friend u32x16 &operator/=(u32x16 &a, u32 b) { return a=a/b; }
	forceinline friend u32x16 operator^(u32x16 a, u32x16 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend u32x16 operator^(u32x16 a, u32 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator^(u32 a, u32x16 b) { return {_mm512_xor_si512(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator^=(u32x16 &a, u32x16 b) { return a=a^b; }
	forceinline friend u32x16 &operator^=(u32x16 &a, u32 b) { return a=a^b; }
	forceinline friend u32x16 operator&(u32x16 a, u32x16 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend u32x16 operator&(u32x16 a, u32 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator&(u32 a, u32x16 b) { return {_mm512_and_si512(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator&=(u32x16 &a, u32x16 b) { return a=a&b; }
	forceinline friend u32x16 &operator&=(u32x16 &a, u32 b) { return a=a&b; }
	forceinline friend u32x16 operator|(u32x16 a, u32x16 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend u32x16 operator|(u32x16 a, u32 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend u32x16 operator|(u32 a, u32x16 b) { return {_mm512_or_si512(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend u32x16 &operator|=(u32x16 &a, u32x16 b) { return a=a|b; }
	forceinline friend u32x16 &operator|=(u32x16 &a, u32 b) { return a=a|b; }
	forceinline friend u32x16 operator<<(u32x16 a, u32 b) { return {_mm512_slli_epi32(a.m, b)}; }
	forceinline friend u32x16 operator>>(u32x16 a, u32 b) { return {_mm512_srli_epi32(a.m, b)}; }
	forceinline operator s32x16() const;
	forceinline operator f32x16() const;
};

forceinline u32x16 U32x16(u32 a) { return {_mm512_set1_epi32(a)}; }
forceinline u32x16 U32x16(u32 _0,u32 _1,u32 _2,u32 _3,u32 _4,u32 _5,u32 _6,u32 _7,u32 _8,u32 _9,u32 _10,u32 _11,u32 _12,u32 _13,u32 _14,u32 _15) { return {_mm512_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }


union u64x8 {
	__m512i m;
	u64x4 x4[2];
	u64x2 x2[4];
	u64 s[8];
	forceinline u64 operator[](umm i) const { return s[i]; }
	forceinline u64 &operator[](umm i) { return s[i]; }
	forceinline friend u64x8 operator+(u64x8 a) { return a; }
	forceinline friend u64x8 operator-(u64x8 a) { return u64x8{} - a; }
	forceinline friend u64x8 operator~(u64x8 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend u64x8 operator+(u64x8 a, u64x8 b) { return {_mm512_add_epi64(a.m, b.m)}; }
	forceinline friend u64x8 operator+(u64x8 a, u64 b) { return {_mm512_add_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator+(u64 a, u64x8 b) { return {_mm512_add_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator+=(u64x8 &a, u64x8 b) { return a=a+b; }
	forceinline friend u64x8 &operator+=(u64x8 &a, u64 b) { return a=a+b; }
	forceinline friend u64x8 operator-(u64x8 a, u64x8 b) { return {_mm512_sub_epi64(a.m, b.m)}; }
	forceinline friend u64x8 operator-(u64x8 a, u64 b) { return {_mm512_sub_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator-(u64 a, u64x8 b) { return {_mm512_sub_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator-=(u64x8 &a, u64x8 b) { return a=a-b; }
	forceinline friend u64x8 &operator-=(u64x8 &a, u64 b) { return a=a-b; }
	forceinline friend u64x8 operator*(u64x8 a, u64x8 b) { return {_mm512_mullo_epi64(a.m, b.m)}; }
	forceinline friend u64x8 operator*(u64x8 a, u64 b) { return {_mm512_mullo_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator*(u64 a, u64x8 b) { return {_mm512_mullo_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator*=(u64x8 &a, u64x8 b) { return a=a*b; }
	forceinline friend u64x8 &operator*=(u64x8 &a, u64 b) { return a=a*b; }
	forceinline friend u64x8 operator/(u64x8 a, u64x8 b) { return {_mm512_div_epu64(a.m, b.m)}; }
	forceinline friend u64x8 operator/(u64x8 a, u64 b) { return {_mm512_div_epu64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator/(u64 a, u64x8 b) { return {_mm512_div_epu64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator/=(u64x8 &a, u64x8 b) { return a=a/b; }
	forceinline friend u64x8 &operator/=(u64x8 &a, u64 b) { return a=a/b; }
	forceinline friend u64x8 operator^(u64x8 a, u64x8 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend u64x8 operator^(u64x8 a, u64 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator^(u64 a, u64x8 b) { return {_mm512_xor_si512(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator^=(u64x8 &a, u64x8 b) { return a=a^b; }
	forceinline friend u64x8 &operator^=(u64x8 &a, u64 b) { return a=a^b; }
	forceinline friend u64x8 operator&(u64x8 a, u64x8 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend u64x8 operator&(u64x8 a, u64 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator&(u64 a, u64x8 b) { return {_mm512_and_si512(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator&=(u64x8 &a, u64x8 b) { return a=a&b; }
	forceinline friend u64x8 &operator&=(u64x8 &a, u64 b) { return a=a&b; }
	forceinline friend u64x8 operator|(u64x8 a, u64x8 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend u64x8 operator|(u64x8 a, u64 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend u64x8 operator|(u64 a, u64x8 b) { return {_mm512_or_si512(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend u64x8 &operator|=(u64x8 &a, u64x8 b) { return a=a|b; }
	forceinline friend u64x8 &operator|=(u64x8 &a, u64 b) { return a=a|b; }
	forceinline friend u64x8 operator<<(u64x8 a, u64 b) { return {_mm512_slli_epi64(a.m, b)}; }
	forceinline friend u64x8 operator>>(u64x8 a, u64 b) { return {_mm512_srli_epi64(a.m, b)}; }
	forceinline operator s64x8() const;
	forceinline operator f64x8() const;
};

forceinline u64x8 U64x8(u64 a) { return {_mm512_set1_epi64x(a)}; }
forceinline u64x8 U64x8(u64 _0,u64 _1,u64 _2,u64 _3,u64 _4,u64 _5,u64 _6,u64 _7) { return {_mm512_setr_epi64x(_0,_1,_2,_3,_4,_5,_6,_7)}; }


union s8x64 {
	__m512i m;
	s8x32 x32[2];
	s8x16 x16[4];
	s8 s[64];
	forceinline s8 operator[](umm i) const { return s[i]; }
	forceinline s8 &operator[](umm i) { return s[i]; }
	forceinline friend s8x64 operator+(s8x64 a) { return a; }
	forceinline friend s8x64 operator-(s8x64 a) { return s8x64{} - a; }
	forceinline friend s8x64 operator~(s8x64 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend s8x64 operator+(s8x64 a, s8x64 b) { return {_mm512_add_epi8(a.m, b.m)}; }
	forceinline friend s8x64 operator+(s8x64 a, s8 b) { return {_mm512_add_epi8(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend s8x64 operator+(s8 a, s8x64 b) { return {_mm512_add_epi8(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend s8x64 &operator+=(s8x64 &a, s8x64 b) { return a=a+b; }
	forceinline friend s8x64 &operator+=(s8x64 &a, s8 b) { return a=a+b; }
	forceinline friend s8x64 operator-(s8x64 a, s8x64 b) { return {_mm512_sub_epi8(a.m, b.m)}; }
	forceinline friend s8x64 operator-(s8x64 a, s8 b) { return {_mm512_sub_epi8(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend s8x64 operator-(s8 a, s8x64 b) { return {_mm512_sub_epi8(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend s8x64 &operator-=(s8x64 &a, s8x64 b) { return a=a-b; }
	forceinline friend s8x64 &operator-=(s8x64 &a, s8 b) { return a=a-b; }
	forceinline friend s8x64 operator*(s8x64 a, s8x64 b) {((s8*)&a)[0] *= ((s8*)&b)[0]; ((s8*)&a)[1] *= ((s8*)&b)[1]; ((s8*)&a)[2] *= ((s8*)&b)[2]; ((s8*)&a)[3] *= ((s8*)&b)[3]; ((s8*)&a)[4] *= ((s8*)&b)[4]; ((s8*)&a)[5] *= ((s8*)&b)[5]; ((s8*)&a)[6] *= ((s8*)&b)[6]; ((s8*)&a)[7] *= ((s8*)&b)[7]; ((s8*)&a)[8] *= ((s8*)&b)[8]; ((s8*)&a)[9] *= ((s8*)&b)[9]; ((s8*)&a)[10] *= ((s8*)&b)[10]; ((s8*)&a)[11] *= ((s8*)&b)[11]; ((s8*)&a)[12] *= ((s8*)&b)[12]; ((s8*)&a)[13] *= ((s8*)&b)[13]; ((s8*)&a)[14] *= ((s8*)&b)[14]; ((s8*)&a)[15] *= ((s8*)&b)[15]; ((s8*)&a)[16] *= ((s8*)&b)[16]; ((s8*)&a)[17] *= ((s8*)&b)[17]; ((s8*)&a)[18] *= ((s8*)&b)[18]; ((s8*)&a)[19] *= ((s8*)&b)[19]; ((s8*)&a)[20] *= ((s8*)&b)[20]; ((s8*)&a)[21] *= ((s8*)&b)[21]; ((s8*)&a)[22] *= ((s8*)&b)[22]; ((s8*)&a)[23] *= ((s8*)&b)[23]; ((s8*)&a)[24] *= ((s8*)&b)[24]; ((s8*)&a)[25] *= ((s8*)&b)[25]; ((s8*)&a)[26] *= ((s8*)&b)[26]; ((s8*)&a)[27] *= ((s8*)&b)[27]; ((s8*)&a)[28] *= ((s8*)&b)[28]; ((s8*)&a)[29] *= ((s8*)&b)[29]; ((s8*)&a)[30] *= ((s8*)&b)[30]; ((s8*)&a)[31] *= ((s8*)&b)[31]; ((s8*)&a)[32] *= ((s8*)&b)[32]; ((s8*)&a)[33] *= ((s8*)&b)[33]; ((s8*)&a)[34] *= ((s8*)&b)[34]; ((s8*)&a)[35] *= ((s8*)&b)[35]; ((s8*)&a)[36] *= ((s8*)&b)[36]; ((s8*)&a)[37] *= ((s8*)&b)[37]; ((s8*)&a)[38] *= ((s8*)&b)[38]; ((s8*)&a)[39] *= ((s8*)&b)[39]; ((s8*)&a)[40] *= ((s8*)&b)[40]; ((s8*)&a)[41] *= ((s8*)&b)[41]; ((s8*)&a)[42] *= ((s8*)&b)[42]; ((s8*)&a)[43] *= ((s8*)&b)[43]; ((s8*)&a)[44] *= ((s8*)&b)[44]; ((s8*)&a)[45] *= ((s8*)&b)[45]; ((s8*)&a)[46] *= ((s8*)&b)[46]; ((s8*)&a)[47] *= ((s8*)&b)[47]; ((s8*)&a)[48] *= ((s8*)&b)[48]; ((s8*)&a)[49] *= ((s8*)&b)[49]; ((s8*)&a)[50] *= ((s8*)&b)[50]; ((s8*)&a)[51] *= ((s8*)&b)[51]; ((s8*)&a)[52] *= ((s8*)&b)[52]; ((s8*)&a)[53] *= ((s8*)&b)[53]; ((s8*)&a)[54] *= ((s8*)&b)[54]; ((s8*)&a)[55] *= ((s8*)&b)[55]; ((s8*)&a)[56] *= ((s8*)&b)[56]; ((s8*)&a)[57] *= ((s8*)&b)[57]; ((s8*)&a)[58] *= ((s8*)&b)[58]; ((s8*)&a)[59] *= ((s8*)&b)[59]; ((s8*)&a)[60] *= ((s8*)&b)[60]; ((s8*)&a)[61] *= ((s8*)&b)[61]; ((s8*)&a)[62] *= ((s8*)&b)[62]; ((s8*)&a)[63] *= ((s8*)&b)[63]; return a; }
	forceinline friend s8x64 operator*(s8x64 a, s8 b) {((s8*)&a)[0] *= b; ((s8*)&a)[1] *= b; ((s8*)&a)[2] *= b; ((s8*)&a)[3] *= b; ((s8*)&a)[4] *= b; ((s8*)&a)[5] *= b; ((s8*)&a)[6] *= b; ((s8*)&a)[7] *= b; ((s8*)&a)[8] *= b; ((s8*)&a)[9] *= b; ((s8*)&a)[10] *= b; ((s8*)&a)[11] *= b; ((s8*)&a)[12] *= b; ((s8*)&a)[13] *= b; ((s8*)&a)[14] *= b; ((s8*)&a)[15] *= b; ((s8*)&a)[16] *= b; ((s8*)&a)[17] *= b; ((s8*)&a)[18] *= b; ((s8*)&a)[19] *= b; ((s8*)&a)[20] *= b; ((s8*)&a)[21] *= b; ((s8*)&a)[22] *= b; ((s8*)&a)[23] *= b; ((s8*)&a)[24] *= b; ((s8*)&a)[25] *= b; ((s8*)&a)[26] *= b; ((s8*)&a)[27] *= b; ((s8*)&a)[28] *= b; ((s8*)&a)[29] *= b; ((s8*)&a)[30] *= b; ((s8*)&a)[31] *= b; ((s8*)&a)[32] *= b; ((s8*)&a)[33] *= b; ((s8*)&a)[34] *= b; ((s8*)&a)[35] *= b; ((s8*)&a)[36] *= b; ((s8*)&a)[37] *= b; ((s8*)&a)[38] *= b; ((s8*)&a)[39] *= b; ((s8*)&a)[40] *= b; ((s8*)&a)[41] *= b; ((s8*)&a)[42] *= b; ((s8*)&a)[43] *= b; ((s8*)&a)[44] *= b; ((s8*)&a)[45] *= b; ((s8*)&a)[46] *= b; ((s8*)&a)[47] *= b; ((s8*)&a)[48] *= b; ((s8*)&a)[49] *= b; ((s8*)&a)[50] *= b; ((s8*)&a)[51] *= b; ((s8*)&a)[52] *= b; ((s8*)&a)[53] *= b; ((s8*)&a)[54] *= b; ((s8*)&a)[55] *= b; ((s8*)&a)[56] *= b; ((s8*)&a)[57] *= b; ((s8*)&a)[58] *= b; ((s8*)&a)[59] *= b; ((s8*)&a)[60] *= b; ((s8*)&a)[61] *= b; ((s8*)&a)[62] *= b; ((s8*)&a)[63] *= b; return a; }
	forceinline friend s8x64 operator*(s8 a, s8x64 b) {((s8*)&b)[0] *= a; ((s8*)&b)[1] *= a; ((s8*)&b)[2] *= a; ((s8*)&b)[3] *= a; ((s8*)&b)[4] *= a; ((s8*)&b)[5] *= a; ((s8*)&b)[6] *= a; ((s8*)&b)[7] *= a; ((s8*)&b)[8] *= a; ((s8*)&b)[9] *= a; ((s8*)&b)[10] *= a; ((s8*)&b)[11] *= a; ((s8*)&b)[12] *= a; ((s8*)&b)[13] *= a; ((s8*)&b)[14] *= a; ((s8*)&b)[15] *= a; ((s8*)&b)[16] *= a; ((s8*)&b)[17] *= a; ((s8*)&b)[18] *= a; ((s8*)&b)[19] *= a; ((s8*)&b)[20] *= a; ((s8*)&b)[21] *= a; ((s8*)&b)[22] *= a; ((s8*)&b)[23] *= a; ((s8*)&b)[24] *= a; ((s8*)&b)[25] *= a; ((s8*)&b)[26] *= a; ((s8*)&b)[27] *= a; ((s8*)&b)[28] *= a; ((s8*)&b)[29] *= a; ((s8*)&b)[30] *= a; ((s8*)&b)[31] *= a; ((s8*)&b)[32] *= a; ((s8*)&b)[33] *= a; ((s8*)&b)[34] *= a; ((s8*)&b)[35] *= a; ((s8*)&b)[36] *= a; ((s8*)&b)[37] *= a; ((s8*)&b)[38] *= a; ((s8*)&b)[39] *= a; ((s8*)&b)[40] *= a; ((s8*)&b)[41] *= a; ((s8*)&b)[42] *= a; ((s8*)&b)[43] *= a; ((s8*)&b)[44] *= a; ((s8*)&b)[45] *= a; ((s8*)&b)[46] *= a; ((s8*)&b)[47] *= a; ((s8*)&b)[48] *= a; ((s8*)&b)[49] *= a; ((s8*)&b)[50] *= a; ((s8*)&b)[51] *= a; ((s8*)&b)[52] *= a; ((s8*)&b)[53] *= a; ((s8*)&b)[54] *= a; ((s8*)&b)[55] *= a; ((s8*)&b)[56] *= a; ((s8*)&b)[57] *= a; ((s8*)&b)[58] *= a; ((s8*)&b)[59] *= a; ((s8*)&b)[60] *= a; ((s8*)&b)[61] *= a; ((s8*)&b)[62] *= a; ((s8*)&b)[63] *= a; return b; }
	forceinline friend s8x64 &operator*=(s8x64 &a, s8x64 b) { return a=a*b; }
	forceinline friend s8x64 &operator*=(s8x64 &a, s8 b) { return a=a*b; }
	forceinline friend s8x64 operator/(s8x64 a, s8x64 b) { return {_mm512_div_epi8(a.m, b.m)}; }
	forceinline friend s8x64 operator/(s8x64 a, s8 b) { return {_mm512_div_epi8(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend s8x64 operator/(s8 a, s8x64 b) { return {_mm512_div_epi8(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend s8x64 &operator/=(s8x64 &a, s8x64 b) { return a=a/b; }
	forceinline friend s8x64 &operator/=(s8x64 &a, s8 b) { return a=a/b; }
	forceinline friend s8x64 operator^(s8x64 a, s8x64 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend s8x64 operator^(s8x64 a, s8 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend s8x64 operator^(s8 a, s8x64 b) { return {_mm512_xor_si512(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend s8x64 &operator^=(s8x64 &a, s8x64 b) { return a=a^b; }
	forceinline friend s8x64 &operator^=(s8x64 &a, s8 b) { return a=a^b; }
	forceinline friend s8x64 operator&(s8x64 a, s8x64 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend s8x64 operator&(s8x64 a, s8 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend s8x64 operator&(s8 a, s8x64 b) { return {_mm512_and_si512(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend s8x64 &operator&=(s8x64 &a, s8x64 b) { return a=a&b; }
	forceinline friend s8x64 &operator&=(s8x64 &a, s8 b) { return a=a&b; }
	forceinline friend s8x64 operator|(s8x64 a, s8x64 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend s8x64 operator|(s8x64 a, s8 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi8(b))}; }
	forceinline friend s8x64 operator|(s8 a, s8x64 b) { return {_mm512_or_si512(_mm512_set1_epi8(a), b.m)}; }
	forceinline friend s8x64 &operator|=(s8x64 &a, s8x64 b) { return a=a|b; }
	forceinline friend s8x64 &operator|=(s8x64 &a, s8 b) { return a=a|b; }
	forceinline operator u8x64() const;
};

forceinline s8x64 S8x64(s8 a) { return {_mm512_set1_epi8(a)}; }
forceinline s8x64 S8x64(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15,s8 _16,s8 _17,s8 _18,s8 _19,s8 _20,s8 _21,s8 _22,s8 _23,s8 _24,s8 _25,s8 _26,s8 _27,s8 _28,s8 _29,s8 _30,s8 _31,s8 _32,s8 _33,s8 _34,s8 _35,s8 _36,s8 _37,s8 _38,s8 _39,s8 _40,s8 _41,s8 _42,s8 _43,s8 _44,s8 _45,s8 _46,s8 _47,s8 _48,s8 _49,s8 _50,s8 _51,s8 _52,s8 _53,s8 _54,s8 _55,s8 _56,s8 _57,s8 _58,s8 _59,s8 _60,s8 _61,s8 _62,s8 _63) { return {_mm512_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63)}; }


union s16x32 {
	__m512i m;
	s16x16 x16[2];
	s16x8 x8[4];
	s16 s[32];
	forceinline s16 operator[](umm i) const { return s[i]; }
	forceinline s16 &operator[](umm i) { return s[i]; }
	forceinline friend s16x32 operator+(s16x32 a) { return a; }
	forceinline friend s16x32 operator-(s16x32 a) { return s16x32{} - a; }
	forceinline friend s16x32 operator~(s16x32 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend s16x32 operator+(s16x32 a, s16x32 b) { return {_mm512_add_epi16(a.m, b.m)}; }
	forceinline friend s16x32 operator+(s16x32 a, s16 b) { return {_mm512_add_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator+(s16 a, s16x32 b) { return {_mm512_add_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator+=(s16x32 &a, s16x32 b) { return a=a+b; }
	forceinline friend s16x32 &operator+=(s16x32 &a, s16 b) { return a=a+b; }
	forceinline friend s16x32 operator-(s16x32 a, s16x32 b) { return {_mm512_sub_epi16(a.m, b.m)}; }
	forceinline friend s16x32 operator-(s16x32 a, s16 b) { return {_mm512_sub_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator-(s16 a, s16x32 b) { return {_mm512_sub_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator-=(s16x32 &a, s16x32 b) { return a=a-b; }
	forceinline friend s16x32 &operator-=(s16x32 &a, s16 b) { return a=a-b; }
	forceinline friend s16x32 operator*(s16x32 a, s16x32 b) { return {_mm512_mullo_epi16(a.m, b.m)}; }
	forceinline friend s16x32 operator*(s16x32 a, s16 b) { return {_mm512_mullo_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator*(s16 a, s16x32 b) { return {_mm512_mullo_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator*=(s16x32 &a, s16x32 b) { return a=a*b; }
	forceinline friend s16x32 &operator*=(s16x32 &a, s16 b) { return a=a*b; }
	forceinline friend s16x32 operator/(s16x32 a, s16x32 b) { return {_mm512_div_epi16(a.m, b.m)}; }
	forceinline friend s16x32 operator/(s16x32 a, s16 b) { return {_mm512_div_epi16(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator/(s16 a, s16x32 b) { return {_mm512_div_epi16(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator/=(s16x32 &a, s16x32 b) { return a=a/b; }
	forceinline friend s16x32 &operator/=(s16x32 &a, s16 b) { return a=a/b; }
	forceinline friend s16x32 operator^(s16x32 a, s16x32 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend s16x32 operator^(s16x32 a, s16 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator^(s16 a, s16x32 b) { return {_mm512_xor_si512(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator^=(s16x32 &a, s16x32 b) { return a=a^b; }
	forceinline friend s16x32 &operator^=(s16x32 &a, s16 b) { return a=a^b; }
	forceinline friend s16x32 operator&(s16x32 a, s16x32 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend s16x32 operator&(s16x32 a, s16 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator&(s16 a, s16x32 b) { return {_mm512_and_si512(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator&=(s16x32 &a, s16x32 b) { return a=a&b; }
	forceinline friend s16x32 &operator&=(s16x32 &a, s16 b) { return a=a&b; }
	forceinline friend s16x32 operator|(s16x32 a, s16x32 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend s16x32 operator|(s16x32 a, s16 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi16(b))}; }
	forceinline friend s16x32 operator|(s16 a, s16x32 b) { return {_mm512_or_si512(_mm512_set1_epi16(a), b.m)}; }
	forceinline friend s16x32 &operator|=(s16x32 &a, s16x32 b) { return a=a|b; }
	forceinline friend s16x32 &operator|=(s16x32 &a, s16 b) { return a=a|b; }
	forceinline friend s16x32 operator<<(s16x32 a, s16 b) { return {_mm512_slli_epi16(a.m, b)}; }
	forceinline friend s16x32 operator>>(s16x32 a, s16 b) { return {_mm512_srai_epi16(a.m, b)}; }
	forceinline operator u16x32() const;
};

forceinline s16x32 S16x32(s16 a) { return {_mm512_set1_epi16(a)}; }
forceinline s16x32 S16x32(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7,s16 _8,s16 _9,s16 _10,s16 _11,s16 _12,s16 _13,s16 _14,s16 _15,s16 _16,s16 _17,s16 _18,s16 _19,s16 _20,s16 _21,s16 _22,s16 _23,s16 _24,s16 _25,s16 _26,s16 _27,s16 _28,s16 _29,s16 _30,s16 _31) { return {_mm512_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }


union s32x16 {
	__m512i m;
	s32x8 x8[2];
	s32x4 x4[4];
	s32 s[16];
	forceinline s32 operator[](umm i) const { return s[i]; }
	forceinline s32 &operator[](umm i) { return s[i]; }
	forceinline friend s32x16 operator+(s32x16 a) { return a; }
	forceinline friend s32x16 operator-(s32x16 a) { return s32x16{} - a; }
	forceinline friend s32x16 operator~(s32x16 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend s32x16 operator+(s32x16 a, s32x16 b) { return {_mm512_add_epi32(a.m, b.m)}; }
	forceinline friend s32x16 operator+(s32x16 a, s32 b) { return {_mm512_add_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator+(s32 a, s32x16 b) { return {_mm512_add_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator+=(s32x16 &a, s32x16 b) { return a=a+b; }
	forceinline friend s32x16 &operator+=(s32x16 &a, s32 b) { return a=a+b; }
	forceinline friend s32x16 operator-(s32x16 a, s32x16 b) { return {_mm512_sub_epi32(a.m, b.m)}; }
	forceinline friend s32x16 operator-(s32x16 a, s32 b) { return {_mm512_sub_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator-(s32 a, s32x16 b) { return {_mm512_sub_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator-=(s32x16 &a, s32x16 b) { return a=a-b; }
	forceinline friend s32x16 &operator-=(s32x16 &a, s32 b) { return a=a-b; }
	forceinline friend s32x16 operator*(s32x16 a, s32x16 b) { return {_mm512_mullo_epi32(a.m, b.m)}; }
	forceinline friend s32x16 operator*(s32x16 a, s32 b) { return {_mm512_mullo_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator*(s32 a, s32x16 b) { return {_mm512_mullo_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator*=(s32x16 &a, s32x16 b) { return a=a*b; }
	forceinline friend s32x16 &operator*=(s32x16 &a, s32 b) { return a=a*b; }
	forceinline friend s32x16 operator/(s32x16 a, s32x16 b) { return {_mm512_div_epi32(a.m, b.m)}; }
	forceinline friend s32x16 operator/(s32x16 a, s32 b) { return {_mm512_div_epi32(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator/(s32 a, s32x16 b) { return {_mm512_div_epi32(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator/=(s32x16 &a, s32x16 b) { return a=a/b; }
	forceinline friend s32x16 &operator/=(s32x16 &a, s32 b) { return a=a/b; }
	forceinline friend s32x16 operator^(s32x16 a, s32x16 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend s32x16 operator^(s32x16 a, s32 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator^(s32 a, s32x16 b) { return {_mm512_xor_si512(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator^=(s32x16 &a, s32x16 b) { return a=a^b; }
	forceinline friend s32x16 &operator^=(s32x16 &a, s32 b) { return a=a^b; }
	forceinline friend s32x16 operator&(s32x16 a, s32x16 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend s32x16 operator&(s32x16 a, s32 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator&(s32 a, s32x16 b) { return {_mm512_and_si512(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator&=(s32x16 &a, s32x16 b) { return a=a&b; }
	forceinline friend s32x16 &operator&=(s32x16 &a, s32 b) { return a=a&b; }
	forceinline friend s32x16 operator|(s32x16 a, s32x16 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend s32x16 operator|(s32x16 a, s32 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi32(b))}; }
	forceinline friend s32x16 operator|(s32 a, s32x16 b) { return {_mm512_or_si512(_mm512_set1_epi32(a), b.m)}; }
	forceinline friend s32x16 &operator|=(s32x16 &a, s32x16 b) { return a=a|b; }
	forceinline friend s32x16 &operator|=(s32x16 &a, s32 b) { return a=a|b; }
	forceinline friend s32x16 operator<<(s32x16 a, s32 b) { return {_mm512_slli_epi32(a.m, b)}; }
	forceinline friend s32x16 operator>>(s32x16 a, s32 b) { return {_mm512_srai_epi32(a.m, b)}; }
	forceinline operator u32x16() const;
	forceinline operator f32x16() const;
};

forceinline s32x16 S32x16(s32 a) { return {_mm512_set1_epi32(a)}; }
forceinline s32x16 S32x16(s32 _0,s32 _1,s32 _2,s32 _3,s32 _4,s32 _5,s32 _6,s32 _7,s32 _8,s32 _9,s32 _10,s32 _11,s32 _12,s32 _13,s32 _14,s32 _15) { return {_mm512_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }


union s64x8 {
	__m512i m;
	s64x4 x4[2];
	s64x2 x2[4];
	s64 s[8];
	forceinline s64 operator[](umm i) const { return s[i]; }
	forceinline s64 &operator[](umm i) { return s[i]; }
	forceinline friend s64x8 operator+(s64x8 a) { return a; }
	forceinline friend s64x8 operator-(s64x8 a) { return s64x8{} - a; }
	forceinline friend s64x8 operator~(s64x8 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
	forceinline friend s64x8 operator+(s64x8 a, s64x8 b) { return {_mm512_add_epi64(a.m, b.m)}; }
	forceinline friend s64x8 operator+(s64x8 a, s64 b) { return {_mm512_add_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator+(s64 a, s64x8 b) { return {_mm512_add_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator+=(s64x8 &a, s64x8 b) { return a=a+b; }
	forceinline friend s64x8 &operator+=(s64x8 &a, s64 b) { return a=a+b; }
	forceinline friend s64x8 operator-(s64x8 a, s64x8 b) { return {_mm512_sub_epi64(a.m, b.m)}; }
	forceinline friend s64x8 operator-(s64x8 a, s64 b) { return {_mm512_sub_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator-(s64 a, s64x8 b) { return {_mm512_sub_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator-=(s64x8 &a, s64x8 b) { return a=a-b; }
	forceinline friend s64x8 &operator-=(s64x8 &a, s64 b) { return a=a-b; }
	forceinline friend s64x8 operator*(s64x8 a, s64x8 b) { return {_mm512_mullo_epi64(a.m, b.m)}; }
	forceinline friend s64x8 operator*(s64x8 a, s64 b) { return {_mm512_mullo_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator*(s64 a, s64x8 b) { return {_mm512_mullo_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator*=(s64x8 &a, s64x8 b) { return a=a*b; }
	forceinline friend s64x8 &operator*=(s64x8 &a, s64 b) { return a=a*b; }
	forceinline friend s64x8 operator/(s64x8 a, s64x8 b) { return {_mm512_div_epi64(a.m, b.m)}; }
	forceinline friend s64x8 operator/(s64x8 a, s64 b) { return {_mm512_div_epi64(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator/(s64 a, s64x8 b) { return {_mm512_div_epi64(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator/=(s64x8 &a, s64x8 b) { return a=a/b; }
	forceinline friend s64x8 &operator/=(s64x8 &a, s64 b) { return a=a/b; }
	forceinline friend s64x8 operator^(s64x8 a, s64x8 b) { return {_mm512_xor_si512(a.m, b.m)}; }
	forceinline friend s64x8 operator^(s64x8 a, s64 b) { return {_mm512_xor_si512(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator^(s64 a, s64x8 b) { return {_mm512_xor_si512(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator^=(s64x8 &a, s64x8 b) { return a=a^b; }
	forceinline friend s64x8 &operator^=(s64x8 &a, s64 b) { return a=a^b; }
	forceinline friend s64x8 operator&(s64x8 a, s64x8 b) { return {_mm512_and_si512(a.m, b.m)}; }
	forceinline friend s64x8 operator&(s64x8 a, s64 b) { return {_mm512_and_si512(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator&(s64 a, s64x8 b) { return {_mm512_and_si512(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator&=(s64x8 &a, s64x8 b) { return a=a&b; }
	forceinline friend s64x8 &operator&=(s64x8 &a, s64 b) { return a=a&b; }
	forceinline friend s64x8 operator|(s64x8 a, s64x8 b) { return {_mm512_or_si512(a.m, b.m)}; }
	forceinline friend s64x8 operator|(s64x8 a, s64 b) { return {_mm512_or_si512(a.m, _mm512_set1_epi64x(b))}; }
	forceinline friend s64x8 operator|(s64 a, s64x8 b) { return {_mm512_or_si512(_mm512_set1_epi64x(a), b.m)}; }
	forceinline friend s64x8 &operator|=(s64x8 &a, s64x8 b) { return a=a|b; }
	forceinline friend s64x8 &operator|=(s64x8 &a, s64 b) { return a=a|b; }
	forceinline friend s64x8 operator<<(s64x8 a, s64 b) { return {_mm512_slli_epi64(a.m, b)}; }
	forceinline friend s64x8 operator>>(s64x8 a, s64 b) { return {_mm512_srai_epi64(a.m, b)}; }
	forceinline operator u64x8() const;
	forceinline operator f64x8() const;
};

forceinline s64x8 S64x8(s64 a) { return {_mm512_set1_epi64x(a)}; }
forceinline s64x8 S64x8(s64 _0,s64 _1,s64 _2,s64 _3,s64 _4,s64 _5,s64 _6,s64 _7) { return {_mm512_setr_epi64x(_0,_1,_2,_3,_4,_5,_6,_7)}; }


union f32x16 {
	__m512 m;
	f32x8 x8[2];
	f32x4 x4[4];
	f32 s[16];
	forceinline f32 operator[](umm i) const { return s[i]; }
	forceinline f32 &operator[](umm i) { return s[i]; }
	forceinline friend f32x16 operator+(f32x16 a) { return a; }
	forceinline friend f32x16 operator-(f32x16 a) { return f32x16{} - a; }
	forceinline friend f32x16 operator+(f32x16 a, f32x16 b) { return {_mm512_add_ps(a.m, b.m)}; }
	forceinline friend f32x16 operator+(f32x16 a, f32 b) { return {_mm512_add_ps(a.m, _mm512_set1_ps(b))}; }
	forceinline friend f32x16 operator+(f32 a, f32x16 b) { return {_mm512_add_ps(_mm512_set1_ps(a), b.m)}; }
	forceinline friend f32x16 &operator+=(f32x16 &a, f32x16 b) { return a=a+b; }
	forceinline friend f32x16 &operator+=(f32x16 &a, f32 b) { return a=a+b; }
	forceinline friend f32x16 operator-(f32x16 a, f32x16 b) { return {_mm512_sub_ps(a.m, b.m)}; }
	forceinline friend f32x16 operator-(f32x16 a, f32 b) { return {_mm512_sub_ps(a.m, _mm512_set1_ps(b))}; }
	forceinline friend f32x16 operator-(f32 a, f32x16 b) { return {_mm512_sub_ps(_mm512_set1_ps(a), b.m)}; }
	forceinline friend f32x16 &operator-=(f32x16 &a, f32x16 b) { return a=a-b; }
	forceinline friend f32x16 &operator-=(f32x16 &a, f32 b) { return a=a-b; }
	forceinline friend f32x16 operator*(f32x16 a, f32x16 b) { return {_mm512_mul_ps(a.m, b.m)}; }
	forceinline friend f32x16 operator*(f32x16 a, f32 b) { return {_mm512_mul_ps(a.m, _mm512_set1_ps(b))}; }
	forceinline friend f32x16 operator*(f32 a, f32x16 b) { return {_mm512_mul_ps(_mm512_set1_ps(a), b.m)}; }
	forceinline friend f32x16 &operator*=(f32x16 &a, f32x16 b) { return a=a*b; }
	forceinline friend f32x16 &operator*=(f32x16 &a, f32 b) { return a=a*b; }
	forceinline friend f32x16 operator/(f32x16 a, f32x16 b) { return {_mm512_div_ps(a.m, b.m)}; }
	forceinline friend f32x16 operator/(f32x16 a, f32 b) { return {_mm512_div_ps(a.m, _mm512_set1_ps(b))}; }
	forceinline friend f32x16 operator/(f32 a, f32x16 b) { return {_mm512_div_ps(_mm512_set1_ps(a), b.m)}; }
	forceinline friend f32x16 &operator/=(f32x16 &a, f32x16 b) { return a=a/b; }
	forceinline friend f32x16 &operator/=(f32x16 &a, f32 b) { return a=a/b; }
	forceinline operator u32x16() const;
	forceinline operator s32x16() const;
};

forceinline f32x16 F32x16(f32 a) { return {_mm512_set1_ps(a)}; }
forceinline f32x16 F32x16(f32 _0,f32 _1,f32 _2,f32 _3,f32 _4,f32 _5,f32 _6,f32 _7,f32 _8,f32 _9,f32 _10,f32 _11,f32 _12,f32 _13,f32 _14,f32 _15) { return {_mm512_setr_ps(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }

forceinline f32x16 fmadd(f32x16 a, f32x16 b, f32x16 c) { return {_mm512_fmadd_ps(a.m, b.m, c.m)}; }

union f64x8 {
	__m512d m;
	f64x4 x4[2];
	f64x2 x2[4];
	f64 s[8];
	forceinline f64 operator[](umm i) const { return s[i]; }
	forceinline f64 &operator[](umm i) { return s[i]; }
	forceinline friend f64x8 operator+(f64x8 a) { return a; }
	forceinline friend f64x8 operator-(f64x8 a) { return f64x8{} - a; }
	forceinline friend f64x8 operator+(f64x8 a, f64x8 b) { return {_mm512_add_pd(a.m, b.m)}; }
	forceinline friend f64x8 operator+(f64x8 a, f64 b) { return {_mm512_add_pd(a.m, _mm512_set1_pd(b))}; }
	forceinline friend f64x8 operator+(f64 a, f64x8 b) { return {_mm512_add_pd(_mm512_set1_pd(a), b.m)}; }
	forceinline friend f64x8 &operator+=(f64x8 &a, f64x8 b) { return a=a+b; }
	forceinline friend f64x8 &operator+=(f64x8 &a, f64 b) { return a=a+b; }
	forceinline friend f64x8 operator-(f64x8 a, f64x8 b) { return {_mm512_sub_pd(a.m, b.m)}; }
	forceinline friend f64x8 operator-(f64x8 a, f64 b) { return {_mm512_sub_pd(a.m, _mm512_set1_pd(b))}; }
	forceinline friend f64x8 operator-(f64 a, f64x8 b) { return {_mm512_sub_pd(_mm512_set1_pd(a), b.m)}; }
	forceinline friend f64x8 &operator-=(f64x8 &a, f64x8 b) { return a=a-b; }
	forceinline friend f64x8 &operator-=(f64x8 &a, f64 b) { return a=a-b; }
	forceinline friend f64x8 operator*(f64x8 a, f64x8 b) { return {_mm512_mul_pd(a.m, b.m)}; }
	forceinline friend f64x8 operator*(f64x8 a, f64 b) { return {_mm512_mul_pd(a.m, _mm512_set1_pd(b))}; }
	forceinline friend f64x8 operator*(f64 a, f64x8 b) { return {_mm512_mul_pd(_mm512_set1_pd(a), b.m)}; }
	forceinline friend f64x8 &operator*=(f64x8 &a, f64x8 b) { return a=a*b; }
	forceinline friend f64x8 &operator*=(f64x8 &a, f64 b) { return a=a*b; }
	forceinline friend f64x8 operator/(f64x8 a, f64x8 b) { return {_mm512_div_pd(a.m, b.m)}; }
	forceinline friend f64x8 operator/(f64x8 a, f64 b) { return {_mm512_div_pd(a.m, _mm512_set1_pd(b))}; }
	forceinline friend f64x8 operator/(f64 a, f64x8 b) { return {_mm512_div_pd(_mm512_set1_pd(a), b.m)}; }
	forceinline friend f64x8 &operator/=(f64x8 &a, f64x8 b) { return a=a/b; }
	forceinline friend f64x8 &operator/=(f64x8 &a, f64 b) { return a=a/b; }
	forceinline operator u64x8() const;
	forceinline operator s64x8() const;
};

forceinline f64x8 F64x8(f64 a) { return {_mm512_set1_pd(a)}; }
forceinline f64x8 F64x8(f64 _0,f64 _1,f64 _2,f64 _3,f64 _4,f64 _5,f64 _6,f64 _7) { return {_mm512_setr_pd(_0,_1,_2,_3,_4,_5,_6,_7)}; }

forceinline f64x8 fmadd(f64x8 a, f64x8 b, f64x8 c) { return {_mm512_fmadd_pd(a.m, b.m, c.m)}; }

forceinline u8x64::operator s8x64() const { return {m}; }
forceinline u16x32::operator s16x32() const { return {m}; }
forceinline u32x16::operator s32x16() const { return {m}; }
forceinline u32x16::operator f32x16() const {
#if ARCH_AVX512
	return {_mm512_cvtepu32_ps(m)}; }
#else
	f32x16 r;
	r[0] = (f32)s[0];
	r[1] = (f32)s[1];
	r[2] = (f32)s[2];
	r[3] = (f32)s[3];
	r[4] = (f32)s[4];
	r[5] = (f32)s[5];
	r[6] = (f32)s[6];
	r[7] = (f32)s[7];
	r[8] = (f32)s[8];
	r[9] = (f32)s[9];
	r[10] = (f32)s[10];
	r[11] = (f32)s[11];
	r[12] = (f32)s[12];
	r[13] = (f32)s[13];
	r[14] = (f32)s[14];
	r[15] = (f32)s[15];
	return r;
#endif
}
forceinline u64x8::operator s64x8() const { return {m}; }
forceinline u64x8::operator f64x8() const {
#if ARCH_AVX512
	return {_mm512_cvtepu64_pd(m)}; }
#else
	f64x8 r;
	r[0] = (f64)s[0];
	r[1] = (f64)s[1];
	r[2] = (f64)s[2];
	r[3] = (f64)s[3];
	r[4] = (f64)s[4];
	r[5] = (f64)s[5];
	r[6] = (f64)s[6];
	r[7] = (f64)s[7];
	return r;
#endif
}
forceinline s8x64::operator u8x64() const { return {m}; }
forceinline s16x32::operator u16x32() const { return {m}; }
forceinline s32x16::operator u32x16() const { return {m}; }
forceinline s32x16::operator f32x16() const { return {_mm512_cvtepi32_ps(m)}; }
forceinline s64x8::operator u64x8() const { return {m}; }
forceinline s64x8::operator f64x8() const { return {_mm512_cvtepi64_pd(m)}; }
forceinline f32x16::operator u32x16() const { return {_mm512_cvtps_epu32(m)}; }
forceinline f32x16::operator s32x16() const { return {_mm512_cvtps_epi32(m)}; }
forceinline f64x8::operator u64x8() const { return {_mm512_cvtpd_epu64(m)}; }
forceinline f64x8::operator s64x8() const { return {_mm512_cvtpd_epi64(m)}; }

#endif

}
}
