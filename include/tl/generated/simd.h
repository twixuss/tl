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
forceinline u8x16 U8x16(u8);
forceinline u8x16 U8x16(u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8);

union u8x16 {
	__m128i m;
	u8 s[16];
	forceinline u8 operator[](umm i) const { return s[i]; }
	forceinline u8 &operator[](umm i) { return s[i]; }
	forceinline friend u8x16 operator+(u8x16 a) { return a; }
	forceinline friend u8x16 operator-(u8x16 a) { return u8x16{} - a; }
	forceinline friend u8x16 operator~(u8x16 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u8x16 operator+(u8x16 a, u8x16 b) { return {_mm_add_epi8(a.m, b.m)}; }
	forceinline friend u8x16 operator+(u8x16 a, u8 b) { return a+U8x16(b); }
	forceinline friend u8x16 operator+(u8 a, u8x16 b) { return U8x16(a)+b; }
	forceinline friend u8x16 &operator+=(u8x16 &a, u8x16 b) { return a=a+b; }
	forceinline friend u8x16 &operator+=(u8x16 &a, u8 b) { return a=a+b; }
	forceinline friend u8x16 operator-(u8x16 a, u8x16 b) { return {_mm_sub_epi8(a.m, b.m)}; }
	forceinline friend u8x16 operator-(u8x16 a, u8 b) { return a-U8x16(b); }
	forceinline friend u8x16 operator-(u8 a, u8x16 b) { return U8x16(a)-b; }
	forceinline friend u8x16 &operator-=(u8x16 &a, u8x16 b) { return a=a-b; }
	forceinline friend u8x16 &operator-=(u8x16 &a, u8 b) { return a=a-b; }
	forceinline friend u8x16 operator*(u8x16 a, u8x16 b) {a.s[0] *= b.s[0]; a.s[1] *= b.s[1]; a.s[2] *= b.s[2]; a.s[3] *= b.s[3]; a.s[4] *= b.s[4]; a.s[5] *= b.s[5]; a.s[6] *= b.s[6]; a.s[7] *= b.s[7]; a.s[8] *= b.s[8]; a.s[9] *= b.s[9]; a.s[10] *= b.s[10]; a.s[11] *= b.s[11]; a.s[12] *= b.s[12]; a.s[13] *= b.s[13]; a.s[14] *= b.s[14]; a.s[15] *= b.s[15]; return a; }
	forceinline friend u8x16 operator*(u8x16 a, u8 b) {a.s[0] *= b; a.s[1] *= b; a.s[2] *= b; a.s[3] *= b; a.s[4] *= b; a.s[5] *= b; a.s[6] *= b; a.s[7] *= b; a.s[8] *= b; a.s[9] *= b; a.s[10] *= b; a.s[11] *= b; a.s[12] *= b; a.s[13] *= b; a.s[14] *= b; a.s[15] *= b; return a; }
	forceinline friend u8x16 operator*(u8 a, u8x16 b) {auto r=U8x16(a);r.s[0] *= b.s[0]; r.s[1] *= b.s[1]; r.s[2] *= b.s[2]; r.s[3] *= b.s[3]; r.s[4] *= b.s[4]; r.s[5] *= b.s[5]; r.s[6] *= b.s[6]; r.s[7] *= b.s[7]; r.s[8] *= b.s[8]; r.s[9] *= b.s[9]; r.s[10] *= b.s[10]; r.s[11] *= b.s[11]; r.s[12] *= b.s[12]; r.s[13] *= b.s[13]; r.s[14] *= b.s[14]; r.s[15] *= b.s[15]; return r; }
	forceinline friend u8x16 &operator*=(u8x16 &a, u8x16 b) { return a=a*b; }
	forceinline friend u8x16 &operator*=(u8x16 &a, u8 b) { return a=a*b; }
	forceinline friend u8x16 operator/(u8x16 a, u8x16 b) { return {_mm_div_epu8(a.m, b.m)}; }
	forceinline friend u8x16 operator/(u8x16 a, u8 b) { return a/U8x16(b); }
	forceinline friend u8x16 operator/(u8 a, u8x16 b) { return U8x16(a)/b; }
	forceinline friend u8x16 &operator/=(u8x16 &a, u8x16 b) { return a=a/b; }
	forceinline friend u8x16 &operator/=(u8x16 &a, u8 b) { return a=a/b; }
	forceinline friend u8x16 operator^(u8x16 a, u8x16 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u8x16 operator^(u8x16 a, u8 b) { return a^U8x16(b); }
	forceinline friend u8x16 operator^(u8 a, u8x16 b) { return U8x16(a)^b; }
	forceinline friend u8x16 &operator^=(u8x16 &a, u8x16 b) { return a=a^b; }
	forceinline friend u8x16 &operator^=(u8x16 &a, u8 b) { return a=a^b; }
	forceinline friend u8x16 operator&(u8x16 a, u8x16 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u8x16 operator&(u8x16 a, u8 b) { return a&U8x16(b); }
	forceinline friend u8x16 operator&(u8 a, u8x16 b) { return U8x16(a)&b; }
	forceinline friend u8x16 &operator&=(u8x16 &a, u8x16 b) { return a=a&b; }
	forceinline friend u8x16 &operator&=(u8x16 &a, u8 b) { return a=a&b; }
	forceinline friend u8x16 operator|(u8x16 a, u8x16 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u8x16 operator|(u8x16 a, u8 b) { return a|U8x16(b); }
	forceinline friend u8x16 operator|(u8 a, u8x16 b) { return U8x16(a)|b; }
	forceinline friend u8x16 &operator|=(u8x16 &a, u8x16 b) { return a=a|b; }
	forceinline friend u8x16 &operator|=(u8x16 &a, u8 b) { return a=a|b; }
	forceinline friend u8x16 operator<<(u8x16 a, u8x16 b) {a.s[0] <<= b.s[0]; a.s[1] <<= b.s[1]; a.s[2] <<= b.s[2]; a.s[3] <<= b.s[3]; a.s[4] <<= b.s[4]; a.s[5] <<= b.s[5]; a.s[6] <<= b.s[6]; a.s[7] <<= b.s[7]; a.s[8] <<= b.s[8]; a.s[9] <<= b.s[9]; a.s[10] <<= b.s[10]; a.s[11] <<= b.s[11]; a.s[12] <<= b.s[12]; a.s[13] <<= b.s[13]; a.s[14] <<= b.s[14]; a.s[15] <<= b.s[15]; return a; }
	forceinline friend u8x16 operator<<(u8x16 a, u8 b) {a.s[0] <<= b; a.s[1] <<= b; a.s[2] <<= b; a.s[3] <<= b; a.s[4] <<= b; a.s[5] <<= b; a.s[6] <<= b; a.s[7] <<= b; a.s[8] <<= b; a.s[9] <<= b; a.s[10] <<= b; a.s[11] <<= b; a.s[12] <<= b; a.s[13] <<= b; a.s[14] <<= b; a.s[15] <<= b; return a; }
	forceinline friend u8x16 operator<<(u8 a, u8x16 b) {auto r=U8x16(a);r.s[0] <<= b.s[0]; r.s[1] <<= b.s[1]; r.s[2] <<= b.s[2]; r.s[3] <<= b.s[3]; r.s[4] <<= b.s[4]; r.s[5] <<= b.s[5]; r.s[6] <<= b.s[6]; r.s[7] <<= b.s[7]; r.s[8] <<= b.s[8]; r.s[9] <<= b.s[9]; r.s[10] <<= b.s[10]; r.s[11] <<= b.s[11]; r.s[12] <<= b.s[12]; r.s[13] <<= b.s[13]; r.s[14] <<= b.s[14]; r.s[15] <<= b.s[15]; return r; }
	forceinline friend u8x16 &operator<<=(u8x16 &a, u8x16 b) { return a=a<<b; }
	forceinline friend u8x16 &operator<<=(u8x16 &a, u8 b) { return a=a<<b; }
	forceinline friend u8x16 operator>>(u8x16 a, u8x16 b) {a.s[0] >>= b.s[0]; a.s[1] >>= b.s[1]; a.s[2] >>= b.s[2]; a.s[3] >>= b.s[3]; a.s[4] >>= b.s[4]; a.s[5] >>= b.s[5]; a.s[6] >>= b.s[6]; a.s[7] >>= b.s[7]; a.s[8] >>= b.s[8]; a.s[9] >>= b.s[9]; a.s[10] >>= b.s[10]; a.s[11] >>= b.s[11]; a.s[12] >>= b.s[12]; a.s[13] >>= b.s[13]; a.s[14] >>= b.s[14]; a.s[15] >>= b.s[15]; return a; }
	forceinline friend u8x16 operator>>(u8x16 a, u8 b) {a.s[0] >>= b; a.s[1] >>= b; a.s[2] >>= b; a.s[3] >>= b; a.s[4] >>= b; a.s[5] >>= b; a.s[6] >>= b; a.s[7] >>= b; a.s[8] >>= b; a.s[9] >>= b; a.s[10] >>= b; a.s[11] >>= b; a.s[12] >>= b; a.s[13] >>= b; a.s[14] >>= b; a.s[15] >>= b; return a; }
	forceinline friend u8x16 operator>>(u8 a, u8x16 b) {auto r=U8x16(a);r.s[0] >>= b.s[0]; r.s[1] >>= b.s[1]; r.s[2] >>= b.s[2]; r.s[3] >>= b.s[3]; r.s[4] >>= b.s[4]; r.s[5] >>= b.s[5]; r.s[6] >>= b.s[6]; r.s[7] >>= b.s[7]; r.s[8] >>= b.s[8]; r.s[9] >>= b.s[9]; r.s[10] >>= b.s[10]; r.s[11] >>= b.s[11]; r.s[12] >>= b.s[12]; r.s[13] >>= b.s[13]; r.s[14] >>= b.s[14]; r.s[15] >>= b.s[15]; return r; }
	forceinline friend u8x16 &operator>>=(u8x16 &a, u8x16 b) { return a=a>>b; }
	forceinline friend u8x16 &operator>>=(u8x16 &a, u8 b) { return a=a>>b; }
	forceinline operator u16x16() const;
	forceinline operator u32x16() const;
	forceinline operator s8x16() const;
	forceinline operator s16x16() const;
	forceinline operator s32x16() const;
	forceinline operator f32x16() const;
};

forceinline u8x16 U8x16(u8 a) { return {_mm_set1_epi8(a)}; }
forceinline u8x16 U8x16(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15) { return {_mm_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
forceinline u16x8 U16x8(u16);
forceinline u16x8 U16x8(u16,u16,u16,u16,u16,u16,u16,u16);

union u16x8 {
	__m128i m;
	u16 s[8];
	forceinline u16 operator[](umm i) const { return s[i]; }
	forceinline u16 &operator[](umm i) { return s[i]; }
	forceinline friend u16x8 operator+(u16x8 a) { return a; }
	forceinline friend u16x8 operator-(u16x8 a) { return u16x8{} - a; }
	forceinline friend u16x8 operator~(u16x8 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u16x8 operator+(u16x8 a, u16x8 b) { return {_mm_add_epi16(a.m, b.m)}; }
	forceinline friend u16x8 operator+(u16x8 a, u16 b) { return a+U16x8(b); }
	forceinline friend u16x8 operator+(u16 a, u16x8 b) { return U16x8(a)+b; }
	forceinline friend u16x8 &operator+=(u16x8 &a, u16x8 b) { return a=a+b; }
	forceinline friend u16x8 &operator+=(u16x8 &a, u16 b) { return a=a+b; }
	forceinline friend u16x8 operator-(u16x8 a, u16x8 b) { return {_mm_sub_epi16(a.m, b.m)}; }
	forceinline friend u16x8 operator-(u16x8 a, u16 b) { return a-U16x8(b); }
	forceinline friend u16x8 operator-(u16 a, u16x8 b) { return U16x8(a)-b; }
	forceinline friend u16x8 &operator-=(u16x8 &a, u16x8 b) { return a=a-b; }
	forceinline friend u16x8 &operator-=(u16x8 &a, u16 b) { return a=a-b; }
	forceinline friend u16x8 operator*(u16x8 a, u16x8 b) { return {_mm_mullo_epi16(a.m, b.m)}; }
	forceinline friend u16x8 operator*(u16x8 a, u16 b) { return a*U16x8(b); }
	forceinline friend u16x8 operator*(u16 a, u16x8 b) { return U16x8(a)*b; }
	forceinline friend u16x8 &operator*=(u16x8 &a, u16x8 b) { return a=a*b; }
	forceinline friend u16x8 &operator*=(u16x8 &a, u16 b) { return a=a*b; }
	forceinline friend u16x8 operator/(u16x8 a, u16x8 b) { return {_mm_div_epu16(a.m, b.m)}; }
	forceinline friend u16x8 operator/(u16x8 a, u16 b) { return a/U16x8(b); }
	forceinline friend u16x8 operator/(u16 a, u16x8 b) { return U16x8(a)/b; }
	forceinline friend u16x8 &operator/=(u16x8 &a, u16x8 b) { return a=a/b; }
	forceinline friend u16x8 &operator/=(u16x8 &a, u16 b) { return a=a/b; }
	forceinline friend u16x8 operator^(u16x8 a, u16x8 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u16x8 operator^(u16x8 a, u16 b) { return a^U16x8(b); }
	forceinline friend u16x8 operator^(u16 a, u16x8 b) { return U16x8(a)^b; }
	forceinline friend u16x8 &operator^=(u16x8 &a, u16x8 b) { return a=a^b; }
	forceinline friend u16x8 &operator^=(u16x8 &a, u16 b) { return a=a^b; }
	forceinline friend u16x8 operator&(u16x8 a, u16x8 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u16x8 operator&(u16x8 a, u16 b) { return a&U16x8(b); }
	forceinline friend u16x8 operator&(u16 a, u16x8 b) { return U16x8(a)&b; }
	forceinline friend u16x8 &operator&=(u16x8 &a, u16x8 b) { return a=a&b; }
	forceinline friend u16x8 &operator&=(u16x8 &a, u16 b) { return a=a&b; }
	forceinline friend u16x8 operator|(u16x8 a, u16x8 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u16x8 operator|(u16x8 a, u16 b) { return a|U16x8(b); }
	forceinline friend u16x8 operator|(u16 a, u16x8 b) { return U16x8(a)|b; }
	forceinline friend u16x8 &operator|=(u16x8 &a, u16x8 b) { return a=a|b; }
	forceinline friend u16x8 &operator|=(u16x8 &a, u16 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend u16x8 operator<<(u16x8 a, u16x8 b) { return {_mm_sllv_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x8 operator<<(u16x8 a, u16x8 b) {a.s[0]<<=b.s[0];a.s[1]<<=b.s[1];a.s[2]<<=b.s[2];a.s[3]<<=b.s[3];a.s[4]<<=b.s[4];a.s[5]<<=b.s[5];a.s[6]<<=b.s[6];a.s[7]<<=b.s[7];return a; }
#endif
	forceinline friend u16x8 operator<<(u16x8 a, u16 b) { return {_mm_slli_epi16(a.m, b)}; }
	forceinline friend u16x8 operator<<(u16 a, u16x8 b) { return U16x8(a)<<b; }
	forceinline friend u16x8 &operator<<=(u16x8 &a, u16x8 b) { return a=a<<b; }
	forceinline friend u16x8 &operator<<=(u16x8 &a, u16 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend u16x8 operator>>(u16x8 a, u16x8 b) { return {_mm_srlv_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x8 operator>>(u16x8 a, u16x8 b) {a.s[0]>>=b.s[0];a.s[1]>>=b.s[1];a.s[2]>>=b.s[2];a.s[3]>>=b.s[3];a.s[4]>>=b.s[4];a.s[5]>>=b.s[5];a.s[6]>>=b.s[6];a.s[7]>>=b.s[7];return a; }
#endif
	forceinline friend u16x8 operator>>(u16x8 a, u16 b) { return {_mm_srli_epi16(a.m, b)}; }
	forceinline friend u16x8 operator>>(u16 a, u16x8 b) { return U16x8(a)>>b; }
	forceinline friend u16x8 &operator>>=(u16x8 &a, u16x8 b) { return a=a>>b; }
	forceinline friend u16x8 &operator>>=(u16x8 &a, u16 b) { return a=a>>b; }
	forceinline operator u32x8() const;
	forceinline operator u64x8() const;
	forceinline operator s16x8() const;
	forceinline operator s32x8() const;
	forceinline operator s64x8() const;
	forceinline operator f32x8() const;
	forceinline operator f64x8() const;
};

forceinline u16x8 U16x8(u16 a) { return {_mm_set1_epi16(a)}; }
forceinline u16x8 U16x8(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7) { return {_mm_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7)}; }
forceinline u32x4 U32x4(u32);
forceinline u32x4 U32x4(u32,u32,u32,u32);

union u32x4 {
	__m128i m;
	u32 s[4];
	forceinline u32 operator[](umm i) const { return s[i]; }
	forceinline u32 &operator[](umm i) { return s[i]; }
	forceinline friend u32x4 operator+(u32x4 a) { return a; }
	forceinline friend u32x4 operator-(u32x4 a) { return u32x4{} - a; }
	forceinline friend u32x4 operator~(u32x4 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u32x4 operator+(u32x4 a, u32x4 b) { return {_mm_add_epi32(a.m, b.m)}; }
	forceinline friend u32x4 operator+(u32x4 a, u32 b) { return a+U32x4(b); }
	forceinline friend u32x4 operator+(u32 a, u32x4 b) { return U32x4(a)+b; }
	forceinline friend u32x4 &operator+=(u32x4 &a, u32x4 b) { return a=a+b; }
	forceinline friend u32x4 &operator+=(u32x4 &a, u32 b) { return a=a+b; }
	forceinline friend u32x4 operator-(u32x4 a, u32x4 b) { return {_mm_sub_epi32(a.m, b.m)}; }
	forceinline friend u32x4 operator-(u32x4 a, u32 b) { return a-U32x4(b); }
	forceinline friend u32x4 operator-(u32 a, u32x4 b) { return U32x4(a)-b; }
	forceinline friend u32x4 &operator-=(u32x4 &a, u32x4 b) { return a=a-b; }
	forceinline friend u32x4 &operator-=(u32x4 &a, u32 b) { return a=a-b; }
	forceinline friend u32x4 operator*(u32x4 a, u32x4 b) { return {_mm_mullo_epi32(a.m, b.m)}; }
	forceinline friend u32x4 operator*(u32x4 a, u32 b) { return a*U32x4(b); }
	forceinline friend u32x4 operator*(u32 a, u32x4 b) { return U32x4(a)*b; }
	forceinline friend u32x4 &operator*=(u32x4 &a, u32x4 b) { return a=a*b; }
	forceinline friend u32x4 &operator*=(u32x4 &a, u32 b) { return a=a*b; }
	forceinline friend u32x4 operator/(u32x4 a, u32x4 b) { return {_mm_div_epu32(a.m, b.m)}; }
	forceinline friend u32x4 operator/(u32x4 a, u32 b) { return a/U32x4(b); }
	forceinline friend u32x4 operator/(u32 a, u32x4 b) { return U32x4(a)/b; }
	forceinline friend u32x4 &operator/=(u32x4 &a, u32x4 b) { return a=a/b; }
	forceinline friend u32x4 &operator/=(u32x4 &a, u32 b) { return a=a/b; }
	forceinline friend u32x4 operator^(u32x4 a, u32x4 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u32x4 operator^(u32x4 a, u32 b) { return a^U32x4(b); }
	forceinline friend u32x4 operator^(u32 a, u32x4 b) { return U32x4(a)^b; }
	forceinline friend u32x4 &operator^=(u32x4 &a, u32x4 b) { return a=a^b; }
	forceinline friend u32x4 &operator^=(u32x4 &a, u32 b) { return a=a^b; }
	forceinline friend u32x4 operator&(u32x4 a, u32x4 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u32x4 operator&(u32x4 a, u32 b) { return a&U32x4(b); }
	forceinline friend u32x4 operator&(u32 a, u32x4 b) { return U32x4(a)&b; }
	forceinline friend u32x4 &operator&=(u32x4 &a, u32x4 b) { return a=a&b; }
	forceinline friend u32x4 &operator&=(u32x4 &a, u32 b) { return a=a&b; }
	forceinline friend u32x4 operator|(u32x4 a, u32x4 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u32x4 operator|(u32x4 a, u32 b) { return a|U32x4(b); }
	forceinline friend u32x4 operator|(u32 a, u32x4 b) { return U32x4(a)|b; }
	forceinline friend u32x4 &operator|=(u32x4 &a, u32x4 b) { return a=a|b; }
	forceinline friend u32x4 &operator|=(u32x4 &a, u32 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend u32x4 operator<<(u32x4 a, u32x4 b) { return {_mm_sllv_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x4 operator<<(u32x4 a, u32x4 b) {a.s[0]<<=b.s[0];a.s[1]<<=b.s[1];a.s[2]<<=b.s[2];a.s[3]<<=b.s[3];return a; }
#endif
	forceinline friend u32x4 operator<<(u32x4 a, u32 b) { return {_mm_slli_epi32(a.m, b)}; }
	forceinline friend u32x4 operator<<(u32 a, u32x4 b) { return U32x4(a)<<b; }
	forceinline friend u32x4 &operator<<=(u32x4 &a, u32x4 b) { return a=a<<b; }
	forceinline friend u32x4 &operator<<=(u32x4 &a, u32 b) { return a=a<<b; }
#if ARCH_AVX2
	forceinline friend u32x4 operator>>(u32x4 a, u32x4 b) { return {_mm_srlv_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x4 operator>>(u32x4 a, u32x4 b) {a.s[0]>>=b.s[0];a.s[1]>>=b.s[1];a.s[2]>>=b.s[2];a.s[3]>>=b.s[3];return a; }
#endif
	forceinline friend u32x4 operator>>(u32x4 a, u32 b) { return {_mm_srli_epi32(a.m, b)}; }
	forceinline friend u32x4 operator>>(u32 a, u32x4 b) { return U32x4(a)>>b; }
	forceinline friend u32x4 &operator>>=(u32x4 &a, u32x4 b) { return a=a>>b; }
	forceinline friend u32x4 &operator>>=(u32x4 &a, u32 b) { return a=a>>b; }
	forceinline operator u64x4() const;
	forceinline operator s32x4() const;
	forceinline operator s64x4() const;
	forceinline operator f32x4() const;
	forceinline operator f64x4() const;
};

forceinline u32x4 U32x4(u32 a) { return {_mm_set1_epi32(a)}; }
forceinline u32x4 U32x4(u32 _0,u32 _1,u32 _2,u32 _3) { return {_mm_setr_epi32(_0,_1,_2,_3)}; }
forceinline u64x2 U64x2(u64);
forceinline u64x2 U64x2(u64,u64);

union u64x2 {
	__m128i m;
	u64 s[2];
	forceinline u64 operator[](umm i) const { return s[i]; }
	forceinline u64 &operator[](umm i) { return s[i]; }
	forceinline friend u64x2 operator+(u64x2 a) { return a; }
	forceinline friend u64x2 operator-(u64x2 a) { return u64x2{} - a; }
	forceinline friend u64x2 operator~(u64x2 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend u64x2 operator+(u64x2 a, u64x2 b) { return {_mm_add_epi64(a.m, b.m)}; }
	forceinline friend u64x2 operator+(u64x2 a, u64 b) { return a+U64x2(b); }
	forceinline friend u64x2 operator+(u64 a, u64x2 b) { return U64x2(a)+b; }
	forceinline friend u64x2 &operator+=(u64x2 &a, u64x2 b) { return a=a+b; }
	forceinline friend u64x2 &operator+=(u64x2 &a, u64 b) { return a=a+b; }
	forceinline friend u64x2 operator-(u64x2 a, u64x2 b) { return {_mm_sub_epi64(a.m, b.m)}; }
	forceinline friend u64x2 operator-(u64x2 a, u64 b) { return a-U64x2(b); }
	forceinline friend u64x2 operator-(u64 a, u64x2 b) { return U64x2(a)-b; }
	forceinline friend u64x2 &operator-=(u64x2 &a, u64x2 b) { return a=a-b; }
	forceinline friend u64x2 &operator-=(u64x2 &a, u64 b) { return a=a-b; }
	forceinline friend u64x2 operator*(u64x2 a, u64x2 b) { return {_mm_mullo_epi64(a.m, b.m)}; }
	forceinline friend u64x2 operator*(u64x2 a, u64 b) { return a*U64x2(b); }
	forceinline friend u64x2 operator*(u64 a, u64x2 b) { return U64x2(a)*b; }
	forceinline friend u64x2 &operator*=(u64x2 &a, u64x2 b) { return a=a*b; }
	forceinline friend u64x2 &operator*=(u64x2 &a, u64 b) { return a=a*b; }
	forceinline friend u64x2 operator/(u64x2 a, u64x2 b) { return {_mm_div_epu64(a.m, b.m)}; }
	forceinline friend u64x2 operator/(u64x2 a, u64 b) { return a/U64x2(b); }
	forceinline friend u64x2 operator/(u64 a, u64x2 b) { return U64x2(a)/b; }
	forceinline friend u64x2 &operator/=(u64x2 &a, u64x2 b) { return a=a/b; }
	forceinline friend u64x2 &operator/=(u64x2 &a, u64 b) { return a=a/b; }
	forceinline friend u64x2 operator^(u64x2 a, u64x2 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend u64x2 operator^(u64x2 a, u64 b) { return a^U64x2(b); }
	forceinline friend u64x2 operator^(u64 a, u64x2 b) { return U64x2(a)^b; }
	forceinline friend u64x2 &operator^=(u64x2 &a, u64x2 b) { return a=a^b; }
	forceinline friend u64x2 &operator^=(u64x2 &a, u64 b) { return a=a^b; }
	forceinline friend u64x2 operator&(u64x2 a, u64x2 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend u64x2 operator&(u64x2 a, u64 b) { return a&U64x2(b); }
	forceinline friend u64x2 operator&(u64 a, u64x2 b) { return U64x2(a)&b; }
	forceinline friend u64x2 &operator&=(u64x2 &a, u64x2 b) { return a=a&b; }
	forceinline friend u64x2 &operator&=(u64x2 &a, u64 b) { return a=a&b; }
	forceinline friend u64x2 operator|(u64x2 a, u64x2 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend u64x2 operator|(u64x2 a, u64 b) { return a|U64x2(b); }
	forceinline friend u64x2 operator|(u64 a, u64x2 b) { return U64x2(a)|b; }
	forceinline friend u64x2 &operator|=(u64x2 &a, u64x2 b) { return a=a|b; }
	forceinline friend u64x2 &operator|=(u64x2 &a, u64 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend u64x2 operator<<(u64x2 a, u64x2 b) { return {_mm_sllv_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x2 operator<<(u64x2 a, u64x2 b) {a.s[0]<<=b.s[0];a.s[1]<<=b.s[1];return a; }
#endif
	forceinline friend u64x2 operator<<(u64x2 a, u64 b) { return {_mm_slli_epi64(a.m, b)}; }
	forceinline friend u64x2 operator<<(u64 a, u64x2 b) { return U64x2(a)<<b; }
	forceinline friend u64x2 &operator<<=(u64x2 &a, u64x2 b) { return a=a<<b; }
	forceinline friend u64x2 &operator<<=(u64x2 &a, u64 b) { return a=a<<b; }
#if ARCH_AVX2
	forceinline friend u64x2 operator>>(u64x2 a, u64x2 b) { return {_mm_srlv_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x2 operator>>(u64x2 a, u64x2 b) {a.s[0]>>=b.s[0];a.s[1]>>=b.s[1];return a; }
#endif
	forceinline friend u64x2 operator>>(u64x2 a, u64 b) { return {_mm_srli_epi64(a.m, b)}; }
	forceinline friend u64x2 operator>>(u64 a, u64x2 b) { return U64x2(a)>>b; }
	forceinline friend u64x2 &operator>>=(u64x2 &a, u64x2 b) { return a=a>>b; }
	forceinline friend u64x2 &operator>>=(u64x2 &a, u64 b) { return a=a>>b; }
	forceinline operator s64x2() const;
	forceinline operator f64x2() const;
};

forceinline u64x2 U64x2(u64 a) { return {_mm_set1_epi64x(a)}; }
forceinline u64x2 U64x2(u64 _0,u64 _1) { return {_mm_setr_epi64x(_0,_1)}; }
forceinline s8x16 S8x16(s8);
forceinline s8x16 S8x16(s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8);

union s8x16 {
	__m128i m;
	s8 s[16];
	forceinline s8 operator[](umm i) const { return s[i]; }
	forceinline s8 &operator[](umm i) { return s[i]; }
	forceinline friend s8x16 operator+(s8x16 a) { return a; }
	forceinline friend s8x16 operator-(s8x16 a) { return s8x16{} - a; }
	forceinline friend s8x16 operator~(s8x16 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s8x16 operator+(s8x16 a, s8x16 b) { return {_mm_add_epi8(a.m, b.m)}; }
	forceinline friend s8x16 operator+(s8x16 a, s8 b) { return a+S8x16(b); }
	forceinline friend s8x16 operator+(s8 a, s8x16 b) { return S8x16(a)+b; }
	forceinline friend s8x16 &operator+=(s8x16 &a, s8x16 b) { return a=a+b; }
	forceinline friend s8x16 &operator+=(s8x16 &a, s8 b) { return a=a+b; }
	forceinline friend s8x16 operator-(s8x16 a, s8x16 b) { return {_mm_sub_epi8(a.m, b.m)}; }
	forceinline friend s8x16 operator-(s8x16 a, s8 b) { return a-S8x16(b); }
	forceinline friend s8x16 operator-(s8 a, s8x16 b) { return S8x16(a)-b; }
	forceinline friend s8x16 &operator-=(s8x16 &a, s8x16 b) { return a=a-b; }
	forceinline friend s8x16 &operator-=(s8x16 &a, s8 b) { return a=a-b; }
	forceinline friend s8x16 operator*(s8x16 a, s8x16 b) {a.s[0] *= b.s[0]; a.s[1] *= b.s[1]; a.s[2] *= b.s[2]; a.s[3] *= b.s[3]; a.s[4] *= b.s[4]; a.s[5] *= b.s[5]; a.s[6] *= b.s[6]; a.s[7] *= b.s[7]; a.s[8] *= b.s[8]; a.s[9] *= b.s[9]; a.s[10] *= b.s[10]; a.s[11] *= b.s[11]; a.s[12] *= b.s[12]; a.s[13] *= b.s[13]; a.s[14] *= b.s[14]; a.s[15] *= b.s[15]; return a; }
	forceinline friend s8x16 operator*(s8x16 a, s8 b) {a.s[0] *= b; a.s[1] *= b; a.s[2] *= b; a.s[3] *= b; a.s[4] *= b; a.s[5] *= b; a.s[6] *= b; a.s[7] *= b; a.s[8] *= b; a.s[9] *= b; a.s[10] *= b; a.s[11] *= b; a.s[12] *= b; a.s[13] *= b; a.s[14] *= b; a.s[15] *= b; return a; }
	forceinline friend s8x16 operator*(s8 a, s8x16 b) {auto r=S8x16(a);r.s[0] *= b.s[0]; r.s[1] *= b.s[1]; r.s[2] *= b.s[2]; r.s[3] *= b.s[3]; r.s[4] *= b.s[4]; r.s[5] *= b.s[5]; r.s[6] *= b.s[6]; r.s[7] *= b.s[7]; r.s[8] *= b.s[8]; r.s[9] *= b.s[9]; r.s[10] *= b.s[10]; r.s[11] *= b.s[11]; r.s[12] *= b.s[12]; r.s[13] *= b.s[13]; r.s[14] *= b.s[14]; r.s[15] *= b.s[15]; return r; }
	forceinline friend s8x16 &operator*=(s8x16 &a, s8x16 b) { return a=a*b; }
	forceinline friend s8x16 &operator*=(s8x16 &a, s8 b) { return a=a*b; }
	forceinline friend s8x16 operator/(s8x16 a, s8x16 b) { return {_mm_div_epi8(a.m, b.m)}; }
	forceinline friend s8x16 operator/(s8x16 a, s8 b) { return a/S8x16(b); }
	forceinline friend s8x16 operator/(s8 a, s8x16 b) { return S8x16(a)/b; }
	forceinline friend s8x16 &operator/=(s8x16 &a, s8x16 b) { return a=a/b; }
	forceinline friend s8x16 &operator/=(s8x16 &a, s8 b) { return a=a/b; }
	forceinline friend s8x16 operator^(s8x16 a, s8x16 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s8x16 operator^(s8x16 a, s8 b) { return a^S8x16(b); }
	forceinline friend s8x16 operator^(s8 a, s8x16 b) { return S8x16(a)^b; }
	forceinline friend s8x16 &operator^=(s8x16 &a, s8x16 b) { return a=a^b; }
	forceinline friend s8x16 &operator^=(s8x16 &a, s8 b) { return a=a^b; }
	forceinline friend s8x16 operator&(s8x16 a, s8x16 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s8x16 operator&(s8x16 a, s8 b) { return a&S8x16(b); }
	forceinline friend s8x16 operator&(s8 a, s8x16 b) { return S8x16(a)&b; }
	forceinline friend s8x16 &operator&=(s8x16 &a, s8x16 b) { return a=a&b; }
	forceinline friend s8x16 &operator&=(s8x16 &a, s8 b) { return a=a&b; }
	forceinline friend s8x16 operator|(s8x16 a, s8x16 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s8x16 operator|(s8x16 a, s8 b) { return a|S8x16(b); }
	forceinline friend s8x16 operator|(s8 a, s8x16 b) { return S8x16(a)|b; }
	forceinline friend s8x16 &operator|=(s8x16 &a, s8x16 b) { return a=a|b; }
	forceinline friend s8x16 &operator|=(s8x16 &a, s8 b) { return a=a|b; }
	forceinline friend s8x16 operator<<(s8x16 a, s8x16 b) {a.s[0] <<= b.s[0]; a.s[1] <<= b.s[1]; a.s[2] <<= b.s[2]; a.s[3] <<= b.s[3]; a.s[4] <<= b.s[4]; a.s[5] <<= b.s[5]; a.s[6] <<= b.s[6]; a.s[7] <<= b.s[7]; a.s[8] <<= b.s[8]; a.s[9] <<= b.s[9]; a.s[10] <<= b.s[10]; a.s[11] <<= b.s[11]; a.s[12] <<= b.s[12]; a.s[13] <<= b.s[13]; a.s[14] <<= b.s[14]; a.s[15] <<= b.s[15]; return a; }
	forceinline friend s8x16 operator<<(s8x16 a, s8 b) {a.s[0] <<= b; a.s[1] <<= b; a.s[2] <<= b; a.s[3] <<= b; a.s[4] <<= b; a.s[5] <<= b; a.s[6] <<= b; a.s[7] <<= b; a.s[8] <<= b; a.s[9] <<= b; a.s[10] <<= b; a.s[11] <<= b; a.s[12] <<= b; a.s[13] <<= b; a.s[14] <<= b; a.s[15] <<= b; return a; }
	forceinline friend s8x16 operator<<(s8 a, s8x16 b) {auto r=S8x16(a);r.s[0] <<= b.s[0]; r.s[1] <<= b.s[1]; r.s[2] <<= b.s[2]; r.s[3] <<= b.s[3]; r.s[4] <<= b.s[4]; r.s[5] <<= b.s[5]; r.s[6] <<= b.s[6]; r.s[7] <<= b.s[7]; r.s[8] <<= b.s[8]; r.s[9] <<= b.s[9]; r.s[10] <<= b.s[10]; r.s[11] <<= b.s[11]; r.s[12] <<= b.s[12]; r.s[13] <<= b.s[13]; r.s[14] <<= b.s[14]; r.s[15] <<= b.s[15]; return r; }
	forceinline friend s8x16 &operator<<=(s8x16 &a, s8x16 b) { return a=a<<b; }
	forceinline friend s8x16 &operator<<=(s8x16 &a, s8 b) { return a=a<<b; }
	forceinline friend s8x16 operator>>(s8x16 a, s8x16 b) {a.s[0] >>= b.s[0]; a.s[1] >>= b.s[1]; a.s[2] >>= b.s[2]; a.s[3] >>= b.s[3]; a.s[4] >>= b.s[4]; a.s[5] >>= b.s[5]; a.s[6] >>= b.s[6]; a.s[7] >>= b.s[7]; a.s[8] >>= b.s[8]; a.s[9] >>= b.s[9]; a.s[10] >>= b.s[10]; a.s[11] >>= b.s[11]; a.s[12] >>= b.s[12]; a.s[13] >>= b.s[13]; a.s[14] >>= b.s[14]; a.s[15] >>= b.s[15]; return a; }
	forceinline friend s8x16 operator>>(s8x16 a, s8 b) {a.s[0] >>= b; a.s[1] >>= b; a.s[2] >>= b; a.s[3] >>= b; a.s[4] >>= b; a.s[5] >>= b; a.s[6] >>= b; a.s[7] >>= b; a.s[8] >>= b; a.s[9] >>= b; a.s[10] >>= b; a.s[11] >>= b; a.s[12] >>= b; a.s[13] >>= b; a.s[14] >>= b; a.s[15] >>= b; return a; }
	forceinline friend s8x16 operator>>(s8 a, s8x16 b) {auto r=S8x16(a);r.s[0] >>= b.s[0]; r.s[1] >>= b.s[1]; r.s[2] >>= b.s[2]; r.s[3] >>= b.s[3]; r.s[4] >>= b.s[4]; r.s[5] >>= b.s[5]; r.s[6] >>= b.s[6]; r.s[7] >>= b.s[7]; r.s[8] >>= b.s[8]; r.s[9] >>= b.s[9]; r.s[10] >>= b.s[10]; r.s[11] >>= b.s[11]; r.s[12] >>= b.s[12]; r.s[13] >>= b.s[13]; r.s[14] >>= b.s[14]; r.s[15] >>= b.s[15]; return r; }
	forceinline friend s8x16 &operator>>=(s8x16 &a, s8x16 b) { return a=a>>b; }
	forceinline friend s8x16 &operator>>=(s8x16 &a, s8 b) { return a=a>>b; }
	forceinline operator u8x16() const;
	forceinline operator u16x16() const;
	forceinline operator u32x16() const;
	forceinline operator s16x16() const;
	forceinline operator s32x16() const;
	forceinline operator f32x16() const;
};

forceinline s8x16 S8x16(s8 a) { return {_mm_set1_epi8(a)}; }
forceinline s8x16 S8x16(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15) { return {_mm_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
forceinline s16x8 S16x8(s16);
forceinline s16x8 S16x8(s16,s16,s16,s16,s16,s16,s16,s16);

union s16x8 {
	__m128i m;
	s16 s[8];
	forceinline s16 operator[](umm i) const { return s[i]; }
	forceinline s16 &operator[](umm i) { return s[i]; }
	forceinline friend s16x8 operator+(s16x8 a) { return a; }
	forceinline friend s16x8 operator-(s16x8 a) { return s16x8{} - a; }
	forceinline friend s16x8 operator~(s16x8 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s16x8 operator+(s16x8 a, s16x8 b) { return {_mm_add_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator+(s16x8 a, s16 b) { return a+S16x8(b); }
	forceinline friend s16x8 operator+(s16 a, s16x8 b) { return S16x8(a)+b; }
	forceinline friend s16x8 &operator+=(s16x8 &a, s16x8 b) { return a=a+b; }
	forceinline friend s16x8 &operator+=(s16x8 &a, s16 b) { return a=a+b; }
	forceinline friend s16x8 operator-(s16x8 a, s16x8 b) { return {_mm_sub_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator-(s16x8 a, s16 b) { return a-S16x8(b); }
	forceinline friend s16x8 operator-(s16 a, s16x8 b) { return S16x8(a)-b; }
	forceinline friend s16x8 &operator-=(s16x8 &a, s16x8 b) { return a=a-b; }
	forceinline friend s16x8 &operator-=(s16x8 &a, s16 b) { return a=a-b; }
	forceinline friend s16x8 operator*(s16x8 a, s16x8 b) { return {_mm_mullo_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator*(s16x8 a, s16 b) { return a*S16x8(b); }
	forceinline friend s16x8 operator*(s16 a, s16x8 b) { return S16x8(a)*b; }
	forceinline friend s16x8 &operator*=(s16x8 &a, s16x8 b) { return a=a*b; }
	forceinline friend s16x8 &operator*=(s16x8 &a, s16 b) { return a=a*b; }
	forceinline friend s16x8 operator/(s16x8 a, s16x8 b) { return {_mm_div_epi16(a.m, b.m)}; }
	forceinline friend s16x8 operator/(s16x8 a, s16 b) { return a/S16x8(b); }
	forceinline friend s16x8 operator/(s16 a, s16x8 b) { return S16x8(a)/b; }
	forceinline friend s16x8 &operator/=(s16x8 &a, s16x8 b) { return a=a/b; }
	forceinline friend s16x8 &operator/=(s16x8 &a, s16 b) { return a=a/b; }
	forceinline friend s16x8 operator^(s16x8 a, s16x8 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s16x8 operator^(s16x8 a, s16 b) { return a^S16x8(b); }
	forceinline friend s16x8 operator^(s16 a, s16x8 b) { return S16x8(a)^b; }
	forceinline friend s16x8 &operator^=(s16x8 &a, s16x8 b) { return a=a^b; }
	forceinline friend s16x8 &operator^=(s16x8 &a, s16 b) { return a=a^b; }
	forceinline friend s16x8 operator&(s16x8 a, s16x8 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s16x8 operator&(s16x8 a, s16 b) { return a&S16x8(b); }
	forceinline friend s16x8 operator&(s16 a, s16x8 b) { return S16x8(a)&b; }
	forceinline friend s16x8 &operator&=(s16x8 &a, s16x8 b) { return a=a&b; }
	forceinline friend s16x8 &operator&=(s16x8 &a, s16 b) { return a=a&b; }
	forceinline friend s16x8 operator|(s16x8 a, s16x8 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s16x8 operator|(s16x8 a, s16 b) { return a|S16x8(b); }
	forceinline friend s16x8 operator|(s16 a, s16x8 b) { return S16x8(a)|b; }
	forceinline friend s16x8 &operator|=(s16x8 &a, s16x8 b) { return a=a|b; }
	forceinline friend s16x8 &operator|=(s16x8 &a, s16 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend s16x8 operator<<(s16x8 a, s16x8 b) { return {_mm_sllv_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x8 operator<<(s16x8 a, s16x8 b) {a.s[0]<<=b.s[0];a.s[1]<<=b.s[1];a.s[2]<<=b.s[2];a.s[3]<<=b.s[3];a.s[4]<<=b.s[4];a.s[5]<<=b.s[5];a.s[6]<<=b.s[6];a.s[7]<<=b.s[7];return a; }
#endif
	forceinline friend s16x8 operator<<(s16x8 a, s16 b) { return {_mm_slli_epi16(a.m, b)}; }
	forceinline friend s16x8 operator<<(s16 a, s16x8 b) { return S16x8(a)<<b; }
	forceinline friend s16x8 &operator<<=(s16x8 &a, s16x8 b) { return a=a<<b; }
	forceinline friend s16x8 &operator<<=(s16x8 &a, s16 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s16x8 operator>>(s16x8 a, s16x8 b) { return {_mm_srav_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x8 operator>>(s16x8 a, s16x8 b) {a.s[0]>>=b.s[0];a.s[1]>>=b.s[1];a.s[2]>>=b.s[2];a.s[3]>>=b.s[3];a.s[4]>>=b.s[4];a.s[5]>>=b.s[5];a.s[6]>>=b.s[6];a.s[7]>>=b.s[7];return a; }
#endif
	forceinline friend s16x8 operator>>(s16x8 a, s16 b) { return {_mm_srai_epi16(a.m, b)}; }
	forceinline friend s16x8 operator>>(s16 a, s16x8 b) { return S16x8(a)>>b; }
	forceinline friend s16x8 &operator>>=(s16x8 &a, s16x8 b) { return a=a>>b; }
	forceinline friend s16x8 &operator>>=(s16x8 &a, s16 b) { return a=a>>b; }
	forceinline operator u16x8() const;
	forceinline operator u32x8() const;
	forceinline operator u64x8() const;
	forceinline operator s32x8() const;
	forceinline operator s64x8() const;
	forceinline operator f32x8() const;
	forceinline operator f64x8() const;
};

forceinline s16x8 S16x8(s16 a) { return {_mm_set1_epi16(a)}; }
forceinline s16x8 S16x8(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7) { return {_mm_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7)}; }
forceinline s32x4 S32x4(s32);
forceinline s32x4 S32x4(s32,s32,s32,s32);

union s32x4 {
	__m128i m;
	s32 s[4];
	forceinline s32 operator[](umm i) const { return s[i]; }
	forceinline s32 &operator[](umm i) { return s[i]; }
	forceinline friend s32x4 operator+(s32x4 a) { return a; }
	forceinline friend s32x4 operator-(s32x4 a) { return s32x4{} - a; }
	forceinline friend s32x4 operator~(s32x4 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s32x4 operator+(s32x4 a, s32x4 b) { return {_mm_add_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator+(s32x4 a, s32 b) { return a+S32x4(b); }
	forceinline friend s32x4 operator+(s32 a, s32x4 b) { return S32x4(a)+b; }
	forceinline friend s32x4 &operator+=(s32x4 &a, s32x4 b) { return a=a+b; }
	forceinline friend s32x4 &operator+=(s32x4 &a, s32 b) { return a=a+b; }
	forceinline friend s32x4 operator-(s32x4 a, s32x4 b) { return {_mm_sub_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator-(s32x4 a, s32 b) { return a-S32x4(b); }
	forceinline friend s32x4 operator-(s32 a, s32x4 b) { return S32x4(a)-b; }
	forceinline friend s32x4 &operator-=(s32x4 &a, s32x4 b) { return a=a-b; }
	forceinline friend s32x4 &operator-=(s32x4 &a, s32 b) { return a=a-b; }
	forceinline friend s32x4 operator*(s32x4 a, s32x4 b) { return {_mm_mullo_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator*(s32x4 a, s32 b) { return a*S32x4(b); }
	forceinline friend s32x4 operator*(s32 a, s32x4 b) { return S32x4(a)*b; }
	forceinline friend s32x4 &operator*=(s32x4 &a, s32x4 b) { return a=a*b; }
	forceinline friend s32x4 &operator*=(s32x4 &a, s32 b) { return a=a*b; }
	forceinline friend s32x4 operator/(s32x4 a, s32x4 b) { return {_mm_div_epi32(a.m, b.m)}; }
	forceinline friend s32x4 operator/(s32x4 a, s32 b) { return a/S32x4(b); }
	forceinline friend s32x4 operator/(s32 a, s32x4 b) { return S32x4(a)/b; }
	forceinline friend s32x4 &operator/=(s32x4 &a, s32x4 b) { return a=a/b; }
	forceinline friend s32x4 &operator/=(s32x4 &a, s32 b) { return a=a/b; }
	forceinline friend s32x4 operator^(s32x4 a, s32x4 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s32x4 operator^(s32x4 a, s32 b) { return a^S32x4(b); }
	forceinline friend s32x4 operator^(s32 a, s32x4 b) { return S32x4(a)^b; }
	forceinline friend s32x4 &operator^=(s32x4 &a, s32x4 b) { return a=a^b; }
	forceinline friend s32x4 &operator^=(s32x4 &a, s32 b) { return a=a^b; }
	forceinline friend s32x4 operator&(s32x4 a, s32x4 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s32x4 operator&(s32x4 a, s32 b) { return a&S32x4(b); }
	forceinline friend s32x4 operator&(s32 a, s32x4 b) { return S32x4(a)&b; }
	forceinline friend s32x4 &operator&=(s32x4 &a, s32x4 b) { return a=a&b; }
	forceinline friend s32x4 &operator&=(s32x4 &a, s32 b) { return a=a&b; }
	forceinline friend s32x4 operator|(s32x4 a, s32x4 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s32x4 operator|(s32x4 a, s32 b) { return a|S32x4(b); }
	forceinline friend s32x4 operator|(s32 a, s32x4 b) { return S32x4(a)|b; }
	forceinline friend s32x4 &operator|=(s32x4 &a, s32x4 b) { return a=a|b; }
	forceinline friend s32x4 &operator|=(s32x4 &a, s32 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend s32x4 operator<<(s32x4 a, s32x4 b) { return {_mm_sllv_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x4 operator<<(s32x4 a, s32x4 b) {a.s[0]<<=b.s[0];a.s[1]<<=b.s[1];a.s[2]<<=b.s[2];a.s[3]<<=b.s[3];return a; }
#endif
	forceinline friend s32x4 operator<<(s32x4 a, s32 b) { return {_mm_slli_epi32(a.m, b)}; }
	forceinline friend s32x4 operator<<(s32 a, s32x4 b) { return S32x4(a)<<b; }
	forceinline friend s32x4 &operator<<=(s32x4 &a, s32x4 b) { return a=a<<b; }
	forceinline friend s32x4 &operator<<=(s32x4 &a, s32 b) { return a=a<<b; }
#if ARCH_AVX2
	forceinline friend s32x4 operator>>(s32x4 a, s32x4 b) { return {_mm_srav_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x4 operator>>(s32x4 a, s32x4 b) {a.s[0]>>=b.s[0];a.s[1]>>=b.s[1];a.s[2]>>=b.s[2];a.s[3]>>=b.s[3];return a; }
#endif
	forceinline friend s32x4 operator>>(s32x4 a, s32 b) { return {_mm_srai_epi32(a.m, b)}; }
	forceinline friend s32x4 operator>>(s32 a, s32x4 b) { return S32x4(a)>>b; }
	forceinline friend s32x4 &operator>>=(s32x4 &a, s32x4 b) { return a=a>>b; }
	forceinline friend s32x4 &operator>>=(s32x4 &a, s32 b) { return a=a>>b; }
	forceinline operator u32x4() const;
	forceinline operator u64x4() const;
	forceinline operator s64x4() const;
	forceinline operator f32x4() const;
	forceinline operator f64x4() const;
};

forceinline s32x4 S32x4(s32 a) { return {_mm_set1_epi32(a)}; }
forceinline s32x4 S32x4(s32 _0,s32 _1,s32 _2,s32 _3) { return {_mm_setr_epi32(_0,_1,_2,_3)}; }
forceinline s64x2 S64x2(s64);
forceinline s64x2 S64x2(s64,s64);

union s64x2 {
	__m128i m;
	s64 s[2];
	forceinline s64 operator[](umm i) const { return s[i]; }
	forceinline s64 &operator[](umm i) { return s[i]; }
	forceinline friend s64x2 operator+(s64x2 a) { return a; }
	forceinline friend s64x2 operator-(s64x2 a) { return s64x2{} - a; }
	forceinline friend s64x2 operator~(s64x2 a) { return {_mm_xor_si128(a.m, _mm_set1_epi32(~0))}; }
	forceinline friend s64x2 operator+(s64x2 a, s64x2 b) { return {_mm_add_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator+(s64x2 a, s64 b) { return a+S64x2(b); }
	forceinline friend s64x2 operator+(s64 a, s64x2 b) { return S64x2(a)+b; }
	forceinline friend s64x2 &operator+=(s64x2 &a, s64x2 b) { return a=a+b; }
	forceinline friend s64x2 &operator+=(s64x2 &a, s64 b) { return a=a+b; }
	forceinline friend s64x2 operator-(s64x2 a, s64x2 b) { return {_mm_sub_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator-(s64x2 a, s64 b) { return a-S64x2(b); }
	forceinline friend s64x2 operator-(s64 a, s64x2 b) { return S64x2(a)-b; }
	forceinline friend s64x2 &operator-=(s64x2 &a, s64x2 b) { return a=a-b; }
	forceinline friend s64x2 &operator-=(s64x2 &a, s64 b) { return a=a-b; }
	forceinline friend s64x2 operator*(s64x2 a, s64x2 b) { return {_mm_mullo_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator*(s64x2 a, s64 b) { return a*S64x2(b); }
	forceinline friend s64x2 operator*(s64 a, s64x2 b) { return S64x2(a)*b; }
	forceinline friend s64x2 &operator*=(s64x2 &a, s64x2 b) { return a=a*b; }
	forceinline friend s64x2 &operator*=(s64x2 &a, s64 b) { return a=a*b; }
	forceinline friend s64x2 operator/(s64x2 a, s64x2 b) { return {_mm_div_epi64(a.m, b.m)}; }
	forceinline friend s64x2 operator/(s64x2 a, s64 b) { return a/S64x2(b); }
	forceinline friend s64x2 operator/(s64 a, s64x2 b) { return S64x2(a)/b; }
	forceinline friend s64x2 &operator/=(s64x2 &a, s64x2 b) { return a=a/b; }
	forceinline friend s64x2 &operator/=(s64x2 &a, s64 b) { return a=a/b; }
	forceinline friend s64x2 operator^(s64x2 a, s64x2 b) { return {_mm_xor_si128(a.m, b.m)}; }
	forceinline friend s64x2 operator^(s64x2 a, s64 b) { return a^S64x2(b); }
	forceinline friend s64x2 operator^(s64 a, s64x2 b) { return S64x2(a)^b; }
	forceinline friend s64x2 &operator^=(s64x2 &a, s64x2 b) { return a=a^b; }
	forceinline friend s64x2 &operator^=(s64x2 &a, s64 b) { return a=a^b; }
	forceinline friend s64x2 operator&(s64x2 a, s64x2 b) { return {_mm_and_si128(a.m, b.m)}; }
	forceinline friend s64x2 operator&(s64x2 a, s64 b) { return a&S64x2(b); }
	forceinline friend s64x2 operator&(s64 a, s64x2 b) { return S64x2(a)&b; }
	forceinline friend s64x2 &operator&=(s64x2 &a, s64x2 b) { return a=a&b; }
	forceinline friend s64x2 &operator&=(s64x2 &a, s64 b) { return a=a&b; }
	forceinline friend s64x2 operator|(s64x2 a, s64x2 b) { return {_mm_or_si128(a.m, b.m)}; }
	forceinline friend s64x2 operator|(s64x2 a, s64 b) { return a|S64x2(b); }
	forceinline friend s64x2 operator|(s64 a, s64x2 b) { return S64x2(a)|b; }
	forceinline friend s64x2 &operator|=(s64x2 &a, s64x2 b) { return a=a|b; }
	forceinline friend s64x2 &operator|=(s64x2 &a, s64 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend s64x2 operator<<(s64x2 a, s64x2 b) { return {_mm_sllv_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x2 operator<<(s64x2 a, s64x2 b) {a.s[0]<<=b.s[0];a.s[1]<<=b.s[1];return a; }
#endif
	forceinline friend s64x2 operator<<(s64x2 a, s64 b) { return {_mm_slli_epi64(a.m, b)}; }
	forceinline friend s64x2 operator<<(s64 a, s64x2 b) { return S64x2(a)<<b; }
	forceinline friend s64x2 &operator<<=(s64x2 &a, s64x2 b) { return a=a<<b; }
	forceinline friend s64x2 &operator<<=(s64x2 &a, s64 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s64x2 operator>>(s64x2 a, s64x2 b) { return {_mm_srav_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x2 operator>>(s64x2 a, s64x2 b) {a.s[0]>>=b.s[0];a.s[1]>>=b.s[1];return a; }
#endif
#if ARCH_AVX512
	forceinline friend s64x2 operator>>(s64x2 a, s64 b) { return {_mm_srai_epi64(a.m, b)}; }
#else
	forceinline friend s64x2 operator>>(s64x2 a, s64 b) {a.s[0]>>=b;a.s[1]>>=b;return a; }
#endif
	forceinline friend s64x2 operator>>(s64 a, s64x2 b) { return S64x2(a)>>b; }
	forceinline friend s64x2 &operator>>=(s64x2 &a, s64x2 b) { return a=a>>b; }
	forceinline friend s64x2 &operator>>=(s64x2 &a, s64 b) { return a=a>>b; }
	forceinline operator u64x2() const;
	forceinline operator f64x2() const;
};

forceinline s64x2 S64x2(s64 a) { return {_mm_set1_epi64x(a)}; }
forceinline s64x2 S64x2(s64 _0,s64 _1) { return {_mm_setr_epi64x(_0,_1)}; }
forceinline f32x4 F32x4(f32);
forceinline f32x4 F32x4(f32,f32,f32,f32);

union f32x4 {
	__m128 m;
	f32 s[4];
	forceinline f32 operator[](umm i) const { return s[i]; }
	forceinline f32 &operator[](umm i) { return s[i]; }
	forceinline friend f32x4 operator+(f32x4 a) { return a; }
	forceinline friend f32x4 operator-(f32x4 a) { return f32x4{} - a; }
	forceinline friend f32x4 operator+(f32x4 a, f32x4 b) { return {_mm_add_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator+(f32x4 a, f32 b) { return a+F32x4(b); }
	forceinline friend f32x4 operator+(f32 a, f32x4 b) { return F32x4(a)+b; }
	forceinline friend f32x4 &operator+=(f32x4 &a, f32x4 b) { return a=a+b; }
	forceinline friend f32x4 &operator+=(f32x4 &a, f32 b) { return a=a+b; }
	forceinline friend f32x4 operator-(f32x4 a, f32x4 b) { return {_mm_sub_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator-(f32x4 a, f32 b) { return a-F32x4(b); }
	forceinline friend f32x4 operator-(f32 a, f32x4 b) { return F32x4(a)-b; }
	forceinline friend f32x4 &operator-=(f32x4 &a, f32x4 b) { return a=a-b; }
	forceinline friend f32x4 &operator-=(f32x4 &a, f32 b) { return a=a-b; }
	forceinline friend f32x4 operator*(f32x4 a, f32x4 b) { return {_mm_mul_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator*(f32x4 a, f32 b) { return a*F32x4(b); }
	forceinline friend f32x4 operator*(f32 a, f32x4 b) { return F32x4(a)*b; }
	forceinline friend f32x4 &operator*=(f32x4 &a, f32x4 b) { return a=a*b; }
	forceinline friend f32x4 &operator*=(f32x4 &a, f32 b) { return a=a*b; }
	forceinline friend f32x4 operator/(f32x4 a, f32x4 b) { return {_mm_div_ps(a.m, b.m)}; }
	forceinline friend f32x4 operator/(f32x4 a, f32 b) { return a/F32x4(b); }
	forceinline friend f32x4 operator/(f32 a, f32x4 b) { return F32x4(a)/b; }
	forceinline friend f32x4 &operator/=(f32x4 &a, f32x4 b) { return a=a/b; }
	forceinline friend f32x4 &operator/=(f32x4 &a, f32 b) { return a=a/b; }
	forceinline operator u32x4() const;
	forceinline operator u64x4() const;
	forceinline operator s32x4() const;
	forceinline operator s64x4() const;
	forceinline operator f64x4() const;
};

forceinline f32x4 F32x4(f32 a) { return {_mm_set1_ps(a)}; }
forceinline f32x4 F32x4(f32 _0,f32 _1,f32 _2,f32 _3) { return {_mm_setr_ps(_0,_1,_2,_3)}; }
#if ARCH_AVX
forceinline f32x4 fmadd(f32x4 a, f32x4 b, f32x4 c) { return {_mm_fmadd_ps(a.m, b.m, c.m)}; }
#else
forceinline f32x4 fmadd(f32x4 a, f32x4 b, f32x4 c) {a.s[0]=a.s[0]*b.s[0]+c.s[0];a.s[1]=a.s[1]*b.s[1]+c.s[1];a.s[2]=a.s[2]*b.s[2]+c.s[2];a.s[3]=a.s[3]*b.s[3]+c.s[3];return a;}
#endif

forceinline f64x2 F64x2(f64);
forceinline f64x2 F64x2(f64,f64);

union f64x2 {
	__m128d m;
	f64 s[2];
	forceinline f64 operator[](umm i) const { return s[i]; }
	forceinline f64 &operator[](umm i) { return s[i]; }
	forceinline friend f64x2 operator+(f64x2 a) { return a; }
	forceinline friend f64x2 operator-(f64x2 a) { return f64x2{} - a; }
	forceinline friend f64x2 operator+(f64x2 a, f64x2 b) { return {_mm_add_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator+(f64x2 a, f64 b) { return a+F64x2(b); }
	forceinline friend f64x2 operator+(f64 a, f64x2 b) { return F64x2(a)+b; }
	forceinline friend f64x2 &operator+=(f64x2 &a, f64x2 b) { return a=a+b; }
	forceinline friend f64x2 &operator+=(f64x2 &a, f64 b) { return a=a+b; }
	forceinline friend f64x2 operator-(f64x2 a, f64x2 b) { return {_mm_sub_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator-(f64x2 a, f64 b) { return a-F64x2(b); }
	forceinline friend f64x2 operator-(f64 a, f64x2 b) { return F64x2(a)-b; }
	forceinline friend f64x2 &operator-=(f64x2 &a, f64x2 b) { return a=a-b; }
	forceinline friend f64x2 &operator-=(f64x2 &a, f64 b) { return a=a-b; }
	forceinline friend f64x2 operator*(f64x2 a, f64x2 b) { return {_mm_mul_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator*(f64x2 a, f64 b) { return a*F64x2(b); }
	forceinline friend f64x2 operator*(f64 a, f64x2 b) { return F64x2(a)*b; }
	forceinline friend f64x2 &operator*=(f64x2 &a, f64x2 b) { return a=a*b; }
	forceinline friend f64x2 &operator*=(f64x2 &a, f64 b) { return a=a*b; }
	forceinline friend f64x2 operator/(f64x2 a, f64x2 b) { return {_mm_div_pd(a.m, b.m)}; }
	forceinline friend f64x2 operator/(f64x2 a, f64 b) { return a/F64x2(b); }
	forceinline friend f64x2 operator/(f64 a, f64x2 b) { return F64x2(a)/b; }
	forceinline friend f64x2 &operator/=(f64x2 &a, f64x2 b) { return a=a/b; }
	forceinline friend f64x2 &operator/=(f64x2 &a, f64 b) { return a=a/b; }
	forceinline operator u64x2() const;
	forceinline operator s64x2() const;
};

forceinline f64x2 F64x2(f64 a) { return {_mm_set1_pd(a)}; }
forceinline f64x2 F64x2(f64 _0,f64 _1) { return {_mm_setr_pd(_0,_1)}; }
#if ARCH_AVX
forceinline f64x2 fmadd(f64x2 a, f64x2 b, f64x2 c) { return {_mm_fmadd_pd(a.m, b.m, c.m)}; }
#else
forceinline f64x2 fmadd(f64x2 a, f64x2 b, f64x2 c) {a.s[0]=a.s[0]*b.s[0]+c.s[0];a.s[1]=a.s[1]*b.s[1]+c.s[1];return a;}
#endif

forceinline u8x16::operator u16x16() const { return *(u16x16*)this; }
forceinline u8x16::operator u32x16() const { return *(u32x16*)this; }
forceinline u8x16::operator s8x16() const { return *(s8x16*)this; }
forceinline u8x16::operator s16x16() const { return *(s16x16*)this; }
forceinline u8x16::operator s32x16() const { return *(s32x16*)this; }
forceinline u8x16::operator f32x16() const {
#if ARCH_AVX512
	return {_mm512_cvtepu8_ps(m)}; }
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
forceinline u16x8::operator u32x8() const { return *(u32x8*)this; }
forceinline u16x8::operator u64x8() const { return *(u64x8*)this; }
forceinline u16x8::operator s16x8() const { return *(s16x8*)this; }
forceinline u16x8::operator s32x8() const { return *(s32x8*)this; }
forceinline u16x8::operator s64x8() const { return *(s64x8*)this; }
forceinline u16x8::operator f32x8() const {
#if ARCH_AVX512
	return {_mm256_cvtepu16_ps(m)}; }
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
forceinline u16x8::operator f64x8() const {
#if ARCH_AVX512
	return {_mm512_cvtepu16_pd(m)}; }
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
forceinline u32x4::operator u64x4() const { return *(u64x4*)this; }
forceinline u32x4::operator s32x4() const { return *(s32x4*)this; }
forceinline u32x4::operator s64x4() const { return *(s64x4*)this; }
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
forceinline u32x4::operator f64x4() const {
#if ARCH_AVX512
	return {_mm256_cvtepu32_pd(m)}; }
#else
	f64x4 r;
	r[0] = (f64)s[0];
	r[1] = (f64)s[1];
	r[2] = (f64)s[2];
	r[3] = (f64)s[3];
	return r;
#endif
}
forceinline u64x2::operator s64x2() const { return *(s64x2*)this; }
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
forceinline s8x16::operator u8x16() const { return *(u8x16*)this; }
forceinline s8x16::operator u16x16() const { return *(u16x16*)this; }
forceinline s8x16::operator u32x16() const { return *(u32x16*)this; }
forceinline s8x16::operator s16x16() const { return *(s16x16*)this; }
forceinline s8x16::operator s32x16() const { return *(s32x16*)this; }
forceinline s8x16::operator f32x16() const {
#if ARCH_AVX512
	return {_mm512_cvtepi8_ps(m)}; }
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
forceinline s16x8::operator u16x8() const { return *(u16x8*)this; }
forceinline s16x8::operator u32x8() const { return *(u32x8*)this; }
forceinline s16x8::operator u64x8() const { return *(u64x8*)this; }
forceinline s16x8::operator s32x8() const { return *(s32x8*)this; }
forceinline s16x8::operator s64x8() const { return *(s64x8*)this; }
forceinline s16x8::operator f32x8() const { return {_mm256_cvtepi16_ps(m)}; }
forceinline s16x8::operator f64x8() const { return {_mm512_cvtepi16_pd(m)}; }
forceinline s32x4::operator u32x4() const { return *(u32x4*)this; }
forceinline s32x4::operator u64x4() const { return *(u64x4*)this; }
forceinline s32x4::operator s64x4() const { return *(s64x4*)this; }
forceinline s32x4::operator f32x4() const { return {_mm_cvtepi32_ps(m)}; }
forceinline s32x4::operator f64x4() const { return {_mm256_cvtepi32_pd(m)}; }
forceinline s64x2::operator u64x2() const { return *(u64x2*)this; }
forceinline s64x2::operator f64x2() const { return {_mm_cvtepi64_pd(m)}; }
forceinline f32x4::operator u32x4() const { return {_mm_cvttps_epu32(m)}; }
forceinline f32x4::operator u64x4() const { return {_mm256_cvttps_epu64(m)}; }
forceinline f32x4::operator s32x4() const { return {_mm_cvttps_epi32(m)}; }
forceinline f32x4::operator s64x4() const { return {_mm256_cvttps_epi64(m)}; }
forceinline f32x4::operator f64x4() const { return {_mm256_cvtps_pd(m)}; }
forceinline f64x2::operator u64x2() const { return {_mm_cvttpd_epu64(m)}; }
forceinline f64x2::operator s64x2() const { return {_mm_cvttpd_epi64(m)}; }
forceinline u8x32 U8x32(u8);
forceinline u8x32 U8x32(u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8);

union u8x32 {
#if ARCH_AVX2
	__m256i m;
#endif
	u8x16 x16[2];
	u8 s[32];
	forceinline u8 operator[](umm i) const { return s[i]; }
	forceinline u8 &operator[](umm i) { return s[i]; }
	forceinline friend u8x32 operator+(u8x32 a) { return a; }
	forceinline friend u8x32 operator-(u8x32 a) { return u8x32{} - a; }
#if ARCH_AVX2
	forceinline friend u8x32 operator~(u8x32 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend u8x32 operator~(u8x32 a) {a.x16[0] = ~a.x16[0]; a.x16[1] = ~a.x16[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u8x32 operator+(u8x32 a, u8x32 b) { return {_mm256_add_epi8(a.m, b.m)}; }
#else
	forceinline friend u8x32 operator+(u8x32 a, u8x32 b) {a.x16[0] += b.x16[0]; a.x16[1] += b.x16[1]; return a; }
#endif
	forceinline friend u8x32 operator+(u8x32 a, u8 b) { return a+U8x32(b); }
	forceinline friend u8x32 operator+(u8 a, u8x32 b) { return U8x32(a)+b; }
	forceinline friend u8x32 &operator+=(u8x32 &a, u8x32 b) { return a=a+b; }
	forceinline friend u8x32 &operator+=(u8x32 &a, u8 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend u8x32 operator-(u8x32 a, u8x32 b) { return {_mm256_sub_epi8(a.m, b.m)}; }
#else
	forceinline friend u8x32 operator-(u8x32 a, u8x32 b) {a.x16[0] -= b.x16[0]; a.x16[1] -= b.x16[1]; return a; }
#endif
	forceinline friend u8x32 operator-(u8x32 a, u8 b) { return a-U8x32(b); }
	forceinline friend u8x32 operator-(u8 a, u8x32 b) { return U8x32(a)-b; }
	forceinline friend u8x32 &operator-=(u8x32 &a, u8x32 b) { return a=a-b; }
	forceinline friend u8x32 &operator-=(u8x32 &a, u8 b) { return a=a-b; }
	forceinline friend u8x32 operator*(u8x32 a, u8x32 b) {a.s[0] *= b.s[0]; a.s[1] *= b.s[1]; a.s[2] *= b.s[2]; a.s[3] *= b.s[3]; a.s[4] *= b.s[4]; a.s[5] *= b.s[5]; a.s[6] *= b.s[6]; a.s[7] *= b.s[7]; a.s[8] *= b.s[8]; a.s[9] *= b.s[9]; a.s[10] *= b.s[10]; a.s[11] *= b.s[11]; a.s[12] *= b.s[12]; a.s[13] *= b.s[13]; a.s[14] *= b.s[14]; a.s[15] *= b.s[15]; a.s[16] *= b.s[16]; a.s[17] *= b.s[17]; a.s[18] *= b.s[18]; a.s[19] *= b.s[19]; a.s[20] *= b.s[20]; a.s[21] *= b.s[21]; a.s[22] *= b.s[22]; a.s[23] *= b.s[23]; a.s[24] *= b.s[24]; a.s[25] *= b.s[25]; a.s[26] *= b.s[26]; a.s[27] *= b.s[27]; a.s[28] *= b.s[28]; a.s[29] *= b.s[29]; a.s[30] *= b.s[30]; a.s[31] *= b.s[31]; return a; }
	forceinline friend u8x32 operator*(u8x32 a, u8 b) {a.s[0] *= b; a.s[1] *= b; a.s[2] *= b; a.s[3] *= b; a.s[4] *= b; a.s[5] *= b; a.s[6] *= b; a.s[7] *= b; a.s[8] *= b; a.s[9] *= b; a.s[10] *= b; a.s[11] *= b; a.s[12] *= b; a.s[13] *= b; a.s[14] *= b; a.s[15] *= b; a.s[16] *= b; a.s[17] *= b; a.s[18] *= b; a.s[19] *= b; a.s[20] *= b; a.s[21] *= b; a.s[22] *= b; a.s[23] *= b; a.s[24] *= b; a.s[25] *= b; a.s[26] *= b; a.s[27] *= b; a.s[28] *= b; a.s[29] *= b; a.s[30] *= b; a.s[31] *= b; return a; }
	forceinline friend u8x32 operator*(u8 a, u8x32 b) {auto r=U8x32(a);r.s[0] *= b.s[0]; r.s[1] *= b.s[1]; r.s[2] *= b.s[2]; r.s[3] *= b.s[3]; r.s[4] *= b.s[4]; r.s[5] *= b.s[5]; r.s[6] *= b.s[6]; r.s[7] *= b.s[7]; r.s[8] *= b.s[8]; r.s[9] *= b.s[9]; r.s[10] *= b.s[10]; r.s[11] *= b.s[11]; r.s[12] *= b.s[12]; r.s[13] *= b.s[13]; r.s[14] *= b.s[14]; r.s[15] *= b.s[15]; r.s[16] *= b.s[16]; r.s[17] *= b.s[17]; r.s[18] *= b.s[18]; r.s[19] *= b.s[19]; r.s[20] *= b.s[20]; r.s[21] *= b.s[21]; r.s[22] *= b.s[22]; r.s[23] *= b.s[23]; r.s[24] *= b.s[24]; r.s[25] *= b.s[25]; r.s[26] *= b.s[26]; r.s[27] *= b.s[27]; r.s[28] *= b.s[28]; r.s[29] *= b.s[29]; r.s[30] *= b.s[30]; r.s[31] *= b.s[31]; return r; }
	forceinline friend u8x32 &operator*=(u8x32 &a, u8x32 b) { return a=a*b; }
	forceinline friend u8x32 &operator*=(u8x32 &a, u8 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend u8x32 operator/(u8x32 a, u8x32 b) { return {_mm256_div_epu8(a.m, b.m)}; }
#else
	forceinline friend u8x32 operator/(u8x32 a, u8x32 b) {a.x16[0] /= b.x16[0]; a.x16[1] /= b.x16[1]; return a; }
#endif
	forceinline friend u8x32 operator/(u8x32 a, u8 b) { return a/U8x32(b); }
	forceinline friend u8x32 operator/(u8 a, u8x32 b) { return U8x32(a)/b; }
	forceinline friend u8x32 &operator/=(u8x32 &a, u8x32 b) { return a=a/b; }
	forceinline friend u8x32 &operator/=(u8x32 &a, u8 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend u8x32 operator^(u8x32 a, u8x32 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend u8x32 operator^(u8x32 a, u8x32 b) {a.x16[0] ^= b.x16[0]; a.x16[1] ^= b.x16[1]; return a; }
#endif
	forceinline friend u8x32 operator^(u8x32 a, u8 b) { return a^U8x32(b); }
	forceinline friend u8x32 operator^(u8 a, u8x32 b) { return U8x32(a)^b; }
	forceinline friend u8x32 &operator^=(u8x32 &a, u8x32 b) { return a=a^b; }
	forceinline friend u8x32 &operator^=(u8x32 &a, u8 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend u8x32 operator&(u8x32 a, u8x32 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend u8x32 operator&(u8x32 a, u8x32 b) {a.x16[0] &= b.x16[0]; a.x16[1] &= b.x16[1]; return a; }
#endif
	forceinline friend u8x32 operator&(u8x32 a, u8 b) { return a&U8x32(b); }
	forceinline friend u8x32 operator&(u8 a, u8x32 b) { return U8x32(a)&b; }
	forceinline friend u8x32 &operator&=(u8x32 &a, u8x32 b) { return a=a&b; }
	forceinline friend u8x32 &operator&=(u8x32 &a, u8 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend u8x32 operator|(u8x32 a, u8x32 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend u8x32 operator|(u8x32 a, u8x32 b) {a.x16[0] |= b.x16[0]; a.x16[1] |= b.x16[1]; return a; }
#endif
	forceinline friend u8x32 operator|(u8x32 a, u8 b) { return a|U8x32(b); }
	forceinline friend u8x32 operator|(u8 a, u8x32 b) { return U8x32(a)|b; }
	forceinline friend u8x32 &operator|=(u8x32 &a, u8x32 b) { return a=a|b; }
	forceinline friend u8x32 &operator|=(u8x32 &a, u8 b) { return a=a|b; }
	forceinline friend u8x32 operator<<(u8x32 a, u8x32 b) {a.s[0] <<= b.s[0]; a.s[1] <<= b.s[1]; a.s[2] <<= b.s[2]; a.s[3] <<= b.s[3]; a.s[4] <<= b.s[4]; a.s[5] <<= b.s[5]; a.s[6] <<= b.s[6]; a.s[7] <<= b.s[7]; a.s[8] <<= b.s[8]; a.s[9] <<= b.s[9]; a.s[10] <<= b.s[10]; a.s[11] <<= b.s[11]; a.s[12] <<= b.s[12]; a.s[13] <<= b.s[13]; a.s[14] <<= b.s[14]; a.s[15] <<= b.s[15]; a.s[16] <<= b.s[16]; a.s[17] <<= b.s[17]; a.s[18] <<= b.s[18]; a.s[19] <<= b.s[19]; a.s[20] <<= b.s[20]; a.s[21] <<= b.s[21]; a.s[22] <<= b.s[22]; a.s[23] <<= b.s[23]; a.s[24] <<= b.s[24]; a.s[25] <<= b.s[25]; a.s[26] <<= b.s[26]; a.s[27] <<= b.s[27]; a.s[28] <<= b.s[28]; a.s[29] <<= b.s[29]; a.s[30] <<= b.s[30]; a.s[31] <<= b.s[31]; return a; }
	forceinline friend u8x32 operator<<(u8x32 a, u8 b) {a.s[0] <<= b; a.s[1] <<= b; a.s[2] <<= b; a.s[3] <<= b; a.s[4] <<= b; a.s[5] <<= b; a.s[6] <<= b; a.s[7] <<= b; a.s[8] <<= b; a.s[9] <<= b; a.s[10] <<= b; a.s[11] <<= b; a.s[12] <<= b; a.s[13] <<= b; a.s[14] <<= b; a.s[15] <<= b; a.s[16] <<= b; a.s[17] <<= b; a.s[18] <<= b; a.s[19] <<= b; a.s[20] <<= b; a.s[21] <<= b; a.s[22] <<= b; a.s[23] <<= b; a.s[24] <<= b; a.s[25] <<= b; a.s[26] <<= b; a.s[27] <<= b; a.s[28] <<= b; a.s[29] <<= b; a.s[30] <<= b; a.s[31] <<= b; return a; }
	forceinline friend u8x32 operator<<(u8 a, u8x32 b) {auto r=U8x32(a);r.s[0] <<= b.s[0]; r.s[1] <<= b.s[1]; r.s[2] <<= b.s[2]; r.s[3] <<= b.s[3]; r.s[4] <<= b.s[4]; r.s[5] <<= b.s[5]; r.s[6] <<= b.s[6]; r.s[7] <<= b.s[7]; r.s[8] <<= b.s[8]; r.s[9] <<= b.s[9]; r.s[10] <<= b.s[10]; r.s[11] <<= b.s[11]; r.s[12] <<= b.s[12]; r.s[13] <<= b.s[13]; r.s[14] <<= b.s[14]; r.s[15] <<= b.s[15]; r.s[16] <<= b.s[16]; r.s[17] <<= b.s[17]; r.s[18] <<= b.s[18]; r.s[19] <<= b.s[19]; r.s[20] <<= b.s[20]; r.s[21] <<= b.s[21]; r.s[22] <<= b.s[22]; r.s[23] <<= b.s[23]; r.s[24] <<= b.s[24]; r.s[25] <<= b.s[25]; r.s[26] <<= b.s[26]; r.s[27] <<= b.s[27]; r.s[28] <<= b.s[28]; r.s[29] <<= b.s[29]; r.s[30] <<= b.s[30]; r.s[31] <<= b.s[31]; return r; }
	forceinline friend u8x32 &operator<<=(u8x32 &a, u8x32 b) { return a=a<<b; }
	forceinline friend u8x32 &operator<<=(u8x32 &a, u8 b) { return a=a<<b; }
	forceinline friend u8x32 operator>>(u8x32 a, u8x32 b) {a.s[0] >>= b.s[0]; a.s[1] >>= b.s[1]; a.s[2] >>= b.s[2]; a.s[3] >>= b.s[3]; a.s[4] >>= b.s[4]; a.s[5] >>= b.s[5]; a.s[6] >>= b.s[6]; a.s[7] >>= b.s[7]; a.s[8] >>= b.s[8]; a.s[9] >>= b.s[9]; a.s[10] >>= b.s[10]; a.s[11] >>= b.s[11]; a.s[12] >>= b.s[12]; a.s[13] >>= b.s[13]; a.s[14] >>= b.s[14]; a.s[15] >>= b.s[15]; a.s[16] >>= b.s[16]; a.s[17] >>= b.s[17]; a.s[18] >>= b.s[18]; a.s[19] >>= b.s[19]; a.s[20] >>= b.s[20]; a.s[21] >>= b.s[21]; a.s[22] >>= b.s[22]; a.s[23] >>= b.s[23]; a.s[24] >>= b.s[24]; a.s[25] >>= b.s[25]; a.s[26] >>= b.s[26]; a.s[27] >>= b.s[27]; a.s[28] >>= b.s[28]; a.s[29] >>= b.s[29]; a.s[30] >>= b.s[30]; a.s[31] >>= b.s[31]; return a; }
	forceinline friend u8x32 operator>>(u8x32 a, u8 b) {a.s[0] >>= b; a.s[1] >>= b; a.s[2] >>= b; a.s[3] >>= b; a.s[4] >>= b; a.s[5] >>= b; a.s[6] >>= b; a.s[7] >>= b; a.s[8] >>= b; a.s[9] >>= b; a.s[10] >>= b; a.s[11] >>= b; a.s[12] >>= b; a.s[13] >>= b; a.s[14] >>= b; a.s[15] >>= b; a.s[16] >>= b; a.s[17] >>= b; a.s[18] >>= b; a.s[19] >>= b; a.s[20] >>= b; a.s[21] >>= b; a.s[22] >>= b; a.s[23] >>= b; a.s[24] >>= b; a.s[25] >>= b; a.s[26] >>= b; a.s[27] >>= b; a.s[28] >>= b; a.s[29] >>= b; a.s[30] >>= b; a.s[31] >>= b; return a; }
	forceinline friend u8x32 operator>>(u8 a, u8x32 b) {auto r=U8x32(a);r.s[0] >>= b.s[0]; r.s[1] >>= b.s[1]; r.s[2] >>= b.s[2]; r.s[3] >>= b.s[3]; r.s[4] >>= b.s[4]; r.s[5] >>= b.s[5]; r.s[6] >>= b.s[6]; r.s[7] >>= b.s[7]; r.s[8] >>= b.s[8]; r.s[9] >>= b.s[9]; r.s[10] >>= b.s[10]; r.s[11] >>= b.s[11]; r.s[12] >>= b.s[12]; r.s[13] >>= b.s[13]; r.s[14] >>= b.s[14]; r.s[15] >>= b.s[15]; r.s[16] >>= b.s[16]; r.s[17] >>= b.s[17]; r.s[18] >>= b.s[18]; r.s[19] >>= b.s[19]; r.s[20] >>= b.s[20]; r.s[21] >>= b.s[21]; r.s[22] >>= b.s[22]; r.s[23] >>= b.s[23]; r.s[24] >>= b.s[24]; r.s[25] >>= b.s[25]; r.s[26] >>= b.s[26]; r.s[27] >>= b.s[27]; r.s[28] >>= b.s[28]; r.s[29] >>= b.s[29]; r.s[30] >>= b.s[30]; r.s[31] >>= b.s[31]; return r; }
	forceinline friend u8x32 &operator>>=(u8x32 &a, u8x32 b) { return a=a>>b; }
	forceinline friend u8x32 &operator>>=(u8x32 &a, u8 b) { return a=a>>b; }
	forceinline operator u16x32() const;
	forceinline operator s8x32() const;
	forceinline operator s16x32() const;
};

#if ARCH_AVX2
forceinline u8x32 U8x32(u8 a) { return {_mm256_set1_epi8(a)}; }
forceinline u8x32 U8x32(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15,u8 _16,u8 _17,u8 _18,u8 _19,u8 _20,u8 _21,u8 _22,u8 _23,u8 _24,u8 _25,u8 _26,u8 _27,u8 _28,u8 _29,u8 _30,u8 _31) { return {_mm256_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }
#else
forceinline u8x32 U8x32(u8 a){u8x32 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;r[16]=a;r[17]=a;r[18]=a;r[19]=a;r[20]=a;r[21]=a;r[22]=a;r[23]=a;r[24]=a;r[25]=a;r[26]=a;r[27]=a;r[28]=a;r[29]=a;r[30]=a;r[31]=a;return r;}
forceinline u8x32 U8x32(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15,u8 _16,u8 _17,u8 _18,u8 _19,u8 _20,u8 _21,u8 _22,u8 _23,u8 _24,u8 _25,u8 _26,u8 _27,u8 _28,u8 _29,u8 _30,u8 _31){u8x32 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;r[16]=_16;r[17]=_17;r[18]=_18;r[19]=_19;r[20]=_20;r[21]=_21;r[22]=_22;r[23]=_23;r[24]=_24;r[25]=_25;r[26]=_26;r[27]=_27;r[28]=_28;r[29]=_29;r[30]=_30;r[31]=_31;return r;}
#endif
forceinline u16x16 U16x16(u16);
forceinline u16x16 U16x16(u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16);

union u16x16 {
#if ARCH_AVX2
	__m256i m;
#endif
	u16x8 x8[2];
	u16 s[16];
	forceinline u16 operator[](umm i) const { return s[i]; }
	forceinline u16 &operator[](umm i) { return s[i]; }
	forceinline friend u16x16 operator+(u16x16 a) { return a; }
	forceinline friend u16x16 operator-(u16x16 a) { return u16x16{} - a; }
#if ARCH_AVX2
	forceinline friend u16x16 operator~(u16x16 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend u16x16 operator~(u16x16 a) {a.x8[0] = ~a.x8[0]; a.x8[1] = ~a.x8[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u16x16 operator+(u16x16 a, u16x16 b) { return {_mm256_add_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator+(u16x16 a, u16x16 b) {a.x8[0] += b.x8[0]; a.x8[1] += b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator+(u16x16 a, u16 b) { return a+U16x16(b); }
	forceinline friend u16x16 operator+(u16 a, u16x16 b) { return U16x16(a)+b; }
	forceinline friend u16x16 &operator+=(u16x16 &a, u16x16 b) { return a=a+b; }
	forceinline friend u16x16 &operator+=(u16x16 &a, u16 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend u16x16 operator-(u16x16 a, u16x16 b) { return {_mm256_sub_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator-(u16x16 a, u16x16 b) {a.x8[0] -= b.x8[0]; a.x8[1] -= b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator-(u16x16 a, u16 b) { return a-U16x16(b); }
	forceinline friend u16x16 operator-(u16 a, u16x16 b) { return U16x16(a)-b; }
	forceinline friend u16x16 &operator-=(u16x16 &a, u16x16 b) { return a=a-b; }
	forceinline friend u16x16 &operator-=(u16x16 &a, u16 b) { return a=a-b; }
#if ARCH_AVX2
	forceinline friend u16x16 operator*(u16x16 a, u16x16 b) { return {_mm256_mullo_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator*(u16x16 a, u16x16 b) {a.x8[0] *= b.x8[0]; a.x8[1] *= b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator*(u16x16 a, u16 b) { return a*U16x16(b); }
	forceinline friend u16x16 operator*(u16 a, u16x16 b) { return U16x16(a)*b; }
	forceinline friend u16x16 &operator*=(u16x16 &a, u16x16 b) { return a=a*b; }
	forceinline friend u16x16 &operator*=(u16x16 &a, u16 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend u16x16 operator/(u16x16 a, u16x16 b) { return {_mm256_div_epu16(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator/(u16x16 a, u16x16 b) {a.x8[0] /= b.x8[0]; a.x8[1] /= b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator/(u16x16 a, u16 b) { return a/U16x16(b); }
	forceinline friend u16x16 operator/(u16 a, u16x16 b) { return U16x16(a)/b; }
	forceinline friend u16x16 &operator/=(u16x16 &a, u16x16 b) { return a=a/b; }
	forceinline friend u16x16 &operator/=(u16x16 &a, u16 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend u16x16 operator^(u16x16 a, u16x16 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator^(u16x16 a, u16x16 b) {a.x8[0] ^= b.x8[0]; a.x8[1] ^= b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator^(u16x16 a, u16 b) { return a^U16x16(b); }
	forceinline friend u16x16 operator^(u16 a, u16x16 b) { return U16x16(a)^b; }
	forceinline friend u16x16 &operator^=(u16x16 &a, u16x16 b) { return a=a^b; }
	forceinline friend u16x16 &operator^=(u16x16 &a, u16 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend u16x16 operator&(u16x16 a, u16x16 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator&(u16x16 a, u16x16 b) {a.x8[0] &= b.x8[0]; a.x8[1] &= b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator&(u16x16 a, u16 b) { return a&U16x16(b); }
	forceinline friend u16x16 operator&(u16 a, u16x16 b) { return U16x16(a)&b; }
	forceinline friend u16x16 &operator&=(u16x16 &a, u16x16 b) { return a=a&b; }
	forceinline friend u16x16 &operator&=(u16x16 &a, u16 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend u16x16 operator|(u16x16 a, u16x16 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator|(u16x16 a, u16x16 b) {a.x8[0] |= b.x8[0]; a.x8[1] |= b.x8[1]; return a; }
#endif
	forceinline friend u16x16 operator|(u16x16 a, u16 b) { return a|U16x16(b); }
	forceinline friend u16x16 operator|(u16 a, u16x16 b) { return U16x16(a)|b; }
	forceinline friend u16x16 &operator|=(u16x16 &a, u16x16 b) { return a=a|b; }
	forceinline friend u16x16 &operator|=(u16x16 &a, u16 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend u16x16 operator<<(u16x16 a, u16x16 b) { return {_mm256_sllv_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator<<(u16x16 a, u16x16 b) {a.x8[0] <<= b.x8[0]; a.x8[1] <<= b.x8[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u16x16 operator<<(u16x16 a, u16 b) { return {_mm256_slli_epi16(a.m, b)}; }
#else
	forceinline friend u16x16 operator<<(u16x16 a, u16 b) {a.x8[0] <<= b; a.x8[1] <<= b; return a; }
#endif
	forceinline friend u16x16 operator<<(u16 a, u16x16 b) { return U16x16(a)<<b; }
	forceinline friend u16x16 &operator<<=(u16x16 &a, u16x16 b) { return a=a<<b; }
	forceinline friend u16x16 &operator<<=(u16x16 &a, u16 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend u16x16 operator>>(u16x16 a, u16x16 b) { return {_mm256_srlv_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x16 operator>>(u16x16 a, u16x16 b) {a.x8[0] >>= b.x8[0]; a.x8[1] >>= b.x8[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u16x16 operator>>(u16x16 a, u16 b) { return {_mm256_srli_epi16(a.m, b)}; }
#else
	forceinline friend u16x16 operator>>(u16x16 a, u16 b) {a.x8[0] >>= b; a.x8[1] >>= b; return a; }
#endif
	forceinline friend u16x16 operator>>(u16 a, u16x16 b) { return U16x16(a)>>b; }
	forceinline friend u16x16 &operator>>=(u16x16 &a, u16x16 b) { return a=a>>b; }
	forceinline friend u16x16 &operator>>=(u16x16 &a, u16 b) { return a=a>>b; }
	forceinline operator u8x16() const;
	forceinline operator u32x16() const;
	forceinline operator s8x16() const;
	forceinline operator s16x16() const;
	forceinline operator s32x16() const;
	forceinline operator f32x16() const;
};

#if ARCH_AVX2
forceinline u16x16 U16x16(u16 a) { return {_mm256_set1_epi16(a)}; }
forceinline u16x16 U16x16(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7,u16 _8,u16 _9,u16 _10,u16 _11,u16 _12,u16 _13,u16 _14,u16 _15) { return {_mm256_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
#else
forceinline u16x16 U16x16(u16 a){u16x16 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;return r;}
forceinline u16x16 U16x16(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7,u16 _8,u16 _9,u16 _10,u16 _11,u16 _12,u16 _13,u16 _14,u16 _15){u16x16 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;return r;}
#endif
forceinline u32x8 U32x8(u32);
forceinline u32x8 U32x8(u32,u32,u32,u32,u32,u32,u32,u32);

union u32x8 {
#if ARCH_AVX2
	__m256i m;
#endif
	u32x4 x4[2];
	u32 s[8];
	forceinline u32 operator[](umm i) const { return s[i]; }
	forceinline u32 &operator[](umm i) { return s[i]; }
	forceinline friend u32x8 operator+(u32x8 a) { return a; }
	forceinline friend u32x8 operator-(u32x8 a) { return u32x8{} - a; }
#if ARCH_AVX2
	forceinline friend u32x8 operator~(u32x8 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend u32x8 operator~(u32x8 a) {a.x4[0] = ~a.x4[0]; a.x4[1] = ~a.x4[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u32x8 operator+(u32x8 a, u32x8 b) { return {_mm256_add_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator+(u32x8 a, u32x8 b) {a.x4[0] += b.x4[0]; a.x4[1] += b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator+(u32x8 a, u32 b) { return a+U32x8(b); }
	forceinline friend u32x8 operator+(u32 a, u32x8 b) { return U32x8(a)+b; }
	forceinline friend u32x8 &operator+=(u32x8 &a, u32x8 b) { return a=a+b; }
	forceinline friend u32x8 &operator+=(u32x8 &a, u32 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator-(u32x8 a, u32x8 b) { return {_mm256_sub_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator-(u32x8 a, u32x8 b) {a.x4[0] -= b.x4[0]; a.x4[1] -= b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator-(u32x8 a, u32 b) { return a-U32x8(b); }
	forceinline friend u32x8 operator-(u32 a, u32x8 b) { return U32x8(a)-b; }
	forceinline friend u32x8 &operator-=(u32x8 &a, u32x8 b) { return a=a-b; }
	forceinline friend u32x8 &operator-=(u32x8 &a, u32 b) { return a=a-b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator*(u32x8 a, u32x8 b) { return {_mm256_mullo_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator*(u32x8 a, u32x8 b) {a.x4[0] *= b.x4[0]; a.x4[1] *= b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator*(u32x8 a, u32 b) { return a*U32x8(b); }
	forceinline friend u32x8 operator*(u32 a, u32x8 b) { return U32x8(a)*b; }
	forceinline friend u32x8 &operator*=(u32x8 &a, u32x8 b) { return a=a*b; }
	forceinline friend u32x8 &operator*=(u32x8 &a, u32 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator/(u32x8 a, u32x8 b) { return {_mm256_div_epu32(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator/(u32x8 a, u32x8 b) {a.x4[0] /= b.x4[0]; a.x4[1] /= b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator/(u32x8 a, u32 b) { return a/U32x8(b); }
	forceinline friend u32x8 operator/(u32 a, u32x8 b) { return U32x8(a)/b; }
	forceinline friend u32x8 &operator/=(u32x8 &a, u32x8 b) { return a=a/b; }
	forceinline friend u32x8 &operator/=(u32x8 &a, u32 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator^(u32x8 a, u32x8 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator^(u32x8 a, u32x8 b) {a.x4[0] ^= b.x4[0]; a.x4[1] ^= b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator^(u32x8 a, u32 b) { return a^U32x8(b); }
	forceinline friend u32x8 operator^(u32 a, u32x8 b) { return U32x8(a)^b; }
	forceinline friend u32x8 &operator^=(u32x8 &a, u32x8 b) { return a=a^b; }
	forceinline friend u32x8 &operator^=(u32x8 &a, u32 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator&(u32x8 a, u32x8 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator&(u32x8 a, u32x8 b) {a.x4[0] &= b.x4[0]; a.x4[1] &= b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator&(u32x8 a, u32 b) { return a&U32x8(b); }
	forceinline friend u32x8 operator&(u32 a, u32x8 b) { return U32x8(a)&b; }
	forceinline friend u32x8 &operator&=(u32x8 &a, u32x8 b) { return a=a&b; }
	forceinline friend u32x8 &operator&=(u32x8 &a, u32 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator|(u32x8 a, u32x8 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator|(u32x8 a, u32x8 b) {a.x4[0] |= b.x4[0]; a.x4[1] |= b.x4[1]; return a; }
#endif
	forceinline friend u32x8 operator|(u32x8 a, u32 b) { return a|U32x8(b); }
	forceinline friend u32x8 operator|(u32 a, u32x8 b) { return U32x8(a)|b; }
	forceinline friend u32x8 &operator|=(u32x8 &a, u32x8 b) { return a=a|b; }
	forceinline friend u32x8 &operator|=(u32x8 &a, u32 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator<<(u32x8 a, u32x8 b) { return {_mm256_sllv_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator<<(u32x8 a, u32x8 b) {a.x4[0] <<= b.x4[0]; a.x4[1] <<= b.x4[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u32x8 operator<<(u32x8 a, u32 b) { return {_mm256_slli_epi32(a.m, b)}; }
#else
	forceinline friend u32x8 operator<<(u32x8 a, u32 b) {a.x4[0] <<= b; a.x4[1] <<= b; return a; }
#endif
	forceinline friend u32x8 operator<<(u32 a, u32x8 b) { return U32x8(a)<<b; }
	forceinline friend u32x8 &operator<<=(u32x8 &a, u32x8 b) { return a=a<<b; }
	forceinline friend u32x8 &operator<<=(u32x8 &a, u32 b) { return a=a<<b; }
#if ARCH_AVX2
	forceinline friend u32x8 operator>>(u32x8 a, u32x8 b) { return {_mm256_srlv_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x8 operator>>(u32x8 a, u32x8 b) {a.x4[0] >>= b.x4[0]; a.x4[1] >>= b.x4[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u32x8 operator>>(u32x8 a, u32 b) { return {_mm256_srli_epi32(a.m, b)}; }
#else
	forceinline friend u32x8 operator>>(u32x8 a, u32 b) {a.x4[0] >>= b; a.x4[1] >>= b; return a; }
#endif
	forceinline friend u32x8 operator>>(u32 a, u32x8 b) { return U32x8(a)>>b; }
	forceinline friend u32x8 &operator>>=(u32x8 &a, u32x8 b) { return a=a>>b; }
	forceinline friend u32x8 &operator>>=(u32x8 &a, u32 b) { return a=a>>b; }
	forceinline operator u16x8() const;
	forceinline operator u64x8() const;
	forceinline operator s16x8() const;
	forceinline operator s32x8() const;
	forceinline operator s64x8() const;
	forceinline operator f32x8() const;
	forceinline operator f64x8() const;
};

#if ARCH_AVX2
forceinline u32x8 U32x8(u32 a) { return {_mm256_set1_epi32(a)}; }
forceinline u32x8 U32x8(u32 _0,u32 _1,u32 _2,u32 _3,u32 _4,u32 _5,u32 _6,u32 _7) { return {_mm256_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7)}; }
#else
forceinline u32x8 U32x8(u32 a){u32x8 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;return r;}
forceinline u32x8 U32x8(u32 _0,u32 _1,u32 _2,u32 _3,u32 _4,u32 _5,u32 _6,u32 _7){u32x8 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;return r;}
#endif
forceinline u64x4 U64x4(u64);
forceinline u64x4 U64x4(u64,u64,u64,u64);

union u64x4 {
#if ARCH_AVX2
	__m256i m;
#endif
	u64x2 x2[2];
	u64 s[4];
	forceinline u64 operator[](umm i) const { return s[i]; }
	forceinline u64 &operator[](umm i) { return s[i]; }
	forceinline friend u64x4 operator+(u64x4 a) { return a; }
	forceinline friend u64x4 operator-(u64x4 a) { return u64x4{} - a; }
#if ARCH_AVX2
	forceinline friend u64x4 operator~(u64x4 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend u64x4 operator~(u64x4 a) {a.x2[0] = ~a.x2[0]; a.x2[1] = ~a.x2[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u64x4 operator+(u64x4 a, u64x4 b) { return {_mm256_add_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator+(u64x4 a, u64x4 b) {a.x2[0] += b.x2[0]; a.x2[1] += b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator+(u64x4 a, u64 b) { return a+U64x4(b); }
	forceinline friend u64x4 operator+(u64 a, u64x4 b) { return U64x4(a)+b; }
	forceinline friend u64x4 &operator+=(u64x4 &a, u64x4 b) { return a=a+b; }
	forceinline friend u64x4 &operator+=(u64x4 &a, u64 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator-(u64x4 a, u64x4 b) { return {_mm256_sub_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator-(u64x4 a, u64x4 b) {a.x2[0] -= b.x2[0]; a.x2[1] -= b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator-(u64x4 a, u64 b) { return a-U64x4(b); }
	forceinline friend u64x4 operator-(u64 a, u64x4 b) { return U64x4(a)-b; }
	forceinline friend u64x4 &operator-=(u64x4 &a, u64x4 b) { return a=a-b; }
	forceinline friend u64x4 &operator-=(u64x4 &a, u64 b) { return a=a-b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator*(u64x4 a, u64x4 b) { return {_mm256_mullo_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator*(u64x4 a, u64x4 b) {a.x2[0] *= b.x2[0]; a.x2[1] *= b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator*(u64x4 a, u64 b) { return a*U64x4(b); }
	forceinline friend u64x4 operator*(u64 a, u64x4 b) { return U64x4(a)*b; }
	forceinline friend u64x4 &operator*=(u64x4 &a, u64x4 b) { return a=a*b; }
	forceinline friend u64x4 &operator*=(u64x4 &a, u64 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator/(u64x4 a, u64x4 b) { return {_mm256_div_epu64(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator/(u64x4 a, u64x4 b) {a.x2[0] /= b.x2[0]; a.x2[1] /= b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator/(u64x4 a, u64 b) { return a/U64x4(b); }
	forceinline friend u64x4 operator/(u64 a, u64x4 b) { return U64x4(a)/b; }
	forceinline friend u64x4 &operator/=(u64x4 &a, u64x4 b) { return a=a/b; }
	forceinline friend u64x4 &operator/=(u64x4 &a, u64 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator^(u64x4 a, u64x4 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator^(u64x4 a, u64x4 b) {a.x2[0] ^= b.x2[0]; a.x2[1] ^= b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator^(u64x4 a, u64 b) { return a^U64x4(b); }
	forceinline friend u64x4 operator^(u64 a, u64x4 b) { return U64x4(a)^b; }
	forceinline friend u64x4 &operator^=(u64x4 &a, u64x4 b) { return a=a^b; }
	forceinline friend u64x4 &operator^=(u64x4 &a, u64 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator&(u64x4 a, u64x4 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator&(u64x4 a, u64x4 b) {a.x2[0] &= b.x2[0]; a.x2[1] &= b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator&(u64x4 a, u64 b) { return a&U64x4(b); }
	forceinline friend u64x4 operator&(u64 a, u64x4 b) { return U64x4(a)&b; }
	forceinline friend u64x4 &operator&=(u64x4 &a, u64x4 b) { return a=a&b; }
	forceinline friend u64x4 &operator&=(u64x4 &a, u64 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator|(u64x4 a, u64x4 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator|(u64x4 a, u64x4 b) {a.x2[0] |= b.x2[0]; a.x2[1] |= b.x2[1]; return a; }
#endif
	forceinline friend u64x4 operator|(u64x4 a, u64 b) { return a|U64x4(b); }
	forceinline friend u64x4 operator|(u64 a, u64x4 b) { return U64x4(a)|b; }
	forceinline friend u64x4 &operator|=(u64x4 &a, u64x4 b) { return a=a|b; }
	forceinline friend u64x4 &operator|=(u64x4 &a, u64 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator<<(u64x4 a, u64x4 b) { return {_mm256_sllv_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator<<(u64x4 a, u64x4 b) {a.x2[0] <<= b.x2[0]; a.x2[1] <<= b.x2[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u64x4 operator<<(u64x4 a, u64 b) { return {_mm256_slli_epi64(a.m, b)}; }
#else
	forceinline friend u64x4 operator<<(u64x4 a, u64 b) {a.x2[0] <<= b; a.x2[1] <<= b; return a; }
#endif
	forceinline friend u64x4 operator<<(u64 a, u64x4 b) { return U64x4(a)<<b; }
	forceinline friend u64x4 &operator<<=(u64x4 &a, u64x4 b) { return a=a<<b; }
	forceinline friend u64x4 &operator<<=(u64x4 &a, u64 b) { return a=a<<b; }
#if ARCH_AVX2
	forceinline friend u64x4 operator>>(u64x4 a, u64x4 b) { return {_mm256_srlv_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x4 operator>>(u64x4 a, u64x4 b) {a.x2[0] >>= b.x2[0]; a.x2[1] >>= b.x2[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend u64x4 operator>>(u64x4 a, u64 b) { return {_mm256_srli_epi64(a.m, b)}; }
#else
	forceinline friend u64x4 operator>>(u64x4 a, u64 b) {a.x2[0] >>= b; a.x2[1] >>= b; return a; }
#endif
	forceinline friend u64x4 operator>>(u64 a, u64x4 b) { return U64x4(a)>>b; }
	forceinline friend u64x4 &operator>>=(u64x4 &a, u64x4 b) { return a=a>>b; }
	forceinline friend u64x4 &operator>>=(u64x4 &a, u64 b) { return a=a>>b; }
	forceinline operator u32x4() const;
	forceinline operator s32x4() const;
	forceinline operator s64x4() const;
	forceinline operator f32x4() const;
	forceinline operator f64x4() const;
};

#if ARCH_AVX2
forceinline u64x4 U64x4(u64 a) { return {_mm256_set1_epi64x(a)}; }
forceinline u64x4 U64x4(u64 _0,u64 _1,u64 _2,u64 _3) { return {_mm256_setr_epi64x(_0,_1,_2,_3)}; }
#else
forceinline u64x4 U64x4(u64 a){u64x4 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;return r;}
forceinline u64x4 U64x4(u64 _0,u64 _1,u64 _2,u64 _3){u64x4 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;return r;}
#endif
forceinline s8x32 S8x32(s8);
forceinline s8x32 S8x32(s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8);

union s8x32 {
#if ARCH_AVX2
	__m256i m;
#endif
	s8x16 x16[2];
	s8 s[32];
	forceinline s8 operator[](umm i) const { return s[i]; }
	forceinline s8 &operator[](umm i) { return s[i]; }
	forceinline friend s8x32 operator+(s8x32 a) { return a; }
	forceinline friend s8x32 operator-(s8x32 a) { return s8x32{} - a; }
#if ARCH_AVX2
	forceinline friend s8x32 operator~(s8x32 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend s8x32 operator~(s8x32 a) {a.x16[0] = ~a.x16[0]; a.x16[1] = ~a.x16[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s8x32 operator+(s8x32 a, s8x32 b) { return {_mm256_add_epi8(a.m, b.m)}; }
#else
	forceinline friend s8x32 operator+(s8x32 a, s8x32 b) {a.x16[0] += b.x16[0]; a.x16[1] += b.x16[1]; return a; }
#endif
	forceinline friend s8x32 operator+(s8x32 a, s8 b) { return a+S8x32(b); }
	forceinline friend s8x32 operator+(s8 a, s8x32 b) { return S8x32(a)+b; }
	forceinline friend s8x32 &operator+=(s8x32 &a, s8x32 b) { return a=a+b; }
	forceinline friend s8x32 &operator+=(s8x32 &a, s8 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend s8x32 operator-(s8x32 a, s8x32 b) { return {_mm256_sub_epi8(a.m, b.m)}; }
#else
	forceinline friend s8x32 operator-(s8x32 a, s8x32 b) {a.x16[0] -= b.x16[0]; a.x16[1] -= b.x16[1]; return a; }
#endif
	forceinline friend s8x32 operator-(s8x32 a, s8 b) { return a-S8x32(b); }
	forceinline friend s8x32 operator-(s8 a, s8x32 b) { return S8x32(a)-b; }
	forceinline friend s8x32 &operator-=(s8x32 &a, s8x32 b) { return a=a-b; }
	forceinline friend s8x32 &operator-=(s8x32 &a, s8 b) { return a=a-b; }
	forceinline friend s8x32 operator*(s8x32 a, s8x32 b) {a.s[0] *= b.s[0]; a.s[1] *= b.s[1]; a.s[2] *= b.s[2]; a.s[3] *= b.s[3]; a.s[4] *= b.s[4]; a.s[5] *= b.s[5]; a.s[6] *= b.s[6]; a.s[7] *= b.s[7]; a.s[8] *= b.s[8]; a.s[9] *= b.s[9]; a.s[10] *= b.s[10]; a.s[11] *= b.s[11]; a.s[12] *= b.s[12]; a.s[13] *= b.s[13]; a.s[14] *= b.s[14]; a.s[15] *= b.s[15]; a.s[16] *= b.s[16]; a.s[17] *= b.s[17]; a.s[18] *= b.s[18]; a.s[19] *= b.s[19]; a.s[20] *= b.s[20]; a.s[21] *= b.s[21]; a.s[22] *= b.s[22]; a.s[23] *= b.s[23]; a.s[24] *= b.s[24]; a.s[25] *= b.s[25]; a.s[26] *= b.s[26]; a.s[27] *= b.s[27]; a.s[28] *= b.s[28]; a.s[29] *= b.s[29]; a.s[30] *= b.s[30]; a.s[31] *= b.s[31]; return a; }
	forceinline friend s8x32 operator*(s8x32 a, s8 b) {a.s[0] *= b; a.s[1] *= b; a.s[2] *= b; a.s[3] *= b; a.s[4] *= b; a.s[5] *= b; a.s[6] *= b; a.s[7] *= b; a.s[8] *= b; a.s[9] *= b; a.s[10] *= b; a.s[11] *= b; a.s[12] *= b; a.s[13] *= b; a.s[14] *= b; a.s[15] *= b; a.s[16] *= b; a.s[17] *= b; a.s[18] *= b; a.s[19] *= b; a.s[20] *= b; a.s[21] *= b; a.s[22] *= b; a.s[23] *= b; a.s[24] *= b; a.s[25] *= b; a.s[26] *= b; a.s[27] *= b; a.s[28] *= b; a.s[29] *= b; a.s[30] *= b; a.s[31] *= b; return a; }
	forceinline friend s8x32 operator*(s8 a, s8x32 b) {auto r=S8x32(a);r.s[0] *= b.s[0]; r.s[1] *= b.s[1]; r.s[2] *= b.s[2]; r.s[3] *= b.s[3]; r.s[4] *= b.s[4]; r.s[5] *= b.s[5]; r.s[6] *= b.s[6]; r.s[7] *= b.s[7]; r.s[8] *= b.s[8]; r.s[9] *= b.s[9]; r.s[10] *= b.s[10]; r.s[11] *= b.s[11]; r.s[12] *= b.s[12]; r.s[13] *= b.s[13]; r.s[14] *= b.s[14]; r.s[15] *= b.s[15]; r.s[16] *= b.s[16]; r.s[17] *= b.s[17]; r.s[18] *= b.s[18]; r.s[19] *= b.s[19]; r.s[20] *= b.s[20]; r.s[21] *= b.s[21]; r.s[22] *= b.s[22]; r.s[23] *= b.s[23]; r.s[24] *= b.s[24]; r.s[25] *= b.s[25]; r.s[26] *= b.s[26]; r.s[27] *= b.s[27]; r.s[28] *= b.s[28]; r.s[29] *= b.s[29]; r.s[30] *= b.s[30]; r.s[31] *= b.s[31]; return r; }
	forceinline friend s8x32 &operator*=(s8x32 &a, s8x32 b) { return a=a*b; }
	forceinline friend s8x32 &operator*=(s8x32 &a, s8 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend s8x32 operator/(s8x32 a, s8x32 b) { return {_mm256_div_epi8(a.m, b.m)}; }
#else
	forceinline friend s8x32 operator/(s8x32 a, s8x32 b) {a.x16[0] /= b.x16[0]; a.x16[1] /= b.x16[1]; return a; }
#endif
	forceinline friend s8x32 operator/(s8x32 a, s8 b) { return a/S8x32(b); }
	forceinline friend s8x32 operator/(s8 a, s8x32 b) { return S8x32(a)/b; }
	forceinline friend s8x32 &operator/=(s8x32 &a, s8x32 b) { return a=a/b; }
	forceinline friend s8x32 &operator/=(s8x32 &a, s8 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend s8x32 operator^(s8x32 a, s8x32 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend s8x32 operator^(s8x32 a, s8x32 b) {a.x16[0] ^= b.x16[0]; a.x16[1] ^= b.x16[1]; return a; }
#endif
	forceinline friend s8x32 operator^(s8x32 a, s8 b) { return a^S8x32(b); }
	forceinline friend s8x32 operator^(s8 a, s8x32 b) { return S8x32(a)^b; }
	forceinline friend s8x32 &operator^=(s8x32 &a, s8x32 b) { return a=a^b; }
	forceinline friend s8x32 &operator^=(s8x32 &a, s8 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend s8x32 operator&(s8x32 a, s8x32 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend s8x32 operator&(s8x32 a, s8x32 b) {a.x16[0] &= b.x16[0]; a.x16[1] &= b.x16[1]; return a; }
#endif
	forceinline friend s8x32 operator&(s8x32 a, s8 b) { return a&S8x32(b); }
	forceinline friend s8x32 operator&(s8 a, s8x32 b) { return S8x32(a)&b; }
	forceinline friend s8x32 &operator&=(s8x32 &a, s8x32 b) { return a=a&b; }
	forceinline friend s8x32 &operator&=(s8x32 &a, s8 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend s8x32 operator|(s8x32 a, s8x32 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend s8x32 operator|(s8x32 a, s8x32 b) {a.x16[0] |= b.x16[0]; a.x16[1] |= b.x16[1]; return a; }
#endif
	forceinline friend s8x32 operator|(s8x32 a, s8 b) { return a|S8x32(b); }
	forceinline friend s8x32 operator|(s8 a, s8x32 b) { return S8x32(a)|b; }
	forceinline friend s8x32 &operator|=(s8x32 &a, s8x32 b) { return a=a|b; }
	forceinline friend s8x32 &operator|=(s8x32 &a, s8 b) { return a=a|b; }
	forceinline friend s8x32 operator<<(s8x32 a, s8x32 b) {a.s[0] <<= b.s[0]; a.s[1] <<= b.s[1]; a.s[2] <<= b.s[2]; a.s[3] <<= b.s[3]; a.s[4] <<= b.s[4]; a.s[5] <<= b.s[5]; a.s[6] <<= b.s[6]; a.s[7] <<= b.s[7]; a.s[8] <<= b.s[8]; a.s[9] <<= b.s[9]; a.s[10] <<= b.s[10]; a.s[11] <<= b.s[11]; a.s[12] <<= b.s[12]; a.s[13] <<= b.s[13]; a.s[14] <<= b.s[14]; a.s[15] <<= b.s[15]; a.s[16] <<= b.s[16]; a.s[17] <<= b.s[17]; a.s[18] <<= b.s[18]; a.s[19] <<= b.s[19]; a.s[20] <<= b.s[20]; a.s[21] <<= b.s[21]; a.s[22] <<= b.s[22]; a.s[23] <<= b.s[23]; a.s[24] <<= b.s[24]; a.s[25] <<= b.s[25]; a.s[26] <<= b.s[26]; a.s[27] <<= b.s[27]; a.s[28] <<= b.s[28]; a.s[29] <<= b.s[29]; a.s[30] <<= b.s[30]; a.s[31] <<= b.s[31]; return a; }
	forceinline friend s8x32 operator<<(s8x32 a, s8 b) {a.s[0] <<= b; a.s[1] <<= b; a.s[2] <<= b; a.s[3] <<= b; a.s[4] <<= b; a.s[5] <<= b; a.s[6] <<= b; a.s[7] <<= b; a.s[8] <<= b; a.s[9] <<= b; a.s[10] <<= b; a.s[11] <<= b; a.s[12] <<= b; a.s[13] <<= b; a.s[14] <<= b; a.s[15] <<= b; a.s[16] <<= b; a.s[17] <<= b; a.s[18] <<= b; a.s[19] <<= b; a.s[20] <<= b; a.s[21] <<= b; a.s[22] <<= b; a.s[23] <<= b; a.s[24] <<= b; a.s[25] <<= b; a.s[26] <<= b; a.s[27] <<= b; a.s[28] <<= b; a.s[29] <<= b; a.s[30] <<= b; a.s[31] <<= b; return a; }
	forceinline friend s8x32 operator<<(s8 a, s8x32 b) {auto r=S8x32(a);r.s[0] <<= b.s[0]; r.s[1] <<= b.s[1]; r.s[2] <<= b.s[2]; r.s[3] <<= b.s[3]; r.s[4] <<= b.s[4]; r.s[5] <<= b.s[5]; r.s[6] <<= b.s[6]; r.s[7] <<= b.s[7]; r.s[8] <<= b.s[8]; r.s[9] <<= b.s[9]; r.s[10] <<= b.s[10]; r.s[11] <<= b.s[11]; r.s[12] <<= b.s[12]; r.s[13] <<= b.s[13]; r.s[14] <<= b.s[14]; r.s[15] <<= b.s[15]; r.s[16] <<= b.s[16]; r.s[17] <<= b.s[17]; r.s[18] <<= b.s[18]; r.s[19] <<= b.s[19]; r.s[20] <<= b.s[20]; r.s[21] <<= b.s[21]; r.s[22] <<= b.s[22]; r.s[23] <<= b.s[23]; r.s[24] <<= b.s[24]; r.s[25] <<= b.s[25]; r.s[26] <<= b.s[26]; r.s[27] <<= b.s[27]; r.s[28] <<= b.s[28]; r.s[29] <<= b.s[29]; r.s[30] <<= b.s[30]; r.s[31] <<= b.s[31]; return r; }
	forceinline friend s8x32 &operator<<=(s8x32 &a, s8x32 b) { return a=a<<b; }
	forceinline friend s8x32 &operator<<=(s8x32 &a, s8 b) { return a=a<<b; }
	forceinline friend s8x32 operator>>(s8x32 a, s8x32 b) {a.s[0] >>= b.s[0]; a.s[1] >>= b.s[1]; a.s[2] >>= b.s[2]; a.s[3] >>= b.s[3]; a.s[4] >>= b.s[4]; a.s[5] >>= b.s[5]; a.s[6] >>= b.s[6]; a.s[7] >>= b.s[7]; a.s[8] >>= b.s[8]; a.s[9] >>= b.s[9]; a.s[10] >>= b.s[10]; a.s[11] >>= b.s[11]; a.s[12] >>= b.s[12]; a.s[13] >>= b.s[13]; a.s[14] >>= b.s[14]; a.s[15] >>= b.s[15]; a.s[16] >>= b.s[16]; a.s[17] >>= b.s[17]; a.s[18] >>= b.s[18]; a.s[19] >>= b.s[19]; a.s[20] >>= b.s[20]; a.s[21] >>= b.s[21]; a.s[22] >>= b.s[22]; a.s[23] >>= b.s[23]; a.s[24] >>= b.s[24]; a.s[25] >>= b.s[25]; a.s[26] >>= b.s[26]; a.s[27] >>= b.s[27]; a.s[28] >>= b.s[28]; a.s[29] >>= b.s[29]; a.s[30] >>= b.s[30]; a.s[31] >>= b.s[31]; return a; }
	forceinline friend s8x32 operator>>(s8x32 a, s8 b) {a.s[0] >>= b; a.s[1] >>= b; a.s[2] >>= b; a.s[3] >>= b; a.s[4] >>= b; a.s[5] >>= b; a.s[6] >>= b; a.s[7] >>= b; a.s[8] >>= b; a.s[9] >>= b; a.s[10] >>= b; a.s[11] >>= b; a.s[12] >>= b; a.s[13] >>= b; a.s[14] >>= b; a.s[15] >>= b; a.s[16] >>= b; a.s[17] >>= b; a.s[18] >>= b; a.s[19] >>= b; a.s[20] >>= b; a.s[21] >>= b; a.s[22] >>= b; a.s[23] >>= b; a.s[24] >>= b; a.s[25] >>= b; a.s[26] >>= b; a.s[27] >>= b; a.s[28] >>= b; a.s[29] >>= b; a.s[30] >>= b; a.s[31] >>= b; return a; }
	forceinline friend s8x32 operator>>(s8 a, s8x32 b) {auto r=S8x32(a);r.s[0] >>= b.s[0]; r.s[1] >>= b.s[1]; r.s[2] >>= b.s[2]; r.s[3] >>= b.s[3]; r.s[4] >>= b.s[4]; r.s[5] >>= b.s[5]; r.s[6] >>= b.s[6]; r.s[7] >>= b.s[7]; r.s[8] >>= b.s[8]; r.s[9] >>= b.s[9]; r.s[10] >>= b.s[10]; r.s[11] >>= b.s[11]; r.s[12] >>= b.s[12]; r.s[13] >>= b.s[13]; r.s[14] >>= b.s[14]; r.s[15] >>= b.s[15]; r.s[16] >>= b.s[16]; r.s[17] >>= b.s[17]; r.s[18] >>= b.s[18]; r.s[19] >>= b.s[19]; r.s[20] >>= b.s[20]; r.s[21] >>= b.s[21]; r.s[22] >>= b.s[22]; r.s[23] >>= b.s[23]; r.s[24] >>= b.s[24]; r.s[25] >>= b.s[25]; r.s[26] >>= b.s[26]; r.s[27] >>= b.s[27]; r.s[28] >>= b.s[28]; r.s[29] >>= b.s[29]; r.s[30] >>= b.s[30]; r.s[31] >>= b.s[31]; return r; }
	forceinline friend s8x32 &operator>>=(s8x32 &a, s8x32 b) { return a=a>>b; }
	forceinline friend s8x32 &operator>>=(s8x32 &a, s8 b) { return a=a>>b; }
	forceinline operator u8x32() const;
	forceinline operator u16x32() const;
	forceinline operator s16x32() const;
};

#if ARCH_AVX2
forceinline s8x32 S8x32(s8 a) { return {_mm256_set1_epi8(a)}; }
forceinline s8x32 S8x32(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15,s8 _16,s8 _17,s8 _18,s8 _19,s8 _20,s8 _21,s8 _22,s8 _23,s8 _24,s8 _25,s8 _26,s8 _27,s8 _28,s8 _29,s8 _30,s8 _31) { return {_mm256_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }
#else
forceinline s8x32 S8x32(s8 a){s8x32 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;r[16]=a;r[17]=a;r[18]=a;r[19]=a;r[20]=a;r[21]=a;r[22]=a;r[23]=a;r[24]=a;r[25]=a;r[26]=a;r[27]=a;r[28]=a;r[29]=a;r[30]=a;r[31]=a;return r;}
forceinline s8x32 S8x32(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15,s8 _16,s8 _17,s8 _18,s8 _19,s8 _20,s8 _21,s8 _22,s8 _23,s8 _24,s8 _25,s8 _26,s8 _27,s8 _28,s8 _29,s8 _30,s8 _31){s8x32 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;r[16]=_16;r[17]=_17;r[18]=_18;r[19]=_19;r[20]=_20;r[21]=_21;r[22]=_22;r[23]=_23;r[24]=_24;r[25]=_25;r[26]=_26;r[27]=_27;r[28]=_28;r[29]=_29;r[30]=_30;r[31]=_31;return r;}
#endif
forceinline s16x16 S16x16(s16);
forceinline s16x16 S16x16(s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16);

union s16x16 {
#if ARCH_AVX2
	__m256i m;
#endif
	s16x8 x8[2];
	s16 s[16];
	forceinline s16 operator[](umm i) const { return s[i]; }
	forceinline s16 &operator[](umm i) { return s[i]; }
	forceinline friend s16x16 operator+(s16x16 a) { return a; }
	forceinline friend s16x16 operator-(s16x16 a) { return s16x16{} - a; }
#if ARCH_AVX2
	forceinline friend s16x16 operator~(s16x16 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend s16x16 operator~(s16x16 a) {a.x8[0] = ~a.x8[0]; a.x8[1] = ~a.x8[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s16x16 operator+(s16x16 a, s16x16 b) { return {_mm256_add_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator+(s16x16 a, s16x16 b) {a.x8[0] += b.x8[0]; a.x8[1] += b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator+(s16x16 a, s16 b) { return a+S16x16(b); }
	forceinline friend s16x16 operator+(s16 a, s16x16 b) { return S16x16(a)+b; }
	forceinline friend s16x16 &operator+=(s16x16 &a, s16x16 b) { return a=a+b; }
	forceinline friend s16x16 &operator+=(s16x16 &a, s16 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend s16x16 operator-(s16x16 a, s16x16 b) { return {_mm256_sub_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator-(s16x16 a, s16x16 b) {a.x8[0] -= b.x8[0]; a.x8[1] -= b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator-(s16x16 a, s16 b) { return a-S16x16(b); }
	forceinline friend s16x16 operator-(s16 a, s16x16 b) { return S16x16(a)-b; }
	forceinline friend s16x16 &operator-=(s16x16 &a, s16x16 b) { return a=a-b; }
	forceinline friend s16x16 &operator-=(s16x16 &a, s16 b) { return a=a-b; }
#if ARCH_AVX2
	forceinline friend s16x16 operator*(s16x16 a, s16x16 b) { return {_mm256_mullo_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator*(s16x16 a, s16x16 b) {a.x8[0] *= b.x8[0]; a.x8[1] *= b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator*(s16x16 a, s16 b) { return a*S16x16(b); }
	forceinline friend s16x16 operator*(s16 a, s16x16 b) { return S16x16(a)*b; }
	forceinline friend s16x16 &operator*=(s16x16 &a, s16x16 b) { return a=a*b; }
	forceinline friend s16x16 &operator*=(s16x16 &a, s16 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend s16x16 operator/(s16x16 a, s16x16 b) { return {_mm256_div_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator/(s16x16 a, s16x16 b) {a.x8[0] /= b.x8[0]; a.x8[1] /= b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator/(s16x16 a, s16 b) { return a/S16x16(b); }
	forceinline friend s16x16 operator/(s16 a, s16x16 b) { return S16x16(a)/b; }
	forceinline friend s16x16 &operator/=(s16x16 &a, s16x16 b) { return a=a/b; }
	forceinline friend s16x16 &operator/=(s16x16 &a, s16 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend s16x16 operator^(s16x16 a, s16x16 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator^(s16x16 a, s16x16 b) {a.x8[0] ^= b.x8[0]; a.x8[1] ^= b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator^(s16x16 a, s16 b) { return a^S16x16(b); }
	forceinline friend s16x16 operator^(s16 a, s16x16 b) { return S16x16(a)^b; }
	forceinline friend s16x16 &operator^=(s16x16 &a, s16x16 b) { return a=a^b; }
	forceinline friend s16x16 &operator^=(s16x16 &a, s16 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend s16x16 operator&(s16x16 a, s16x16 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator&(s16x16 a, s16x16 b) {a.x8[0] &= b.x8[0]; a.x8[1] &= b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator&(s16x16 a, s16 b) { return a&S16x16(b); }
	forceinline friend s16x16 operator&(s16 a, s16x16 b) { return S16x16(a)&b; }
	forceinline friend s16x16 &operator&=(s16x16 &a, s16x16 b) { return a=a&b; }
	forceinline friend s16x16 &operator&=(s16x16 &a, s16 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend s16x16 operator|(s16x16 a, s16x16 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator|(s16x16 a, s16x16 b) {a.x8[0] |= b.x8[0]; a.x8[1] |= b.x8[1]; return a; }
#endif
	forceinline friend s16x16 operator|(s16x16 a, s16 b) { return a|S16x16(b); }
	forceinline friend s16x16 operator|(s16 a, s16x16 b) { return S16x16(a)|b; }
	forceinline friend s16x16 &operator|=(s16x16 &a, s16x16 b) { return a=a|b; }
	forceinline friend s16x16 &operator|=(s16x16 &a, s16 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend s16x16 operator<<(s16x16 a, s16x16 b) { return {_mm256_sllv_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator<<(s16x16 a, s16x16 b) {a.x8[0] <<= b.x8[0]; a.x8[1] <<= b.x8[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s16x16 operator<<(s16x16 a, s16 b) { return {_mm256_slli_epi16(a.m, b)}; }
#else
	forceinline friend s16x16 operator<<(s16x16 a, s16 b) {a.x8[0] <<= b; a.x8[1] <<= b; return a; }
#endif
	forceinline friend s16x16 operator<<(s16 a, s16x16 b) { return S16x16(a)<<b; }
	forceinline friend s16x16 &operator<<=(s16x16 &a, s16x16 b) { return a=a<<b; }
	forceinline friend s16x16 &operator<<=(s16x16 &a, s16 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s16x16 operator>>(s16x16 a, s16x16 b) { return {_mm256_srav_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x16 operator>>(s16x16 a, s16x16 b) {a.x8[0] >>= b.x8[0]; a.x8[1] >>= b.x8[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s16x16 operator>>(s16x16 a, s16 b) { return {_mm256_srai_epi16(a.m, b)}; }
#else
	forceinline friend s16x16 operator>>(s16x16 a, s16 b) {a.x8[0] >>= b; a.x8[1] >>= b; return a; }
#endif
	forceinline friend s16x16 operator>>(s16 a, s16x16 b) { return S16x16(a)>>b; }
	forceinline friend s16x16 &operator>>=(s16x16 &a, s16x16 b) { return a=a>>b; }
	forceinline friend s16x16 &operator>>=(s16x16 &a, s16 b) { return a=a>>b; }
	forceinline operator u8x16() const;
	forceinline operator u16x16() const;
	forceinline operator u32x16() const;
	forceinline operator s8x16() const;
	forceinline operator s32x16() const;
	forceinline operator f32x16() const;
};

#if ARCH_AVX2
forceinline s16x16 S16x16(s16 a) { return {_mm256_set1_epi16(a)}; }
forceinline s16x16 S16x16(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7,s16 _8,s16 _9,s16 _10,s16 _11,s16 _12,s16 _13,s16 _14,s16 _15) { return {_mm256_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
#else
forceinline s16x16 S16x16(s16 a){s16x16 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;return r;}
forceinline s16x16 S16x16(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7,s16 _8,s16 _9,s16 _10,s16 _11,s16 _12,s16 _13,s16 _14,s16 _15){s16x16 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;return r;}
#endif
forceinline s32x8 S32x8(s32);
forceinline s32x8 S32x8(s32,s32,s32,s32,s32,s32,s32,s32);

union s32x8 {
#if ARCH_AVX2
	__m256i m;
#endif
	s32x4 x4[2];
	s32 s[8];
	forceinline s32 operator[](umm i) const { return s[i]; }
	forceinline s32 &operator[](umm i) { return s[i]; }
	forceinline friend s32x8 operator+(s32x8 a) { return a; }
	forceinline friend s32x8 operator-(s32x8 a) { return s32x8{} - a; }
#if ARCH_AVX2
	forceinline friend s32x8 operator~(s32x8 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend s32x8 operator~(s32x8 a) {a.x4[0] = ~a.x4[0]; a.x4[1] = ~a.x4[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s32x8 operator+(s32x8 a, s32x8 b) { return {_mm256_add_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator+(s32x8 a, s32x8 b) {a.x4[0] += b.x4[0]; a.x4[1] += b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator+(s32x8 a, s32 b) { return a+S32x8(b); }
	forceinline friend s32x8 operator+(s32 a, s32x8 b) { return S32x8(a)+b; }
	forceinline friend s32x8 &operator+=(s32x8 &a, s32x8 b) { return a=a+b; }
	forceinline friend s32x8 &operator+=(s32x8 &a, s32 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator-(s32x8 a, s32x8 b) { return {_mm256_sub_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator-(s32x8 a, s32x8 b) {a.x4[0] -= b.x4[0]; a.x4[1] -= b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator-(s32x8 a, s32 b) { return a-S32x8(b); }
	forceinline friend s32x8 operator-(s32 a, s32x8 b) { return S32x8(a)-b; }
	forceinline friend s32x8 &operator-=(s32x8 &a, s32x8 b) { return a=a-b; }
	forceinline friend s32x8 &operator-=(s32x8 &a, s32 b) { return a=a-b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator*(s32x8 a, s32x8 b) { return {_mm256_mullo_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator*(s32x8 a, s32x8 b) {a.x4[0] *= b.x4[0]; a.x4[1] *= b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator*(s32x8 a, s32 b) { return a*S32x8(b); }
	forceinline friend s32x8 operator*(s32 a, s32x8 b) { return S32x8(a)*b; }
	forceinline friend s32x8 &operator*=(s32x8 &a, s32x8 b) { return a=a*b; }
	forceinline friend s32x8 &operator*=(s32x8 &a, s32 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator/(s32x8 a, s32x8 b) { return {_mm256_div_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator/(s32x8 a, s32x8 b) {a.x4[0] /= b.x4[0]; a.x4[1] /= b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator/(s32x8 a, s32 b) { return a/S32x8(b); }
	forceinline friend s32x8 operator/(s32 a, s32x8 b) { return S32x8(a)/b; }
	forceinline friend s32x8 &operator/=(s32x8 &a, s32x8 b) { return a=a/b; }
	forceinline friend s32x8 &operator/=(s32x8 &a, s32 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator^(s32x8 a, s32x8 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator^(s32x8 a, s32x8 b) {a.x4[0] ^= b.x4[0]; a.x4[1] ^= b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator^(s32x8 a, s32 b) { return a^S32x8(b); }
	forceinline friend s32x8 operator^(s32 a, s32x8 b) { return S32x8(a)^b; }
	forceinline friend s32x8 &operator^=(s32x8 &a, s32x8 b) { return a=a^b; }
	forceinline friend s32x8 &operator^=(s32x8 &a, s32 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator&(s32x8 a, s32x8 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator&(s32x8 a, s32x8 b) {a.x4[0] &= b.x4[0]; a.x4[1] &= b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator&(s32x8 a, s32 b) { return a&S32x8(b); }
	forceinline friend s32x8 operator&(s32 a, s32x8 b) { return S32x8(a)&b; }
	forceinline friend s32x8 &operator&=(s32x8 &a, s32x8 b) { return a=a&b; }
	forceinline friend s32x8 &operator&=(s32x8 &a, s32 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator|(s32x8 a, s32x8 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator|(s32x8 a, s32x8 b) {a.x4[0] |= b.x4[0]; a.x4[1] |= b.x4[1]; return a; }
#endif
	forceinline friend s32x8 operator|(s32x8 a, s32 b) { return a|S32x8(b); }
	forceinline friend s32x8 operator|(s32 a, s32x8 b) { return S32x8(a)|b; }
	forceinline friend s32x8 &operator|=(s32x8 &a, s32x8 b) { return a=a|b; }
	forceinline friend s32x8 &operator|=(s32x8 &a, s32 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator<<(s32x8 a, s32x8 b) { return {_mm256_sllv_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator<<(s32x8 a, s32x8 b) {a.x4[0] <<= b.x4[0]; a.x4[1] <<= b.x4[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s32x8 operator<<(s32x8 a, s32 b) { return {_mm256_slli_epi32(a.m, b)}; }
#else
	forceinline friend s32x8 operator<<(s32x8 a, s32 b) {a.x4[0] <<= b; a.x4[1] <<= b; return a; }
#endif
	forceinline friend s32x8 operator<<(s32 a, s32x8 b) { return S32x8(a)<<b; }
	forceinline friend s32x8 &operator<<=(s32x8 &a, s32x8 b) { return a=a<<b; }
	forceinline friend s32x8 &operator<<=(s32x8 &a, s32 b) { return a=a<<b; }
#if ARCH_AVX2
	forceinline friend s32x8 operator>>(s32x8 a, s32x8 b) { return {_mm256_srav_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x8 operator>>(s32x8 a, s32x8 b) {a.x4[0] >>= b.x4[0]; a.x4[1] >>= b.x4[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s32x8 operator>>(s32x8 a, s32 b) { return {_mm256_srai_epi32(a.m, b)}; }
#else
	forceinline friend s32x8 operator>>(s32x8 a, s32 b) {a.x4[0] >>= b; a.x4[1] >>= b; return a; }
#endif
	forceinline friend s32x8 operator>>(s32 a, s32x8 b) { return S32x8(a)>>b; }
	forceinline friend s32x8 &operator>>=(s32x8 &a, s32x8 b) { return a=a>>b; }
	forceinline friend s32x8 &operator>>=(s32x8 &a, s32 b) { return a=a>>b; }
	forceinline operator u16x8() const;
	forceinline operator u32x8() const;
	forceinline operator u64x8() const;
	forceinline operator s16x8() const;
	forceinline operator s64x8() const;
	forceinline operator f32x8() const;
	forceinline operator f64x8() const;
};

#if ARCH_AVX2
forceinline s32x8 S32x8(s32 a) { return {_mm256_set1_epi32(a)}; }
forceinline s32x8 S32x8(s32 _0,s32 _1,s32 _2,s32 _3,s32 _4,s32 _5,s32 _6,s32 _7) { return {_mm256_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7)}; }
#else
forceinline s32x8 S32x8(s32 a){s32x8 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;return r;}
forceinline s32x8 S32x8(s32 _0,s32 _1,s32 _2,s32 _3,s32 _4,s32 _5,s32 _6,s32 _7){s32x8 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;return r;}
#endif
forceinline s64x4 S64x4(s64);
forceinline s64x4 S64x4(s64,s64,s64,s64);

union s64x4 {
#if ARCH_AVX2
	__m256i m;
#endif
	s64x2 x2[2];
	s64 s[4];
	forceinline s64 operator[](umm i) const { return s[i]; }
	forceinline s64 &operator[](umm i) { return s[i]; }
	forceinline friend s64x4 operator+(s64x4 a) { return a; }
	forceinline friend s64x4 operator-(s64x4 a) { return s64x4{} - a; }
#if ARCH_AVX2
	forceinline friend s64x4 operator~(s64x4 a) { return {_mm256_xor_si256(a.m, _mm256_set1_epi32(~0))}; }
#else
	forceinline friend s64x4 operator~(s64x4 a) {a.x2[0] = ~a.x2[0]; a.x2[1] = ~a.x2[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s64x4 operator+(s64x4 a, s64x4 b) { return {_mm256_add_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator+(s64x4 a, s64x4 b) {a.x2[0] += b.x2[0]; a.x2[1] += b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator+(s64x4 a, s64 b) { return a+S64x4(b); }
	forceinline friend s64x4 operator+(s64 a, s64x4 b) { return S64x4(a)+b; }
	forceinline friend s64x4 &operator+=(s64x4 &a, s64x4 b) { return a=a+b; }
	forceinline friend s64x4 &operator+=(s64x4 &a, s64 b) { return a=a+b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator-(s64x4 a, s64x4 b) { return {_mm256_sub_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator-(s64x4 a, s64x4 b) {a.x2[0] -= b.x2[0]; a.x2[1] -= b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator-(s64x4 a, s64 b) { return a-S64x4(b); }
	forceinline friend s64x4 operator-(s64 a, s64x4 b) { return S64x4(a)-b; }
	forceinline friend s64x4 &operator-=(s64x4 &a, s64x4 b) { return a=a-b; }
	forceinline friend s64x4 &operator-=(s64x4 &a, s64 b) { return a=a-b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator*(s64x4 a, s64x4 b) { return {_mm256_mullo_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator*(s64x4 a, s64x4 b) {a.x2[0] *= b.x2[0]; a.x2[1] *= b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator*(s64x4 a, s64 b) { return a*S64x4(b); }
	forceinline friend s64x4 operator*(s64 a, s64x4 b) { return S64x4(a)*b; }
	forceinline friend s64x4 &operator*=(s64x4 &a, s64x4 b) { return a=a*b; }
	forceinline friend s64x4 &operator*=(s64x4 &a, s64 b) { return a=a*b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator/(s64x4 a, s64x4 b) { return {_mm256_div_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator/(s64x4 a, s64x4 b) {a.x2[0] /= b.x2[0]; a.x2[1] /= b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator/(s64x4 a, s64 b) { return a/S64x4(b); }
	forceinline friend s64x4 operator/(s64 a, s64x4 b) { return S64x4(a)/b; }
	forceinline friend s64x4 &operator/=(s64x4 &a, s64x4 b) { return a=a/b; }
	forceinline friend s64x4 &operator/=(s64x4 &a, s64 b) { return a=a/b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator^(s64x4 a, s64x4 b) { return {_mm256_xor_si256(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator^(s64x4 a, s64x4 b) {a.x2[0] ^= b.x2[0]; a.x2[1] ^= b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator^(s64x4 a, s64 b) { return a^S64x4(b); }
	forceinline friend s64x4 operator^(s64 a, s64x4 b) { return S64x4(a)^b; }
	forceinline friend s64x4 &operator^=(s64x4 &a, s64x4 b) { return a=a^b; }
	forceinline friend s64x4 &operator^=(s64x4 &a, s64 b) { return a=a^b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator&(s64x4 a, s64x4 b) { return {_mm256_and_si256(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator&(s64x4 a, s64x4 b) {a.x2[0] &= b.x2[0]; a.x2[1] &= b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator&(s64x4 a, s64 b) { return a&S64x4(b); }
	forceinline friend s64x4 operator&(s64 a, s64x4 b) { return S64x4(a)&b; }
	forceinline friend s64x4 &operator&=(s64x4 &a, s64x4 b) { return a=a&b; }
	forceinline friend s64x4 &operator&=(s64x4 &a, s64 b) { return a=a&b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator|(s64x4 a, s64x4 b) { return {_mm256_or_si256(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator|(s64x4 a, s64x4 b) {a.x2[0] |= b.x2[0]; a.x2[1] |= b.x2[1]; return a; }
#endif
	forceinline friend s64x4 operator|(s64x4 a, s64 b) { return a|S64x4(b); }
	forceinline friend s64x4 operator|(s64 a, s64x4 b) { return S64x4(a)|b; }
	forceinline friend s64x4 &operator|=(s64x4 &a, s64x4 b) { return a=a|b; }
	forceinline friend s64x4 &operator|=(s64x4 &a, s64 b) { return a=a|b; }
#if ARCH_AVX2
	forceinline friend s64x4 operator<<(s64x4 a, s64x4 b) { return {_mm256_sllv_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator<<(s64x4 a, s64x4 b) {a.x2[0] <<= b.x2[0]; a.x2[1] <<= b.x2[1]; return a; }
#endif
#if ARCH_AVX2
	forceinline friend s64x4 operator<<(s64x4 a, s64 b) { return {_mm256_slli_epi64(a.m, b)}; }
#else
	forceinline friend s64x4 operator<<(s64x4 a, s64 b) {a.x2[0] <<= b; a.x2[1] <<= b; return a; }
#endif
	forceinline friend s64x4 operator<<(s64 a, s64x4 b) { return S64x4(a)<<b; }
	forceinline friend s64x4 &operator<<=(s64x4 &a, s64x4 b) { return a=a<<b; }
	forceinline friend s64x4 &operator<<=(s64x4 &a, s64 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s64x4 operator>>(s64x4 a, s64x4 b) { return {_mm256_srav_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x4 operator>>(s64x4 a, s64x4 b) {a.x2[0] >>= b.x2[0]; a.x2[1] >>= b.x2[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s64x4 operator>>(s64x4 a, s64 b) { return {_mm256_srai_epi64(a.m, b)}; }
#else
	forceinline friend s64x4 operator>>(s64x4 a, s64 b) {a.x2[0] >>= b; a.x2[1] >>= b; return a; }
#endif
	forceinline friend s64x4 operator>>(s64 a, s64x4 b) { return S64x4(a)>>b; }
	forceinline friend s64x4 &operator>>=(s64x4 &a, s64x4 b) { return a=a>>b; }
	forceinline friend s64x4 &operator>>=(s64x4 &a, s64 b) { return a=a>>b; }
	forceinline operator u32x4() const;
	forceinline operator u64x4() const;
	forceinline operator s32x4() const;
	forceinline operator f32x4() const;
	forceinline operator f64x4() const;
};

#if ARCH_AVX2
forceinline s64x4 S64x4(s64 a) { return {_mm256_set1_epi64x(a)}; }
forceinline s64x4 S64x4(s64 _0,s64 _1,s64 _2,s64 _3) { return {_mm256_setr_epi64x(_0,_1,_2,_3)}; }
#else
forceinline s64x4 S64x4(s64 a){s64x4 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;return r;}
forceinline s64x4 S64x4(s64 _0,s64 _1,s64 _2,s64 _3){s64x4 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;return r;}
#endif
forceinline f32x8 F32x8(f32);
forceinline f32x8 F32x8(f32,f32,f32,f32,f32,f32,f32,f32);

union f32x8 {
#if ARCH_AVX
	__m256 m;
#endif
	f32x4 x4[2];
	f32 s[8];
	forceinline f32 operator[](umm i) const { return s[i]; }
	forceinline f32 &operator[](umm i) { return s[i]; }
	forceinline friend f32x8 operator+(f32x8 a) { return a; }
	forceinline friend f32x8 operator-(f32x8 a) { return f32x8{} - a; }
#if ARCH_AVX
	forceinline friend f32x8 operator+(f32x8 a, f32x8 b) { return {_mm256_add_ps(a.m, b.m)}; }
#else
	forceinline friend f32x8 operator+(f32x8 a, f32x8 b) {a.x4[0] += b.x4[0]; a.x4[1] += b.x4[1]; return a; }
#endif
	forceinline friend f32x8 operator+(f32x8 a, f32 b) { return a+F32x8(b); }
	forceinline friend f32x8 operator+(f32 a, f32x8 b) { return F32x8(a)+b; }
	forceinline friend f32x8 &operator+=(f32x8 &a, f32x8 b) { return a=a+b; }
	forceinline friend f32x8 &operator+=(f32x8 &a, f32 b) { return a=a+b; }
#if ARCH_AVX
	forceinline friend f32x8 operator-(f32x8 a, f32x8 b) { return {_mm256_sub_ps(a.m, b.m)}; }
#else
	forceinline friend f32x8 operator-(f32x8 a, f32x8 b) {a.x4[0] -= b.x4[0]; a.x4[1] -= b.x4[1]; return a; }
#endif
	forceinline friend f32x8 operator-(f32x8 a, f32 b) { return a-F32x8(b); }
	forceinline friend f32x8 operator-(f32 a, f32x8 b) { return F32x8(a)-b; }
	forceinline friend f32x8 &operator-=(f32x8 &a, f32x8 b) { return a=a-b; }
	forceinline friend f32x8 &operator-=(f32x8 &a, f32 b) { return a=a-b; }
#if ARCH_AVX
	forceinline friend f32x8 operator*(f32x8 a, f32x8 b) { return {_mm256_mul_ps(a.m, b.m)}; }
#else
	forceinline friend f32x8 operator*(f32x8 a, f32x8 b) {a.x4[0] *= b.x4[0]; a.x4[1] *= b.x4[1]; return a; }
#endif
	forceinline friend f32x8 operator*(f32x8 a, f32 b) { return a*F32x8(b); }
	forceinline friend f32x8 operator*(f32 a, f32x8 b) { return F32x8(a)*b; }
	forceinline friend f32x8 &operator*=(f32x8 &a, f32x8 b) { return a=a*b; }
	forceinline friend f32x8 &operator*=(f32x8 &a, f32 b) { return a=a*b; }
#if ARCH_AVX
	forceinline friend f32x8 operator/(f32x8 a, f32x8 b) { return {_mm256_div_ps(a.m, b.m)}; }
#else
	forceinline friend f32x8 operator/(f32x8 a, f32x8 b) {a.x4[0] /= b.x4[0]; a.x4[1] /= b.x4[1]; return a; }
#endif
	forceinline friend f32x8 operator/(f32x8 a, f32 b) { return a/F32x8(b); }
	forceinline friend f32x8 operator/(f32 a, f32x8 b) { return F32x8(a)/b; }
	forceinline friend f32x8 &operator/=(f32x8 &a, f32x8 b) { return a=a/b; }
	forceinline friend f32x8 &operator/=(f32x8 &a, f32 b) { return a=a/b; }
	forceinline operator u16x8() const;
	forceinline operator u32x8() const;
	forceinline operator u64x8() const;
	forceinline operator s16x8() const;
	forceinline operator s32x8() const;
	forceinline operator s64x8() const;
	forceinline operator f64x8() const;
};

#if ARCH_AVX
forceinline f32x8 F32x8(f32 a) { return {_mm256_set1_ps(a)}; }
forceinline f32x8 F32x8(f32 _0,f32 _1,f32 _2,f32 _3,f32 _4,f32 _5,f32 _6,f32 _7) { return {_mm256_setr_ps(_0,_1,_2,_3,_4,_5,_6,_7)}; }
#else
forceinline f32x8 F32x8(f32 a){f32x8 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;return r;}
forceinline f32x8 F32x8(f32 _0,f32 _1,f32 _2,f32 _3,f32 _4,f32 _5,f32 _6,f32 _7){f32x8 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;return r;}
#endif
#if ARCH_AVX
forceinline f32x8 fmadd(f32x8 a, f32x8 b, f32x8 c) { return {_mm256_fmadd_ps(a.m, b.m, c.m)}; }
#else
forceinline f32x8 fmadd(f32x8 a, f32x8 b, f32x8 c) {a.s[0]=a.s[0]*b.s[0]+c.s[0];a.s[1]=a.s[1]*b.s[1]+c.s[1];a.s[2]=a.s[2]*b.s[2]+c.s[2];a.s[3]=a.s[3]*b.s[3]+c.s[3];a.s[4]=a.s[4]*b.s[4]+c.s[4];a.s[5]=a.s[5]*b.s[5]+c.s[5];a.s[6]=a.s[6]*b.s[6]+c.s[6];a.s[7]=a.s[7]*b.s[7]+c.s[7];return a;}
#endif

forceinline f64x4 F64x4(f64);
forceinline f64x4 F64x4(f64,f64,f64,f64);

union f64x4 {
#if ARCH_AVX
	__m256d m;
#endif
	f64x2 x2[2];
	f64 s[4];
	forceinline f64 operator[](umm i) const { return s[i]; }
	forceinline f64 &operator[](umm i) { return s[i]; }
	forceinline friend f64x4 operator+(f64x4 a) { return a; }
	forceinline friend f64x4 operator-(f64x4 a) { return f64x4{} - a; }
#if ARCH_AVX
	forceinline friend f64x4 operator+(f64x4 a, f64x4 b) { return {_mm256_add_pd(a.m, b.m)}; }
#else
	forceinline friend f64x4 operator+(f64x4 a, f64x4 b) {a.x2[0] += b.x2[0]; a.x2[1] += b.x2[1]; return a; }
#endif
	forceinline friend f64x4 operator+(f64x4 a, f64 b) { return a+F64x4(b); }
	forceinline friend f64x4 operator+(f64 a, f64x4 b) { return F64x4(a)+b; }
	forceinline friend f64x4 &operator+=(f64x4 &a, f64x4 b) { return a=a+b; }
	forceinline friend f64x4 &operator+=(f64x4 &a, f64 b) { return a=a+b; }
#if ARCH_AVX
	forceinline friend f64x4 operator-(f64x4 a, f64x4 b) { return {_mm256_sub_pd(a.m, b.m)}; }
#else
	forceinline friend f64x4 operator-(f64x4 a, f64x4 b) {a.x2[0] -= b.x2[0]; a.x2[1] -= b.x2[1]; return a; }
#endif
	forceinline friend f64x4 operator-(f64x4 a, f64 b) { return a-F64x4(b); }
	forceinline friend f64x4 operator-(f64 a, f64x4 b) { return F64x4(a)-b; }
	forceinline friend f64x4 &operator-=(f64x4 &a, f64x4 b) { return a=a-b; }
	forceinline friend f64x4 &operator-=(f64x4 &a, f64 b) { return a=a-b; }
#if ARCH_AVX
	forceinline friend f64x4 operator*(f64x4 a, f64x4 b) { return {_mm256_mul_pd(a.m, b.m)}; }
#else
	forceinline friend f64x4 operator*(f64x4 a, f64x4 b) {a.x2[0] *= b.x2[0]; a.x2[1] *= b.x2[1]; return a; }
#endif
	forceinline friend f64x4 operator*(f64x4 a, f64 b) { return a*F64x4(b); }
	forceinline friend f64x4 operator*(f64 a, f64x4 b) { return F64x4(a)*b; }
	forceinline friend f64x4 &operator*=(f64x4 &a, f64x4 b) { return a=a*b; }
	forceinline friend f64x4 &operator*=(f64x4 &a, f64 b) { return a=a*b; }
#if ARCH_AVX
	forceinline friend f64x4 operator/(f64x4 a, f64x4 b) { return {_mm256_div_pd(a.m, b.m)}; }
#else
	forceinline friend f64x4 operator/(f64x4 a, f64x4 b) {a.x2[0] /= b.x2[0]; a.x2[1] /= b.x2[1]; return a; }
#endif
	forceinline friend f64x4 operator/(f64x4 a, f64 b) { return a/F64x4(b); }
	forceinline friend f64x4 operator/(f64 a, f64x4 b) { return F64x4(a)/b; }
	forceinline friend f64x4 &operator/=(f64x4 &a, f64x4 b) { return a=a/b; }
	forceinline friend f64x4 &operator/=(f64x4 &a, f64 b) { return a=a/b; }
	forceinline operator u32x4() const;
	forceinline operator u64x4() const;
	forceinline operator s32x4() const;
	forceinline operator s64x4() const;
	forceinline operator f32x4() const;
};

#if ARCH_AVX
forceinline f64x4 F64x4(f64 a) { return {_mm256_set1_pd(a)}; }
forceinline f64x4 F64x4(f64 _0,f64 _1,f64 _2,f64 _3) { return {_mm256_setr_pd(_0,_1,_2,_3)}; }
#else
forceinline f64x4 F64x4(f64 a){f64x4 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;return r;}
forceinline f64x4 F64x4(f64 _0,f64 _1,f64 _2,f64 _3){f64x4 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;return r;}
#endif
#if ARCH_AVX
forceinline f64x4 fmadd(f64x4 a, f64x4 b, f64x4 c) { return {_mm256_fmadd_pd(a.m, b.m, c.m)}; }
#else
forceinline f64x4 fmadd(f64x4 a, f64x4 b, f64x4 c) {a.s[0]=a.s[0]*b.s[0]+c.s[0];a.s[1]=a.s[1]*b.s[1]+c.s[1];a.s[2]=a.s[2]*b.s[2]+c.s[2];a.s[3]=a.s[3]*b.s[3]+c.s[3];return a;}
#endif

forceinline u8x32::operator u16x32() const { return *(u16x32*)this; }
forceinline u8x32::operator s8x32() const { return *(s8x32*)this; }
forceinline u8x32::operator s16x32() const { return *(s16x32*)this; }
forceinline u16x16::operator u8x16() const { return *(u8x16*)this; }
forceinline u16x16::operator u32x16() const { return *(u32x16*)this; }
forceinline u16x16::operator s8x16() const { return *(s8x16*)this; }
forceinline u16x16::operator s16x16() const { return *(s16x16*)this; }
forceinline u16x16::operator s32x16() const { return *(s32x16*)this; }
forceinline u16x16::operator f32x16() const {
#if ARCH_AVX512
	return {_mm512_cvtepu16_ps(m)}; }
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
forceinline u32x8::operator u16x8() const { return *(u16x8*)this; }
forceinline u32x8::operator u64x8() const { return *(u64x8*)this; }
forceinline u32x8::operator s16x8() const { return *(s16x8*)this; }
forceinline u32x8::operator s32x8() const { return *(s32x8*)this; }
forceinline u32x8::operator s64x8() const { return *(s64x8*)this; }
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
forceinline u32x8::operator f64x8() const {
#if ARCH_AVX512
	return {_mm512_cvtepu32_pd(m)}; }
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
forceinline u64x4::operator u32x4() const { return *(u32x4*)this; }
forceinline u64x4::operator s32x4() const { return *(s32x4*)this; }
forceinline u64x4::operator s64x4() const { return *(s64x4*)this; }
forceinline u64x4::operator f32x4() const {
#if ARCH_AVX512
	return {_mm256_cvtepu64_ps(m)}; }
#else
	f32x4 r;
	r[0] = (f32)s[0];
	r[1] = (f32)s[1];
	r[2] = (f32)s[2];
	r[3] = (f32)s[3];
	return r;
#endif
}
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
forceinline s8x32::operator u8x32() const { return *(u8x32*)this; }
forceinline s8x32::operator u16x32() const { return *(u16x32*)this; }
forceinline s8x32::operator s16x32() const { return *(s16x32*)this; }
forceinline s16x16::operator u8x16() const { return *(u8x16*)this; }
forceinline s16x16::operator u16x16() const { return *(u16x16*)this; }
forceinline s16x16::operator u32x16() const { return *(u32x16*)this; }
forceinline s16x16::operator s8x16() const { return *(s8x16*)this; }
forceinline s16x16::operator s32x16() const { return *(s32x16*)this; }
forceinline s16x16::operator f32x16() const { return {_mm512_cvtepi16_ps(m)}; }
forceinline s32x8::operator u16x8() const { return *(u16x8*)this; }
forceinline s32x8::operator u32x8() const { return *(u32x8*)this; }
forceinline s32x8::operator u64x8() const { return *(u64x8*)this; }
forceinline s32x8::operator s16x8() const { return *(s16x8*)this; }
forceinline s32x8::operator s64x8() const { return *(s64x8*)this; }
forceinline s32x8::operator f32x8() const { return {_mm256_cvtepi32_ps(m)}; }
forceinline s32x8::operator f64x8() const { return {_mm512_cvtepi32_pd(m)}; }
forceinline s64x4::operator u32x4() const { return *(u32x4*)this; }
forceinline s64x4::operator u64x4() const { return *(u64x4*)this; }
forceinline s64x4::operator s32x4() const { return *(s32x4*)this; }
forceinline s64x4::operator f32x4() const { return {_mm256_cvtepi64_ps(m)}; }
forceinline s64x4::operator f64x4() const { return {_mm256_cvtepi64_pd(m)}; }
forceinline f32x8::operator u16x8() const { return {_mm256_cvttps_epu16(m)}; }
forceinline f32x8::operator u32x8() const { return {_mm256_cvttps_epu32(m)}; }
forceinline f32x8::operator u64x8() const { return {_mm512_cvttps_epu64(m)}; }
forceinline f32x8::operator s16x8() const { return {_mm256_cvttps_epi16(m)}; }
forceinline f32x8::operator s32x8() const { return {_mm256_cvttps_epi32(m)}; }
forceinline f32x8::operator s64x8() const { return {_mm512_cvttps_epi64(m)}; }
forceinline f32x8::operator f64x8() const { return {_mm512_cvtps_pd(m)}; }
forceinline f64x4::operator u32x4() const { return {_mm256_cvttpd_epu32(m)}; }
forceinline f64x4::operator u64x4() const { return {_mm256_cvttpd_epu64(m)}; }
forceinline f64x4::operator s32x4() const { return {_mm256_cvttpd_epi32(m)}; }
forceinline f64x4::operator s64x4() const { return {_mm256_cvttpd_epi64(m)}; }
forceinline f64x4::operator f32x4() const { return {_mm256_cvtpd_ps(m)}; }
forceinline u8x64 U8x64(u8);
forceinline u8x64 U8x64(u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8,u8);

union u8x64 {
#if ARCH_AVX512
	__m512i m;
#endif
	u8x32 x32[2];
	u8x16 x16[4];
	u8 s[64];
	forceinline u8 operator[](umm i) const { return s[i]; }
	forceinline u8 &operator[](umm i) { return s[i]; }
	forceinline friend u8x64 operator+(u8x64 a) { return a; }
	forceinline friend u8x64 operator-(u8x64 a) { return u8x64{} - a; }
#if ARCH_AVX512
	forceinline friend u8x64 operator~(u8x64 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend u8x64 operator~(u8x64 a) {a.x32[0] = ~a.x32[0]; a.x32[1] = ~a.x32[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u8x64 operator+(u8x64 a, u8x64 b) { return {_mm512_add_epi8(a.m, b.m)}; }
#else
	forceinline friend u8x64 operator+(u8x64 a, u8x64 b) {a.x32[0] += b.x32[0]; a.x32[1] += b.x32[1]; return a; }
#endif
	forceinline friend u8x64 operator+(u8x64 a, u8 b) { return a+U8x64(b); }
	forceinline friend u8x64 operator+(u8 a, u8x64 b) { return U8x64(a)+b; }
	forceinline friend u8x64 &operator+=(u8x64 &a, u8x64 b) { return a=a+b; }
	forceinline friend u8x64 &operator+=(u8x64 &a, u8 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend u8x64 operator-(u8x64 a, u8x64 b) { return {_mm512_sub_epi8(a.m, b.m)}; }
#else
	forceinline friend u8x64 operator-(u8x64 a, u8x64 b) {a.x32[0] -= b.x32[0]; a.x32[1] -= b.x32[1]; return a; }
#endif
	forceinline friend u8x64 operator-(u8x64 a, u8 b) { return a-U8x64(b); }
	forceinline friend u8x64 operator-(u8 a, u8x64 b) { return U8x64(a)-b; }
	forceinline friend u8x64 &operator-=(u8x64 &a, u8x64 b) { return a=a-b; }
	forceinline friend u8x64 &operator-=(u8x64 &a, u8 b) { return a=a-b; }
	forceinline friend u8x64 operator*(u8x64 a, u8x64 b) {a.s[0] *= b.s[0]; a.s[1] *= b.s[1]; a.s[2] *= b.s[2]; a.s[3] *= b.s[3]; a.s[4] *= b.s[4]; a.s[5] *= b.s[5]; a.s[6] *= b.s[6]; a.s[7] *= b.s[7]; a.s[8] *= b.s[8]; a.s[9] *= b.s[9]; a.s[10] *= b.s[10]; a.s[11] *= b.s[11]; a.s[12] *= b.s[12]; a.s[13] *= b.s[13]; a.s[14] *= b.s[14]; a.s[15] *= b.s[15]; a.s[16] *= b.s[16]; a.s[17] *= b.s[17]; a.s[18] *= b.s[18]; a.s[19] *= b.s[19]; a.s[20] *= b.s[20]; a.s[21] *= b.s[21]; a.s[22] *= b.s[22]; a.s[23] *= b.s[23]; a.s[24] *= b.s[24]; a.s[25] *= b.s[25]; a.s[26] *= b.s[26]; a.s[27] *= b.s[27]; a.s[28] *= b.s[28]; a.s[29] *= b.s[29]; a.s[30] *= b.s[30]; a.s[31] *= b.s[31]; a.s[32] *= b.s[32]; a.s[33] *= b.s[33]; a.s[34] *= b.s[34]; a.s[35] *= b.s[35]; a.s[36] *= b.s[36]; a.s[37] *= b.s[37]; a.s[38] *= b.s[38]; a.s[39] *= b.s[39]; a.s[40] *= b.s[40]; a.s[41] *= b.s[41]; a.s[42] *= b.s[42]; a.s[43] *= b.s[43]; a.s[44] *= b.s[44]; a.s[45] *= b.s[45]; a.s[46] *= b.s[46]; a.s[47] *= b.s[47]; a.s[48] *= b.s[48]; a.s[49] *= b.s[49]; a.s[50] *= b.s[50]; a.s[51] *= b.s[51]; a.s[52] *= b.s[52]; a.s[53] *= b.s[53]; a.s[54] *= b.s[54]; a.s[55] *= b.s[55]; a.s[56] *= b.s[56]; a.s[57] *= b.s[57]; a.s[58] *= b.s[58]; a.s[59] *= b.s[59]; a.s[60] *= b.s[60]; a.s[61] *= b.s[61]; a.s[62] *= b.s[62]; a.s[63] *= b.s[63]; return a; }
	forceinline friend u8x64 operator*(u8x64 a, u8 b) {a.s[0] *= b; a.s[1] *= b; a.s[2] *= b; a.s[3] *= b; a.s[4] *= b; a.s[5] *= b; a.s[6] *= b; a.s[7] *= b; a.s[8] *= b; a.s[9] *= b; a.s[10] *= b; a.s[11] *= b; a.s[12] *= b; a.s[13] *= b; a.s[14] *= b; a.s[15] *= b; a.s[16] *= b; a.s[17] *= b; a.s[18] *= b; a.s[19] *= b; a.s[20] *= b; a.s[21] *= b; a.s[22] *= b; a.s[23] *= b; a.s[24] *= b; a.s[25] *= b; a.s[26] *= b; a.s[27] *= b; a.s[28] *= b; a.s[29] *= b; a.s[30] *= b; a.s[31] *= b; a.s[32] *= b; a.s[33] *= b; a.s[34] *= b; a.s[35] *= b; a.s[36] *= b; a.s[37] *= b; a.s[38] *= b; a.s[39] *= b; a.s[40] *= b; a.s[41] *= b; a.s[42] *= b; a.s[43] *= b; a.s[44] *= b; a.s[45] *= b; a.s[46] *= b; a.s[47] *= b; a.s[48] *= b; a.s[49] *= b; a.s[50] *= b; a.s[51] *= b; a.s[52] *= b; a.s[53] *= b; a.s[54] *= b; a.s[55] *= b; a.s[56] *= b; a.s[57] *= b; a.s[58] *= b; a.s[59] *= b; a.s[60] *= b; a.s[61] *= b; a.s[62] *= b; a.s[63] *= b; return a; }
	forceinline friend u8x64 operator*(u8 a, u8x64 b) {auto r=U8x64(a);r.s[0] *= b.s[0]; r.s[1] *= b.s[1]; r.s[2] *= b.s[2]; r.s[3] *= b.s[3]; r.s[4] *= b.s[4]; r.s[5] *= b.s[5]; r.s[6] *= b.s[6]; r.s[7] *= b.s[7]; r.s[8] *= b.s[8]; r.s[9] *= b.s[9]; r.s[10] *= b.s[10]; r.s[11] *= b.s[11]; r.s[12] *= b.s[12]; r.s[13] *= b.s[13]; r.s[14] *= b.s[14]; r.s[15] *= b.s[15]; r.s[16] *= b.s[16]; r.s[17] *= b.s[17]; r.s[18] *= b.s[18]; r.s[19] *= b.s[19]; r.s[20] *= b.s[20]; r.s[21] *= b.s[21]; r.s[22] *= b.s[22]; r.s[23] *= b.s[23]; r.s[24] *= b.s[24]; r.s[25] *= b.s[25]; r.s[26] *= b.s[26]; r.s[27] *= b.s[27]; r.s[28] *= b.s[28]; r.s[29] *= b.s[29]; r.s[30] *= b.s[30]; r.s[31] *= b.s[31]; r.s[32] *= b.s[32]; r.s[33] *= b.s[33]; r.s[34] *= b.s[34]; r.s[35] *= b.s[35]; r.s[36] *= b.s[36]; r.s[37] *= b.s[37]; r.s[38] *= b.s[38]; r.s[39] *= b.s[39]; r.s[40] *= b.s[40]; r.s[41] *= b.s[41]; r.s[42] *= b.s[42]; r.s[43] *= b.s[43]; r.s[44] *= b.s[44]; r.s[45] *= b.s[45]; r.s[46] *= b.s[46]; r.s[47] *= b.s[47]; r.s[48] *= b.s[48]; r.s[49] *= b.s[49]; r.s[50] *= b.s[50]; r.s[51] *= b.s[51]; r.s[52] *= b.s[52]; r.s[53] *= b.s[53]; r.s[54] *= b.s[54]; r.s[55] *= b.s[55]; r.s[56] *= b.s[56]; r.s[57] *= b.s[57]; r.s[58] *= b.s[58]; r.s[59] *= b.s[59]; r.s[60] *= b.s[60]; r.s[61] *= b.s[61]; r.s[62] *= b.s[62]; r.s[63] *= b.s[63]; return r; }
	forceinline friend u8x64 &operator*=(u8x64 &a, u8x64 b) { return a=a*b; }
	forceinline friend u8x64 &operator*=(u8x64 &a, u8 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend u8x64 operator/(u8x64 a, u8x64 b) { return {_mm512_div_epu8(a.m, b.m)}; }
#else
	forceinline friend u8x64 operator/(u8x64 a, u8x64 b) {a.x32[0] /= b.x32[0]; a.x32[1] /= b.x32[1]; return a; }
#endif
	forceinline friend u8x64 operator/(u8x64 a, u8 b) { return a/U8x64(b); }
	forceinline friend u8x64 operator/(u8 a, u8x64 b) { return U8x64(a)/b; }
	forceinline friend u8x64 &operator/=(u8x64 &a, u8x64 b) { return a=a/b; }
	forceinline friend u8x64 &operator/=(u8x64 &a, u8 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend u8x64 operator^(u8x64 a, u8x64 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend u8x64 operator^(u8x64 a, u8x64 b) {a.x32[0] ^= b.x32[0]; a.x32[1] ^= b.x32[1]; return a; }
#endif
	forceinline friend u8x64 operator^(u8x64 a, u8 b) { return a^U8x64(b); }
	forceinline friend u8x64 operator^(u8 a, u8x64 b) { return U8x64(a)^b; }
	forceinline friend u8x64 &operator^=(u8x64 &a, u8x64 b) { return a=a^b; }
	forceinline friend u8x64 &operator^=(u8x64 &a, u8 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend u8x64 operator&(u8x64 a, u8x64 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend u8x64 operator&(u8x64 a, u8x64 b) {a.x32[0] &= b.x32[0]; a.x32[1] &= b.x32[1]; return a; }
#endif
	forceinline friend u8x64 operator&(u8x64 a, u8 b) { return a&U8x64(b); }
	forceinline friend u8x64 operator&(u8 a, u8x64 b) { return U8x64(a)&b; }
	forceinline friend u8x64 &operator&=(u8x64 &a, u8x64 b) { return a=a&b; }
	forceinline friend u8x64 &operator&=(u8x64 &a, u8 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend u8x64 operator|(u8x64 a, u8x64 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend u8x64 operator|(u8x64 a, u8x64 b) {a.x32[0] |= b.x32[0]; a.x32[1] |= b.x32[1]; return a; }
#endif
	forceinline friend u8x64 operator|(u8x64 a, u8 b) { return a|U8x64(b); }
	forceinline friend u8x64 operator|(u8 a, u8x64 b) { return U8x64(a)|b; }
	forceinline friend u8x64 &operator|=(u8x64 &a, u8x64 b) { return a=a|b; }
	forceinline friend u8x64 &operator|=(u8x64 &a, u8 b) { return a=a|b; }
	forceinline friend u8x64 operator<<(u8x64 a, u8x64 b) {a.s[0] <<= b.s[0]; a.s[1] <<= b.s[1]; a.s[2] <<= b.s[2]; a.s[3] <<= b.s[3]; a.s[4] <<= b.s[4]; a.s[5] <<= b.s[5]; a.s[6] <<= b.s[6]; a.s[7] <<= b.s[7]; a.s[8] <<= b.s[8]; a.s[9] <<= b.s[9]; a.s[10] <<= b.s[10]; a.s[11] <<= b.s[11]; a.s[12] <<= b.s[12]; a.s[13] <<= b.s[13]; a.s[14] <<= b.s[14]; a.s[15] <<= b.s[15]; a.s[16] <<= b.s[16]; a.s[17] <<= b.s[17]; a.s[18] <<= b.s[18]; a.s[19] <<= b.s[19]; a.s[20] <<= b.s[20]; a.s[21] <<= b.s[21]; a.s[22] <<= b.s[22]; a.s[23] <<= b.s[23]; a.s[24] <<= b.s[24]; a.s[25] <<= b.s[25]; a.s[26] <<= b.s[26]; a.s[27] <<= b.s[27]; a.s[28] <<= b.s[28]; a.s[29] <<= b.s[29]; a.s[30] <<= b.s[30]; a.s[31] <<= b.s[31]; a.s[32] <<= b.s[32]; a.s[33] <<= b.s[33]; a.s[34] <<= b.s[34]; a.s[35] <<= b.s[35]; a.s[36] <<= b.s[36]; a.s[37] <<= b.s[37]; a.s[38] <<= b.s[38]; a.s[39] <<= b.s[39]; a.s[40] <<= b.s[40]; a.s[41] <<= b.s[41]; a.s[42] <<= b.s[42]; a.s[43] <<= b.s[43]; a.s[44] <<= b.s[44]; a.s[45] <<= b.s[45]; a.s[46] <<= b.s[46]; a.s[47] <<= b.s[47]; a.s[48] <<= b.s[48]; a.s[49] <<= b.s[49]; a.s[50] <<= b.s[50]; a.s[51] <<= b.s[51]; a.s[52] <<= b.s[52]; a.s[53] <<= b.s[53]; a.s[54] <<= b.s[54]; a.s[55] <<= b.s[55]; a.s[56] <<= b.s[56]; a.s[57] <<= b.s[57]; a.s[58] <<= b.s[58]; a.s[59] <<= b.s[59]; a.s[60] <<= b.s[60]; a.s[61] <<= b.s[61]; a.s[62] <<= b.s[62]; a.s[63] <<= b.s[63]; return a; }
	forceinline friend u8x64 operator<<(u8x64 a, u8 b) {a.s[0] <<= b; a.s[1] <<= b; a.s[2] <<= b; a.s[3] <<= b; a.s[4] <<= b; a.s[5] <<= b; a.s[6] <<= b; a.s[7] <<= b; a.s[8] <<= b; a.s[9] <<= b; a.s[10] <<= b; a.s[11] <<= b; a.s[12] <<= b; a.s[13] <<= b; a.s[14] <<= b; a.s[15] <<= b; a.s[16] <<= b; a.s[17] <<= b; a.s[18] <<= b; a.s[19] <<= b; a.s[20] <<= b; a.s[21] <<= b; a.s[22] <<= b; a.s[23] <<= b; a.s[24] <<= b; a.s[25] <<= b; a.s[26] <<= b; a.s[27] <<= b; a.s[28] <<= b; a.s[29] <<= b; a.s[30] <<= b; a.s[31] <<= b; a.s[32] <<= b; a.s[33] <<= b; a.s[34] <<= b; a.s[35] <<= b; a.s[36] <<= b; a.s[37] <<= b; a.s[38] <<= b; a.s[39] <<= b; a.s[40] <<= b; a.s[41] <<= b; a.s[42] <<= b; a.s[43] <<= b; a.s[44] <<= b; a.s[45] <<= b; a.s[46] <<= b; a.s[47] <<= b; a.s[48] <<= b; a.s[49] <<= b; a.s[50] <<= b; a.s[51] <<= b; a.s[52] <<= b; a.s[53] <<= b; a.s[54] <<= b; a.s[55] <<= b; a.s[56] <<= b; a.s[57] <<= b; a.s[58] <<= b; a.s[59] <<= b; a.s[60] <<= b; a.s[61] <<= b; a.s[62] <<= b; a.s[63] <<= b; return a; }
	forceinline friend u8x64 operator<<(u8 a, u8x64 b) {auto r=U8x64(a);r.s[0] <<= b.s[0]; r.s[1] <<= b.s[1]; r.s[2] <<= b.s[2]; r.s[3] <<= b.s[3]; r.s[4] <<= b.s[4]; r.s[5] <<= b.s[5]; r.s[6] <<= b.s[6]; r.s[7] <<= b.s[7]; r.s[8] <<= b.s[8]; r.s[9] <<= b.s[9]; r.s[10] <<= b.s[10]; r.s[11] <<= b.s[11]; r.s[12] <<= b.s[12]; r.s[13] <<= b.s[13]; r.s[14] <<= b.s[14]; r.s[15] <<= b.s[15]; r.s[16] <<= b.s[16]; r.s[17] <<= b.s[17]; r.s[18] <<= b.s[18]; r.s[19] <<= b.s[19]; r.s[20] <<= b.s[20]; r.s[21] <<= b.s[21]; r.s[22] <<= b.s[22]; r.s[23] <<= b.s[23]; r.s[24] <<= b.s[24]; r.s[25] <<= b.s[25]; r.s[26] <<= b.s[26]; r.s[27] <<= b.s[27]; r.s[28] <<= b.s[28]; r.s[29] <<= b.s[29]; r.s[30] <<= b.s[30]; r.s[31] <<= b.s[31]; r.s[32] <<= b.s[32]; r.s[33] <<= b.s[33]; r.s[34] <<= b.s[34]; r.s[35] <<= b.s[35]; r.s[36] <<= b.s[36]; r.s[37] <<= b.s[37]; r.s[38] <<= b.s[38]; r.s[39] <<= b.s[39]; r.s[40] <<= b.s[40]; r.s[41] <<= b.s[41]; r.s[42] <<= b.s[42]; r.s[43] <<= b.s[43]; r.s[44] <<= b.s[44]; r.s[45] <<= b.s[45]; r.s[46] <<= b.s[46]; r.s[47] <<= b.s[47]; r.s[48] <<= b.s[48]; r.s[49] <<= b.s[49]; r.s[50] <<= b.s[50]; r.s[51] <<= b.s[51]; r.s[52] <<= b.s[52]; r.s[53] <<= b.s[53]; r.s[54] <<= b.s[54]; r.s[55] <<= b.s[55]; r.s[56] <<= b.s[56]; r.s[57] <<= b.s[57]; r.s[58] <<= b.s[58]; r.s[59] <<= b.s[59]; r.s[60] <<= b.s[60]; r.s[61] <<= b.s[61]; r.s[62] <<= b.s[62]; r.s[63] <<= b.s[63]; return r; }
	forceinline friend u8x64 &operator<<=(u8x64 &a, u8x64 b) { return a=a<<b; }
	forceinline friend u8x64 &operator<<=(u8x64 &a, u8 b) { return a=a<<b; }
	forceinline friend u8x64 operator>>(u8x64 a, u8x64 b) {a.s[0] >>= b.s[0]; a.s[1] >>= b.s[1]; a.s[2] >>= b.s[2]; a.s[3] >>= b.s[3]; a.s[4] >>= b.s[4]; a.s[5] >>= b.s[5]; a.s[6] >>= b.s[6]; a.s[7] >>= b.s[7]; a.s[8] >>= b.s[8]; a.s[9] >>= b.s[9]; a.s[10] >>= b.s[10]; a.s[11] >>= b.s[11]; a.s[12] >>= b.s[12]; a.s[13] >>= b.s[13]; a.s[14] >>= b.s[14]; a.s[15] >>= b.s[15]; a.s[16] >>= b.s[16]; a.s[17] >>= b.s[17]; a.s[18] >>= b.s[18]; a.s[19] >>= b.s[19]; a.s[20] >>= b.s[20]; a.s[21] >>= b.s[21]; a.s[22] >>= b.s[22]; a.s[23] >>= b.s[23]; a.s[24] >>= b.s[24]; a.s[25] >>= b.s[25]; a.s[26] >>= b.s[26]; a.s[27] >>= b.s[27]; a.s[28] >>= b.s[28]; a.s[29] >>= b.s[29]; a.s[30] >>= b.s[30]; a.s[31] >>= b.s[31]; a.s[32] >>= b.s[32]; a.s[33] >>= b.s[33]; a.s[34] >>= b.s[34]; a.s[35] >>= b.s[35]; a.s[36] >>= b.s[36]; a.s[37] >>= b.s[37]; a.s[38] >>= b.s[38]; a.s[39] >>= b.s[39]; a.s[40] >>= b.s[40]; a.s[41] >>= b.s[41]; a.s[42] >>= b.s[42]; a.s[43] >>= b.s[43]; a.s[44] >>= b.s[44]; a.s[45] >>= b.s[45]; a.s[46] >>= b.s[46]; a.s[47] >>= b.s[47]; a.s[48] >>= b.s[48]; a.s[49] >>= b.s[49]; a.s[50] >>= b.s[50]; a.s[51] >>= b.s[51]; a.s[52] >>= b.s[52]; a.s[53] >>= b.s[53]; a.s[54] >>= b.s[54]; a.s[55] >>= b.s[55]; a.s[56] >>= b.s[56]; a.s[57] >>= b.s[57]; a.s[58] >>= b.s[58]; a.s[59] >>= b.s[59]; a.s[60] >>= b.s[60]; a.s[61] >>= b.s[61]; a.s[62] >>= b.s[62]; a.s[63] >>= b.s[63]; return a; }
	forceinline friend u8x64 operator>>(u8x64 a, u8 b) {a.s[0] >>= b; a.s[1] >>= b; a.s[2] >>= b; a.s[3] >>= b; a.s[4] >>= b; a.s[5] >>= b; a.s[6] >>= b; a.s[7] >>= b; a.s[8] >>= b; a.s[9] >>= b; a.s[10] >>= b; a.s[11] >>= b; a.s[12] >>= b; a.s[13] >>= b; a.s[14] >>= b; a.s[15] >>= b; a.s[16] >>= b; a.s[17] >>= b; a.s[18] >>= b; a.s[19] >>= b; a.s[20] >>= b; a.s[21] >>= b; a.s[22] >>= b; a.s[23] >>= b; a.s[24] >>= b; a.s[25] >>= b; a.s[26] >>= b; a.s[27] >>= b; a.s[28] >>= b; a.s[29] >>= b; a.s[30] >>= b; a.s[31] >>= b; a.s[32] >>= b; a.s[33] >>= b; a.s[34] >>= b; a.s[35] >>= b; a.s[36] >>= b; a.s[37] >>= b; a.s[38] >>= b; a.s[39] >>= b; a.s[40] >>= b; a.s[41] >>= b; a.s[42] >>= b; a.s[43] >>= b; a.s[44] >>= b; a.s[45] >>= b; a.s[46] >>= b; a.s[47] >>= b; a.s[48] >>= b; a.s[49] >>= b; a.s[50] >>= b; a.s[51] >>= b; a.s[52] >>= b; a.s[53] >>= b; a.s[54] >>= b; a.s[55] >>= b; a.s[56] >>= b; a.s[57] >>= b; a.s[58] >>= b; a.s[59] >>= b; a.s[60] >>= b; a.s[61] >>= b; a.s[62] >>= b; a.s[63] >>= b; return a; }
	forceinline friend u8x64 operator>>(u8 a, u8x64 b) {auto r=U8x64(a);r.s[0] >>= b.s[0]; r.s[1] >>= b.s[1]; r.s[2] >>= b.s[2]; r.s[3] >>= b.s[3]; r.s[4] >>= b.s[4]; r.s[5] >>= b.s[5]; r.s[6] >>= b.s[6]; r.s[7] >>= b.s[7]; r.s[8] >>= b.s[8]; r.s[9] >>= b.s[9]; r.s[10] >>= b.s[10]; r.s[11] >>= b.s[11]; r.s[12] >>= b.s[12]; r.s[13] >>= b.s[13]; r.s[14] >>= b.s[14]; r.s[15] >>= b.s[15]; r.s[16] >>= b.s[16]; r.s[17] >>= b.s[17]; r.s[18] >>= b.s[18]; r.s[19] >>= b.s[19]; r.s[20] >>= b.s[20]; r.s[21] >>= b.s[21]; r.s[22] >>= b.s[22]; r.s[23] >>= b.s[23]; r.s[24] >>= b.s[24]; r.s[25] >>= b.s[25]; r.s[26] >>= b.s[26]; r.s[27] >>= b.s[27]; r.s[28] >>= b.s[28]; r.s[29] >>= b.s[29]; r.s[30] >>= b.s[30]; r.s[31] >>= b.s[31]; r.s[32] >>= b.s[32]; r.s[33] >>= b.s[33]; r.s[34] >>= b.s[34]; r.s[35] >>= b.s[35]; r.s[36] >>= b.s[36]; r.s[37] >>= b.s[37]; r.s[38] >>= b.s[38]; r.s[39] >>= b.s[39]; r.s[40] >>= b.s[40]; r.s[41] >>= b.s[41]; r.s[42] >>= b.s[42]; r.s[43] >>= b.s[43]; r.s[44] >>= b.s[44]; r.s[45] >>= b.s[45]; r.s[46] >>= b.s[46]; r.s[47] >>= b.s[47]; r.s[48] >>= b.s[48]; r.s[49] >>= b.s[49]; r.s[50] >>= b.s[50]; r.s[51] >>= b.s[51]; r.s[52] >>= b.s[52]; r.s[53] >>= b.s[53]; r.s[54] >>= b.s[54]; r.s[55] >>= b.s[55]; r.s[56] >>= b.s[56]; r.s[57] >>= b.s[57]; r.s[58] >>= b.s[58]; r.s[59] >>= b.s[59]; r.s[60] >>= b.s[60]; r.s[61] >>= b.s[61]; r.s[62] >>= b.s[62]; r.s[63] >>= b.s[63]; return r; }
	forceinline friend u8x64 &operator>>=(u8x64 &a, u8x64 b) { return a=a>>b; }
	forceinline friend u8x64 &operator>>=(u8x64 &a, u8 b) { return a=a>>b; }
	forceinline operator s8x64() const;
};

#if ARCH_AVX512
forceinline u8x64 U8x64(u8 a) { return {_mm512_set1_epi8(a)}; }
forceinline u8x64 U8x64(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15,u8 _16,u8 _17,u8 _18,u8 _19,u8 _20,u8 _21,u8 _22,u8 _23,u8 _24,u8 _25,u8 _26,u8 _27,u8 _28,u8 _29,u8 _30,u8 _31,u8 _32,u8 _33,u8 _34,u8 _35,u8 _36,u8 _37,u8 _38,u8 _39,u8 _40,u8 _41,u8 _42,u8 _43,u8 _44,u8 _45,u8 _46,u8 _47,u8 _48,u8 _49,u8 _50,u8 _51,u8 _52,u8 _53,u8 _54,u8 _55,u8 _56,u8 _57,u8 _58,u8 _59,u8 _60,u8 _61,u8 _62,u8 _63) { return {_mm512_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63)}; }
#else
forceinline u8x64 U8x64(u8 a){u8x64 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;r[16]=a;r[17]=a;r[18]=a;r[19]=a;r[20]=a;r[21]=a;r[22]=a;r[23]=a;r[24]=a;r[25]=a;r[26]=a;r[27]=a;r[28]=a;r[29]=a;r[30]=a;r[31]=a;r[32]=a;r[33]=a;r[34]=a;r[35]=a;r[36]=a;r[37]=a;r[38]=a;r[39]=a;r[40]=a;r[41]=a;r[42]=a;r[43]=a;r[44]=a;r[45]=a;r[46]=a;r[47]=a;r[48]=a;r[49]=a;r[50]=a;r[51]=a;r[52]=a;r[53]=a;r[54]=a;r[55]=a;r[56]=a;r[57]=a;r[58]=a;r[59]=a;r[60]=a;r[61]=a;r[62]=a;r[63]=a;return r;}
forceinline u8x64 U8x64(u8 _0,u8 _1,u8 _2,u8 _3,u8 _4,u8 _5,u8 _6,u8 _7,u8 _8,u8 _9,u8 _10,u8 _11,u8 _12,u8 _13,u8 _14,u8 _15,u8 _16,u8 _17,u8 _18,u8 _19,u8 _20,u8 _21,u8 _22,u8 _23,u8 _24,u8 _25,u8 _26,u8 _27,u8 _28,u8 _29,u8 _30,u8 _31,u8 _32,u8 _33,u8 _34,u8 _35,u8 _36,u8 _37,u8 _38,u8 _39,u8 _40,u8 _41,u8 _42,u8 _43,u8 _44,u8 _45,u8 _46,u8 _47,u8 _48,u8 _49,u8 _50,u8 _51,u8 _52,u8 _53,u8 _54,u8 _55,u8 _56,u8 _57,u8 _58,u8 _59,u8 _60,u8 _61,u8 _62,u8 _63){u8x64 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;r[16]=_16;r[17]=_17;r[18]=_18;r[19]=_19;r[20]=_20;r[21]=_21;r[22]=_22;r[23]=_23;r[24]=_24;r[25]=_25;r[26]=_26;r[27]=_27;r[28]=_28;r[29]=_29;r[30]=_30;r[31]=_31;r[32]=_32;r[33]=_33;r[34]=_34;r[35]=_35;r[36]=_36;r[37]=_37;r[38]=_38;r[39]=_39;r[40]=_40;r[41]=_41;r[42]=_42;r[43]=_43;r[44]=_44;r[45]=_45;r[46]=_46;r[47]=_47;r[48]=_48;r[49]=_49;r[50]=_50;r[51]=_51;r[52]=_52;r[53]=_53;r[54]=_54;r[55]=_55;r[56]=_56;r[57]=_57;r[58]=_58;r[59]=_59;r[60]=_60;r[61]=_61;r[62]=_62;r[63]=_63;return r;}
#endif
forceinline u16x32 U16x32(u16);
forceinline u16x32 U16x32(u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16,u16);

union u16x32 {
#if ARCH_AVX512
	__m512i m;
#endif
	u16x16 x16[2];
	u16x8 x8[4];
	u16 s[32];
	forceinline u16 operator[](umm i) const { return s[i]; }
	forceinline u16 &operator[](umm i) { return s[i]; }
	forceinline friend u16x32 operator+(u16x32 a) { return a; }
	forceinline friend u16x32 operator-(u16x32 a) { return u16x32{} - a; }
#if ARCH_AVX512
	forceinline friend u16x32 operator~(u16x32 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend u16x32 operator~(u16x32 a) {a.x16[0] = ~a.x16[0]; a.x16[1] = ~a.x16[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u16x32 operator+(u16x32 a, u16x32 b) { return {_mm512_add_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator+(u16x32 a, u16x32 b) {a.x16[0] += b.x16[0]; a.x16[1] += b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator+(u16x32 a, u16 b) { return a+U16x32(b); }
	forceinline friend u16x32 operator+(u16 a, u16x32 b) { return U16x32(a)+b; }
	forceinline friend u16x32 &operator+=(u16x32 &a, u16x32 b) { return a=a+b; }
	forceinline friend u16x32 &operator+=(u16x32 &a, u16 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator-(u16x32 a, u16x32 b) { return {_mm512_sub_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator-(u16x32 a, u16x32 b) {a.x16[0] -= b.x16[0]; a.x16[1] -= b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator-(u16x32 a, u16 b) { return a-U16x32(b); }
	forceinline friend u16x32 operator-(u16 a, u16x32 b) { return U16x32(a)-b; }
	forceinline friend u16x32 &operator-=(u16x32 &a, u16x32 b) { return a=a-b; }
	forceinline friend u16x32 &operator-=(u16x32 &a, u16 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator*(u16x32 a, u16x32 b) { return {_mm512_mullo_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator*(u16x32 a, u16x32 b) {a.x16[0] *= b.x16[0]; a.x16[1] *= b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator*(u16x32 a, u16 b) { return a*U16x32(b); }
	forceinline friend u16x32 operator*(u16 a, u16x32 b) { return U16x32(a)*b; }
	forceinline friend u16x32 &operator*=(u16x32 &a, u16x32 b) { return a=a*b; }
	forceinline friend u16x32 &operator*=(u16x32 &a, u16 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator/(u16x32 a, u16x32 b) { return {_mm512_div_epu16(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator/(u16x32 a, u16x32 b) {a.x16[0] /= b.x16[0]; a.x16[1] /= b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator/(u16x32 a, u16 b) { return a/U16x32(b); }
	forceinline friend u16x32 operator/(u16 a, u16x32 b) { return U16x32(a)/b; }
	forceinline friend u16x32 &operator/=(u16x32 &a, u16x32 b) { return a=a/b; }
	forceinline friend u16x32 &operator/=(u16x32 &a, u16 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator^(u16x32 a, u16x32 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator^(u16x32 a, u16x32 b) {a.x16[0] ^= b.x16[0]; a.x16[1] ^= b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator^(u16x32 a, u16 b) { return a^U16x32(b); }
	forceinline friend u16x32 operator^(u16 a, u16x32 b) { return U16x32(a)^b; }
	forceinline friend u16x32 &operator^=(u16x32 &a, u16x32 b) { return a=a^b; }
	forceinline friend u16x32 &operator^=(u16x32 &a, u16 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator&(u16x32 a, u16x32 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator&(u16x32 a, u16x32 b) {a.x16[0] &= b.x16[0]; a.x16[1] &= b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator&(u16x32 a, u16 b) { return a&U16x32(b); }
	forceinline friend u16x32 operator&(u16 a, u16x32 b) { return U16x32(a)&b; }
	forceinline friend u16x32 &operator&=(u16x32 &a, u16x32 b) { return a=a&b; }
	forceinline friend u16x32 &operator&=(u16x32 &a, u16 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator|(u16x32 a, u16x32 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator|(u16x32 a, u16x32 b) {a.x16[0] |= b.x16[0]; a.x16[1] |= b.x16[1]; return a; }
#endif
	forceinline friend u16x32 operator|(u16x32 a, u16 b) { return a|U16x32(b); }
	forceinline friend u16x32 operator|(u16 a, u16x32 b) { return U16x32(a)|b; }
	forceinline friend u16x32 &operator|=(u16x32 &a, u16x32 b) { return a=a|b; }
	forceinline friend u16x32 &operator|=(u16x32 &a, u16 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator<<(u16x32 a, u16x32 b) { return {_mm512_sllv_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator<<(u16x32 a, u16x32 b) {a.x16[0] <<= b.x16[0]; a.x16[1] <<= b.x16[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u16x32 operator<<(u16x32 a, u16 b) { return {_mm512_slli_epi16(a.m, b)}; }
#else
	forceinline friend u16x32 operator<<(u16x32 a, u16 b) {a.x16[0] <<= b; a.x16[1] <<= b; return a; }
#endif
	forceinline friend u16x32 operator<<(u16 a, u16x32 b) { return U16x32(a)<<b; }
	forceinline friend u16x32 &operator<<=(u16x32 &a, u16x32 b) { return a=a<<b; }
	forceinline friend u16x32 &operator<<=(u16x32 &a, u16 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend u16x32 operator>>(u16x32 a, u16x32 b) { return {_mm512_srlv_epi16(a.m, b.m)}; }
#else
	forceinline friend u16x32 operator>>(u16x32 a, u16x32 b) {a.x16[0] >>= b.x16[0]; a.x16[1] >>= b.x16[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u16x32 operator>>(u16x32 a, u16 b) { return {_mm512_srli_epi16(a.m, b)}; }
#else
	forceinline friend u16x32 operator>>(u16x32 a, u16 b) {a.x16[0] >>= b; a.x16[1] >>= b; return a; }
#endif
	forceinline friend u16x32 operator>>(u16 a, u16x32 b) { return U16x32(a)>>b; }
	forceinline friend u16x32 &operator>>=(u16x32 &a, u16x32 b) { return a=a>>b; }
	forceinline friend u16x32 &operator>>=(u16x32 &a, u16 b) { return a=a>>b; }
	forceinline operator u8x32() const;
	forceinline operator s8x32() const;
	forceinline operator s16x32() const;
};

#if ARCH_AVX512
forceinline u16x32 U16x32(u16 a) { return {_mm512_set1_epi16(a)}; }
forceinline u16x32 U16x32(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7,u16 _8,u16 _9,u16 _10,u16 _11,u16 _12,u16 _13,u16 _14,u16 _15,u16 _16,u16 _17,u16 _18,u16 _19,u16 _20,u16 _21,u16 _22,u16 _23,u16 _24,u16 _25,u16 _26,u16 _27,u16 _28,u16 _29,u16 _30,u16 _31) { return {_mm512_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }
#else
forceinline u16x32 U16x32(u16 a){u16x32 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;r[16]=a;r[17]=a;r[18]=a;r[19]=a;r[20]=a;r[21]=a;r[22]=a;r[23]=a;r[24]=a;r[25]=a;r[26]=a;r[27]=a;r[28]=a;r[29]=a;r[30]=a;r[31]=a;return r;}
forceinline u16x32 U16x32(u16 _0,u16 _1,u16 _2,u16 _3,u16 _4,u16 _5,u16 _6,u16 _7,u16 _8,u16 _9,u16 _10,u16 _11,u16 _12,u16 _13,u16 _14,u16 _15,u16 _16,u16 _17,u16 _18,u16 _19,u16 _20,u16 _21,u16 _22,u16 _23,u16 _24,u16 _25,u16 _26,u16 _27,u16 _28,u16 _29,u16 _30,u16 _31){u16x32 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;r[16]=_16;r[17]=_17;r[18]=_18;r[19]=_19;r[20]=_20;r[21]=_21;r[22]=_22;r[23]=_23;r[24]=_24;r[25]=_25;r[26]=_26;r[27]=_27;r[28]=_28;r[29]=_29;r[30]=_30;r[31]=_31;return r;}
#endif
forceinline u32x16 U32x16(u32);
forceinline u32x16 U32x16(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);

union u32x16 {
#if ARCH_AVX512
	__m512i m;
#endif
	u32x8 x8[2];
	u32x4 x4[4];
	u32 s[16];
	forceinline u32 operator[](umm i) const { return s[i]; }
	forceinline u32 &operator[](umm i) { return s[i]; }
	forceinline friend u32x16 operator+(u32x16 a) { return a; }
	forceinline friend u32x16 operator-(u32x16 a) { return u32x16{} - a; }
#if ARCH_AVX512
	forceinline friend u32x16 operator~(u32x16 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend u32x16 operator~(u32x16 a) {a.x8[0] = ~a.x8[0]; a.x8[1] = ~a.x8[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u32x16 operator+(u32x16 a, u32x16 b) { return {_mm512_add_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator+(u32x16 a, u32x16 b) {a.x8[0] += b.x8[0]; a.x8[1] += b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator+(u32x16 a, u32 b) { return a+U32x16(b); }
	forceinline friend u32x16 operator+(u32 a, u32x16 b) { return U32x16(a)+b; }
	forceinline friend u32x16 &operator+=(u32x16 &a, u32x16 b) { return a=a+b; }
	forceinline friend u32x16 &operator+=(u32x16 &a, u32 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator-(u32x16 a, u32x16 b) { return {_mm512_sub_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator-(u32x16 a, u32x16 b) {a.x8[0] -= b.x8[0]; a.x8[1] -= b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator-(u32x16 a, u32 b) { return a-U32x16(b); }
	forceinline friend u32x16 operator-(u32 a, u32x16 b) { return U32x16(a)-b; }
	forceinline friend u32x16 &operator-=(u32x16 &a, u32x16 b) { return a=a-b; }
	forceinline friend u32x16 &operator-=(u32x16 &a, u32 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator*(u32x16 a, u32x16 b) { return {_mm512_mullo_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator*(u32x16 a, u32x16 b) {a.x8[0] *= b.x8[0]; a.x8[1] *= b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator*(u32x16 a, u32 b) { return a*U32x16(b); }
	forceinline friend u32x16 operator*(u32 a, u32x16 b) { return U32x16(a)*b; }
	forceinline friend u32x16 &operator*=(u32x16 &a, u32x16 b) { return a=a*b; }
	forceinline friend u32x16 &operator*=(u32x16 &a, u32 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator/(u32x16 a, u32x16 b) { return {_mm512_div_epu32(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator/(u32x16 a, u32x16 b) {a.x8[0] /= b.x8[0]; a.x8[1] /= b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator/(u32x16 a, u32 b) { return a/U32x16(b); }
	forceinline friend u32x16 operator/(u32 a, u32x16 b) { return U32x16(a)/b; }
	forceinline friend u32x16 &operator/=(u32x16 &a, u32x16 b) { return a=a/b; }
	forceinline friend u32x16 &operator/=(u32x16 &a, u32 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator^(u32x16 a, u32x16 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator^(u32x16 a, u32x16 b) {a.x8[0] ^= b.x8[0]; a.x8[1] ^= b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator^(u32x16 a, u32 b) { return a^U32x16(b); }
	forceinline friend u32x16 operator^(u32 a, u32x16 b) { return U32x16(a)^b; }
	forceinline friend u32x16 &operator^=(u32x16 &a, u32x16 b) { return a=a^b; }
	forceinline friend u32x16 &operator^=(u32x16 &a, u32 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator&(u32x16 a, u32x16 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator&(u32x16 a, u32x16 b) {a.x8[0] &= b.x8[0]; a.x8[1] &= b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator&(u32x16 a, u32 b) { return a&U32x16(b); }
	forceinline friend u32x16 operator&(u32 a, u32x16 b) { return U32x16(a)&b; }
	forceinline friend u32x16 &operator&=(u32x16 &a, u32x16 b) { return a=a&b; }
	forceinline friend u32x16 &operator&=(u32x16 &a, u32 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator|(u32x16 a, u32x16 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator|(u32x16 a, u32x16 b) {a.x8[0] |= b.x8[0]; a.x8[1] |= b.x8[1]; return a; }
#endif
	forceinline friend u32x16 operator|(u32x16 a, u32 b) { return a|U32x16(b); }
	forceinline friend u32x16 operator|(u32 a, u32x16 b) { return U32x16(a)|b; }
	forceinline friend u32x16 &operator|=(u32x16 &a, u32x16 b) { return a=a|b; }
	forceinline friend u32x16 &operator|=(u32x16 &a, u32 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator<<(u32x16 a, u32x16 b) { return {_mm512_sllv_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator<<(u32x16 a, u32x16 b) {a.x8[0] <<= b.x8[0]; a.x8[1] <<= b.x8[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u32x16 operator<<(u32x16 a, u32 b) { return {_mm512_slli_epi32(a.m, b)}; }
#else
	forceinline friend u32x16 operator<<(u32x16 a, u32 b) {a.x8[0] <<= b; a.x8[1] <<= b; return a; }
#endif
	forceinline friend u32x16 operator<<(u32 a, u32x16 b) { return U32x16(a)<<b; }
	forceinline friend u32x16 &operator<<=(u32x16 &a, u32x16 b) { return a=a<<b; }
	forceinline friend u32x16 &operator<<=(u32x16 &a, u32 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend u32x16 operator>>(u32x16 a, u32x16 b) { return {_mm512_srlv_epi32(a.m, b.m)}; }
#else
	forceinline friend u32x16 operator>>(u32x16 a, u32x16 b) {a.x8[0] >>= b.x8[0]; a.x8[1] >>= b.x8[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u32x16 operator>>(u32x16 a, u32 b) { return {_mm512_srli_epi32(a.m, b)}; }
#else
	forceinline friend u32x16 operator>>(u32x16 a, u32 b) {a.x8[0] >>= b; a.x8[1] >>= b; return a; }
#endif
	forceinline friend u32x16 operator>>(u32 a, u32x16 b) { return U32x16(a)>>b; }
	forceinline friend u32x16 &operator>>=(u32x16 &a, u32x16 b) { return a=a>>b; }
	forceinline friend u32x16 &operator>>=(u32x16 &a, u32 b) { return a=a>>b; }
	forceinline operator u8x16() const;
	forceinline operator u16x16() const;
	forceinline operator s8x16() const;
	forceinline operator s16x16() const;
	forceinline operator s32x16() const;
	forceinline operator f32x16() const;
};

#if ARCH_AVX512
forceinline u32x16 U32x16(u32 a) { return {_mm512_set1_epi32(a)}; }
forceinline u32x16 U32x16(u32 _0,u32 _1,u32 _2,u32 _3,u32 _4,u32 _5,u32 _6,u32 _7,u32 _8,u32 _9,u32 _10,u32 _11,u32 _12,u32 _13,u32 _14,u32 _15) { return {_mm512_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
#else
forceinline u32x16 U32x16(u32 a){u32x16 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;return r;}
forceinline u32x16 U32x16(u32 _0,u32 _1,u32 _2,u32 _3,u32 _4,u32 _5,u32 _6,u32 _7,u32 _8,u32 _9,u32 _10,u32 _11,u32 _12,u32 _13,u32 _14,u32 _15){u32x16 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;return r;}
#endif
forceinline u64x8 U64x8(u64);
forceinline u64x8 U64x8(u64,u64,u64,u64,u64,u64,u64,u64);

union u64x8 {
#if ARCH_AVX512
	__m512i m;
#endif
	u64x4 x4[2];
	u64x2 x2[4];
	u64 s[8];
	forceinline u64 operator[](umm i) const { return s[i]; }
	forceinline u64 &operator[](umm i) { return s[i]; }
	forceinline friend u64x8 operator+(u64x8 a) { return a; }
	forceinline friend u64x8 operator-(u64x8 a) { return u64x8{} - a; }
#if ARCH_AVX512
	forceinline friend u64x8 operator~(u64x8 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend u64x8 operator~(u64x8 a) {a.x4[0] = ~a.x4[0]; a.x4[1] = ~a.x4[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u64x8 operator+(u64x8 a, u64x8 b) { return {_mm512_add_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator+(u64x8 a, u64x8 b) {a.x4[0] += b.x4[0]; a.x4[1] += b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator+(u64x8 a, u64 b) { return a+U64x8(b); }
	forceinline friend u64x8 operator+(u64 a, u64x8 b) { return U64x8(a)+b; }
	forceinline friend u64x8 &operator+=(u64x8 &a, u64x8 b) { return a=a+b; }
	forceinline friend u64x8 &operator+=(u64x8 &a, u64 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator-(u64x8 a, u64x8 b) { return {_mm512_sub_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator-(u64x8 a, u64x8 b) {a.x4[0] -= b.x4[0]; a.x4[1] -= b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator-(u64x8 a, u64 b) { return a-U64x8(b); }
	forceinline friend u64x8 operator-(u64 a, u64x8 b) { return U64x8(a)-b; }
	forceinline friend u64x8 &operator-=(u64x8 &a, u64x8 b) { return a=a-b; }
	forceinline friend u64x8 &operator-=(u64x8 &a, u64 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator*(u64x8 a, u64x8 b) { return {_mm512_mullo_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator*(u64x8 a, u64x8 b) {a.x4[0] *= b.x4[0]; a.x4[1] *= b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator*(u64x8 a, u64 b) { return a*U64x8(b); }
	forceinline friend u64x8 operator*(u64 a, u64x8 b) { return U64x8(a)*b; }
	forceinline friend u64x8 &operator*=(u64x8 &a, u64x8 b) { return a=a*b; }
	forceinline friend u64x8 &operator*=(u64x8 &a, u64 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator/(u64x8 a, u64x8 b) { return {_mm512_div_epu64(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator/(u64x8 a, u64x8 b) {a.x4[0] /= b.x4[0]; a.x4[1] /= b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator/(u64x8 a, u64 b) { return a/U64x8(b); }
	forceinline friend u64x8 operator/(u64 a, u64x8 b) { return U64x8(a)/b; }
	forceinline friend u64x8 &operator/=(u64x8 &a, u64x8 b) { return a=a/b; }
	forceinline friend u64x8 &operator/=(u64x8 &a, u64 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator^(u64x8 a, u64x8 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator^(u64x8 a, u64x8 b) {a.x4[0] ^= b.x4[0]; a.x4[1] ^= b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator^(u64x8 a, u64 b) { return a^U64x8(b); }
	forceinline friend u64x8 operator^(u64 a, u64x8 b) { return U64x8(a)^b; }
	forceinline friend u64x8 &operator^=(u64x8 &a, u64x8 b) { return a=a^b; }
	forceinline friend u64x8 &operator^=(u64x8 &a, u64 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator&(u64x8 a, u64x8 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator&(u64x8 a, u64x8 b) {a.x4[0] &= b.x4[0]; a.x4[1] &= b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator&(u64x8 a, u64 b) { return a&U64x8(b); }
	forceinline friend u64x8 operator&(u64 a, u64x8 b) { return U64x8(a)&b; }
	forceinline friend u64x8 &operator&=(u64x8 &a, u64x8 b) { return a=a&b; }
	forceinline friend u64x8 &operator&=(u64x8 &a, u64 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator|(u64x8 a, u64x8 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator|(u64x8 a, u64x8 b) {a.x4[0] |= b.x4[0]; a.x4[1] |= b.x4[1]; return a; }
#endif
	forceinline friend u64x8 operator|(u64x8 a, u64 b) { return a|U64x8(b); }
	forceinline friend u64x8 operator|(u64 a, u64x8 b) { return U64x8(a)|b; }
	forceinline friend u64x8 &operator|=(u64x8 &a, u64x8 b) { return a=a|b; }
	forceinline friend u64x8 &operator|=(u64x8 &a, u64 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator<<(u64x8 a, u64x8 b) { return {_mm512_sllv_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator<<(u64x8 a, u64x8 b) {a.x4[0] <<= b.x4[0]; a.x4[1] <<= b.x4[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u64x8 operator<<(u64x8 a, u64 b) { return {_mm512_slli_epi64(a.m, b)}; }
#else
	forceinline friend u64x8 operator<<(u64x8 a, u64 b) {a.x4[0] <<= b; a.x4[1] <<= b; return a; }
#endif
	forceinline friend u64x8 operator<<(u64 a, u64x8 b) { return U64x8(a)<<b; }
	forceinline friend u64x8 &operator<<=(u64x8 &a, u64x8 b) { return a=a<<b; }
	forceinline friend u64x8 &operator<<=(u64x8 &a, u64 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend u64x8 operator>>(u64x8 a, u64x8 b) { return {_mm512_srlv_epi64(a.m, b.m)}; }
#else
	forceinline friend u64x8 operator>>(u64x8 a, u64x8 b) {a.x4[0] >>= b.x4[0]; a.x4[1] >>= b.x4[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend u64x8 operator>>(u64x8 a, u64 b) { return {_mm512_srli_epi64(a.m, b)}; }
#else
	forceinline friend u64x8 operator>>(u64x8 a, u64 b) {a.x4[0] >>= b; a.x4[1] >>= b; return a; }
#endif
	forceinline friend u64x8 operator>>(u64 a, u64x8 b) { return U64x8(a)>>b; }
	forceinline friend u64x8 &operator>>=(u64x8 &a, u64x8 b) { return a=a>>b; }
	forceinline friend u64x8 &operator>>=(u64x8 &a, u64 b) { return a=a>>b; }
	forceinline operator u16x8() const;
	forceinline operator u32x8() const;
	forceinline operator s16x8() const;
	forceinline operator s32x8() const;
	forceinline operator s64x8() const;
	forceinline operator f32x8() const;
	forceinline operator f64x8() const;
};

#if ARCH_AVX512
forceinline u64x8 U64x8(u64 a) { return {_mm512_set1_epi64(a)}; }
forceinline u64x8 U64x8(u64 _0,u64 _1,u64 _2,u64 _3,u64 _4,u64 _5,u64 _6,u64 _7) { return {_mm512_setr_epi64(_0,_1,_2,_3,_4,_5,_6,_7)}; }
#else
forceinline u64x8 U64x8(u64 a){u64x8 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;return r;}
forceinline u64x8 U64x8(u64 _0,u64 _1,u64 _2,u64 _3,u64 _4,u64 _5,u64 _6,u64 _7){u64x8 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;return r;}
#endif
forceinline s8x64 S8x64(s8);
forceinline s8x64 S8x64(s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8,s8);

union s8x64 {
#if ARCH_AVX512
	__m512i m;
#endif
	s8x32 x32[2];
	s8x16 x16[4];
	s8 s[64];
	forceinline s8 operator[](umm i) const { return s[i]; }
	forceinline s8 &operator[](umm i) { return s[i]; }
	forceinline friend s8x64 operator+(s8x64 a) { return a; }
	forceinline friend s8x64 operator-(s8x64 a) { return s8x64{} - a; }
#if ARCH_AVX512
	forceinline friend s8x64 operator~(s8x64 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend s8x64 operator~(s8x64 a) {a.x32[0] = ~a.x32[0]; a.x32[1] = ~a.x32[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s8x64 operator+(s8x64 a, s8x64 b) { return {_mm512_add_epi8(a.m, b.m)}; }
#else
	forceinline friend s8x64 operator+(s8x64 a, s8x64 b) {a.x32[0] += b.x32[0]; a.x32[1] += b.x32[1]; return a; }
#endif
	forceinline friend s8x64 operator+(s8x64 a, s8 b) { return a+S8x64(b); }
	forceinline friend s8x64 operator+(s8 a, s8x64 b) { return S8x64(a)+b; }
	forceinline friend s8x64 &operator+=(s8x64 &a, s8x64 b) { return a=a+b; }
	forceinline friend s8x64 &operator+=(s8x64 &a, s8 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend s8x64 operator-(s8x64 a, s8x64 b) { return {_mm512_sub_epi8(a.m, b.m)}; }
#else
	forceinline friend s8x64 operator-(s8x64 a, s8x64 b) {a.x32[0] -= b.x32[0]; a.x32[1] -= b.x32[1]; return a; }
#endif
	forceinline friend s8x64 operator-(s8x64 a, s8 b) { return a-S8x64(b); }
	forceinline friend s8x64 operator-(s8 a, s8x64 b) { return S8x64(a)-b; }
	forceinline friend s8x64 &operator-=(s8x64 &a, s8x64 b) { return a=a-b; }
	forceinline friend s8x64 &operator-=(s8x64 &a, s8 b) { return a=a-b; }
	forceinline friend s8x64 operator*(s8x64 a, s8x64 b) {a.s[0] *= b.s[0]; a.s[1] *= b.s[1]; a.s[2] *= b.s[2]; a.s[3] *= b.s[3]; a.s[4] *= b.s[4]; a.s[5] *= b.s[5]; a.s[6] *= b.s[6]; a.s[7] *= b.s[7]; a.s[8] *= b.s[8]; a.s[9] *= b.s[9]; a.s[10] *= b.s[10]; a.s[11] *= b.s[11]; a.s[12] *= b.s[12]; a.s[13] *= b.s[13]; a.s[14] *= b.s[14]; a.s[15] *= b.s[15]; a.s[16] *= b.s[16]; a.s[17] *= b.s[17]; a.s[18] *= b.s[18]; a.s[19] *= b.s[19]; a.s[20] *= b.s[20]; a.s[21] *= b.s[21]; a.s[22] *= b.s[22]; a.s[23] *= b.s[23]; a.s[24] *= b.s[24]; a.s[25] *= b.s[25]; a.s[26] *= b.s[26]; a.s[27] *= b.s[27]; a.s[28] *= b.s[28]; a.s[29] *= b.s[29]; a.s[30] *= b.s[30]; a.s[31] *= b.s[31]; a.s[32] *= b.s[32]; a.s[33] *= b.s[33]; a.s[34] *= b.s[34]; a.s[35] *= b.s[35]; a.s[36] *= b.s[36]; a.s[37] *= b.s[37]; a.s[38] *= b.s[38]; a.s[39] *= b.s[39]; a.s[40] *= b.s[40]; a.s[41] *= b.s[41]; a.s[42] *= b.s[42]; a.s[43] *= b.s[43]; a.s[44] *= b.s[44]; a.s[45] *= b.s[45]; a.s[46] *= b.s[46]; a.s[47] *= b.s[47]; a.s[48] *= b.s[48]; a.s[49] *= b.s[49]; a.s[50] *= b.s[50]; a.s[51] *= b.s[51]; a.s[52] *= b.s[52]; a.s[53] *= b.s[53]; a.s[54] *= b.s[54]; a.s[55] *= b.s[55]; a.s[56] *= b.s[56]; a.s[57] *= b.s[57]; a.s[58] *= b.s[58]; a.s[59] *= b.s[59]; a.s[60] *= b.s[60]; a.s[61] *= b.s[61]; a.s[62] *= b.s[62]; a.s[63] *= b.s[63]; return a; }
	forceinline friend s8x64 operator*(s8x64 a, s8 b) {a.s[0] *= b; a.s[1] *= b; a.s[2] *= b; a.s[3] *= b; a.s[4] *= b; a.s[5] *= b; a.s[6] *= b; a.s[7] *= b; a.s[8] *= b; a.s[9] *= b; a.s[10] *= b; a.s[11] *= b; a.s[12] *= b; a.s[13] *= b; a.s[14] *= b; a.s[15] *= b; a.s[16] *= b; a.s[17] *= b; a.s[18] *= b; a.s[19] *= b; a.s[20] *= b; a.s[21] *= b; a.s[22] *= b; a.s[23] *= b; a.s[24] *= b; a.s[25] *= b; a.s[26] *= b; a.s[27] *= b; a.s[28] *= b; a.s[29] *= b; a.s[30] *= b; a.s[31] *= b; a.s[32] *= b; a.s[33] *= b; a.s[34] *= b; a.s[35] *= b; a.s[36] *= b; a.s[37] *= b; a.s[38] *= b; a.s[39] *= b; a.s[40] *= b; a.s[41] *= b; a.s[42] *= b; a.s[43] *= b; a.s[44] *= b; a.s[45] *= b; a.s[46] *= b; a.s[47] *= b; a.s[48] *= b; a.s[49] *= b; a.s[50] *= b; a.s[51] *= b; a.s[52] *= b; a.s[53] *= b; a.s[54] *= b; a.s[55] *= b; a.s[56] *= b; a.s[57] *= b; a.s[58] *= b; a.s[59] *= b; a.s[60] *= b; a.s[61] *= b; a.s[62] *= b; a.s[63] *= b; return a; }
	forceinline friend s8x64 operator*(s8 a, s8x64 b) {auto r=S8x64(a);r.s[0] *= b.s[0]; r.s[1] *= b.s[1]; r.s[2] *= b.s[2]; r.s[3] *= b.s[3]; r.s[4] *= b.s[4]; r.s[5] *= b.s[5]; r.s[6] *= b.s[6]; r.s[7] *= b.s[7]; r.s[8] *= b.s[8]; r.s[9] *= b.s[9]; r.s[10] *= b.s[10]; r.s[11] *= b.s[11]; r.s[12] *= b.s[12]; r.s[13] *= b.s[13]; r.s[14] *= b.s[14]; r.s[15] *= b.s[15]; r.s[16] *= b.s[16]; r.s[17] *= b.s[17]; r.s[18] *= b.s[18]; r.s[19] *= b.s[19]; r.s[20] *= b.s[20]; r.s[21] *= b.s[21]; r.s[22] *= b.s[22]; r.s[23] *= b.s[23]; r.s[24] *= b.s[24]; r.s[25] *= b.s[25]; r.s[26] *= b.s[26]; r.s[27] *= b.s[27]; r.s[28] *= b.s[28]; r.s[29] *= b.s[29]; r.s[30] *= b.s[30]; r.s[31] *= b.s[31]; r.s[32] *= b.s[32]; r.s[33] *= b.s[33]; r.s[34] *= b.s[34]; r.s[35] *= b.s[35]; r.s[36] *= b.s[36]; r.s[37] *= b.s[37]; r.s[38] *= b.s[38]; r.s[39] *= b.s[39]; r.s[40] *= b.s[40]; r.s[41] *= b.s[41]; r.s[42] *= b.s[42]; r.s[43] *= b.s[43]; r.s[44] *= b.s[44]; r.s[45] *= b.s[45]; r.s[46] *= b.s[46]; r.s[47] *= b.s[47]; r.s[48] *= b.s[48]; r.s[49] *= b.s[49]; r.s[50] *= b.s[50]; r.s[51] *= b.s[51]; r.s[52] *= b.s[52]; r.s[53] *= b.s[53]; r.s[54] *= b.s[54]; r.s[55] *= b.s[55]; r.s[56] *= b.s[56]; r.s[57] *= b.s[57]; r.s[58] *= b.s[58]; r.s[59] *= b.s[59]; r.s[60] *= b.s[60]; r.s[61] *= b.s[61]; r.s[62] *= b.s[62]; r.s[63] *= b.s[63]; return r; }
	forceinline friend s8x64 &operator*=(s8x64 &a, s8x64 b) { return a=a*b; }
	forceinline friend s8x64 &operator*=(s8x64 &a, s8 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend s8x64 operator/(s8x64 a, s8x64 b) { return {_mm512_div_epi8(a.m, b.m)}; }
#else
	forceinline friend s8x64 operator/(s8x64 a, s8x64 b) {a.x32[0] /= b.x32[0]; a.x32[1] /= b.x32[1]; return a; }
#endif
	forceinline friend s8x64 operator/(s8x64 a, s8 b) { return a/S8x64(b); }
	forceinline friend s8x64 operator/(s8 a, s8x64 b) { return S8x64(a)/b; }
	forceinline friend s8x64 &operator/=(s8x64 &a, s8x64 b) { return a=a/b; }
	forceinline friend s8x64 &operator/=(s8x64 &a, s8 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend s8x64 operator^(s8x64 a, s8x64 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend s8x64 operator^(s8x64 a, s8x64 b) {a.x32[0] ^= b.x32[0]; a.x32[1] ^= b.x32[1]; return a; }
#endif
	forceinline friend s8x64 operator^(s8x64 a, s8 b) { return a^S8x64(b); }
	forceinline friend s8x64 operator^(s8 a, s8x64 b) { return S8x64(a)^b; }
	forceinline friend s8x64 &operator^=(s8x64 &a, s8x64 b) { return a=a^b; }
	forceinline friend s8x64 &operator^=(s8x64 &a, s8 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend s8x64 operator&(s8x64 a, s8x64 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend s8x64 operator&(s8x64 a, s8x64 b) {a.x32[0] &= b.x32[0]; a.x32[1] &= b.x32[1]; return a; }
#endif
	forceinline friend s8x64 operator&(s8x64 a, s8 b) { return a&S8x64(b); }
	forceinline friend s8x64 operator&(s8 a, s8x64 b) { return S8x64(a)&b; }
	forceinline friend s8x64 &operator&=(s8x64 &a, s8x64 b) { return a=a&b; }
	forceinline friend s8x64 &operator&=(s8x64 &a, s8 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend s8x64 operator|(s8x64 a, s8x64 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend s8x64 operator|(s8x64 a, s8x64 b) {a.x32[0] |= b.x32[0]; a.x32[1] |= b.x32[1]; return a; }
#endif
	forceinline friend s8x64 operator|(s8x64 a, s8 b) { return a|S8x64(b); }
	forceinline friend s8x64 operator|(s8 a, s8x64 b) { return S8x64(a)|b; }
	forceinline friend s8x64 &operator|=(s8x64 &a, s8x64 b) { return a=a|b; }
	forceinline friend s8x64 &operator|=(s8x64 &a, s8 b) { return a=a|b; }
	forceinline friend s8x64 operator<<(s8x64 a, s8x64 b) {a.s[0] <<= b.s[0]; a.s[1] <<= b.s[1]; a.s[2] <<= b.s[2]; a.s[3] <<= b.s[3]; a.s[4] <<= b.s[4]; a.s[5] <<= b.s[5]; a.s[6] <<= b.s[6]; a.s[7] <<= b.s[7]; a.s[8] <<= b.s[8]; a.s[9] <<= b.s[9]; a.s[10] <<= b.s[10]; a.s[11] <<= b.s[11]; a.s[12] <<= b.s[12]; a.s[13] <<= b.s[13]; a.s[14] <<= b.s[14]; a.s[15] <<= b.s[15]; a.s[16] <<= b.s[16]; a.s[17] <<= b.s[17]; a.s[18] <<= b.s[18]; a.s[19] <<= b.s[19]; a.s[20] <<= b.s[20]; a.s[21] <<= b.s[21]; a.s[22] <<= b.s[22]; a.s[23] <<= b.s[23]; a.s[24] <<= b.s[24]; a.s[25] <<= b.s[25]; a.s[26] <<= b.s[26]; a.s[27] <<= b.s[27]; a.s[28] <<= b.s[28]; a.s[29] <<= b.s[29]; a.s[30] <<= b.s[30]; a.s[31] <<= b.s[31]; a.s[32] <<= b.s[32]; a.s[33] <<= b.s[33]; a.s[34] <<= b.s[34]; a.s[35] <<= b.s[35]; a.s[36] <<= b.s[36]; a.s[37] <<= b.s[37]; a.s[38] <<= b.s[38]; a.s[39] <<= b.s[39]; a.s[40] <<= b.s[40]; a.s[41] <<= b.s[41]; a.s[42] <<= b.s[42]; a.s[43] <<= b.s[43]; a.s[44] <<= b.s[44]; a.s[45] <<= b.s[45]; a.s[46] <<= b.s[46]; a.s[47] <<= b.s[47]; a.s[48] <<= b.s[48]; a.s[49] <<= b.s[49]; a.s[50] <<= b.s[50]; a.s[51] <<= b.s[51]; a.s[52] <<= b.s[52]; a.s[53] <<= b.s[53]; a.s[54] <<= b.s[54]; a.s[55] <<= b.s[55]; a.s[56] <<= b.s[56]; a.s[57] <<= b.s[57]; a.s[58] <<= b.s[58]; a.s[59] <<= b.s[59]; a.s[60] <<= b.s[60]; a.s[61] <<= b.s[61]; a.s[62] <<= b.s[62]; a.s[63] <<= b.s[63]; return a; }
	forceinline friend s8x64 operator<<(s8x64 a, s8 b) {a.s[0] <<= b; a.s[1] <<= b; a.s[2] <<= b; a.s[3] <<= b; a.s[4] <<= b; a.s[5] <<= b; a.s[6] <<= b; a.s[7] <<= b; a.s[8] <<= b; a.s[9] <<= b; a.s[10] <<= b; a.s[11] <<= b; a.s[12] <<= b; a.s[13] <<= b; a.s[14] <<= b; a.s[15] <<= b; a.s[16] <<= b; a.s[17] <<= b; a.s[18] <<= b; a.s[19] <<= b; a.s[20] <<= b; a.s[21] <<= b; a.s[22] <<= b; a.s[23] <<= b; a.s[24] <<= b; a.s[25] <<= b; a.s[26] <<= b; a.s[27] <<= b; a.s[28] <<= b; a.s[29] <<= b; a.s[30] <<= b; a.s[31] <<= b; a.s[32] <<= b; a.s[33] <<= b; a.s[34] <<= b; a.s[35] <<= b; a.s[36] <<= b; a.s[37] <<= b; a.s[38] <<= b; a.s[39] <<= b; a.s[40] <<= b; a.s[41] <<= b; a.s[42] <<= b; a.s[43] <<= b; a.s[44] <<= b; a.s[45] <<= b; a.s[46] <<= b; a.s[47] <<= b; a.s[48] <<= b; a.s[49] <<= b; a.s[50] <<= b; a.s[51] <<= b; a.s[52] <<= b; a.s[53] <<= b; a.s[54] <<= b; a.s[55] <<= b; a.s[56] <<= b; a.s[57] <<= b; a.s[58] <<= b; a.s[59] <<= b; a.s[60] <<= b; a.s[61] <<= b; a.s[62] <<= b; a.s[63] <<= b; return a; }
	forceinline friend s8x64 operator<<(s8 a, s8x64 b) {auto r=S8x64(a);r.s[0] <<= b.s[0]; r.s[1] <<= b.s[1]; r.s[2] <<= b.s[2]; r.s[3] <<= b.s[3]; r.s[4] <<= b.s[4]; r.s[5] <<= b.s[5]; r.s[6] <<= b.s[6]; r.s[7] <<= b.s[7]; r.s[8] <<= b.s[8]; r.s[9] <<= b.s[9]; r.s[10] <<= b.s[10]; r.s[11] <<= b.s[11]; r.s[12] <<= b.s[12]; r.s[13] <<= b.s[13]; r.s[14] <<= b.s[14]; r.s[15] <<= b.s[15]; r.s[16] <<= b.s[16]; r.s[17] <<= b.s[17]; r.s[18] <<= b.s[18]; r.s[19] <<= b.s[19]; r.s[20] <<= b.s[20]; r.s[21] <<= b.s[21]; r.s[22] <<= b.s[22]; r.s[23] <<= b.s[23]; r.s[24] <<= b.s[24]; r.s[25] <<= b.s[25]; r.s[26] <<= b.s[26]; r.s[27] <<= b.s[27]; r.s[28] <<= b.s[28]; r.s[29] <<= b.s[29]; r.s[30] <<= b.s[30]; r.s[31] <<= b.s[31]; r.s[32] <<= b.s[32]; r.s[33] <<= b.s[33]; r.s[34] <<= b.s[34]; r.s[35] <<= b.s[35]; r.s[36] <<= b.s[36]; r.s[37] <<= b.s[37]; r.s[38] <<= b.s[38]; r.s[39] <<= b.s[39]; r.s[40] <<= b.s[40]; r.s[41] <<= b.s[41]; r.s[42] <<= b.s[42]; r.s[43] <<= b.s[43]; r.s[44] <<= b.s[44]; r.s[45] <<= b.s[45]; r.s[46] <<= b.s[46]; r.s[47] <<= b.s[47]; r.s[48] <<= b.s[48]; r.s[49] <<= b.s[49]; r.s[50] <<= b.s[50]; r.s[51] <<= b.s[51]; r.s[52] <<= b.s[52]; r.s[53] <<= b.s[53]; r.s[54] <<= b.s[54]; r.s[55] <<= b.s[55]; r.s[56] <<= b.s[56]; r.s[57] <<= b.s[57]; r.s[58] <<= b.s[58]; r.s[59] <<= b.s[59]; r.s[60] <<= b.s[60]; r.s[61] <<= b.s[61]; r.s[62] <<= b.s[62]; r.s[63] <<= b.s[63]; return r; }
	forceinline friend s8x64 &operator<<=(s8x64 &a, s8x64 b) { return a=a<<b; }
	forceinline friend s8x64 &operator<<=(s8x64 &a, s8 b) { return a=a<<b; }
	forceinline friend s8x64 operator>>(s8x64 a, s8x64 b) {a.s[0] >>= b.s[0]; a.s[1] >>= b.s[1]; a.s[2] >>= b.s[2]; a.s[3] >>= b.s[3]; a.s[4] >>= b.s[4]; a.s[5] >>= b.s[5]; a.s[6] >>= b.s[6]; a.s[7] >>= b.s[7]; a.s[8] >>= b.s[8]; a.s[9] >>= b.s[9]; a.s[10] >>= b.s[10]; a.s[11] >>= b.s[11]; a.s[12] >>= b.s[12]; a.s[13] >>= b.s[13]; a.s[14] >>= b.s[14]; a.s[15] >>= b.s[15]; a.s[16] >>= b.s[16]; a.s[17] >>= b.s[17]; a.s[18] >>= b.s[18]; a.s[19] >>= b.s[19]; a.s[20] >>= b.s[20]; a.s[21] >>= b.s[21]; a.s[22] >>= b.s[22]; a.s[23] >>= b.s[23]; a.s[24] >>= b.s[24]; a.s[25] >>= b.s[25]; a.s[26] >>= b.s[26]; a.s[27] >>= b.s[27]; a.s[28] >>= b.s[28]; a.s[29] >>= b.s[29]; a.s[30] >>= b.s[30]; a.s[31] >>= b.s[31]; a.s[32] >>= b.s[32]; a.s[33] >>= b.s[33]; a.s[34] >>= b.s[34]; a.s[35] >>= b.s[35]; a.s[36] >>= b.s[36]; a.s[37] >>= b.s[37]; a.s[38] >>= b.s[38]; a.s[39] >>= b.s[39]; a.s[40] >>= b.s[40]; a.s[41] >>= b.s[41]; a.s[42] >>= b.s[42]; a.s[43] >>= b.s[43]; a.s[44] >>= b.s[44]; a.s[45] >>= b.s[45]; a.s[46] >>= b.s[46]; a.s[47] >>= b.s[47]; a.s[48] >>= b.s[48]; a.s[49] >>= b.s[49]; a.s[50] >>= b.s[50]; a.s[51] >>= b.s[51]; a.s[52] >>= b.s[52]; a.s[53] >>= b.s[53]; a.s[54] >>= b.s[54]; a.s[55] >>= b.s[55]; a.s[56] >>= b.s[56]; a.s[57] >>= b.s[57]; a.s[58] >>= b.s[58]; a.s[59] >>= b.s[59]; a.s[60] >>= b.s[60]; a.s[61] >>= b.s[61]; a.s[62] >>= b.s[62]; a.s[63] >>= b.s[63]; return a; }
	forceinline friend s8x64 operator>>(s8x64 a, s8 b) {a.s[0] >>= b; a.s[1] >>= b; a.s[2] >>= b; a.s[3] >>= b; a.s[4] >>= b; a.s[5] >>= b; a.s[6] >>= b; a.s[7] >>= b; a.s[8] >>= b; a.s[9] >>= b; a.s[10] >>= b; a.s[11] >>= b; a.s[12] >>= b; a.s[13] >>= b; a.s[14] >>= b; a.s[15] >>= b; a.s[16] >>= b; a.s[17] >>= b; a.s[18] >>= b; a.s[19] >>= b; a.s[20] >>= b; a.s[21] >>= b; a.s[22] >>= b; a.s[23] >>= b; a.s[24] >>= b; a.s[25] >>= b; a.s[26] >>= b; a.s[27] >>= b; a.s[28] >>= b; a.s[29] >>= b; a.s[30] >>= b; a.s[31] >>= b; a.s[32] >>= b; a.s[33] >>= b; a.s[34] >>= b; a.s[35] >>= b; a.s[36] >>= b; a.s[37] >>= b; a.s[38] >>= b; a.s[39] >>= b; a.s[40] >>= b; a.s[41] >>= b; a.s[42] >>= b; a.s[43] >>= b; a.s[44] >>= b; a.s[45] >>= b; a.s[46] >>= b; a.s[47] >>= b; a.s[48] >>= b; a.s[49] >>= b; a.s[50] >>= b; a.s[51] >>= b; a.s[52] >>= b; a.s[53] >>= b; a.s[54] >>= b; a.s[55] >>= b; a.s[56] >>= b; a.s[57] >>= b; a.s[58] >>= b; a.s[59] >>= b; a.s[60] >>= b; a.s[61] >>= b; a.s[62] >>= b; a.s[63] >>= b; return a; }
	forceinline friend s8x64 operator>>(s8 a, s8x64 b) {auto r=S8x64(a);r.s[0] >>= b.s[0]; r.s[1] >>= b.s[1]; r.s[2] >>= b.s[2]; r.s[3] >>= b.s[3]; r.s[4] >>= b.s[4]; r.s[5] >>= b.s[5]; r.s[6] >>= b.s[6]; r.s[7] >>= b.s[7]; r.s[8] >>= b.s[8]; r.s[9] >>= b.s[9]; r.s[10] >>= b.s[10]; r.s[11] >>= b.s[11]; r.s[12] >>= b.s[12]; r.s[13] >>= b.s[13]; r.s[14] >>= b.s[14]; r.s[15] >>= b.s[15]; r.s[16] >>= b.s[16]; r.s[17] >>= b.s[17]; r.s[18] >>= b.s[18]; r.s[19] >>= b.s[19]; r.s[20] >>= b.s[20]; r.s[21] >>= b.s[21]; r.s[22] >>= b.s[22]; r.s[23] >>= b.s[23]; r.s[24] >>= b.s[24]; r.s[25] >>= b.s[25]; r.s[26] >>= b.s[26]; r.s[27] >>= b.s[27]; r.s[28] >>= b.s[28]; r.s[29] >>= b.s[29]; r.s[30] >>= b.s[30]; r.s[31] >>= b.s[31]; r.s[32] >>= b.s[32]; r.s[33] >>= b.s[33]; r.s[34] >>= b.s[34]; r.s[35] >>= b.s[35]; r.s[36] >>= b.s[36]; r.s[37] >>= b.s[37]; r.s[38] >>= b.s[38]; r.s[39] >>= b.s[39]; r.s[40] >>= b.s[40]; r.s[41] >>= b.s[41]; r.s[42] >>= b.s[42]; r.s[43] >>= b.s[43]; r.s[44] >>= b.s[44]; r.s[45] >>= b.s[45]; r.s[46] >>= b.s[46]; r.s[47] >>= b.s[47]; r.s[48] >>= b.s[48]; r.s[49] >>= b.s[49]; r.s[50] >>= b.s[50]; r.s[51] >>= b.s[51]; r.s[52] >>= b.s[52]; r.s[53] >>= b.s[53]; r.s[54] >>= b.s[54]; r.s[55] >>= b.s[55]; r.s[56] >>= b.s[56]; r.s[57] >>= b.s[57]; r.s[58] >>= b.s[58]; r.s[59] >>= b.s[59]; r.s[60] >>= b.s[60]; r.s[61] >>= b.s[61]; r.s[62] >>= b.s[62]; r.s[63] >>= b.s[63]; return r; }
	forceinline friend s8x64 &operator>>=(s8x64 &a, s8x64 b) { return a=a>>b; }
	forceinline friend s8x64 &operator>>=(s8x64 &a, s8 b) { return a=a>>b; }
	forceinline operator u8x64() const;
};

#if ARCH_AVX512
forceinline s8x64 S8x64(s8 a) { return {_mm512_set1_epi8(a)}; }
forceinline s8x64 S8x64(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15,s8 _16,s8 _17,s8 _18,s8 _19,s8 _20,s8 _21,s8 _22,s8 _23,s8 _24,s8 _25,s8 _26,s8 _27,s8 _28,s8 _29,s8 _30,s8 _31,s8 _32,s8 _33,s8 _34,s8 _35,s8 _36,s8 _37,s8 _38,s8 _39,s8 _40,s8 _41,s8 _42,s8 _43,s8 _44,s8 _45,s8 _46,s8 _47,s8 _48,s8 _49,s8 _50,s8 _51,s8 _52,s8 _53,s8 _54,s8 _55,s8 _56,s8 _57,s8 _58,s8 _59,s8 _60,s8 _61,s8 _62,s8 _63) { return {_mm512_setr_epi8(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63)}; }
#else
forceinline s8x64 S8x64(s8 a){s8x64 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;r[16]=a;r[17]=a;r[18]=a;r[19]=a;r[20]=a;r[21]=a;r[22]=a;r[23]=a;r[24]=a;r[25]=a;r[26]=a;r[27]=a;r[28]=a;r[29]=a;r[30]=a;r[31]=a;r[32]=a;r[33]=a;r[34]=a;r[35]=a;r[36]=a;r[37]=a;r[38]=a;r[39]=a;r[40]=a;r[41]=a;r[42]=a;r[43]=a;r[44]=a;r[45]=a;r[46]=a;r[47]=a;r[48]=a;r[49]=a;r[50]=a;r[51]=a;r[52]=a;r[53]=a;r[54]=a;r[55]=a;r[56]=a;r[57]=a;r[58]=a;r[59]=a;r[60]=a;r[61]=a;r[62]=a;r[63]=a;return r;}
forceinline s8x64 S8x64(s8 _0,s8 _1,s8 _2,s8 _3,s8 _4,s8 _5,s8 _6,s8 _7,s8 _8,s8 _9,s8 _10,s8 _11,s8 _12,s8 _13,s8 _14,s8 _15,s8 _16,s8 _17,s8 _18,s8 _19,s8 _20,s8 _21,s8 _22,s8 _23,s8 _24,s8 _25,s8 _26,s8 _27,s8 _28,s8 _29,s8 _30,s8 _31,s8 _32,s8 _33,s8 _34,s8 _35,s8 _36,s8 _37,s8 _38,s8 _39,s8 _40,s8 _41,s8 _42,s8 _43,s8 _44,s8 _45,s8 _46,s8 _47,s8 _48,s8 _49,s8 _50,s8 _51,s8 _52,s8 _53,s8 _54,s8 _55,s8 _56,s8 _57,s8 _58,s8 _59,s8 _60,s8 _61,s8 _62,s8 _63){s8x64 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;r[16]=_16;r[17]=_17;r[18]=_18;r[19]=_19;r[20]=_20;r[21]=_21;r[22]=_22;r[23]=_23;r[24]=_24;r[25]=_25;r[26]=_26;r[27]=_27;r[28]=_28;r[29]=_29;r[30]=_30;r[31]=_31;r[32]=_32;r[33]=_33;r[34]=_34;r[35]=_35;r[36]=_36;r[37]=_37;r[38]=_38;r[39]=_39;r[40]=_40;r[41]=_41;r[42]=_42;r[43]=_43;r[44]=_44;r[45]=_45;r[46]=_46;r[47]=_47;r[48]=_48;r[49]=_49;r[50]=_50;r[51]=_51;r[52]=_52;r[53]=_53;r[54]=_54;r[55]=_55;r[56]=_56;r[57]=_57;r[58]=_58;r[59]=_59;r[60]=_60;r[61]=_61;r[62]=_62;r[63]=_63;return r;}
#endif
forceinline s16x32 S16x32(s16);
forceinline s16x32 S16x32(s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16,s16);

union s16x32 {
#if ARCH_AVX512
	__m512i m;
#endif
	s16x16 x16[2];
	s16x8 x8[4];
	s16 s[32];
	forceinline s16 operator[](umm i) const { return s[i]; }
	forceinline s16 &operator[](umm i) { return s[i]; }
	forceinline friend s16x32 operator+(s16x32 a) { return a; }
	forceinline friend s16x32 operator-(s16x32 a) { return s16x32{} - a; }
#if ARCH_AVX512
	forceinline friend s16x32 operator~(s16x32 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend s16x32 operator~(s16x32 a) {a.x16[0] = ~a.x16[0]; a.x16[1] = ~a.x16[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s16x32 operator+(s16x32 a, s16x32 b) { return {_mm512_add_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator+(s16x32 a, s16x32 b) {a.x16[0] += b.x16[0]; a.x16[1] += b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator+(s16x32 a, s16 b) { return a+S16x32(b); }
	forceinline friend s16x32 operator+(s16 a, s16x32 b) { return S16x32(a)+b; }
	forceinline friend s16x32 &operator+=(s16x32 &a, s16x32 b) { return a=a+b; }
	forceinline friend s16x32 &operator+=(s16x32 &a, s16 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator-(s16x32 a, s16x32 b) { return {_mm512_sub_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator-(s16x32 a, s16x32 b) {a.x16[0] -= b.x16[0]; a.x16[1] -= b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator-(s16x32 a, s16 b) { return a-S16x32(b); }
	forceinline friend s16x32 operator-(s16 a, s16x32 b) { return S16x32(a)-b; }
	forceinline friend s16x32 &operator-=(s16x32 &a, s16x32 b) { return a=a-b; }
	forceinline friend s16x32 &operator-=(s16x32 &a, s16 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator*(s16x32 a, s16x32 b) { return {_mm512_mullo_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator*(s16x32 a, s16x32 b) {a.x16[0] *= b.x16[0]; a.x16[1] *= b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator*(s16x32 a, s16 b) { return a*S16x32(b); }
	forceinline friend s16x32 operator*(s16 a, s16x32 b) { return S16x32(a)*b; }
	forceinline friend s16x32 &operator*=(s16x32 &a, s16x32 b) { return a=a*b; }
	forceinline friend s16x32 &operator*=(s16x32 &a, s16 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator/(s16x32 a, s16x32 b) { return {_mm512_div_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator/(s16x32 a, s16x32 b) {a.x16[0] /= b.x16[0]; a.x16[1] /= b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator/(s16x32 a, s16 b) { return a/S16x32(b); }
	forceinline friend s16x32 operator/(s16 a, s16x32 b) { return S16x32(a)/b; }
	forceinline friend s16x32 &operator/=(s16x32 &a, s16x32 b) { return a=a/b; }
	forceinline friend s16x32 &operator/=(s16x32 &a, s16 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator^(s16x32 a, s16x32 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator^(s16x32 a, s16x32 b) {a.x16[0] ^= b.x16[0]; a.x16[1] ^= b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator^(s16x32 a, s16 b) { return a^S16x32(b); }
	forceinline friend s16x32 operator^(s16 a, s16x32 b) { return S16x32(a)^b; }
	forceinline friend s16x32 &operator^=(s16x32 &a, s16x32 b) { return a=a^b; }
	forceinline friend s16x32 &operator^=(s16x32 &a, s16 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator&(s16x32 a, s16x32 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator&(s16x32 a, s16x32 b) {a.x16[0] &= b.x16[0]; a.x16[1] &= b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator&(s16x32 a, s16 b) { return a&S16x32(b); }
	forceinline friend s16x32 operator&(s16 a, s16x32 b) { return S16x32(a)&b; }
	forceinline friend s16x32 &operator&=(s16x32 &a, s16x32 b) { return a=a&b; }
	forceinline friend s16x32 &operator&=(s16x32 &a, s16 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator|(s16x32 a, s16x32 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator|(s16x32 a, s16x32 b) {a.x16[0] |= b.x16[0]; a.x16[1] |= b.x16[1]; return a; }
#endif
	forceinline friend s16x32 operator|(s16x32 a, s16 b) { return a|S16x32(b); }
	forceinline friend s16x32 operator|(s16 a, s16x32 b) { return S16x32(a)|b; }
	forceinline friend s16x32 &operator|=(s16x32 &a, s16x32 b) { return a=a|b; }
	forceinline friend s16x32 &operator|=(s16x32 &a, s16 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator<<(s16x32 a, s16x32 b) { return {_mm512_sllv_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator<<(s16x32 a, s16x32 b) {a.x16[0] <<= b.x16[0]; a.x16[1] <<= b.x16[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s16x32 operator<<(s16x32 a, s16 b) { return {_mm512_slli_epi16(a.m, b)}; }
#else
	forceinline friend s16x32 operator<<(s16x32 a, s16 b) {a.x16[0] <<= b; a.x16[1] <<= b; return a; }
#endif
	forceinline friend s16x32 operator<<(s16 a, s16x32 b) { return S16x32(a)<<b; }
	forceinline friend s16x32 &operator<<=(s16x32 &a, s16x32 b) { return a=a<<b; }
	forceinline friend s16x32 &operator<<=(s16x32 &a, s16 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s16x32 operator>>(s16x32 a, s16x32 b) { return {_mm512_srav_epi16(a.m, b.m)}; }
#else
	forceinline friend s16x32 operator>>(s16x32 a, s16x32 b) {a.x16[0] >>= b.x16[0]; a.x16[1] >>= b.x16[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s16x32 operator>>(s16x32 a, s16 b) { return {_mm512_srai_epi16(a.m, b)}; }
#else
	forceinline friend s16x32 operator>>(s16x32 a, s16 b) {a.x16[0] >>= b; a.x16[1] >>= b; return a; }
#endif
	forceinline friend s16x32 operator>>(s16 a, s16x32 b) { return S16x32(a)>>b; }
	forceinline friend s16x32 &operator>>=(s16x32 &a, s16x32 b) { return a=a>>b; }
	forceinline friend s16x32 &operator>>=(s16x32 &a, s16 b) { return a=a>>b; }
	forceinline operator u8x32() const;
	forceinline operator u16x32() const;
	forceinline operator s8x32() const;
};

#if ARCH_AVX512
forceinline s16x32 S16x32(s16 a) { return {_mm512_set1_epi16(a)}; }
forceinline s16x32 S16x32(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7,s16 _8,s16 _9,s16 _10,s16 _11,s16 _12,s16 _13,s16 _14,s16 _15,s16 _16,s16 _17,s16 _18,s16 _19,s16 _20,s16 _21,s16 _22,s16 _23,s16 _24,s16 _25,s16 _26,s16 _27,s16 _28,s16 _29,s16 _30,s16 _31) { return {_mm512_setr_epi16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31)}; }
#else
forceinline s16x32 S16x32(s16 a){s16x32 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;r[16]=a;r[17]=a;r[18]=a;r[19]=a;r[20]=a;r[21]=a;r[22]=a;r[23]=a;r[24]=a;r[25]=a;r[26]=a;r[27]=a;r[28]=a;r[29]=a;r[30]=a;r[31]=a;return r;}
forceinline s16x32 S16x32(s16 _0,s16 _1,s16 _2,s16 _3,s16 _4,s16 _5,s16 _6,s16 _7,s16 _8,s16 _9,s16 _10,s16 _11,s16 _12,s16 _13,s16 _14,s16 _15,s16 _16,s16 _17,s16 _18,s16 _19,s16 _20,s16 _21,s16 _22,s16 _23,s16 _24,s16 _25,s16 _26,s16 _27,s16 _28,s16 _29,s16 _30,s16 _31){s16x32 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;r[16]=_16;r[17]=_17;r[18]=_18;r[19]=_19;r[20]=_20;r[21]=_21;r[22]=_22;r[23]=_23;r[24]=_24;r[25]=_25;r[26]=_26;r[27]=_27;r[28]=_28;r[29]=_29;r[30]=_30;r[31]=_31;return r;}
#endif
forceinline s32x16 S32x16(s32);
forceinline s32x16 S32x16(s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,s32);

union s32x16 {
#if ARCH_AVX512
	__m512i m;
#endif
	s32x8 x8[2];
	s32x4 x4[4];
	s32 s[16];
	forceinline s32 operator[](umm i) const { return s[i]; }
	forceinline s32 &operator[](umm i) { return s[i]; }
	forceinline friend s32x16 operator+(s32x16 a) { return a; }
	forceinline friend s32x16 operator-(s32x16 a) { return s32x16{} - a; }
#if ARCH_AVX512
	forceinline friend s32x16 operator~(s32x16 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend s32x16 operator~(s32x16 a) {a.x8[0] = ~a.x8[0]; a.x8[1] = ~a.x8[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s32x16 operator+(s32x16 a, s32x16 b) { return {_mm512_add_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator+(s32x16 a, s32x16 b) {a.x8[0] += b.x8[0]; a.x8[1] += b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator+(s32x16 a, s32 b) { return a+S32x16(b); }
	forceinline friend s32x16 operator+(s32 a, s32x16 b) { return S32x16(a)+b; }
	forceinline friend s32x16 &operator+=(s32x16 &a, s32x16 b) { return a=a+b; }
	forceinline friend s32x16 &operator+=(s32x16 &a, s32 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator-(s32x16 a, s32x16 b) { return {_mm512_sub_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator-(s32x16 a, s32x16 b) {a.x8[0] -= b.x8[0]; a.x8[1] -= b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator-(s32x16 a, s32 b) { return a-S32x16(b); }
	forceinline friend s32x16 operator-(s32 a, s32x16 b) { return S32x16(a)-b; }
	forceinline friend s32x16 &operator-=(s32x16 &a, s32x16 b) { return a=a-b; }
	forceinline friend s32x16 &operator-=(s32x16 &a, s32 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator*(s32x16 a, s32x16 b) { return {_mm512_mullo_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator*(s32x16 a, s32x16 b) {a.x8[0] *= b.x8[0]; a.x8[1] *= b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator*(s32x16 a, s32 b) { return a*S32x16(b); }
	forceinline friend s32x16 operator*(s32 a, s32x16 b) { return S32x16(a)*b; }
	forceinline friend s32x16 &operator*=(s32x16 &a, s32x16 b) { return a=a*b; }
	forceinline friend s32x16 &operator*=(s32x16 &a, s32 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator/(s32x16 a, s32x16 b) { return {_mm512_div_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator/(s32x16 a, s32x16 b) {a.x8[0] /= b.x8[0]; a.x8[1] /= b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator/(s32x16 a, s32 b) { return a/S32x16(b); }
	forceinline friend s32x16 operator/(s32 a, s32x16 b) { return S32x16(a)/b; }
	forceinline friend s32x16 &operator/=(s32x16 &a, s32x16 b) { return a=a/b; }
	forceinline friend s32x16 &operator/=(s32x16 &a, s32 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator^(s32x16 a, s32x16 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator^(s32x16 a, s32x16 b) {a.x8[0] ^= b.x8[0]; a.x8[1] ^= b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator^(s32x16 a, s32 b) { return a^S32x16(b); }
	forceinline friend s32x16 operator^(s32 a, s32x16 b) { return S32x16(a)^b; }
	forceinline friend s32x16 &operator^=(s32x16 &a, s32x16 b) { return a=a^b; }
	forceinline friend s32x16 &operator^=(s32x16 &a, s32 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator&(s32x16 a, s32x16 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator&(s32x16 a, s32x16 b) {a.x8[0] &= b.x8[0]; a.x8[1] &= b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator&(s32x16 a, s32 b) { return a&S32x16(b); }
	forceinline friend s32x16 operator&(s32 a, s32x16 b) { return S32x16(a)&b; }
	forceinline friend s32x16 &operator&=(s32x16 &a, s32x16 b) { return a=a&b; }
	forceinline friend s32x16 &operator&=(s32x16 &a, s32 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator|(s32x16 a, s32x16 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator|(s32x16 a, s32x16 b) {a.x8[0] |= b.x8[0]; a.x8[1] |= b.x8[1]; return a; }
#endif
	forceinline friend s32x16 operator|(s32x16 a, s32 b) { return a|S32x16(b); }
	forceinline friend s32x16 operator|(s32 a, s32x16 b) { return S32x16(a)|b; }
	forceinline friend s32x16 &operator|=(s32x16 &a, s32x16 b) { return a=a|b; }
	forceinline friend s32x16 &operator|=(s32x16 &a, s32 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator<<(s32x16 a, s32x16 b) { return {_mm512_sllv_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator<<(s32x16 a, s32x16 b) {a.x8[0] <<= b.x8[0]; a.x8[1] <<= b.x8[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s32x16 operator<<(s32x16 a, s32 b) { return {_mm512_slli_epi32(a.m, b)}; }
#else
	forceinline friend s32x16 operator<<(s32x16 a, s32 b) {a.x8[0] <<= b; a.x8[1] <<= b; return a; }
#endif
	forceinline friend s32x16 operator<<(s32 a, s32x16 b) { return S32x16(a)<<b; }
	forceinline friend s32x16 &operator<<=(s32x16 &a, s32x16 b) { return a=a<<b; }
	forceinline friend s32x16 &operator<<=(s32x16 &a, s32 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s32x16 operator>>(s32x16 a, s32x16 b) { return {_mm512_srav_epi32(a.m, b.m)}; }
#else
	forceinline friend s32x16 operator>>(s32x16 a, s32x16 b) {a.x8[0] >>= b.x8[0]; a.x8[1] >>= b.x8[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s32x16 operator>>(s32x16 a, s32 b) { return {_mm512_srai_epi32(a.m, b)}; }
#else
	forceinline friend s32x16 operator>>(s32x16 a, s32 b) {a.x8[0] >>= b; a.x8[1] >>= b; return a; }
#endif
	forceinline friend s32x16 operator>>(s32 a, s32x16 b) { return S32x16(a)>>b; }
	forceinline friend s32x16 &operator>>=(s32x16 &a, s32x16 b) { return a=a>>b; }
	forceinline friend s32x16 &operator>>=(s32x16 &a, s32 b) { return a=a>>b; }
	forceinline operator u8x16() const;
	forceinline operator u16x16() const;
	forceinline operator u32x16() const;
	forceinline operator s8x16() const;
	forceinline operator s16x16() const;
	forceinline operator f32x16() const;
};

#if ARCH_AVX512
forceinline s32x16 S32x16(s32 a) { return {_mm512_set1_epi32(a)}; }
forceinline s32x16 S32x16(s32 _0,s32 _1,s32 _2,s32 _3,s32 _4,s32 _5,s32 _6,s32 _7,s32 _8,s32 _9,s32 _10,s32 _11,s32 _12,s32 _13,s32 _14,s32 _15) { return {_mm512_setr_epi32(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
#else
forceinline s32x16 S32x16(s32 a){s32x16 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;return r;}
forceinline s32x16 S32x16(s32 _0,s32 _1,s32 _2,s32 _3,s32 _4,s32 _5,s32 _6,s32 _7,s32 _8,s32 _9,s32 _10,s32 _11,s32 _12,s32 _13,s32 _14,s32 _15){s32x16 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;return r;}
#endif
forceinline s64x8 S64x8(s64);
forceinline s64x8 S64x8(s64,s64,s64,s64,s64,s64,s64,s64);

union s64x8 {
#if ARCH_AVX512
	__m512i m;
#endif
	s64x4 x4[2];
	s64x2 x2[4];
	s64 s[8];
	forceinline s64 operator[](umm i) const { return s[i]; }
	forceinline s64 &operator[](umm i) { return s[i]; }
	forceinline friend s64x8 operator+(s64x8 a) { return a; }
	forceinline friend s64x8 operator-(s64x8 a) { return s64x8{} - a; }
#if ARCH_AVX512
	forceinline friend s64x8 operator~(s64x8 a) { return {_mm512_xor_si512(a.m, _mm512_set1_epi32(~0))}; }
#else
	forceinline friend s64x8 operator~(s64x8 a) {a.x4[0] = ~a.x4[0]; a.x4[1] = ~a.x4[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s64x8 operator+(s64x8 a, s64x8 b) { return {_mm512_add_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator+(s64x8 a, s64x8 b) {a.x4[0] += b.x4[0]; a.x4[1] += b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator+(s64x8 a, s64 b) { return a+S64x8(b); }
	forceinline friend s64x8 operator+(s64 a, s64x8 b) { return S64x8(a)+b; }
	forceinline friend s64x8 &operator+=(s64x8 &a, s64x8 b) { return a=a+b; }
	forceinline friend s64x8 &operator+=(s64x8 &a, s64 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator-(s64x8 a, s64x8 b) { return {_mm512_sub_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator-(s64x8 a, s64x8 b) {a.x4[0] -= b.x4[0]; a.x4[1] -= b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator-(s64x8 a, s64 b) { return a-S64x8(b); }
	forceinline friend s64x8 operator-(s64 a, s64x8 b) { return S64x8(a)-b; }
	forceinline friend s64x8 &operator-=(s64x8 &a, s64x8 b) { return a=a-b; }
	forceinline friend s64x8 &operator-=(s64x8 &a, s64 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator*(s64x8 a, s64x8 b) { return {_mm512_mullo_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator*(s64x8 a, s64x8 b) {a.x4[0] *= b.x4[0]; a.x4[1] *= b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator*(s64x8 a, s64 b) { return a*S64x8(b); }
	forceinline friend s64x8 operator*(s64 a, s64x8 b) { return S64x8(a)*b; }
	forceinline friend s64x8 &operator*=(s64x8 &a, s64x8 b) { return a=a*b; }
	forceinline friend s64x8 &operator*=(s64x8 &a, s64 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator/(s64x8 a, s64x8 b) { return {_mm512_div_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator/(s64x8 a, s64x8 b) {a.x4[0] /= b.x4[0]; a.x4[1] /= b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator/(s64x8 a, s64 b) { return a/S64x8(b); }
	forceinline friend s64x8 operator/(s64 a, s64x8 b) { return S64x8(a)/b; }
	forceinline friend s64x8 &operator/=(s64x8 &a, s64x8 b) { return a=a/b; }
	forceinline friend s64x8 &operator/=(s64x8 &a, s64 b) { return a=a/b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator^(s64x8 a, s64x8 b) { return {_mm512_xor_si512(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator^(s64x8 a, s64x8 b) {a.x4[0] ^= b.x4[0]; a.x4[1] ^= b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator^(s64x8 a, s64 b) { return a^S64x8(b); }
	forceinline friend s64x8 operator^(s64 a, s64x8 b) { return S64x8(a)^b; }
	forceinline friend s64x8 &operator^=(s64x8 &a, s64x8 b) { return a=a^b; }
	forceinline friend s64x8 &operator^=(s64x8 &a, s64 b) { return a=a^b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator&(s64x8 a, s64x8 b) { return {_mm512_and_si512(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator&(s64x8 a, s64x8 b) {a.x4[0] &= b.x4[0]; a.x4[1] &= b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator&(s64x8 a, s64 b) { return a&S64x8(b); }
	forceinline friend s64x8 operator&(s64 a, s64x8 b) { return S64x8(a)&b; }
	forceinline friend s64x8 &operator&=(s64x8 &a, s64x8 b) { return a=a&b; }
	forceinline friend s64x8 &operator&=(s64x8 &a, s64 b) { return a=a&b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator|(s64x8 a, s64x8 b) { return {_mm512_or_si512(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator|(s64x8 a, s64x8 b) {a.x4[0] |= b.x4[0]; a.x4[1] |= b.x4[1]; return a; }
#endif
	forceinline friend s64x8 operator|(s64x8 a, s64 b) { return a|S64x8(b); }
	forceinline friend s64x8 operator|(s64 a, s64x8 b) { return S64x8(a)|b; }
	forceinline friend s64x8 &operator|=(s64x8 &a, s64x8 b) { return a=a|b; }
	forceinline friend s64x8 &operator|=(s64x8 &a, s64 b) { return a=a|b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator<<(s64x8 a, s64x8 b) { return {_mm512_sllv_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator<<(s64x8 a, s64x8 b) {a.x4[0] <<= b.x4[0]; a.x4[1] <<= b.x4[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s64x8 operator<<(s64x8 a, s64 b) { return {_mm512_slli_epi64(a.m, b)}; }
#else
	forceinline friend s64x8 operator<<(s64x8 a, s64 b) {a.x4[0] <<= b; a.x4[1] <<= b; return a; }
#endif
	forceinline friend s64x8 operator<<(s64 a, s64x8 b) { return S64x8(a)<<b; }
	forceinline friend s64x8 &operator<<=(s64x8 &a, s64x8 b) { return a=a<<b; }
	forceinline friend s64x8 &operator<<=(s64x8 &a, s64 b) { return a=a<<b; }
#if ARCH_AVX512
	forceinline friend s64x8 operator>>(s64x8 a, s64x8 b) { return {_mm512_srav_epi64(a.m, b.m)}; }
#else
	forceinline friend s64x8 operator>>(s64x8 a, s64x8 b) {a.x4[0] >>= b.x4[0]; a.x4[1] >>= b.x4[1]; return a; }
#endif
#if ARCH_AVX512
	forceinline friend s64x8 operator>>(s64x8 a, s64 b) { return {_mm512_srai_epi64(a.m, b)}; }
#else
	forceinline friend s64x8 operator>>(s64x8 a, s64 b) {a.x4[0] >>= b; a.x4[1] >>= b; return a; }
#endif
	forceinline friend s64x8 operator>>(s64 a, s64x8 b) { return S64x8(a)>>b; }
	forceinline friend s64x8 &operator>>=(s64x8 &a, s64x8 b) { return a=a>>b; }
	forceinline friend s64x8 &operator>>=(s64x8 &a, s64 b) { return a=a>>b; }
	forceinline operator u16x8() const;
	forceinline operator u32x8() const;
	forceinline operator u64x8() const;
	forceinline operator s16x8() const;
	forceinline operator s32x8() const;
	forceinline operator f32x8() const;
	forceinline operator f64x8() const;
};

#if ARCH_AVX512
forceinline s64x8 S64x8(s64 a) { return {_mm512_set1_epi64(a)}; }
forceinline s64x8 S64x8(s64 _0,s64 _1,s64 _2,s64 _3,s64 _4,s64 _5,s64 _6,s64 _7) { return {_mm512_setr_epi64(_0,_1,_2,_3,_4,_5,_6,_7)}; }
#else
forceinline s64x8 S64x8(s64 a){s64x8 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;return r;}
forceinline s64x8 S64x8(s64 _0,s64 _1,s64 _2,s64 _3,s64 _4,s64 _5,s64 _6,s64 _7){s64x8 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;return r;}
#endif
forceinline f32x16 F32x16(f32);
forceinline f32x16 F32x16(f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32);

union f32x16 {
#if ARCH_AVX512
	__m512 m;
#endif
	f32x8 x8[2];
	f32x4 x4[4];
	f32 s[16];
	forceinline f32 operator[](umm i) const { return s[i]; }
	forceinline f32 &operator[](umm i) { return s[i]; }
	forceinline friend f32x16 operator+(f32x16 a) { return a; }
	forceinline friend f32x16 operator-(f32x16 a) { return f32x16{} - a; }
#if ARCH_AVX512
	forceinline friend f32x16 operator+(f32x16 a, f32x16 b) { return {_mm512_add_ps(a.m, b.m)}; }
#else
	forceinline friend f32x16 operator+(f32x16 a, f32x16 b) {a.x8[0] += b.x8[0]; a.x8[1] += b.x8[1]; return a; }
#endif
	forceinline friend f32x16 operator+(f32x16 a, f32 b) { return a+F32x16(b); }
	forceinline friend f32x16 operator+(f32 a, f32x16 b) { return F32x16(a)+b; }
	forceinline friend f32x16 &operator+=(f32x16 &a, f32x16 b) { return a=a+b; }
	forceinline friend f32x16 &operator+=(f32x16 &a, f32 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend f32x16 operator-(f32x16 a, f32x16 b) { return {_mm512_sub_ps(a.m, b.m)}; }
#else
	forceinline friend f32x16 operator-(f32x16 a, f32x16 b) {a.x8[0] -= b.x8[0]; a.x8[1] -= b.x8[1]; return a; }
#endif
	forceinline friend f32x16 operator-(f32x16 a, f32 b) { return a-F32x16(b); }
	forceinline friend f32x16 operator-(f32 a, f32x16 b) { return F32x16(a)-b; }
	forceinline friend f32x16 &operator-=(f32x16 &a, f32x16 b) { return a=a-b; }
	forceinline friend f32x16 &operator-=(f32x16 &a, f32 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend f32x16 operator*(f32x16 a, f32x16 b) { return {_mm512_mul_ps(a.m, b.m)}; }
#else
	forceinline friend f32x16 operator*(f32x16 a, f32x16 b) {a.x8[0] *= b.x8[0]; a.x8[1] *= b.x8[1]; return a; }
#endif
	forceinline friend f32x16 operator*(f32x16 a, f32 b) { return a*F32x16(b); }
	forceinline friend f32x16 operator*(f32 a, f32x16 b) { return F32x16(a)*b; }
	forceinline friend f32x16 &operator*=(f32x16 &a, f32x16 b) { return a=a*b; }
	forceinline friend f32x16 &operator*=(f32x16 &a, f32 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend f32x16 operator/(f32x16 a, f32x16 b) { return {_mm512_div_ps(a.m, b.m)}; }
#else
	forceinline friend f32x16 operator/(f32x16 a, f32x16 b) {a.x8[0] /= b.x8[0]; a.x8[1] /= b.x8[1]; return a; }
#endif
	forceinline friend f32x16 operator/(f32x16 a, f32 b) { return a/F32x16(b); }
	forceinline friend f32x16 operator/(f32 a, f32x16 b) { return F32x16(a)/b; }
	forceinline friend f32x16 &operator/=(f32x16 &a, f32x16 b) { return a=a/b; }
	forceinline friend f32x16 &operator/=(f32x16 &a, f32 b) { return a=a/b; }
	forceinline operator u8x16() const;
	forceinline operator u16x16() const;
	forceinline operator u32x16() const;
	forceinline operator s8x16() const;
	forceinline operator s16x16() const;
	forceinline operator s32x16() const;
};

#if ARCH_AVX512
forceinline f32x16 F32x16(f32 a) { return {_mm512_set1_ps(a)}; }
forceinline f32x16 F32x16(f32 _0,f32 _1,f32 _2,f32 _3,f32 _4,f32 _5,f32 _6,f32 _7,f32 _8,f32 _9,f32 _10,f32 _11,f32 _12,f32 _13,f32 _14,f32 _15) { return {_mm512_setr_ps(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)}; }
#else
forceinline f32x16 F32x16(f32 a){f32x16 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;r[8]=a;r[9]=a;r[10]=a;r[11]=a;r[12]=a;r[13]=a;r[14]=a;r[15]=a;return r;}
forceinline f32x16 F32x16(f32 _0,f32 _1,f32 _2,f32 _3,f32 _4,f32 _5,f32 _6,f32 _7,f32 _8,f32 _9,f32 _10,f32 _11,f32 _12,f32 _13,f32 _14,f32 _15){f32x16 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;r[8]=_8;r[9]=_9;r[10]=_10;r[11]=_11;r[12]=_12;r[13]=_13;r[14]=_14;r[15]=_15;return r;}
#endif
#if ARCH_AVX512
forceinline f32x16 fmadd(f32x16 a, f32x16 b, f32x16 c) { return {_mm512_fmadd_ps(a.m, b.m, c.m)}; }
#else
forceinline f32x16 fmadd(f32x16 a, f32x16 b, f32x16 c) {a.s[0]=a.s[0]*b.s[0]+c.s[0];a.s[1]=a.s[1]*b.s[1]+c.s[1];a.s[2]=a.s[2]*b.s[2]+c.s[2];a.s[3]=a.s[3]*b.s[3]+c.s[3];a.s[4]=a.s[4]*b.s[4]+c.s[4];a.s[5]=a.s[5]*b.s[5]+c.s[5];a.s[6]=a.s[6]*b.s[6]+c.s[6];a.s[7]=a.s[7]*b.s[7]+c.s[7];a.s[8]=a.s[8]*b.s[8]+c.s[8];a.s[9]=a.s[9]*b.s[9]+c.s[9];a.s[10]=a.s[10]*b.s[10]+c.s[10];a.s[11]=a.s[11]*b.s[11]+c.s[11];a.s[12]=a.s[12]*b.s[12]+c.s[12];a.s[13]=a.s[13]*b.s[13]+c.s[13];a.s[14]=a.s[14]*b.s[14]+c.s[14];a.s[15]=a.s[15]*b.s[15]+c.s[15];return a;}
#endif

forceinline f64x8 F64x8(f64);
forceinline f64x8 F64x8(f64,f64,f64,f64,f64,f64,f64,f64);

union f64x8 {
#if ARCH_AVX512
	__m512d m;
#endif
	f64x4 x4[2];
	f64x2 x2[4];
	f64 s[8];
	forceinline f64 operator[](umm i) const { return s[i]; }
	forceinline f64 &operator[](umm i) { return s[i]; }
	forceinline friend f64x8 operator+(f64x8 a) { return a; }
	forceinline friend f64x8 operator-(f64x8 a) { return f64x8{} - a; }
#if ARCH_AVX512
	forceinline friend f64x8 operator+(f64x8 a, f64x8 b) { return {_mm512_add_pd(a.m, b.m)}; }
#else
	forceinline friend f64x8 operator+(f64x8 a, f64x8 b) {a.x4[0] += b.x4[0]; a.x4[1] += b.x4[1]; return a; }
#endif
	forceinline friend f64x8 operator+(f64x8 a, f64 b) { return a+F64x8(b); }
	forceinline friend f64x8 operator+(f64 a, f64x8 b) { return F64x8(a)+b; }
	forceinline friend f64x8 &operator+=(f64x8 &a, f64x8 b) { return a=a+b; }
	forceinline friend f64x8 &operator+=(f64x8 &a, f64 b) { return a=a+b; }
#if ARCH_AVX512
	forceinline friend f64x8 operator-(f64x8 a, f64x8 b) { return {_mm512_sub_pd(a.m, b.m)}; }
#else
	forceinline friend f64x8 operator-(f64x8 a, f64x8 b) {a.x4[0] -= b.x4[0]; a.x4[1] -= b.x4[1]; return a; }
#endif
	forceinline friend f64x8 operator-(f64x8 a, f64 b) { return a-F64x8(b); }
	forceinline friend f64x8 operator-(f64 a, f64x8 b) { return F64x8(a)-b; }
	forceinline friend f64x8 &operator-=(f64x8 &a, f64x8 b) { return a=a-b; }
	forceinline friend f64x8 &operator-=(f64x8 &a, f64 b) { return a=a-b; }
#if ARCH_AVX512
	forceinline friend f64x8 operator*(f64x8 a, f64x8 b) { return {_mm512_mul_pd(a.m, b.m)}; }
#else
	forceinline friend f64x8 operator*(f64x8 a, f64x8 b) {a.x4[0] *= b.x4[0]; a.x4[1] *= b.x4[1]; return a; }
#endif
	forceinline friend f64x8 operator*(f64x8 a, f64 b) { return a*F64x8(b); }
	forceinline friend f64x8 operator*(f64 a, f64x8 b) { return F64x8(a)*b; }
	forceinline friend f64x8 &operator*=(f64x8 &a, f64x8 b) { return a=a*b; }
	forceinline friend f64x8 &operator*=(f64x8 &a, f64 b) { return a=a*b; }
#if ARCH_AVX512
	forceinline friend f64x8 operator/(f64x8 a, f64x8 b) { return {_mm512_div_pd(a.m, b.m)}; }
#else
	forceinline friend f64x8 operator/(f64x8 a, f64x8 b) {a.x4[0] /= b.x4[0]; a.x4[1] /= b.x4[1]; return a; }
#endif
	forceinline friend f64x8 operator/(f64x8 a, f64 b) { return a/F64x8(b); }
	forceinline friend f64x8 operator/(f64 a, f64x8 b) { return F64x8(a)/b; }
	forceinline friend f64x8 &operator/=(f64x8 &a, f64x8 b) { return a=a/b; }
	forceinline friend f64x8 &operator/=(f64x8 &a, f64 b) { return a=a/b; }
	forceinline operator u16x8() const;
	forceinline operator u32x8() const;
	forceinline operator u64x8() const;
	forceinline operator s16x8() const;
	forceinline operator s32x8() const;
	forceinline operator s64x8() const;
	forceinline operator f32x8() const;
};

#if ARCH_AVX512
forceinline f64x8 F64x8(f64 a) { return {_mm512_set1_pd(a)}; }
forceinline f64x8 F64x8(f64 _0,f64 _1,f64 _2,f64 _3,f64 _4,f64 _5,f64 _6,f64 _7) { return {_mm512_setr_pd(_0,_1,_2,_3,_4,_5,_6,_7)}; }
#else
forceinline f64x8 F64x8(f64 a){f64x8 r;r[0]=a;r[1]=a;r[2]=a;r[3]=a;r[4]=a;r[5]=a;r[6]=a;r[7]=a;return r;}
forceinline f64x8 F64x8(f64 _0,f64 _1,f64 _2,f64 _3,f64 _4,f64 _5,f64 _6,f64 _7){f64x8 r;r[0]=_0;r[1]=_1;r[2]=_2;r[3]=_3;r[4]=_4;r[5]=_5;r[6]=_6;r[7]=_7;return r;}
#endif
#if ARCH_AVX512
forceinline f64x8 fmadd(f64x8 a, f64x8 b, f64x8 c) { return {_mm512_fmadd_pd(a.m, b.m, c.m)}; }
#else
forceinline f64x8 fmadd(f64x8 a, f64x8 b, f64x8 c) {a.s[0]=a.s[0]*b.s[0]+c.s[0];a.s[1]=a.s[1]*b.s[1]+c.s[1];a.s[2]=a.s[2]*b.s[2]+c.s[2];a.s[3]=a.s[3]*b.s[3]+c.s[3];a.s[4]=a.s[4]*b.s[4]+c.s[4];a.s[5]=a.s[5]*b.s[5]+c.s[5];a.s[6]=a.s[6]*b.s[6]+c.s[6];a.s[7]=a.s[7]*b.s[7]+c.s[7];return a;}
#endif

forceinline u8x64::operator s8x64() const { return *(s8x64*)this; }
forceinline u16x32::operator u8x32() const { return *(u8x32*)this; }
forceinline u16x32::operator s8x32() const { return *(s8x32*)this; }
forceinline u16x32::operator s16x32() const { return *(s16x32*)this; }
forceinline u32x16::operator u8x16() const { return *(u8x16*)this; }
forceinline u32x16::operator u16x16() const { return *(u16x16*)this; }
forceinline u32x16::operator s8x16() const { return *(s8x16*)this; }
forceinline u32x16::operator s16x16() const { return *(s16x16*)this; }
forceinline u32x16::operator s32x16() const { return *(s32x16*)this; }
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
forceinline u64x8::operator u16x8() const { return *(u16x8*)this; }
forceinline u64x8::operator u32x8() const { return *(u32x8*)this; }
forceinline u64x8::operator s16x8() const { return *(s16x8*)this; }
forceinline u64x8::operator s32x8() const { return *(s32x8*)this; }
forceinline u64x8::operator s64x8() const { return *(s64x8*)this; }
forceinline u64x8::operator f32x8() const {
#if ARCH_AVX512
	return {_mm512_cvtepu64_ps(m)}; }
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
forceinline s8x64::operator u8x64() const { return *(u8x64*)this; }
forceinline s16x32::operator u8x32() const { return *(u8x32*)this; }
forceinline s16x32::operator u16x32() const { return *(u16x32*)this; }
forceinline s16x32::operator s8x32() const { return *(s8x32*)this; }
forceinline s32x16::operator u8x16() const { return *(u8x16*)this; }
forceinline s32x16::operator u16x16() const { return *(u16x16*)this; }
forceinline s32x16::operator u32x16() const { return *(u32x16*)this; }
forceinline s32x16::operator s8x16() const { return *(s8x16*)this; }
forceinline s32x16::operator s16x16() const { return *(s16x16*)this; }
forceinline s32x16::operator f32x16() const { return {_mm512_cvtepi32_ps(m)}; }
forceinline s64x8::operator u16x8() const { return *(u16x8*)this; }
forceinline s64x8::operator u32x8() const { return *(u32x8*)this; }
forceinline s64x8::operator u64x8() const { return *(u64x8*)this; }
forceinline s64x8::operator s16x8() const { return *(s16x8*)this; }
forceinline s64x8::operator s32x8() const { return *(s32x8*)this; }
forceinline s64x8::operator f32x8() const { return {_mm512_cvtepi64_ps(m)}; }
forceinline s64x8::operator f64x8() const { return {_mm512_cvtepi64_pd(m)}; }
forceinline f32x16::operator u8x16() const { return {_mm512_cvttps_epu8(m)}; }
forceinline f32x16::operator u16x16() const { return {_mm512_cvttps_epu16(m)}; }
forceinline f32x16::operator u32x16() const { return {_mm512_cvttps_epu32(m)}; }
forceinline f32x16::operator s8x16() const { return {_mm512_cvttps_epi8(m)}; }
forceinline f32x16::operator s16x16() const { return {_mm512_cvttps_epi16(m)}; }
forceinline f32x16::operator s32x16() const { return {_mm512_cvttps_epi32(m)}; }
forceinline f64x8::operator u16x8() const { return {_mm512_cvttpd_epu16(m)}; }
forceinline f64x8::operator u32x8() const { return {_mm512_cvttpd_epu32(m)}; }
forceinline f64x8::operator u64x8() const { return {_mm512_cvttpd_epu64(m)}; }
forceinline f64x8::operator s16x8() const { return {_mm512_cvttpd_epi16(m)}; }
forceinline f64x8::operator s32x8() const { return {_mm512_cvttpd_epi32(m)}; }
forceinline f64x8::operator s64x8() const { return {_mm512_cvttpd_epi64(m)}; }
forceinline f64x8::operator f32x8() const { return {_mm512_cvtpd_ps(m)}; }

}
}
