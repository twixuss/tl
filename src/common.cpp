#include "../include/tl/common.h"
using namespace TL;

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
	static_assert(TL::floor(-4, 3) == -6);
	static_assert(TL::floor(-3, 3) == -3);
	static_assert(TL::floor(-2, 3) == -3);
	static_assert(TL::floor(-1, 3) == -3);
	static_assert(TL::floor( 0, 3) == 0);
	static_assert(TL::floor( 1, 3) == 0);
	static_assert(TL::floor( 2, 3) == 0);
	static_assert(TL::floor( 3, 3) == 3);
	static_assert(TL::floor(0u, 3u) == 0);
	static_assert(TL::floor(1u, 3u) == 0);
	static_assert(TL::floor(2u, 3u) == 0);
	static_assert(TL::floor(3u, 3u) == 3);
	static_assert(TL::floor(4u, 3u) == 3);
	static_assert(TL::floor(5u, 3u) == 3);
	static_assert(TL::floor(6u, 3u) == 6);

	static_assert(TL::ceil(-3, 3) == -3);
	static_assert(TL::ceil(-2, 3) == 0);
	static_assert(TL::ceil(-1, 3) == 0);
	static_assert(TL::ceil( 0, 3) == 0);
	static_assert(TL::ceil( 1, 3) == 3);
	static_assert(TL::ceil( 2, 3) == 3);
	static_assert(TL::ceil( 3, 3) == 3);
	static_assert(TL::ceil( 4, 3) == 6);
	static_assert(TL::ceil(0u, 3u) == 0u);
	static_assert(TL::ceil(1u, 3u) == 3u);
	static_assert(TL::ceil(2u, 3u) == 3u);
	static_assert(TL::ceil(3u, 3u) == 3u);
	static_assert(TL::ceil(4u, 3u) == 6u);
	static_assert(TL::ceil(5u, 3u) == 6u);
	static_assert(TL::ceil(6u, 3u) == 6u);
	static_assert(TL::ceil(7u, 3u) == 9u);

	assert(TL::is_negative(-0.0f));
	assert(!TL::is_negative(0.0f));

	assert(TL::log( 1u, 2u) == 0);
	assert(TL::log( 2u, 2u) == 1);
	assert(TL::log( 3u, 2u) == 1);
	assert(TL::log( 4u, 2u) == 2);
	assert(TL::log( 5u, 2u) == 2);
	assert(TL::log( 6u, 2u) == 2);
	assert(TL::log( 7u, 2u) == 2);
	assert(TL::log( 8u, 2u) == 3);
	assert(TL::log(  1u, 10u) == 0);
	assert(TL::log(  9u, 10u) == 0);
	assert(TL::log( 10u, 10u) == 1);
	assert(TL::log( 11u, 10u) == 1);
	assert(TL::log( 99u, 10u) == 1);
	assert(TL::log(100u, 10u) == 2);
	assert(TL::log(101u, 10u) == 2);

	static_assert(TL::pow(1, 0) == 1);
	static_assert(TL::pow(2, 0) == 1);
	static_assert(TL::pow(3, 0) == 1);
	static_assert(TL::pow(4, 0) == 1);
	static_assert(TL::pow(1, 1) == 1);
	static_assert(TL::pow(2, 1) == 2);
	static_assert(TL::pow(3, 1) == 3);
	static_assert(TL::pow(4, 1) == 4);
	static_assert(TL::pow(1, 2) == 1);
	static_assert(TL::pow(2, 2) == 4);
	static_assert(TL::pow(3, 2) == 9);
	static_assert(TL::pow(4, 2) == 16);
	static_assert(TL::pow(1, 3) == 1);
	static_assert(TL::pow(2, 3) == 8);
	static_assert(TL::pow(3, 3) == 27);
	static_assert(TL::pow(4, 3) == 64);
	static_assert(TL::pow(1, 4) == 1);
	static_assert(TL::pow(2, 4) == 16);
	static_assert(TL::pow(3, 4) == 81);
	static_assert(TL::pow(4, 4) == 256);

	static_assert(TL::midpoint(0, 0) == 0);
	static_assert(TL::midpoint(0, 1) == 0);
	static_assert(TL::midpoint(0, 2) == 1);
	static_assert(TL::midpoint(0, 3) == 1);
	static_assert(TL::midpoint(0, 4) == 2);

	static_assert(TL::midpoint(1, 0) == 0);
	static_assert(TL::midpoint(1, 1) == 1);
	static_assert(TL::midpoint(1, 2) == 1);
	static_assert(TL::midpoint(1, 3) == 2);
	static_assert(TL::midpoint(1, 4) == 2);

	static_assert(TL::midpoint((u32)0xFFFFFFFF, (u32)0) == (u32)0x7FFFFFFF);
	static_assert(TL::midpoint((u32)0, (u32)0xFFFFFFFF) == (u32)0x7FFFFFFF);
	static_assert(TL::midpoint((u32)0xFFFFFFFD, (u32)0xFFFFFFFF) == (u32)0xFFFFFFFE);
	static_assert(TL::midpoint((u32)0xFFFFFFFF, (u32)0xFFFFFFFD) == (u32)0xFFFFFFFE);
	static_assert(TL::midpoint((s32)0xFFFFFFFD, (s32)0xFFFFFFFF) == (s32)0xFFFFFFFE);
	static_assert(TL::midpoint((s32)0xFFFFFFFF, (s32)0xFFFFFFFD) == (s32)0xFFFFFFFE);


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
