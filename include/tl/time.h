#pragma once
#include "system.h"

namespace TL {

struct PreciseTimer {
	s64 counter;
};

TL_API PreciseTimer create_precise_timer();
TL_API f64 get_time(PreciseTimer timer);
TL_API f64 reset_timer(PreciseTimer &timer);

TL_API u64 read_timestamp_counter();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "win32.h"

namespace TL {

static s64 frequency = getPerformanceFrequency();

PreciseTimer create_precise_timer() {
	return {get_performance_counter()};
}
f64 get_time(PreciseTimer timer) {
	return (f64)(get_performance_counter() - timer.counter) / frequency;
}
f64 reset_timer(PreciseTimer &timer) {
	s64 end = get_performance_counter();
	defer { timer.counter = end; };
	return (f64)(end - timer.counter) / frequency;
}

u64 read_timestamp_counter() {
	return __rdtsc();
}

}

#endif

#endif
