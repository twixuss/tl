#pragma once
#include "common.h"
#include "math.h"

#include <xhash>

namespace std {

template <>
struct hash<tl::Span<char>> {
	tl::umm operator()(tl::Span<char> const &v) const {
		using namespace tl;
		umm hash = 0x0F1E2D3C4B5A6978;
		umm index = 0;
		for (auto c : v) {
			hash ^= (umm)c << (index & ((sizeof(umm) - 1) * 8));
		}
		return hash;
	}
};

template <>
struct hash<tl::List<char>> {
	tl::umm operator()(tl::List<char> const &v) const {
		using namespace tl;
		umm hash = 0x0F1E2D3C4B5A6978;
		umm index = 0;
		for (auto c : v) {
			hash ^= (umm)c << (index & ((sizeof(umm) - 1) * 8));
		}
		return hash;
	}
};

template <>
struct hash<tl::Span<tl::utf8>> {
	tl::umm operator()(tl::Span<tl::utf8> const &v) const {
		using namespace tl;
		umm hash = 0x0F1E2D3C4B5A6978;
		umm index = 0;
		for (auto c : v) {
			hash = tl::rotate_left(hash, 1) | c;
		}
		return hash;
	}
};

template <>
struct hash<tl::v3s> {
	tl::umm operator()(tl::v3s const &v) const {
		using namespace tl;
		umm hash = 0x0F1E2D3C4B5A6978;
		hash ^= (umm)v.x << 0;
		hash ^= (umm)v.y << 16;
		hash ^= (umm)v.z << 32;
		return hash;
	}
};

}
