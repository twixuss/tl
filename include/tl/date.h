#pragma once
#include "common.h"
#include "list.h"
#include "string.h"

namespace tl {

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

inline List<char> get_time_string() {
	auto d = get_date();
	return format("{}:{}:{}.{}", d.hour, d.minute, d.second, d.millisecond);
}

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

inline void append(StringBuilder &b, Date d) {
	append_format(b,
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

namespace tl {

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