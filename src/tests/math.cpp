#include <tl/math.h>
using namespace tl;

void math_test() {
	aabb<v2s> a, b;
	a = {{0, 0}, {10, 10}};
	b = {{5, 5}, {15, 15}};

	auto c = subtract_volumes(a, b);
	assert(c.count == 3);
	assert(find(c, aabb<v2s>{{0,0},{ 5, 5}}));
	assert(find(c, aabb<v2s>{{0,5},{ 5,10}}));
	assert(find(c, aabb<v2s>{{5,0},{10, 5}}));

	c = subtract_points(a, b);
	assert(c.count == 3);
	assert(find(c, aabb<v2s>{{0,0},{ 4, 4}}));
	assert(find(c, aabb<v2s>{{0,5},{ 4,10}}));
	assert(find(c, aabb<v2s>{{5,0},{10, 4}}));
}
