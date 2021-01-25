#pragma once
#include "system.h"

namespace TL {

inline u8 reverse_bits(u8 b) {
	b = (u8)((b >> 4) | (b << 4));
	b = (u8)(((b & 0xCC) >> 2) | ((b & 0x33) << 2));
	b = (u8)(((b & 0xAA) >> 1) | ((b & 0x55) << 1));
	return b;
}
inline u16 reverse_bits(u16 b) {
	b = (u16)((b >> 8) | (b << 8));
	b = (u16)(((b & 0xF0F0) >> 4) | ((b & 0x0F0F) << 4));
	b = (u16)(((b & 0xCCCC) >> 2) | ((b & 0x3333) << 2));
	b = (u16)(((b & 0xAAAA) >> 1) | ((b & 0x5555) << 1));
	return b;
}
inline u32 reverse_bits(u32 b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >>  8) | ((b & 0x00FF00FF) <<  8);
	b = ((b & 0xF0F0F0F0) >>  4) | ((b & 0x0F0F0F0F) <<  4);
	b = ((b & 0xCCCCCCCC) >>  2) | ((b & 0x33333333) <<  2);
	b = ((b & 0xAAAAAAAA) >>  1) | ((b & 0x55555555) <<  1);
	return b;
}
inline u64 reverse_bits(u64 b) {
	b = (b >> 32) | (b << 32);
	b = ((b & 0xFFFF0000FFFF0000) >> 16) | ((b & 0x0000FFFF0000FFFF) << 16);
	b = ((b & 0xFF00FF00FF00FF00) >>  8) | ((b & 0x00FF00FF00FF00FF) <<  8);
	b = ((b & 0xF0F0F0F0F0F0F0F0) >>  4) | ((b & 0x0F0F0F0F0F0F0F0F) <<  4);
	b = ((b & 0xCCCCCCCCCCCCCCCC) >>  2) | ((b & 0x3333333333333333) <<  2);
	b = ((b & 0xAAAAAAAAAAAAAAAA) >>  1) | ((b & 0x5555555555555555) <<  1);
	return b;
}
inline u16 reverse_bytes(u16 b) {
	return (u16)((b >> 8) | (b << 8));
}
inline u32 reverse_bytes(u32 b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >> 8) | ((b & 0x00FF00FF) << 8);
	return b;
}
inline u64 reverse_bytes(u64 b) {
	b = (b >> 32) | (b << 32);
	b = ((b & 0xFFFF0000FFFF0000) >> 16) | ((b & 0x0000FFFF0000FFFF) << 16);
	b = ((b & 0xFF00FF00FF00FF00) >>  8) | ((b & 0x00FF00FF00FF00FF) <<  8);
	return b;
}

inline s8  reverse_bits(s8  b) { return (s8 )reverse_bits((u8 )b); }
inline s16 reverse_bits(s16 b) { return (s16)reverse_bits((u16)b); }
inline s32 reverse_bits(s32 b) { return (s32)reverse_bits((u32)b); }
inline s64 reverse_bits(s64 b) { return (s64)reverse_bits((u64)b); }

}
