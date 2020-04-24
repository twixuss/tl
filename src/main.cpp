#pragma warning(disable: 4711)
#pragma warning(disable: 4710)
#pragma warning(suppress: 4464)
#include "../include/tl/math.h"
#pragma warning(push, 0)
#pragma warning(disable: 5045)
#include <time.h>
#include <stdio.h>
#include <vector>
using namespace TL;
#define A2	V2f(1, 2)
#define A3	V3f(1, 2, 3)
#define A4	V4f(1, 2, 3, 4)
#define B2	V2f(5, 6)
#define B3	V3f(5, 6, 7)
#define B4	V4f(5, 6, 7, 8)
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
mask32x4 approxEqual(f32x4 a, f32x4 b) { return abs(a - b) < FLT_EPSILON; }
mask32x4 approxEqual(v2fx4 a, v2fx4 b) { 
	auto v = abs(a - b);
	return v.x < FLT_EPSILON && v.y < FLT_EPSILON; 
}
f32 _a;
template<class T>
void raycastTest() {
	struct Random {
		s32 seed = 0;
		s32 s32() { return seed = randomize(seed); }
		u32 u32() { return (::u32)s32(); }
		f32 f32() { return (s32() / 256) * (1.0f / 8388608.f); }
		bool u1() { return u32() & 0x10; }
		s32x4 s32x4() { return {s32(), s32(), s32(), s32()}; }
		s32x8 s32x8() { return {s32(), s32(), s32(), s32(), s32(), s32(), s32(), s32()}; }
		u32x4 u32x4() { return (::u32x4)s32x4(); }
		u32x8 u32x8() { return (::u32x8)s32x8(); }
		f32x4 f32x4() { return (::f32x4)(s32x4() / 256) * (1.0f / 8388608.f); }
		f32x8 f32x8() { return (::f32x8)(s32x8() / 256) * (1.0f / 8388608.f); }
	} random;
	for (u32 j = 0; j < 1024 * 16; ++j) {
		mask32x<widthOf<T>> result{};
		T a4{};
		T b4{};
		T c4{};
		T d4{};
		T point4{}, normal4{};
		for (u32 i = 0; i < widthOf<T>; ++i) {
			v2f a = {random.f32(), random.f32()};
			v2f b = {random.f32(), random.f32()};
			v2f c = {random.f32(), random.f32()};
			v2f d = {random.f32(), random.f32()};
			v2f point, normal;
			result[i]			= raycastLine(a, b, c, d, point, normal) ? ~0u : 0u;
			a4[i]		= a;
			b4[i]		= b;
			c4[i]		= c;
			d4[i]		= d;
			point4[i]	= point;
			normal4[i] = normal;
		}
		a4		= pack(a4);
		b4		= pack(b4);
		c4		= pack(c4);
		d4		= pack(d4);
		point4	= pack(point4);
		normal4 = pack(normal4);

		T point4new, normal4new;
		auto resultNew = raycastLine(a4, b4, c4, d4, point4new, normal4new);
		auto pdiff = abs(point4new - point4);
		auto ndiff = abs(normal4new - normal4);
		test(allTrue(resultNew == result), true);
		test(allTrue((pdiff.x < FLT_EPSILON && pdiff.y < FLT_EPSILON) == result), true);
		test(allTrue((ndiff.x < FLT_EPSILON && ndiff.y < FLT_EPSILON) == result), true);
		_a = 1;
	}
	for (u32 j = 0; j < 1024 * 16; ++j) {
		mask32x<widthOf<T>> result{};
		T a4{};
		T b4{};
		T c4{};
		T d4{};
		T point4{}, normal4{};
		for (u32 i = 0; i < widthOf<T>; ++i) {
			v2f a = {random.f32(), random.f32()};
			v2f b = {random.f32(), random.f32()};
			v2f c = {random.f32(), random.f32()};
			v2f d = {random.f32(), random.f32()};
			v2f point, normal;
			result[i]			= raycastRect(a, b, c, d, point, normal) ? ~0u : 0u;
			a4[i]		= a;
			b4[i]		= b;
			c4[i]		= c;
			d4[i]		= d;
			point4[i]	= point;
			normal4[i] = normal;
		}
		a4		= pack(a4);
		b4		= pack(b4);
		c4		= pack(c4);
		d4		= pack(d4);
		point4	= pack(point4);
		normal4 = pack(normal4);

		T point4new, normal4new;
		auto resultNew = raycastRect(a4, b4, c4, d4, point4new, normal4new);
		auto pdiff = abs(point4new - point4);
		auto ndiff = abs(normal4new - normal4);
		test(allTrue(resultNew == result), true);
		test(allTrue(((pdiff.x < FLT_EPSILON) && (pdiff.y < FLT_EPSILON)) == result), true);
		test(allTrue(((ndiff.x < FLT_EPSILON) && (ndiff.y < FLT_EPSILON)) == result), true);
		_a = 1;
	}
}
template<class T>
void funTest() {
	T t{};
	t.data();
	t.s;
#define OP(op) t op t; t op##= t; t op 1; t op##= 1;
	if constexpr(std::is_same_v<RemoveQualifiers<decltype(t.s[0])>::Type, f32>) {
		OP(+)OP(-)OP(*)OP(/)
	} else if constexpr(std::is_same_v<RemoveQualifiers<decltype(t.s[0])>::Type, s32>) {
		OP(+)OP(-)OP(*)OP(/)
		OP(|)OP(&)OP(^)OP(%)
	} else if constexpr(std::is_same_v<RemoveQualifiers<decltype(t.s[0])>::Type, u32>) {
		OP(+)OP(-)OP(*)OP(/)
		OP(|)OP(&)OP(^)OP(%)
	} else {
		static_assert(false);	
	}
}
template<class T, class Fn>
void perfTest(char const* name, Fn&& op) {
	std::vector<T> fracArr(1024*1024);
	auto begin = __rdtsc();
	for(auto& v : fracArr) {
		op(v);
	}
	printf("%s time: %llu cy\n", name, (__rdtsc() - begin) / fracArr.size());
}
struct v4 {
	f32 x,y,z,w;
};
void mathTest() {
	//perfTest<v4f>("x4", [](auto& v){ v += 15.5f; });
	//perfTest<v4 >("x1", [](auto& v){ v.x += 15.5f; v.y += 15.5f; v.z += 15.5f; v.w += 15.5f; });
	//funTest<v2f>(); funTest<v3f>(); funTest<v4f>();
	//funTest<v2s>(); funTest<v3s>();	funTest<v4s>();
	//funTest<v2u>(); funTest<v3u>();	funTest<v4u>();
	//funTest<v2fx4>(); funTest<v3fx4>(); funTest<v4fx4>();
	//funTest<v2sx4>(); funTest<v3sx4>();	funTest<v4sx4>();
	//funTest<v2ux4>(); funTest<v3ux4>();	funTest<v4ux4>();
	//funTest<v2fx8>(); funTest<v3fx8>(); funTest<v4fx8>();
	//funTest<v2sx8>(); funTest<v3sx8>();	funTest<v4sx8>();
	//funTest<v2ux8>(); funTest<v3ux8>();	funTest<v4ux8>();

	static constexpr auto magic  = 135521;
	static constexpr auto magic4 = V4s(135521, 6177, 46178, 51758);

#define TEST(op) \
	perfTest<v4s>(#op "=s4", [](auto& v){ v op##= magic; }); perfTest<v4s>(#op "=s1", [](auto& v){ v.x op##= magic; v.y op##= magic; v.z op##= magic; v.w op##= magic; });\
	perfTest<v4s>(#op " s4", [](auto& v){ v = v op magic; }); perfTest<v4s>(#op " s1", [](auto& v){ v.x = v.x op magic; v.y = v.y op magic; v.z = v.z op magic; v.w = v.w op magic; });\
	perfTest<v4s>(#op "=p4", [](auto& v){ v op##= magic4; }); perfTest<v4s>(#op "=p1", [](auto& v){ v.x op##= magic4.data()[0]; v.y op##= magic4.data()[1]; v.z op##= magic4.data()[2]; v.w op##= magic4.data()[3]; });\
	perfTest<v4s>(#op " p4", [](auto& v){ v = v op magic4; }); perfTest<v4s>(#op " p1", [](auto& v){ v.x = v.x op magic4.data()[0]; v.y = v.y op magic4.data()[1]; v.z = v.z op magic4.data()[2]; v.w = v.w op magic4.data()[3]; });

	TEST(+);
	TEST(-);
	TEST(*);
	TEST(/);
	TEST(%);
	TEST(^);
	TEST(|);
	TEST(&);

	//perfTest<v4sx4>("mod", [](auto& v){ v %= magic; });
	//perfTest<v4sx8>("mod", [](auto& v){ v %= magic; });
	//raycastTest<v2fx4>();
	//raycastTest<v2fx8>();
	
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