#pragma once
#include "simd.h"
#include "../vector.h"

namespace tl {
namespace simd_vector {

union v2u8x16;
union v2u16x8;
union v2u32x4;
union v2u64x2;
union v2s8x16;
union v2s16x8;
union v2s32x4;
union v2s64x2;
union v2f32x4;
union v2f64x2;
union v3u8x16;
union v3u16x8;
union v3u32x4;
union v3u64x2;
union v3s8x16;
union v3s16x8;
union v3s32x4;
union v3s64x2;
union v3f32x4;
union v3f64x2;
union v4u8x16;
union v4u16x8;
union v4u32x4;
union v4u64x2;
union v4s8x16;
union v4s16x8;
union v4s32x4;
union v4s64x2;
union v4f32x4;
union v4f64x2;

#if ARCH_AVX
union v2u8x32;
union v2u16x16;
union v2u32x8;
union v2u64x4;
union v2s8x32;
union v2s16x16;
union v2s32x8;
union v2s64x4;
union v2f32x8;
union v2f64x4;
union v3u8x32;
union v3u16x16;
union v3u32x8;
union v3u64x4;
union v3s8x32;
union v3s16x16;
union v3s32x8;
union v3s64x4;
union v3f32x8;
union v3f64x4;
union v4u8x32;
union v4u16x16;
union v4u32x8;
union v4u64x4;
union v4s8x32;
union v4s16x16;
union v4s32x8;
union v4s64x4;
union v4f32x8;
union v4f64x4;

#endif

#if ARCH_AVX512
union v2u8x64;
union v2u16x32;
union v2u32x16;
union v2u64x8;
union v2s8x64;
union v2s16x32;
union v2s32x16;
union v2s64x8;
union v2f32x16;
union v2f64x8;
union v3u8x64;
union v3u16x32;
union v3u32x16;
union v3u64x8;
union v3s8x64;
union v3s16x32;
union v3s32x16;
union v3s64x8;
union v3f32x16;
union v3f64x8;
union v4u8x64;
union v4u16x32;
union v4u32x16;
union v4u64x8;
union v4s8x64;
union v4s16x32;
union v4s32x16;
union v4s64x8;
union v4f32x16;
union v4f64x8;

#endif
union v2u8x16 {
	struct { simd_typed::u8x16 x, y; };
	simd_typed::u8x16 d[2];
	forceinline v2u8 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u8x16 operator+(v2u8x16 a, v2u8x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x16 operator+(v2u8x16 a, v2u8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x16 operator+(v2u8 a, v2u8x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x16 operator+(v2u8x16 a, u8 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u8x16 operator+(u8 a, v2u8x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u8x16 operator-(v2u8x16 a, v2u8x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x16 operator-(v2u8x16 a, v2u8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x16 operator-(v2u8 a, v2u8x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x16 operator-(v2u8x16 a, u8 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u8x16 operator-(u8 a, v2u8x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u8x16 operator*(v2u8x16 a, v2u8x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x16 operator*(v2u8x16 a, v2u8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x16 operator*(v2u8 a, v2u8x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x16 operator*(v2u8x16 a, u8 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u8x16 operator*(u8 a, v2u8x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u8x16 operator/(v2u8x16 a, v2u8x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x16 operator/(v2u8x16 a, v2u8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x16 operator/(v2u8 a, v2u8x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x16 operator/(v2u8x16 a, u8 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u8x16 operator/(u8 a, v2u8x16 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u8x16 operator^(v2u8x16 a, v2u8x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x16 operator^(v2u8x16 a, v2u8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x16 operator^(v2u8 a, v2u8x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x16 operator^(v2u8x16 a, u8 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u8x16 operator^(u8 a, v2u8x16 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u8x16 operator&(v2u8x16 a, v2u8x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x16 operator&(v2u8x16 a, v2u8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x16 operator&(v2u8 a, v2u8x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x16 operator&(v2u8x16 a, u8 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u8x16 operator&(u8 a, v2u8x16 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u8x16 operator|(v2u8x16 a, v2u8x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x16 operator|(v2u8x16 a, v2u8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x16 operator|(v2u8 a, v2u8x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x16 operator|(v2u8x16 a, u8 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u8x16 operator|(u8 a, v2u8x16 b) { return {a|b.x, a|b.y};}
	forceinline operator v2s8x16() const;
};

forceinline v2u8x16 V2u8x16(v2u8 a) { return {simd_typed::U8x16(a.x), simd_typed::U8x16(a.y), }; }
forceinline simd_typed::u8x16 dot(v2u8x16 a, v2u8x16 b) { return a.x*b.x+a.y*b.y; }

union v2u16x8 {
	struct { simd_typed::u16x8 x, y; };
	simd_typed::u16x8 d[2];
	forceinline v2u16 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u16x8 operator+(v2u16x8 a, v2u16x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x8 operator+(v2u16x8 a, v2u16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x8 operator+(v2u16 a, v2u16x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x8 operator+(v2u16x8 a, u16 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u16x8 operator+(u16 a, v2u16x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u16x8 operator-(v2u16x8 a, v2u16x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x8 operator-(v2u16x8 a, v2u16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x8 operator-(v2u16 a, v2u16x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x8 operator-(v2u16x8 a, u16 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u16x8 operator-(u16 a, v2u16x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u16x8 operator*(v2u16x8 a, v2u16x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x8 operator*(v2u16x8 a, v2u16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x8 operator*(v2u16 a, v2u16x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x8 operator*(v2u16x8 a, u16 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u16x8 operator*(u16 a, v2u16x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u16x8 operator/(v2u16x8 a, v2u16x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x8 operator/(v2u16x8 a, v2u16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x8 operator/(v2u16 a, v2u16x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x8 operator/(v2u16x8 a, u16 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u16x8 operator/(u16 a, v2u16x8 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u16x8 operator^(v2u16x8 a, v2u16x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x8 operator^(v2u16x8 a, v2u16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x8 operator^(v2u16 a, v2u16x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x8 operator^(v2u16x8 a, u16 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u16x8 operator^(u16 a, v2u16x8 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u16x8 operator&(v2u16x8 a, v2u16x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x8 operator&(v2u16x8 a, v2u16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x8 operator&(v2u16 a, v2u16x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x8 operator&(v2u16x8 a, u16 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u16x8 operator&(u16 a, v2u16x8 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u16x8 operator|(v2u16x8 a, v2u16x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x8 operator|(v2u16x8 a, v2u16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x8 operator|(v2u16 a, v2u16x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x8 operator|(v2u16x8 a, u16 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u16x8 operator|(u16 a, v2u16x8 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u16x8 operator<<(v2u16x8 a, u16 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u16x8 operator>>(v2u16x8 a, u16 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s16x8() const;
};

forceinline v2u16x8 V2u16x8(v2u16 a) { return {simd_typed::U16x8(a.x), simd_typed::U16x8(a.y), }; }
forceinline simd_typed::u16x8 dot(v2u16x8 a, v2u16x8 b) { return a.x*b.x+a.y*b.y; }

union v2u32x4 {
	struct { simd_typed::u32x4 x, y; };
	simd_typed::u32x4 d[2];
	forceinline v2u32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u32x4 operator+(v2u32x4 a, v2u32x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x4 operator+(v2u32x4 a, v2u32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x4 operator+(v2u32 a, v2u32x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x4 operator+(v2u32x4 a, u32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u32x4 operator+(u32 a, v2u32x4 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u32x4 operator-(v2u32x4 a, v2u32x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x4 operator-(v2u32x4 a, v2u32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x4 operator-(v2u32 a, v2u32x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x4 operator-(v2u32x4 a, u32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u32x4 operator-(u32 a, v2u32x4 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u32x4 operator*(v2u32x4 a, v2u32x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x4 operator*(v2u32x4 a, v2u32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x4 operator*(v2u32 a, v2u32x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x4 operator*(v2u32x4 a, u32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u32x4 operator*(u32 a, v2u32x4 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u32x4 operator/(v2u32x4 a, v2u32x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x4 operator/(v2u32x4 a, v2u32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x4 operator/(v2u32 a, v2u32x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x4 operator/(v2u32x4 a, u32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u32x4 operator/(u32 a, v2u32x4 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u32x4 operator^(v2u32x4 a, v2u32x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x4 operator^(v2u32x4 a, v2u32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x4 operator^(v2u32 a, v2u32x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x4 operator^(v2u32x4 a, u32 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u32x4 operator^(u32 a, v2u32x4 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u32x4 operator&(v2u32x4 a, v2u32x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x4 operator&(v2u32x4 a, v2u32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x4 operator&(v2u32 a, v2u32x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x4 operator&(v2u32x4 a, u32 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u32x4 operator&(u32 a, v2u32x4 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u32x4 operator|(v2u32x4 a, v2u32x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x4 operator|(v2u32x4 a, v2u32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x4 operator|(v2u32 a, v2u32x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x4 operator|(v2u32x4 a, u32 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u32x4 operator|(u32 a, v2u32x4 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u32x4 operator<<(v2u32x4 a, u32 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u32x4 operator>>(v2u32x4 a, u32 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s32x4() const;
	forceinline operator v2f32x4() const;
};

forceinline v2u32x4 V2u32x4(v2u32 a) { return {simd_typed::U32x4(a.x), simd_typed::U32x4(a.y), }; }
forceinline simd_typed::u32x4 dot(v2u32x4 a, v2u32x4 b) { return a.x*b.x+a.y*b.y; }

union v2u64x2 {
	struct { simd_typed::u64x2 x, y; };
	simd_typed::u64x2 d[2];
	forceinline v2u64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u64x2 operator+(v2u64x2 a, v2u64x2 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x2 operator+(v2u64x2 a, v2u64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x2 operator+(v2u64 a, v2u64x2 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x2 operator+(v2u64x2 a, u64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u64x2 operator+(u64 a, v2u64x2 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u64x2 operator-(v2u64x2 a, v2u64x2 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x2 operator-(v2u64x2 a, v2u64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x2 operator-(v2u64 a, v2u64x2 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x2 operator-(v2u64x2 a, u64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u64x2 operator-(u64 a, v2u64x2 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u64x2 operator*(v2u64x2 a, v2u64x2 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x2 operator*(v2u64x2 a, v2u64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x2 operator*(v2u64 a, v2u64x2 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x2 operator*(v2u64x2 a, u64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u64x2 operator*(u64 a, v2u64x2 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u64x2 operator/(v2u64x2 a, v2u64x2 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x2 operator/(v2u64x2 a, v2u64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x2 operator/(v2u64 a, v2u64x2 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x2 operator/(v2u64x2 a, u64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u64x2 operator/(u64 a, v2u64x2 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u64x2 operator^(v2u64x2 a, v2u64x2 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x2 operator^(v2u64x2 a, v2u64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x2 operator^(v2u64 a, v2u64x2 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x2 operator^(v2u64x2 a, u64 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u64x2 operator^(u64 a, v2u64x2 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u64x2 operator&(v2u64x2 a, v2u64x2 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x2 operator&(v2u64x2 a, v2u64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x2 operator&(v2u64 a, v2u64x2 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x2 operator&(v2u64x2 a, u64 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u64x2 operator&(u64 a, v2u64x2 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u64x2 operator|(v2u64x2 a, v2u64x2 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x2 operator|(v2u64x2 a, v2u64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x2 operator|(v2u64 a, v2u64x2 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x2 operator|(v2u64x2 a, u64 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u64x2 operator|(u64 a, v2u64x2 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u64x2 operator<<(v2u64x2 a, u64 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u64x2 operator>>(v2u64x2 a, u64 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s64x2() const;
	forceinline operator v2f64x2() const;
};

forceinline v2u64x2 V2u64x2(v2u64 a) { return {simd_typed::U64x2(a.x), simd_typed::U64x2(a.y), }; }
forceinline simd_typed::u64x2 dot(v2u64x2 a, v2u64x2 b) { return a.x*b.x+a.y*b.y; }

union v2s8x16 {
	struct { simd_typed::s8x16 x, y; };
	simd_typed::s8x16 d[2];
	forceinline v2s8 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s8x16 operator+(v2s8x16 a, v2s8x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x16 operator+(v2s8x16 a, v2s8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x16 operator+(v2s8 a, v2s8x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x16 operator+(v2s8x16 a, s8 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s8x16 operator+(s8 a, v2s8x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s8x16 operator-(v2s8x16 a, v2s8x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x16 operator-(v2s8x16 a, v2s8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x16 operator-(v2s8 a, v2s8x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x16 operator-(v2s8x16 a, s8 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s8x16 operator-(s8 a, v2s8x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s8x16 operator*(v2s8x16 a, v2s8x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x16 operator*(v2s8x16 a, v2s8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x16 operator*(v2s8 a, v2s8x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x16 operator*(v2s8x16 a, s8 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s8x16 operator*(s8 a, v2s8x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s8x16 operator/(v2s8x16 a, v2s8x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x16 operator/(v2s8x16 a, v2s8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x16 operator/(v2s8 a, v2s8x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x16 operator/(v2s8x16 a, s8 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s8x16 operator/(s8 a, v2s8x16 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s8x16 operator^(v2s8x16 a, v2s8x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x16 operator^(v2s8x16 a, v2s8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x16 operator^(v2s8 a, v2s8x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x16 operator^(v2s8x16 a, s8 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s8x16 operator^(s8 a, v2s8x16 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s8x16 operator&(v2s8x16 a, v2s8x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x16 operator&(v2s8x16 a, v2s8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x16 operator&(v2s8 a, v2s8x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x16 operator&(v2s8x16 a, s8 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s8x16 operator&(s8 a, v2s8x16 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s8x16 operator|(v2s8x16 a, v2s8x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x16 operator|(v2s8x16 a, v2s8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x16 operator|(v2s8 a, v2s8x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x16 operator|(v2s8x16 a, s8 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s8x16 operator|(s8 a, v2s8x16 b) { return {a|b.x, a|b.y};}
	forceinline operator v2u8x16() const;
};

forceinline v2s8x16 V2s8x16(v2s8 a) { return {simd_typed::S8x16(a.x), simd_typed::S8x16(a.y), }; }
forceinline simd_typed::s8x16 dot(v2s8x16 a, v2s8x16 b) { return a.x*b.x+a.y*b.y; }

union v2s16x8 {
	struct { simd_typed::s16x8 x, y; };
	simd_typed::s16x8 d[2];
	forceinline v2s16 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s16x8 operator+(v2s16x8 a, v2s16x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x8 operator+(v2s16x8 a, v2s16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x8 operator+(v2s16 a, v2s16x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x8 operator+(v2s16x8 a, s16 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s16x8 operator+(s16 a, v2s16x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s16x8 operator-(v2s16x8 a, v2s16x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x8 operator-(v2s16x8 a, v2s16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x8 operator-(v2s16 a, v2s16x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x8 operator-(v2s16x8 a, s16 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s16x8 operator-(s16 a, v2s16x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s16x8 operator*(v2s16x8 a, v2s16x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x8 operator*(v2s16x8 a, v2s16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x8 operator*(v2s16 a, v2s16x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x8 operator*(v2s16x8 a, s16 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s16x8 operator*(s16 a, v2s16x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s16x8 operator/(v2s16x8 a, v2s16x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x8 operator/(v2s16x8 a, v2s16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x8 operator/(v2s16 a, v2s16x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x8 operator/(v2s16x8 a, s16 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s16x8 operator/(s16 a, v2s16x8 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s16x8 operator^(v2s16x8 a, v2s16x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x8 operator^(v2s16x8 a, v2s16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x8 operator^(v2s16 a, v2s16x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x8 operator^(v2s16x8 a, s16 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s16x8 operator^(s16 a, v2s16x8 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s16x8 operator&(v2s16x8 a, v2s16x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x8 operator&(v2s16x8 a, v2s16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x8 operator&(v2s16 a, v2s16x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x8 operator&(v2s16x8 a, s16 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s16x8 operator&(s16 a, v2s16x8 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s16x8 operator|(v2s16x8 a, v2s16x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x8 operator|(v2s16x8 a, v2s16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x8 operator|(v2s16 a, v2s16x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x8 operator|(v2s16x8 a, s16 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s16x8 operator|(s16 a, v2s16x8 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s16x8 operator<<(v2s16x8 a, s16 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s16x8 operator>>(v2s16x8 a, s16 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u16x8() const;
};

forceinline v2s16x8 V2s16x8(v2s16 a) { return {simd_typed::S16x8(a.x), simd_typed::S16x8(a.y), }; }
forceinline simd_typed::s16x8 dot(v2s16x8 a, v2s16x8 b) { return a.x*b.x+a.y*b.y; }

union v2s32x4 {
	struct { simd_typed::s32x4 x, y; };
	simd_typed::s32x4 d[2];
	forceinline v2s32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s32x4 operator+(v2s32x4 a, v2s32x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x4 operator+(v2s32x4 a, v2s32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x4 operator+(v2s32 a, v2s32x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x4 operator+(v2s32x4 a, s32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s32x4 operator+(s32 a, v2s32x4 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s32x4 operator-(v2s32x4 a, v2s32x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x4 operator-(v2s32x4 a, v2s32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x4 operator-(v2s32 a, v2s32x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x4 operator-(v2s32x4 a, s32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s32x4 operator-(s32 a, v2s32x4 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s32x4 operator*(v2s32x4 a, v2s32x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x4 operator*(v2s32x4 a, v2s32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x4 operator*(v2s32 a, v2s32x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x4 operator*(v2s32x4 a, s32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s32x4 operator*(s32 a, v2s32x4 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s32x4 operator/(v2s32x4 a, v2s32x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x4 operator/(v2s32x4 a, v2s32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x4 operator/(v2s32 a, v2s32x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x4 operator/(v2s32x4 a, s32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s32x4 operator/(s32 a, v2s32x4 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s32x4 operator^(v2s32x4 a, v2s32x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x4 operator^(v2s32x4 a, v2s32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x4 operator^(v2s32 a, v2s32x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x4 operator^(v2s32x4 a, s32 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s32x4 operator^(s32 a, v2s32x4 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s32x4 operator&(v2s32x4 a, v2s32x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x4 operator&(v2s32x4 a, v2s32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x4 operator&(v2s32 a, v2s32x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x4 operator&(v2s32x4 a, s32 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s32x4 operator&(s32 a, v2s32x4 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s32x4 operator|(v2s32x4 a, v2s32x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x4 operator|(v2s32x4 a, v2s32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x4 operator|(v2s32 a, v2s32x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x4 operator|(v2s32x4 a, s32 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s32x4 operator|(s32 a, v2s32x4 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s32x4 operator<<(v2s32x4 a, s32 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s32x4 operator>>(v2s32x4 a, s32 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u32x4() const;
	forceinline operator v2f32x4() const;
};

forceinline v2s32x4 V2s32x4(v2s32 a) { return {simd_typed::S32x4(a.x), simd_typed::S32x4(a.y), }; }
forceinline simd_typed::s32x4 dot(v2s32x4 a, v2s32x4 b) { return a.x*b.x+a.y*b.y; }

union v2s64x2 {
	struct { simd_typed::s64x2 x, y; };
	simd_typed::s64x2 d[2];
	forceinline v2s64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s64x2 operator+(v2s64x2 a, v2s64x2 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x2 operator+(v2s64x2 a, v2s64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x2 operator+(v2s64 a, v2s64x2 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x2 operator+(v2s64x2 a, s64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s64x2 operator+(s64 a, v2s64x2 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s64x2 operator-(v2s64x2 a, v2s64x2 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x2 operator-(v2s64x2 a, v2s64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x2 operator-(v2s64 a, v2s64x2 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x2 operator-(v2s64x2 a, s64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s64x2 operator-(s64 a, v2s64x2 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s64x2 operator*(v2s64x2 a, v2s64x2 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x2 operator*(v2s64x2 a, v2s64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x2 operator*(v2s64 a, v2s64x2 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x2 operator*(v2s64x2 a, s64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s64x2 operator*(s64 a, v2s64x2 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s64x2 operator/(v2s64x2 a, v2s64x2 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x2 operator/(v2s64x2 a, v2s64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x2 operator/(v2s64 a, v2s64x2 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x2 operator/(v2s64x2 a, s64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s64x2 operator/(s64 a, v2s64x2 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s64x2 operator^(v2s64x2 a, v2s64x2 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x2 operator^(v2s64x2 a, v2s64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x2 operator^(v2s64 a, v2s64x2 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x2 operator^(v2s64x2 a, s64 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s64x2 operator^(s64 a, v2s64x2 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s64x2 operator&(v2s64x2 a, v2s64x2 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x2 operator&(v2s64x2 a, v2s64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x2 operator&(v2s64 a, v2s64x2 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x2 operator&(v2s64x2 a, s64 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s64x2 operator&(s64 a, v2s64x2 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s64x2 operator|(v2s64x2 a, v2s64x2 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x2 operator|(v2s64x2 a, v2s64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x2 operator|(v2s64 a, v2s64x2 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x2 operator|(v2s64x2 a, s64 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s64x2 operator|(s64 a, v2s64x2 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s64x2 operator<<(v2s64x2 a, s64 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s64x2 operator>>(v2s64x2 a, s64 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u64x2() const;
	forceinline operator v2f64x2() const;
};

forceinline v2s64x2 V2s64x2(v2s64 a) { return {simd_typed::S64x2(a.x), simd_typed::S64x2(a.y), }; }
forceinline simd_typed::s64x2 dot(v2s64x2 a, v2s64x2 b) { return a.x*b.x+a.y*b.y; }

union v2f32x4 {
	struct { simd_typed::f32x4 x, y; };
	simd_typed::f32x4 d[2];
	forceinline v2f32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2f32x4 operator+(v2f32x4 a, v2f32x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x4 operator+(v2f32x4 a, v2f32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x4 operator+(v2f32 a, v2f32x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x4 operator+(v2f32x4 a, f32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2f32x4 operator+(f32 a, v2f32x4 b) { return {a+b.x, a+b.y};}
	forceinline friend v2f32x4 operator-(v2f32x4 a, v2f32x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x4 operator-(v2f32x4 a, v2f32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x4 operator-(v2f32 a, v2f32x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x4 operator-(v2f32x4 a, f32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2f32x4 operator-(f32 a, v2f32x4 b) { return {a-b.x, a-b.y};}
	forceinline friend v2f32x4 operator*(v2f32x4 a, v2f32x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x4 operator*(v2f32x4 a, v2f32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x4 operator*(v2f32 a, v2f32x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x4 operator*(v2f32x4 a, f32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2f32x4 operator*(f32 a, v2f32x4 b) { return {a*b.x, a*b.y};}
	forceinline friend v2f32x4 operator/(v2f32x4 a, v2f32x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x4 operator/(v2f32x4 a, v2f32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x4 operator/(v2f32 a, v2f32x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x4 operator/(v2f32x4 a, f32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2f32x4 operator/(f32 a, v2f32x4 b) { return {a/b.x, a/b.y};}
	forceinline operator v2u32x4() const;
	forceinline operator v2s32x4() const;
};

forceinline v2f32x4 V2f32x4(v2f32 a) { return {simd_typed::F32x4(a.x), simd_typed::F32x4(a.y), }; }
forceinline simd_typed::f32x4 dot(v2f32x4 a, v2f32x4 b) { return a.x*b.x+a.y*b.y; }

union v2f64x2 {
	struct { simd_typed::f64x2 x, y; };
	simd_typed::f64x2 d[2];
	forceinline v2f64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2f64x2 operator+(v2f64x2 a, v2f64x2 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x2 operator+(v2f64x2 a, v2f64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x2 operator+(v2f64 a, v2f64x2 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x2 operator+(v2f64x2 a, f64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2f64x2 operator+(f64 a, v2f64x2 b) { return {a+b.x, a+b.y};}
	forceinline friend v2f64x2 operator-(v2f64x2 a, v2f64x2 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x2 operator-(v2f64x2 a, v2f64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x2 operator-(v2f64 a, v2f64x2 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x2 operator-(v2f64x2 a, f64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2f64x2 operator-(f64 a, v2f64x2 b) { return {a-b.x, a-b.y};}
	forceinline friend v2f64x2 operator*(v2f64x2 a, v2f64x2 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x2 operator*(v2f64x2 a, v2f64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x2 operator*(v2f64 a, v2f64x2 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x2 operator*(v2f64x2 a, f64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2f64x2 operator*(f64 a, v2f64x2 b) { return {a*b.x, a*b.y};}
	forceinline friend v2f64x2 operator/(v2f64x2 a, v2f64x2 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x2 operator/(v2f64x2 a, v2f64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x2 operator/(v2f64 a, v2f64x2 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x2 operator/(v2f64x2 a, f64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2f64x2 operator/(f64 a, v2f64x2 b) { return {a/b.x, a/b.y};}
	forceinline operator v2u64x2() const;
	forceinline operator v2s64x2() const;
};

forceinline v2f64x2 V2f64x2(v2f64 a) { return {simd_typed::F64x2(a.x), simd_typed::F64x2(a.y), }; }
forceinline simd_typed::f64x2 dot(v2f64x2 a, v2f64x2 b) { return a.x*b.x+a.y*b.y; }

forceinline v2u8x16::operator v2s8x16() const { return {(simd_typed::s8x16)x, (simd_typed::s8x16)y, }; }
forceinline v2u16x8::operator v2s16x8() const { return {(simd_typed::s16x8)x, (simd_typed::s16x8)y, }; }
forceinline v2u32x4::operator v2s32x4() const { return {(simd_typed::s32x4)x, (simd_typed::s32x4)y, }; }
forceinline v2u32x4::operator v2f32x4() const { return {(simd_typed::f32x4)x, (simd_typed::f32x4)y, }; }
forceinline v2u64x2::operator v2s64x2() const { return {(simd_typed::s64x2)x, (simd_typed::s64x2)y, }; }
forceinline v2u64x2::operator v2f64x2() const { return {(simd_typed::f64x2)x, (simd_typed::f64x2)y, }; }
forceinline v2s8x16::operator v2u8x16() const { return {(simd_typed::u8x16)x, (simd_typed::u8x16)y, }; }
forceinline v2s16x8::operator v2u16x8() const { return {(simd_typed::u16x8)x, (simd_typed::u16x8)y, }; }
forceinline v2s32x4::operator v2u32x4() const { return {(simd_typed::u32x4)x, (simd_typed::u32x4)y, }; }
forceinline v2s32x4::operator v2f32x4() const { return {(simd_typed::f32x4)x, (simd_typed::f32x4)y, }; }
forceinline v2s64x2::operator v2u64x2() const { return {(simd_typed::u64x2)x, (simd_typed::u64x2)y, }; }
forceinline v2s64x2::operator v2f64x2() const { return {(simd_typed::f64x2)x, (simd_typed::f64x2)y, }; }
forceinline v2f32x4::operator v2u32x4() const { return {(simd_typed::u32x4)x, (simd_typed::u32x4)y, }; }
forceinline v2f32x4::operator v2s32x4() const { return {(simd_typed::s32x4)x, (simd_typed::s32x4)y, }; }
forceinline v2f64x2::operator v2u64x2() const { return {(simd_typed::u64x2)x, (simd_typed::u64x2)y, }; }
forceinline v2f64x2::operator v2s64x2() const { return {(simd_typed::s64x2)x, (simd_typed::s64x2)y, }; }
union v3u8x16 {
	struct { simd_typed::u8x16 x, y, z; };
	simd_typed::u8x16 d[3];
	forceinline v3u8 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u8x16 operator+(v3u8x16 a, v3u8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x16 operator+(v3u8x16 a, v3u8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x16 operator+(v3u8 a, v3u8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x16 operator+(v3u8x16 a, u8 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u8x16 operator+(u8 a, v3u8x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u8x16 operator-(v3u8x16 a, v3u8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x16 operator-(v3u8x16 a, v3u8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x16 operator-(v3u8 a, v3u8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x16 operator-(v3u8x16 a, u8 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u8x16 operator-(u8 a, v3u8x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u8x16 operator*(v3u8x16 a, v3u8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x16 operator*(v3u8x16 a, v3u8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x16 operator*(v3u8 a, v3u8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x16 operator*(v3u8x16 a, u8 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u8x16 operator*(u8 a, v3u8x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u8x16 operator/(v3u8x16 a, v3u8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x16 operator/(v3u8x16 a, v3u8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x16 operator/(v3u8 a, v3u8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x16 operator/(v3u8x16 a, u8 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u8x16 operator/(u8 a, v3u8x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u8x16 operator^(v3u8x16 a, v3u8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x16 operator^(v3u8x16 a, v3u8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x16 operator^(v3u8 a, v3u8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x16 operator^(v3u8x16 a, u8 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u8x16 operator^(u8 a, v3u8x16 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u8x16 operator&(v3u8x16 a, v3u8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x16 operator&(v3u8x16 a, v3u8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x16 operator&(v3u8 a, v3u8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x16 operator&(v3u8x16 a, u8 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u8x16 operator&(u8 a, v3u8x16 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u8x16 operator|(v3u8x16 a, v3u8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x16 operator|(v3u8x16 a, v3u8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x16 operator|(v3u8 a, v3u8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x16 operator|(v3u8x16 a, u8 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u8x16 operator|(u8 a, v3u8x16 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline operator v3s8x16() const;
};

forceinline v3u8x16 V3u8x16(v3u8 a) { return {simd_typed::U8x16(a.x), simd_typed::U8x16(a.y), simd_typed::U8x16(a.z), }; }
forceinline simd_typed::u8x16 dot(v3u8x16 a, v3u8x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u16x8 {
	struct { simd_typed::u16x8 x, y, z; };
	simd_typed::u16x8 d[3];
	forceinline v3u16 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u16x8 operator+(v3u16x8 a, v3u16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x8 operator+(v3u16x8 a, v3u16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x8 operator+(v3u16 a, v3u16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x8 operator+(v3u16x8 a, u16 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u16x8 operator+(u16 a, v3u16x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u16x8 operator-(v3u16x8 a, v3u16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x8 operator-(v3u16x8 a, v3u16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x8 operator-(v3u16 a, v3u16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x8 operator-(v3u16x8 a, u16 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u16x8 operator-(u16 a, v3u16x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u16x8 operator*(v3u16x8 a, v3u16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x8 operator*(v3u16x8 a, v3u16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x8 operator*(v3u16 a, v3u16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x8 operator*(v3u16x8 a, u16 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u16x8 operator*(u16 a, v3u16x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u16x8 operator/(v3u16x8 a, v3u16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x8 operator/(v3u16x8 a, v3u16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x8 operator/(v3u16 a, v3u16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x8 operator/(v3u16x8 a, u16 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u16x8 operator/(u16 a, v3u16x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u16x8 operator^(v3u16x8 a, v3u16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x8 operator^(v3u16x8 a, v3u16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x8 operator^(v3u16 a, v3u16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x8 operator^(v3u16x8 a, u16 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u16x8 operator^(u16 a, v3u16x8 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u16x8 operator&(v3u16x8 a, v3u16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x8 operator&(v3u16x8 a, v3u16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x8 operator&(v3u16 a, v3u16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x8 operator&(v3u16x8 a, u16 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u16x8 operator&(u16 a, v3u16x8 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u16x8 operator|(v3u16x8 a, v3u16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x8 operator|(v3u16x8 a, v3u16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x8 operator|(v3u16 a, v3u16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x8 operator|(v3u16x8 a, u16 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u16x8 operator|(u16 a, v3u16x8 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u16x8 operator<<(v3u16x8 a, u16 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u16x8 operator>>(v3u16x8 a, u16 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s16x8() const;
};

forceinline v3u16x8 V3u16x8(v3u16 a) { return {simd_typed::U16x8(a.x), simd_typed::U16x8(a.y), simd_typed::U16x8(a.z), }; }
forceinline simd_typed::u16x8 dot(v3u16x8 a, v3u16x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u32x4 {
	struct { simd_typed::u32x4 x, y, z; };
	simd_typed::u32x4 d[3];
	forceinline v3u32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u32x4 operator+(v3u32x4 a, v3u32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x4 operator+(v3u32x4 a, v3u32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x4 operator+(v3u32 a, v3u32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x4 operator+(v3u32x4 a, u32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u32x4 operator+(u32 a, v3u32x4 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u32x4 operator-(v3u32x4 a, v3u32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x4 operator-(v3u32x4 a, v3u32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x4 operator-(v3u32 a, v3u32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x4 operator-(v3u32x4 a, u32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u32x4 operator-(u32 a, v3u32x4 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u32x4 operator*(v3u32x4 a, v3u32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x4 operator*(v3u32x4 a, v3u32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x4 operator*(v3u32 a, v3u32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x4 operator*(v3u32x4 a, u32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u32x4 operator*(u32 a, v3u32x4 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u32x4 operator/(v3u32x4 a, v3u32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x4 operator/(v3u32x4 a, v3u32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x4 operator/(v3u32 a, v3u32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x4 operator/(v3u32x4 a, u32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u32x4 operator/(u32 a, v3u32x4 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u32x4 operator^(v3u32x4 a, v3u32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x4 operator^(v3u32x4 a, v3u32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x4 operator^(v3u32 a, v3u32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x4 operator^(v3u32x4 a, u32 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u32x4 operator^(u32 a, v3u32x4 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u32x4 operator&(v3u32x4 a, v3u32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x4 operator&(v3u32x4 a, v3u32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x4 operator&(v3u32 a, v3u32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x4 operator&(v3u32x4 a, u32 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u32x4 operator&(u32 a, v3u32x4 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u32x4 operator|(v3u32x4 a, v3u32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x4 operator|(v3u32x4 a, v3u32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x4 operator|(v3u32 a, v3u32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x4 operator|(v3u32x4 a, u32 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u32x4 operator|(u32 a, v3u32x4 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u32x4 operator<<(v3u32x4 a, u32 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u32x4 operator>>(v3u32x4 a, u32 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s32x4() const;
	forceinline operator v3f32x4() const;
};

forceinline v3u32x4 V3u32x4(v3u32 a) { return {simd_typed::U32x4(a.x), simd_typed::U32x4(a.y), simd_typed::U32x4(a.z), }; }
forceinline simd_typed::u32x4 dot(v3u32x4 a, v3u32x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u64x2 {
	struct { simd_typed::u64x2 x, y, z; };
	simd_typed::u64x2 d[3];
	forceinline v3u64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u64x2 operator+(v3u64x2 a, v3u64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x2 operator+(v3u64x2 a, v3u64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x2 operator+(v3u64 a, v3u64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x2 operator+(v3u64x2 a, u64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u64x2 operator+(u64 a, v3u64x2 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u64x2 operator-(v3u64x2 a, v3u64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x2 operator-(v3u64x2 a, v3u64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x2 operator-(v3u64 a, v3u64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x2 operator-(v3u64x2 a, u64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u64x2 operator-(u64 a, v3u64x2 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u64x2 operator*(v3u64x2 a, v3u64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x2 operator*(v3u64x2 a, v3u64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x2 operator*(v3u64 a, v3u64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x2 operator*(v3u64x2 a, u64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u64x2 operator*(u64 a, v3u64x2 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u64x2 operator/(v3u64x2 a, v3u64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x2 operator/(v3u64x2 a, v3u64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x2 operator/(v3u64 a, v3u64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x2 operator/(v3u64x2 a, u64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u64x2 operator/(u64 a, v3u64x2 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u64x2 operator^(v3u64x2 a, v3u64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x2 operator^(v3u64x2 a, v3u64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x2 operator^(v3u64 a, v3u64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x2 operator^(v3u64x2 a, u64 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u64x2 operator^(u64 a, v3u64x2 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u64x2 operator&(v3u64x2 a, v3u64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x2 operator&(v3u64x2 a, v3u64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x2 operator&(v3u64 a, v3u64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x2 operator&(v3u64x2 a, u64 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u64x2 operator&(u64 a, v3u64x2 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u64x2 operator|(v3u64x2 a, v3u64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x2 operator|(v3u64x2 a, v3u64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x2 operator|(v3u64 a, v3u64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x2 operator|(v3u64x2 a, u64 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u64x2 operator|(u64 a, v3u64x2 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u64x2 operator<<(v3u64x2 a, u64 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u64x2 operator>>(v3u64x2 a, u64 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s64x2() const;
	forceinline operator v3f64x2() const;
};

forceinline v3u64x2 V3u64x2(v3u64 a) { return {simd_typed::U64x2(a.x), simd_typed::U64x2(a.y), simd_typed::U64x2(a.z), }; }
forceinline simd_typed::u64x2 dot(v3u64x2 a, v3u64x2 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s8x16 {
	struct { simd_typed::s8x16 x, y, z; };
	simd_typed::s8x16 d[3];
	forceinline v3s8 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s8x16 operator+(v3s8x16 a, v3s8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x16 operator+(v3s8x16 a, v3s8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x16 operator+(v3s8 a, v3s8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x16 operator+(v3s8x16 a, s8 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s8x16 operator+(s8 a, v3s8x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s8x16 operator-(v3s8x16 a, v3s8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x16 operator-(v3s8x16 a, v3s8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x16 operator-(v3s8 a, v3s8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x16 operator-(v3s8x16 a, s8 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s8x16 operator-(s8 a, v3s8x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s8x16 operator*(v3s8x16 a, v3s8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x16 operator*(v3s8x16 a, v3s8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x16 operator*(v3s8 a, v3s8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x16 operator*(v3s8x16 a, s8 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s8x16 operator*(s8 a, v3s8x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s8x16 operator/(v3s8x16 a, v3s8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x16 operator/(v3s8x16 a, v3s8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x16 operator/(v3s8 a, v3s8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x16 operator/(v3s8x16 a, s8 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s8x16 operator/(s8 a, v3s8x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s8x16 operator^(v3s8x16 a, v3s8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x16 operator^(v3s8x16 a, v3s8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x16 operator^(v3s8 a, v3s8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x16 operator^(v3s8x16 a, s8 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s8x16 operator^(s8 a, v3s8x16 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s8x16 operator&(v3s8x16 a, v3s8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x16 operator&(v3s8x16 a, v3s8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x16 operator&(v3s8 a, v3s8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x16 operator&(v3s8x16 a, s8 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s8x16 operator&(s8 a, v3s8x16 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s8x16 operator|(v3s8x16 a, v3s8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x16 operator|(v3s8x16 a, v3s8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x16 operator|(v3s8 a, v3s8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x16 operator|(v3s8x16 a, s8 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s8x16 operator|(s8 a, v3s8x16 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline operator v3u8x16() const;
};

forceinline v3s8x16 V3s8x16(v3s8 a) { return {simd_typed::S8x16(a.x), simd_typed::S8x16(a.y), simd_typed::S8x16(a.z), }; }
forceinline simd_typed::s8x16 dot(v3s8x16 a, v3s8x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s16x8 {
	struct { simd_typed::s16x8 x, y, z; };
	simd_typed::s16x8 d[3];
	forceinline v3s16 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s16x8 operator+(v3s16x8 a, v3s16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x8 operator+(v3s16x8 a, v3s16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x8 operator+(v3s16 a, v3s16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x8 operator+(v3s16x8 a, s16 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s16x8 operator+(s16 a, v3s16x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s16x8 operator-(v3s16x8 a, v3s16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x8 operator-(v3s16x8 a, v3s16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x8 operator-(v3s16 a, v3s16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x8 operator-(v3s16x8 a, s16 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s16x8 operator-(s16 a, v3s16x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s16x8 operator*(v3s16x8 a, v3s16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x8 operator*(v3s16x8 a, v3s16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x8 operator*(v3s16 a, v3s16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x8 operator*(v3s16x8 a, s16 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s16x8 operator*(s16 a, v3s16x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s16x8 operator/(v3s16x8 a, v3s16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x8 operator/(v3s16x8 a, v3s16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x8 operator/(v3s16 a, v3s16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x8 operator/(v3s16x8 a, s16 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s16x8 operator/(s16 a, v3s16x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s16x8 operator^(v3s16x8 a, v3s16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x8 operator^(v3s16x8 a, v3s16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x8 operator^(v3s16 a, v3s16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x8 operator^(v3s16x8 a, s16 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s16x8 operator^(s16 a, v3s16x8 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s16x8 operator&(v3s16x8 a, v3s16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x8 operator&(v3s16x8 a, v3s16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x8 operator&(v3s16 a, v3s16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x8 operator&(v3s16x8 a, s16 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s16x8 operator&(s16 a, v3s16x8 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s16x8 operator|(v3s16x8 a, v3s16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x8 operator|(v3s16x8 a, v3s16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x8 operator|(v3s16 a, v3s16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x8 operator|(v3s16x8 a, s16 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s16x8 operator|(s16 a, v3s16x8 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s16x8 operator<<(v3s16x8 a, s16 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s16x8 operator>>(v3s16x8 a, s16 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u16x8() const;
};

forceinline v3s16x8 V3s16x8(v3s16 a) { return {simd_typed::S16x8(a.x), simd_typed::S16x8(a.y), simd_typed::S16x8(a.z), }; }
forceinline simd_typed::s16x8 dot(v3s16x8 a, v3s16x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s32x4 {
	struct { simd_typed::s32x4 x, y, z; };
	simd_typed::s32x4 d[3];
	forceinline v3s32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s32x4 operator+(v3s32x4 a, v3s32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x4 operator+(v3s32x4 a, v3s32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x4 operator+(v3s32 a, v3s32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x4 operator+(v3s32x4 a, s32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s32x4 operator+(s32 a, v3s32x4 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s32x4 operator-(v3s32x4 a, v3s32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x4 operator-(v3s32x4 a, v3s32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x4 operator-(v3s32 a, v3s32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x4 operator-(v3s32x4 a, s32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s32x4 operator-(s32 a, v3s32x4 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s32x4 operator*(v3s32x4 a, v3s32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x4 operator*(v3s32x4 a, v3s32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x4 operator*(v3s32 a, v3s32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x4 operator*(v3s32x4 a, s32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s32x4 operator*(s32 a, v3s32x4 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s32x4 operator/(v3s32x4 a, v3s32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x4 operator/(v3s32x4 a, v3s32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x4 operator/(v3s32 a, v3s32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x4 operator/(v3s32x4 a, s32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s32x4 operator/(s32 a, v3s32x4 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s32x4 operator^(v3s32x4 a, v3s32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x4 operator^(v3s32x4 a, v3s32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x4 operator^(v3s32 a, v3s32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x4 operator^(v3s32x4 a, s32 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s32x4 operator^(s32 a, v3s32x4 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s32x4 operator&(v3s32x4 a, v3s32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x4 operator&(v3s32x4 a, v3s32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x4 operator&(v3s32 a, v3s32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x4 operator&(v3s32x4 a, s32 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s32x4 operator&(s32 a, v3s32x4 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s32x4 operator|(v3s32x4 a, v3s32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x4 operator|(v3s32x4 a, v3s32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x4 operator|(v3s32 a, v3s32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x4 operator|(v3s32x4 a, s32 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s32x4 operator|(s32 a, v3s32x4 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s32x4 operator<<(v3s32x4 a, s32 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s32x4 operator>>(v3s32x4 a, s32 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u32x4() const;
	forceinline operator v3f32x4() const;
};

forceinline v3s32x4 V3s32x4(v3s32 a) { return {simd_typed::S32x4(a.x), simd_typed::S32x4(a.y), simd_typed::S32x4(a.z), }; }
forceinline simd_typed::s32x4 dot(v3s32x4 a, v3s32x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s64x2 {
	struct { simd_typed::s64x2 x, y, z; };
	simd_typed::s64x2 d[3];
	forceinline v3s64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s64x2 operator+(v3s64x2 a, v3s64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x2 operator+(v3s64x2 a, v3s64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x2 operator+(v3s64 a, v3s64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x2 operator+(v3s64x2 a, s64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s64x2 operator+(s64 a, v3s64x2 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s64x2 operator-(v3s64x2 a, v3s64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x2 operator-(v3s64x2 a, v3s64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x2 operator-(v3s64 a, v3s64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x2 operator-(v3s64x2 a, s64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s64x2 operator-(s64 a, v3s64x2 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s64x2 operator*(v3s64x2 a, v3s64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x2 operator*(v3s64x2 a, v3s64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x2 operator*(v3s64 a, v3s64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x2 operator*(v3s64x2 a, s64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s64x2 operator*(s64 a, v3s64x2 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s64x2 operator/(v3s64x2 a, v3s64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x2 operator/(v3s64x2 a, v3s64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x2 operator/(v3s64 a, v3s64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x2 operator/(v3s64x2 a, s64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s64x2 operator/(s64 a, v3s64x2 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s64x2 operator^(v3s64x2 a, v3s64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x2 operator^(v3s64x2 a, v3s64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x2 operator^(v3s64 a, v3s64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x2 operator^(v3s64x2 a, s64 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s64x2 operator^(s64 a, v3s64x2 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s64x2 operator&(v3s64x2 a, v3s64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x2 operator&(v3s64x2 a, v3s64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x2 operator&(v3s64 a, v3s64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x2 operator&(v3s64x2 a, s64 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s64x2 operator&(s64 a, v3s64x2 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s64x2 operator|(v3s64x2 a, v3s64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x2 operator|(v3s64x2 a, v3s64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x2 operator|(v3s64 a, v3s64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x2 operator|(v3s64x2 a, s64 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s64x2 operator|(s64 a, v3s64x2 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s64x2 operator<<(v3s64x2 a, s64 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s64x2 operator>>(v3s64x2 a, s64 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u64x2() const;
	forceinline operator v3f64x2() const;
};

forceinline v3s64x2 V3s64x2(v3s64 a) { return {simd_typed::S64x2(a.x), simd_typed::S64x2(a.y), simd_typed::S64x2(a.z), }; }
forceinline simd_typed::s64x2 dot(v3s64x2 a, v3s64x2 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3f32x4 {
	struct { simd_typed::f32x4 x, y, z; };
	simd_typed::f32x4 d[3];
	forceinline v3f32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3f32x4 operator+(v3f32x4 a, v3f32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x4 operator+(v3f32x4 a, v3f32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x4 operator+(v3f32 a, v3f32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x4 operator+(v3f32x4 a, f32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3f32x4 operator+(f32 a, v3f32x4 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3f32x4 operator-(v3f32x4 a, v3f32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x4 operator-(v3f32x4 a, v3f32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x4 operator-(v3f32 a, v3f32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x4 operator-(v3f32x4 a, f32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3f32x4 operator-(f32 a, v3f32x4 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3f32x4 operator*(v3f32x4 a, v3f32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x4 operator*(v3f32x4 a, v3f32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x4 operator*(v3f32 a, v3f32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x4 operator*(v3f32x4 a, f32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3f32x4 operator*(f32 a, v3f32x4 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3f32x4 operator/(v3f32x4 a, v3f32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x4 operator/(v3f32x4 a, v3f32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x4 operator/(v3f32 a, v3f32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x4 operator/(v3f32x4 a, f32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3f32x4 operator/(f32 a, v3f32x4 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline operator v3u32x4() const;
	forceinline operator v3s32x4() const;
};

forceinline v3f32x4 V3f32x4(v3f32 a) { return {simd_typed::F32x4(a.x), simd_typed::F32x4(a.y), simd_typed::F32x4(a.z), }; }
forceinline simd_typed::f32x4 dot(v3f32x4 a, v3f32x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3f64x2 {
	struct { simd_typed::f64x2 x, y, z; };
	simd_typed::f64x2 d[3];
	forceinline v3f64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3f64x2 operator+(v3f64x2 a, v3f64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x2 operator+(v3f64x2 a, v3f64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x2 operator+(v3f64 a, v3f64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x2 operator+(v3f64x2 a, f64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3f64x2 operator+(f64 a, v3f64x2 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3f64x2 operator-(v3f64x2 a, v3f64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x2 operator-(v3f64x2 a, v3f64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x2 operator-(v3f64 a, v3f64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x2 operator-(v3f64x2 a, f64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3f64x2 operator-(f64 a, v3f64x2 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3f64x2 operator*(v3f64x2 a, v3f64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x2 operator*(v3f64x2 a, v3f64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x2 operator*(v3f64 a, v3f64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x2 operator*(v3f64x2 a, f64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3f64x2 operator*(f64 a, v3f64x2 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3f64x2 operator/(v3f64x2 a, v3f64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x2 operator/(v3f64x2 a, v3f64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x2 operator/(v3f64 a, v3f64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x2 operator/(v3f64x2 a, f64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3f64x2 operator/(f64 a, v3f64x2 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline operator v3u64x2() const;
	forceinline operator v3s64x2() const;
};

forceinline v3f64x2 V3f64x2(v3f64 a) { return {simd_typed::F64x2(a.x), simd_typed::F64x2(a.y), simd_typed::F64x2(a.z), }; }
forceinline simd_typed::f64x2 dot(v3f64x2 a, v3f64x2 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

forceinline v3u8x16::operator v3s8x16() const { return {(simd_typed::s8x16)x, (simd_typed::s8x16)y, (simd_typed::s8x16)z, }; }
forceinline v3u16x8::operator v3s16x8() const { return {(simd_typed::s16x8)x, (simd_typed::s16x8)y, (simd_typed::s16x8)z, }; }
forceinline v3u32x4::operator v3s32x4() const { return {(simd_typed::s32x4)x, (simd_typed::s32x4)y, (simd_typed::s32x4)z, }; }
forceinline v3u32x4::operator v3f32x4() const { return {(simd_typed::f32x4)x, (simd_typed::f32x4)y, (simd_typed::f32x4)z, }; }
forceinline v3u64x2::operator v3s64x2() const { return {(simd_typed::s64x2)x, (simd_typed::s64x2)y, (simd_typed::s64x2)z, }; }
forceinline v3u64x2::operator v3f64x2() const { return {(simd_typed::f64x2)x, (simd_typed::f64x2)y, (simd_typed::f64x2)z, }; }
forceinline v3s8x16::operator v3u8x16() const { return {(simd_typed::u8x16)x, (simd_typed::u8x16)y, (simd_typed::u8x16)z, }; }
forceinline v3s16x8::operator v3u16x8() const { return {(simd_typed::u16x8)x, (simd_typed::u16x8)y, (simd_typed::u16x8)z, }; }
forceinline v3s32x4::operator v3u32x4() const { return {(simd_typed::u32x4)x, (simd_typed::u32x4)y, (simd_typed::u32x4)z, }; }
forceinline v3s32x4::operator v3f32x4() const { return {(simd_typed::f32x4)x, (simd_typed::f32x4)y, (simd_typed::f32x4)z, }; }
forceinline v3s64x2::operator v3u64x2() const { return {(simd_typed::u64x2)x, (simd_typed::u64x2)y, (simd_typed::u64x2)z, }; }
forceinline v3s64x2::operator v3f64x2() const { return {(simd_typed::f64x2)x, (simd_typed::f64x2)y, (simd_typed::f64x2)z, }; }
forceinline v3f32x4::operator v3u32x4() const { return {(simd_typed::u32x4)x, (simd_typed::u32x4)y, (simd_typed::u32x4)z, }; }
forceinline v3f32x4::operator v3s32x4() const { return {(simd_typed::s32x4)x, (simd_typed::s32x4)y, (simd_typed::s32x4)z, }; }
forceinline v3f64x2::operator v3u64x2() const { return {(simd_typed::u64x2)x, (simd_typed::u64x2)y, (simd_typed::u64x2)z, }; }
forceinline v3f64x2::operator v3s64x2() const { return {(simd_typed::s64x2)x, (simd_typed::s64x2)y, (simd_typed::s64x2)z, }; }
union v4u8x16 {
	struct { simd_typed::u8x16 x, y, z, w; };
	simd_typed::u8x16 d[4];
	forceinline v4u8 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u8x16 operator+(v4u8x16 a, v4u8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x16 operator+(v4u8x16 a, v4u8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x16 operator+(v4u8 a, v4u8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x16 operator+(v4u8x16 a, u8 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u8x16 operator+(u8 a, v4u8x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u8x16 operator-(v4u8x16 a, v4u8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x16 operator-(v4u8x16 a, v4u8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x16 operator-(v4u8 a, v4u8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x16 operator-(v4u8x16 a, u8 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u8x16 operator-(u8 a, v4u8x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u8x16 operator*(v4u8x16 a, v4u8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x16 operator*(v4u8x16 a, v4u8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x16 operator*(v4u8 a, v4u8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x16 operator*(v4u8x16 a, u8 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u8x16 operator*(u8 a, v4u8x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u8x16 operator/(v4u8x16 a, v4u8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x16 operator/(v4u8x16 a, v4u8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x16 operator/(v4u8 a, v4u8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x16 operator/(v4u8x16 a, u8 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u8x16 operator/(u8 a, v4u8x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u8x16 operator^(v4u8x16 a, v4u8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x16 operator^(v4u8x16 a, v4u8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x16 operator^(v4u8 a, v4u8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x16 operator^(v4u8x16 a, u8 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u8x16 operator^(u8 a, v4u8x16 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u8x16 operator&(v4u8x16 a, v4u8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x16 operator&(v4u8x16 a, v4u8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x16 operator&(v4u8 a, v4u8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x16 operator&(v4u8x16 a, u8 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u8x16 operator&(u8 a, v4u8x16 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u8x16 operator|(v4u8x16 a, v4u8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x16 operator|(v4u8x16 a, v4u8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x16 operator|(v4u8 a, v4u8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x16 operator|(v4u8x16 a, u8 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u8x16 operator|(u8 a, v4u8x16 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline operator v4s8x16() const;
};

forceinline v4u8x16 V4u8x16(v4u8 a) { return {simd_typed::U8x16(a.x), simd_typed::U8x16(a.y), simd_typed::U8x16(a.z), simd_typed::U8x16(a.w), }; }
forceinline simd_typed::u8x16 dot(v4u8x16 a, v4u8x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u16x8 {
	struct { simd_typed::u16x8 x, y, z, w; };
	simd_typed::u16x8 d[4];
	forceinline v4u16 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u16x8 operator+(v4u16x8 a, v4u16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x8 operator+(v4u16x8 a, v4u16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x8 operator+(v4u16 a, v4u16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x8 operator+(v4u16x8 a, u16 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u16x8 operator+(u16 a, v4u16x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u16x8 operator-(v4u16x8 a, v4u16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x8 operator-(v4u16x8 a, v4u16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x8 operator-(v4u16 a, v4u16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x8 operator-(v4u16x8 a, u16 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u16x8 operator-(u16 a, v4u16x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u16x8 operator*(v4u16x8 a, v4u16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x8 operator*(v4u16x8 a, v4u16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x8 operator*(v4u16 a, v4u16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x8 operator*(v4u16x8 a, u16 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u16x8 operator*(u16 a, v4u16x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u16x8 operator/(v4u16x8 a, v4u16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x8 operator/(v4u16x8 a, v4u16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x8 operator/(v4u16 a, v4u16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x8 operator/(v4u16x8 a, u16 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u16x8 operator/(u16 a, v4u16x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u16x8 operator^(v4u16x8 a, v4u16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x8 operator^(v4u16x8 a, v4u16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x8 operator^(v4u16 a, v4u16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x8 operator^(v4u16x8 a, u16 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u16x8 operator^(u16 a, v4u16x8 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u16x8 operator&(v4u16x8 a, v4u16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x8 operator&(v4u16x8 a, v4u16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x8 operator&(v4u16 a, v4u16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x8 operator&(v4u16x8 a, u16 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u16x8 operator&(u16 a, v4u16x8 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u16x8 operator|(v4u16x8 a, v4u16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x8 operator|(v4u16x8 a, v4u16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x8 operator|(v4u16 a, v4u16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x8 operator|(v4u16x8 a, u16 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u16x8 operator|(u16 a, v4u16x8 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u16x8 operator<<(v4u16x8 a, u16 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u16x8 operator>>(v4u16x8 a, u16 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s16x8() const;
};

forceinline v4u16x8 V4u16x8(v4u16 a) { return {simd_typed::U16x8(a.x), simd_typed::U16x8(a.y), simd_typed::U16x8(a.z), simd_typed::U16x8(a.w), }; }
forceinline simd_typed::u16x8 dot(v4u16x8 a, v4u16x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u32x4 {
	struct { simd_typed::u32x4 x, y, z, w; };
	simd_typed::u32x4 d[4];
	forceinline v4u32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u32x4 operator+(v4u32x4 a, v4u32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x4 operator+(v4u32x4 a, v4u32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x4 operator+(v4u32 a, v4u32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x4 operator+(v4u32x4 a, u32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u32x4 operator+(u32 a, v4u32x4 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u32x4 operator-(v4u32x4 a, v4u32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x4 operator-(v4u32x4 a, v4u32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x4 operator-(v4u32 a, v4u32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x4 operator-(v4u32x4 a, u32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u32x4 operator-(u32 a, v4u32x4 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u32x4 operator*(v4u32x4 a, v4u32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x4 operator*(v4u32x4 a, v4u32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x4 operator*(v4u32 a, v4u32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x4 operator*(v4u32x4 a, u32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u32x4 operator*(u32 a, v4u32x4 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u32x4 operator/(v4u32x4 a, v4u32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x4 operator/(v4u32x4 a, v4u32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x4 operator/(v4u32 a, v4u32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x4 operator/(v4u32x4 a, u32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u32x4 operator/(u32 a, v4u32x4 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u32x4 operator^(v4u32x4 a, v4u32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x4 operator^(v4u32x4 a, v4u32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x4 operator^(v4u32 a, v4u32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x4 operator^(v4u32x4 a, u32 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u32x4 operator^(u32 a, v4u32x4 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u32x4 operator&(v4u32x4 a, v4u32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x4 operator&(v4u32x4 a, v4u32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x4 operator&(v4u32 a, v4u32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x4 operator&(v4u32x4 a, u32 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u32x4 operator&(u32 a, v4u32x4 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u32x4 operator|(v4u32x4 a, v4u32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x4 operator|(v4u32x4 a, v4u32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x4 operator|(v4u32 a, v4u32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x4 operator|(v4u32x4 a, u32 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u32x4 operator|(u32 a, v4u32x4 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u32x4 operator<<(v4u32x4 a, u32 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u32x4 operator>>(v4u32x4 a, u32 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s32x4() const;
	forceinline operator v4f32x4() const;
};

forceinline v4u32x4 V4u32x4(v4u32 a) { return {simd_typed::U32x4(a.x), simd_typed::U32x4(a.y), simd_typed::U32x4(a.z), simd_typed::U32x4(a.w), }; }
forceinline simd_typed::u32x4 dot(v4u32x4 a, v4u32x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u64x2 {
	struct { simd_typed::u64x2 x, y, z, w; };
	simd_typed::u64x2 d[4];
	forceinline v4u64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u64x2 operator+(v4u64x2 a, v4u64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x2 operator+(v4u64x2 a, v4u64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x2 operator+(v4u64 a, v4u64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x2 operator+(v4u64x2 a, u64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u64x2 operator+(u64 a, v4u64x2 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u64x2 operator-(v4u64x2 a, v4u64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x2 operator-(v4u64x2 a, v4u64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x2 operator-(v4u64 a, v4u64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x2 operator-(v4u64x2 a, u64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u64x2 operator-(u64 a, v4u64x2 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u64x2 operator*(v4u64x2 a, v4u64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x2 operator*(v4u64x2 a, v4u64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x2 operator*(v4u64 a, v4u64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x2 operator*(v4u64x2 a, u64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u64x2 operator*(u64 a, v4u64x2 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u64x2 operator/(v4u64x2 a, v4u64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x2 operator/(v4u64x2 a, v4u64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x2 operator/(v4u64 a, v4u64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x2 operator/(v4u64x2 a, u64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u64x2 operator/(u64 a, v4u64x2 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u64x2 operator^(v4u64x2 a, v4u64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x2 operator^(v4u64x2 a, v4u64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x2 operator^(v4u64 a, v4u64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x2 operator^(v4u64x2 a, u64 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u64x2 operator^(u64 a, v4u64x2 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u64x2 operator&(v4u64x2 a, v4u64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x2 operator&(v4u64x2 a, v4u64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x2 operator&(v4u64 a, v4u64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x2 operator&(v4u64x2 a, u64 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u64x2 operator&(u64 a, v4u64x2 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u64x2 operator|(v4u64x2 a, v4u64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x2 operator|(v4u64x2 a, v4u64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x2 operator|(v4u64 a, v4u64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x2 operator|(v4u64x2 a, u64 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u64x2 operator|(u64 a, v4u64x2 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u64x2 operator<<(v4u64x2 a, u64 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u64x2 operator>>(v4u64x2 a, u64 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s64x2() const;
	forceinline operator v4f64x2() const;
};

forceinline v4u64x2 V4u64x2(v4u64 a) { return {simd_typed::U64x2(a.x), simd_typed::U64x2(a.y), simd_typed::U64x2(a.z), simd_typed::U64x2(a.w), }; }
forceinline simd_typed::u64x2 dot(v4u64x2 a, v4u64x2 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s8x16 {
	struct { simd_typed::s8x16 x, y, z, w; };
	simd_typed::s8x16 d[4];
	forceinline v4s8 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s8x16 operator+(v4s8x16 a, v4s8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x16 operator+(v4s8x16 a, v4s8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x16 operator+(v4s8 a, v4s8x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x16 operator+(v4s8x16 a, s8 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s8x16 operator+(s8 a, v4s8x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s8x16 operator-(v4s8x16 a, v4s8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x16 operator-(v4s8x16 a, v4s8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x16 operator-(v4s8 a, v4s8x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x16 operator-(v4s8x16 a, s8 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s8x16 operator-(s8 a, v4s8x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s8x16 operator*(v4s8x16 a, v4s8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x16 operator*(v4s8x16 a, v4s8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x16 operator*(v4s8 a, v4s8x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x16 operator*(v4s8x16 a, s8 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s8x16 operator*(s8 a, v4s8x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s8x16 operator/(v4s8x16 a, v4s8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x16 operator/(v4s8x16 a, v4s8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x16 operator/(v4s8 a, v4s8x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x16 operator/(v4s8x16 a, s8 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s8x16 operator/(s8 a, v4s8x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s8x16 operator^(v4s8x16 a, v4s8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x16 operator^(v4s8x16 a, v4s8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x16 operator^(v4s8 a, v4s8x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x16 operator^(v4s8x16 a, s8 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s8x16 operator^(s8 a, v4s8x16 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s8x16 operator&(v4s8x16 a, v4s8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x16 operator&(v4s8x16 a, v4s8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x16 operator&(v4s8 a, v4s8x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x16 operator&(v4s8x16 a, s8 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s8x16 operator&(s8 a, v4s8x16 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s8x16 operator|(v4s8x16 a, v4s8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x16 operator|(v4s8x16 a, v4s8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x16 operator|(v4s8 a, v4s8x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x16 operator|(v4s8x16 a, s8 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s8x16 operator|(s8 a, v4s8x16 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline operator v4u8x16() const;
};

forceinline v4s8x16 V4s8x16(v4s8 a) { return {simd_typed::S8x16(a.x), simd_typed::S8x16(a.y), simd_typed::S8x16(a.z), simd_typed::S8x16(a.w), }; }
forceinline simd_typed::s8x16 dot(v4s8x16 a, v4s8x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s16x8 {
	struct { simd_typed::s16x8 x, y, z, w; };
	simd_typed::s16x8 d[4];
	forceinline v4s16 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s16x8 operator+(v4s16x8 a, v4s16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x8 operator+(v4s16x8 a, v4s16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x8 operator+(v4s16 a, v4s16x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x8 operator+(v4s16x8 a, s16 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s16x8 operator+(s16 a, v4s16x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s16x8 operator-(v4s16x8 a, v4s16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x8 operator-(v4s16x8 a, v4s16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x8 operator-(v4s16 a, v4s16x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x8 operator-(v4s16x8 a, s16 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s16x8 operator-(s16 a, v4s16x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s16x8 operator*(v4s16x8 a, v4s16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x8 operator*(v4s16x8 a, v4s16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x8 operator*(v4s16 a, v4s16x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x8 operator*(v4s16x8 a, s16 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s16x8 operator*(s16 a, v4s16x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s16x8 operator/(v4s16x8 a, v4s16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x8 operator/(v4s16x8 a, v4s16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x8 operator/(v4s16 a, v4s16x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x8 operator/(v4s16x8 a, s16 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s16x8 operator/(s16 a, v4s16x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s16x8 operator^(v4s16x8 a, v4s16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x8 operator^(v4s16x8 a, v4s16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x8 operator^(v4s16 a, v4s16x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x8 operator^(v4s16x8 a, s16 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s16x8 operator^(s16 a, v4s16x8 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s16x8 operator&(v4s16x8 a, v4s16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x8 operator&(v4s16x8 a, v4s16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x8 operator&(v4s16 a, v4s16x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x8 operator&(v4s16x8 a, s16 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s16x8 operator&(s16 a, v4s16x8 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s16x8 operator|(v4s16x8 a, v4s16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x8 operator|(v4s16x8 a, v4s16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x8 operator|(v4s16 a, v4s16x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x8 operator|(v4s16x8 a, s16 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s16x8 operator|(s16 a, v4s16x8 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s16x8 operator<<(v4s16x8 a, s16 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s16x8 operator>>(v4s16x8 a, s16 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u16x8() const;
};

forceinline v4s16x8 V4s16x8(v4s16 a) { return {simd_typed::S16x8(a.x), simd_typed::S16x8(a.y), simd_typed::S16x8(a.z), simd_typed::S16x8(a.w), }; }
forceinline simd_typed::s16x8 dot(v4s16x8 a, v4s16x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s32x4 {
	struct { simd_typed::s32x4 x, y, z, w; };
	simd_typed::s32x4 d[4];
	forceinline v4s32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s32x4 operator+(v4s32x4 a, v4s32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x4 operator+(v4s32x4 a, v4s32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x4 operator+(v4s32 a, v4s32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x4 operator+(v4s32x4 a, s32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s32x4 operator+(s32 a, v4s32x4 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s32x4 operator-(v4s32x4 a, v4s32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x4 operator-(v4s32x4 a, v4s32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x4 operator-(v4s32 a, v4s32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x4 operator-(v4s32x4 a, s32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s32x4 operator-(s32 a, v4s32x4 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s32x4 operator*(v4s32x4 a, v4s32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x4 operator*(v4s32x4 a, v4s32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x4 operator*(v4s32 a, v4s32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x4 operator*(v4s32x4 a, s32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s32x4 operator*(s32 a, v4s32x4 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s32x4 operator/(v4s32x4 a, v4s32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x4 operator/(v4s32x4 a, v4s32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x4 operator/(v4s32 a, v4s32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x4 operator/(v4s32x4 a, s32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s32x4 operator/(s32 a, v4s32x4 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s32x4 operator^(v4s32x4 a, v4s32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x4 operator^(v4s32x4 a, v4s32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x4 operator^(v4s32 a, v4s32x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x4 operator^(v4s32x4 a, s32 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s32x4 operator^(s32 a, v4s32x4 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s32x4 operator&(v4s32x4 a, v4s32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x4 operator&(v4s32x4 a, v4s32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x4 operator&(v4s32 a, v4s32x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x4 operator&(v4s32x4 a, s32 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s32x4 operator&(s32 a, v4s32x4 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s32x4 operator|(v4s32x4 a, v4s32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x4 operator|(v4s32x4 a, v4s32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x4 operator|(v4s32 a, v4s32x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x4 operator|(v4s32x4 a, s32 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s32x4 operator|(s32 a, v4s32x4 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s32x4 operator<<(v4s32x4 a, s32 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s32x4 operator>>(v4s32x4 a, s32 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u32x4() const;
	forceinline operator v4f32x4() const;
};

forceinline v4s32x4 V4s32x4(v4s32 a) { return {simd_typed::S32x4(a.x), simd_typed::S32x4(a.y), simd_typed::S32x4(a.z), simd_typed::S32x4(a.w), }; }
forceinline simd_typed::s32x4 dot(v4s32x4 a, v4s32x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s64x2 {
	struct { simd_typed::s64x2 x, y, z, w; };
	simd_typed::s64x2 d[4];
	forceinline v4s64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s64x2 operator+(v4s64x2 a, v4s64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x2 operator+(v4s64x2 a, v4s64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x2 operator+(v4s64 a, v4s64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x2 operator+(v4s64x2 a, s64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s64x2 operator+(s64 a, v4s64x2 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s64x2 operator-(v4s64x2 a, v4s64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x2 operator-(v4s64x2 a, v4s64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x2 operator-(v4s64 a, v4s64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x2 operator-(v4s64x2 a, s64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s64x2 operator-(s64 a, v4s64x2 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s64x2 operator*(v4s64x2 a, v4s64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x2 operator*(v4s64x2 a, v4s64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x2 operator*(v4s64 a, v4s64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x2 operator*(v4s64x2 a, s64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s64x2 operator*(s64 a, v4s64x2 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s64x2 operator/(v4s64x2 a, v4s64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x2 operator/(v4s64x2 a, v4s64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x2 operator/(v4s64 a, v4s64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x2 operator/(v4s64x2 a, s64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s64x2 operator/(s64 a, v4s64x2 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s64x2 operator^(v4s64x2 a, v4s64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x2 operator^(v4s64x2 a, v4s64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x2 operator^(v4s64 a, v4s64x2 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x2 operator^(v4s64x2 a, s64 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s64x2 operator^(s64 a, v4s64x2 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s64x2 operator&(v4s64x2 a, v4s64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x2 operator&(v4s64x2 a, v4s64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x2 operator&(v4s64 a, v4s64x2 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x2 operator&(v4s64x2 a, s64 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s64x2 operator&(s64 a, v4s64x2 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s64x2 operator|(v4s64x2 a, v4s64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x2 operator|(v4s64x2 a, v4s64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x2 operator|(v4s64 a, v4s64x2 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x2 operator|(v4s64x2 a, s64 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s64x2 operator|(s64 a, v4s64x2 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s64x2 operator<<(v4s64x2 a, s64 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s64x2 operator>>(v4s64x2 a, s64 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u64x2() const;
	forceinline operator v4f64x2() const;
};

forceinline v4s64x2 V4s64x2(v4s64 a) { return {simd_typed::S64x2(a.x), simd_typed::S64x2(a.y), simd_typed::S64x2(a.z), simd_typed::S64x2(a.w), }; }
forceinline simd_typed::s64x2 dot(v4s64x2 a, v4s64x2 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4f32x4 {
	struct { simd_typed::f32x4 x, y, z, w; };
	simd_typed::f32x4 d[4];
	forceinline v4f32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4f32x4 operator+(v4f32x4 a, v4f32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x4 operator+(v4f32x4 a, v4f32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x4 operator+(v4f32 a, v4f32x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x4 operator+(v4f32x4 a, f32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4f32x4 operator+(f32 a, v4f32x4 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4f32x4 operator-(v4f32x4 a, v4f32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x4 operator-(v4f32x4 a, v4f32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x4 operator-(v4f32 a, v4f32x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x4 operator-(v4f32x4 a, f32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4f32x4 operator-(f32 a, v4f32x4 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4f32x4 operator*(v4f32x4 a, v4f32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x4 operator*(v4f32x4 a, v4f32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x4 operator*(v4f32 a, v4f32x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x4 operator*(v4f32x4 a, f32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4f32x4 operator*(f32 a, v4f32x4 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4f32x4 operator/(v4f32x4 a, v4f32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x4 operator/(v4f32x4 a, v4f32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x4 operator/(v4f32 a, v4f32x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x4 operator/(v4f32x4 a, f32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4f32x4 operator/(f32 a, v4f32x4 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline operator v4u32x4() const;
	forceinline operator v4s32x4() const;
};

forceinline v4f32x4 V4f32x4(v4f32 a) { return {simd_typed::F32x4(a.x), simd_typed::F32x4(a.y), simd_typed::F32x4(a.z), simd_typed::F32x4(a.w), }; }
forceinline simd_typed::f32x4 dot(v4f32x4 a, v4f32x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4f64x2 {
	struct { simd_typed::f64x2 x, y, z, w; };
	simd_typed::f64x2 d[4];
	forceinline v4f64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4f64x2 operator+(v4f64x2 a, v4f64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x2 operator+(v4f64x2 a, v4f64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x2 operator+(v4f64 a, v4f64x2 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x2 operator+(v4f64x2 a, f64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4f64x2 operator+(f64 a, v4f64x2 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4f64x2 operator-(v4f64x2 a, v4f64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x2 operator-(v4f64x2 a, v4f64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x2 operator-(v4f64 a, v4f64x2 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x2 operator-(v4f64x2 a, f64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4f64x2 operator-(f64 a, v4f64x2 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4f64x2 operator*(v4f64x2 a, v4f64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x2 operator*(v4f64x2 a, v4f64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x2 operator*(v4f64 a, v4f64x2 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x2 operator*(v4f64x2 a, f64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4f64x2 operator*(f64 a, v4f64x2 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4f64x2 operator/(v4f64x2 a, v4f64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x2 operator/(v4f64x2 a, v4f64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x2 operator/(v4f64 a, v4f64x2 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x2 operator/(v4f64x2 a, f64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4f64x2 operator/(f64 a, v4f64x2 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline operator v4u64x2() const;
	forceinline operator v4s64x2() const;
};

forceinline v4f64x2 V4f64x2(v4f64 a) { return {simd_typed::F64x2(a.x), simd_typed::F64x2(a.y), simd_typed::F64x2(a.z), simd_typed::F64x2(a.w), }; }
forceinline simd_typed::f64x2 dot(v4f64x2 a, v4f64x2 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

forceinline v4u8x16::operator v4s8x16() const { return {(simd_typed::s8x16)x, (simd_typed::s8x16)y, (simd_typed::s8x16)z, (simd_typed::s8x16)w, }; }
forceinline v4u16x8::operator v4s16x8() const { return {(simd_typed::s16x8)x, (simd_typed::s16x8)y, (simd_typed::s16x8)z, (simd_typed::s16x8)w, }; }
forceinline v4u32x4::operator v4s32x4() const { return {(simd_typed::s32x4)x, (simd_typed::s32x4)y, (simd_typed::s32x4)z, (simd_typed::s32x4)w, }; }
forceinline v4u32x4::operator v4f32x4() const { return {(simd_typed::f32x4)x, (simd_typed::f32x4)y, (simd_typed::f32x4)z, (simd_typed::f32x4)w, }; }
forceinline v4u64x2::operator v4s64x2() const { return {(simd_typed::s64x2)x, (simd_typed::s64x2)y, (simd_typed::s64x2)z, (simd_typed::s64x2)w, }; }
forceinline v4u64x2::operator v4f64x2() const { return {(simd_typed::f64x2)x, (simd_typed::f64x2)y, (simd_typed::f64x2)z, (simd_typed::f64x2)w, }; }
forceinline v4s8x16::operator v4u8x16() const { return {(simd_typed::u8x16)x, (simd_typed::u8x16)y, (simd_typed::u8x16)z, (simd_typed::u8x16)w, }; }
forceinline v4s16x8::operator v4u16x8() const { return {(simd_typed::u16x8)x, (simd_typed::u16x8)y, (simd_typed::u16x8)z, (simd_typed::u16x8)w, }; }
forceinline v4s32x4::operator v4u32x4() const { return {(simd_typed::u32x4)x, (simd_typed::u32x4)y, (simd_typed::u32x4)z, (simd_typed::u32x4)w, }; }
forceinline v4s32x4::operator v4f32x4() const { return {(simd_typed::f32x4)x, (simd_typed::f32x4)y, (simd_typed::f32x4)z, (simd_typed::f32x4)w, }; }
forceinline v4s64x2::operator v4u64x2() const { return {(simd_typed::u64x2)x, (simd_typed::u64x2)y, (simd_typed::u64x2)z, (simd_typed::u64x2)w, }; }
forceinline v4s64x2::operator v4f64x2() const { return {(simd_typed::f64x2)x, (simd_typed::f64x2)y, (simd_typed::f64x2)z, (simd_typed::f64x2)w, }; }
forceinline v4f32x4::operator v4u32x4() const { return {(simd_typed::u32x4)x, (simd_typed::u32x4)y, (simd_typed::u32x4)z, (simd_typed::u32x4)w, }; }
forceinline v4f32x4::operator v4s32x4() const { return {(simd_typed::s32x4)x, (simd_typed::s32x4)y, (simd_typed::s32x4)z, (simd_typed::s32x4)w, }; }
forceinline v4f64x2::operator v4u64x2() const { return {(simd_typed::u64x2)x, (simd_typed::u64x2)y, (simd_typed::u64x2)z, (simd_typed::u64x2)w, }; }
forceinline v4f64x2::operator v4s64x2() const { return {(simd_typed::s64x2)x, (simd_typed::s64x2)y, (simd_typed::s64x2)z, (simd_typed::s64x2)w, }; }

#if ARCH_AVX
union v2u8x32 {
	struct { simd_typed::u8x32 x, y; };
	simd_typed::u8x32 d[2];
	forceinline v2u8 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u8x32 operator+(v2u8x32 a, v2u8x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x32 operator+(v2u8x32 a, v2u8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x32 operator+(v2u8 a, v2u8x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x32 operator+(v2u8x32 a, u8 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u8x32 operator+(u8 a, v2u8x32 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u8x32 operator-(v2u8x32 a, v2u8x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x32 operator-(v2u8x32 a, v2u8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x32 operator-(v2u8 a, v2u8x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x32 operator-(v2u8x32 a, u8 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u8x32 operator-(u8 a, v2u8x32 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u8x32 operator*(v2u8x32 a, v2u8x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x32 operator*(v2u8x32 a, v2u8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x32 operator*(v2u8 a, v2u8x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x32 operator*(v2u8x32 a, u8 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u8x32 operator*(u8 a, v2u8x32 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u8x32 operator/(v2u8x32 a, v2u8x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x32 operator/(v2u8x32 a, v2u8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x32 operator/(v2u8 a, v2u8x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x32 operator/(v2u8x32 a, u8 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u8x32 operator/(u8 a, v2u8x32 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u8x32 operator^(v2u8x32 a, v2u8x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x32 operator^(v2u8x32 a, v2u8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x32 operator^(v2u8 a, v2u8x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x32 operator^(v2u8x32 a, u8 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u8x32 operator^(u8 a, v2u8x32 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u8x32 operator&(v2u8x32 a, v2u8x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x32 operator&(v2u8x32 a, v2u8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x32 operator&(v2u8 a, v2u8x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x32 operator&(v2u8x32 a, u8 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u8x32 operator&(u8 a, v2u8x32 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u8x32 operator|(v2u8x32 a, v2u8x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x32 operator|(v2u8x32 a, v2u8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x32 operator|(v2u8 a, v2u8x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x32 operator|(v2u8x32 a, u8 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u8x32 operator|(u8 a, v2u8x32 b) { return {a|b.x, a|b.y};}
	forceinline operator v2s8x32() const;
};

forceinline v2u8x32 V2u8x32(v2u8 a) { return {simd_typed::U8x32(a.x), simd_typed::U8x32(a.y), }; }
forceinline simd_typed::u8x32 dot(v2u8x32 a, v2u8x32 b) { return a.x*b.x+a.y*b.y; }

union v2u16x16 {
	struct { simd_typed::u16x16 x, y; };
	simd_typed::u16x16 d[2];
	forceinline v2u16 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u16x16 operator+(v2u16x16 a, v2u16x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x16 operator+(v2u16x16 a, v2u16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x16 operator+(v2u16 a, v2u16x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x16 operator+(v2u16x16 a, u16 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u16x16 operator+(u16 a, v2u16x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u16x16 operator-(v2u16x16 a, v2u16x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x16 operator-(v2u16x16 a, v2u16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x16 operator-(v2u16 a, v2u16x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x16 operator-(v2u16x16 a, u16 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u16x16 operator-(u16 a, v2u16x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u16x16 operator*(v2u16x16 a, v2u16x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x16 operator*(v2u16x16 a, v2u16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x16 operator*(v2u16 a, v2u16x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x16 operator*(v2u16x16 a, u16 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u16x16 operator*(u16 a, v2u16x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u16x16 operator/(v2u16x16 a, v2u16x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x16 operator/(v2u16x16 a, v2u16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x16 operator/(v2u16 a, v2u16x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x16 operator/(v2u16x16 a, u16 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u16x16 operator/(u16 a, v2u16x16 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u16x16 operator^(v2u16x16 a, v2u16x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x16 operator^(v2u16x16 a, v2u16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x16 operator^(v2u16 a, v2u16x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x16 operator^(v2u16x16 a, u16 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u16x16 operator^(u16 a, v2u16x16 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u16x16 operator&(v2u16x16 a, v2u16x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x16 operator&(v2u16x16 a, v2u16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x16 operator&(v2u16 a, v2u16x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x16 operator&(v2u16x16 a, u16 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u16x16 operator&(u16 a, v2u16x16 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u16x16 operator|(v2u16x16 a, v2u16x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x16 operator|(v2u16x16 a, v2u16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x16 operator|(v2u16 a, v2u16x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x16 operator|(v2u16x16 a, u16 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u16x16 operator|(u16 a, v2u16x16 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u16x16 operator<<(v2u16x16 a, u16 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u16x16 operator>>(v2u16x16 a, u16 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s16x16() const;
};

forceinline v2u16x16 V2u16x16(v2u16 a) { return {simd_typed::U16x16(a.x), simd_typed::U16x16(a.y), }; }
forceinline simd_typed::u16x16 dot(v2u16x16 a, v2u16x16 b) { return a.x*b.x+a.y*b.y; }

union v2u32x8 {
	struct { simd_typed::u32x8 x, y; };
	simd_typed::u32x8 d[2];
	forceinline v2u32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u32x8 operator+(v2u32x8 a, v2u32x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x8 operator+(v2u32x8 a, v2u32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x8 operator+(v2u32 a, v2u32x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x8 operator+(v2u32x8 a, u32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u32x8 operator+(u32 a, v2u32x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u32x8 operator-(v2u32x8 a, v2u32x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x8 operator-(v2u32x8 a, v2u32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x8 operator-(v2u32 a, v2u32x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x8 operator-(v2u32x8 a, u32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u32x8 operator-(u32 a, v2u32x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u32x8 operator*(v2u32x8 a, v2u32x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x8 operator*(v2u32x8 a, v2u32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x8 operator*(v2u32 a, v2u32x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x8 operator*(v2u32x8 a, u32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u32x8 operator*(u32 a, v2u32x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u32x8 operator/(v2u32x8 a, v2u32x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x8 operator/(v2u32x8 a, v2u32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x8 operator/(v2u32 a, v2u32x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x8 operator/(v2u32x8 a, u32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u32x8 operator/(u32 a, v2u32x8 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u32x8 operator^(v2u32x8 a, v2u32x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x8 operator^(v2u32x8 a, v2u32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x8 operator^(v2u32 a, v2u32x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x8 operator^(v2u32x8 a, u32 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u32x8 operator^(u32 a, v2u32x8 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u32x8 operator&(v2u32x8 a, v2u32x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x8 operator&(v2u32x8 a, v2u32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x8 operator&(v2u32 a, v2u32x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x8 operator&(v2u32x8 a, u32 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u32x8 operator&(u32 a, v2u32x8 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u32x8 operator|(v2u32x8 a, v2u32x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x8 operator|(v2u32x8 a, v2u32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x8 operator|(v2u32 a, v2u32x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x8 operator|(v2u32x8 a, u32 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u32x8 operator|(u32 a, v2u32x8 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u32x8 operator<<(v2u32x8 a, u32 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u32x8 operator>>(v2u32x8 a, u32 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s32x8() const;
	forceinline operator v2f32x8() const;
};

forceinline v2u32x8 V2u32x8(v2u32 a) { return {simd_typed::U32x8(a.x), simd_typed::U32x8(a.y), }; }
forceinline simd_typed::u32x8 dot(v2u32x8 a, v2u32x8 b) { return a.x*b.x+a.y*b.y; }

union v2u64x4 {
	struct { simd_typed::u64x4 x, y; };
	simd_typed::u64x4 d[2];
	forceinline v2u64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u64x4 operator+(v2u64x4 a, v2u64x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x4 operator+(v2u64x4 a, v2u64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x4 operator+(v2u64 a, v2u64x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x4 operator+(v2u64x4 a, u64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u64x4 operator+(u64 a, v2u64x4 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u64x4 operator-(v2u64x4 a, v2u64x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x4 operator-(v2u64x4 a, v2u64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x4 operator-(v2u64 a, v2u64x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x4 operator-(v2u64x4 a, u64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u64x4 operator-(u64 a, v2u64x4 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u64x4 operator*(v2u64x4 a, v2u64x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x4 operator*(v2u64x4 a, v2u64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x4 operator*(v2u64 a, v2u64x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x4 operator*(v2u64x4 a, u64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u64x4 operator*(u64 a, v2u64x4 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u64x4 operator/(v2u64x4 a, v2u64x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x4 operator/(v2u64x4 a, v2u64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x4 operator/(v2u64 a, v2u64x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x4 operator/(v2u64x4 a, u64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u64x4 operator/(u64 a, v2u64x4 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u64x4 operator^(v2u64x4 a, v2u64x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x4 operator^(v2u64x4 a, v2u64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x4 operator^(v2u64 a, v2u64x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x4 operator^(v2u64x4 a, u64 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u64x4 operator^(u64 a, v2u64x4 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u64x4 operator&(v2u64x4 a, v2u64x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x4 operator&(v2u64x4 a, v2u64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x4 operator&(v2u64 a, v2u64x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x4 operator&(v2u64x4 a, u64 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u64x4 operator&(u64 a, v2u64x4 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u64x4 operator|(v2u64x4 a, v2u64x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x4 operator|(v2u64x4 a, v2u64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x4 operator|(v2u64 a, v2u64x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x4 operator|(v2u64x4 a, u64 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u64x4 operator|(u64 a, v2u64x4 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u64x4 operator<<(v2u64x4 a, u64 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u64x4 operator>>(v2u64x4 a, u64 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s64x4() const;
	forceinline operator v2f64x4() const;
};

forceinline v2u64x4 V2u64x4(v2u64 a) { return {simd_typed::U64x4(a.x), simd_typed::U64x4(a.y), }; }
forceinline simd_typed::u64x4 dot(v2u64x4 a, v2u64x4 b) { return a.x*b.x+a.y*b.y; }

union v2s8x32 {
	struct { simd_typed::s8x32 x, y; };
	simd_typed::s8x32 d[2];
	forceinline v2s8 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s8x32 operator+(v2s8x32 a, v2s8x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x32 operator+(v2s8x32 a, v2s8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x32 operator+(v2s8 a, v2s8x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x32 operator+(v2s8x32 a, s8 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s8x32 operator+(s8 a, v2s8x32 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s8x32 operator-(v2s8x32 a, v2s8x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x32 operator-(v2s8x32 a, v2s8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x32 operator-(v2s8 a, v2s8x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x32 operator-(v2s8x32 a, s8 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s8x32 operator-(s8 a, v2s8x32 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s8x32 operator*(v2s8x32 a, v2s8x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x32 operator*(v2s8x32 a, v2s8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x32 operator*(v2s8 a, v2s8x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x32 operator*(v2s8x32 a, s8 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s8x32 operator*(s8 a, v2s8x32 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s8x32 operator/(v2s8x32 a, v2s8x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x32 operator/(v2s8x32 a, v2s8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x32 operator/(v2s8 a, v2s8x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x32 operator/(v2s8x32 a, s8 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s8x32 operator/(s8 a, v2s8x32 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s8x32 operator^(v2s8x32 a, v2s8x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x32 operator^(v2s8x32 a, v2s8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x32 operator^(v2s8 a, v2s8x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x32 operator^(v2s8x32 a, s8 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s8x32 operator^(s8 a, v2s8x32 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s8x32 operator&(v2s8x32 a, v2s8x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x32 operator&(v2s8x32 a, v2s8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x32 operator&(v2s8 a, v2s8x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x32 operator&(v2s8x32 a, s8 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s8x32 operator&(s8 a, v2s8x32 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s8x32 operator|(v2s8x32 a, v2s8x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x32 operator|(v2s8x32 a, v2s8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x32 operator|(v2s8 a, v2s8x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x32 operator|(v2s8x32 a, s8 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s8x32 operator|(s8 a, v2s8x32 b) { return {a|b.x, a|b.y};}
	forceinline operator v2u8x32() const;
};

forceinline v2s8x32 V2s8x32(v2s8 a) { return {simd_typed::S8x32(a.x), simd_typed::S8x32(a.y), }; }
forceinline simd_typed::s8x32 dot(v2s8x32 a, v2s8x32 b) { return a.x*b.x+a.y*b.y; }

union v2s16x16 {
	struct { simd_typed::s16x16 x, y; };
	simd_typed::s16x16 d[2];
	forceinline v2s16 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s16x16 operator+(v2s16x16 a, v2s16x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x16 operator+(v2s16x16 a, v2s16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x16 operator+(v2s16 a, v2s16x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x16 operator+(v2s16x16 a, s16 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s16x16 operator+(s16 a, v2s16x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s16x16 operator-(v2s16x16 a, v2s16x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x16 operator-(v2s16x16 a, v2s16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x16 operator-(v2s16 a, v2s16x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x16 operator-(v2s16x16 a, s16 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s16x16 operator-(s16 a, v2s16x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s16x16 operator*(v2s16x16 a, v2s16x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x16 operator*(v2s16x16 a, v2s16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x16 operator*(v2s16 a, v2s16x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x16 operator*(v2s16x16 a, s16 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s16x16 operator*(s16 a, v2s16x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s16x16 operator/(v2s16x16 a, v2s16x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x16 operator/(v2s16x16 a, v2s16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x16 operator/(v2s16 a, v2s16x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x16 operator/(v2s16x16 a, s16 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s16x16 operator/(s16 a, v2s16x16 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s16x16 operator^(v2s16x16 a, v2s16x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x16 operator^(v2s16x16 a, v2s16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x16 operator^(v2s16 a, v2s16x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x16 operator^(v2s16x16 a, s16 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s16x16 operator^(s16 a, v2s16x16 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s16x16 operator&(v2s16x16 a, v2s16x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x16 operator&(v2s16x16 a, v2s16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x16 operator&(v2s16 a, v2s16x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x16 operator&(v2s16x16 a, s16 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s16x16 operator&(s16 a, v2s16x16 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s16x16 operator|(v2s16x16 a, v2s16x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x16 operator|(v2s16x16 a, v2s16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x16 operator|(v2s16 a, v2s16x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x16 operator|(v2s16x16 a, s16 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s16x16 operator|(s16 a, v2s16x16 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s16x16 operator<<(v2s16x16 a, s16 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s16x16 operator>>(v2s16x16 a, s16 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u16x16() const;
};

forceinline v2s16x16 V2s16x16(v2s16 a) { return {simd_typed::S16x16(a.x), simd_typed::S16x16(a.y), }; }
forceinline simd_typed::s16x16 dot(v2s16x16 a, v2s16x16 b) { return a.x*b.x+a.y*b.y; }

union v2s32x8 {
	struct { simd_typed::s32x8 x, y; };
	simd_typed::s32x8 d[2];
	forceinline v2s32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s32x8 operator+(v2s32x8 a, v2s32x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x8 operator+(v2s32x8 a, v2s32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x8 operator+(v2s32 a, v2s32x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x8 operator+(v2s32x8 a, s32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s32x8 operator+(s32 a, v2s32x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s32x8 operator-(v2s32x8 a, v2s32x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x8 operator-(v2s32x8 a, v2s32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x8 operator-(v2s32 a, v2s32x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x8 operator-(v2s32x8 a, s32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s32x8 operator-(s32 a, v2s32x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s32x8 operator*(v2s32x8 a, v2s32x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x8 operator*(v2s32x8 a, v2s32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x8 operator*(v2s32 a, v2s32x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x8 operator*(v2s32x8 a, s32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s32x8 operator*(s32 a, v2s32x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s32x8 operator/(v2s32x8 a, v2s32x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x8 operator/(v2s32x8 a, v2s32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x8 operator/(v2s32 a, v2s32x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x8 operator/(v2s32x8 a, s32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s32x8 operator/(s32 a, v2s32x8 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s32x8 operator^(v2s32x8 a, v2s32x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x8 operator^(v2s32x8 a, v2s32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x8 operator^(v2s32 a, v2s32x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x8 operator^(v2s32x8 a, s32 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s32x8 operator^(s32 a, v2s32x8 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s32x8 operator&(v2s32x8 a, v2s32x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x8 operator&(v2s32x8 a, v2s32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x8 operator&(v2s32 a, v2s32x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x8 operator&(v2s32x8 a, s32 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s32x8 operator&(s32 a, v2s32x8 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s32x8 operator|(v2s32x8 a, v2s32x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x8 operator|(v2s32x8 a, v2s32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x8 operator|(v2s32 a, v2s32x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x8 operator|(v2s32x8 a, s32 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s32x8 operator|(s32 a, v2s32x8 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s32x8 operator<<(v2s32x8 a, s32 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s32x8 operator>>(v2s32x8 a, s32 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u32x8() const;
	forceinline operator v2f32x8() const;
};

forceinline v2s32x8 V2s32x8(v2s32 a) { return {simd_typed::S32x8(a.x), simd_typed::S32x8(a.y), }; }
forceinline simd_typed::s32x8 dot(v2s32x8 a, v2s32x8 b) { return a.x*b.x+a.y*b.y; }

union v2s64x4 {
	struct { simd_typed::s64x4 x, y; };
	simd_typed::s64x4 d[2];
	forceinline v2s64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s64x4 operator+(v2s64x4 a, v2s64x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x4 operator+(v2s64x4 a, v2s64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x4 operator+(v2s64 a, v2s64x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x4 operator+(v2s64x4 a, s64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s64x4 operator+(s64 a, v2s64x4 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s64x4 operator-(v2s64x4 a, v2s64x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x4 operator-(v2s64x4 a, v2s64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x4 operator-(v2s64 a, v2s64x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x4 operator-(v2s64x4 a, s64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s64x4 operator-(s64 a, v2s64x4 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s64x4 operator*(v2s64x4 a, v2s64x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x4 operator*(v2s64x4 a, v2s64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x4 operator*(v2s64 a, v2s64x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x4 operator*(v2s64x4 a, s64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s64x4 operator*(s64 a, v2s64x4 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s64x4 operator/(v2s64x4 a, v2s64x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x4 operator/(v2s64x4 a, v2s64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x4 operator/(v2s64 a, v2s64x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x4 operator/(v2s64x4 a, s64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s64x4 operator/(s64 a, v2s64x4 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s64x4 operator^(v2s64x4 a, v2s64x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x4 operator^(v2s64x4 a, v2s64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x4 operator^(v2s64 a, v2s64x4 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x4 operator^(v2s64x4 a, s64 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s64x4 operator^(s64 a, v2s64x4 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s64x4 operator&(v2s64x4 a, v2s64x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x4 operator&(v2s64x4 a, v2s64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x4 operator&(v2s64 a, v2s64x4 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x4 operator&(v2s64x4 a, s64 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s64x4 operator&(s64 a, v2s64x4 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s64x4 operator|(v2s64x4 a, v2s64x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x4 operator|(v2s64x4 a, v2s64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x4 operator|(v2s64 a, v2s64x4 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x4 operator|(v2s64x4 a, s64 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s64x4 operator|(s64 a, v2s64x4 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s64x4 operator<<(v2s64x4 a, s64 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s64x4 operator>>(v2s64x4 a, s64 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u64x4() const;
	forceinline operator v2f64x4() const;
};

forceinline v2s64x4 V2s64x4(v2s64 a) { return {simd_typed::S64x4(a.x), simd_typed::S64x4(a.y), }; }
forceinline simd_typed::s64x4 dot(v2s64x4 a, v2s64x4 b) { return a.x*b.x+a.y*b.y; }

union v2f32x8 {
	struct { simd_typed::f32x8 x, y; };
	simd_typed::f32x8 d[2];
	forceinline v2f32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2f32x8 operator+(v2f32x8 a, v2f32x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x8 operator+(v2f32x8 a, v2f32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x8 operator+(v2f32 a, v2f32x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x8 operator+(v2f32x8 a, f32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2f32x8 operator+(f32 a, v2f32x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2f32x8 operator-(v2f32x8 a, v2f32x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x8 operator-(v2f32x8 a, v2f32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x8 operator-(v2f32 a, v2f32x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x8 operator-(v2f32x8 a, f32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2f32x8 operator-(f32 a, v2f32x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2f32x8 operator*(v2f32x8 a, v2f32x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x8 operator*(v2f32x8 a, v2f32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x8 operator*(v2f32 a, v2f32x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x8 operator*(v2f32x8 a, f32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2f32x8 operator*(f32 a, v2f32x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2f32x8 operator/(v2f32x8 a, v2f32x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x8 operator/(v2f32x8 a, v2f32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x8 operator/(v2f32 a, v2f32x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x8 operator/(v2f32x8 a, f32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2f32x8 operator/(f32 a, v2f32x8 b) { return {a/b.x, a/b.y};}
	forceinline operator v2u32x8() const;
	forceinline operator v2s32x8() const;
};

forceinline v2f32x8 V2f32x8(v2f32 a) { return {simd_typed::F32x8(a.x), simd_typed::F32x8(a.y), }; }
forceinline simd_typed::f32x8 dot(v2f32x8 a, v2f32x8 b) { return a.x*b.x+a.y*b.y; }

union v2f64x4 {
	struct { simd_typed::f64x4 x, y; };
	simd_typed::f64x4 d[2];
	forceinline v2f64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2f64x4 operator+(v2f64x4 a, v2f64x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x4 operator+(v2f64x4 a, v2f64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x4 operator+(v2f64 a, v2f64x4 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x4 operator+(v2f64x4 a, f64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2f64x4 operator+(f64 a, v2f64x4 b) { return {a+b.x, a+b.y};}
	forceinline friend v2f64x4 operator-(v2f64x4 a, v2f64x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x4 operator-(v2f64x4 a, v2f64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x4 operator-(v2f64 a, v2f64x4 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x4 operator-(v2f64x4 a, f64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2f64x4 operator-(f64 a, v2f64x4 b) { return {a-b.x, a-b.y};}
	forceinline friend v2f64x4 operator*(v2f64x4 a, v2f64x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x4 operator*(v2f64x4 a, v2f64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x4 operator*(v2f64 a, v2f64x4 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x4 operator*(v2f64x4 a, f64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2f64x4 operator*(f64 a, v2f64x4 b) { return {a*b.x, a*b.y};}
	forceinline friend v2f64x4 operator/(v2f64x4 a, v2f64x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x4 operator/(v2f64x4 a, v2f64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x4 operator/(v2f64 a, v2f64x4 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x4 operator/(v2f64x4 a, f64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2f64x4 operator/(f64 a, v2f64x4 b) { return {a/b.x, a/b.y};}
	forceinline operator v2u64x4() const;
	forceinline operator v2s64x4() const;
};

forceinline v2f64x4 V2f64x4(v2f64 a) { return {simd_typed::F64x4(a.x), simd_typed::F64x4(a.y), }; }
forceinline simd_typed::f64x4 dot(v2f64x4 a, v2f64x4 b) { return a.x*b.x+a.y*b.y; }

forceinline v2u8x32::operator v2s8x32() const { return {(simd_typed::s8x32)x, (simd_typed::s8x32)y, }; }
forceinline v2u16x16::operator v2s16x16() const { return {(simd_typed::s16x16)x, (simd_typed::s16x16)y, }; }
forceinline v2u32x8::operator v2s32x8() const { return {(simd_typed::s32x8)x, (simd_typed::s32x8)y, }; }
forceinline v2u32x8::operator v2f32x8() const { return {(simd_typed::f32x8)x, (simd_typed::f32x8)y, }; }
forceinline v2u64x4::operator v2s64x4() const { return {(simd_typed::s64x4)x, (simd_typed::s64x4)y, }; }
forceinline v2u64x4::operator v2f64x4() const { return {(simd_typed::f64x4)x, (simd_typed::f64x4)y, }; }
forceinline v2s8x32::operator v2u8x32() const { return {(simd_typed::u8x32)x, (simd_typed::u8x32)y, }; }
forceinline v2s16x16::operator v2u16x16() const { return {(simd_typed::u16x16)x, (simd_typed::u16x16)y, }; }
forceinline v2s32x8::operator v2u32x8() const { return {(simd_typed::u32x8)x, (simd_typed::u32x8)y, }; }
forceinline v2s32x8::operator v2f32x8() const { return {(simd_typed::f32x8)x, (simd_typed::f32x8)y, }; }
forceinline v2s64x4::operator v2u64x4() const { return {(simd_typed::u64x4)x, (simd_typed::u64x4)y, }; }
forceinline v2s64x4::operator v2f64x4() const { return {(simd_typed::f64x4)x, (simd_typed::f64x4)y, }; }
forceinline v2f32x8::operator v2u32x8() const { return {(simd_typed::u32x8)x, (simd_typed::u32x8)y, }; }
forceinline v2f32x8::operator v2s32x8() const { return {(simd_typed::s32x8)x, (simd_typed::s32x8)y, }; }
forceinline v2f64x4::operator v2u64x4() const { return {(simd_typed::u64x4)x, (simd_typed::u64x4)y, }; }
forceinline v2f64x4::operator v2s64x4() const { return {(simd_typed::s64x4)x, (simd_typed::s64x4)y, }; }
union v3u8x32 {
	struct { simd_typed::u8x32 x, y, z; };
	simd_typed::u8x32 d[3];
	forceinline v3u8 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u8x32 operator+(v3u8x32 a, v3u8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x32 operator+(v3u8x32 a, v3u8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x32 operator+(v3u8 a, v3u8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x32 operator+(v3u8x32 a, u8 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u8x32 operator+(u8 a, v3u8x32 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u8x32 operator-(v3u8x32 a, v3u8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x32 operator-(v3u8x32 a, v3u8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x32 operator-(v3u8 a, v3u8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x32 operator-(v3u8x32 a, u8 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u8x32 operator-(u8 a, v3u8x32 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u8x32 operator*(v3u8x32 a, v3u8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x32 operator*(v3u8x32 a, v3u8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x32 operator*(v3u8 a, v3u8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x32 operator*(v3u8x32 a, u8 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u8x32 operator*(u8 a, v3u8x32 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u8x32 operator/(v3u8x32 a, v3u8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x32 operator/(v3u8x32 a, v3u8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x32 operator/(v3u8 a, v3u8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x32 operator/(v3u8x32 a, u8 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u8x32 operator/(u8 a, v3u8x32 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u8x32 operator^(v3u8x32 a, v3u8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x32 operator^(v3u8x32 a, v3u8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x32 operator^(v3u8 a, v3u8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x32 operator^(v3u8x32 a, u8 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u8x32 operator^(u8 a, v3u8x32 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u8x32 operator&(v3u8x32 a, v3u8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x32 operator&(v3u8x32 a, v3u8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x32 operator&(v3u8 a, v3u8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x32 operator&(v3u8x32 a, u8 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u8x32 operator&(u8 a, v3u8x32 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u8x32 operator|(v3u8x32 a, v3u8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x32 operator|(v3u8x32 a, v3u8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x32 operator|(v3u8 a, v3u8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x32 operator|(v3u8x32 a, u8 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u8x32 operator|(u8 a, v3u8x32 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline operator v3s8x32() const;
};

forceinline v3u8x32 V3u8x32(v3u8 a) { return {simd_typed::U8x32(a.x), simd_typed::U8x32(a.y), simd_typed::U8x32(a.z), }; }
forceinline simd_typed::u8x32 dot(v3u8x32 a, v3u8x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u16x16 {
	struct { simd_typed::u16x16 x, y, z; };
	simd_typed::u16x16 d[3];
	forceinline v3u16 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u16x16 operator+(v3u16x16 a, v3u16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x16 operator+(v3u16x16 a, v3u16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x16 operator+(v3u16 a, v3u16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x16 operator+(v3u16x16 a, u16 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u16x16 operator+(u16 a, v3u16x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u16x16 operator-(v3u16x16 a, v3u16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x16 operator-(v3u16x16 a, v3u16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x16 operator-(v3u16 a, v3u16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x16 operator-(v3u16x16 a, u16 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u16x16 operator-(u16 a, v3u16x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u16x16 operator*(v3u16x16 a, v3u16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x16 operator*(v3u16x16 a, v3u16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x16 operator*(v3u16 a, v3u16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x16 operator*(v3u16x16 a, u16 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u16x16 operator*(u16 a, v3u16x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u16x16 operator/(v3u16x16 a, v3u16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x16 operator/(v3u16x16 a, v3u16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x16 operator/(v3u16 a, v3u16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x16 operator/(v3u16x16 a, u16 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u16x16 operator/(u16 a, v3u16x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u16x16 operator^(v3u16x16 a, v3u16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x16 operator^(v3u16x16 a, v3u16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x16 operator^(v3u16 a, v3u16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x16 operator^(v3u16x16 a, u16 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u16x16 operator^(u16 a, v3u16x16 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u16x16 operator&(v3u16x16 a, v3u16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x16 operator&(v3u16x16 a, v3u16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x16 operator&(v3u16 a, v3u16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x16 operator&(v3u16x16 a, u16 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u16x16 operator&(u16 a, v3u16x16 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u16x16 operator|(v3u16x16 a, v3u16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x16 operator|(v3u16x16 a, v3u16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x16 operator|(v3u16 a, v3u16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x16 operator|(v3u16x16 a, u16 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u16x16 operator|(u16 a, v3u16x16 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u16x16 operator<<(v3u16x16 a, u16 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u16x16 operator>>(v3u16x16 a, u16 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s16x16() const;
};

forceinline v3u16x16 V3u16x16(v3u16 a) { return {simd_typed::U16x16(a.x), simd_typed::U16x16(a.y), simd_typed::U16x16(a.z), }; }
forceinline simd_typed::u16x16 dot(v3u16x16 a, v3u16x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u32x8 {
	struct { simd_typed::u32x8 x, y, z; };
	simd_typed::u32x8 d[3];
	forceinline v3u32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u32x8 operator+(v3u32x8 a, v3u32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x8 operator+(v3u32x8 a, v3u32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x8 operator+(v3u32 a, v3u32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x8 operator+(v3u32x8 a, u32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u32x8 operator+(u32 a, v3u32x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u32x8 operator-(v3u32x8 a, v3u32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x8 operator-(v3u32x8 a, v3u32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x8 operator-(v3u32 a, v3u32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x8 operator-(v3u32x8 a, u32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u32x8 operator-(u32 a, v3u32x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u32x8 operator*(v3u32x8 a, v3u32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x8 operator*(v3u32x8 a, v3u32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x8 operator*(v3u32 a, v3u32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x8 operator*(v3u32x8 a, u32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u32x8 operator*(u32 a, v3u32x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u32x8 operator/(v3u32x8 a, v3u32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x8 operator/(v3u32x8 a, v3u32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x8 operator/(v3u32 a, v3u32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x8 operator/(v3u32x8 a, u32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u32x8 operator/(u32 a, v3u32x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u32x8 operator^(v3u32x8 a, v3u32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x8 operator^(v3u32x8 a, v3u32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x8 operator^(v3u32 a, v3u32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x8 operator^(v3u32x8 a, u32 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u32x8 operator^(u32 a, v3u32x8 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u32x8 operator&(v3u32x8 a, v3u32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x8 operator&(v3u32x8 a, v3u32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x8 operator&(v3u32 a, v3u32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x8 operator&(v3u32x8 a, u32 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u32x8 operator&(u32 a, v3u32x8 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u32x8 operator|(v3u32x8 a, v3u32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x8 operator|(v3u32x8 a, v3u32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x8 operator|(v3u32 a, v3u32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x8 operator|(v3u32x8 a, u32 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u32x8 operator|(u32 a, v3u32x8 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u32x8 operator<<(v3u32x8 a, u32 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u32x8 operator>>(v3u32x8 a, u32 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s32x8() const;
	forceinline operator v3f32x8() const;
};

forceinline v3u32x8 V3u32x8(v3u32 a) { return {simd_typed::U32x8(a.x), simd_typed::U32x8(a.y), simd_typed::U32x8(a.z), }; }
forceinline simd_typed::u32x8 dot(v3u32x8 a, v3u32x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u64x4 {
	struct { simd_typed::u64x4 x, y, z; };
	simd_typed::u64x4 d[3];
	forceinline v3u64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u64x4 operator+(v3u64x4 a, v3u64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x4 operator+(v3u64x4 a, v3u64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x4 operator+(v3u64 a, v3u64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x4 operator+(v3u64x4 a, u64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u64x4 operator+(u64 a, v3u64x4 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u64x4 operator-(v3u64x4 a, v3u64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x4 operator-(v3u64x4 a, v3u64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x4 operator-(v3u64 a, v3u64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x4 operator-(v3u64x4 a, u64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u64x4 operator-(u64 a, v3u64x4 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u64x4 operator*(v3u64x4 a, v3u64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x4 operator*(v3u64x4 a, v3u64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x4 operator*(v3u64 a, v3u64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x4 operator*(v3u64x4 a, u64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u64x4 operator*(u64 a, v3u64x4 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u64x4 operator/(v3u64x4 a, v3u64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x4 operator/(v3u64x4 a, v3u64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x4 operator/(v3u64 a, v3u64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x4 operator/(v3u64x4 a, u64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u64x4 operator/(u64 a, v3u64x4 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u64x4 operator^(v3u64x4 a, v3u64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x4 operator^(v3u64x4 a, v3u64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x4 operator^(v3u64 a, v3u64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x4 operator^(v3u64x4 a, u64 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u64x4 operator^(u64 a, v3u64x4 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u64x4 operator&(v3u64x4 a, v3u64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x4 operator&(v3u64x4 a, v3u64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x4 operator&(v3u64 a, v3u64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x4 operator&(v3u64x4 a, u64 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u64x4 operator&(u64 a, v3u64x4 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u64x4 operator|(v3u64x4 a, v3u64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x4 operator|(v3u64x4 a, v3u64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x4 operator|(v3u64 a, v3u64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x4 operator|(v3u64x4 a, u64 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u64x4 operator|(u64 a, v3u64x4 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u64x4 operator<<(v3u64x4 a, u64 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u64x4 operator>>(v3u64x4 a, u64 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s64x4() const;
	forceinline operator v3f64x4() const;
};

forceinline v3u64x4 V3u64x4(v3u64 a) { return {simd_typed::U64x4(a.x), simd_typed::U64x4(a.y), simd_typed::U64x4(a.z), }; }
forceinline simd_typed::u64x4 dot(v3u64x4 a, v3u64x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s8x32 {
	struct { simd_typed::s8x32 x, y, z; };
	simd_typed::s8x32 d[3];
	forceinline v3s8 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s8x32 operator+(v3s8x32 a, v3s8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x32 operator+(v3s8x32 a, v3s8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x32 operator+(v3s8 a, v3s8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x32 operator+(v3s8x32 a, s8 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s8x32 operator+(s8 a, v3s8x32 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s8x32 operator-(v3s8x32 a, v3s8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x32 operator-(v3s8x32 a, v3s8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x32 operator-(v3s8 a, v3s8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x32 operator-(v3s8x32 a, s8 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s8x32 operator-(s8 a, v3s8x32 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s8x32 operator*(v3s8x32 a, v3s8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x32 operator*(v3s8x32 a, v3s8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x32 operator*(v3s8 a, v3s8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x32 operator*(v3s8x32 a, s8 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s8x32 operator*(s8 a, v3s8x32 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s8x32 operator/(v3s8x32 a, v3s8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x32 operator/(v3s8x32 a, v3s8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x32 operator/(v3s8 a, v3s8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x32 operator/(v3s8x32 a, s8 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s8x32 operator/(s8 a, v3s8x32 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s8x32 operator^(v3s8x32 a, v3s8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x32 operator^(v3s8x32 a, v3s8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x32 operator^(v3s8 a, v3s8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x32 operator^(v3s8x32 a, s8 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s8x32 operator^(s8 a, v3s8x32 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s8x32 operator&(v3s8x32 a, v3s8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x32 operator&(v3s8x32 a, v3s8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x32 operator&(v3s8 a, v3s8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x32 operator&(v3s8x32 a, s8 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s8x32 operator&(s8 a, v3s8x32 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s8x32 operator|(v3s8x32 a, v3s8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x32 operator|(v3s8x32 a, v3s8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x32 operator|(v3s8 a, v3s8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x32 operator|(v3s8x32 a, s8 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s8x32 operator|(s8 a, v3s8x32 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline operator v3u8x32() const;
};

forceinline v3s8x32 V3s8x32(v3s8 a) { return {simd_typed::S8x32(a.x), simd_typed::S8x32(a.y), simd_typed::S8x32(a.z), }; }
forceinline simd_typed::s8x32 dot(v3s8x32 a, v3s8x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s16x16 {
	struct { simd_typed::s16x16 x, y, z; };
	simd_typed::s16x16 d[3];
	forceinline v3s16 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s16x16 operator+(v3s16x16 a, v3s16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x16 operator+(v3s16x16 a, v3s16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x16 operator+(v3s16 a, v3s16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x16 operator+(v3s16x16 a, s16 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s16x16 operator+(s16 a, v3s16x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s16x16 operator-(v3s16x16 a, v3s16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x16 operator-(v3s16x16 a, v3s16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x16 operator-(v3s16 a, v3s16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x16 operator-(v3s16x16 a, s16 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s16x16 operator-(s16 a, v3s16x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s16x16 operator*(v3s16x16 a, v3s16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x16 operator*(v3s16x16 a, v3s16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x16 operator*(v3s16 a, v3s16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x16 operator*(v3s16x16 a, s16 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s16x16 operator*(s16 a, v3s16x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s16x16 operator/(v3s16x16 a, v3s16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x16 operator/(v3s16x16 a, v3s16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x16 operator/(v3s16 a, v3s16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x16 operator/(v3s16x16 a, s16 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s16x16 operator/(s16 a, v3s16x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s16x16 operator^(v3s16x16 a, v3s16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x16 operator^(v3s16x16 a, v3s16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x16 operator^(v3s16 a, v3s16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x16 operator^(v3s16x16 a, s16 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s16x16 operator^(s16 a, v3s16x16 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s16x16 operator&(v3s16x16 a, v3s16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x16 operator&(v3s16x16 a, v3s16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x16 operator&(v3s16 a, v3s16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x16 operator&(v3s16x16 a, s16 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s16x16 operator&(s16 a, v3s16x16 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s16x16 operator|(v3s16x16 a, v3s16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x16 operator|(v3s16x16 a, v3s16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x16 operator|(v3s16 a, v3s16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x16 operator|(v3s16x16 a, s16 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s16x16 operator|(s16 a, v3s16x16 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s16x16 operator<<(v3s16x16 a, s16 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s16x16 operator>>(v3s16x16 a, s16 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u16x16() const;
};

forceinline v3s16x16 V3s16x16(v3s16 a) { return {simd_typed::S16x16(a.x), simd_typed::S16x16(a.y), simd_typed::S16x16(a.z), }; }
forceinline simd_typed::s16x16 dot(v3s16x16 a, v3s16x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s32x8 {
	struct { simd_typed::s32x8 x, y, z; };
	simd_typed::s32x8 d[3];
	forceinline v3s32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s32x8 operator+(v3s32x8 a, v3s32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x8 operator+(v3s32x8 a, v3s32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x8 operator+(v3s32 a, v3s32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x8 operator+(v3s32x8 a, s32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s32x8 operator+(s32 a, v3s32x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s32x8 operator-(v3s32x8 a, v3s32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x8 operator-(v3s32x8 a, v3s32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x8 operator-(v3s32 a, v3s32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x8 operator-(v3s32x8 a, s32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s32x8 operator-(s32 a, v3s32x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s32x8 operator*(v3s32x8 a, v3s32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x8 operator*(v3s32x8 a, v3s32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x8 operator*(v3s32 a, v3s32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x8 operator*(v3s32x8 a, s32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s32x8 operator*(s32 a, v3s32x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s32x8 operator/(v3s32x8 a, v3s32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x8 operator/(v3s32x8 a, v3s32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x8 operator/(v3s32 a, v3s32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x8 operator/(v3s32x8 a, s32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s32x8 operator/(s32 a, v3s32x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s32x8 operator^(v3s32x8 a, v3s32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x8 operator^(v3s32x8 a, v3s32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x8 operator^(v3s32 a, v3s32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x8 operator^(v3s32x8 a, s32 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s32x8 operator^(s32 a, v3s32x8 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s32x8 operator&(v3s32x8 a, v3s32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x8 operator&(v3s32x8 a, v3s32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x8 operator&(v3s32 a, v3s32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x8 operator&(v3s32x8 a, s32 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s32x8 operator&(s32 a, v3s32x8 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s32x8 operator|(v3s32x8 a, v3s32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x8 operator|(v3s32x8 a, v3s32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x8 operator|(v3s32 a, v3s32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x8 operator|(v3s32x8 a, s32 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s32x8 operator|(s32 a, v3s32x8 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s32x8 operator<<(v3s32x8 a, s32 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s32x8 operator>>(v3s32x8 a, s32 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u32x8() const;
	forceinline operator v3f32x8() const;
};

forceinline v3s32x8 V3s32x8(v3s32 a) { return {simd_typed::S32x8(a.x), simd_typed::S32x8(a.y), simd_typed::S32x8(a.z), }; }
forceinline simd_typed::s32x8 dot(v3s32x8 a, v3s32x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s64x4 {
	struct { simd_typed::s64x4 x, y, z; };
	simd_typed::s64x4 d[3];
	forceinline v3s64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s64x4 operator+(v3s64x4 a, v3s64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x4 operator+(v3s64x4 a, v3s64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x4 operator+(v3s64 a, v3s64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x4 operator+(v3s64x4 a, s64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s64x4 operator+(s64 a, v3s64x4 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s64x4 operator-(v3s64x4 a, v3s64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x4 operator-(v3s64x4 a, v3s64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x4 operator-(v3s64 a, v3s64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x4 operator-(v3s64x4 a, s64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s64x4 operator-(s64 a, v3s64x4 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s64x4 operator*(v3s64x4 a, v3s64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x4 operator*(v3s64x4 a, v3s64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x4 operator*(v3s64 a, v3s64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x4 operator*(v3s64x4 a, s64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s64x4 operator*(s64 a, v3s64x4 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s64x4 operator/(v3s64x4 a, v3s64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x4 operator/(v3s64x4 a, v3s64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x4 operator/(v3s64 a, v3s64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x4 operator/(v3s64x4 a, s64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s64x4 operator/(s64 a, v3s64x4 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s64x4 operator^(v3s64x4 a, v3s64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x4 operator^(v3s64x4 a, v3s64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x4 operator^(v3s64 a, v3s64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x4 operator^(v3s64x4 a, s64 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s64x4 operator^(s64 a, v3s64x4 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s64x4 operator&(v3s64x4 a, v3s64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x4 operator&(v3s64x4 a, v3s64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x4 operator&(v3s64 a, v3s64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x4 operator&(v3s64x4 a, s64 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s64x4 operator&(s64 a, v3s64x4 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s64x4 operator|(v3s64x4 a, v3s64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x4 operator|(v3s64x4 a, v3s64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x4 operator|(v3s64 a, v3s64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x4 operator|(v3s64x4 a, s64 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s64x4 operator|(s64 a, v3s64x4 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s64x4 operator<<(v3s64x4 a, s64 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s64x4 operator>>(v3s64x4 a, s64 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u64x4() const;
	forceinline operator v3f64x4() const;
};

forceinline v3s64x4 V3s64x4(v3s64 a) { return {simd_typed::S64x4(a.x), simd_typed::S64x4(a.y), simd_typed::S64x4(a.z), }; }
forceinline simd_typed::s64x4 dot(v3s64x4 a, v3s64x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3f32x8 {
	struct { simd_typed::f32x8 x, y, z; };
	simd_typed::f32x8 d[3];
	forceinline v3f32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3f32x8 operator+(v3f32x8 a, v3f32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x8 operator+(v3f32x8 a, v3f32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x8 operator+(v3f32 a, v3f32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x8 operator+(v3f32x8 a, f32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3f32x8 operator+(f32 a, v3f32x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3f32x8 operator-(v3f32x8 a, v3f32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x8 operator-(v3f32x8 a, v3f32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x8 operator-(v3f32 a, v3f32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x8 operator-(v3f32x8 a, f32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3f32x8 operator-(f32 a, v3f32x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3f32x8 operator*(v3f32x8 a, v3f32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x8 operator*(v3f32x8 a, v3f32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x8 operator*(v3f32 a, v3f32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x8 operator*(v3f32x8 a, f32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3f32x8 operator*(f32 a, v3f32x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3f32x8 operator/(v3f32x8 a, v3f32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x8 operator/(v3f32x8 a, v3f32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x8 operator/(v3f32 a, v3f32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x8 operator/(v3f32x8 a, f32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3f32x8 operator/(f32 a, v3f32x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline operator v3u32x8() const;
	forceinline operator v3s32x8() const;
};

forceinline v3f32x8 V3f32x8(v3f32 a) { return {simd_typed::F32x8(a.x), simd_typed::F32x8(a.y), simd_typed::F32x8(a.z), }; }
forceinline simd_typed::f32x8 dot(v3f32x8 a, v3f32x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3f64x4 {
	struct { simd_typed::f64x4 x, y, z; };
	simd_typed::f64x4 d[3];
	forceinline v3f64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3f64x4 operator+(v3f64x4 a, v3f64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x4 operator+(v3f64x4 a, v3f64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x4 operator+(v3f64 a, v3f64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x4 operator+(v3f64x4 a, f64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3f64x4 operator+(f64 a, v3f64x4 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3f64x4 operator-(v3f64x4 a, v3f64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x4 operator-(v3f64x4 a, v3f64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x4 operator-(v3f64 a, v3f64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x4 operator-(v3f64x4 a, f64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3f64x4 operator-(f64 a, v3f64x4 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3f64x4 operator*(v3f64x4 a, v3f64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x4 operator*(v3f64x4 a, v3f64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x4 operator*(v3f64 a, v3f64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x4 operator*(v3f64x4 a, f64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3f64x4 operator*(f64 a, v3f64x4 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3f64x4 operator/(v3f64x4 a, v3f64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x4 operator/(v3f64x4 a, v3f64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x4 operator/(v3f64 a, v3f64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x4 operator/(v3f64x4 a, f64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3f64x4 operator/(f64 a, v3f64x4 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline operator v3u64x4() const;
	forceinline operator v3s64x4() const;
};

forceinline v3f64x4 V3f64x4(v3f64 a) { return {simd_typed::F64x4(a.x), simd_typed::F64x4(a.y), simd_typed::F64x4(a.z), }; }
forceinline simd_typed::f64x4 dot(v3f64x4 a, v3f64x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

forceinline v3u8x32::operator v3s8x32() const { return {(simd_typed::s8x32)x, (simd_typed::s8x32)y, (simd_typed::s8x32)z, }; }
forceinline v3u16x16::operator v3s16x16() const { return {(simd_typed::s16x16)x, (simd_typed::s16x16)y, (simd_typed::s16x16)z, }; }
forceinline v3u32x8::operator v3s32x8() const { return {(simd_typed::s32x8)x, (simd_typed::s32x8)y, (simd_typed::s32x8)z, }; }
forceinline v3u32x8::operator v3f32x8() const { return {(simd_typed::f32x8)x, (simd_typed::f32x8)y, (simd_typed::f32x8)z, }; }
forceinline v3u64x4::operator v3s64x4() const { return {(simd_typed::s64x4)x, (simd_typed::s64x4)y, (simd_typed::s64x4)z, }; }
forceinline v3u64x4::operator v3f64x4() const { return {(simd_typed::f64x4)x, (simd_typed::f64x4)y, (simd_typed::f64x4)z, }; }
forceinline v3s8x32::operator v3u8x32() const { return {(simd_typed::u8x32)x, (simd_typed::u8x32)y, (simd_typed::u8x32)z, }; }
forceinline v3s16x16::operator v3u16x16() const { return {(simd_typed::u16x16)x, (simd_typed::u16x16)y, (simd_typed::u16x16)z, }; }
forceinline v3s32x8::operator v3u32x8() const { return {(simd_typed::u32x8)x, (simd_typed::u32x8)y, (simd_typed::u32x8)z, }; }
forceinline v3s32x8::operator v3f32x8() const { return {(simd_typed::f32x8)x, (simd_typed::f32x8)y, (simd_typed::f32x8)z, }; }
forceinline v3s64x4::operator v3u64x4() const { return {(simd_typed::u64x4)x, (simd_typed::u64x4)y, (simd_typed::u64x4)z, }; }
forceinline v3s64x4::operator v3f64x4() const { return {(simd_typed::f64x4)x, (simd_typed::f64x4)y, (simd_typed::f64x4)z, }; }
forceinline v3f32x8::operator v3u32x8() const { return {(simd_typed::u32x8)x, (simd_typed::u32x8)y, (simd_typed::u32x8)z, }; }
forceinline v3f32x8::operator v3s32x8() const { return {(simd_typed::s32x8)x, (simd_typed::s32x8)y, (simd_typed::s32x8)z, }; }
forceinline v3f64x4::operator v3u64x4() const { return {(simd_typed::u64x4)x, (simd_typed::u64x4)y, (simd_typed::u64x4)z, }; }
forceinline v3f64x4::operator v3s64x4() const { return {(simd_typed::s64x4)x, (simd_typed::s64x4)y, (simd_typed::s64x4)z, }; }
union v4u8x32 {
	struct { simd_typed::u8x32 x, y, z, w; };
	simd_typed::u8x32 d[4];
	forceinline v4u8 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u8x32 operator+(v4u8x32 a, v4u8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x32 operator+(v4u8x32 a, v4u8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x32 operator+(v4u8 a, v4u8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x32 operator+(v4u8x32 a, u8 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u8x32 operator+(u8 a, v4u8x32 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u8x32 operator-(v4u8x32 a, v4u8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x32 operator-(v4u8x32 a, v4u8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x32 operator-(v4u8 a, v4u8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x32 operator-(v4u8x32 a, u8 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u8x32 operator-(u8 a, v4u8x32 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u8x32 operator*(v4u8x32 a, v4u8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x32 operator*(v4u8x32 a, v4u8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x32 operator*(v4u8 a, v4u8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x32 operator*(v4u8x32 a, u8 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u8x32 operator*(u8 a, v4u8x32 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u8x32 operator/(v4u8x32 a, v4u8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x32 operator/(v4u8x32 a, v4u8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x32 operator/(v4u8 a, v4u8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x32 operator/(v4u8x32 a, u8 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u8x32 operator/(u8 a, v4u8x32 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u8x32 operator^(v4u8x32 a, v4u8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x32 operator^(v4u8x32 a, v4u8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x32 operator^(v4u8 a, v4u8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x32 operator^(v4u8x32 a, u8 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u8x32 operator^(u8 a, v4u8x32 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u8x32 operator&(v4u8x32 a, v4u8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x32 operator&(v4u8x32 a, v4u8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x32 operator&(v4u8 a, v4u8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x32 operator&(v4u8x32 a, u8 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u8x32 operator&(u8 a, v4u8x32 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u8x32 operator|(v4u8x32 a, v4u8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x32 operator|(v4u8x32 a, v4u8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x32 operator|(v4u8 a, v4u8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x32 operator|(v4u8x32 a, u8 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u8x32 operator|(u8 a, v4u8x32 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline operator v4s8x32() const;
};

forceinline v4u8x32 V4u8x32(v4u8 a) { return {simd_typed::U8x32(a.x), simd_typed::U8x32(a.y), simd_typed::U8x32(a.z), simd_typed::U8x32(a.w), }; }
forceinline simd_typed::u8x32 dot(v4u8x32 a, v4u8x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u16x16 {
	struct { simd_typed::u16x16 x, y, z, w; };
	simd_typed::u16x16 d[4];
	forceinline v4u16 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u16x16 operator+(v4u16x16 a, v4u16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x16 operator+(v4u16x16 a, v4u16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x16 operator+(v4u16 a, v4u16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x16 operator+(v4u16x16 a, u16 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u16x16 operator+(u16 a, v4u16x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u16x16 operator-(v4u16x16 a, v4u16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x16 operator-(v4u16x16 a, v4u16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x16 operator-(v4u16 a, v4u16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x16 operator-(v4u16x16 a, u16 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u16x16 operator-(u16 a, v4u16x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u16x16 operator*(v4u16x16 a, v4u16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x16 operator*(v4u16x16 a, v4u16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x16 operator*(v4u16 a, v4u16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x16 operator*(v4u16x16 a, u16 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u16x16 operator*(u16 a, v4u16x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u16x16 operator/(v4u16x16 a, v4u16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x16 operator/(v4u16x16 a, v4u16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x16 operator/(v4u16 a, v4u16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x16 operator/(v4u16x16 a, u16 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u16x16 operator/(u16 a, v4u16x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u16x16 operator^(v4u16x16 a, v4u16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x16 operator^(v4u16x16 a, v4u16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x16 operator^(v4u16 a, v4u16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x16 operator^(v4u16x16 a, u16 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u16x16 operator^(u16 a, v4u16x16 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u16x16 operator&(v4u16x16 a, v4u16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x16 operator&(v4u16x16 a, v4u16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x16 operator&(v4u16 a, v4u16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x16 operator&(v4u16x16 a, u16 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u16x16 operator&(u16 a, v4u16x16 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u16x16 operator|(v4u16x16 a, v4u16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x16 operator|(v4u16x16 a, v4u16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x16 operator|(v4u16 a, v4u16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x16 operator|(v4u16x16 a, u16 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u16x16 operator|(u16 a, v4u16x16 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u16x16 operator<<(v4u16x16 a, u16 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u16x16 operator>>(v4u16x16 a, u16 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s16x16() const;
};

forceinline v4u16x16 V4u16x16(v4u16 a) { return {simd_typed::U16x16(a.x), simd_typed::U16x16(a.y), simd_typed::U16x16(a.z), simd_typed::U16x16(a.w), }; }
forceinline simd_typed::u16x16 dot(v4u16x16 a, v4u16x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u32x8 {
	struct { simd_typed::u32x8 x, y, z, w; };
	simd_typed::u32x8 d[4];
	forceinline v4u32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u32x8 operator+(v4u32x8 a, v4u32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x8 operator+(v4u32x8 a, v4u32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x8 operator+(v4u32 a, v4u32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x8 operator+(v4u32x8 a, u32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u32x8 operator+(u32 a, v4u32x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u32x8 operator-(v4u32x8 a, v4u32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x8 operator-(v4u32x8 a, v4u32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x8 operator-(v4u32 a, v4u32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x8 operator-(v4u32x8 a, u32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u32x8 operator-(u32 a, v4u32x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u32x8 operator*(v4u32x8 a, v4u32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x8 operator*(v4u32x8 a, v4u32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x8 operator*(v4u32 a, v4u32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x8 operator*(v4u32x8 a, u32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u32x8 operator*(u32 a, v4u32x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u32x8 operator/(v4u32x8 a, v4u32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x8 operator/(v4u32x8 a, v4u32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x8 operator/(v4u32 a, v4u32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x8 operator/(v4u32x8 a, u32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u32x8 operator/(u32 a, v4u32x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u32x8 operator^(v4u32x8 a, v4u32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x8 operator^(v4u32x8 a, v4u32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x8 operator^(v4u32 a, v4u32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x8 operator^(v4u32x8 a, u32 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u32x8 operator^(u32 a, v4u32x8 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u32x8 operator&(v4u32x8 a, v4u32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x8 operator&(v4u32x8 a, v4u32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x8 operator&(v4u32 a, v4u32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x8 operator&(v4u32x8 a, u32 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u32x8 operator&(u32 a, v4u32x8 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u32x8 operator|(v4u32x8 a, v4u32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x8 operator|(v4u32x8 a, v4u32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x8 operator|(v4u32 a, v4u32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x8 operator|(v4u32x8 a, u32 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u32x8 operator|(u32 a, v4u32x8 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u32x8 operator<<(v4u32x8 a, u32 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u32x8 operator>>(v4u32x8 a, u32 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s32x8() const;
	forceinline operator v4f32x8() const;
};

forceinline v4u32x8 V4u32x8(v4u32 a) { return {simd_typed::U32x8(a.x), simd_typed::U32x8(a.y), simd_typed::U32x8(a.z), simd_typed::U32x8(a.w), }; }
forceinline simd_typed::u32x8 dot(v4u32x8 a, v4u32x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u64x4 {
	struct { simd_typed::u64x4 x, y, z, w; };
	simd_typed::u64x4 d[4];
	forceinline v4u64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u64x4 operator+(v4u64x4 a, v4u64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x4 operator+(v4u64x4 a, v4u64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x4 operator+(v4u64 a, v4u64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x4 operator+(v4u64x4 a, u64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u64x4 operator+(u64 a, v4u64x4 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u64x4 operator-(v4u64x4 a, v4u64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x4 operator-(v4u64x4 a, v4u64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x4 operator-(v4u64 a, v4u64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x4 operator-(v4u64x4 a, u64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u64x4 operator-(u64 a, v4u64x4 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u64x4 operator*(v4u64x4 a, v4u64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x4 operator*(v4u64x4 a, v4u64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x4 operator*(v4u64 a, v4u64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x4 operator*(v4u64x4 a, u64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u64x4 operator*(u64 a, v4u64x4 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u64x4 operator/(v4u64x4 a, v4u64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x4 operator/(v4u64x4 a, v4u64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x4 operator/(v4u64 a, v4u64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x4 operator/(v4u64x4 a, u64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u64x4 operator/(u64 a, v4u64x4 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u64x4 operator^(v4u64x4 a, v4u64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x4 operator^(v4u64x4 a, v4u64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x4 operator^(v4u64 a, v4u64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x4 operator^(v4u64x4 a, u64 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u64x4 operator^(u64 a, v4u64x4 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u64x4 operator&(v4u64x4 a, v4u64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x4 operator&(v4u64x4 a, v4u64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x4 operator&(v4u64 a, v4u64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x4 operator&(v4u64x4 a, u64 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u64x4 operator&(u64 a, v4u64x4 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u64x4 operator|(v4u64x4 a, v4u64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x4 operator|(v4u64x4 a, v4u64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x4 operator|(v4u64 a, v4u64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x4 operator|(v4u64x4 a, u64 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u64x4 operator|(u64 a, v4u64x4 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u64x4 operator<<(v4u64x4 a, u64 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u64x4 operator>>(v4u64x4 a, u64 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s64x4() const;
	forceinline operator v4f64x4() const;
};

forceinline v4u64x4 V4u64x4(v4u64 a) { return {simd_typed::U64x4(a.x), simd_typed::U64x4(a.y), simd_typed::U64x4(a.z), simd_typed::U64x4(a.w), }; }
forceinline simd_typed::u64x4 dot(v4u64x4 a, v4u64x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s8x32 {
	struct { simd_typed::s8x32 x, y, z, w; };
	simd_typed::s8x32 d[4];
	forceinline v4s8 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s8x32 operator+(v4s8x32 a, v4s8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x32 operator+(v4s8x32 a, v4s8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x32 operator+(v4s8 a, v4s8x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x32 operator+(v4s8x32 a, s8 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s8x32 operator+(s8 a, v4s8x32 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s8x32 operator-(v4s8x32 a, v4s8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x32 operator-(v4s8x32 a, v4s8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x32 operator-(v4s8 a, v4s8x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x32 operator-(v4s8x32 a, s8 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s8x32 operator-(s8 a, v4s8x32 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s8x32 operator*(v4s8x32 a, v4s8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x32 operator*(v4s8x32 a, v4s8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x32 operator*(v4s8 a, v4s8x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x32 operator*(v4s8x32 a, s8 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s8x32 operator*(s8 a, v4s8x32 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s8x32 operator/(v4s8x32 a, v4s8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x32 operator/(v4s8x32 a, v4s8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x32 operator/(v4s8 a, v4s8x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x32 operator/(v4s8x32 a, s8 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s8x32 operator/(s8 a, v4s8x32 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s8x32 operator^(v4s8x32 a, v4s8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x32 operator^(v4s8x32 a, v4s8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x32 operator^(v4s8 a, v4s8x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x32 operator^(v4s8x32 a, s8 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s8x32 operator^(s8 a, v4s8x32 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s8x32 operator&(v4s8x32 a, v4s8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x32 operator&(v4s8x32 a, v4s8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x32 operator&(v4s8 a, v4s8x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x32 operator&(v4s8x32 a, s8 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s8x32 operator&(s8 a, v4s8x32 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s8x32 operator|(v4s8x32 a, v4s8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x32 operator|(v4s8x32 a, v4s8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x32 operator|(v4s8 a, v4s8x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x32 operator|(v4s8x32 a, s8 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s8x32 operator|(s8 a, v4s8x32 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline operator v4u8x32() const;
};

forceinline v4s8x32 V4s8x32(v4s8 a) { return {simd_typed::S8x32(a.x), simd_typed::S8x32(a.y), simd_typed::S8x32(a.z), simd_typed::S8x32(a.w), }; }
forceinline simd_typed::s8x32 dot(v4s8x32 a, v4s8x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s16x16 {
	struct { simd_typed::s16x16 x, y, z, w; };
	simd_typed::s16x16 d[4];
	forceinline v4s16 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s16x16 operator+(v4s16x16 a, v4s16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x16 operator+(v4s16x16 a, v4s16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x16 operator+(v4s16 a, v4s16x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x16 operator+(v4s16x16 a, s16 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s16x16 operator+(s16 a, v4s16x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s16x16 operator-(v4s16x16 a, v4s16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x16 operator-(v4s16x16 a, v4s16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x16 operator-(v4s16 a, v4s16x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x16 operator-(v4s16x16 a, s16 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s16x16 operator-(s16 a, v4s16x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s16x16 operator*(v4s16x16 a, v4s16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x16 operator*(v4s16x16 a, v4s16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x16 operator*(v4s16 a, v4s16x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x16 operator*(v4s16x16 a, s16 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s16x16 operator*(s16 a, v4s16x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s16x16 operator/(v4s16x16 a, v4s16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x16 operator/(v4s16x16 a, v4s16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x16 operator/(v4s16 a, v4s16x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x16 operator/(v4s16x16 a, s16 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s16x16 operator/(s16 a, v4s16x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s16x16 operator^(v4s16x16 a, v4s16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x16 operator^(v4s16x16 a, v4s16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x16 operator^(v4s16 a, v4s16x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x16 operator^(v4s16x16 a, s16 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s16x16 operator^(s16 a, v4s16x16 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s16x16 operator&(v4s16x16 a, v4s16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x16 operator&(v4s16x16 a, v4s16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x16 operator&(v4s16 a, v4s16x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x16 operator&(v4s16x16 a, s16 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s16x16 operator&(s16 a, v4s16x16 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s16x16 operator|(v4s16x16 a, v4s16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x16 operator|(v4s16x16 a, v4s16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x16 operator|(v4s16 a, v4s16x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x16 operator|(v4s16x16 a, s16 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s16x16 operator|(s16 a, v4s16x16 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s16x16 operator<<(v4s16x16 a, s16 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s16x16 operator>>(v4s16x16 a, s16 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u16x16() const;
};

forceinline v4s16x16 V4s16x16(v4s16 a) { return {simd_typed::S16x16(a.x), simd_typed::S16x16(a.y), simd_typed::S16x16(a.z), simd_typed::S16x16(a.w), }; }
forceinline simd_typed::s16x16 dot(v4s16x16 a, v4s16x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s32x8 {
	struct { simd_typed::s32x8 x, y, z, w; };
	simd_typed::s32x8 d[4];
	forceinline v4s32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s32x8 operator+(v4s32x8 a, v4s32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x8 operator+(v4s32x8 a, v4s32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x8 operator+(v4s32 a, v4s32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x8 operator+(v4s32x8 a, s32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s32x8 operator+(s32 a, v4s32x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s32x8 operator-(v4s32x8 a, v4s32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x8 operator-(v4s32x8 a, v4s32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x8 operator-(v4s32 a, v4s32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x8 operator-(v4s32x8 a, s32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s32x8 operator-(s32 a, v4s32x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s32x8 operator*(v4s32x8 a, v4s32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x8 operator*(v4s32x8 a, v4s32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x8 operator*(v4s32 a, v4s32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x8 operator*(v4s32x8 a, s32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s32x8 operator*(s32 a, v4s32x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s32x8 operator/(v4s32x8 a, v4s32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x8 operator/(v4s32x8 a, v4s32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x8 operator/(v4s32 a, v4s32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x8 operator/(v4s32x8 a, s32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s32x8 operator/(s32 a, v4s32x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s32x8 operator^(v4s32x8 a, v4s32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x8 operator^(v4s32x8 a, v4s32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x8 operator^(v4s32 a, v4s32x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x8 operator^(v4s32x8 a, s32 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s32x8 operator^(s32 a, v4s32x8 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s32x8 operator&(v4s32x8 a, v4s32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x8 operator&(v4s32x8 a, v4s32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x8 operator&(v4s32 a, v4s32x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x8 operator&(v4s32x8 a, s32 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s32x8 operator&(s32 a, v4s32x8 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s32x8 operator|(v4s32x8 a, v4s32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x8 operator|(v4s32x8 a, v4s32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x8 operator|(v4s32 a, v4s32x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x8 operator|(v4s32x8 a, s32 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s32x8 operator|(s32 a, v4s32x8 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s32x8 operator<<(v4s32x8 a, s32 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s32x8 operator>>(v4s32x8 a, s32 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u32x8() const;
	forceinline operator v4f32x8() const;
};

forceinline v4s32x8 V4s32x8(v4s32 a) { return {simd_typed::S32x8(a.x), simd_typed::S32x8(a.y), simd_typed::S32x8(a.z), simd_typed::S32x8(a.w), }; }
forceinline simd_typed::s32x8 dot(v4s32x8 a, v4s32x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s64x4 {
	struct { simd_typed::s64x4 x, y, z, w; };
	simd_typed::s64x4 d[4];
	forceinline v4s64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s64x4 operator+(v4s64x4 a, v4s64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x4 operator+(v4s64x4 a, v4s64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x4 operator+(v4s64 a, v4s64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x4 operator+(v4s64x4 a, s64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s64x4 operator+(s64 a, v4s64x4 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s64x4 operator-(v4s64x4 a, v4s64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x4 operator-(v4s64x4 a, v4s64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x4 operator-(v4s64 a, v4s64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x4 operator-(v4s64x4 a, s64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s64x4 operator-(s64 a, v4s64x4 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s64x4 operator*(v4s64x4 a, v4s64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x4 operator*(v4s64x4 a, v4s64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x4 operator*(v4s64 a, v4s64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x4 operator*(v4s64x4 a, s64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s64x4 operator*(s64 a, v4s64x4 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s64x4 operator/(v4s64x4 a, v4s64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x4 operator/(v4s64x4 a, v4s64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x4 operator/(v4s64 a, v4s64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x4 operator/(v4s64x4 a, s64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s64x4 operator/(s64 a, v4s64x4 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s64x4 operator^(v4s64x4 a, v4s64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x4 operator^(v4s64x4 a, v4s64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x4 operator^(v4s64 a, v4s64x4 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x4 operator^(v4s64x4 a, s64 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s64x4 operator^(s64 a, v4s64x4 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s64x4 operator&(v4s64x4 a, v4s64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x4 operator&(v4s64x4 a, v4s64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x4 operator&(v4s64 a, v4s64x4 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x4 operator&(v4s64x4 a, s64 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s64x4 operator&(s64 a, v4s64x4 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s64x4 operator|(v4s64x4 a, v4s64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x4 operator|(v4s64x4 a, v4s64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x4 operator|(v4s64 a, v4s64x4 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x4 operator|(v4s64x4 a, s64 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s64x4 operator|(s64 a, v4s64x4 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s64x4 operator<<(v4s64x4 a, s64 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s64x4 operator>>(v4s64x4 a, s64 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u64x4() const;
	forceinline operator v4f64x4() const;
};

forceinline v4s64x4 V4s64x4(v4s64 a) { return {simd_typed::S64x4(a.x), simd_typed::S64x4(a.y), simd_typed::S64x4(a.z), simd_typed::S64x4(a.w), }; }
forceinline simd_typed::s64x4 dot(v4s64x4 a, v4s64x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4f32x8 {
	struct { simd_typed::f32x8 x, y, z, w; };
	simd_typed::f32x8 d[4];
	forceinline v4f32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4f32x8 operator+(v4f32x8 a, v4f32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x8 operator+(v4f32x8 a, v4f32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x8 operator+(v4f32 a, v4f32x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x8 operator+(v4f32x8 a, f32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4f32x8 operator+(f32 a, v4f32x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4f32x8 operator-(v4f32x8 a, v4f32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x8 operator-(v4f32x8 a, v4f32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x8 operator-(v4f32 a, v4f32x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x8 operator-(v4f32x8 a, f32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4f32x8 operator-(f32 a, v4f32x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4f32x8 operator*(v4f32x8 a, v4f32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x8 operator*(v4f32x8 a, v4f32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x8 operator*(v4f32 a, v4f32x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x8 operator*(v4f32x8 a, f32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4f32x8 operator*(f32 a, v4f32x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4f32x8 operator/(v4f32x8 a, v4f32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x8 operator/(v4f32x8 a, v4f32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x8 operator/(v4f32 a, v4f32x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x8 operator/(v4f32x8 a, f32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4f32x8 operator/(f32 a, v4f32x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline operator v4u32x8() const;
	forceinline operator v4s32x8() const;
};

forceinline v4f32x8 V4f32x8(v4f32 a) { return {simd_typed::F32x8(a.x), simd_typed::F32x8(a.y), simd_typed::F32x8(a.z), simd_typed::F32x8(a.w), }; }
forceinline simd_typed::f32x8 dot(v4f32x8 a, v4f32x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4f64x4 {
	struct { simd_typed::f64x4 x, y, z, w; };
	simd_typed::f64x4 d[4];
	forceinline v4f64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4f64x4 operator+(v4f64x4 a, v4f64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x4 operator+(v4f64x4 a, v4f64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x4 operator+(v4f64 a, v4f64x4 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x4 operator+(v4f64x4 a, f64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4f64x4 operator+(f64 a, v4f64x4 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4f64x4 operator-(v4f64x4 a, v4f64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x4 operator-(v4f64x4 a, v4f64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x4 operator-(v4f64 a, v4f64x4 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x4 operator-(v4f64x4 a, f64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4f64x4 operator-(f64 a, v4f64x4 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4f64x4 operator*(v4f64x4 a, v4f64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x4 operator*(v4f64x4 a, v4f64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x4 operator*(v4f64 a, v4f64x4 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x4 operator*(v4f64x4 a, f64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4f64x4 operator*(f64 a, v4f64x4 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4f64x4 operator/(v4f64x4 a, v4f64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x4 operator/(v4f64x4 a, v4f64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x4 operator/(v4f64 a, v4f64x4 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x4 operator/(v4f64x4 a, f64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4f64x4 operator/(f64 a, v4f64x4 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline operator v4u64x4() const;
	forceinline operator v4s64x4() const;
};

forceinline v4f64x4 V4f64x4(v4f64 a) { return {simd_typed::F64x4(a.x), simd_typed::F64x4(a.y), simd_typed::F64x4(a.z), simd_typed::F64x4(a.w), }; }
forceinline simd_typed::f64x4 dot(v4f64x4 a, v4f64x4 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

forceinline v4u8x32::operator v4s8x32() const { return {(simd_typed::s8x32)x, (simd_typed::s8x32)y, (simd_typed::s8x32)z, (simd_typed::s8x32)w, }; }
forceinline v4u16x16::operator v4s16x16() const { return {(simd_typed::s16x16)x, (simd_typed::s16x16)y, (simd_typed::s16x16)z, (simd_typed::s16x16)w, }; }
forceinline v4u32x8::operator v4s32x8() const { return {(simd_typed::s32x8)x, (simd_typed::s32x8)y, (simd_typed::s32x8)z, (simd_typed::s32x8)w, }; }
forceinline v4u32x8::operator v4f32x8() const { return {(simd_typed::f32x8)x, (simd_typed::f32x8)y, (simd_typed::f32x8)z, (simd_typed::f32x8)w, }; }
forceinline v4u64x4::operator v4s64x4() const { return {(simd_typed::s64x4)x, (simd_typed::s64x4)y, (simd_typed::s64x4)z, (simd_typed::s64x4)w, }; }
forceinline v4u64x4::operator v4f64x4() const { return {(simd_typed::f64x4)x, (simd_typed::f64x4)y, (simd_typed::f64x4)z, (simd_typed::f64x4)w, }; }
forceinline v4s8x32::operator v4u8x32() const { return {(simd_typed::u8x32)x, (simd_typed::u8x32)y, (simd_typed::u8x32)z, (simd_typed::u8x32)w, }; }
forceinline v4s16x16::operator v4u16x16() const { return {(simd_typed::u16x16)x, (simd_typed::u16x16)y, (simd_typed::u16x16)z, (simd_typed::u16x16)w, }; }
forceinline v4s32x8::operator v4u32x8() const { return {(simd_typed::u32x8)x, (simd_typed::u32x8)y, (simd_typed::u32x8)z, (simd_typed::u32x8)w, }; }
forceinline v4s32x8::operator v4f32x8() const { return {(simd_typed::f32x8)x, (simd_typed::f32x8)y, (simd_typed::f32x8)z, (simd_typed::f32x8)w, }; }
forceinline v4s64x4::operator v4u64x4() const { return {(simd_typed::u64x4)x, (simd_typed::u64x4)y, (simd_typed::u64x4)z, (simd_typed::u64x4)w, }; }
forceinline v4s64x4::operator v4f64x4() const { return {(simd_typed::f64x4)x, (simd_typed::f64x4)y, (simd_typed::f64x4)z, (simd_typed::f64x4)w, }; }
forceinline v4f32x8::operator v4u32x8() const { return {(simd_typed::u32x8)x, (simd_typed::u32x8)y, (simd_typed::u32x8)z, (simd_typed::u32x8)w, }; }
forceinline v4f32x8::operator v4s32x8() const { return {(simd_typed::s32x8)x, (simd_typed::s32x8)y, (simd_typed::s32x8)z, (simd_typed::s32x8)w, }; }
forceinline v4f64x4::operator v4u64x4() const { return {(simd_typed::u64x4)x, (simd_typed::u64x4)y, (simd_typed::u64x4)z, (simd_typed::u64x4)w, }; }
forceinline v4f64x4::operator v4s64x4() const { return {(simd_typed::s64x4)x, (simd_typed::s64x4)y, (simd_typed::s64x4)z, (simd_typed::s64x4)w, }; }

#endif

#if ARCH_AVX512
union v2u8x64 {
	struct { simd_typed::u8x64 x, y; };
	simd_typed::u8x64 d[2];
	forceinline v2u8 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u8x64 operator+(v2u8x64 a, v2u8x64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x64 operator+(v2u8x64 a, v2u8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x64 operator+(v2u8 a, v2u8x64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u8x64 operator+(v2u8x64 a, u8 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u8x64 operator+(u8 a, v2u8x64 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u8x64 operator-(v2u8x64 a, v2u8x64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x64 operator-(v2u8x64 a, v2u8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x64 operator-(v2u8 a, v2u8x64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u8x64 operator-(v2u8x64 a, u8 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u8x64 operator-(u8 a, v2u8x64 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u8x64 operator*(v2u8x64 a, v2u8x64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x64 operator*(v2u8x64 a, v2u8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x64 operator*(v2u8 a, v2u8x64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u8x64 operator*(v2u8x64 a, u8 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u8x64 operator*(u8 a, v2u8x64 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u8x64 operator/(v2u8x64 a, v2u8x64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x64 operator/(v2u8x64 a, v2u8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x64 operator/(v2u8 a, v2u8x64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u8x64 operator/(v2u8x64 a, u8 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u8x64 operator/(u8 a, v2u8x64 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u8x64 operator^(v2u8x64 a, v2u8x64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x64 operator^(v2u8x64 a, v2u8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x64 operator^(v2u8 a, v2u8x64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u8x64 operator^(v2u8x64 a, u8 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u8x64 operator^(u8 a, v2u8x64 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u8x64 operator&(v2u8x64 a, v2u8x64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x64 operator&(v2u8x64 a, v2u8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x64 operator&(v2u8 a, v2u8x64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u8x64 operator&(v2u8x64 a, u8 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u8x64 operator&(u8 a, v2u8x64 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u8x64 operator|(v2u8x64 a, v2u8x64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x64 operator|(v2u8x64 a, v2u8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x64 operator|(v2u8 a, v2u8x64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u8x64 operator|(v2u8x64 a, u8 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u8x64 operator|(u8 a, v2u8x64 b) { return {a|b.x, a|b.y};}
	forceinline operator v2s8x64() const;
};

forceinline v2u8x64 V2u8x64(v2u8 a) { return {simd_typed::U8x64(a.x), simd_typed::U8x64(a.y), }; }
forceinline simd_typed::u8x64 dot(v2u8x64 a, v2u8x64 b) { return a.x*b.x+a.y*b.y; }

union v2u16x32 {
	struct { simd_typed::u16x32 x, y; };
	simd_typed::u16x32 d[2];
	forceinline v2u16 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u16x32 operator+(v2u16x32 a, v2u16x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x32 operator+(v2u16x32 a, v2u16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x32 operator+(v2u16 a, v2u16x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u16x32 operator+(v2u16x32 a, u16 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u16x32 operator+(u16 a, v2u16x32 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u16x32 operator-(v2u16x32 a, v2u16x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x32 operator-(v2u16x32 a, v2u16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x32 operator-(v2u16 a, v2u16x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u16x32 operator-(v2u16x32 a, u16 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u16x32 operator-(u16 a, v2u16x32 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u16x32 operator*(v2u16x32 a, v2u16x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x32 operator*(v2u16x32 a, v2u16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x32 operator*(v2u16 a, v2u16x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u16x32 operator*(v2u16x32 a, u16 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u16x32 operator*(u16 a, v2u16x32 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u16x32 operator/(v2u16x32 a, v2u16x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x32 operator/(v2u16x32 a, v2u16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x32 operator/(v2u16 a, v2u16x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u16x32 operator/(v2u16x32 a, u16 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u16x32 operator/(u16 a, v2u16x32 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u16x32 operator^(v2u16x32 a, v2u16x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x32 operator^(v2u16x32 a, v2u16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x32 operator^(v2u16 a, v2u16x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u16x32 operator^(v2u16x32 a, u16 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u16x32 operator^(u16 a, v2u16x32 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u16x32 operator&(v2u16x32 a, v2u16x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x32 operator&(v2u16x32 a, v2u16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x32 operator&(v2u16 a, v2u16x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u16x32 operator&(v2u16x32 a, u16 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u16x32 operator&(u16 a, v2u16x32 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u16x32 operator|(v2u16x32 a, v2u16x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x32 operator|(v2u16x32 a, v2u16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x32 operator|(v2u16 a, v2u16x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u16x32 operator|(v2u16x32 a, u16 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u16x32 operator|(u16 a, v2u16x32 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u16x32 operator<<(v2u16x32 a, u16 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u16x32 operator>>(v2u16x32 a, u16 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s16x32() const;
};

forceinline v2u16x32 V2u16x32(v2u16 a) { return {simd_typed::U16x32(a.x), simd_typed::U16x32(a.y), }; }
forceinline simd_typed::u16x32 dot(v2u16x32 a, v2u16x32 b) { return a.x*b.x+a.y*b.y; }

union v2u32x16 {
	struct { simd_typed::u32x16 x, y; };
	simd_typed::u32x16 d[2];
	forceinline v2u32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u32x16 operator+(v2u32x16 a, v2u32x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x16 operator+(v2u32x16 a, v2u32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x16 operator+(v2u32 a, v2u32x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u32x16 operator+(v2u32x16 a, u32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u32x16 operator+(u32 a, v2u32x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u32x16 operator-(v2u32x16 a, v2u32x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x16 operator-(v2u32x16 a, v2u32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x16 operator-(v2u32 a, v2u32x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u32x16 operator-(v2u32x16 a, u32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u32x16 operator-(u32 a, v2u32x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u32x16 operator*(v2u32x16 a, v2u32x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x16 operator*(v2u32x16 a, v2u32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x16 operator*(v2u32 a, v2u32x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u32x16 operator*(v2u32x16 a, u32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u32x16 operator*(u32 a, v2u32x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u32x16 operator/(v2u32x16 a, v2u32x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x16 operator/(v2u32x16 a, v2u32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x16 operator/(v2u32 a, v2u32x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u32x16 operator/(v2u32x16 a, u32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u32x16 operator/(u32 a, v2u32x16 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u32x16 operator^(v2u32x16 a, v2u32x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x16 operator^(v2u32x16 a, v2u32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x16 operator^(v2u32 a, v2u32x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u32x16 operator^(v2u32x16 a, u32 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u32x16 operator^(u32 a, v2u32x16 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u32x16 operator&(v2u32x16 a, v2u32x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x16 operator&(v2u32x16 a, v2u32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x16 operator&(v2u32 a, v2u32x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u32x16 operator&(v2u32x16 a, u32 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u32x16 operator&(u32 a, v2u32x16 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u32x16 operator|(v2u32x16 a, v2u32x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x16 operator|(v2u32x16 a, v2u32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x16 operator|(v2u32 a, v2u32x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u32x16 operator|(v2u32x16 a, u32 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u32x16 operator|(u32 a, v2u32x16 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u32x16 operator<<(v2u32x16 a, u32 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u32x16 operator>>(v2u32x16 a, u32 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s32x16() const;
	forceinline operator v2f32x16() const;
};

forceinline v2u32x16 V2u32x16(v2u32 a) { return {simd_typed::U32x16(a.x), simd_typed::U32x16(a.y), }; }
forceinline simd_typed::u32x16 dot(v2u32x16 a, v2u32x16 b) { return a.x*b.x+a.y*b.y; }

union v2u64x8 {
	struct { simd_typed::u64x8 x, y; };
	simd_typed::u64x8 d[2];
	forceinline v2u64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2u64x8 operator+(v2u64x8 a, v2u64x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x8 operator+(v2u64x8 a, v2u64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x8 operator+(v2u64 a, v2u64x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2u64x8 operator+(v2u64x8 a, u64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2u64x8 operator+(u64 a, v2u64x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2u64x8 operator-(v2u64x8 a, v2u64x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x8 operator-(v2u64x8 a, v2u64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x8 operator-(v2u64 a, v2u64x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2u64x8 operator-(v2u64x8 a, u64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2u64x8 operator-(u64 a, v2u64x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2u64x8 operator*(v2u64x8 a, v2u64x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x8 operator*(v2u64x8 a, v2u64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x8 operator*(v2u64 a, v2u64x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2u64x8 operator*(v2u64x8 a, u64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2u64x8 operator*(u64 a, v2u64x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2u64x8 operator/(v2u64x8 a, v2u64x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x8 operator/(v2u64x8 a, v2u64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x8 operator/(v2u64 a, v2u64x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2u64x8 operator/(v2u64x8 a, u64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2u64x8 operator/(u64 a, v2u64x8 b) { return {a/b.x, a/b.y};}
	forceinline friend v2u64x8 operator^(v2u64x8 a, v2u64x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x8 operator^(v2u64x8 a, v2u64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x8 operator^(v2u64 a, v2u64x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2u64x8 operator^(v2u64x8 a, u64 b) { return {a.x^b, a.y^b};}
	forceinline friend v2u64x8 operator^(u64 a, v2u64x8 b) { return {a^b.x, a^b.y};}
	forceinline friend v2u64x8 operator&(v2u64x8 a, v2u64x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x8 operator&(v2u64x8 a, v2u64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x8 operator&(v2u64 a, v2u64x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2u64x8 operator&(v2u64x8 a, u64 b) { return {a.x&b, a.y&b};}
	forceinline friend v2u64x8 operator&(u64 a, v2u64x8 b) { return {a&b.x, a&b.y};}
	forceinline friend v2u64x8 operator|(v2u64x8 a, v2u64x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x8 operator|(v2u64x8 a, v2u64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x8 operator|(v2u64 a, v2u64x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2u64x8 operator|(v2u64x8 a, u64 b) { return {a.x|b, a.y|b};}
	forceinline friend v2u64x8 operator|(u64 a, v2u64x8 b) { return {a|b.x, a|b.y};}
	forceinline friend v2u64x8 operator<<(v2u64x8 a, u64 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2u64x8 operator>>(v2u64x8 a, u64 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2s64x8() const;
	forceinline operator v2f64x8() const;
};

forceinline v2u64x8 V2u64x8(v2u64 a) { return {simd_typed::U64x8(a.x), simd_typed::U64x8(a.y), }; }
forceinline simd_typed::u64x8 dot(v2u64x8 a, v2u64x8 b) { return a.x*b.x+a.y*b.y; }

union v2s8x64 {
	struct { simd_typed::s8x64 x, y; };
	simd_typed::s8x64 d[2];
	forceinline v2s8 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s8x64 operator+(v2s8x64 a, v2s8x64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x64 operator+(v2s8x64 a, v2s8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x64 operator+(v2s8 a, v2s8x64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s8x64 operator+(v2s8x64 a, s8 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s8x64 operator+(s8 a, v2s8x64 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s8x64 operator-(v2s8x64 a, v2s8x64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x64 operator-(v2s8x64 a, v2s8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x64 operator-(v2s8 a, v2s8x64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s8x64 operator-(v2s8x64 a, s8 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s8x64 operator-(s8 a, v2s8x64 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s8x64 operator*(v2s8x64 a, v2s8x64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x64 operator*(v2s8x64 a, v2s8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x64 operator*(v2s8 a, v2s8x64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s8x64 operator*(v2s8x64 a, s8 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s8x64 operator*(s8 a, v2s8x64 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s8x64 operator/(v2s8x64 a, v2s8x64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x64 operator/(v2s8x64 a, v2s8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x64 operator/(v2s8 a, v2s8x64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s8x64 operator/(v2s8x64 a, s8 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s8x64 operator/(s8 a, v2s8x64 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s8x64 operator^(v2s8x64 a, v2s8x64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x64 operator^(v2s8x64 a, v2s8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x64 operator^(v2s8 a, v2s8x64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s8x64 operator^(v2s8x64 a, s8 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s8x64 operator^(s8 a, v2s8x64 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s8x64 operator&(v2s8x64 a, v2s8x64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x64 operator&(v2s8x64 a, v2s8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x64 operator&(v2s8 a, v2s8x64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s8x64 operator&(v2s8x64 a, s8 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s8x64 operator&(s8 a, v2s8x64 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s8x64 operator|(v2s8x64 a, v2s8x64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x64 operator|(v2s8x64 a, v2s8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x64 operator|(v2s8 a, v2s8x64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s8x64 operator|(v2s8x64 a, s8 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s8x64 operator|(s8 a, v2s8x64 b) { return {a|b.x, a|b.y};}
	forceinline operator v2u8x64() const;
};

forceinline v2s8x64 V2s8x64(v2s8 a) { return {simd_typed::S8x64(a.x), simd_typed::S8x64(a.y), }; }
forceinline simd_typed::s8x64 dot(v2s8x64 a, v2s8x64 b) { return a.x*b.x+a.y*b.y; }

union v2s16x32 {
	struct { simd_typed::s16x32 x, y; };
	simd_typed::s16x32 d[2];
	forceinline v2s16 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s16x32 operator+(v2s16x32 a, v2s16x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x32 operator+(v2s16x32 a, v2s16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x32 operator+(v2s16 a, v2s16x32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s16x32 operator+(v2s16x32 a, s16 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s16x32 operator+(s16 a, v2s16x32 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s16x32 operator-(v2s16x32 a, v2s16x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x32 operator-(v2s16x32 a, v2s16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x32 operator-(v2s16 a, v2s16x32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s16x32 operator-(v2s16x32 a, s16 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s16x32 operator-(s16 a, v2s16x32 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s16x32 operator*(v2s16x32 a, v2s16x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x32 operator*(v2s16x32 a, v2s16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x32 operator*(v2s16 a, v2s16x32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s16x32 operator*(v2s16x32 a, s16 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s16x32 operator*(s16 a, v2s16x32 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s16x32 operator/(v2s16x32 a, v2s16x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x32 operator/(v2s16x32 a, v2s16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x32 operator/(v2s16 a, v2s16x32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s16x32 operator/(v2s16x32 a, s16 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s16x32 operator/(s16 a, v2s16x32 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s16x32 operator^(v2s16x32 a, v2s16x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x32 operator^(v2s16x32 a, v2s16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x32 operator^(v2s16 a, v2s16x32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s16x32 operator^(v2s16x32 a, s16 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s16x32 operator^(s16 a, v2s16x32 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s16x32 operator&(v2s16x32 a, v2s16x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x32 operator&(v2s16x32 a, v2s16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x32 operator&(v2s16 a, v2s16x32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s16x32 operator&(v2s16x32 a, s16 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s16x32 operator&(s16 a, v2s16x32 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s16x32 operator|(v2s16x32 a, v2s16x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x32 operator|(v2s16x32 a, v2s16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x32 operator|(v2s16 a, v2s16x32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s16x32 operator|(v2s16x32 a, s16 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s16x32 operator|(s16 a, v2s16x32 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s16x32 operator<<(v2s16x32 a, s16 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s16x32 operator>>(v2s16x32 a, s16 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u16x32() const;
};

forceinline v2s16x32 V2s16x32(v2s16 a) { return {simd_typed::S16x32(a.x), simd_typed::S16x32(a.y), }; }
forceinline simd_typed::s16x32 dot(v2s16x32 a, v2s16x32 b) { return a.x*b.x+a.y*b.y; }

union v2s32x16 {
	struct { simd_typed::s32x16 x, y; };
	simd_typed::s32x16 d[2];
	forceinline v2s32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s32x16 operator+(v2s32x16 a, v2s32x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x16 operator+(v2s32x16 a, v2s32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x16 operator+(v2s32 a, v2s32x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s32x16 operator+(v2s32x16 a, s32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s32x16 operator+(s32 a, v2s32x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s32x16 operator-(v2s32x16 a, v2s32x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x16 operator-(v2s32x16 a, v2s32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x16 operator-(v2s32 a, v2s32x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s32x16 operator-(v2s32x16 a, s32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s32x16 operator-(s32 a, v2s32x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s32x16 operator*(v2s32x16 a, v2s32x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x16 operator*(v2s32x16 a, v2s32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x16 operator*(v2s32 a, v2s32x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s32x16 operator*(v2s32x16 a, s32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s32x16 operator*(s32 a, v2s32x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s32x16 operator/(v2s32x16 a, v2s32x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x16 operator/(v2s32x16 a, v2s32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x16 operator/(v2s32 a, v2s32x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s32x16 operator/(v2s32x16 a, s32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s32x16 operator/(s32 a, v2s32x16 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s32x16 operator^(v2s32x16 a, v2s32x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x16 operator^(v2s32x16 a, v2s32 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x16 operator^(v2s32 a, v2s32x16 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s32x16 operator^(v2s32x16 a, s32 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s32x16 operator^(s32 a, v2s32x16 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s32x16 operator&(v2s32x16 a, v2s32x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x16 operator&(v2s32x16 a, v2s32 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x16 operator&(v2s32 a, v2s32x16 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s32x16 operator&(v2s32x16 a, s32 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s32x16 operator&(s32 a, v2s32x16 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s32x16 operator|(v2s32x16 a, v2s32x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x16 operator|(v2s32x16 a, v2s32 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x16 operator|(v2s32 a, v2s32x16 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s32x16 operator|(v2s32x16 a, s32 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s32x16 operator|(s32 a, v2s32x16 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s32x16 operator<<(v2s32x16 a, s32 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s32x16 operator>>(v2s32x16 a, s32 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u32x16() const;
	forceinline operator v2f32x16() const;
};

forceinline v2s32x16 V2s32x16(v2s32 a) { return {simd_typed::S32x16(a.x), simd_typed::S32x16(a.y), }; }
forceinline simd_typed::s32x16 dot(v2s32x16 a, v2s32x16 b) { return a.x*b.x+a.y*b.y; }

union v2s64x8 {
	struct { simd_typed::s64x8 x, y; };
	simd_typed::s64x8 d[2];
	forceinline v2s64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2s64x8 operator+(v2s64x8 a, v2s64x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x8 operator+(v2s64x8 a, v2s64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x8 operator+(v2s64 a, v2s64x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2s64x8 operator+(v2s64x8 a, s64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2s64x8 operator+(s64 a, v2s64x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2s64x8 operator-(v2s64x8 a, v2s64x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x8 operator-(v2s64x8 a, v2s64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x8 operator-(v2s64 a, v2s64x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2s64x8 operator-(v2s64x8 a, s64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2s64x8 operator-(s64 a, v2s64x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2s64x8 operator*(v2s64x8 a, v2s64x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x8 operator*(v2s64x8 a, v2s64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x8 operator*(v2s64 a, v2s64x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2s64x8 operator*(v2s64x8 a, s64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2s64x8 operator*(s64 a, v2s64x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2s64x8 operator/(v2s64x8 a, v2s64x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x8 operator/(v2s64x8 a, v2s64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x8 operator/(v2s64 a, v2s64x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2s64x8 operator/(v2s64x8 a, s64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2s64x8 operator/(s64 a, v2s64x8 b) { return {a/b.x, a/b.y};}
	forceinline friend v2s64x8 operator^(v2s64x8 a, v2s64x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x8 operator^(v2s64x8 a, v2s64 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x8 operator^(v2s64 a, v2s64x8 b) { return {a.x^b.x, a.y^b.y};}
	forceinline friend v2s64x8 operator^(v2s64x8 a, s64 b) { return {a.x^b, a.y^b};}
	forceinline friend v2s64x8 operator^(s64 a, v2s64x8 b) { return {a^b.x, a^b.y};}
	forceinline friend v2s64x8 operator&(v2s64x8 a, v2s64x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x8 operator&(v2s64x8 a, v2s64 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x8 operator&(v2s64 a, v2s64x8 b) { return {a.x&b.x, a.y&b.y};}
	forceinline friend v2s64x8 operator&(v2s64x8 a, s64 b) { return {a.x&b, a.y&b};}
	forceinline friend v2s64x8 operator&(s64 a, v2s64x8 b) { return {a&b.x, a&b.y};}
	forceinline friend v2s64x8 operator|(v2s64x8 a, v2s64x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x8 operator|(v2s64x8 a, v2s64 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x8 operator|(v2s64 a, v2s64x8 b) { return {a.x|b.x, a.y|b.y};}
	forceinline friend v2s64x8 operator|(v2s64x8 a, s64 b) { return {a.x|b, a.y|b};}
	forceinline friend v2s64x8 operator|(s64 a, v2s64x8 b) { return {a|b.x, a|b.y};}
	forceinline friend v2s64x8 operator<<(v2s64x8 a, s64 b) { return {a.x<<b, a.y<<b};}
	forceinline friend v2s64x8 operator>>(v2s64x8 a, s64 b) { return {a.x>>b, a.y>>b};}
	forceinline operator v2u64x8() const;
	forceinline operator v2f64x8() const;
};

forceinline v2s64x8 V2s64x8(v2s64 a) { return {simd_typed::S64x8(a.x), simd_typed::S64x8(a.y), }; }
forceinline simd_typed::s64x8 dot(v2s64x8 a, v2s64x8 b) { return a.x*b.x+a.y*b.y; }

union v2f32x16 {
	struct { simd_typed::f32x16 x, y; };
	simd_typed::f32x16 d[2];
	forceinline v2f32 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2f32x16 operator+(v2f32x16 a, v2f32x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x16 operator+(v2f32x16 a, v2f32 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x16 operator+(v2f32 a, v2f32x16 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f32x16 operator+(v2f32x16 a, f32 b) { return {a.x+b, a.y+b};}
	forceinline friend v2f32x16 operator+(f32 a, v2f32x16 b) { return {a+b.x, a+b.y};}
	forceinline friend v2f32x16 operator-(v2f32x16 a, v2f32x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x16 operator-(v2f32x16 a, v2f32 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x16 operator-(v2f32 a, v2f32x16 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f32x16 operator-(v2f32x16 a, f32 b) { return {a.x-b, a.y-b};}
	forceinline friend v2f32x16 operator-(f32 a, v2f32x16 b) { return {a-b.x, a-b.y};}
	forceinline friend v2f32x16 operator*(v2f32x16 a, v2f32x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x16 operator*(v2f32x16 a, v2f32 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x16 operator*(v2f32 a, v2f32x16 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f32x16 operator*(v2f32x16 a, f32 b) { return {a.x*b, a.y*b};}
	forceinline friend v2f32x16 operator*(f32 a, v2f32x16 b) { return {a*b.x, a*b.y};}
	forceinline friend v2f32x16 operator/(v2f32x16 a, v2f32x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x16 operator/(v2f32x16 a, v2f32 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x16 operator/(v2f32 a, v2f32x16 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f32x16 operator/(v2f32x16 a, f32 b) { return {a.x/b, a.y/b};}
	forceinline friend v2f32x16 operator/(f32 a, v2f32x16 b) { return {a/b.x, a/b.y};}
	forceinline operator v2u32x16() const;
	forceinline operator v2s32x16() const;
};

forceinline v2f32x16 V2f32x16(v2f32 a) { return {simd_typed::F32x16(a.x), simd_typed::F32x16(a.y), }; }
forceinline simd_typed::f32x16 dot(v2f32x16 a, v2f32x16 b) { return a.x*b.x+a.y*b.y; }

union v2f64x8 {
	struct { simd_typed::f64x8 x, y; };
	simd_typed::f64x8 d[2];
	forceinline v2f64 subvector(umm i) const { return {x[i], y[i], }; }
	forceinline friend v2f64x8 operator+(v2f64x8 a, v2f64x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x8 operator+(v2f64x8 a, v2f64 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x8 operator+(v2f64 a, v2f64x8 b) { return {a.x+b.x, a.y+b.y};}
	forceinline friend v2f64x8 operator+(v2f64x8 a, f64 b) { return {a.x+b, a.y+b};}
	forceinline friend v2f64x8 operator+(f64 a, v2f64x8 b) { return {a+b.x, a+b.y};}
	forceinline friend v2f64x8 operator-(v2f64x8 a, v2f64x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x8 operator-(v2f64x8 a, v2f64 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x8 operator-(v2f64 a, v2f64x8 b) { return {a.x-b.x, a.y-b.y};}
	forceinline friend v2f64x8 operator-(v2f64x8 a, f64 b) { return {a.x-b, a.y-b};}
	forceinline friend v2f64x8 operator-(f64 a, v2f64x8 b) { return {a-b.x, a-b.y};}
	forceinline friend v2f64x8 operator*(v2f64x8 a, v2f64x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x8 operator*(v2f64x8 a, v2f64 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x8 operator*(v2f64 a, v2f64x8 b) { return {a.x*b.x, a.y*b.y};}
	forceinline friend v2f64x8 operator*(v2f64x8 a, f64 b) { return {a.x*b, a.y*b};}
	forceinline friend v2f64x8 operator*(f64 a, v2f64x8 b) { return {a*b.x, a*b.y};}
	forceinline friend v2f64x8 operator/(v2f64x8 a, v2f64x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x8 operator/(v2f64x8 a, v2f64 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x8 operator/(v2f64 a, v2f64x8 b) { return {a.x/b.x, a.y/b.y};}
	forceinline friend v2f64x8 operator/(v2f64x8 a, f64 b) { return {a.x/b, a.y/b};}
	forceinline friend v2f64x8 operator/(f64 a, v2f64x8 b) { return {a/b.x, a/b.y};}
	forceinline operator v2u64x8() const;
	forceinline operator v2s64x8() const;
};

forceinline v2f64x8 V2f64x8(v2f64 a) { return {simd_typed::F64x8(a.x), simd_typed::F64x8(a.y), }; }
forceinline simd_typed::f64x8 dot(v2f64x8 a, v2f64x8 b) { return a.x*b.x+a.y*b.y; }

forceinline v2u8x64::operator v2s8x64() const { return {(simd_typed::s8x64)x, (simd_typed::s8x64)y, }; }
forceinline v2u16x32::operator v2s16x32() const { return {(simd_typed::s16x32)x, (simd_typed::s16x32)y, }; }
forceinline v2u32x16::operator v2s32x16() const { return {(simd_typed::s32x16)x, (simd_typed::s32x16)y, }; }
forceinline v2u32x16::operator v2f32x16() const { return {(simd_typed::f32x16)x, (simd_typed::f32x16)y, }; }
forceinline v2u64x8::operator v2s64x8() const { return {(simd_typed::s64x8)x, (simd_typed::s64x8)y, }; }
forceinline v2u64x8::operator v2f64x8() const { return {(simd_typed::f64x8)x, (simd_typed::f64x8)y, }; }
forceinline v2s8x64::operator v2u8x64() const { return {(simd_typed::u8x64)x, (simd_typed::u8x64)y, }; }
forceinline v2s16x32::operator v2u16x32() const { return {(simd_typed::u16x32)x, (simd_typed::u16x32)y, }; }
forceinline v2s32x16::operator v2u32x16() const { return {(simd_typed::u32x16)x, (simd_typed::u32x16)y, }; }
forceinline v2s32x16::operator v2f32x16() const { return {(simd_typed::f32x16)x, (simd_typed::f32x16)y, }; }
forceinline v2s64x8::operator v2u64x8() const { return {(simd_typed::u64x8)x, (simd_typed::u64x8)y, }; }
forceinline v2s64x8::operator v2f64x8() const { return {(simd_typed::f64x8)x, (simd_typed::f64x8)y, }; }
forceinline v2f32x16::operator v2u32x16() const { return {(simd_typed::u32x16)x, (simd_typed::u32x16)y, }; }
forceinline v2f32x16::operator v2s32x16() const { return {(simd_typed::s32x16)x, (simd_typed::s32x16)y, }; }
forceinline v2f64x8::operator v2u64x8() const { return {(simd_typed::u64x8)x, (simd_typed::u64x8)y, }; }
forceinline v2f64x8::operator v2s64x8() const { return {(simd_typed::s64x8)x, (simd_typed::s64x8)y, }; }
union v3u8x64 {
	struct { simd_typed::u8x64 x, y, z; };
	simd_typed::u8x64 d[3];
	forceinline v3u8 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u8x64 operator+(v3u8x64 a, v3u8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x64 operator+(v3u8x64 a, v3u8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x64 operator+(v3u8 a, v3u8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u8x64 operator+(v3u8x64 a, u8 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u8x64 operator+(u8 a, v3u8x64 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u8x64 operator-(v3u8x64 a, v3u8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x64 operator-(v3u8x64 a, v3u8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x64 operator-(v3u8 a, v3u8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u8x64 operator-(v3u8x64 a, u8 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u8x64 operator-(u8 a, v3u8x64 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u8x64 operator*(v3u8x64 a, v3u8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x64 operator*(v3u8x64 a, v3u8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x64 operator*(v3u8 a, v3u8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u8x64 operator*(v3u8x64 a, u8 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u8x64 operator*(u8 a, v3u8x64 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u8x64 operator/(v3u8x64 a, v3u8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x64 operator/(v3u8x64 a, v3u8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x64 operator/(v3u8 a, v3u8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u8x64 operator/(v3u8x64 a, u8 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u8x64 operator/(u8 a, v3u8x64 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u8x64 operator^(v3u8x64 a, v3u8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x64 operator^(v3u8x64 a, v3u8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x64 operator^(v3u8 a, v3u8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u8x64 operator^(v3u8x64 a, u8 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u8x64 operator^(u8 a, v3u8x64 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u8x64 operator&(v3u8x64 a, v3u8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x64 operator&(v3u8x64 a, v3u8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x64 operator&(v3u8 a, v3u8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u8x64 operator&(v3u8x64 a, u8 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u8x64 operator&(u8 a, v3u8x64 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u8x64 operator|(v3u8x64 a, v3u8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x64 operator|(v3u8x64 a, v3u8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x64 operator|(v3u8 a, v3u8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u8x64 operator|(v3u8x64 a, u8 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u8x64 operator|(u8 a, v3u8x64 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline operator v3s8x64() const;
};

forceinline v3u8x64 V3u8x64(v3u8 a) { return {simd_typed::U8x64(a.x), simd_typed::U8x64(a.y), simd_typed::U8x64(a.z), }; }
forceinline simd_typed::u8x64 dot(v3u8x64 a, v3u8x64 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u16x32 {
	struct { simd_typed::u16x32 x, y, z; };
	simd_typed::u16x32 d[3];
	forceinline v3u16 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u16x32 operator+(v3u16x32 a, v3u16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x32 operator+(v3u16x32 a, v3u16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x32 operator+(v3u16 a, v3u16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u16x32 operator+(v3u16x32 a, u16 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u16x32 operator+(u16 a, v3u16x32 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u16x32 operator-(v3u16x32 a, v3u16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x32 operator-(v3u16x32 a, v3u16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x32 operator-(v3u16 a, v3u16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u16x32 operator-(v3u16x32 a, u16 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u16x32 operator-(u16 a, v3u16x32 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u16x32 operator*(v3u16x32 a, v3u16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x32 operator*(v3u16x32 a, v3u16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x32 operator*(v3u16 a, v3u16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u16x32 operator*(v3u16x32 a, u16 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u16x32 operator*(u16 a, v3u16x32 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u16x32 operator/(v3u16x32 a, v3u16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x32 operator/(v3u16x32 a, v3u16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x32 operator/(v3u16 a, v3u16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u16x32 operator/(v3u16x32 a, u16 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u16x32 operator/(u16 a, v3u16x32 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u16x32 operator^(v3u16x32 a, v3u16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x32 operator^(v3u16x32 a, v3u16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x32 operator^(v3u16 a, v3u16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u16x32 operator^(v3u16x32 a, u16 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u16x32 operator^(u16 a, v3u16x32 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u16x32 operator&(v3u16x32 a, v3u16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x32 operator&(v3u16x32 a, v3u16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x32 operator&(v3u16 a, v3u16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u16x32 operator&(v3u16x32 a, u16 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u16x32 operator&(u16 a, v3u16x32 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u16x32 operator|(v3u16x32 a, v3u16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x32 operator|(v3u16x32 a, v3u16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x32 operator|(v3u16 a, v3u16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u16x32 operator|(v3u16x32 a, u16 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u16x32 operator|(u16 a, v3u16x32 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u16x32 operator<<(v3u16x32 a, u16 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u16x32 operator>>(v3u16x32 a, u16 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s16x32() const;
};

forceinline v3u16x32 V3u16x32(v3u16 a) { return {simd_typed::U16x32(a.x), simd_typed::U16x32(a.y), simd_typed::U16x32(a.z), }; }
forceinline simd_typed::u16x32 dot(v3u16x32 a, v3u16x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u32x16 {
	struct { simd_typed::u32x16 x, y, z; };
	simd_typed::u32x16 d[3];
	forceinline v3u32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u32x16 operator+(v3u32x16 a, v3u32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x16 operator+(v3u32x16 a, v3u32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x16 operator+(v3u32 a, v3u32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u32x16 operator+(v3u32x16 a, u32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u32x16 operator+(u32 a, v3u32x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u32x16 operator-(v3u32x16 a, v3u32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x16 operator-(v3u32x16 a, v3u32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x16 operator-(v3u32 a, v3u32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u32x16 operator-(v3u32x16 a, u32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u32x16 operator-(u32 a, v3u32x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u32x16 operator*(v3u32x16 a, v3u32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x16 operator*(v3u32x16 a, v3u32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x16 operator*(v3u32 a, v3u32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u32x16 operator*(v3u32x16 a, u32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u32x16 operator*(u32 a, v3u32x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u32x16 operator/(v3u32x16 a, v3u32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x16 operator/(v3u32x16 a, v3u32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x16 operator/(v3u32 a, v3u32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u32x16 operator/(v3u32x16 a, u32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u32x16 operator/(u32 a, v3u32x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u32x16 operator^(v3u32x16 a, v3u32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x16 operator^(v3u32x16 a, v3u32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x16 operator^(v3u32 a, v3u32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u32x16 operator^(v3u32x16 a, u32 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u32x16 operator^(u32 a, v3u32x16 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u32x16 operator&(v3u32x16 a, v3u32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x16 operator&(v3u32x16 a, v3u32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x16 operator&(v3u32 a, v3u32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u32x16 operator&(v3u32x16 a, u32 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u32x16 operator&(u32 a, v3u32x16 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u32x16 operator|(v3u32x16 a, v3u32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x16 operator|(v3u32x16 a, v3u32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x16 operator|(v3u32 a, v3u32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u32x16 operator|(v3u32x16 a, u32 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u32x16 operator|(u32 a, v3u32x16 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u32x16 operator<<(v3u32x16 a, u32 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u32x16 operator>>(v3u32x16 a, u32 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s32x16() const;
	forceinline operator v3f32x16() const;
};

forceinline v3u32x16 V3u32x16(v3u32 a) { return {simd_typed::U32x16(a.x), simd_typed::U32x16(a.y), simd_typed::U32x16(a.z), }; }
forceinline simd_typed::u32x16 dot(v3u32x16 a, v3u32x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3u64x8 {
	struct { simd_typed::u64x8 x, y, z; };
	simd_typed::u64x8 d[3];
	forceinline v3u64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3u64x8 operator+(v3u64x8 a, v3u64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x8 operator+(v3u64x8 a, v3u64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x8 operator+(v3u64 a, v3u64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3u64x8 operator+(v3u64x8 a, u64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3u64x8 operator+(u64 a, v3u64x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3u64x8 operator-(v3u64x8 a, v3u64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x8 operator-(v3u64x8 a, v3u64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x8 operator-(v3u64 a, v3u64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3u64x8 operator-(v3u64x8 a, u64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3u64x8 operator-(u64 a, v3u64x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3u64x8 operator*(v3u64x8 a, v3u64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x8 operator*(v3u64x8 a, v3u64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x8 operator*(v3u64 a, v3u64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3u64x8 operator*(v3u64x8 a, u64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3u64x8 operator*(u64 a, v3u64x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3u64x8 operator/(v3u64x8 a, v3u64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x8 operator/(v3u64x8 a, v3u64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x8 operator/(v3u64 a, v3u64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3u64x8 operator/(v3u64x8 a, u64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3u64x8 operator/(u64 a, v3u64x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3u64x8 operator^(v3u64x8 a, v3u64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x8 operator^(v3u64x8 a, v3u64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x8 operator^(v3u64 a, v3u64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3u64x8 operator^(v3u64x8 a, u64 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3u64x8 operator^(u64 a, v3u64x8 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3u64x8 operator&(v3u64x8 a, v3u64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x8 operator&(v3u64x8 a, v3u64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x8 operator&(v3u64 a, v3u64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3u64x8 operator&(v3u64x8 a, u64 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3u64x8 operator&(u64 a, v3u64x8 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3u64x8 operator|(v3u64x8 a, v3u64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x8 operator|(v3u64x8 a, v3u64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x8 operator|(v3u64 a, v3u64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3u64x8 operator|(v3u64x8 a, u64 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3u64x8 operator|(u64 a, v3u64x8 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3u64x8 operator<<(v3u64x8 a, u64 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3u64x8 operator>>(v3u64x8 a, u64 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3s64x8() const;
	forceinline operator v3f64x8() const;
};

forceinline v3u64x8 V3u64x8(v3u64 a) { return {simd_typed::U64x8(a.x), simd_typed::U64x8(a.y), simd_typed::U64x8(a.z), }; }
forceinline simd_typed::u64x8 dot(v3u64x8 a, v3u64x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s8x64 {
	struct { simd_typed::s8x64 x, y, z; };
	simd_typed::s8x64 d[3];
	forceinline v3s8 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s8x64 operator+(v3s8x64 a, v3s8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x64 operator+(v3s8x64 a, v3s8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x64 operator+(v3s8 a, v3s8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s8x64 operator+(v3s8x64 a, s8 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s8x64 operator+(s8 a, v3s8x64 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s8x64 operator-(v3s8x64 a, v3s8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x64 operator-(v3s8x64 a, v3s8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x64 operator-(v3s8 a, v3s8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s8x64 operator-(v3s8x64 a, s8 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s8x64 operator-(s8 a, v3s8x64 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s8x64 operator*(v3s8x64 a, v3s8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x64 operator*(v3s8x64 a, v3s8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x64 operator*(v3s8 a, v3s8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s8x64 operator*(v3s8x64 a, s8 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s8x64 operator*(s8 a, v3s8x64 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s8x64 operator/(v3s8x64 a, v3s8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x64 operator/(v3s8x64 a, v3s8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x64 operator/(v3s8 a, v3s8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s8x64 operator/(v3s8x64 a, s8 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s8x64 operator/(s8 a, v3s8x64 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s8x64 operator^(v3s8x64 a, v3s8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x64 operator^(v3s8x64 a, v3s8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x64 operator^(v3s8 a, v3s8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s8x64 operator^(v3s8x64 a, s8 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s8x64 operator^(s8 a, v3s8x64 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s8x64 operator&(v3s8x64 a, v3s8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x64 operator&(v3s8x64 a, v3s8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x64 operator&(v3s8 a, v3s8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s8x64 operator&(v3s8x64 a, s8 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s8x64 operator&(s8 a, v3s8x64 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s8x64 operator|(v3s8x64 a, v3s8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x64 operator|(v3s8x64 a, v3s8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x64 operator|(v3s8 a, v3s8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s8x64 operator|(v3s8x64 a, s8 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s8x64 operator|(s8 a, v3s8x64 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline operator v3u8x64() const;
};

forceinline v3s8x64 V3s8x64(v3s8 a) { return {simd_typed::S8x64(a.x), simd_typed::S8x64(a.y), simd_typed::S8x64(a.z), }; }
forceinline simd_typed::s8x64 dot(v3s8x64 a, v3s8x64 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s16x32 {
	struct { simd_typed::s16x32 x, y, z; };
	simd_typed::s16x32 d[3];
	forceinline v3s16 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s16x32 operator+(v3s16x32 a, v3s16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x32 operator+(v3s16x32 a, v3s16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x32 operator+(v3s16 a, v3s16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s16x32 operator+(v3s16x32 a, s16 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s16x32 operator+(s16 a, v3s16x32 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s16x32 operator-(v3s16x32 a, v3s16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x32 operator-(v3s16x32 a, v3s16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x32 operator-(v3s16 a, v3s16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s16x32 operator-(v3s16x32 a, s16 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s16x32 operator-(s16 a, v3s16x32 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s16x32 operator*(v3s16x32 a, v3s16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x32 operator*(v3s16x32 a, v3s16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x32 operator*(v3s16 a, v3s16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s16x32 operator*(v3s16x32 a, s16 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s16x32 operator*(s16 a, v3s16x32 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s16x32 operator/(v3s16x32 a, v3s16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x32 operator/(v3s16x32 a, v3s16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x32 operator/(v3s16 a, v3s16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s16x32 operator/(v3s16x32 a, s16 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s16x32 operator/(s16 a, v3s16x32 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s16x32 operator^(v3s16x32 a, v3s16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x32 operator^(v3s16x32 a, v3s16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x32 operator^(v3s16 a, v3s16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s16x32 operator^(v3s16x32 a, s16 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s16x32 operator^(s16 a, v3s16x32 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s16x32 operator&(v3s16x32 a, v3s16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x32 operator&(v3s16x32 a, v3s16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x32 operator&(v3s16 a, v3s16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s16x32 operator&(v3s16x32 a, s16 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s16x32 operator&(s16 a, v3s16x32 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s16x32 operator|(v3s16x32 a, v3s16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x32 operator|(v3s16x32 a, v3s16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x32 operator|(v3s16 a, v3s16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s16x32 operator|(v3s16x32 a, s16 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s16x32 operator|(s16 a, v3s16x32 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s16x32 operator<<(v3s16x32 a, s16 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s16x32 operator>>(v3s16x32 a, s16 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u16x32() const;
};

forceinline v3s16x32 V3s16x32(v3s16 a) { return {simd_typed::S16x32(a.x), simd_typed::S16x32(a.y), simd_typed::S16x32(a.z), }; }
forceinline simd_typed::s16x32 dot(v3s16x32 a, v3s16x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s32x16 {
	struct { simd_typed::s32x16 x, y, z; };
	simd_typed::s32x16 d[3];
	forceinline v3s32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s32x16 operator+(v3s32x16 a, v3s32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x16 operator+(v3s32x16 a, v3s32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x16 operator+(v3s32 a, v3s32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s32x16 operator+(v3s32x16 a, s32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s32x16 operator+(s32 a, v3s32x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s32x16 operator-(v3s32x16 a, v3s32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x16 operator-(v3s32x16 a, v3s32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x16 operator-(v3s32 a, v3s32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s32x16 operator-(v3s32x16 a, s32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s32x16 operator-(s32 a, v3s32x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s32x16 operator*(v3s32x16 a, v3s32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x16 operator*(v3s32x16 a, v3s32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x16 operator*(v3s32 a, v3s32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s32x16 operator*(v3s32x16 a, s32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s32x16 operator*(s32 a, v3s32x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s32x16 operator/(v3s32x16 a, v3s32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x16 operator/(v3s32x16 a, v3s32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x16 operator/(v3s32 a, v3s32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s32x16 operator/(v3s32x16 a, s32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s32x16 operator/(s32 a, v3s32x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s32x16 operator^(v3s32x16 a, v3s32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x16 operator^(v3s32x16 a, v3s32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x16 operator^(v3s32 a, v3s32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s32x16 operator^(v3s32x16 a, s32 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s32x16 operator^(s32 a, v3s32x16 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s32x16 operator&(v3s32x16 a, v3s32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x16 operator&(v3s32x16 a, v3s32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x16 operator&(v3s32 a, v3s32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s32x16 operator&(v3s32x16 a, s32 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s32x16 operator&(s32 a, v3s32x16 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s32x16 operator|(v3s32x16 a, v3s32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x16 operator|(v3s32x16 a, v3s32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x16 operator|(v3s32 a, v3s32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s32x16 operator|(v3s32x16 a, s32 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s32x16 operator|(s32 a, v3s32x16 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s32x16 operator<<(v3s32x16 a, s32 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s32x16 operator>>(v3s32x16 a, s32 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u32x16() const;
	forceinline operator v3f32x16() const;
};

forceinline v3s32x16 V3s32x16(v3s32 a) { return {simd_typed::S32x16(a.x), simd_typed::S32x16(a.y), simd_typed::S32x16(a.z), }; }
forceinline simd_typed::s32x16 dot(v3s32x16 a, v3s32x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3s64x8 {
	struct { simd_typed::s64x8 x, y, z; };
	simd_typed::s64x8 d[3];
	forceinline v3s64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3s64x8 operator+(v3s64x8 a, v3s64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x8 operator+(v3s64x8 a, v3s64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x8 operator+(v3s64 a, v3s64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3s64x8 operator+(v3s64x8 a, s64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3s64x8 operator+(s64 a, v3s64x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3s64x8 operator-(v3s64x8 a, v3s64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x8 operator-(v3s64x8 a, v3s64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x8 operator-(v3s64 a, v3s64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3s64x8 operator-(v3s64x8 a, s64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3s64x8 operator-(s64 a, v3s64x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3s64x8 operator*(v3s64x8 a, v3s64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x8 operator*(v3s64x8 a, v3s64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x8 operator*(v3s64 a, v3s64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3s64x8 operator*(v3s64x8 a, s64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3s64x8 operator*(s64 a, v3s64x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3s64x8 operator/(v3s64x8 a, v3s64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x8 operator/(v3s64x8 a, v3s64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x8 operator/(v3s64 a, v3s64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3s64x8 operator/(v3s64x8 a, s64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3s64x8 operator/(s64 a, v3s64x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline friend v3s64x8 operator^(v3s64x8 a, v3s64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x8 operator^(v3s64x8 a, v3s64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x8 operator^(v3s64 a, v3s64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z};}
	forceinline friend v3s64x8 operator^(v3s64x8 a, s64 b) { return {a.x^b, a.y^b, a.z^b};}
	forceinline friend v3s64x8 operator^(s64 a, v3s64x8 b) { return {a^b.x, a^b.y, a^b.z};}
	forceinline friend v3s64x8 operator&(v3s64x8 a, v3s64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x8 operator&(v3s64x8 a, v3s64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x8 operator&(v3s64 a, v3s64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z};}
	forceinline friend v3s64x8 operator&(v3s64x8 a, s64 b) { return {a.x&b, a.y&b, a.z&b};}
	forceinline friend v3s64x8 operator&(s64 a, v3s64x8 b) { return {a&b.x, a&b.y, a&b.z};}
	forceinline friend v3s64x8 operator|(v3s64x8 a, v3s64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x8 operator|(v3s64x8 a, v3s64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x8 operator|(v3s64 a, v3s64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z};}
	forceinline friend v3s64x8 operator|(v3s64x8 a, s64 b) { return {a.x|b, a.y|b, a.z|b};}
	forceinline friend v3s64x8 operator|(s64 a, v3s64x8 b) { return {a|b.x, a|b.y, a|b.z};}
	forceinline friend v3s64x8 operator<<(v3s64x8 a, s64 b) { return {a.x<<b, a.y<<b, a.z<<b};}
	forceinline friend v3s64x8 operator>>(v3s64x8 a, s64 b) { return {a.x>>b, a.y>>b, a.z>>b};}
	forceinline operator v3u64x8() const;
	forceinline operator v3f64x8() const;
};

forceinline v3s64x8 V3s64x8(v3s64 a) { return {simd_typed::S64x8(a.x), simd_typed::S64x8(a.y), simd_typed::S64x8(a.z), }; }
forceinline simd_typed::s64x8 dot(v3s64x8 a, v3s64x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3f32x16 {
	struct { simd_typed::f32x16 x, y, z; };
	simd_typed::f32x16 d[3];
	forceinline v3f32 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3f32x16 operator+(v3f32x16 a, v3f32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x16 operator+(v3f32x16 a, v3f32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x16 operator+(v3f32 a, v3f32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f32x16 operator+(v3f32x16 a, f32 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3f32x16 operator+(f32 a, v3f32x16 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3f32x16 operator-(v3f32x16 a, v3f32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x16 operator-(v3f32x16 a, v3f32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x16 operator-(v3f32 a, v3f32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f32x16 operator-(v3f32x16 a, f32 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3f32x16 operator-(f32 a, v3f32x16 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3f32x16 operator*(v3f32x16 a, v3f32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x16 operator*(v3f32x16 a, v3f32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x16 operator*(v3f32 a, v3f32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f32x16 operator*(v3f32x16 a, f32 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3f32x16 operator*(f32 a, v3f32x16 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3f32x16 operator/(v3f32x16 a, v3f32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x16 operator/(v3f32x16 a, v3f32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x16 operator/(v3f32 a, v3f32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f32x16 operator/(v3f32x16 a, f32 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3f32x16 operator/(f32 a, v3f32x16 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline operator v3u32x16() const;
	forceinline operator v3s32x16() const;
};

forceinline v3f32x16 V3f32x16(v3f32 a) { return {simd_typed::F32x16(a.x), simd_typed::F32x16(a.y), simd_typed::F32x16(a.z), }; }
forceinline simd_typed::f32x16 dot(v3f32x16 a, v3f32x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

union v3f64x8 {
	struct { simd_typed::f64x8 x, y, z; };
	simd_typed::f64x8 d[3];
	forceinline v3f64 subvector(umm i) const { return {x[i], y[i], z[i], }; }
	forceinline friend v3f64x8 operator+(v3f64x8 a, v3f64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x8 operator+(v3f64x8 a, v3f64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x8 operator+(v3f64 a, v3f64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z};}
	forceinline friend v3f64x8 operator+(v3f64x8 a, f64 b) { return {a.x+b, a.y+b, a.z+b};}
	forceinline friend v3f64x8 operator+(f64 a, v3f64x8 b) { return {a+b.x, a+b.y, a+b.z};}
	forceinline friend v3f64x8 operator-(v3f64x8 a, v3f64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x8 operator-(v3f64x8 a, v3f64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x8 operator-(v3f64 a, v3f64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z};}
	forceinline friend v3f64x8 operator-(v3f64x8 a, f64 b) { return {a.x-b, a.y-b, a.z-b};}
	forceinline friend v3f64x8 operator-(f64 a, v3f64x8 b) { return {a-b.x, a-b.y, a-b.z};}
	forceinline friend v3f64x8 operator*(v3f64x8 a, v3f64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x8 operator*(v3f64x8 a, v3f64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x8 operator*(v3f64 a, v3f64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z};}
	forceinline friend v3f64x8 operator*(v3f64x8 a, f64 b) { return {a.x*b, a.y*b, a.z*b};}
	forceinline friend v3f64x8 operator*(f64 a, v3f64x8 b) { return {a*b.x, a*b.y, a*b.z};}
	forceinline friend v3f64x8 operator/(v3f64x8 a, v3f64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x8 operator/(v3f64x8 a, v3f64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x8 operator/(v3f64 a, v3f64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z};}
	forceinline friend v3f64x8 operator/(v3f64x8 a, f64 b) { return {a.x/b, a.y/b, a.z/b};}
	forceinline friend v3f64x8 operator/(f64 a, v3f64x8 b) { return {a/b.x, a/b.y, a/b.z};}
	forceinline operator v3u64x8() const;
	forceinline operator v3s64x8() const;
};

forceinline v3f64x8 V3f64x8(v3f64 a) { return {simd_typed::F64x8(a.x), simd_typed::F64x8(a.y), simd_typed::F64x8(a.z), }; }
forceinline simd_typed::f64x8 dot(v3f64x8 a, v3f64x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

forceinline v3u8x64::operator v3s8x64() const { return {(simd_typed::s8x64)x, (simd_typed::s8x64)y, (simd_typed::s8x64)z, }; }
forceinline v3u16x32::operator v3s16x32() const { return {(simd_typed::s16x32)x, (simd_typed::s16x32)y, (simd_typed::s16x32)z, }; }
forceinline v3u32x16::operator v3s32x16() const { return {(simd_typed::s32x16)x, (simd_typed::s32x16)y, (simd_typed::s32x16)z, }; }
forceinline v3u32x16::operator v3f32x16() const { return {(simd_typed::f32x16)x, (simd_typed::f32x16)y, (simd_typed::f32x16)z, }; }
forceinline v3u64x8::operator v3s64x8() const { return {(simd_typed::s64x8)x, (simd_typed::s64x8)y, (simd_typed::s64x8)z, }; }
forceinline v3u64x8::operator v3f64x8() const { return {(simd_typed::f64x8)x, (simd_typed::f64x8)y, (simd_typed::f64x8)z, }; }
forceinline v3s8x64::operator v3u8x64() const { return {(simd_typed::u8x64)x, (simd_typed::u8x64)y, (simd_typed::u8x64)z, }; }
forceinline v3s16x32::operator v3u16x32() const { return {(simd_typed::u16x32)x, (simd_typed::u16x32)y, (simd_typed::u16x32)z, }; }
forceinline v3s32x16::operator v3u32x16() const { return {(simd_typed::u32x16)x, (simd_typed::u32x16)y, (simd_typed::u32x16)z, }; }
forceinline v3s32x16::operator v3f32x16() const { return {(simd_typed::f32x16)x, (simd_typed::f32x16)y, (simd_typed::f32x16)z, }; }
forceinline v3s64x8::operator v3u64x8() const { return {(simd_typed::u64x8)x, (simd_typed::u64x8)y, (simd_typed::u64x8)z, }; }
forceinline v3s64x8::operator v3f64x8() const { return {(simd_typed::f64x8)x, (simd_typed::f64x8)y, (simd_typed::f64x8)z, }; }
forceinline v3f32x16::operator v3u32x16() const { return {(simd_typed::u32x16)x, (simd_typed::u32x16)y, (simd_typed::u32x16)z, }; }
forceinline v3f32x16::operator v3s32x16() const { return {(simd_typed::s32x16)x, (simd_typed::s32x16)y, (simd_typed::s32x16)z, }; }
forceinline v3f64x8::operator v3u64x8() const { return {(simd_typed::u64x8)x, (simd_typed::u64x8)y, (simd_typed::u64x8)z, }; }
forceinline v3f64x8::operator v3s64x8() const { return {(simd_typed::s64x8)x, (simd_typed::s64x8)y, (simd_typed::s64x8)z, }; }
union v4u8x64 {
	struct { simd_typed::u8x64 x, y, z, w; };
	simd_typed::u8x64 d[4];
	forceinline v4u8 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u8x64 operator+(v4u8x64 a, v4u8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x64 operator+(v4u8x64 a, v4u8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x64 operator+(v4u8 a, v4u8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u8x64 operator+(v4u8x64 a, u8 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u8x64 operator+(u8 a, v4u8x64 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u8x64 operator-(v4u8x64 a, v4u8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x64 operator-(v4u8x64 a, v4u8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x64 operator-(v4u8 a, v4u8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u8x64 operator-(v4u8x64 a, u8 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u8x64 operator-(u8 a, v4u8x64 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u8x64 operator*(v4u8x64 a, v4u8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x64 operator*(v4u8x64 a, v4u8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x64 operator*(v4u8 a, v4u8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u8x64 operator*(v4u8x64 a, u8 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u8x64 operator*(u8 a, v4u8x64 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u8x64 operator/(v4u8x64 a, v4u8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x64 operator/(v4u8x64 a, v4u8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x64 operator/(v4u8 a, v4u8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u8x64 operator/(v4u8x64 a, u8 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u8x64 operator/(u8 a, v4u8x64 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u8x64 operator^(v4u8x64 a, v4u8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x64 operator^(v4u8x64 a, v4u8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x64 operator^(v4u8 a, v4u8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u8x64 operator^(v4u8x64 a, u8 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u8x64 operator^(u8 a, v4u8x64 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u8x64 operator&(v4u8x64 a, v4u8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x64 operator&(v4u8x64 a, v4u8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x64 operator&(v4u8 a, v4u8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u8x64 operator&(v4u8x64 a, u8 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u8x64 operator&(u8 a, v4u8x64 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u8x64 operator|(v4u8x64 a, v4u8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x64 operator|(v4u8x64 a, v4u8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x64 operator|(v4u8 a, v4u8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u8x64 operator|(v4u8x64 a, u8 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u8x64 operator|(u8 a, v4u8x64 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline operator v4s8x64() const;
};

forceinline v4u8x64 V4u8x64(v4u8 a) { return {simd_typed::U8x64(a.x), simd_typed::U8x64(a.y), simd_typed::U8x64(a.z), simd_typed::U8x64(a.w), }; }
forceinline simd_typed::u8x64 dot(v4u8x64 a, v4u8x64 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u16x32 {
	struct { simd_typed::u16x32 x, y, z, w; };
	simd_typed::u16x32 d[4];
	forceinline v4u16 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u16x32 operator+(v4u16x32 a, v4u16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x32 operator+(v4u16x32 a, v4u16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x32 operator+(v4u16 a, v4u16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u16x32 operator+(v4u16x32 a, u16 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u16x32 operator+(u16 a, v4u16x32 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u16x32 operator-(v4u16x32 a, v4u16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x32 operator-(v4u16x32 a, v4u16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x32 operator-(v4u16 a, v4u16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u16x32 operator-(v4u16x32 a, u16 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u16x32 operator-(u16 a, v4u16x32 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u16x32 operator*(v4u16x32 a, v4u16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x32 operator*(v4u16x32 a, v4u16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x32 operator*(v4u16 a, v4u16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u16x32 operator*(v4u16x32 a, u16 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u16x32 operator*(u16 a, v4u16x32 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u16x32 operator/(v4u16x32 a, v4u16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x32 operator/(v4u16x32 a, v4u16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x32 operator/(v4u16 a, v4u16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u16x32 operator/(v4u16x32 a, u16 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u16x32 operator/(u16 a, v4u16x32 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u16x32 operator^(v4u16x32 a, v4u16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x32 operator^(v4u16x32 a, v4u16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x32 operator^(v4u16 a, v4u16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u16x32 operator^(v4u16x32 a, u16 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u16x32 operator^(u16 a, v4u16x32 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u16x32 operator&(v4u16x32 a, v4u16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x32 operator&(v4u16x32 a, v4u16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x32 operator&(v4u16 a, v4u16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u16x32 operator&(v4u16x32 a, u16 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u16x32 operator&(u16 a, v4u16x32 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u16x32 operator|(v4u16x32 a, v4u16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x32 operator|(v4u16x32 a, v4u16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x32 operator|(v4u16 a, v4u16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u16x32 operator|(v4u16x32 a, u16 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u16x32 operator|(u16 a, v4u16x32 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u16x32 operator<<(v4u16x32 a, u16 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u16x32 operator>>(v4u16x32 a, u16 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s16x32() const;
};

forceinline v4u16x32 V4u16x32(v4u16 a) { return {simd_typed::U16x32(a.x), simd_typed::U16x32(a.y), simd_typed::U16x32(a.z), simd_typed::U16x32(a.w), }; }
forceinline simd_typed::u16x32 dot(v4u16x32 a, v4u16x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u32x16 {
	struct { simd_typed::u32x16 x, y, z, w; };
	simd_typed::u32x16 d[4];
	forceinline v4u32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u32x16 operator+(v4u32x16 a, v4u32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x16 operator+(v4u32x16 a, v4u32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x16 operator+(v4u32 a, v4u32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u32x16 operator+(v4u32x16 a, u32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u32x16 operator+(u32 a, v4u32x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u32x16 operator-(v4u32x16 a, v4u32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x16 operator-(v4u32x16 a, v4u32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x16 operator-(v4u32 a, v4u32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u32x16 operator-(v4u32x16 a, u32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u32x16 operator-(u32 a, v4u32x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u32x16 operator*(v4u32x16 a, v4u32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x16 operator*(v4u32x16 a, v4u32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x16 operator*(v4u32 a, v4u32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u32x16 operator*(v4u32x16 a, u32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u32x16 operator*(u32 a, v4u32x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u32x16 operator/(v4u32x16 a, v4u32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x16 operator/(v4u32x16 a, v4u32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x16 operator/(v4u32 a, v4u32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u32x16 operator/(v4u32x16 a, u32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u32x16 operator/(u32 a, v4u32x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u32x16 operator^(v4u32x16 a, v4u32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x16 operator^(v4u32x16 a, v4u32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x16 operator^(v4u32 a, v4u32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u32x16 operator^(v4u32x16 a, u32 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u32x16 operator^(u32 a, v4u32x16 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u32x16 operator&(v4u32x16 a, v4u32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x16 operator&(v4u32x16 a, v4u32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x16 operator&(v4u32 a, v4u32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u32x16 operator&(v4u32x16 a, u32 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u32x16 operator&(u32 a, v4u32x16 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u32x16 operator|(v4u32x16 a, v4u32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x16 operator|(v4u32x16 a, v4u32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x16 operator|(v4u32 a, v4u32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u32x16 operator|(v4u32x16 a, u32 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u32x16 operator|(u32 a, v4u32x16 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u32x16 operator<<(v4u32x16 a, u32 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u32x16 operator>>(v4u32x16 a, u32 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s32x16() const;
	forceinline operator v4f32x16() const;
};

forceinline v4u32x16 V4u32x16(v4u32 a) { return {simd_typed::U32x16(a.x), simd_typed::U32x16(a.y), simd_typed::U32x16(a.z), simd_typed::U32x16(a.w), }; }
forceinline simd_typed::u32x16 dot(v4u32x16 a, v4u32x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4u64x8 {
	struct { simd_typed::u64x8 x, y, z, w; };
	simd_typed::u64x8 d[4];
	forceinline v4u64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4u64x8 operator+(v4u64x8 a, v4u64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x8 operator+(v4u64x8 a, v4u64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x8 operator+(v4u64 a, v4u64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4u64x8 operator+(v4u64x8 a, u64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4u64x8 operator+(u64 a, v4u64x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4u64x8 operator-(v4u64x8 a, v4u64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x8 operator-(v4u64x8 a, v4u64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x8 operator-(v4u64 a, v4u64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4u64x8 operator-(v4u64x8 a, u64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4u64x8 operator-(u64 a, v4u64x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4u64x8 operator*(v4u64x8 a, v4u64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x8 operator*(v4u64x8 a, v4u64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x8 operator*(v4u64 a, v4u64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4u64x8 operator*(v4u64x8 a, u64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4u64x8 operator*(u64 a, v4u64x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4u64x8 operator/(v4u64x8 a, v4u64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x8 operator/(v4u64x8 a, v4u64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x8 operator/(v4u64 a, v4u64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4u64x8 operator/(v4u64x8 a, u64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4u64x8 operator/(u64 a, v4u64x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4u64x8 operator^(v4u64x8 a, v4u64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x8 operator^(v4u64x8 a, v4u64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x8 operator^(v4u64 a, v4u64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4u64x8 operator^(v4u64x8 a, u64 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4u64x8 operator^(u64 a, v4u64x8 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4u64x8 operator&(v4u64x8 a, v4u64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x8 operator&(v4u64x8 a, v4u64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x8 operator&(v4u64 a, v4u64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4u64x8 operator&(v4u64x8 a, u64 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4u64x8 operator&(u64 a, v4u64x8 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4u64x8 operator|(v4u64x8 a, v4u64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x8 operator|(v4u64x8 a, v4u64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x8 operator|(v4u64 a, v4u64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4u64x8 operator|(v4u64x8 a, u64 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4u64x8 operator|(u64 a, v4u64x8 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4u64x8 operator<<(v4u64x8 a, u64 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4u64x8 operator>>(v4u64x8 a, u64 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4s64x8() const;
	forceinline operator v4f64x8() const;
};

forceinline v4u64x8 V4u64x8(v4u64 a) { return {simd_typed::U64x8(a.x), simd_typed::U64x8(a.y), simd_typed::U64x8(a.z), simd_typed::U64x8(a.w), }; }
forceinline simd_typed::u64x8 dot(v4u64x8 a, v4u64x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s8x64 {
	struct { simd_typed::s8x64 x, y, z, w; };
	simd_typed::s8x64 d[4];
	forceinline v4s8 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s8x64 operator+(v4s8x64 a, v4s8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x64 operator+(v4s8x64 a, v4s8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x64 operator+(v4s8 a, v4s8x64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s8x64 operator+(v4s8x64 a, s8 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s8x64 operator+(s8 a, v4s8x64 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s8x64 operator-(v4s8x64 a, v4s8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x64 operator-(v4s8x64 a, v4s8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x64 operator-(v4s8 a, v4s8x64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s8x64 operator-(v4s8x64 a, s8 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s8x64 operator-(s8 a, v4s8x64 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s8x64 operator*(v4s8x64 a, v4s8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x64 operator*(v4s8x64 a, v4s8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x64 operator*(v4s8 a, v4s8x64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s8x64 operator*(v4s8x64 a, s8 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s8x64 operator*(s8 a, v4s8x64 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s8x64 operator/(v4s8x64 a, v4s8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x64 operator/(v4s8x64 a, v4s8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x64 operator/(v4s8 a, v4s8x64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s8x64 operator/(v4s8x64 a, s8 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s8x64 operator/(s8 a, v4s8x64 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s8x64 operator^(v4s8x64 a, v4s8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x64 operator^(v4s8x64 a, v4s8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x64 operator^(v4s8 a, v4s8x64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s8x64 operator^(v4s8x64 a, s8 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s8x64 operator^(s8 a, v4s8x64 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s8x64 operator&(v4s8x64 a, v4s8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x64 operator&(v4s8x64 a, v4s8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x64 operator&(v4s8 a, v4s8x64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s8x64 operator&(v4s8x64 a, s8 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s8x64 operator&(s8 a, v4s8x64 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s8x64 operator|(v4s8x64 a, v4s8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x64 operator|(v4s8x64 a, v4s8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x64 operator|(v4s8 a, v4s8x64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s8x64 operator|(v4s8x64 a, s8 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s8x64 operator|(s8 a, v4s8x64 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline operator v4u8x64() const;
};

forceinline v4s8x64 V4s8x64(v4s8 a) { return {simd_typed::S8x64(a.x), simd_typed::S8x64(a.y), simd_typed::S8x64(a.z), simd_typed::S8x64(a.w), }; }
forceinline simd_typed::s8x64 dot(v4s8x64 a, v4s8x64 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s16x32 {
	struct { simd_typed::s16x32 x, y, z, w; };
	simd_typed::s16x32 d[4];
	forceinline v4s16 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s16x32 operator+(v4s16x32 a, v4s16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x32 operator+(v4s16x32 a, v4s16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x32 operator+(v4s16 a, v4s16x32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s16x32 operator+(v4s16x32 a, s16 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s16x32 operator+(s16 a, v4s16x32 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s16x32 operator-(v4s16x32 a, v4s16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x32 operator-(v4s16x32 a, v4s16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x32 operator-(v4s16 a, v4s16x32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s16x32 operator-(v4s16x32 a, s16 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s16x32 operator-(s16 a, v4s16x32 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s16x32 operator*(v4s16x32 a, v4s16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x32 operator*(v4s16x32 a, v4s16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x32 operator*(v4s16 a, v4s16x32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s16x32 operator*(v4s16x32 a, s16 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s16x32 operator*(s16 a, v4s16x32 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s16x32 operator/(v4s16x32 a, v4s16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x32 operator/(v4s16x32 a, v4s16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x32 operator/(v4s16 a, v4s16x32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s16x32 operator/(v4s16x32 a, s16 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s16x32 operator/(s16 a, v4s16x32 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s16x32 operator^(v4s16x32 a, v4s16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x32 operator^(v4s16x32 a, v4s16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x32 operator^(v4s16 a, v4s16x32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s16x32 operator^(v4s16x32 a, s16 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s16x32 operator^(s16 a, v4s16x32 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s16x32 operator&(v4s16x32 a, v4s16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x32 operator&(v4s16x32 a, v4s16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x32 operator&(v4s16 a, v4s16x32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s16x32 operator&(v4s16x32 a, s16 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s16x32 operator&(s16 a, v4s16x32 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s16x32 operator|(v4s16x32 a, v4s16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x32 operator|(v4s16x32 a, v4s16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x32 operator|(v4s16 a, v4s16x32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s16x32 operator|(v4s16x32 a, s16 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s16x32 operator|(s16 a, v4s16x32 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s16x32 operator<<(v4s16x32 a, s16 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s16x32 operator>>(v4s16x32 a, s16 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u16x32() const;
};

forceinline v4s16x32 V4s16x32(v4s16 a) { return {simd_typed::S16x32(a.x), simd_typed::S16x32(a.y), simd_typed::S16x32(a.z), simd_typed::S16x32(a.w), }; }
forceinline simd_typed::s16x32 dot(v4s16x32 a, v4s16x32 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s32x16 {
	struct { simd_typed::s32x16 x, y, z, w; };
	simd_typed::s32x16 d[4];
	forceinline v4s32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s32x16 operator+(v4s32x16 a, v4s32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x16 operator+(v4s32x16 a, v4s32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x16 operator+(v4s32 a, v4s32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s32x16 operator+(v4s32x16 a, s32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s32x16 operator+(s32 a, v4s32x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s32x16 operator-(v4s32x16 a, v4s32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x16 operator-(v4s32x16 a, v4s32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x16 operator-(v4s32 a, v4s32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s32x16 operator-(v4s32x16 a, s32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s32x16 operator-(s32 a, v4s32x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s32x16 operator*(v4s32x16 a, v4s32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x16 operator*(v4s32x16 a, v4s32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x16 operator*(v4s32 a, v4s32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s32x16 operator*(v4s32x16 a, s32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s32x16 operator*(s32 a, v4s32x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s32x16 operator/(v4s32x16 a, v4s32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x16 operator/(v4s32x16 a, v4s32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x16 operator/(v4s32 a, v4s32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s32x16 operator/(v4s32x16 a, s32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s32x16 operator/(s32 a, v4s32x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s32x16 operator^(v4s32x16 a, v4s32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x16 operator^(v4s32x16 a, v4s32 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x16 operator^(v4s32 a, v4s32x16 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s32x16 operator^(v4s32x16 a, s32 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s32x16 operator^(s32 a, v4s32x16 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s32x16 operator&(v4s32x16 a, v4s32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x16 operator&(v4s32x16 a, v4s32 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x16 operator&(v4s32 a, v4s32x16 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s32x16 operator&(v4s32x16 a, s32 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s32x16 operator&(s32 a, v4s32x16 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s32x16 operator|(v4s32x16 a, v4s32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x16 operator|(v4s32x16 a, v4s32 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x16 operator|(v4s32 a, v4s32x16 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s32x16 operator|(v4s32x16 a, s32 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s32x16 operator|(s32 a, v4s32x16 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s32x16 operator<<(v4s32x16 a, s32 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s32x16 operator>>(v4s32x16 a, s32 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u32x16() const;
	forceinline operator v4f32x16() const;
};

forceinline v4s32x16 V4s32x16(v4s32 a) { return {simd_typed::S32x16(a.x), simd_typed::S32x16(a.y), simd_typed::S32x16(a.z), simd_typed::S32x16(a.w), }; }
forceinline simd_typed::s32x16 dot(v4s32x16 a, v4s32x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4s64x8 {
	struct { simd_typed::s64x8 x, y, z, w; };
	simd_typed::s64x8 d[4];
	forceinline v4s64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4s64x8 operator+(v4s64x8 a, v4s64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x8 operator+(v4s64x8 a, v4s64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x8 operator+(v4s64 a, v4s64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4s64x8 operator+(v4s64x8 a, s64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4s64x8 operator+(s64 a, v4s64x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4s64x8 operator-(v4s64x8 a, v4s64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x8 operator-(v4s64x8 a, v4s64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x8 operator-(v4s64 a, v4s64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4s64x8 operator-(v4s64x8 a, s64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4s64x8 operator-(s64 a, v4s64x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4s64x8 operator*(v4s64x8 a, v4s64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x8 operator*(v4s64x8 a, v4s64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x8 operator*(v4s64 a, v4s64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4s64x8 operator*(v4s64x8 a, s64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4s64x8 operator*(s64 a, v4s64x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4s64x8 operator/(v4s64x8 a, v4s64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x8 operator/(v4s64x8 a, v4s64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x8 operator/(v4s64 a, v4s64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4s64x8 operator/(v4s64x8 a, s64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4s64x8 operator/(s64 a, v4s64x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline friend v4s64x8 operator^(v4s64x8 a, v4s64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x8 operator^(v4s64x8 a, v4s64 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x8 operator^(v4s64 a, v4s64x8 b) { return {a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w};}
	forceinline friend v4s64x8 operator^(v4s64x8 a, s64 b) { return {a.x^b, a.y^b, a.z^b, a.w^b};}
	forceinline friend v4s64x8 operator^(s64 a, v4s64x8 b) { return {a^b.x, a^b.y, a^b.z, a^b.w};}
	forceinline friend v4s64x8 operator&(v4s64x8 a, v4s64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x8 operator&(v4s64x8 a, v4s64 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x8 operator&(v4s64 a, v4s64x8 b) { return {a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w};}
	forceinline friend v4s64x8 operator&(v4s64x8 a, s64 b) { return {a.x&b, a.y&b, a.z&b, a.w&b};}
	forceinline friend v4s64x8 operator&(s64 a, v4s64x8 b) { return {a&b.x, a&b.y, a&b.z, a&b.w};}
	forceinline friend v4s64x8 operator|(v4s64x8 a, v4s64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x8 operator|(v4s64x8 a, v4s64 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x8 operator|(v4s64 a, v4s64x8 b) { return {a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w};}
	forceinline friend v4s64x8 operator|(v4s64x8 a, s64 b) { return {a.x|b, a.y|b, a.z|b, a.w|b};}
	forceinline friend v4s64x8 operator|(s64 a, v4s64x8 b) { return {a|b.x, a|b.y, a|b.z, a|b.w};}
	forceinline friend v4s64x8 operator<<(v4s64x8 a, s64 b) { return {a.x<<b, a.y<<b, a.z<<b, a.w<<b};}
	forceinline friend v4s64x8 operator>>(v4s64x8 a, s64 b) { return {a.x>>b, a.y>>b, a.z>>b, a.w>>b};}
	forceinline operator v4u64x8() const;
	forceinline operator v4f64x8() const;
};

forceinline v4s64x8 V4s64x8(v4s64 a) { return {simd_typed::S64x8(a.x), simd_typed::S64x8(a.y), simd_typed::S64x8(a.z), simd_typed::S64x8(a.w), }; }
forceinline simd_typed::s64x8 dot(v4s64x8 a, v4s64x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4f32x16 {
	struct { simd_typed::f32x16 x, y, z, w; };
	simd_typed::f32x16 d[4];
	forceinline v4f32 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4f32x16 operator+(v4f32x16 a, v4f32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x16 operator+(v4f32x16 a, v4f32 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x16 operator+(v4f32 a, v4f32x16 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f32x16 operator+(v4f32x16 a, f32 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4f32x16 operator+(f32 a, v4f32x16 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4f32x16 operator-(v4f32x16 a, v4f32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x16 operator-(v4f32x16 a, v4f32 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x16 operator-(v4f32 a, v4f32x16 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f32x16 operator-(v4f32x16 a, f32 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4f32x16 operator-(f32 a, v4f32x16 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4f32x16 operator*(v4f32x16 a, v4f32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x16 operator*(v4f32x16 a, v4f32 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x16 operator*(v4f32 a, v4f32x16 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f32x16 operator*(v4f32x16 a, f32 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4f32x16 operator*(f32 a, v4f32x16 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4f32x16 operator/(v4f32x16 a, v4f32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x16 operator/(v4f32x16 a, v4f32 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x16 operator/(v4f32 a, v4f32x16 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f32x16 operator/(v4f32x16 a, f32 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4f32x16 operator/(f32 a, v4f32x16 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline operator v4u32x16() const;
	forceinline operator v4s32x16() const;
};

forceinline v4f32x16 V4f32x16(v4f32 a) { return {simd_typed::F32x16(a.x), simd_typed::F32x16(a.y), simd_typed::F32x16(a.z), simd_typed::F32x16(a.w), }; }
forceinline simd_typed::f32x16 dot(v4f32x16 a, v4f32x16 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

union v4f64x8 {
	struct { simd_typed::f64x8 x, y, z, w; };
	simd_typed::f64x8 d[4];
	forceinline v4f64 subvector(umm i) const { return {x[i], y[i], z[i], w[i], }; }
	forceinline friend v4f64x8 operator+(v4f64x8 a, v4f64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x8 operator+(v4f64x8 a, v4f64 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x8 operator+(v4f64 a, v4f64x8 b) { return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
	forceinline friend v4f64x8 operator+(v4f64x8 a, f64 b) { return {a.x+b, a.y+b, a.z+b, a.w+b};}
	forceinline friend v4f64x8 operator+(f64 a, v4f64x8 b) { return {a+b.x, a+b.y, a+b.z, a+b.w};}
	forceinline friend v4f64x8 operator-(v4f64x8 a, v4f64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x8 operator-(v4f64x8 a, v4f64 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x8 operator-(v4f64 a, v4f64x8 b) { return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}
	forceinline friend v4f64x8 operator-(v4f64x8 a, f64 b) { return {a.x-b, a.y-b, a.z-b, a.w-b};}
	forceinline friend v4f64x8 operator-(f64 a, v4f64x8 b) { return {a-b.x, a-b.y, a-b.z, a-b.w};}
	forceinline friend v4f64x8 operator*(v4f64x8 a, v4f64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x8 operator*(v4f64x8 a, v4f64 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x8 operator*(v4f64 a, v4f64x8 b) { return {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};}
	forceinline friend v4f64x8 operator*(v4f64x8 a, f64 b) { return {a.x*b, a.y*b, a.z*b, a.w*b};}
	forceinline friend v4f64x8 operator*(f64 a, v4f64x8 b) { return {a*b.x, a*b.y, a*b.z, a*b.w};}
	forceinline friend v4f64x8 operator/(v4f64x8 a, v4f64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x8 operator/(v4f64x8 a, v4f64 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x8 operator/(v4f64 a, v4f64x8 b) { return {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};}
	forceinline friend v4f64x8 operator/(v4f64x8 a, f64 b) { return {a.x/b, a.y/b, a.z/b, a.w/b};}
	forceinline friend v4f64x8 operator/(f64 a, v4f64x8 b) { return {a/b.x, a/b.y, a/b.z, a/b.w};}
	forceinline operator v4u64x8() const;
	forceinline operator v4s64x8() const;
};

forceinline v4f64x8 V4f64x8(v4f64 a) { return {simd_typed::F64x8(a.x), simd_typed::F64x8(a.y), simd_typed::F64x8(a.z), simd_typed::F64x8(a.w), }; }
forceinline simd_typed::f64x8 dot(v4f64x8 a, v4f64x8 b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }

forceinline v4u8x64::operator v4s8x64() const { return {(simd_typed::s8x64)x, (simd_typed::s8x64)y, (simd_typed::s8x64)z, (simd_typed::s8x64)w, }; }
forceinline v4u16x32::operator v4s16x32() const { return {(simd_typed::s16x32)x, (simd_typed::s16x32)y, (simd_typed::s16x32)z, (simd_typed::s16x32)w, }; }
forceinline v4u32x16::operator v4s32x16() const { return {(simd_typed::s32x16)x, (simd_typed::s32x16)y, (simd_typed::s32x16)z, (simd_typed::s32x16)w, }; }
forceinline v4u32x16::operator v4f32x16() const { return {(simd_typed::f32x16)x, (simd_typed::f32x16)y, (simd_typed::f32x16)z, (simd_typed::f32x16)w, }; }
forceinline v4u64x8::operator v4s64x8() const { return {(simd_typed::s64x8)x, (simd_typed::s64x8)y, (simd_typed::s64x8)z, (simd_typed::s64x8)w, }; }
forceinline v4u64x8::operator v4f64x8() const { return {(simd_typed::f64x8)x, (simd_typed::f64x8)y, (simd_typed::f64x8)z, (simd_typed::f64x8)w, }; }
forceinline v4s8x64::operator v4u8x64() const { return {(simd_typed::u8x64)x, (simd_typed::u8x64)y, (simd_typed::u8x64)z, (simd_typed::u8x64)w, }; }
forceinline v4s16x32::operator v4u16x32() const { return {(simd_typed::u16x32)x, (simd_typed::u16x32)y, (simd_typed::u16x32)z, (simd_typed::u16x32)w, }; }
forceinline v4s32x16::operator v4u32x16() const { return {(simd_typed::u32x16)x, (simd_typed::u32x16)y, (simd_typed::u32x16)z, (simd_typed::u32x16)w, }; }
forceinline v4s32x16::operator v4f32x16() const { return {(simd_typed::f32x16)x, (simd_typed::f32x16)y, (simd_typed::f32x16)z, (simd_typed::f32x16)w, }; }
forceinline v4s64x8::operator v4u64x8() const { return {(simd_typed::u64x8)x, (simd_typed::u64x8)y, (simd_typed::u64x8)z, (simd_typed::u64x8)w, }; }
forceinline v4s64x8::operator v4f64x8() const { return {(simd_typed::f64x8)x, (simd_typed::f64x8)y, (simd_typed::f64x8)z, (simd_typed::f64x8)w, }; }
forceinline v4f32x16::operator v4u32x16() const { return {(simd_typed::u32x16)x, (simd_typed::u32x16)y, (simd_typed::u32x16)z, (simd_typed::u32x16)w, }; }
forceinline v4f32x16::operator v4s32x16() const { return {(simd_typed::s32x16)x, (simd_typed::s32x16)y, (simd_typed::s32x16)z, (simd_typed::s32x16)w, }; }
forceinline v4f64x8::operator v4u64x8() const { return {(simd_typed::u64x8)x, (simd_typed::u64x8)y, (simd_typed::u64x8)z, (simd_typed::u64x8)w, }; }
forceinline v4f64x8::operator v4s64x8() const { return {(simd_typed::s64x8)x, (simd_typed::s64x8)y, (simd_typed::s64x8)z, (simd_typed::s64x8)w, }; }

#endif

using v2sx8 = v2s32x8;
using v2ux8 = v2u32x8;
using v2fx8 = v2f32x8;
using v3sx8 = v3s32x8;
using v3ux8 = v3u32x8;
using v3fx8 = v3f32x8;
using v4sx8 = v4s32x8;
using v4ux8 = v4u32x8;
using v4fx8 = v4f32x8;
}
}
