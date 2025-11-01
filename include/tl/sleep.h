#pragma once
#include "common.h"

namespace tl {

TL_API void sleep_nanoseconds(u64 nanoseconds);
TL_API void sleep_milliseconds(u64 milliseconds);
TL_API void sleep_seconds(u64 seconds);
TL_API void sleep_forever();

}

#ifdef TL_IMPL
#if OS_WINDOWS

namespace tl {

u64 current_precise_counter();
extern u64 precise_counter_resolution;

extern "C" void __stdcall Sleep(unsigned long milliseconds);
extern "C" int __stdcall SwitchToThread();
extern "C" void _mm_pause();

void sleep_nanoseconds(u64 nanoseconds) {
	auto target = current_precise_counter() + precise_counter_resolution * nanoseconds / 1'000'000'000;

	// if two milliseconds remaining, sleep for one.
	while (current_precise_counter() + precise_counter_resolution * 2 < target) {
		sleep_milliseconds(1);
	}

	// spin for remaining time
	while (current_precise_counter() < target) {
		_mm_pause();
	}
}
void sleep_milliseconds(u64 milliseconds) {
	constexpr u32 max_milliseconds = 0xffff'fffe;
	while (milliseconds > max_milliseconds) {
		Sleep(max_milliseconds);
		milliseconds -= max_milliseconds;
	}
	Sleep(milliseconds);
}
void sleep_seconds(u64 seconds) {
	constexpr u32 max_milliseconds = 1'000'000'000;
	while (seconds > max_milliseconds / 1000) {
		Sleep(max_milliseconds);
		seconds -= max_milliseconds / 1000;
	}
	Sleep(seconds * 1000);
}
void sleep_forever() {
	Sleep(0xffff'ffff);
}

}

#include "time.h"

#elif OS_LINUX

#include <time.h>

namespace tl {

void sleep_nanoseconds(u64 nanoseconds) {
	timespec t = {};
	t.tv_sec = nanoseconds / 1'000'000'000;
	t.tv_nsec = nanoseconds % 1'000'000'000;
	nanosleep(&t, 0);
}
void sleep_milliseconds(u64 milliseconds) {
	timespec t = {};
	t.tv_sec = milliseconds / 1'000;
	t.tv_nsec = (milliseconds % 1'000) * 1'000'000;
	nanosleep(&t, 0);
}
void sleep_seconds(u64 seconds) {
	timespec t = {};
	t.tv_sec = seconds;
	t.tv_nsec = 0;
	nanosleep(&t, 0);
}
void sleep_forever() {
	timespec t = {};
	t.tv_sec = std::numeric_limits<decltype(t.tv_sec)>::max();
	t.tv_nsec = 999'999'999;
	nanosleep(&t, 0);
}

}


#endif
#endif