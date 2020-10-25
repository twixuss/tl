#pragma once
#include "common.h"
namespace TL {

struct xorshift32 {
	u32 v = 1;
};
struct xorshift64 {
	u64 v = 1;
};
struct xorshift128 {
	u32 a = 1;
	u32 b = 0;
	u32 c = 0;
	u32 d = 0;
};

u32 next(xorshift32 &x) {
	x.v ^= x.v << 13;
	x.v ^= x.v >> 17;
	x.v ^= x.v << 5;
	return x.v;
}
u64 next(xorshift64 &x) {
	x.v ^= x.v << 13;
	x.v ^= x.v >> 7;
	x.v ^= x.v << 17;
	return x.v;
}

u32 next(xorshift128 &x) {
	u32 t = x.d;
	u32 s = x.a;
	x.d = x.c;
	x.c = x.b;
	x.b = s;
	t ^= t << 11;
	t ^= t >> 8;
	return x.a = t ^ s ^ (s >> 19);
}
#if 0

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

#endif

}