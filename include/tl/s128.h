#pragma once
#include "common.h"

namespace tl {

#define fc forceinline constexpr

union s128 {
	u64 s[2];
	
	static fc s128 from(s8  x) { return { (u64)x, (u64)(x >> 7) }; }
	static fc s128 from(s16 x) { return { (u64)x, (u64)(x >> 15) }; }
	static fc s128 from(s32 x) { return { (u64)x, (u64)(x >> 31) }; }
	static fc s128 from(s64 x) { return { (u64)x, (u64)(x >> 63) }; }
	static fc s128 from(u8  x) { return { (u64)x, 0 }; }
	static fc s128 from(u16 x) { return { (u64)x, 0 }; }
	static fc s128 from(u32 x) { return { (u64)x, 0 }; }
	static fc s128 from(u64 x) { return { (u64)x, 0 }; }

	fc s128 &operator++() { return *this += {.s = {1}}; }
	fc s128 operator++(int) { s128 copy = *this; ++*this; return copy; }

	fc s128 operator~() const {
		return {
			~s[0],
			~s[1],
		};
	}
	fc s128 operator-() const {
		s128 result = ~*this;
		return ++result;
	}

	fc s128 operator|(s128 b) const {
		return {
			s[0] | b.s[0],
			s[1] | b.s[1],
		};
	}
	fc s128 operator&(s128 b) const {
		return {
			s[0] & b.s[0],
			s[1] & b.s[1],
		};
	}
	fc s128 operator^(s128 b) const {
		return {
			s[0] ^ b.s[0],
			s[1] ^ b.s[1],
		};
	}
	fc s128 operator<<(s128 b) const {
		u64 e = b.s[0] & 64;
		u64 i = b.s[0] & 63;

		s128 r;

		switch (e) {
			case 0: {
				r.s[0] = s[0] << i;
				r.s[1] = (s[1] << i) | (s[0] >> (64 - i));
				break;
			}
			case 1: {
				r.s[0] = 0;
				r.s[1] = s[0] << i;
				break;
			}
		}
		return r;
	}
	fc s128 operator>>(s128 b) const {
		u64 e = b.s[0] & 64;
		u64 i = b.s[0] & 63;

		s128 r;

		switch (e) {
			case 0: {
				r.s[0] = (s[0] >> i) | (s[1] << (64 - i));
				r.s[1] = (s64)s[1] >> i;
				break;
			}
			case 1: {
				r.s[0] = (s64)s[1] >> i;
				r.s[1] = b.s[1] & 0x8000'0000'0000'0000 ? -1 : 0;
				break;
			}
		}
		return r;
	}
	fc s128 operator+(s128 b) const {
		bool carry = false;
		add_carry(s[0], b.s[0], carry, &b.s[0], &carry);
		add_carry(s[1], b.s[1], carry, &b.s[1], &carry);
		return b;
	}
	fc s128 operator-(s128 b) const {
		bool carry = false;
		sub_borrow(s[0], b.s[0], carry, &b.s[0], &carry);
		sub_borrow(s[1], b.s[1], carry, &b.s[1], &carry);
		return b;
	}
	fc s128 operator*(u8 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 8; ++bit_index) {
			if (b & ((u8)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(u16 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 16; ++bit_index) {
			if (b & ((u16)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(u32 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 32; ++bit_index) {
			if (b & ((u32)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(u64 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 64; ++bit_index) {
			if (b & ((u64)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(s8 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 7; ++bit_index) {
			if (b & ((u8)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		if (b & 0x80) {
			for (u32 bit_index = 7; bit_index < 128; ++bit_index) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(s16 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 15; ++bit_index) {
			if (b & ((u16)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		if (b & 0x8000) {
			for (u32 bit_index = 15; bit_index < 128; ++bit_index) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(s32 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 31; ++bit_index) {
			if (b & ((u32)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		if (b & 0x80000000) {
			for (u32 bit_index = 31; bit_index < 128; ++bit_index) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(s64 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 63; ++bit_index) {
			if (b & ((u64)1 << bit_index)) {
				result += *this << bit_index;
			}
		}
		if (b & 0x8000000000000000) {
			for (u32 bit_index = 63; bit_index < 128; ++bit_index) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator*(s128 b) const {
		s128 result = {};
		for (u32 bit_index = 0; bit_index < 128; ++bit_index) {
			if (b.get_bit_unchecked(bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}
	fc s128 operator/(s128 b) const {
		s128 N = *this;
		s128 quotient = {};
		s128 remainder = {};
		for (u32 i = 127; i != ~(u32)0; --i) {
			remainder <<= 1;
			remainder.set_bit_unchecked(0, N.get_bit_unchecked(i));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit_unchecked(i, 1);
			}
		}
		return quotient;
	}
	fc s128 operator%(s128 b) const {
		s128 N = *this;
		s128 remainder = {};
		for (u32 i = 127; i != ~(u32)0; --i) {
			remainder <<= 1;
			remainder.set_bit_unchecked(0, N.get_bit_unchecked(i));
			if (remainder >= b) {
				remainder -= b;
			}
		}
		return remainder;
	}

	fc bool operator==(s128 b) const { return (s[0] == b.s[0]) & (s[1] == b.s[1]); }
	fc bool operator!=(s128 b) const { return (s[0] != b.s[0]) | (s[1] != b.s[1]); }

	fc bool operator>(s128 b) const {
		if (s[1] != b.s[1]) return (s64)s[1] > (s64)b.s[1];
		return s[0] > b.s[0];
	}
	fc bool operator<(s128 b) const {
		if (s[1] != b.s[1]) return (s64)s[1] < (s64)b.s[1];
		return s[0] < b.s[0];
	}
	fc bool operator>=(s128 b) const {
		if (s[1] != b.s[1]) return (s64)s[1] >= (s64)b.s[1];
		return s[0] >= b.s[0];
	}
	fc bool operator<=(s128 b) const {
		if (s[1] != b.s[1]) return (s64)s[1] <= (s64)b.s[1];
		return s[0] <= b.s[0];
	}
	
	fc bool get_bit_unchecked(u64 index) const {
		return ((u64 *)this)[index >> 6] & ((u64)1 << (index & 63));
	}
	fc void set_bit_unchecked(u64 index, bool value) {
		if (value) ((u64 *)this)[index >> 6] |=  ((u64)1 << (index & 63));
		else       ((u64 *)this)[index >> 6] &= ~((u64)1 << (index & 63));
	}

	fc s128 &operator^=(s128 b) { return *this = *this | b; }
	fc s128 &operator&=(s128 b) { return *this = *this | b; }
	fc s128 &operator|=(s128 b) { return *this = *this | b; }
	fc s128 &operator+=(s128 b) { return *this = *this + b; }
	fc s128 &operator-=(s128 b) { return *this = *this - b; }
	fc s128 &operator*=(s128 b) { return *this = *this * b; }
	fc s128 &operator/=(s128 b) { return *this = *this / b; }
	fc s128 &operator%=(s128 b) { return *this = *this / b; }

	fc explicit operator u8 () const { return s[0]; };
	fc explicit operator u16() const { return s[0]; };
	fc explicit operator u32() const { return s[0]; };
	fc explicit operator u64() const { return s[0]; };
	fc explicit operator s8 () const { return s[0]; };
	fc explicit operator s16() const { return s[0]; };
	fc explicit operator s32() const { return s[0]; };
	fc explicit operator s64() const { return s[0]; };
	fc explicit operator f32() const { return (f32)(f64)*this; };
	fc explicit operator f64() const { return (f64)(s64)s[1] * 18446744073709551616.0 + (f64)(u64)s[0]; };
	
	#define BOILERPLATE(op) \
		fc s128 operator op(u8  b) const { return *this op from(b); } \
		fc s128 operator op(u16 b) const { return *this op from(b); } \
		fc s128 operator op(u32 b) const { return *this op from(b); } \
		fc s128 operator op(u64 b) const { return *this op from(b); } \
		fc s128 operator op(s8  b) const { return *this op from(b); } \
		fc s128 operator op(s16 b) const { return *this op from(b); } \
		fc s128 operator op(s32 b) const { return *this op from(b); } \
		fc s128 operator op(s64 b) const { return *this op from(b); } \
		fc s128 &operator op##=(u8  b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(u16 b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(u32 b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(u64 b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(s8  b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(s16 b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(s32 b) { return *this = *this op from(b); } \
		fc s128 &operator op##=(s64 b) { return *this = *this op from(b); } \
	
	BOILERPLATE(+)
	BOILERPLATE(-)
	// BOILERPLATE(*)
	BOILERPLATE(/)
	BOILERPLATE(%)
	BOILERPLATE(^)
	BOILERPLATE(&)
	BOILERPLATE(|)
	BOILERPLATE(<<)
	BOILERPLATE(>>)
	
	#undef BOILERPLATE
	
	#define BOILERPLATE(op) \
		fc bool operator op(u8  b) const { return *this op from(b); } \
		fc bool operator op(u16 b) const { return *this op from(b); } \
		fc bool operator op(u32 b) const { return *this op from(b); } \
		fc bool operator op(u64 b) const { return *this op from(b); } \
		fc bool operator op(s8  b) const { return *this op from(b); } \
		fc bool operator op(s16 b) const { return *this op from(b); } \
		fc bool operator op(s32 b) const { return *this op from(b); } \
		fc bool operator op(s64 b) const { return *this op from(b); } \

	BOILERPLATE(==)
	BOILERPLATE(!=)
	BOILERPLATE(<)
	BOILERPLATE(>)
	BOILERPLATE(<=)
	BOILERPLATE(>=)
	
	#undef BOILERPLATE

};

template <> inline constexpr bool is_integer<s128> = true;
template <> inline constexpr bool is_integer_like<s128> = true;
template <> inline constexpr bool is_signed<s128> = true;

fc s128 S128(u64 val) {
	return s128::from(val);
}

template <> fc s128 convert(u8  val) { return s128::from(val); }
template <> fc s128 convert(u16 val) { return s128::from(val); }
template <> fc s128 convert(u32 val) { return s128::from(val); }
template <> fc s128 convert(u64 val) { return s128::from(val); }

#undef fc

}
