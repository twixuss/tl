#pragma once
#include "common.h"
#include "console.h"
#include "string.h"

/*
#define TL_LOGGER_CONTEXT_MEMBERS \
	x(tl::Logger, logger) \
	x(tl::Logger, app_logger) \
	x(tl::Logger, tl_logger) \

*/

namespace tl {

#define TL_ENUMERATE_LOGGER_SEVERITIES \
x(debug) \
x(info) \
x(warning) \
x(error) \

enum class LogSeverity : u8 {
	#define x(name) name,
	TL_ENUMERATE_LOGGER_SEVERITIES
	#undef x
};

inline void append(StringBuilder &builder, LogSeverity s) {
	switch (s) {
		#define x(name) case LogSeverity::name: return append(builder, u8###name##s);
		TL_ENUMERATE_LOGGER_SEVERITIES
		#undef x
	}
	return append_format(builder, "LogSeverity({})", (int)s);
}

inline ConsoleColor to_color(LogSeverity severity) {
	switch (severity) {
		default:
		case LogSeverity::debug:   return ConsoleColor::cyan;
		case LogSeverity::info:    return ConsoleColor::gray;
		case LogSeverity::warning: return ConsoleColor::yellow;
		case LogSeverity::error:   return ConsoleColor::red;
	}
}

template <class Derived>
struct LoggerBase {
	inline void log(LogSeverity severity, auto &&...args) {
		derived().impl(severity, (Span<utf8>)(TL_TMP(tl::format(args...))));
	}
	inline void debug  (auto &&...args) { log(LogSeverity::debug,   args...); }
	inline void info   (auto &&...args) { log(LogSeverity::info,    args...); }
	inline void warning(auto &&...args) { log(LogSeverity::warning, args...); }
	inline void error  (auto &&...args) { log(LogSeverity::error,   args...); }

	Derived &derived() { return *(Derived *)this; }
};

inline void _initial_logger_proc(struct Logger *logger, LogSeverity severity, Span<utf8> message) {
	println("({}) {}", severity, message);
}

struct Logger : LoggerBase<Logger> {
	void (*func)(Logger *logger, LogSeverity severity, Span<utf8> message) = _initial_logger_proc;
	void *state = 0;

	void impl(LogSeverity severity, Span<utf8> message) { 
		func(this, severity, message);
	}
};

#ifndef TL_USE_CONTEXT
extern TL_API Logger app_logger;
extern TL_API Logger tl_logger;
extern TL_API thread_local Logger current_logger;
#endif

namespace global_log {

inline void log(LogSeverity severity, char const *format, auto &&...args) {
	current_logger.log(severity, format, args...);
}
inline void log_debug  (char const *format, auto &&...args) { current_logger.debug  (format, args...); }
inline void log_info   (char const *format, auto &&...args) { current_logger.info   (format, args...); }
inline void log_warning(char const *format, auto &&...args) { current_logger.warning(format, args...); }
inline void log_error  (char const *format, auto &&...args) { current_logger.error  (format, args...); }

}

#ifdef TL_IMPL

#ifndef TL_USE_CONTEXT
Logger app_logger;
Logger tl_logger;
thread_local Logger current_logger;
#endif

#endif

}
