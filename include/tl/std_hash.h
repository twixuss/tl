#pragma once
#include "common.h"
#include "math.h"

#include <xhash>

namespace std {

template <>
struct hash<TL::Span<char>> {
	TL::umm operator()(TL::Span<char> const &v) const {
		using namespace TL;
		umm hash = 0x0F1E2D3C4B5A6978;
		umm index = 0;
		for (auto c : v) {
			hash ^= (umm)c << (index & ((sizeof(umm) - 1) * 8));
		}
		return hash;
	}
};

template <>
struct hash<TL::Span<utf8>> {
	TL::umm operator()(TL::Span<utf8> const &v) const {
		using namespace TL;
		umm hash = 0x0F1E2D3C4B5A6978;
		umm index = 0;
		for (auto c : v) {
			hash = TL::rotate_left(hash, 1) | c;
		}
		return hash;
	}
};

template <>
struct hash<TL::v3s> {
	TL::umm operator()(TL::v3s const &v) const {
		using namespace TL;
		umm hash = 0x0F1E2D3C4B5A6978;
		hash ^= (umm)v.x << 0;
		hash ^= (umm)v.y << 16;
		hash ^= (umm)v.z << 32;
		return hash;
	}
};

}
