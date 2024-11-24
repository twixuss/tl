#pragma once
/*

Basic example at the end

*/

#include "fiber.h"
#include "function.h"

#include <setjmp.h>

namespace tl {

struct ReusableFiber : Fiber {
	struct Data {
		void (*start_address)(void *) = 0;
		void *parameter = 0;
		jmp_buf reuse_point = {};
		bool should_jump_back = false;
	};

	Allocator allocator = Allocator::current();
	Data *data = 0; // Needs to be stable in memory.
};

TL_API ReusableFiber create_reusable_fiber();
TL_API void set_start(ReusableFiber fiber, void (*start_address)(void *), void *parameter);
 // NOTE: `current` must be the running fiber
TL_API void jump_back(ReusableFiber current);
 // NOTE: `current` must be the running fiber
TL_API void yield_reuse(Fiber into, ReusableFiber current);
TL_API void free(ReusableFiber &fiber);

}

#ifdef TL_IMPL
#if OS_WINDOWS

namespace tl {

ReusableFiber create_reusable_fiber() {
	ReusableFiber result = {};
	result.data = result.allocator.allocate<ReusableFiber::Data>();
	result.handle = CreateFiberEx(0, 0, 0, [](void *parameter) {
		auto data = (ReusableFiber::Data *)parameter;
		setjmp(data->reuse_point);
		data->should_jump_back = false;
		data->start_address(data->parameter);
	}, result.data);
	return result;
}
void jump_back(ReusableFiber current) {
	longjmp(current.data->reuse_point, 1);
}
void yield_reuse(Fiber into, ReusableFiber current) {
	SwitchToFiber(into.handle);
	if (current.data->should_jump_back) {
		jump_back(current);
	}
}
void set_start(ReusableFiber fiber, void(*start_address)(void *), void *parameter) {
	fiber.data->start_address = start_address;
	fiber.data->parameter = parameter;
	fiber.data->should_jump_back = true;
}

void free(ReusableFiber &fiber) {
	DeleteFiber(fiber.handle);
	fiber.allocator.free(fiber.data);
	fiber = {};
}

}

#endif
#endif

#if TL_COMPILE_EXAMPLES

namespace tl {

void _reusable_fiber_basic_example() {
	auto main_fiber = init_or_get_current_fiber();

	auto child = create_reusable_fiber();
	
	auto foo = create_fat_function_pointer([&] {
		println("foo1");
		yield_reuse(main_fiber, child);
		println("foo2"); // will not run because we'll override with `bar`
	});

	auto bar = create_fat_function_pointer([&] {
		println("bar");
		yield_reuse(main_fiber, child);
	});

	set_start(child, foo.function, foo.parameter);
	yield(child);

	set_start(child, bar.function, bar.parameter);
	yield(child);
}

}

#endif