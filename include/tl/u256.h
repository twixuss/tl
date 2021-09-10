#pragma once
#include "common.h"

namespace tl {

union u256 {
	u64 s[4];

	u256 &operator++() { return *this += {.s = {1}}; }
	u256 operator++(int) { u256 copy = *this; ++*this; return copy; }

	u256 operator~() const {
		return {
			~s[0],
			~s[1],
			~s[2],
			~s[3],
		};
	}
	u256 operator-() const {
		u256 result = ~*this;
		return ++result;
	}

	u256 operator|(u256 b) const {
		return {
			s[0] | b.s[0],
			s[1] | b.s[1],
			s[2] | b.s[2],
			s[3] | b.s[3],
		};
	}
	u256 operator&(u256 b) const {
		return {
			s[0] & b.s[0],
			s[1] & b.s[1],
			s[2] & b.s[2],
			s[3] & b.s[3],
		};
	}
	u256 operator<<(u32 b) const {
		u256 result = {};
		switch (b) {
			case 0: return *this;
			case 64: {
				result.s[0] = 0;
				result.s[1] = s[0];
				result.s[2] = s[1];
				result.s[3] = s[2];
				break;
			}
			case 128: {
				result.s[0] = 0;
				result.s[1] = 0;
				result.s[2] = s[0];
				result.s[3] = s[1];
				break;
			}
			case 192: {
				result.s[0] = 0;
				result.s[1] = 0;
				result.s[2] = 0;
				result.s[3] = s[0];
				break;
			}
			default: {
				if (b >= 256) {
					return {};
				} else if (b > 192) {
					result.s[0] = 0;
					result.s[1] = 0;
					result.s[2] = 0;
					result.s[3] = (s[0] << b);
				} else if (b > 128) {
					result.s[0] = 0;
					result.s[1] = 0;
					result.s[2] = (s[0] << b);
					result.s[3] = (s[1] << b) | (s[1] >> (64 - b));
				} else if (b > 64) {
					result.s[0] = 0;
					result.s[1] = (s[0] << b);
					result.s[2] = (s[1] << b) | (s[1] >> (64 - b));
					result.s[3] = (s[2] << b) | (s[2] >> (64 - b));
				} else {
					result.s[0] = s[0] << b;
					result.s[1] = (s[1] << b) | (s[0] >> (64 - b));
					result.s[2] = (s[2] << b) | (s[1] >> (64 - b));
					result.s[3] = (s[3] << b) | (s[2] >> (64 - b));
				}
				break;
			}
		}
		return result;
	}
	u256 operator+(u256 b) const {
		u256 result;
		bool carry;
		add_carry(s[0], b.s[0], &result.s[0], &carry);
		add_carry(s[1], b.s[1], carry, &result.s[1], &carry);
		add_carry(s[2], b.s[2], carry, &result.s[2], &carry);
		result.s[3] = s[3] + b.s[3] + carry;
		return result;
	}
	u256 operator-(u256 b) const {
		return *this + -b;
	}
	u256 operator*(u256 b) const {
		u256 result = {};
		for (u32 bit_index = 0; bit_index < 256; ++bit_index) {
			if (b.get_bit(bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	u256 operator*(u8 b) const {
		u256 result = {};
		for (u32 bit_index = 0; bit_index < 8; ++bit_index) {
			if (b & ((u8)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	u256 operator*(u16 b) const {
		u256 result = {};
		for (u32 bit_index = 0; bit_index < 16; ++bit_index) {
			if (b & ((u16)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	u256 operator*(u32 b) const {
		u256 result = {};
		for (u32 bit_index = 0; bit_index < 32; ++bit_index) {
			if (b & ((u32)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	u256 operator*(u64 b) const {
		u256 result = {};
		for (u32 bit_index = 0; bit_index < 64; ++bit_index) {
			if (b & ((u64)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	u256 operator/(u256 b) const {
		u256 N = *this;
		u256 quotient = {};
		u256 remainder = {};
		for (u32 i = 255; i != ~(u32)0; --i) {
			remainder <<= 1;
			remainder.set_bit(0, N.get_bit(i));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(i, 1);
			}
		}
		return quotient;
	}
	u256 operator%(u256 b) const {
		u256 N = *this;
		u256 remainder = {};
		for (u32 i = 255; i != ~(u32)0; --i) {
			remainder <<= 1;
			remainder.set_bit(0, N.get_bit(i));
			if (remainder >= b) {
				remainder -= b;
			}
		}
		return remainder;
	}

	bool operator==(u256 b) const { return s[0] == b.s[0] && s[1] == b.s[1] && s[2] == b.s[2] && s[3] == b.s[3]; }
	bool operator!=(u256 b) const { return s[0] != b.s[0] || s[1] != b.s[1] || s[2] != b.s[2] || s[3] != b.s[3]; }

	bool operator>(u256 b) const {
		if (s[3] != b.s[3]) return s[3] > b.s[3];
		if (s[2] != b.s[2]) return s[2] > b.s[2];
		if (s[1] != b.s[1]) return s[1] > b.s[1];
		return s[0] > b.s[0];
	}
	bool operator<(u256 b) const {
		if (s[3] != b.s[3]) return s[3] < b.s[3];
		if (s[2] != b.s[2]) return s[2] < b.s[2];
		if (s[1] != b.s[1]) return s[1] < b.s[1];
		return s[0] < b.s[0];
	}
	bool operator>=(u256 b) const {
		if (s[3] != b.s[3]) return s[3] >= b.s[3];
		if (s[2] != b.s[2]) return s[2] >= b.s[2];
		if (s[1] != b.s[1]) return s[1] >= b.s[1];
		return s[0] >= b.s[0];
	}
	bool operator<=(u256 b) const {
		if (s[3] != b.s[3]) return s[3] <= b.s[3];
		if (s[2] != b.s[2]) return s[2] <= b.s[2];
		if (s[1] != b.s[1]) return s[1] <= b.s[1];
		return s[0] <= b.s[0];
	}

	bool operator>(u64 b) const {
		if (s[3] != 0) return true;
		if (s[2] != 0) return true;
		if (s[1] != 0) return true;
		return s[0] > b;
	}

	bool get_bit(u64 index) const {
		return ((u64 *)this)[index >> 6] & ((u64)1 << (index & 63));
	}
	void set_bit(u64 index, bool value) {
		if (value) ((u64 *)this)[index >> 6] |=  ((u64)1 << (index & 63));
		else       ((u64 *)this)[index >> 6] &= ~((u64)1 << (index & 63));
	}

	u256 &operator|=(u256 b) { return *this = *this | b; }
	u256 &operator+=(u256 b) { return *this = *this + b; }
	u256 &operator-=(u256 b) { return *this = *this - b; }
	u256 &operator*=(u256 b) { return *this = *this * b; }
	u256 &operator/=(u256 b) { return *this = *this / b; }

	u256 &operator*=(u8  b) { return *this = *this * b; }
	u256 &operator*=(u16 b) { return *this = *this * b; }
	u256 &operator*=(u32 b) { return *this = *this * b; }
	u256 &operator*=(u64 b) { return *this = *this * b; }
	u256 &operator<<=(u32 b) { return *this = *this << b; }

	explicit operator u8 () const { return s[0]; };
	explicit operator u16() const { return s[0]; };
	explicit operator u32() const { return s[0]; };
	explicit operator u64() const { return s[0]; };
};

template <> inline static constexpr bool is_integer<u256> = true;
template <> inline static constexpr bool is_integer_like<u256> = true;
template <> inline static constexpr bool is_signed<u256> = false;

inline u256 U256(u64 val) {
	return {.s = {val}};
}

inline u256 operator""ou(u64 val) {
	return {.s = {val}};
}


template <> forceinline constexpr u256 convert(u8   val) { return {.s = {val}}; }
template <> forceinline constexpr u256 convert(u16  val) { return {.s = {val}}; }
template <> forceinline constexpr u256 convert(u32  val) { return {.s = {val}}; }
template <> forceinline constexpr u256 convert(u64  val) { return {.s = {val}}; }


}
