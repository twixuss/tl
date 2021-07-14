#include <tl/common.h>
#include <tl/console.h>
using namespace tl;

void allocator_test(Allocator allocator) {
	for (u32 i = 0; i < 1024; ++i) {
		u32 *x = allocator.allocate<u32>();
		assert(x);
		assert(*x == 0);
	}
}

void common_test() {
	allocator_test(default_allocator);
	allocator_test(temporary_allocator);
	static_assert(tl::floor(-4, 3) == -6);
	static_assert(tl::floor(-3, 3) == -3);
	static_assert(tl::floor(-2, 3) == -3);
	static_assert(tl::floor(-1, 3) == -3);
	static_assert(tl::floor( 0, 3) == 0);
	static_assert(tl::floor( 1, 3) == 0);
	static_assert(tl::floor( 2, 3) == 0);
	static_assert(tl::floor( 3, 3) == 3);
	static_assert(tl::floor(0u, 3u) == 0);
	static_assert(tl::floor(1u, 3u) == 0);
	static_assert(tl::floor(2u, 3u) == 0);
	static_assert(tl::floor(3u, 3u) == 3);
	static_assert(tl::floor(4u, 3u) == 3);
	static_assert(tl::floor(5u, 3u) == 3);
	static_assert(tl::floor(6u, 3u) == 6);

	static_assert(tl::ceil(-3, 3) == -3);
	static_assert(tl::ceil(-2, 3) == 0);
	static_assert(tl::ceil(-1, 3) == 0);
	static_assert(tl::ceil( 0, 3) == 0);
	static_assert(tl::ceil( 1, 3) == 3);
	static_assert(tl::ceil( 2, 3) == 3);
	static_assert(tl::ceil( 3, 3) == 3);
	static_assert(tl::ceil( 4, 3) == 6);
	static_assert(tl::ceil(0u, 3u) == 0u);
	static_assert(tl::ceil(1u, 3u) == 3u);
	static_assert(tl::ceil(2u, 3u) == 3u);
	static_assert(tl::ceil(3u, 3u) == 3u);
	static_assert(tl::ceil(4u, 3u) == 6u);
	static_assert(tl::ceil(5u, 3u) == 6u);
	static_assert(tl::ceil(6u, 3u) == 6u);
	static_assert(tl::ceil(7u, 3u) == 9u);

	assert(tl::is_negative(-0.0f));
	assert(!tl::is_negative(0.0f));

	assert(tl::log( 1u, 2u) == 0);
	assert(tl::log( 2u, 2u) == 1);
	assert(tl::log( 3u, 2u) == 1);
	assert(tl::log( 4u, 2u) == 2);
	assert(tl::log( 5u, 2u) == 2);
	assert(tl::log( 6u, 2u) == 2);
	assert(tl::log( 7u, 2u) == 2);
	assert(tl::log( 8u, 2u) == 3);
	assert(tl::log(  1u, 10u) == 0);
	assert(tl::log(  9u, 10u) == 0);
	assert(tl::log( 10u, 10u) == 1);
	assert(tl::log( 11u, 10u) == 1);
	assert(tl::log( 99u, 10u) == 1);
	assert(tl::log(100u, 10u) == 2);
	assert(tl::log(101u, 10u) == 2);

	static_assert(tl::pow(1, 0) == 1);
	static_assert(tl::pow(2, 0) == 1);
	static_assert(tl::pow(3, 0) == 1);
	static_assert(tl::pow(4, 0) == 1);
	static_assert(tl::pow(1, 1) == 1);
	static_assert(tl::pow(2, 1) == 2);
	static_assert(tl::pow(3, 1) == 3);
	static_assert(tl::pow(4, 1) == 4);
	static_assert(tl::pow(1, 2) == 1);
	static_assert(tl::pow(2, 2) == 4);
	static_assert(tl::pow(3, 2) == 9);
	static_assert(tl::pow(4, 2) == 16);
	static_assert(tl::pow(1, 3) == 1);
	static_assert(tl::pow(2, 3) == 8);
	static_assert(tl::pow(3, 3) == 27);
	static_assert(tl::pow(4, 3) == 64);
	static_assert(tl::pow(1, 4) == 1);
	static_assert(tl::pow(2, 4) == 16);
	static_assert(tl::pow(3, 4) == 81);
	static_assert(tl::pow(4, 4) == 256);

	static_assert(tl::midpoint(0, 0) == 0);
	static_assert(tl::midpoint(0, 1) == 0);
	static_assert(tl::midpoint(0, 2) == 1);
	static_assert(tl::midpoint(0, 3) == 1);
	static_assert(tl::midpoint(0, 4) == 2);

	static_assert(tl::midpoint(1, 0) == 0);
	static_assert(tl::midpoint(1, 1) == 1);
	static_assert(tl::midpoint(1, 2) == 1);
	static_assert(tl::midpoint(1, 3) == 2);
	static_assert(tl::midpoint(1, 4) == 2);

	static_assert(tl::midpoint((u32)0xFFFFFFFF, (u32)0) == (u32)0x7FFFFFFF);
	static_assert(tl::midpoint((u32)0, (u32)0xFFFFFFFF) == (u32)0x7FFFFFFF);
	static_assert(tl::midpoint((u32)0xFFFFFFFD, (u32)0xFFFFFFFF) == (u32)0xFFFFFFFE);
	static_assert(tl::midpoint((u32)0xFFFFFFFF, (u32)0xFFFFFFFD) == (u32)0xFFFFFFFE);
	static_assert(tl::midpoint((s32)0xFFFFFFFD, (s32)0xFFFFFFFF) == (s32)0xFFFFFFFE);
	static_assert(tl::midpoint((s32)0xFFFFFFFF, (s32)0xFFFFFFFD) == (s32)0xFFFFFFFE);

#define T(x, t) \
	assert(tl::floor_to_power_of_2((u8 )x) == t);\
	assert(tl::floor_to_power_of_2((u16)x) == t);\
	assert(tl::floor_to_power_of_2((u32)x) == t);\
	assert(tl::floor_to_power_of_2((u64)x) == t);\
	static_assert(tl::CE::floor_to_power_of_2((u8)x) == t);\
	static_assert(tl::CE::floor_to_power_of_2((u16)x) == t);\
	static_assert(tl::CE::floor_to_power_of_2((u32)x) == t);\
	static_assert(tl::CE::floor_to_power_of_2((u64)x) == t);

	T(0, 0);
	T(1, 1);
	T(2, 2);
	T(3, 2);
	T(4, 4);
	T(5, 4);
	T(6, 4);
	T(7, 4);
	T(8, 8);
	T(9, 8);

#undef T

#define T(x, t) \
	assert(tl::ceil_to_power_of_2((u8)x) == t);\
	assert(tl::ceil_to_power_of_2((u16)x) == t);\
	assert(tl::ceil_to_power_of_2((u32)x) == t);\
	assert(tl::ceil_to_power_of_2((u64)x) == t);\
	static_assert(tl::CE::ceil_to_power_of_2((u8)x) == t);\
	static_assert(tl::CE::ceil_to_power_of_2((u16)x) == t);\
	static_assert(tl::CE::ceil_to_power_of_2((u32)x) == t);\
	static_assert(tl::CE::ceil_to_power_of_2((u64)x) == t);

	T(0, 0);
	T(1, 1);
	T(2, 2);
	T(3, 4);
	T(4, 4);
	T(5, 8);
	T(6, 8);
	T(7, 8);
	T(8, 8);
	T(9, 16);

#undef T

	int arr[] = {0,1,2,3,4,5,6,7};
	rotate(array_as_span(arr), 2);

	assert(arr[0] == 2);
	assert(arr[1] == 3);
	assert(arr[2] == 4);
	assert(arr[3] == 5);
	assert(arr[4] == 6);
	assert(arr[5] == 7);
	assert(arr[6] == 0);
	assert(arr[7] == 1);

	rotate(array_as_span(arr), -2);

	assert(arr[0] == 0);
	assert(arr[1] == 1);
	assert(arr[2] == 2);
	assert(arr[3] == 3);
	assert(arr[4] == 4);
	assert(arr[5] == 5);
	assert(arr[6] == 6);
	assert(arr[7] == 7);

}
