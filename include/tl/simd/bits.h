#pragma once
#include "../math.h"

namespace TL {

forceinline u32x8 reverse_bits(u32x8 b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >>  8) | ((b & 0x00FF00FF) <<  8);
	b = ((b & 0xF0F0F0F0) >>  4) | ((b & 0x0F0F0F0F) <<  4);
	b = ((b & 0xCCCCCCCC) >>  2) | ((b & 0x33333333) <<  2);
	b = ((b & 0xAAAAAAAA) >>  1) | ((b & 0x55555555) <<  1);
	return b;
}
forceinline s32x8 reverse_bits(s32x8 b) { return (s32x8)reverse_bits((u32x8)b); }

forceinline u32x8 reverse_bytes(u32x8 b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >> 8) | ((b & 0x00FF00FF) << 8);
	return b;
}
forceinline s32x8 reverse_bytes(s32x8 b) { return (s32x8)reverse_bytes((u32x8)b); }

}
