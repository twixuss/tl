#include "common.h"

namespace tl {

struct s128 {
	s64 low  = 0;
	s64 high = 0;
};

TL_API s128 multiply(s64 a, s64 b);
TL_API s64 divide(s128 a, s64 b);

#ifdef TL_IMPL

#if OS_WINDOWS

s128 multiply(s64 a, s64 b) {
	s128 result;

	result.low = Multiply128(a, b, &result.high);

	return result;
}
s64 divide(s128 a, s64 b) {
	s64 remainder;
	return _div128(a.high, a.low, b, &remainder);
}

#endif

#endif
}
