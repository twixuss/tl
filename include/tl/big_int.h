#pragma once
#include "list.h"
#include "string.h"

namespace tl {

struct BigInt;
inline BigInt copy(BigInt const &a TL_LP);
inline void free(BigInt &a);

struct BigInt {
	bool msb = false;
	List<u64> parts;

	void normalize() {
		for (umm i = parts.count - 1; i != 0; --i) {
			if (parts[i] == (msb ? (u64)-1 : (u64)0)) {
				parts.pop();
			} else {
				break;
			}
		}
	}

	BigInt operator~() const {
		BigInt result;
		result.msb = !msb;
		result.parts.reserve(parts.count);
		for (auto &part : parts) {
			result.parts.add(~part);
		}
		return result;
	}

	BigInt operator-() const {
		BigInt result = ~*this;
		result += 1;
		return result;
	}
	/*
	BigInt &operator&=(BigInt const &b) {
		if (!msb) {

			if (!b.msb) {
				if (b.parts.count < parts.count) {
					// this 000000012345
					// b    000000000123
					parts.resize(b.parts.count);
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] &= b.parts[i];
					}
				} else {
					// this 000000012345
					// b    000001234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] &= b.parts[i];
					}
				}

			} else {
				if (b.parts.count < parts.count) {
					// this 000000012345
					// b    fffffffff123

					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] &= b.parts[i];
					}

				} else {
					// this 000000012345
					// b    fffff1234567

					for (umm i = 0; i < parts.count; ++i) {
						parts[i] &= b.parts[i];
					}
				}
			}

		} else {

			if (!b.msb) {

				if (b.parts.count < parts.count) {
					// this fffffff12345
					// b    000000000123
					msb = false;
					parts.resize(b.parts.count);
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] &= b.parts[i];
					}

				} else {
					// this fffffff12345
					// b    000001234567
					msb = false;
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] &= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(b.parts[i]);
					}
				}

			} else {
				if (b.parts.count < parts.count) {
					// this fffffff12345
					// b    fffffffff123
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] &= b.parts[i];
					}
				} else {
					// this fffffff12345
					// b    fffff1234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] &= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(b.parts[i]);
					}
				}
			}
		}

		normalize();
		return *this;
	}
	BigInt &operator|=(BigInt const &b) {
		if (!msb) {

			if (!b.msb) {
				if (b.parts.count < parts.count) {
					// this 000000012345
					// b    000000000123
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
				} else {
					// this 000000012345
					// b    000001234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(b.parts[i]);
					}
				}

			} else {
				if (b.parts.count < parts.count) {
					// this 000000012345
					// b    fffffffff123
					msb = true;
					parts.resize(b.parts.count);
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
				} else {
					// this 000000012345
					// b    fffff1234567
					msb = true;
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(b.parts[i]);
					}
				}
			}
		} else {
			if (!b.msb) {
				if (b.parts.count < parts.count) {
					// this fffffff12345
					// b    000000000123
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
				} else {
					// this fffffff12345
					// b    000001234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
				}

			} else {
				if (b.parts.count < parts.count) {
					// this fffffff12345
					// b    fffffffff123
					parts.resize(b.parts.count);
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
				} else {
					// this fffffff12345
					// b    fffff1234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] |= b.parts[i];
					}
				}
			}
		}

		normalize();
		return *this;
	}
	BigInt &operator^=(BigInt const &b) {
		if (!msb) {
			if (!b.msb) {
				if (b.parts.count < parts.count) {
					// this 000000012345
					// b    000000000123
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
				} else {
					// this 000000012345
					// b    000001234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}

					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(b.parts[i]);
					}
				}

			} else {
				if (b.parts.count < parts.count) {
					// this 000000012345
					// b    fffffffff123
					msb = true;
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
					for (umm i = b.parts.count; i < parts.count; ++i) {
						parts[i] = ~parts[i];
					}
				} else {
					// this 000000012345
					// b    fffff1234567
					msb = true;
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(b.parts[i]);
					}
				}
			}
		} else {
			if (!b.msb) {
				if (b.parts.count < parts.count) {
					// this fffffff12345
					// b    000000000123
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
				} else {
					// this fffffff12345
					// b    000001234567
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(~b.parts[i]);
					}
				}

			} else {
				if (b.parts.count < parts.count) {
					// this fffffff12345
					// b    fffffffff123
					msb = false;
					for (umm i = 0; i < b.parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
					for (umm i = b.parts.count; i < parts.count; ++i) {
						parts[i] = ~parts[i];
					}
				} else {
					// this fffffff12345
					// b    fffff1234567
					msb = false;
					for (umm i = 0; i < parts.count; ++i) {
						parts[i] ^= b.parts[i];
					}
					for (umm i = parts.count; i < b.parts.count; ++i) {
						parts.add(~b.parts[i]);
					}
				}
			}
		}

		normalize();
		return *this;
	}
	*/
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
	BigInt &operator<<=(u64 b) {
		u64 zero_part_count = b >> 6;
		while (zero_part_count--)
			parts.insert_at(0, 0);

		b &= 63;
		if (b) {
			if (parts.count) {
				List<u64> new_parts;
				new_parts.add(parts[0] << b);
				for (umm part_index = 1; part_index != parts.count; ++part_index) {
					new_parts.add((parts[part_index] << b) | (parts[part_index - 1] >> (64 - b)));
				}
				new_parts.add(((msb ? (u64)-1 : (u64)0) << b) | (parts[parts.count - 1] >> (64 - b)));
				free(parts);
				parts = new_parts;
			}
		}

		normalize();
		return *this;
	}

