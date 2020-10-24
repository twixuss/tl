#pragma once
#include "common.h"
namespace TL {

#if 0

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

#endif

}