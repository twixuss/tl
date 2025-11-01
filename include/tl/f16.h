#pragma once
// 
//  WARNING: This header is unfinished. Some functions may not produce expected results.
// 
// TODO: write tests
//

#include "common.h"
#include "string.h"

namespace tl {

union f32Parts {
	struct {
		u32 mantissa : 23;
		u32 exponent : 8;
		u32 sign : 1;
	};
	u32 bits;
	f32 value;

	constexpr s32 decoded_exponent() {
		return (s32)(exponent - 127) << 24 >> 24;
	}
	constexpr void encode_exponent(s32 e) { exponent = e + 127; }
};

union f16 {
	struct {
		u16 mantissa : 10;
		u16 exponent :  5;
		u16 sign     :  1;
	};
	u16 bits;

	constexpr s16 decoded_exponent() {
		return (s16)(exponent - 15) << 11 >> 11;
	}
	constexpr void encode_exponent(s16 e) { exponent = e + 15; }

	constexpr operator f32() const {
		if (exponent == 0x1f) {
			// NaN / Infinity
			return f32Parts {
				.mantissa = mantissa,
				.exponent = 0xff,
				.sign = sign,
			}.value;
		}

		if (exponent == 0) {
			if (mantissa == 0) {
				// Zero
				return f32Parts{.sign = sign}.value;
			}
			// Subnormal
			auto x = count_leading_zeros(mantissa);
			return f32Parts {
				.mantissa = (u32)mantissa << (x + 8),
				.exponent = exponent - x + 118,
				.sign = sign,
			}.value;
		}
		return f32Parts {
			.mantissa = (u32)mantissa << 13,
			.exponent = (u32)exponent + 112,
			.sign = sign,
		}.value;
	}

	constexpr friend f16 operator+(f16 a) { return a; }
	constexpr friend f16 operator-(f16 a) { return a.sign ^= 1, a; }

	constexpr friend f16 operator+(f16 a, f16 b) {
		return from_f32((f32)a + (f32)b);

		if (a.decoded_exponent() < b.decoded_exponent()) {
			Swap(a, b);
		} else if (a.decoded_exponent() == b.decoded_exponent() && a.mantissa < b.mantissa) {
			Swap(a, b);
		}

		if (a.is_nan()) {
			return a;
		}
		if (b.is_nan()) {
			return b;
		}

		if (a.is_inf() && b.is_inf()) {
			if (a.sign == b.sign) {
				return a;
			} else {
				return make_nan();
			}
		}

		// HACK
		if (a.exponent == 0 && b.exponent == 0) {
			return {};
		}

		u32 a_mantissa = 1024 * (a.exponent != 0) + a.mantissa;
		u32 b_mantissa = 1024 * (b.exponent != 0) + b.mantissa;
		b_mantissa >>= a.decoded_exponent() - b.decoded_exponent();

		u32 new_mantissa = a_mantissa + b_mantissa * ((a.sign == b.sign) * 2 - 1);
		
		if (a.sign != b.sign && new_mantissa == 0) {
			return {};
		}

		s32 x = log2(new_mantissa) - 10;
		new_mantissa = rotate_right(new_mantissa, x);
		a.exponent += x;

		if (a.exponent == 0) {
			if (new_mantissa >= 1024) {
				a.exponent = 1;
			}
		}

		if (a.exponent == 31) {
			a.mantissa = 0;
		} else {
			a.mantissa = new_mantissa;
		}

		return a;
	}

	constexpr friend f16 operator*(f16 a, f16 b) {
		return from_f32((f32)a * (f32)b);

		if (a.exponent == 0 && b.exponent == 0) {
			return { .sign = (u16)(a.sign ^ b.sign), };
		}

		if (a.exponent == 0 || b.exponent == 0) {
			if (a.exponent == 0) {
				Swap(a, b);
			}

			// 1.am * 0.bm * 2^(ae-15+be-14);

			u32 m = (1024 + (u32)a.mantissa) * ((u32)b.mantissa) / 1024;
			u32 e = a.exponent + b.exponent - 14;
			u32 x = 25 - count_leading_zeros(m);
			m <<= x;
			e -= x;

			m -= 1024;

			return {
				.mantissa = (u16)m,
				.exponent = (u16)e,
				.sign = (u16)(a.sign ^ b.sign),
			};
		}

		a.sign ^= b.sign;
		a.encode_exponent(clamp(a.decoded_exponent() + b.decoded_exponent(), -15, 16));
		u32 new_mantissa = (a.mantissa + 1024) * (b.mantissa + 1024) / 1024;
		bool x = new_mantissa >= 2048;
		a.exponent += x;
		a.mantissa = (new_mantissa >> x) - 1024;
		return a;
	}
	constexpr friend f16 operator/(f16 a, f16 b) {
		return from_f32((f32)a / (f32)b);

		a.sign ^= b.sign;
		a.encode_exponent(a.decoded_exponent() - b.decoded_exponent());
		u32 new_mantissa = (a.mantissa + 1024) * 1024 / (b.mantissa + 1024);
		bool x = new_mantissa < 1024;
		a.exponent -= x;
		a.mantissa = (new_mantissa << x) - 1024;
		return a;
	}

