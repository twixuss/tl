#pragma once

#include <tl/sorted_list.h>

using namespace tl;

void sorted_list_test() {
	SortedList<int> list;

	list.add(11);

	assert(list.count == 1);
	assert(list[0] == 11);

	list.add(55);

	assert(list.count == 2);
	assert(list[0] == 11);
	assert(list[1] == 55);

	list.add(33);

	assert(list.count == 3);
	assert(list[0] == 11);
	assert(list[1] == 33);
	assert(list[2] == 55);

	list.add(33);

	assert(list.count == 4);
	assert(list[0] == 11);
	assert(list[1] == 33);
	assert(list[2] == 33);
	assert(list[3] == 55);

	list.add(22);

	assert(list.count == 5);
	assert(list[0] == 11);
	assert(list[1] == 22);
	assert(list[2] == 33);
	assert(list[3] == 33);
	assert(list[4] == 55);

	list.add(44);

	assert(list.count == 6);
	assert(list[0] == 11);
	assert(list[1] == 22);
	assert(list[2] == 33);
	assert(list[3] == 33);
	assert(list[4] == 44);
	assert(list[5] == 55);

}
