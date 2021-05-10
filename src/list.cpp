#pragma once

#include "../include/tl/list.h"

using namespace TL;

void list_test() {
	Queue<int> test;

	assert(test.allocator != 0);
	assert(test.data == 0);
	assert(test.size == 0);
	assert(test.alloc_data == 0);
	assert(test.alloc_size == 0);

	test.push(42);

	assert(test.size == 1);
	assert(test.alloc_size == 1);
	assert(test.data[0] == 42);
	assert(test.alloc_data[0] == 42);

	test.push(69);

	assert(test.size == 2);
	assert(test.alloc_size == 2);
	assert(test.data[0] == 42);
	assert(test.data[1] == 69);
	assert(test.alloc_data[0] == 42);
	assert(test.alloc_data[1] == 69);

	test.push(23);

	assert(test.size == 3);
	assert(test.alloc_size == 4);
	assert(test.data[0] == 42);
	assert(test.data[1] == 69);
	assert(test.data[2] == 23);
	assert(test.alloc_data[0] == 42);
	assert(test.alloc_data[1] == 69);
	assert(test.alloc_data[2] == 23);

	test.pop();

	assert(test.size == 2);
	assert(test.alloc_size == 4);
	assert(test.data[0] == 69);
	assert(test.data[1] == 23);
	assert(test.alloc_data[0] == 42);
	assert(test.alloc_data[1] == 69);
	assert(test.alloc_data[2] == 23);

	free(test);
}
