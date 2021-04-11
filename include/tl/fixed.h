#pragma once
#include "common.h"

namespace TL {

template <class Int, class Fract>
struct Fixed {
	Fixed operator+(Fixed that) {
		bool carry;
		Fixed result;
		add_carry(fraction, that.fraction, &result.fraction, &carry);
		result.integer = integer + that.integer + carry;
		return result;
	}

	Int   integer  = {};
	Fract fraction = {};
};

}
