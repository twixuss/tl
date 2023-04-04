#pragma once
#include "list.h"
#include "string.h"

namespace tl {
namespace impl {

template <class List>
struct BigInt;

template <class List>
BigInt<List> copy(BigInt<List> that);

template <class List>
void free(BigInt<List> &a);

//
// Arbitrarily long, signed, two's complement integer.
// parts[0]               is the least significant part
// parts[parts.count - 1] is the most  significant part
//
// Note that `BigInt` must be normalized (some algorithms rely on this), i.e. most significant part should be removed if it is 'equal' to `msb` (most significant bit)
// For example, while last part is 0 and msb is false, remove last part.
// `parts` can be empty, meaning that the number is equal to 0 if msb is false, or -1 otherwise.
//
// Note that division by zero is not exceptional; you decide what to do in that case.
//
template <class List_>
struct BigInt {
	using List = List_;
	using Part = typename List::ElementType;
	using SignedPart = std::make_signed_t<typename List::ElementType>;
	using Size = typename List::Size;

	static_assert(is_unsigned<Part>, "BigInt::List::ElementType must be unsigned");

	inline static constexpr umm bytes_in_part = sizeof(Part);
	inline static constexpr umm bits_in_part = bytes_in_part * 8;

	bool msb = false;
	List parts;

	void normalize() {
		for (umm i = parts.count - 1; i != 0; --i) {
			if (parts[i] == (msb ? (Part)-1 : (Part)0)) {
				parts.pop();
			} else {
				break;
			}
		}
	}

	BigInt &invert() {
		msb = !msb;
		for (auto &part : parts)
			part = ~part;
		return *this;
	}

	BigInt operator~() const { return copy(*this).invert(); }

	BigInt &negate() { return invert() += (u64)1; }
	BigInt operator-() const { return copy(*this).negate(); }

	BigInt &operator^=(BigInt const &b) {
		for (umm i = 0; i < min(parts.count, b.parts.count); ++i) {
			parts[i] ^= b.parts[i];
		}

		if (b.msb) {
			for (umm i = b.parts.count; i < parts.count; ++i) {
				parts[i] = ~parts[i];
			}
		}

		for (umm i = parts.count; i < b.parts.count; ++i) {
			parts.add(msb ? ~b.parts[i] : b.parts[i]);
		}

		msb ^= b.msb;

		normalize();
		return *this;
	}
	BigInt &operator|=(SignedPart b) {
		if (parts.count == 0) {
			if (msb) {
				// already all ones, nothing to change
			} else {
				parts.add(b);
				msb = b < 0;
			}
		} else {
			parts.data[0] |= b;
			if (b < 0) {
				// parts.resize(1);
				// that's simpler.
				parts.count = 1;
			}
		}
		// no need to normalize
		return *this;
	}
	BigInt &operator|=(BigInt const &b) {
		if (b.msb) {
			if (b.parts.count < parts.count) {
				parts.resize(b.parts.count);
			}
		}

		for (umm i = 0; i < ((msb && b.msb) || (parts.count < b.parts.count) ? parts.count : b.parts.count); ++i) {
			parts[i] |= b.parts[i];
		}

		if (!msb) {
			if (b.parts.count >= parts.count) {
				for (umm i = parts.count; i < b.parts.count; ++i) {
					parts.add(b.parts[i]);
				}
			}
		}

		msb |= b.msb;

		normalize();
		return *this;
	}

	BigInt &operator&=(BigInt b) {
		if (!b.msb) {
			if (b.parts.count < parts.count) {
				parts.resize(b.parts.count);
			}
		}

		if (b.msb) {
			min(parts.count, b.parts.count);
		} else {
			parts.count;
		}

		for (umm i = 0; i < ((b.msb && b.parts.count < parts.count) ? b.parts.count : parts.count); ++i) {
			parts[i] &= b.parts[i];
		}

		if (msb) {
			for (umm i = parts.count; i < b.parts.count; ++i) {
				parts.add(b.parts[i]);
			}
		}
		msb &= b.msb;

		normalize();
		return *this;
	}

