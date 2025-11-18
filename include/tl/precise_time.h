#pragma once
#include "system.h"
#include "sleep.h"

namespace tl {

TL_API u64 current_precise_counter();
TL_API extern u64 precise_counter_resolution;


struct PreciseTimer {
	u64 counter;
};

inline f64 current_precise_time() {
	return (f64)current_precise_counter() / precise_counter_resolution;
}

inline PreciseTimer create_precise_timer() {
	return {current_precise_counter()};
}
inline f64 elapsed_time(PreciseTimer timer) {
	return (f64)(current_precise_counter() - timer.counter) / precise_counter_resolution;
}
inline f64 reset(PreciseTimer &timer) {
	s64 end = current_precise_counter();
	defer { timer.counter = end; };
	return (f64)(end - timer.counter) / precise_counter_resolution;
}

inline void sync(PreciseTimer &timer, f32 target_delta_time) {
	auto target_counter = timer.counter + (u64)(precise_counter_resolution * target_delta_time);

	auto now = current_precise_counter();

	if (now < target_counter) {
		while ((s64)(target_counter - current_precise_counter()) > (s64)(precise_counter_resolution / 1000))
			sleep_milliseconds(1);

		while (current_precise_counter() < target_counter) {}

		timer.counter = target_counter;
	} else {
		timer.counter = now;
	}
}

TL_API u64 read_timestamp_counter();

TL_API void make_os_timing_precise();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "win32.h"
#include "thread.h"

#pragma comment(lib, "winmm")

namespace tl {

u64 current_precise_counter() {
	return get_performance_counter();
}

u64 precise_counter_resolution = [] {
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	return f.QuadPart;
}();

u64 read_timestamp_counter() {
	return __rdtsc();
}

void make_os_timing_precise() {
	timeBeginPeriod(1);
}

}

#elif OS_LINUX

#include <time.h>

namespace tl {

u64 current_precise_counter() {
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return {(u64)t.tv_sec * (u64)1'000'000'000 + (u64)t.tv_nsec};
}

u64 precise_counter_resolution = 1'000'000'000;

u64 read_timestamp_counter() {
	return __rdtsc();
}

void make_os_timing_precise() {
}

}

#endif

#endif
