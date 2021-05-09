#pragma once
#include "system.h"

namespace TL {

struct PreciseTimer {
	s64 counter;
};

TL_API PreciseTimer create_precise_timer();
TL_API f64 get_time(PreciseTimer timer);
TL_API f64 reset(PreciseTimer &timer);

TL_API u64 read_timestamp_counter();

TL_API List<char> get_time_string();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "win32.h"

namespace TL {

PreciseTimer create_precise_timer() {
	return {get_performance_counter()};
}
f64 get_time(PreciseTimer timer) {
	return (f64)(get_performance_counter() - timer.counter) / performance_frequency;
}
f64 reset(PreciseTimer &timer) {
	s64 end = get_performance_counter();
	defer { timer.counter = end; };
	return (f64)(end - timer.counter) / performance_frequency;
}

u64 read_timestamp_counter() {
	return __rdtsc();
}

List<char> get_time_string() {
	SYSTEMTIME t;
	GetSystemTime(&t);

	return format("%:%:%.%", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

}

#endif

#endif
