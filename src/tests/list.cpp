#pragma once

#include <tl/list.h>
#include <tl/console.h>

using namespace tl;

void list_test() {
	Queue<int> test;

	assert(test.count == 0);
	test.push(42);
	assert(test.count == 1);
	test.push(69);
	assert(test.count == 2);
	test.push(23);
	assert(test.count == 3);

	assert(test.pop().value() == 42);

	assert(test.count == 2);

	assert(test[0] == 69);
	assert(test[1] == 23);

	free(test);
}
