#pragma once

#include <tl/list.h>

using namespace tl;

void list_test() {
	Queue<int> test;

	assert(test.allocator != 0);
	assert(test.data == 0);
	assert(test.count == 0);
	assert(test.alloc_data == 0);
	assert(test.alloc_count == 0);

	test.push(42);

	assert(test.count == 1);
	assert(test.alloc_count == 1);
	assert(test.data[0] == 42);
	assert(test.alloc_data[0] == 42);

	test.push(69);

	assert(test.count == 2);
	assert(test.alloc_count == 2);
	assert(test.data[0] == 42);
	assert(test.data[1] == 69);
	assert(test.alloc_data[0] == 42);
	assert(test.alloc_data[1] == 69);

	test.push(23);

	assert(test.count == 3);
	assert(test.alloc_count == 4);
	assert(test.data[0] == 42);
	assert(test.data[1] == 69);
	assert(test.data[2] == 23);
	assert(test.alloc_data[0] == 42);
	assert(test.alloc_data[1] == 69);
	assert(test.alloc_data[2] == 23);

	test.pop();

	assert(test.count == 2);
	assert(test.alloc_count == 4);
	assert(test.data[0] == 69);
	assert(test.data[1] == 23);
	assert(test.alloc_data[0] == 42);
	assert(test.alloc_data[1] == 69);
	assert(test.alloc_data[2] == 23);

	free(test);
}
