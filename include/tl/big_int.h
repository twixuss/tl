#pragma once
#include "list.h"
#include "string.h"

namespace tl {

struct BigIntView {
	using Part = umm;


	bool msb = false;
	Span<Part> parts = {};

	
	bool get_bit_unchecked(umm index) const {
		umm part_index = index / (sizeof(parts[0]) * 8);
		umm bit_index  = index % (sizeof(parts[0]) * 8);
		return parts[part_index] & ((Part)1 << bit_index);
	}
	bool get_bit(umm index) const {
		umm part_index = index / (sizeof(parts[0]) * 8);
		if (part_index >= parts.count)
			return msb;

		umm bit_index  = index % (sizeof(parts[0]) * 8);
		return parts[part_index] & ((Part)1 << bit_index);
	}
	Part get_part(umm index) const {
		if (index < parts.count) {
			return parts.data[index];
		}
		return msb ? -1 : 0;
	}
};

template <class Allocator = Allocator>
struct BigInt;

BigInt<DefaultAllocator> big_int_scratch_pop();
void big_int_scratch_add(BigInt<DefaultAllocator>);

#define big_int_scratch_scoped(name) \
	auto name = big_int_scratch_pop(); \
	defer { big_int_scratch_add(name); }

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
template <class Allocator>
struct BigInt {
	using Part = umm;
	using SignedPart = std::make_signed_t<Part>;
	using List = List<Part, Allocator>;
	using Size = typename List::Size;

	static_assert(is_unsigned<Part>, "BigInt::List::Element must be unsigned");

	inline static constexpr umm bytes_in_part = sizeof(Part);
	inline static constexpr umm bits_in_part = bytes_in_part * 8;

	bool msb = false;
	List parts;
	
	BigIntView view() const { return {msb, parts.span()}; }
	operator BigIntView() const { return view(); }

	void set(SignedPart p) {
		parts.set(p);
		msb = p < 0;
		normalize();
	}

	void set(BigIntView that) {
		parts.set(that.parts);
		msb = that.msb;
	}

	void normalize() {
		for (smm i = parts.count - 1; i != -1; --i) {
			if (parts.data[i] == (msb ? (Part)-1 : (Part)0)) {
				parts.pop();
			} else {
				break;
			}
		}
	}

	void invert() {
		msb = !msb;
		for (auto &part : parts)
			part = ~part;
	}

	void negate() {
		invert();
		*this += (u64)1;
	}

	BigInt &operator^=(BigIntView b) {
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
	BigInt &operator|=(umm b) {
		if (parts.count == 0) {
			if (msb) {
				// already all ones, nothing to change
			} else {
				parts.add(b);
			}
		} else {
			parts.data[0] |= b;
		}

		normalize();
		return *this;
	}
	BigInt &operator|=(smm b) {
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
				parts.count = 1;
			}
		}

