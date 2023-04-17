#include "common.h"

namespace tl {

TL_DECLARE_HANDLE(Fiber);

TL_API Fiber fiber_init(void *parameter);
TL_API Fiber fiber_create(void (*start_address)(void *), void *parameter);
TL_API void fiber_destroy(Fiber fiber);
TL_API void fiber_yield(Fiber into);

#ifdef TL_IMPL
#if OS_WINDOWS

Fiber fiber_init(void *parameter) {
	return (Fiber)ConvertThreadToFiberEx(parameter, 0);
}
Fiber fiber_create(void (*start_address)(void *), void *parameter) {
	return (Fiber)CreateFiberEx(0, 0, 0, start_address, parameter);
}
void fiber_destroy(Fiber fiber) {
	DeleteFiber(fiber);
}

void fiber_yield(Fiber into) {
	SwitchToFiber(into);
}

#endif
#endif

}
