#pragma once
#include "common.h"

namespace tl {

#define fc forceinline constexpr

template <umm _bits>
struct SignedIntWithBits {
	inline static constexpr umm bits = _bits;

	using Part = u64;
	using SignedPart = std::make_signed_t<Part>;
	inline static constexpr umm bits_in_part = sizeof(Part) * 8;
	inline static constexpr umm part_count = (bits + bits_in_part - 1) / bits_in_part;

	// [0] is low
	// [part_count-1] is high
	u64 parts[part_count] = {};
	
	static fc SignedIntWithBits from_first_and_rest(Part first, Part rest) {
		SignedIntWithBits r;
		r.parts[0] = first;
		for (umm i = 1; i < part_count; ++i)
			r.parts[i] = rest;
		r.fix();
		return r;
	}
	static fc SignedIntWithBits from(s8  x) { return from_first_and_rest(x, x >> 7); }
	static fc SignedIntWithBits from(s16 x) { return from_first_and_rest(x, x >> 15); }
	static fc SignedIntWithBits from(s32 x) { return from_first_and_rest(x, x >> 31); }
	static fc SignedIntWithBits from(s64 x) { return from_first_and_rest(x, x >> 63); }
	static fc SignedIntWithBits from(u8  x) { return from_first_and_rest(x, 0); }
	static fc SignedIntWithBits from(u16 x) { return from_first_and_rest(x, 0); }
	static fc SignedIntWithBits from(u32 x) { return from_first_and_rest(x, 0); }
	static fc SignedIntWithBits from(u64 x) { return from_first_and_rest(x, 0); }
	
	explicit operator u8 () const { return (u8 ) parts[0]; }
	explicit operator u16() const { return (u16) parts[0]; }
	explicit operator u32() const { return (u32) parts[0]; }
	explicit operator u64() const { return (u64) parts[0]; }
	explicit operator s8 () const { return (s8 ) parts[0]; }
	explicit operator s16() const { return (s16) parts[0]; }
	explicit operator s32() const { return (s32) parts[0]; }
	explicit operator s64() const { return (s64) parts[0]; }
	explicit operator f64() const {
		bool neg = false;
		SignedIntWithBits a = *this;
		if (a < 0) {
			a = -a;
			neg = true;
		}

		f64 r = 0;
		for (umm i = 0; i < part_count; ++i) {
			r = r * 18446744073709551616.0 + (f64)parts[i];
		}
		if (neg)
			r = -r;
		return r;
	}

	fc bool sign() const {
		return get_bit_unchecked(bits - 1);
	}
	fc bool get_bit_unchecked(umm i) const {
		return parts[i / bits_in_part] & ((Part)1 << (i % bits_in_part));
	}
	fc void set_bit_unchecked(umm i, bool bit) {
		Part mask = (Part)1 << (i % bits_in_part);
		if (bit) parts[i / bits_in_part] |=  mask;
		else     parts[i / bits_in_part] &= ~mask;
	}
	fc void fix() {
		constexpr umm unused_bits = bits_in_part - bits % bits_in_part;
		if constexpr (unused_bits) {
			parts[part_count-1] = ((s64)parts[part_count-1] << unused_bits) >> unused_bits;
		}
	}

