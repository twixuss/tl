#include "../include/tl/math.h"
using namespace TL;
#define A2	V2(1, 2)
#define A3	V3(1, 2, 3)
#define A4	V4(1, 2, 3, 4)
#define B2	V2(5, 6)
#define B3	V3(5, 6, 7)
#define B4	V4(5, 6, 7, 8)
#define A2i V2i(1, 2)
#define A3i V3i(1, 2, 3)
#define A4i V4i(1, 2, 3, 4)
#define B2i V2i(5, 6)
#define B3i V3i(5, 6, 7)
#define B4i V4i(5, 6, 7, 8)
void mathTest() {
	{
		ASSERT(A4 + B4 == V4(6, 8, 10, 12));
		ASSERT(A4 - B4 == V4(-4, -4, -4, -4));
		ASSERT(A4 * B4 == V4(5, 12, 21, 32));
		ASSERT(B4 / A4 == V4(5, 3, 7.f / 3, 2));
	}
	{
		ASSERT(A4i + B4i == V4i(6, 8, 10, 12));
		ASSERT(A4i - B4i == V4i(-4, -4, -4, -4));
		ASSERT(A4i * B4i == V4i(5, 12, 21, 32));
		ASSERT(B4i / A4i == V4i(5, 3, 7 / 3, 2));
		ASSERT(B4i % A4i == V4i(0, 0, 1, 0));
	}

	ASSERT(dot(A2, B2) == 17);
	ASSERT(dot(A3, B3) == 38);
	ASSERT(dot(A4, B4) == 70);
	ASSERT(dot(A2i, B2i) == 17);
	ASSERT(dot(A3i, B3i) == 38);
	ASSERT(dot(A4i, B4i) == 70);

	ASSERT(normalize(A4) == A4 / 5.477225575051661134569697828008f);
	ASSERT(normalize(A4) == A4 / sqrtf(1 * 1 + 2 * 2 + 3 * 3 + 4 * 4));

	f32 _mathTestArr[4]{0, 1, 2, 3};
	ASSERT(linearSample(_mathTestArr, 0) == 0);
	ASSERT(linearSample(_mathTestArr, 0.125f) == 0.5f);
	ASSERT(linearSample(_mathTestArr, 0.25f) == 1);
	ASSERT(linearSample(_mathTestArr, 0.375f) == 1.5f);
	ASSERT(linearSample(_mathTestArr, 0.5f) == 2);
	ASSERT(linearSample(_mathTestArr, 0.625f) == 2.5);
	ASSERT(linearSample(_mathTestArr, 0.75f) == 3);
	ASSERT(linearSample(_mathTestArr, 0.875f) == 1.5f);
	ASSERT(linearSample(_mathTestArr, 1) == 0);
	ASSERT(frac(-6, 5) == 4);
	ASSERT(frac(-5, 5) == 0);
	ASSERT(frac(-4, 5) == 1);
	ASSERT(frac(-3, 5) == 2);
	ASSERT(frac(-2, 5) == 3);
	ASSERT(frac(-1, 5) == 4);
	ASSERT(frac(0, 5) == 0);
	ASSERT(frac(1, 5) == 1);
	ASSERT(frac(2, 5) == 2);
	ASSERT(frac(3, 5) == 3);
	ASSERT(frac(4, 5) == 4);
	ASSERT(frac(5, 5) == 0);
	ASSERT(frac(6, 5) == 1);
	ASSERT(floor(-6, 3) == -2);
	ASSERT(floor(-5, 3) == -2);
	ASSERT(floor(-4, 3) == -2);
	ASSERT(floor(-3, 3) == -1);
	ASSERT(floor(-2, 3) == -1);
	ASSERT(floor(-1, 3) == -1);
	ASSERT(floor(0, 3) == 0);
	ASSERT(floor(1, 3) == 0);
	ASSERT(floor(2, 3) == 0);
	ASSERT(floor(3, 3) == 1);
	ASSERT(floor(4, 3) == 1);
	ASSERT(floor(5, 3) == 1);
	ASSERT(floor(6, 3) == 2);
	ASSERT(roundToInt(-1) == -1);
	ASSERT(roundToInt(-0.6) == -1);
	ASSERT(roundToInt(-0.5) == -1);
	ASSERT(roundToInt(-0.4) == 0);
	ASSERT(roundToInt(0) == 0);
	ASSERT(roundToInt(0.4) == 0);
	ASSERT(roundToInt(0.5) == 1);
	ASSERT(roundToInt(0.6) == 1);
	ASSERT(roundToInt(1) == 1);
}
int main() { mathTest(); }