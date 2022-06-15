#pragma once
#include "system.h"

namespace tl {

struct PreciseTimer {
	u64 counter;
};

TL_API PreciseTimer create_precise_timer();
TL_API f64 get_time(PreciseTimer timer);
TL_API f64 reset(PreciseTimer &timer);

TL_API u64 read_timestamp_counter();

TL_API List<char> get_time_string();

TL_API void make_os_timing_precise();

TL_API void sync(u64 &frame_time_counter, f32 frame_time);

struct Date {
	u16 year;        // 1601 - 30827
	u8 month;        // 1(January) - 12(December)
	u8 day;          // 1 - 31
	u8 week_day;     // 0(Sunday) - 6(Saturday)
	u8 hour;         // 0 - 23
	u8 minute;       // 0 - 59
	u8 second;       // 0 - 59
	u16 millisecond; // 0 - 999
};

TL_API Date get_date();

inline Span<utf8> month_string(u8 month) {
	switch (month) {
		case  1: return u8"January"s;
		case  2: return u8"February"s;
		case  3: return u8"March"s;
		case  4: return u8"April"s;
		case  5: return u8"May"s;
		case  6: return u8"June"s;
		case  7: return u8"July"s;
		case  8: return u8"August"s;
		case  9: return u8"September"s;
		case 10: return u8"October"s;
		case 11: return u8"November"s;
		case 12: return u8"December"s;
	}
	return u8"UnknownMonth"s;
}

inline umm append(StringBuilder &b, Date d) {
	return append_format(b,
		"{} {} {} - {}:{}:{}",
		d.day,
		month_string(d.month),
		d.year,
		FormatInt{.value=d.hour, .leading_zero_count=2},
		FormatInt{.value=d.minute, .leading_zero_count=2},
		FormatInt{.value=d.second, .leading_zero_count=2}
	);
}

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "win32.h"

#pragma comment(lib, "winmm")

namespace tl {

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
	GetLocalTime(&t);

	return format("{}:{}:{}.{}", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

void make_os_timing_precise() {
	timeBeginPeriod(1);
}

void sync(u64 &frame_time_counter, f32 frame_time) {
	auto target_counter = frame_time_counter + (u64)(performance_frequency * frame_time);

	auto now = get_performance_counter();

	if (now < target_counter) {
		while ((s64)(target_counter - get_performance_counter()) > (s64)(performance_frequency / 1000))
			Sleep(1);

		while (get_performance_counter() < target_counter) {}

		frame_time_counter = target_counter;
	} else {
		frame_time_counter = now;
	}

}
Date get_date() {
	SYSTEMTIME t;
	GetLocalTime(&t);

	return {
		.year = t.wYear,
		.month = (u8)t.wMonth,
		.day = (u8)t.wDay,
		.hour = (u8)t.wHour,
		.minute = (u8)t.wMinute,
		.second = (u8)t.wSecond,
		.millisecond = t.wMilliseconds,
	};
}

}

#endif

#endif