	fc SignedIntWithBits operator~() const {
		SignedIntWithBits r;
		for (umm i = 0; i < part_count; ++i)
			r.parts[i] = ~parts[i];
		return r;
	}
	fc SignedIntWithBits operator-() const {
		return ~*this + 1;
	}
	fc SignedIntWithBits &operator^=(SignedIntWithBits that) {
		for (umm i = 0; i < part_count; ++i)
			parts[i] ^= that.parts[i];
		fix();
		return *this;
	}
	fc SignedIntWithBits &operator&=(SignedIntWithBits that) {
		for (umm i = 0; i < part_count; ++i)
			parts[i] &= that.parts[i];
		fix();
		return *this;
	}
	fc SignedIntWithBits &operator|=(SignedIntWithBits that) {
		for (umm i = 0; i < part_count; ++i)
			parts[i] |= that.parts[i];
		fix();
		return *this;
	}
	fc SignedIntWithBits &operator<<=(SignedIntWithBits that) {
		umm g = (umm)that & (ceil_to_power_of_2(bits) - 1);

		u64 x = g / bits_in_part;
		u64 y = g % bits_in_part;

		smm i = part_count - 1;
		
		// construct part from two
		for (; i > x; --i)
			parts[i] = (parts[i - x] << y) | (parts[i - x - 1] >> (bits_in_part - y));
		
		// last part
		parts[i] = parts[i - x] << y;

		// pad with zeros
		for (--i; i >= 0; --i)
			parts[i] = 0;

		fix();
		return *this;
	}
	fc SignedIntWithBits &operator>>=(SignedIntWithBits that) {
		umm g = (umm)that & (ceil_to_power_of_2(bits) - 1);

		u64 x = g / bits_in_part;
		u64 y = g % bits_in_part;

		Part fill = sign() ? -1 : 0;

		smm i = 0;
		for (; i < part_count - x - 2; ++i) {
			parts[i] = (parts[i + x] >> y) | (parts[i + x + 1] << (bits_in_part - y));
		}
		parts[i] = (SignedPart)parts[i + x] >> y;

		for (++i; i < part_count; ++i) {
			parts[i] = fill;
		}
		return *this;
	}
	fc SignedIntWithBits &operator+=(SignedIntWithBits that) {
		bool carry = false;
		for (umm i = 0; i < part_count; ++i)
			add_carry(parts[i], that.parts[i], carry, &parts[i], &carry);
		fix();
		return *this;
	}
	fc SignedIntWithBits &operator-=(SignedIntWithBits that) {
		bool carry = false;
		for (umm i = 0; i < part_count; ++i)
			sub_borrow(parts[i], that.parts[i], carry, &parts[i], &carry);
		fix();
		return *this;
	}
	fc SignedIntWithBits operator*(SignedIntWithBits b) const {
		SignedIntWithBits result = {};
		for (u32 bit_index = 0; bit_index < 128; ++bit_index) {
			if (b.get_bit_unchecked(bit_index)) {
				result += *this << bit_index;
			}
		}
		result.fix();
		return result;
	}
	static fc Array<SignedIntWithBits, 2> divmod(SignedIntWithBits a, SignedIntWithBits b) {
		SignedIntWithBits quotient = {};
		SignedIntWithBits remainder = {};
		for (umm i = bits - 1; i != (umm)-1; --i) {
			remainder <<= 1;
			remainder.set_bit_unchecked(0, a.get_bit_unchecked(i));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit_unchecked(i, 1);
			}
		}
		quotient.fix();
		remainder.fix();
		return {quotient, remainder};
	}
	fc SignedIntWithBits operator/(SignedIntWithBits b) const {
		return divmod(*this, b)[0];
	}
	fc SignedIntWithBits operator%(SignedIntWithBits b) const {
		return divmod(*this, b)[1];
	}
		
	fc bool operator==(SignedIntWithBits that) const {
		bool r = true;
		for (umm i = 0; i < part_count; ++i)
			r &= parts[i] == that.parts[i];
		return r;
	}
	fc bool operator<(SignedIntWithBits that) const {
		if (parts[part_count-1] != that.parts[part_count-1])
			return (SignedPart)parts[part_count-1] < (SignedPart)that.parts[part_count-1];
		for (smm i = part_count - 1; i >= 0; --i)
			if (parts[i] != that.parts[i])
				return parts[i] < that.parts[i];
		return false;
	}
	fc bool operator>(SignedIntWithBits that) const {
		if (parts[part_count-1] != that.parts[part_count-1])
			return (SignedPart)parts[part_count-1] > (SignedPart)that.parts[part_count-1];
		for (smm i = part_count - 1; i >= 0; --i)
			if (parts[i] != that.parts[i])
				return parts[i] > that.parts[i];
		return false;
	}
	fc bool operator!=(SignedIntWithBits that) const { return !(*this == that); }
	fc bool operator<=(SignedIntWithBits that) const { return !(*this > that); }
	fc bool operator>=(SignedIntWithBits that) const { return !(*this < that); }
	
