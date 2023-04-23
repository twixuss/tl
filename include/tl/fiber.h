#include "common.h"

namespace tl {

struct Fiber {
	void *handle = 0;
};

TL_API Fiber fiber_init(void *parameter);
TL_API Fiber fiber_create(void (*start_address)(void *), void *parameter);
TL_API void fiber_destroy(Fiber fiber);
TL_API void fiber_yield(Fiber into);

#ifdef TL_IMPL
#if OS_WINDOWS

Fiber fiber_init(void *parameter) {
	return {ConvertThreadToFiberEx(parameter, 0)};
}
Fiber fiber_create(void (*start_address)(void *), void *parameter) {
	return {CreateFiberEx(0, 0, 0, start_address, parameter)};
}
void fiber_destroy(Fiber fiber) {
	DeleteFiber(fiber.handle);
}

void fiber_yield(Fiber into) {
	SwitchToFiber(into.handle);
}

#endif
#endif

}
