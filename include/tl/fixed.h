#pragma once
#include "common.h"

namespace TL {

template <class Int, class Fract>
Int fixed_addition_overflow(Fract a, Fract b, Fract *result) {
	static_assert(false);
}

template <>
s32 fixed_addition_overflow(f32 a, f32 b, f32 *result) {
	f32 sum = a + b;
	if (sum > 1) {
	
	}
	return 0;
}

template <class Int, class Fract>
struct Fixed {
	Fixed operator+(Fixed that) {
		bool carry;
		Fixed result;
		result.integer = integer + that.integer;
		result.integer += fixed_addition_overflow(fraction, that.fraction, &result.fraction);
		return result;
	}

	Int   integer  = {};
	Fract fraction = {};
};

}
