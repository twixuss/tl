#include <stdio.h>
#include "../include/tl/math.h"
#include <typeinfo>

using namespace TL;

void _assert(bool v, char const *expression) {
	if (!v) {
		DEBUG_BREAK;
		puts("\nAssertion failed");
		puts(expression);
		exit(-1);
	}
}
void _assert(b32x4 v, char const *expression) { _assert(allTrue(v), expression); }
void _assert(b32x8 v, char const *expression) { _assert(allTrue(v), expression); }
void _assert(b64x2 v, char const *expression) { _assert(allTrue(v), expression); }
void _assert(b64x4 v, char const *expression) { _assert(allTrue(v), expression); }

void noop() {}

#define assert(...) _assert(__VA_ARGS__, #__VA_ARGS__)

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
			set<isSame<decltype(result), Vector>>(check.s[i], op data[i]);\
		}\
		assert(result == check);\
		noop();\
	} while(0)

#define TEST_BINARY(op) \
	do {\
		auto result = a op b;\
		decltype(result) check;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Vector>>(check.s[i], data[i] op data[i+width]);\
		}\
		assert(result == check);\
		result = a op scalar;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Vector>>(check.s[i], data[i] op scalar);\
		}\
		assert(result == check);\
		result = scalar op b;\
		for (u32 i = 0; i < width; ++i) {\
			set<isSame<decltype(result), Vector>>(check.s[i], scalar op data[i+width]);\
		}\
		assert(result == check);\
		noop();\
	} while(0)

#define TEST_PREFIX								\
	printf("%s... ", typeid(T).name());			\
	using Scalar = typename T::Scalar;			\
	using Vector = typename T::Vector;			\
	constexpr u32 width = widthOf<Vector>;		\
	Scalar data[width*2];			            \
	for (u32 i = 0; i < _countof(data); ++i) {	\
		data[i] = (Scalar)(i + 1);				\
	}											\
	Scalar scalar = 1;                          \
	Vector a = load<Vector>(data + 0);			\
	Vector b = load<Vector>(data + width);		\
	for (u32 i = 0; i < width; ++i) {			\
		assert(a[i] == data[i]);				\
		assert(b[i] == data[i + width]);		\
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
	puts("ok");
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
	puts("ok");
}

int main() {
	//F32x4(1); F32x4(1,2,3,4);
	//S32x4(1); S32x4(1,2,3,4);
	//U32x4(1); U32x4(1,2,3,4);
	
	test_float<f32x4>();
	test_float<f32x8>();
	test_float<f64x2>();
	test_float<f64x4>();
	test_int<s32x4>();
	test_int<s32x8>();
	test_int<u32x4>();
	test_int<u32x8>();
}