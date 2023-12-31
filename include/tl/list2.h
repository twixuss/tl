#include "common.h"
#include "vector.h"

template <class T, class Allocator, class Size>
struct List2 {
	T *data;
	v2umm count;
	umm capacity;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	void set(T *new_data, v2umm new_count) {
	}
};