	BigInt &operator+=(u64 b) {
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
	BigInt &operator+=(BigInt b) {
		BigInt *min_int = this;
		BigInt *max_int = &b;
		if (min_int->parts.count > max_int->parts.count) {
			swap(min_int, max_int);
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
				msb = false;
			} else {
				parts.add(1);
			}
		} else {
			if (b.msb)
				msb = true;
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

	bool operator==(BigInt b) const {
		if (msb != b.msb)
			return false;

		BigInt min_int = *this, max_int = b;
		if (min_int.parts.count > max_int.parts.count) {
			swap(min_int, max_int);
		}

		for (umm i = 0; i < min_int.parts.count; ++i) {
			if (max_int.parts[i] != min_int.parts[i])
				return false;
		}

		for (umm index = max_int.parts.count - 1; index != min_int.parts.count - 1; --index) {
			if (max_int.parts[index] != 0) {
				return false;
			}
		}
		for (umm index = min_int.parts.count - 1; index != (umm)-1; --index) {
			if (min_int.parts[index] != max_int.parts[index]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(BigInt b) const {
		return !operator==(b);
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
	bool operator>=(BigInt b) const { return !(*this < b); }
	bool operator<=(BigInt b) const { return !(*this > b); }

	BigInt &operator-=(BigInt const &b) { return *this += -b; }
	BigInt operator&(BigInt const &b) const { auto a = copy(*this); a &= b; return a; }
	BigInt operator|(BigInt const &b) const { auto a = copy(*this); a |= b; return a; }
	BigInt operator^(BigInt const &b) const { auto a = copy(*this); a ^= b; return a; }
	BigInt operator+(BigInt const &b) const { auto a = copy(*this); a += b; return a; }
	BigInt operator-(BigInt const &b) const { auto a = copy(*this); a -= b; return a; }
	BigInt operator*(BigInt const &b) const { auto a = copy(*this); a *= b; return a; }
	BigInt operator<<(u64 b) const { auto a = copy(*this); a <<= b; return a; }

	//BigInt operator-(BigInt const &b) const { return *this + -b; }

	bool get_bit(umm index) const {
		umm part_index = index / (sizeof(parts[0]) * 8);
		umm bit_index  = index % (sizeof(parts[0]) * 8);
		return parts[part_index] & ((u64)1 << bit_index);
	}
	void set_bit(umm index, bool value) {
		umm part_index = index / (sizeof(parts[0]) * 8);
		umm bit_index  = index % (sizeof(parts[0]) * 8);
		if (value) parts[part_index] |=  ((u64)1 << bit_index);
		else       parts[part_index] &= ~((u64)1 << bit_index);
	}

	explicit operator u8 () { return parts.count ? (u8 )parts[0] : 0; }
	explicit operator u16() { return parts.count ? (u16)parts[0] : 0; }
	explicit operator u32() { return parts.count ? (u32)parts[0] : 0; }
	explicit operator u64() { return parts.count ? (u64)parts[0] : 0; }
	explicit operator s8 () { return parts.count ? (s8 )parts[0] : 0; }
	explicit operator s16() { return parts.count ? (s16)parts[0] : 0; }
	explicit operator s32() { return parts.count ? (s32)parts[0] : 0; }
	explicit operator s64() { return parts.count ? (s64)parts[0] : 0; }

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
	BigInt operator/(BigInt const &b) const {
		BigInt min_int = *this, max_int = b;
		if (min_int.parts.count > max_int.parts.count) {
			swap(min_int, max_int);
		}

		BigInt N = *this;
		BigInt quotient;
		BigInt remainder;

		N.parts.resize(max_int.parts.count);
		quotient.parts.resize(max_int.parts.count);
		remainder.parts.resize(max_int.parts.count);

		for (umm bit_index = max_int.parts.count * 64 - 1; bit_index != ~(umm)0; --bit_index) {
			remainder <<= 1;
			remainder.set_bit(0, N.get_bit(bit_index));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(bit_index, 1);
			}
		}
		return quotient;
	}
	BigInt operator%(BigInt const &b) const {
		BigInt min_int = *this, max_int = b;
		if (min_int.parts.count > max_int.parts.count) {
			swap(min_int, max_int);
		}

		BigInt N = *this;
		BigInt quotient;
		BigInt remainder;

		N.parts.resize(max_int.parts.count);
		quotient.parts.resize(max_int.parts.count);
		remainder.parts.resize(max_int.parts.count);

		for (umm bit_index = max_int.parts.count * 64 - 1; bit_index != ~(umm)0; --bit_index) {
			remainder <<= 1;
			remainder.set_bit(0, N.get_bit(bit_index));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(bit_index, 1);
			}
		}
		return remainder;
	}


	BigInt &operator-=(BigInt b) { return *this = *this - b; }
	BigInt &operator*=(BigInt b) { return *this = *this * b; }
	BigInt &operator/=(BigInt b) { return *this = *this / b; }
	BigInt &operator<<=(u64 b) { return *this = *this << b; }
#endif
};


template <> inline constexpr bool is_integer<BigInt> = true;
template <> inline constexpr bool is_integer_like<BigInt> = true;
template <> inline constexpr bool is_signed<BigInt> = false;


inline BigInt make_big_int(u64 value TL_LP) {
	return {.msb = false, .parts = make_list({value} TL_LA)};
}
inline BigInt make_big_int(u64 high, u64 low TL_LP) {
	return {.msb = false, .parts = make_list({low, high} TL_LA)};
}
inline BigInt make_big_int(u64 high, u64 mid, u64 low TL_LP) {
	return {.msb = false, .parts = make_list({low, mid, high} TL_LA)};
}
inline BigInt operator""_ib(u64 value) {
	return {.msb = false, .parts = make_list({value})};
}
BigInt copy(BigInt const &that TL_LPD) {
	return {.msb = that.msb, .parts = copy(that.parts TL_LA)};
}
void free(BigInt &a) {
	free(a.parts);
}

inline umm append(StringBuilder &builder, BigInt value) {
	umm chars_appended = 0;
	auto append = [&] (StringBuilder &builder, auto const &x) {
		chars_appended += ::tl::append(builder, x);
	};

	append(builder, "0x"s);
	append(builder, FormatInt{.value=value.parts.back(), .radix=16});
	for (smm i = (smm)value.parts.count - 2; i >= 0; --i) {
		append(builder, FormatInt{.value=value.parts[i], .radix=16, .leading_zero_count=16});
	}

	return chars_appended;
}

}
