#include "../include/tl/math.h"
#include <time.h>
using namespace TL;
#define A2	V2(1, 2)
#define A3	V3(1, 2, 3)
#define A4	V4(1, 2, 3, 4)
#define B2	V2(5, 6)
#define B3	V3(5, 6, 7)
#define B4	V4(5, 6, 7, 8)
#define A2i V2s(1, 2)
#define A3i V3s(1, 2, 3)
#define A4i V4s(1, 2, 3, 4)
#define B2i V2s(5, 6)
#define B3i V3s(5, 6, 7)
#define B4i V4s(5, 6, 7, 8)
template <class T, class U>
void test(T t, U u) {
	ASSERT(t == u);
}
void mathTest() {
	struct Random {
		s32 seed = (TL::s32)time(0);
		s32 s32() { return seed = randomize(seed); }
		u32 u32() { return (::u32)s32(); }
		f32 f32() { return (s32() / 256) * (1.0f / 8388608.f); }
		bool u1() { return u32() & 0x10; }
		s32x4 s32x4() { return {s32(), s32(), s32(), s32()}; }
		s32x8 s32x8() { return {s32(), s32(), s32(), s32(), s32(), s32(), s32(), s32()}; }
		u32x4 u32x4() { return U32x4(s32x4()); }
		u32x8 u32x8() { return U32x8(s32x8()); }
		f32x4 f32x4() { return F32x4(s32x4() / 256) * (1.0f / 8388608.f); }
		f32x8 f32x8() { return F32x8(s32x8() / 256) * (1.0f / 8388608.f); }
	} random;
	for (u32 j = 0; j < 1024*128; ++j) {
		mask32x4 result;
		v2x4 a4;
		v2x4 b4;
		v2x4 c4;
		v2x4 d4;
		v2x4 point4, normal4;
		for (u32 i = 0; i < 4; ++i) {
			v2 a = {random.f32(), random.f32()};
			v2 b = {random.f32(), random.f32()};
			v2 c = {random.f32(), random.f32()};
			v2 d = {random.f32(), random.f32()};
			v2 point, normal;
			result[i]		   = raycastLine(a, b, c, d, point, normal) ? ~0u : 0u;
			((v2*)&a4)[i]	   = a;
			((v2*)&b4)[i]	   = b;
			((v2*)&c4)[i]	   = c;
			((v2*)&d4)[i]	   = d;
			((v2*)&point4)[i]  = point;
			((v2*)&normal4)[i] = normal;
		}
		a4		= pack(a4);
		b4		= pack(b4);
		c4		= pack(c4);
		d4		= pack(d4);
		point4	= pack(point4);
		normal4 = pack(normal4);

		v2x4 point4new, normal4new;
		auto resultNew = raycastLine(a4, b4, c4, d4, point4new, normal4new);
		test(allOne(resultNew == result), true);
		test(allOne((point4new == point4) == result), true);
		test(allOne((normal4new == normal4) == result), true);
	}
	for (u32 j = 0; j < 1024*128; ++j) {
		mask32x4 result;
		v2x4 a4;
		v2x4 b4;
		v2x4 c4;
		v2x4 d4;
		v2x4 point4, normal4;
		for (u32 i = 0; i < 4; ++i) {
			v2 a = {random.f32(), random.f32()};
			v2 b = {random.f32(), random.f32()};
			v2 c = {random.f32(), random.f32()};
			v2 d = {random.f32(), random.f32()};
			v2 point, normal;
			result[i]		   = raycastRect(a, b, c, d, point, normal) ? ~0u : 0u;
			((v2*)&a4)[i]	   = a;
			((v2*)&b4)[i]	   = b;
			((v2*)&c4)[i]	   = c;
			((v2*)&d4)[i]	   = d;
			((v2*)&point4)[i]  = point;
			((v2*)&normal4)[i] = normal;
		}
		a4		= pack(a4);
		b4		= pack(b4);
		c4		= pack(c4);
		d4		= pack(d4);
		point4	= pack(point4);
		normal4 = pack(normal4);

		v2x4 point4new, normal4new;
		auto resultNew = raycastRect(a4, b4, c4, d4, point4new, normal4new);
		test(allOne(resultNew == result), true);
		test(allOne((point4new == point4) == result), true);
		test(allOne((normal4new == normal4) == result), true);
	}
	{
		test(A4 + B4, V4(6, 8, 10, 12));
		test(A4 - B4, V4(-4, -4, -4, -4));
		test(A4 * B4, V4(5, 12, 21, 32));
		test(B4 / A4, V4(5, 3, 7.f / 3, 2));
	}
	{
		test(A4i + B4i, V4s(6, 8, 10, 12));
		test(A4i - B4i, V4s(-4, -4, -4, -4));
		test(A4i * B4i, V4s(5, 12, 21, 32));
		test(B4i / A4i, V4s(5, 3, 7 / 3, 2));
		test(B4i % A4i, V4s(0, 0, 1, 0));
	}
	{
		test(A2 + 0.5f, V2(1.5f, 2.5f));
		test(A3 + 0.5f, V3(1.5f, 2.5f, 3.5f));
		test(A4 + 0.5f, V4(1.5f, 2.5f, 3.5f, 4.5f));
	}

	test(dot(A2, B2), 17);
	test(dot(A3, B3), 38);
	test(dot(A4, B4), 70);
	test(dot(A2i, B2i), 17);
	test(dot(A3i, B3i), 38);
	test(dot(A4i, B4i), 70);

	test(normalize(A4), A4 / 5.477225575051661134569697828008f);
	test(normalize(A4), A4 / sqrtf(1 * 1 + 2 * 2 + 3 * 3 + 4 * 4));

	f32 arr[4]{0, 1, 2, 3};
	test(linearSample(arr, 0), 0);
	test(linearSample(arr, 0.125f), 0.5f);
	test(linearSample(arr, 0.25f), 1);
	test(linearSample(arr, 0.375f), 1.5f);
	test(linearSample(arr, 0.5f), 2);
	test(linearSample(arr, 0.625f), 2.5);
	test(linearSample(arr, 0.75f), 3);
	test(linearSample(arr, 0.875f), 1.5f);
	test(linearSample(arr, 1), 0);
	test(frac(-6, 5), 4);
	test(frac(-5, 5), 0);
	test(frac(-4, 5), 1);
	test(frac(-3, 5), 2);
	test(frac(-2, 5), 3);
	test(frac(-1, 5), 4);
	test(frac(0, 5), 0);
	test(frac(1, 5), 1);
	test(frac(2, 5), 2);
	test(frac(3, 5), 3);
	test(frac(4, 5), 4);
	test(frac(5, 5), 0);
	test(frac(6, 5), 1);

	test(floor(-6, 3), -6);
	test(floor(-5, 3), -6);
	test(floor(-4, 3), -6);
	test(floor(-3, 3), -3);
	test(floor(-2, 3), -3);
	test(floor(-1, 3), -3);
	test(floor(0, 3), 0);
	test(floor(1, 3), 0);
	test(floor(2, 3), 0);
	test(floor(3, 3), 3);
	test(floor(4, 3), 3);
	test(floor(5, 3), 3);
	test(floor(6, 3), 6);
	test(floor(7, 3), 6);
	test(floor(8, 3), 6);

	test(roundToInt(-1), -1);
	test(roundToInt(-0.6), -1);
	test(roundToInt(-0.5), 0);
	test(roundToInt(-0.4), 0);
	test(roundToInt(0), 0);
	test(roundToInt(0.4), 0);
	test(roundToInt(0.5), 1);
	test(roundToInt(0.6), 1);
	test(roundToInt(1), 1);

	test(roundToInt(A2 + 0.5f), A2i + 1);
	test(roundToInt(A3 + 0.5f), A3i + 1);
	test(roundToInt(A4 + 0.5f), A4i + 1);
}
int main() { mathTest(); }