	constexpr friend f16 operator-(f16 a, f16 b) { return a + -b; }
	constexpr f16 &operator+=(f16 that) { return *this = *this + that; }
	constexpr f16 &operator-=(f16 that) { return *this = *this - that; }
	constexpr f16 &operator*=(f16 that) { return *this = *this * that; }
	constexpr f16 &operator/=(f16 that) { return *this = *this / that; }

	constexpr bool is_nan() const {
		return exponent == 31 && mantissa;
	}
	constexpr bool is_inf() const {
		return exponent == 31 && !mantissa;
	}

	static constexpr f16 from_f32(f32 v) {
		f32Parts parts;
		parts.value = v;

		if (parts.exponent == 0xff) {
			// NaN / Infinity
			return {
				.mantissa = parts.mantissa != 0,
				.exponent = 0x1f,
				.sign     = (u16)parts.sign,
			};
		}


		auto e = (s32)parts.exponent - 127 + 15;
		if (e <= 0) {
			if (e < -10) {
				// Zero & 32-bit subnormals
				return {.sign = (u16)parts.sign};
			}

			// 16-bit Subnormals
			u32 m = parts.mantissa | 0x00800000;
			u32 round_bit = 1 << (13 - e);
			u32 bits = (m >> (14 - e)) + ((m & round_bit) != 0 && (m & (3 * round_bit - 1)) != 0);

			return {
				.mantissa = (u16)(bits),
				.exponent = (u16)(bits >> 10),
				.sign = (u16)parts.sign,
			};
		}


		if ((parts.bits & 0x7fff'ffff) > 0x477fe000) {
			// too big, set to infinity.
			return {
				.mantissa = 0,
				.exponent = 0x1f,
				.sign     = (u16)parts.sign,
			};
		}

		u32 roundBit = 0x00001000;
		return {
			.mantissa = (u16)(((parts.mantissa & roundBit) != 0 && (parts.mantissa&(3*roundBit-1)) != 0) ? ((parts.mantissa >> 13) + 1) : (parts.mantissa >> 13)),
			.exponent = (u16)(parts.exponent - (127 - 15)),
			.sign     = (u16)parts.sign,
		};
	}
	static constexpr f16 make_nan() {
		return {.mantissa = 1, .exponent = 31};
	}
};

template<>
inline constexpr f16 nan<f16> = f16::make_nan();

constexpr f16 to_f16(f32 v) {
	return f16::from_f32(v);
}

template <>
forceinline constexpr f16 convert(f32 from) {
	return to_f16(from);
}

template <>
forceinline constexpr f32 convert(f16 from) {
	return (f32)from;
}

inline void append(StringBuilder &builder, FormatFloat<f16> fmt) {
	auto v = fmt.value;
	if (v.exponent == 31) {
		if (v.mantissa == 0) {
			return append_format(builder, "{}inf", v.sign ? "-" : "");
		} else {
			return append(builder, "NaN");
		}
	}
	if (v.exponent == 0) {
		if (v.mantissa == 0) {
			return append_format(builder, "{}0", v.sign ? "-" : "");
		} else {
			f32 f = (v.sign?-1:1)*powf(2,(f32)v.exponent-14)*((f32)v.mantissa / 1024);
			return append(builder, fmt.with_value(f));
		}
	}
	return append(builder, fmt.with_value((v.sign?-1:1)*powf(2,v.exponent-15)*((f32)(1024+v.mantissa) / 1024)));
}
inline void append(StringBuilder &builder, f16 v) {
	append(builder, FormatFloat{.value = v});
}

}
