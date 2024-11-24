#pragma once
#include "common.h"

namespace tl {

struct Fiber {
	void *handle = 0;
};

TL_API Fiber init_fiber(void *parameter);
TL_API Fiber create_fiber(void (*start_address)(void *), void *parameter);
TL_API void free(Fiber &fiber);
TL_API void yield(Fiber into);
TL_API Fiber get_current_fiber();

inline Fiber init_or_get_current_fiber() {
	auto fiber = init_fiber(0);
	if (!fiber.handle) {
		fiber = get_current_fiber();
	}
	return fiber;
}

#ifdef TL_IMPL
#if OS_WINDOWS

Fiber init_fiber(void *parameter) {
	return {ConvertThreadToFiberEx(parameter, 0)};
}
Fiber create_fiber(void (*start_address)(void *), void *parameter) {
	return {CreateFiberEx(0, 0, 0, start_address, parameter)};
}
void free(Fiber &fiber) {
	DeleteFiber(fiber.handle);
	fiber = {};
}

void yield(Fiber into) {
	SwitchToFiber(into.handle);
}

Fiber get_current_fiber() {
	return {GetCurrentFiber()};
}

#endif
#endif

}