	template <class A, class B>
	bool shift_left(BigInt const &b, u64 bytes_threshold, A &&error_shift_by_negative, B &&error_too_big) {
		BigInt zero_part_count_big, shift_amount;
		b.divmod(bits_in_part, zero_part_count_big, shift_amount);
		if (zero_part_count_big.msb) {
			error_shift_by_negative();
			return false;
		}

		if (zero_part_count_big.parts.count > 1) {
			error_too_big(zero_part_count_big * bytes_in_part);
			return false;
		}

		if (zero_part_count_big.parts.count == 1) {
			if (zero_part_count_big.parts.data[0] > max_value<u64> / bytes_in_part) {
				error_too_big(zero_part_count_big * bytes_in_part);
				return false;
			}
			if (zero_part_count_big.parts.data[0] * bytes_in_part > bytes_threshold) {
				error_too_big(zero_part_count_big * bytes_in_part);
				return false;
			}
		}

		*this <<= (u64)shift_amount;

		parts.insert_n_at(0, 0, (u64)zero_part_count_big);
		return true;
	}

	BigInt &operator<<=(Part b) {
		parts.insert_n_at(0, 0, b >> 6);

		auto part_shift = b & (bits_in_part - 1);
		if (part_shift) {
			if (parts.count) {
				parts.resize(parts.count+1);
				parts.data[parts.count-1] = ((msb ? (Part)-1 : (Part)0) << part_shift) | (parts.data[parts.count-2] >> (bits_in_part - part_shift));
				for (umm part_index = parts.count-2; part_index != 0; --part_index) {
					// NOTE: there is shld instruction, which does 128 bit shift, but i didn't find an intrinsic for it.
					// And this is sad, because msvc can't optimize this...
					parts.data[part_index] = (parts.data[part_index] << part_shift) | (parts.data[part_index - 1] >> (bits_in_part - part_shift));
				}
				parts.data[0] <<= part_shift;
			}
		}

		normalize();
		return *this;
	}
	BigInt &operator<<=(BigInt const &b) {
		shift_left(b, max_value<u64>, []{}, [](auto){});
		return *this;
	}

	inline void shift_right_arithmetic(Part delete_part_count, Part shift_amount) {
		if (delete_part_count >= parts.count) {
			parts.clear();
		} else {
			//    ____    ____    ____
			//   1....1001101100000000 >> 6
			//      ____    ____
			// 1....111001101100

			parts.erase(parts.subspan(0, delete_part_count));

			if (shift_amount) {
				for (umm part_index = 1; part_index < parts.count-1; ++part_index) {
					parts.data[part_index-1] = (parts.data[part_index-1] >> shift_amount) | (parts.data[part_index] << (bits_in_part - shift_amount));
				}
				// NOTE: sign bit must be involved
				parts.data[parts.count - 1] = (SignedPart)parts.data[parts.count - 1] >> shift_amount;
			}
		}
	}
	BigInt &operator>>=(Part b) {
		if (!parts.count)
			return *this;

		shift_right_arithmetic(b / bits_in_part, b % bits_in_part);
		return *this;
	}
	BigInt &operator>>=(BigInt const &b) {
		if (!parts.count)
			return *this;

		BigInt delete_part_count, shift_amount_big;
		b.divmod(bits_in_part, delete_part_count, shift_amount_big);

		if (delete_part_count >= parts.count) {
			parts.clear();
		} else {
			shift_right_arithmetic((Part)delete_part_count, (Part)shift_amount_big);
		}
		return *this;
	}

