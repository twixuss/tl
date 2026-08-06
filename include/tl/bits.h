#pragma once
#include "system.h"

namespace tl {

template <class T>
inline constexpr T reverse_bytes_16(T b) {
	return (b >> 8) | (b << 8);
}
template <class T>
inline constexpr T reverse_bytes_32(T b) {
	b = (b >> 16) | (b << 16);
	b = ((b & 0xFF00FF00) >> 8) | ((b << 8) & 0xFF00FF00);
	return b;
}
template <class T>
inline constexpr T reverse_bytes_64(T b) {
	b = (b >> 32) | (b << 32);
	b = ((b & 0xFFFF0000FFFF0000) >> 16) | ((b << 16) & 0xFFFF0000FFFF0000);
	b = ((b & 0xFF00FF00FF00FF00) >>  8) | ((b <<  8) & 0xFF00FF00FF00FF00);
	return b;
}

template <class T>
inline constexpr T reverse_nibbles_8(T b) {
	b = (b >> 4) | (b << 4);
	return b;
}
template <class T>
inline constexpr T reverse_nibbles_16(T b) {
	b = reverse_bytes_16(b);
	b = ((b & 0xF0F0) >> 4) | ((b << 4) & 0xF0F0);
	return b;
}
template <class T>
inline constexpr T reverse_nibbles_32(T b) {
	b = reverse_bytes_32(b);
	b = ((b & 0xF0F0F0F0) >> 4) | ((b << 4) & 0xF0F0F0F0);
	return b;
}
template <class T>
inline constexpr T reverse_nibbles_64(T b) {
	b = reverse_bytes_64(b);
	b = ((b & 0xF0F0F0F0F0F0F0F0) >>  4) | ((b <<  4) & 0xF0F0F0F0F0F0F0F0);
	return b;
}

template <class T>
inline constexpr T reverse_bit_pairs_8(T b) {
	b = reverse_nibbles_8(b);
	b = ((b & 0xCC) >> 2) | ((b << 2) & 0xCC);
	return b;
}
template <class T>
inline constexpr T reverse_bit_pairs_16(T b) {
	b = reverse_nibbles_16(b);
	b = ((b & 0xCCCC) >> 2) | ((b << 2) & 0xCCCC);
	return b;
}
template <class T>
inline constexpr T reverse_bit_pairs_32(T b) {
	b = reverse_nibbles_32(b);
	b = ((b & 0xCCCCCCCC) >> 2) | ((b << 2) & 0xCCCCCCCC);
	return b;
}
template <class T>
inline constexpr T reverse_bit_pairs_64(T b) {
	b = reverse_nibbles_64(b);
	b = ((b & 0xCCCCCCCCCCCCCCCC) >>  2) | ((b <<  2) & 0xCCCCCCCCCCCCCCCC);
	return b;
}

template <class T>
inline constexpr T reverse_bits_8(T b) {
	b = reverse_bit_pairs_8(b);
	b = ((b & 0xAA) >> 1) | ((b << 1) & 0xAA);
	return b;
}
template <class T>
inline constexpr T reverse_bits_16(T b) {
	b = reverse_bit_pairs_16(b);
	b = ((b & 0xAAAA) >> 1) | ((b << 1) & 0xAAAA);
	return b;
}
template <class T>
inline constexpr T reverse_bits_32(T b) {
	b = reverse_bit_pairs_32(b);
	b = ((b & 0xAAAAAAAA) >> 1) | ((b << 1) & 0xAAAAAAAA);
	return b;
}
template <class T>
inline constexpr T reverse_bits_64(T b) {
	b = reverse_bit_pairs_64(b);
	b = ((b & 0xAAAAAAAAAAAAAAAA) >>  1) | ((b <<  1) & 0xAAAAAAAAAAAAAAAA);
	return b;
}

forceinline constexpr u16 reverse_bytes(u16 b) { return reverse_bytes_16(b); }
forceinline constexpr u32 reverse_bytes(u32 b) { return reverse_bytes_32(b); }
forceinline constexpr u64 reverse_bytes(u64 b) { return reverse_bytes_64(b); }

forceinline constexpr s16 reverse_bytes(s16 b) { return (s16)reverse_bytes_16((u16)b); }
forceinline constexpr s32 reverse_bytes(s32 b) { return (s32)reverse_bytes_32((u32)b); }
forceinline constexpr s64 reverse_bytes(s64 b) { return (s64)reverse_bytes_64((u64)b); }

forceinline constexpr u8  reverse_nibbles(u8  b) { return reverse_nibbles_8 (b); }
forceinline constexpr u16 reverse_nibbles(u16 b) { return reverse_nibbles_16(b); }
forceinline constexpr u32 reverse_nibbles(u32 b) { return reverse_nibbles_32(b); }
forceinline constexpr u64 reverse_nibbles(u64 b) { return reverse_nibbles_64(b); }

forceinline constexpr s8  reverse_nibbles(s8  b) { return (s8 )reverse_nibbles_8 ((u8 )b); }
forceinline constexpr s16 reverse_nibbles(s16 b) { return (s16)reverse_nibbles_16((u16)b); }
forceinline constexpr s32 reverse_nibbles(s32 b) { return (s32)reverse_nibbles_32((u32)b); }
forceinline constexpr s64 reverse_nibbles(s64 b) { return (s64)reverse_nibbles_64((u64)b); }

forceinline constexpr u8  reverse_bit_pairs(u8  b) { return reverse_bit_pairs_8 (b); }
forceinline constexpr u16 reverse_bit_pairs(u16 b) { return reverse_bit_pairs_16(b); }
forceinline constexpr u32 reverse_bit_pairs(u32 b) { return reverse_bit_pairs_32(b); }
forceinline constexpr u64 reverse_bit_pairs(u64 b) { return reverse_bit_pairs_64(b); }

forceinline constexpr s8  reverse_bit_pairs(s8  b) { return (s8 )reverse_bit_pairs_8 ((u8 )b); }
forceinline constexpr s16 reverse_bit_pairs(s16 b) { return (s16)reverse_bit_pairs_16((u16)b); }
forceinline constexpr s32 reverse_bit_pairs(s32 b) { return (s32)reverse_bit_pairs_32((u32)b); }
forceinline constexpr s64 reverse_bit_pairs(s64 b) { return (s64)reverse_bit_pairs_64((u64)b); }

forceinline constexpr u8  reverse_bits(u8  b) { return reverse_bits_8 (b); }
forceinline constexpr u16 reverse_bits(u16 b) { return reverse_bits_16(b); }
forceinline constexpr u32 reverse_bits(u32 b) { return reverse_bits_32(b); }
forceinline constexpr u64 reverse_bits(u64 b) { return reverse_bits_64(b); }

forceinline constexpr s8  reverse_bits(s8  b) { return (s8 )reverse_bits_8 ((u8 )b); }
forceinline constexpr s16 reverse_bits(s16 b) { return (s16)reverse_bits_16((u16)b); }
forceinline constexpr s32 reverse_bits(s32 b) { return (s32)reverse_bits_32((u32)b); }
forceinline constexpr s64 reverse_bits(s64 b) { return (s64)reverse_bits_64((u64)b); }

}
