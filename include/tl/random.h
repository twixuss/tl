#pragma once
#include "common.h"
namespace TL {

struct xorshift32 {
	u32 v = 1;
};
u32 next(xorshift32 &state) {
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	state.v ^= state.v << 13;
	state.v ^= state.v >> 17;
	state.v ^= state.v << 5;
	return state.v;
}

struct xorshift64 {
	u64 v = 1;
};
u64 next(xorshift64 &state) {
	state.v ^= state.v << 13;
	state.v ^= state.v >> 7;
	state.v ^= state.v << 17;
	return state.v;
}

struct xorshift128 {
	u32 a = 1;
	u32 b = 1;
	u32 c = 1;
	u32 d = 1;
};
u32 next(xorshift128 &state) {
	/* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
	u32 t = state.d;
	u32 s = state.a;
	state.d = state.c;
	state.c = state.b;
	state.b = s;
	t ^= t << 11;
	t ^= t >> 8;
	return state.a = t ^ s ^ (s >> 19);
}

struct xorwow {
    u32 a = 1;
	u32 b = 1;
	u32 c = 1;
	u32 d = 1;
	u32 e = 1;
    u32 counter = 0;
};
u32 next(xorwow &state) {
    /* Algorithm "xorwow" from p. 5 of Marsaglia, "Xorshift RNGs" */
    u32 t = state.e;
    u32 s = state.a;
    state.e = state.d;
    state.d = state.c;
    state.c = state.b;
    state.b = s;
    t ^= t >> 2;
    t ^= t << 1;
    t ^= s ^ (s << 4);
    state.a = t;
    state.counter += 362437;
    return t + state.counter;
}

struct xorshift64s {
	u64 a = 1;
};
u32 next(xorshift64s &state) {
	u64 x = state.a;
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	state.a = x;
	return x * (u64)0x2545F4914F6CDD1D;
}

struct xorshift1024s {
	u64 array[16] = { 1 };
	u64 index = 0;
};
u64 next(xorshift1024s &state) {
	u32 index = state.index;
	u64 s = state.array[index++];
	u64 t = state.array[index &= 15];
	t ^= t << 31;
	t ^= t >> 11;
	t ^= s ^ (s >> 30);
	state.array[index] = t;
	state.index = index;
	return t * (u64)0x106689D45497FDB5;
}

struct xorshift128p {
	u64 a = 1;
	u64 b = 1;
};
u64 next(xorshift128p &state) {
	u64 t = state.a;
	u64 s = state.b;
	state.a = s;
	t ^= t << 23;
	t ^= t >> 17;
	t ^= s ^ (s >> 26);
	state.b = t;
	return t + s;
}

struct xoshiro256ss {
	u64 s[4] = {};
};
u64 next(xoshiro256ss &state) {
	u64 *s = state.s;
	u64 result = rotateLeft(s[1] * 5, 7) * 9;
	u64 t = s[1] << 17;
	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];
	s[2] ^= t;
	s[3] = rotateLeft(s[3], 45);
	return result;
}

struct xoshiro256p {
	u64 s[4] = {};
};
u64 next(xoshiro256p &state) {
	u64 *s = state.s;
	u64 result = s[0] + s[3];
	u64 t = s[1] << 17;
	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];
	s[2] ^= t;
	s[3] = rotateLeft(s[3], 45);
	return result;
}

struct splitmix64 {
	u64 s;
};
u64 next(splitmix64 &state) {
	u64 result = state.s;
	state.s = result + 0x9E3779B97f4A7C15;
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

}