	BigInt &operator+=(Part b) {
		if (parts.count == 0)
			parts.add(0);

		bool carry = false;

		add_carry(parts[0], b, carry, &parts[0], &carry);
		for (umm part_index = 1; part_index < parts.count; ++part_index) {
			add_carry(parts[part_index], 0, carry, &parts[part_index], &carry);
		}

		if (carry) {
			if (msb) {
				msb = false;
			} else {
				parts.add(1);
			}
		}

		normalize();
		return *this;
	}
	BigInt &operator+=(SignedPart b) {
		if (parts.count == 0)
			parts.add(0);

		bool carry = false;

		add_carry(b, parts[0], false, &parts[0], &carry);
		for (umm part_index = 1; part_index < parts.count; ++part_index) {
			add_carry(parts[part_index], 0, carry, &parts[part_index], &carry);
		}

		auto bmsb = b < 0;
		// if needed wrap around zero or grow
		if (carry) {
			if (msb) {
				if (bmsb) {
					// this       b          result
					// -1(...F) + -1(...F) = -2(...E)     don't need carry bit!
					// msb = true; // noop
				} else {
					// this     b       result
					// -1(...F) + 1(1) = 10 but need 0! - don't add carry bit!
					msb = false;
				}
			} else {
				if (bmsb) {
					// this     b       result
					// 1(1) + -1(...F) = 10 but need 0! - don't add carry bit!
					// msb = false; // noop
				} else {
					// this     b       result
					// 1(1) + 15(F) = 16(10)     DO add carry bit!
					// msb = false; // noop
					parts.add(1);
				}
			}
		} else {
			if (bmsb)
				msb = true;
		}

		normalize();
		return *this;
	}
	BigInt &operator+=(BigInt b) {
		BigInt *min_int = this;
		BigInt *max_int = &b;
		if (min_int->parts.count > max_int->parts.count) {
			Swap(min_int, max_int);
		}

		parts.resize(max_int->parts.count);

		bool carry = false;

		// sum up low part
		// min_int         165a6f31
		//                 ++++++++
		// max_int   21765ac6ef371c
		for (umm part_index = 0; part_index < min_int->parts.count; ++part_index) {
			add_carry(min_int->parts[part_index], max_int->parts[part_index], carry, &parts[part_index], &carry);
		}

		// propagate carry into high part
		for (umm part_index = min_int->parts.count; part_index < max_int->parts.count; ++part_index) {
			add_carry(max_int->parts[part_index], 0, carry, &parts[part_index], &carry);
		}



		// if needed wrap around zero or grow
		if (carry) {
			if (msb) {
				if (b.msb) {
					// this       b          result
					// -1(...F) + -1(...F) = -2(...E)     don't need carry bit!
					// msb = true; // noop
				} else {
					// this     b       result
					// -1(...F) + 1(1) = 10 but need 0! - don't add carry bit!
					msb = false;
				}
			} else {
				if (b.msb) {
					// this     b       result
					// 1(1) + -1(...F) = 10 but need 0! - don't add carry bit!
					// msb = false; // noop
				} else {
					// this     b       result
					// 1(1) + 15(F) = 16(10)     DO add carry bit!
					// msb = false; // noop
					parts.add(1);
				}
			}
		} else {
			if (b.msb)
				msb = true;
		}

		normalize();
		return *this;
	}
	BigInt &operator*=(Part b) {
		auto a = *this;
		defer { free(a); };
		*this = {};
		for (u32 bit_index = 0; bit_index < bits_in_part; ++bit_index) {
			if ((b >> bit_index) & 1) {
				*this += a << bit_index;
			}
		}
		normalize();
		return *this;
	}
	BigInt &operator*=(BigInt b) {
		auto a = *this;
		defer { free(a); };
		*this = {};
		for (u32 bit_index = 0; bit_index < b.parts.count * (sizeof(b.parts[0]) * 8); ++bit_index) {
			if (b.get_bit(bit_index)) {
				*this += a << bit_index;
			}
		}
		normalize();
		return *this;
	}

	void divmod(Part b, BigInt &quotient, BigInt &remainder) const {
		umm max_parts_count = max(parts.count, 1);

		quotient.parts.resize(max_parts_count);
		remainder.parts.resize(max_parts_count);

		for (umm bit_index = max_parts_count * bits_in_part - 1; bit_index != ~(umm)0; --bit_index) {
			remainder <<= 1;
			remainder.set_bit(0, get_bit(bit_index));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(bit_index, 1);
			}
		}

		remainder.normalize();
		quotient.normalize();
	}
	void divmod(BigInt const &b, BigInt &quotient, BigInt &remainder) const {
		umm max_parts_count = max(parts.count, b.parts.count);

		quotient.parts.resize(max_parts_count);
		remainder.parts.resize(max_parts_count);

		for (umm bit_index = max_parts_count * bits_in_part - 1; bit_index != ~(umm)0; --bit_index) {
			remainder <<= 1;

			if (get_bit(bit_index))
				remainder.set_bit(0, true);

			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(bit_index, 1);
			}
		}

		remainder.normalize();
		quotient.normalize();
	}

