#include "../include/tl/math.h"

#pragma warning(push, 0)
#include <typeinfo>
#include <windows.h> // for EXCEPTION_ACCESS_VIOLATION
#include <excpt.h>

#if COMPILER_GCC
#include <cxxabi.h>
#endif

#include <stdio.h>
#pragma warning(pop)

using namespace TL;

void clearLine() {
	printf("\r                                                                                                                                \r");
}

template <class Mask>
void _assert(Mask mask, char const *expression, u32 line) {
	printf("test %s at line %u... ", expression, line);
	if (!allTrue(mask)) {
		puts("\nAssertion failed");
		puts(expression);
		DEBUG_BREAK;
		exit(-1);
	}
	puts("ok");
}

template <class T>
auto equals(T a, T b) { return a == b; }
b64x2 equals(f64x2 a, f64x2 b) { return absolute(a - b) < 0.000001f; }
b32x4 equals(f32x4 a, f32x4 b) { return absolute(a - b) < 0.000001f; }
b64x4 equals(f64x4 a, f64x4 b) { return absolute(a - b) < 0.000001f; }
b32x8 equals(f32x8 a, f32x8 b) { return absolute(a - b) < 0.000001f; }

void print(f32 v) { printf("f32{%.20f}", v); }
void print(f64 v) { printf("f64{%.20f}", v); }
void print(s32 v) { printf("s32{%d}", v); }
void print(u32 v) { printf("u32{%u}", v); }
void print(b64x2 v) { printf("b64x2{%d %d}", !!v[0], !!v[1]); }
void print(f64x2 v) { printf("f64x2{%.20f %.20f}", v[0], v[1]); }
void print(b32x4 v) { printf("b32x4{%d %d %d %d}", !!v[0], !!v[1], !!v[2], !!v[3]); }
void print(b64x4 v) { printf("b64x4{%d %d %d %d}", !!v[0], !!v[1], !!v[2], !!v[3]); }
void print(f32x4 v) { printf("f32x4{%.20f %.20f %.20f %.20f}", v[0], v[1], v[2], v[3]); }
void print(f64x4 v) { printf("f64x4{%.20f %.20f %.20f %.20f}", v[0], v[1], v[2], v[3]); }
void print(s32x4 v) { printf("s32x4{%d %d %d %d}", v[0], v[1], v[2], v[3]); }
void print(u32x4 v) { printf("u32x4{%u %u %u %u}", v[0], v[1], v[2], v[3]); }
void print(b32x8 v) { printf("b32x8{%d %d %d %d %d %d %d %d}", !!v[0], !!v[1], !!v[2], !!v[3], !!v[4], !!v[5], !!v[6], !!v[7]); }
void print(f32x8 v) { printf("f32x8{%.20f %.20f %.20f %.20f %.20f %.20f %.20f %.20f}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }
void print(s32x8 v) { printf("s32x8{%d %d %d %d %d %d %d %d}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }
void print(u32x8 v) { printf("u32x8{%u %u %u %u %u %u %u %u}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }

template <class T>
void _assertEqual(T a, T b, char const *expression, u32 line) {
	printf("%u: %s", line, expression);
	auto mask = equals(a, b);
	if (!allTrue(mask)) {
		puts("\nAssertion failed");
		puts(expression);
		printf("a: "); print(a); puts("");
		printf("b: "); print(b); puts("");
		printf("mask: "); print(mask); puts("");
		DEBUG_BREAK;
		exit(-1);
	}
	clearLine();
}

void noop() {}

template <class T>
void printType() {
#if COMPILER_GCC
	int status;
	auto realName = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	puts(realName);
	free(realName);
#else
	puts(typeid(T).name());
#endif
}

#define assert(...) _assert(__VA_ARGS__, #__VA_ARGS__, __LINE__)
#define assertEqual(a, b) _assertEqual(a, b, #a " == " #b, __LINE__)

template <bool isVector, class T, class U>
void set(T &dst, U src) {
	if constexpr (isVector)
		dst = src;
	else
		dst = (src) ? ~0 : 0;
}

#define TEST_UNARY(op) \
	do {\
		auto result = op a;\
		decltype(result) check;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Pack>>(check.s[i], op data[i]);\
		}\
		assertEqual(result, check);\
		noop();\
	} while(0)

#define TEST_BINARY(op) \
	do {\
		auto result = a op b;\
		decltype(result) check;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Pack>>(check.s[i], data[i] op data[i+width]);\
		}\
		assertEqual(result, check);\
		result = a op scalar;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Pack>>(check.s[i], data[i] op scalar);\
		}\
		assertEqual(result, check);\
		result = scalar op b;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Pack>>(check.s[i], scalar op data[i+width]);\
		}\
		assertEqual(result, check);\
		noop();\
	} while(0)

#define TEST_PREFIX							   \
	printType<T>();                            \
	using Scalar = typename T::Scalar;		   \
	using Pack = typename T::Pack;			   \
	constexpr u32 width = widthOf<Pack>;	   \
	alignas(64) Scalar data[width*2];		   \
	for (u32 i = 0; i < _countof(data); ++i) { \
		data[i] = (Scalar)(i + 1);			   \
	}										   \
	alignas(64) Scalar scalar = 1;                 \
	alignas(64) Pack a = load<Pack>(data + 0);	   \
	alignas(64) Pack b = load<Pack>(data + width); \
	for (u32 i = 0; i < width; ++i) {		   \
		assertEqual(a[i], data[i]);			   \
		assertEqual(b[i], data[i + width]);	   \
	}

template <class T>
void test_float() {
	TEST_PREFIX;
	TEST_UNARY(-);
	TEST_BINARY(+);
	TEST_BINARY(-);
	TEST_BINARY(*);
	TEST_BINARY(/);
	TEST_BINARY(>);
	TEST_BINARY(<);
	TEST_BINARY(>=);
	TEST_BINARY(<=);
	TEST_BINARY(==);
	TEST_BINARY(!=);
}

template <class T>
void test_int() {
	TEST_PREFIX;
	TEST_UNARY(!);
	TEST_UNARY(~);
	TEST_UNARY(-);
	TEST_BINARY(+);
	TEST_BINARY(-);
	TEST_BINARY(*);
	TEST_BINARY(/);
	TEST_BINARY(%);
	TEST_BINARY(^);
	TEST_BINARY(&);
	TEST_BINARY(|);
	TEST_BINARY(<<);
	TEST_BINARY(>>);
	TEST_BINARY(>);
	TEST_BINARY(<);
	TEST_BINARY(>=);
	TEST_BINARY(<=);
	TEST_BINARY(==);
	TEST_BINARY(!=);
}

int main() {
#ifdef TEST_SSE
	static_assert(!ARCH_AVX);
	static_assert(!ARCH_AVX2);
	static_assert(!ARCH_AVX512F);
	static_assert(!ARCH_AVX512DQ);
#endif
#ifdef TEST_AVX
	static_assert(ARCH_AVX);
	static_assert(!ARCH_AVX2);
	static_assert(!ARCH_AVX512F);
	static_assert(!ARCH_AVX512DQ);
#endif
#ifdef TEST_AVX2
	static_assert(ARCH_AVX);
	static_assert(ARCH_AVX2);
	static_assert(!ARCH_AVX512F);
	static_assert(!ARCH_AVX512DQ);
#endif
	F32x4(1); F32x4(1,2,3,4);
	S32x4(1); S32x4(1,2,3,4);
	U32x4(1); U32x4(1,2,3,4);
	test_float<f32x4>();
	test_float<f32x8>();
	test_float<f64x2>();
	test_float<f64x4>();
	test_int<s32x4>();
	test_int<s32x8>();
	test_int<u32x4>();
	test_int<u32x8>();
}
