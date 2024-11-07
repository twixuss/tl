#include "common.h"
#include "math.h"

#include <concepts>
#include <bit>

namespace tl {

struct Turns {
	f32 value;
	friend Turns operator+(Turns a, Turns b) { return {frac(a.value + b.value)}; }
	friend Turns operator+(Turns a, f32 b) { return {frac(a.value + b)}; }
};

inline f32 sin_bhaskara(Turns turns) {
	f32 v = frac(turns.value);
	auto mask = v >= 0.5f;
	v = select(mask, v - 0.5f, v);
	f32 v8mvv16 = v*(8 - v*16);
	return v8mvv16 * reciprocal(5 - v8mvv16) * select(mask, -4.0f, 4.0f);
}


inline f32 cos_bhaskara(Turns turns) { return sin_bhaskara(turns + 0.25f); }

inline v2f cos_sin_bhaskara(Turns turns) { return {cos_bhaskara(turns), sin_bhaskara(turns)}; }

}