	BigInt &operator/=(Part b) {
		BigInt result, dummy;
		divmod(b, result, dummy);
		free(dummy);
		return *this = result;
	}
	BigInt &operator/=(BigInt const &b) {
		BigInt result, dummy;
		divmod(b, result, dummy);
		free(dummy);
		return *this = result;
	}
	BigInt &operator%=(Part b) {
		BigInt result, dummy;
		divmod(b, dummy, result);
		free(dummy);
		return *this = result;
	}
	BigInt &operator%=(BigInt const &b) {
		BigInt result, dummy;
		divmod(b, dummy, result);
		free(dummy);
		return *this = result;
	}

	bool operator==(SignedPart b) const {
		if (parts.count == 1)
			return (SignedPart)parts[0] == b;
		if (parts.count == 0)
			return (msb ? -1 : 0) == b;
		return false;
	}
	bool operator==(BigInt b) const {
		if (msb != b.msb)
			return false;

		BigInt min_int = *this, max_int = b;
		if (min_int.parts.count > max_int.parts.count) {
			Swap(min_int, max_int);
		}

		for (Size i = 0; i < min_int.parts.count; ++i) {
			if (max_int.parts[i] != min_int.parts[i])
				return false;
		}

		for (Size index = max_int.parts.count - 1; index != min_int.parts.count - 1; --index) {
			if (max_int.parts[index] != 0) {
				return false;
			}
		}
		for (Size index = min_int.parts.count - 1; index != (Size)-1; --index) {
			if (min_int.parts[index] != max_int.parts[index]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(Part   b) const { return !operator==(b); }
	bool operator!=(BigInt b) const { return !operator==(b); }

	bool operator<(SignedPart b) const {
		if (parts.count == 1) {
			if (!msb) {
				// this is positive
				if (parts[0] > max_value<SignedPart>)
					return false;
			}
			return (SignedPart)parts[0] < b;
		}
		if (parts.count == 0)
			return (msb ? -1 : 0) < b;
		return msb;
	}
	bool operator<(BigInt b) const {
		if (msb != b.msb) {
			return msb;
		}

		/*
		3 2	false   1
		3 3 false   0
		3 4 true   -1
		*/

		auto diff = *this - b;
		defer { free(diff); };
		return diff.msb;
	}

	bool operator>(SignedPart b) const {
		if (parts.count == 1)
			return (SignedPart)parts[0] > b;
		if (parts.count == 0)
			return (msb ? -1 : 0) < b;
		return !msb;
	}
	bool operator>(BigInt b) const {
		if (msb != b.msb) {
			return b.msb;
		}

		/*
		3 2	true    1
		3 3 false   0
		3 4 false  -1
		*/

		auto diff = *this - b;
		defer { free(diff); };
		if (diff.msb) {
			return false;
		} else {
			if (diff == BigInt{}) {
				return false;
			}
			return true;
		}
	}

	bool operator>=(SignedPart b) const { return !(*this < b); }
	bool operator>=(BigInt b) const { return !(*this < b); }

	bool operator<=(SignedPart b) const { return !(*this > b); }
	bool operator<=(BigInt b) const { return !(*this > b); }

	BigInt &operator-=(SignedPart b) { return *this += -b; }
	BigInt &operator-=(BigInt const &b) { return *this += -b; }

	BigInt operator&(BigInt const &b) const { auto a = copy(*this); a &= b; return a; }
	BigInt operator|(BigInt const &b) const { auto a = copy(*this); a |= b; return a; }
	BigInt operator^(BigInt const &b) const { auto a = copy(*this); a ^= b; return a; }
	BigInt operator+(BigInt const &b) const { auto a = copy(*this); a += b; return a; }

	BigInt operator-(SignedPart    b) const { auto a = copy(*this); a -= b; return a; }
	BigInt operator-(BigInt const &b) const { auto a = copy(*this); a -= b; return a; }

	BigInt operator*(Part   b) const { auto a = copy(*this); a *= b; return a; }
	BigInt operator*(BigInt b) const { auto a = copy(*this); a *= b; return a; }
	BigInt operator/(Part   b) const { auto a = copy(*this); a /= b; return a; }
	BigInt operator/(BigInt b) const { auto a = copy(*this); a /= b; return a; }
	BigInt operator%(Part   b) const { auto a = copy(*this); a %= b; return a; }
	BigInt operator%(BigInt b) const { auto a = copy(*this); a %= b; return a; }
	BigInt operator<<(Part   b) const { auto a = copy(*this); a <<= b; return a; }
	BigInt operator<<(BigInt b) const { auto a = copy(*this); a <<= b; return a; }
	BigInt operator>>(Part   b) const { auto a = copy(*this); a >>= b; return a; }
	BigInt operator>>(BigInt b) const { auto a = copy(*this); a >>= b; return a; }

	//BigInt operator-(BigInt const &b) const { return *this + -b; }

	bool get_bit(umm index) const {
		umm part_index = index / (sizeof(parts[0]) * 8);
		if (part_index >= parts.count)
			return msb;

		umm bit_index  = index % (sizeof(parts[0]) * 8);
		return parts[part_index] & ((Part)1 << bit_index);
	}
	void set_bit(umm index, bool value) {
		umm part_index = index / (sizeof(parts[0]) * 8);
		umm bit_index  = index % (sizeof(parts[0]) * 8);
		if (value) parts[part_index] |=  ((Part)1 << bit_index);
		else       parts[part_index] &= ~((Part)1 << bit_index);
	}

	explicit operator u8 () { return parts.count ? (u8 )parts[0] : 0; }
	explicit operator u16() { return parts.count ? (u16)parts[0] : 0; }
	explicit operator u32() { return parts.count ? (u32)parts[0] : 0; }
	explicit operator u64() {
		if constexpr (bits_in_part == 64) {
			return parts.count ? (u64)parts[0] : 0;
		} else {
			static_error_v(bits_in_part, "not implemented");
		}
	}
	explicit operator s8 () { return (s8 )operator u8 (); }
	explicit operator s16() { return (s16)operator u16(); }
	explicit operator s32() { return (s32)operator u32(); }
	explicit operator s64() { return (s64)operator u64(); }

	explicit operator f64() {
		f64 result = 0;

		BigInt src = *this;

		if (msb) {
			src = copy(*this);
		}

		for (auto part : reverse_iterate(src.parts)) {
			result *= 18'446'744'073'709'551'616.0; // 2^64
			result += part;
		}

		if (msb) {
			free(src);
			result = -result;
		}

		return result;
	}

	template <class T>
	explicit operator T() { return (T)operator u64(); }

#if 0
	BigInt &set(BigInt const &that) { free(parts); parts = copy(that.parts); return *this; }

	u64 try_get_part(umm index) const {
		return index < parts.count ? parts[index] : (u64)0;
	}

	BigInt operator~() const {
		BigInt result;
		result.parts.reserve(parts.count);
		for (auto &part : parts) {
			result.parts.add(~part);
		}
		return result;
	}
	BigInt operator-() const {
		BigInt result = ~*this;
		return ++result;
	}

	BigInt &operator++() { return *this += 1bi; }
	BigInt operator++(int) { BigInt copy = *this; ++*this; return copy; }


	BigInt operator&(BigInt const &b) const { auto a = copy(*this); a &= b; return a; }

	BigInt &operator|=(BigInt const &b) {
		parts.resize(b.parts.count);
		for (umm i = 0; i < parts.count; ++i) {
			parts[i] |= b.parts[i];
		}
		return *this;
	}
	BigInt operator|(BigInt const &b) const { auto a = copy(*this); a |= b; return a; }

	BigInt operator-(BigInt const &b) const {
		assert(*this >= b);
		if (parts.count == 0)
			return {};

		BigInt result = *this;
		if (result.parts[0] < b.parts[0]) {
			for (umm part_index = 1; part_index != result.parts.count; part_index += 1) {
				result.parts[part_index]--;
				if (result.parts[part_index] != -1) {
					break;
				}
			}
		}
		result.parts[0] -= b.parts[0];
		return result;
	}
	BigInt operator*(BigInt b) const {
		BigInt result = {};
		for (u32 bit_index = 0; bit_index < b.parts.count * (sizeof(b.parts[0]) * 8); ++bit_index) {
			if (b.get_bit(bit_index)) {
				result += *this << bit_index;
			}
		}
		return result;
	}


	BigInt &operator-=(BigInt b) { return *this = *this - b; }
	BigInt &operator*=(BigInt b) { return *this = *this * b; }
	BigInt &operator/=(BigInt b) { return *this = *this / b; }
	BigInt &operator<<=(u64 b) { return *this = *this << b; }
#endif
};

template <class List>
BigInt<List> copy(BigInt<List> that) {
	BigInt<List> result;
	result.msb = that.msb;
	result.parts = copy(that.parts);
	return result;
}

template <class List>
void free(BigInt<List> &a) {
	free(a.parts);
}

//template <class List>
//inline umm append(StringBuilder &builder, impl::BigInt<List> value) {
//	using Part = decltype(value)::Part;
//
//	// BigInt temp = copy(value);
	// defer { free(temp); };
//
//	umm chars_appended = 0;
//	auto append = [&] (StringBuilder &builder, auto const &x) {
//		chars_appended += ::tl::append(builder, x);
//	};
//
//	// do {
	// 	temp.divmod(10, quotient, remainder);
	// 	append(builder, )
	// } while (temp != 0);
//
//	append(builder, "0x"s);
//	append(builder, FormatInt<Part>{.value=value.parts.back(), .radix=16});
//	for (smm i = (smm)value.parts.count - 2; i >= 0; --i) {
//		append(builder, FormatInt<Part>{.value=value.parts[i], .radix=16, .leading_zero_count=16});
//	}
//
//	return chars_appended;
//}

}

template <class List>
inline constexpr bool is_integer<impl::BigInt<List>> = true;

template <class List>
inline constexpr bool is_signed<impl::BigInt<List>> = true;

template <class List>
umm append(StringBuilder &builder, FormatInt<impl::BigInt<List>> f) {
	using Int = impl::BigInt<List>;

	Int v = f.value;
	auto radix = f.radix;
	u32 maxDigits = sizeof(Int::Part) * 8 * v.parts.count + 1;
	char *buf = (char *)alloca(maxDigits);

	auto charMap = f.char_set;
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if constexpr (is_signed<Int>) {
		if (v < 0) {
			negative = true;
			if constexpr (is_same<Int, s8> || is_same<Int, s16> || is_same<Int, s32> || is_same<Int, s64>) {
				if (v == min_value<Int>) {
					*lsc-- = charMap[(u32)-(v % radix)];
					v /= radix;
					++charsWritten;
				}
			}
			v = -v;
		}
	}

	for (;;) {
		u32 char_index = (u32)(v % radix);
		*lsc-- = charMap[char_index];
		++charsWritten;
		v /= radix;
		if (v == Int{})
			break;
	}
	lsc += f.skip_digits;
	charsWritten -= f.skip_digits;
	if constexpr (is_signed<Int>) {
		if (negative) {
			++charsWritten;
			*lsc-- = '-';
		}
	} else {
		(void)negative;
	}
	if (f.leading_zero_count) {
		for (u32 i = charsWritten; i < f.leading_zero_count; ++i) {
			*lsc-- = '0';
		}
		if (f.leading_zero_count > charsWritten)
			charsWritten = f.leading_zero_count;
	}
	return append(builder, Span(lsc + 1, charsWritten));
}

using BigInt = impl::BigInt<List<umm>>;

template <> inline constexpr bool is_integer<BigInt> = true;
template <> inline constexpr bool is_integer_like<BigInt> = true;
template <> inline constexpr bool is_signed<BigInt> = true;


template <class BigInt = BigInt, class Allocator = Allocator>
inline BigInt make_big_int(typename BigInt::Part value TL_LP) {
	BigInt result;
	result.msb = false;
	result.parts.set({value} TL_LA);
	return result;
}
template <class BigInt = BigInt, class Allocator = Allocator>
inline BigInt make_big_int(typename BigInt::SignedPart value TL_LP) {
	BigInt result;
	result.msb = value < 0;
	result.parts.set({(typename BigInt::Part)value} TL_LA);
	return result;
}
template <class BigInt = BigInt, class Allocator = Allocator>
inline BigInt make_big_int(typename BigInt::Part high, typename BigInt::Part low TL_LP) {
	BigInt result;
	result.msb = false;
	result.parts.set({low, high} TL_LA);
	return result;
}
template <class BigInt = BigInt, class Allocator = Allocator>
inline BigInt make_big_int(typename BigInt::Part high, typename BigInt::Part mid, typename BigInt::Part low TL_LP) {
	BigInt result;
	result.msb = false;
	result.parts.set({low, mid, high} TL_LA);
	return result;
}

inline BigInt operator""_ib(u64 value) {
	return make_big_int(value);
}

}
