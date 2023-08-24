#pragma once
#include "system.h"

namespace tl {

template <class T>
inline constexpr T reverse_bits_8(T b) {
	b = (b >> 4) | (b << 4);
	b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2);
	b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1);
	return b;
}
template <class T>
inline constexpr T reverse_bits_16(T b) {
	b = (b >> 8) | (b << 8);
	b = ((b & 0xF0F0) >> 4) | ((b & 0x0F0F) << 4);
	b = ((b & 0xCCCC) >> 2) | ((b & 0x3333) << 2);
	b = ((b & 0xAAAA) >> 1) | ((b & 0x5555) << 1);
	return b;
}
template <class T>
inline constexpr T reverse_bits_32(T b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >>  8) | ((b & 0x00FF00FF) <<  8);
	b = ((b & 0xF0F0F0F0) >>  4) | ((b & 0x0F0F0F0F) <<  4);
	b = ((b & 0xCCCCCCCC) >>  2) | ((b & 0x33333333) <<  2);
	b = ((b & 0xAAAAAAAA) >>  1) | ((b & 0x55555555) <<  1);
	return b;
}
template <class T>
inline constexpr T reverse_bits_64(T b) {
	b = (b >> 32) | (b << 32);
	b = ((b & 0xFFFF0000FFFF0000) >> 16) | ((b & 0x0000FFFF0000FFFF) << 16);
	b = ((b & 0xFF00FF00FF00FF00) >>  8) | ((b & 0x00FF00FF00FF00FF) <<  8);
	b = ((b & 0xF0F0F0F0F0F0F0F0) >>  4) | ((b & 0x0F0F0F0F0F0F0F0F) <<  4);
	b = ((b & 0xCCCCCCCCCCCCCCCC) >>  2) | ((b & 0x3333333333333333) <<  2);
	b = ((b & 0xAAAAAAAAAAAAAAAA) >>  1) | ((b & 0x5555555555555555) <<  1);
	return b;
}

inline constexpr u16 reverse_bits(u16 b) { return reverse_bits_16(b); }
inline constexpr u32 reverse_bits(u32 b) { return reverse_bits_32(b); }
inline constexpr u64 reverse_bits(u64 b) { return reverse_bits_64(b); }

inline constexpr s8  reverse_bits(s8  b) { return (s8 )reverse_bits_8 ((u8 )b); }
inline constexpr s16 reverse_bits(s16 b) { return (s16)reverse_bits_16((u16)b); }
inline constexpr s32 reverse_bits(s32 b) { return (s32)reverse_bits_32((u32)b); }
inline constexpr s64 reverse_bits(s64 b) { return (s64)reverse_bits_64((u64)b); }

template <class T>
inline constexpr T reverse_bytes_16(T b) {
	return (b >> 8) | (b << 8);
}
template <class T>
inline constexpr T reverse_bytes_32(T b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >> 8) | ((b & 0x00FF00FF) << 8);
	return b;
}
template <class T>
inline constexpr T reverse_bytes_64(T b) {
	b = (b >> 32) | (b << 32);
	b = ((b & 0xFFFF0000FFFF0000) >> 16) | ((b & 0x0000FFFF0000FFFF) << 16);
	b = ((b & 0xFF00FF00FF00FF00) >>  8) | ((b & 0x00FF00FF00FF00FF) <<  8);
	return b;
}

inline constexpr u16 reverse_bytes(u16 b) { return reverse_bytes_16(b); }
inline constexpr u32 reverse_bytes(u32 b) { return reverse_bytes_32(b); }
inline constexpr u64 reverse_bytes(u64 b) { return reverse_bytes_64(b); }

}
