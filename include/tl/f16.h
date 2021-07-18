#pragma once
#include "system.h"

namespace tl {

struct f16 {
	u16 sign : 1;
	u16 exponent : 5;
	u16 mantissa : 10;
	operator f32() const {
		union Result {
			f32 value;
			struct {
				u32 sign : 1;
				u32 exponent : 8;
				u32 mantissa : 23;
			};
		} result;

		result.sign     = sign;
		result.exponent = exponent + (127 - 15);
		result.mantissa = mantissa << (23 - 10);

		return result.value;
	}
};

}