		normalize();
		return *this;
	}
	BigInt &operator|=(BigIntView b) {
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

	BigInt &operator&=(BigIntView b) {
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


	#if 0
	template <class A, class B>
	bool shift_left(BigIntView b, u64 bytes_threshold, A &&error_shift_by_negative, B &&error_too_big) {
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

		parts.insert_at(Repeat{(Part)0, zero_part_count_big}, 0);
		return true;
	}
	BigInt &operator<<=(BigIntView b) {
		shift_left(b, max_value<u64>, []{}, [](auto){});
		return *this;
	}
	#endif

	BigInt &operator<<=(umm b) {
		parts.insert_at(Repeat{.value = (Part)0, .count = b / bits_in_part}, 0);

		auto part_shift = b % bits_in_part;
		if (part_shift) {
			if (parts.count) {
				parts.resize(parts.count+1);
				parts.data[parts.count-1] = ((msb ? (Part)-1 : (Part)0) << part_shift) | (parts.data[parts.count-2] >> (bits_in_part - part_shift));
				for (umm part_index = parts.count-2; part_index != 0; --part_index) {
					// NOTE: there is shld instruction, which does 128 bit shift.
					// There's no intrinsic for it and msvc can't optimize this.
					parts.data[part_index] = (parts.data[part_index] << part_shift) | (parts.data[part_index - 1] >> (bits_in_part - part_shift));
				}
				parts.data[0] <<= part_shift;
			}
		}

		normalize();
		return *this;
	}

	inline void shift_right_arithmetic(umm delete_part_count, umm shift_amount) {
		if (delete_part_count >= parts.count) {
			parts.clear();
		} else {
			parts.erase(parts.subspan(0, delete_part_count));

			if (shift_amount) {
				for (umm part_index = 1; part_index < parts.count-1; ++part_index) {
					// NOTE: there is shrd instruction, which does 128 bit shift.
					// There's no intrinsic for it and msvc can't optimize this.
					parts.data[part_index-1] = (parts.data[part_index-1] >> shift_amount) | (parts.data[part_index] << (bits_in_part - shift_amount));
				}
				if (msb) {
					parts.data[parts.count - 1] = (SignedPart)parts.data[parts.count - 1] >> shift_amount;
				} else {
					parts.data[parts.count - 1] =             parts.data[parts.count - 1] >> shift_amount;
				}
			}
		}
		normalize();
	}
	BigInt &operator>>=(umm b) {
		if (!parts.count)
			return *this;

		shift_right_arithmetic(b / bits_in_part, b % bits_in_part);
		return *this;
	}
	// NOTE: shift amount is masked by one part
	BigInt &operator>>=(BigIntView b) {
		u64 total_shift_amount = b.get_part(0);

		u64 delete_part_count = total_shift_amount >> log2(bits_in_part);
		u64 part_shift_amount = total_shift_amount & (bits_in_part - 1);

		if (delete_part_count >= parts.count) {
			parts.clear();
		} else {
			shift_right_arithmetic(delete_part_count, part_shift_amount);
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
	BigInt &operator+=(BigIntView b) {
		bool carry = false;

		umm max_parts = max(parts.count, b.parts.count);

		parts.resize(max_parts, msb ? -1 : 0);

		for (umm i = 0; i < max_parts; ++i) {
			umm result = 0;
			add_carry(get_part(i), b.get_part(i), carry, &result, &carry);
			parts[i] = result;
		}

		if ( msb &&  b.msb && !carry) parts.add(0);
		if ( msb && !b.msb &&  carry) msb = false;
		if (!msb &&  b.msb && !carry) msb = true;
		if (!msb && !b.msb &&  carry) parts.add(1);

		normalize();
		return *this;
	}
	BigInt &operator-=(Part       b) {
		if (parts.count == 0)
			parts.add(0);

		bool borrow = false;

		//sub_borrow(parts[0], b, borrow, &parts[0], &borrow);
		borrow = _subborrow_u64(borrow, parts[0], b, &parts[0]);
		for (umm part_index = 1; part_index < parts.count; ++part_index) {
			//sub_borrow(parts[part_index], 0, borrow, &parts[part_index], &borrow);
			borrow = _subborrow_u64(borrow, parts[part_index], 0, &parts[part_index]);
		}

		if (borrow) {
			if (!msb) {
				msb = true;
			} else {
				parts.add(~0ull);
			}
		}

		normalize();
		return *this;
	}
	BigInt &operator-=(SignedPart b) { return *this += -b; }
	BigInt &operator-=(BigIntView b) {
		// Not optimal
		big_int_scratch_scoped(x);

		x.set(b);
		x.negate();
		*this += x;
		return *this;
	}
	
	BigInt &operator*=(Part that) {
		big_int_scratch_scoped(a);
		big_int_scratch_scoped(b);

		a.set(*this);
		
		*this = {};

		for (u32 bit_index = 0; bit_index < bits_in_part; ++bit_index) {
			if ((that >> bit_index) & 1) {
				b.set(a);
				b <<= bit_index;
				*this += b;
			}
		}
		normalize();
		return *this;
	}
	BigInt &operator*=(BigIntView that) {
		big_int_scratch_scoped(a);
		big_int_scratch_scoped(b);

		a.set(*this);

		*this = {};

		for (u32 bit_index = 0; bit_index < that.parts.count * bits_in_part; ++bit_index) {
			if (that.get_bit_unchecked(bit_index)) {
				b.set(a);
				b <<= bit_index;
				*this += b;
			}
		}
		normalize();
		return *this;
	}

	template <class QA, class RA>
	void divmod(Part b, BigInt<QA> &quotient, BigInt<RA> &remainder) const {
		umm max_parts_count = max(parts.count, (Size)1);

		quotient.msb = 0;
		quotient.parts.clear();
		quotient.parts.resize(max_parts_count);
		remainder.msb = 0;
		remainder.parts.clear();

		for (umm bit_index = max_parts_count * bits_in_part - 1; bit_index != ~(umm)0; --bit_index) {
			remainder <<= 1;
			remainder.set_bit(0, get_bit(bit_index));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit_unchecked(bit_index, 1);
			}
		}

		remainder.normalize();
		quotient.normalize();
	}
	template <class QA, class RA>
	void divmod(BigIntView b, BigInt<QA> &quotient, BigInt<RA> &remainder) const {
		umm max_parts_count = max(parts.count, b.parts.count);
		
		quotient.msb = 0;
		quotient.parts.clear();
		quotient.parts.resize(max_parts_count);
		remainder.msb = 0;
		remainder.parts.clear();

		for (umm bit_index = max_parts_count * bits_in_part - 1; bit_index != ~(umm)0; --bit_index) {
			remainder <<= 1;

			if (get_bit(bit_index))
				remainder.set_bit(0, true);

			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit_unchecked(bit_index, 1);
			}
		}

		remainder.normalize();
		quotient.normalize();
	}

	BigInt &operator/=(Part b) {
		big_int_scratch_scoped(quotient);
		big_int_scratch_scoped(remainder);
		divmod(b, quotient, remainder);
		set(quotient);
		return *this;
	}
	BigInt &operator/=(BigIntView b) {
		big_int_scratch_scoped(quotient);
		big_int_scratch_scoped(remainder);
		divmod(b, quotient, remainder);
		set(quotient);
		return *this;
	}
	BigInt &operator%=(Part b) {
		big_int_scratch_scoped(quotient);
		big_int_scratch_scoped(remainder);
		divmod(b, quotient, remainder);
		set(remainder);
		return *this;
	}
	BigInt &operator%=(BigIntView b) {
		big_int_scratch_scoped(quotient);
		big_int_scratch_scoped(remainder);
		divmod(b, quotient, remainder);
		set(remainder);
		return *this;
	}
	
	bool operator==(Part b) const {
		if (parts.count == 1)
			return msb == 0 && parts[0] == b;
		if (parts.count == 0)
			return msb == 0 && b == 0;
		return false;
	}
	bool operator==(SignedPart b) const {
		if (parts.count == 1)
			return msb == (b < 0) && (SignedPart)parts[0] == b;
		if (parts.count == 0)
			return (msb ? -1 : 0) == b;
		return false;
	}
	bool operator==(BigIntView b) const {
		if (msb != b.msb)
			return false;

		if (parts.count != b.parts.count)
			return false;

		for (Size i = 0; i < parts.count; ++i) {
			if (parts[i] != b.parts[i])
				return false;
		}

		return true;
	}

	bool operator!=(Part       b) const { return !operator==(b); }
	bool operator!=(SignedPart b) const { return !operator==(b); }
	bool operator!=(BigIntView b) const { return !operator==(b); }

	bool operator<(Part b) const {
		if (msb)
			return true;

		if (parts.count == 0)
			return 0 < b;
		if (parts.count == 1)
			return parts[0] < b;
		return false;
	}
	bool operator<(SignedPart b) const {
		if (parts.count == 1) {
			if (parts[0] >> 63 != msb)
				return msb;
			return (SignedPart)parts[0] < b;
		}
		if (parts.count == 0)
			return (msb ? -1 : 0) < b;
		return msb;
	}
	bool operator<(BigIntView b) const {
		if (msb != b.msb) {
			return msb;
		}

		for (umm i = max(parts.count, b.parts.count) - 1; i != -1; --i) {
			if (get_part(i) != b.get_part(i)) {
				return get_part(i) < b.get_part(i);
			}
		}

		return false;
	}

	bool operator>(Part b) const {
		if (msb)
			return false;

		if (parts.count == 0)
			return 0 > b;
		if (parts.count == 1)
			return parts[0] > b;
		return true;
	}
	bool operator>(SignedPart b) const {
		if (parts.count == 1) {
			if (parts[0] >> 63 != msb)
				return !msb;
			return (SignedPart)parts[0] > b;
		}
		if (parts.count == 0)
			return (msb ? -1 : 0) < b;
		return !msb;
	}
	bool operator>(BigIntView b) const {
		if (msb != b.msb) {
			return b.msb;
		}
		
		for (umm i = max(parts.count, b.parts.count) - 1; i != -1; --i) {
			if (get_part(i) != b.get_part(i)) {
				return get_part(i) > b.get_part(i);
			}
		}

		return false;
	}

	bool operator>=(Part       b) const { return !(*this < b); }
	bool operator>=(SignedPart b) const { return !(*this < b); }
	bool operator>=(BigIntView b) const { return !(*this < b); }

	bool operator<=(Part       b) const { return !(*this > b); }
	bool operator<=(SignedPart b) const { return !(*this > b); }
	bool operator<=(BigIntView b) const { return !(*this > b); }
	
	bool operator==(u8  b) const { return *this == (Part)b; }
	bool operator==(u16 b) const { return *this == (Part)b; }
	bool operator==(u32 b) const { return *this == (Part)b; }
	bool operator==(s8  b) const { return *this == (SignedPart)b; }
	bool operator==(s16 b) const { return *this == (SignedPart)b; }
	bool operator==(s32 b) const { return *this == (SignedPart)b; }

	bool operator!=(u8  b) const { return *this != (Part)b; }
	bool operator!=(u16 b) const { return *this != (Part)b; }
	bool operator!=(u32 b) const { return *this != (Part)b; }
	bool operator!=(s8  b) const { return *this != (SignedPart)b; }
	bool operator!=(s16 b) const { return *this != (SignedPart)b; }
	bool operator!=(s32 b) const { return *this != (SignedPart)b; }

	bool operator<(u8  b) const { return *this < (Part)b; }
	bool operator<(u16 b) const { return *this < (Part)b; }
	bool operator<(u32 b) const { return *this < (Part)b; }
	bool operator<(s8  b) const { return *this < (SignedPart)b; }
	bool operator<(s16 b) const { return *this < (SignedPart)b; }
	bool operator<(s32 b) const { return *this < (SignedPart)b; }

	bool operator>(u8  b) const { return *this > (Part)b; }
	bool operator>(u16 b) const { return *this > (Part)b; }
	bool operator>(u32 b) const { return *this > (Part)b; }
	bool operator>(s8  b) const { return *this > (SignedPart)b; }
	bool operator>(s16 b) const { return *this > (SignedPart)b; }
	bool operator>(s32 b) const { return *this > (SignedPart)b; }

	bool operator<=(u8  b) const { return *this <= (Part)b; }
	bool operator<=(u16 b) const { return *this <= (Part)b; }
	bool operator<=(u32 b) const { return *this <= (Part)b; }
	bool operator<=(s8  b) const { return *this <= (SignedPart)b; }
	bool operator<=(s16 b) const { return *this <= (SignedPart)b; }
	bool operator<=(s32 b) const { return *this <= (SignedPart)b; }

	bool operator>=(u8  b) const { return *this >= (Part)b; }
	bool operator>=(u16 b) const { return *this >= (Part)b; }
	bool operator>=(u32 b) const { return *this >= (Part)b; }
	bool operator>=(s8  b) const { return *this >= (SignedPart)b; }
	bool operator>=(s16 b) const { return *this >= (SignedPart)b; }
	bool operator>=(s32 b) const { return *this >= (SignedPart)b; }

	bool is_zero() const {
		return parts.count == 0 && !msb;
	}

	//BigInt operator-(BigIntView b) const { return *this + -b; }
	
	bool get_bit_unchecked(umm index) const { return view().get_bit_unchecked(index); }
	bool get_bit(umm index) const { return view().get_bit(index); }
	Part get_part(umm index) const { return view().get_part(index); }

	void set_bit_unchecked(umm index, bool value) {
		umm part_index = index / (sizeof(parts[0]) * 8);
		umm bit_index  = index % (sizeof(parts[0]) * 8);
		if (value) parts[part_index] |=  ((Part)1 << bit_index);
		else       parts[part_index] &= ~((Part)1 << bit_index);
	}
	void set_bit(umm index, bool value) {
		umm part_index = index / (sizeof(parts[0]) * 8);
		umm bit_index  = index % (sizeof(parts[0]) * 8);
		if (part_index >= parts.count) {
			auto old_count = parts.count;
			parts.resize(part_index + 1);
			for (umm i = old_count; i < parts.count; ++i) {
				parts.data[i] = msb ? -1 : 0;
			}
		}
		if (value) parts[part_index] |=  ((Part)1 << bit_index);
		else       parts[part_index] &= ~((Part)1 << bit_index);
	}

	explicit operator u8 () const { return get_part(0); }
	explicit operator u16() const { return get_part(0); }
	explicit operator u32() const { return get_part(0); }
	explicit operator u64() const { return get_part(0); }
	explicit operator s8 () const { return get_part(0); }
	explicit operator s16() const { return get_part(0); }
	explicit operator s32() const { return get_part(0); }
	explicit operator s64() const { return get_part(0); }

	explicit operator f32() const { return (f32)*this; }
	explicit operator f64() const {
		f64 result = 0;

		big_int_scratch_scoped(x);

		x.set(*this);

		if (msb) {
			x.negate();
		}

		for (auto part : reversed(x.parts)) {
			result *= 18'446'744'073'709'551'616.0; // 2^64
			result += part;
		}

		if (msb) {
			result = -result;
		}

		return result;
	}

	BigInt copy() const {
		BigInt result;
		result.msb = msb;
		result.parts = tl::copy(parts);
		return result;
	}

	void free() {
		tl::free(parts);
	}

	void make_absolute() {
		if (msb) {
			negate();
		}
	}

	umm trailing_zeros_count() const {
		umm result = 0;
		for (auto part : parts) {
			for (umm bit = 0; bit < bits_in_part; ++bit) {
				if (part & (1 << bit))
					return result;
				++result;
			}
		}
		return result;
	}
	
	void pow(umm exponent) {
		big_int_scratch_scoped(base);
		base.set(*this);
		set((umm)1);

		while (exponent) {
			if (exponent & 1)
				*this *= base;
			exponent >>= 1;
			base *= base;
		}
	}

	#if 0
	// Operators that return new value. Disabled because easy to leak.
	BigInt operator~() const { return copy().invert(); }
	BigInt operator-() const { return copy().negate(); }

	BigInt operator&(BigIntView b) const { auto a = copy(); a &= b; return a; }
	BigInt operator|(BigIntView b) const { auto a = copy(); a |= b; return a; }
	BigInt operator^(BigIntView b) const { auto a = copy(); a ^= b; return a; }
	BigInt operator+(BigIntView b) const { auto a = copy(); a += b; return a; }

	BigInt operator-(SignedPart    b) const { auto a = copy(); a -= b; return a; }
	BigInt operator-(BigIntView b) const { auto a = copy(); a -= b; return a; }

	BigInt operator*(Part   b) const { auto a = copy(); a *= b; return a; }
	BigInt operator*(BigInt b) const { auto a = copy(); a *= b; return a; }
	BigInt operator/(Part   b) const { auto a = copy(); a /= b; return a; }
	BigInt operator/(BigInt b) const { auto a = copy(); a /= b; return a; }
	BigInt operator%(Part   b) const { auto a = copy(); a %= b; return a; }
	BigInt operator%(BigInt b) const { auto a = copy(); a %= b; return a; }
	BigInt operator<<(Part   b) const { auto a = copy(); a <<= b; return a; }
	BigInt operator<<(BigInt b) const { auto a = copy(); a <<= b; return a; }
	BigInt operator>>(Part   b) const { auto a = copy(); a >>= b; return a; }
	BigInt operator>>(BigInt b) const { auto a = copy(); a >>= b; return a; }
	#endif
};

inline static List<BigInt<DefaultAllocator>, DefaultAllocator> scratch_ints;

inline BigInt<DefaultAllocator> big_int_scratch_pop() {
	if (auto popped = scratch_ints.pop()) {
		popped.value().parts.clear();
		popped.value().msb = 0;
		return popped.value();
	}
	return {};
}
inline void big_int_scratch_add(BigInt<DefaultAllocator> value) {
	scratch_ints.add(value);
}

TL_DECLARE_CONCEPT(BigInt);

inline void gcd(CBigInt auto &a, CBigInt auto _b) {
	if (a.is_zero()) {
		return;
	}
	if (_b.is_zero()) {
		a.set((u64)0);
		return;
	}

	big_int_scratch_scoped(b);
	b.set(_b);

	auto at = a.trailing_zeros_count();
	auto bt = b.trailing_zeros_count();
	auto d = min(at, bt);

	a >>= at;
	b >>= bt;

	while (a != b) {
		if (a > b) {
			a -= b;
			a >>= a.trailing_zeros_count();
		} else {
			b -= a;
			b >>= b.trailing_zeros_count();
		}
	}
	
	// b = pow(2, d)
	b.parts.clear();
	b.msb = 0;
	b.set_bit(d, 1);

	a *= b;
}
inline void lcm(CBigInt auto &a, CBigInt auto b) {
	//  |a|*|b|
	// ---------
	// gcd(a, b)
	
	big_int_scratch_scoped(x);
	big_int_scratch_scoped(y);

	x.set(a);
	y.set(b);

	a.make_absolute();
	y.make_absolute();

	gcd(x, y);
	a /= x;

	a *= y;
}

template<class Allocator> inline constexpr bool is_integer<BigInt<Allocator>> = true;
template<class Allocator> inline constexpr bool is_integer_like<BigInt<Allocator>> = true;
template<class Allocator> inline constexpr bool is_signed<BigInt<Allocator>> = true;

template <class Allocator = Allocator>
BigInt<Allocator> make_big_int(s64 value) {
	BigInt<Allocator> result;
	result.set(value);
	return result;
}

template <class Allocator>
inline umm append(StringBuilder &builder, FormatInt<BigInt<Allocator>> f) {
	auto v = f.value;
	auto radix = f.radix;
	u32 maxDigits = sizeof(decltype(v)::Part) * 8 * v.parts.count + 1;
	char *buf = (char *)alloca(maxDigits);

	auto charMap = f.char_set;
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if (v < 0) {
		negative = true;
		v.negate();
	}

	for (;;) {
		big_int_scratch_scoped(quotient);
		big_int_scratch_scoped(remainder);

		v.divmod(radix, quotient, remainder);
		
		u32 char_index = (u32)remainder;

		*lsc-- = charMap[char_index];
		++charsWritten;

		v.set(quotient);

		if (v.is_zero())
			break;
	}
	lsc += f.skip_digits;
	charsWritten -= f.skip_digits;
	if (negative) {
		++charsWritten;
		*lsc-- = '-';
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

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	using BigInt = BigInt<DefaultAllocator>;

	BigInt a;
	BigInt b;
	
	auto set = [](BigInt &val, bool msb, Span<u64> rev_parts) {
		val.msb = msb;
		val.parts.reserve(rev_parts.count);
		val.parts.count = rev_parts.count;
		for (umm i = 0; i < rev_parts.count; ++i) {
			val.parts.data[i] = rev_parts.data[rev_parts.count - 1 - i];
		}
	};

	// NOTE: order of parts in `set` is reverse of store order, so rightmost parts are smallest and parts to the left are bigger.

	/* BITWISE OR  | */ {

		set(a, false, {0x0202, 0x0101});
		set(b, false, {0x0022, 0x0011});
		a |= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0111);
		assert(a.parts[1] == 0x0222);
	

		set(a, false, {        0x0101});
		set(b, false, {0x0022, 0x0011});
		a |= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0111);
		assert(a.parts[1] == 0x0022);


		set(a, false, {0x0202, 0x0101});
		set(b, false, {        0x0011});
		a |= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0111);
		assert(a.parts[1] == 0x0202);


		set(a, false, {0x0202, 0x0101});
		set(b, true,  {        0x0011});
		a |= b;
		assert(a.msb == true);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 0x0111);

	
		set(a, true,  {        0x0101});
		set(b, false, {0x0022, 0x0011});
		a |= b;
		assert(a.msb == true);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 0x0111);


		set(a, true,  {0x0202, 0x0101});
		set(b, false, {        0x0011});
		a |= b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0111);
		assert(a.parts[1] == 0x0202);

	
		set(a, false, {        0x0101});
		set(b, true,  {0x0022, 0x0011});
		a |= b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0111);
		assert(a.parts[1] == 0x0022);
	}
	/* BITWISE AND & */ {

		set(a, false, {0x0202, 0x0101});
		set(b, false, {0x0022, 0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0001);
		assert(a.parts[1] == 0x0002);
	

		set(a, false, {        0x0101});
		set(b, false, {0x0022, 0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 0x0001);


		set(a, false, {0x0202, 0x0101});
		set(b, false, {        0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 0x0001);


		set(a, false, {0x0202, 0x0101});
		set(b, true,  {        0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0001);
		assert(a.parts[1] == 0x0202);

	
		set(a, true,  {        0x0101});
		set(b, false, {0x0022, 0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0001);
		assert(a.parts[1] == 0x0022);


		set(a, true,  {0x0202, 0x0101});
		set(b, false, {        0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 0x0001);

	
		set(a, false, {        0x0101});
		set(b, true,  {0x0022, 0x0011});
		a &= b;
		assert(a.msb == false);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 0x0001);
	}
	/* BITWISE XOR ^ */ {

		set(a, false, {0x0202, 0x0101});
		set(b, false, {0x0022, 0x0011});
		a ^= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0110);
		assert(a.parts[1] == 0x0220);
	

		set(a, false, {        0x0101});
		set(b, false, {0x0022, 0x0011});
		a ^= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0110);
		assert(a.parts[1] == 0x0022);


		set(a, false, {0x0202, 0x0101});
		set(b, false, {        0x0011});
		a ^= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0110);
		assert(a.parts[1] == 0x0202);


		set(a, false, {0x0202, 0x0101});
		set(b, true,  {        0x0011});
		a ^= b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] ==  0x0110);
		assert(a.parts[1] == ~0x0202);
	

		set(a, true,  {        0x0101});
		set(b, false, {0x0022, 0x0011});
		a ^= b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] ==  0x0110);
		assert(a.parts[1] == ~0x0022);


		set(a, true,  {0x0202, 0x0101});
		set(b, false, {        0x0011});
		a ^= b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0110);
		assert(a.parts[1] == 0x0202);

	
		set(a, false, {        0x0101});
		set(b, true,  {0x0022, 0x0011});
		a ^= b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x0110);
		assert(a.parts[1] == 0x0022);
	}
	/* ADD + */ {
		set(a, false, {34});
		set(b, false, {35});
		a += b;
		assert(a.msb == false);
		assert(a.parts.count == 1);
		assert(a.parts[0] == 69);

		
		set(a, false, {34, 34});
		set(b, false, {35, 35});
		a += b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 69);
		assert(a.parts[1] == 69);

		
		set(a, false, {0x8000'0000'0000'1234});
		set(b, false, {0x8000'0000'0000'0000});
		a += b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x1234);
		assert(a.parts[1] == 1);
		

		set(a, false, {           2});
		set(b, true,  {autocast -10});
		a += b;
		assert(a.msb == true);
		assert(a.parts.count == 1);
		assert(a.parts[0] == -8);
		

		set(a, false, {           2, 100});
		set(b, true,  {autocast -10,  10});
		a += b;
		assert(a.msb == true);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 110);
		assert(a.parts[1] == -8);
	}
	/* MULTIPLY * */ {
		set(a, false, {0x8000'0000'0000'0000});
		set(b, false, {5});
		a *= b;
		assert(a.msb == false);
		assert(a.parts.count == 2);
		assert(a.parts[0] == 0x8000'0000'0000'0000);
		assert(a.parts[1] == 2);
	}
};

#endif