	#define BP3(op) \
		fc auto operator op(u8  that) const { return *this op from(that); } \
		fc auto operator op(u16 that) const { return *this op from(that); } \
		fc auto operator op(u32 that) const { return *this op from(that); } \
		fc auto operator op(u64 that) const { return *this op from(that); } \
		fc auto operator op(s8  that) const { return *this op from(that); } \
		fc auto operator op(s16 that) const { return *this op from(that); } \
		fc auto operator op(s32 that) const { return *this op from(that); } \
		fc auto operator op(s64 that) const { return *this op from(that); } \
		fc friend auto operator op(u8  that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(u16 that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(u32 that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(u64 that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(s8  that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(s16 that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(s32 that, SignedIntWithBits thiss) { return from(that) op thiss; } \
		fc friend auto operator op(s64 that, SignedIntWithBits thiss) { return from(that) op thiss; } \

	#define BP1(op) \
		BP3(op) \
		fc SignedIntWithBits &operator op##=(u8  that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(u16 that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(u32 that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(u64 that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(s8  that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(s16 that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(s32 that) { return *this op##= from(that); } \
		fc SignedIntWithBits &operator op##=(s64 that) { return *this op##= from(that); } \
		fc SignedIntWithBits operator op(SignedIntWithBits that) const { SignedIntWithBits r = *this; return r op##= that; } \

	#define BP2(op) \
		BP3(op) \
		fc SignedIntWithBits &operator op##=(SignedIntWithBits that) { return *this = *this op that; } \
		fc SignedIntWithBits &operator op##=(u8  that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(u16 that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(u32 that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(u64 that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(s8  that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(s16 that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(s32 that) { return *this = *this op from(that); } \
		fc SignedIntWithBits &operator op##=(s64 that) { return *this = *this op from(that); } \
	
	BP1(^)
	BP1(&)
	BP1(|)
	BP1(<<)
	BP1(>>)
	BP1(+)
	BP1(-)
	BP2(*)
	BP2(/)
	BP2(%)
	BP3(==)
	BP3(!=)
	BP3(<)
	BP3(>)
	BP3(<=)
	BP3(>=)
		
#undef BP1
#undef BP2
#undef BP3
};

template <umm bits> inline constexpr bool is_integer<SignedIntWithBits<bits>> = true;
template <umm bits> inline constexpr bool is_integer_like<SignedIntWithBits<bits>> = true;
template <umm bits> inline constexpr bool is_signed<SignedIntWithBits<bits>> = true;

template <umm bits> 
fc SignedIntWithBits<bits> S128(u64 val) {
	return SignedIntWithBits<bits>::from(val);
}

template <umm bits> fc void convert(SignedIntWithBits<bits> &to, u8  from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, u16 from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, u32 from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, u64 from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, s8  from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, s16 from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, s32 from) { to = SignedIntWithBits<bits>::from(from); }
template <umm bits> fc void convert(SignedIntWithBits<bits> &to, s64 from) { to = SignedIntWithBits<bits>::from(from); }

#undef fc

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	{
		using s10 = SignedIntWithBits<10>;
		s10 a;
		s10 b;

		a = s10::from(511);
		b = s10::from(1);

		a += b;
		assert(a == -512);
	}
	{
		using s66 = SignedIntWithBits<66>;
		s66 a = s66::from(0x8000'0000'0000'0000);
		a *= 2;
		a /= 2;
		assert(a == 0x8000'0000'0000'0000);
		a *= 4; // after mul a should be negative due to overflow
		a /= 4;
		a >>= 2;
		assert(a == 0xe000'0000'0000'0000);
	}
};

#endif
