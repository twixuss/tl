#pragma once
#include "common.h"
#include "console.h"
#include "thread.h"

namespace tl {

#define TL_ENUMERATE_LOGGER_SEVERITIES \
x(debug) \
x(info) \
x(warning) \
x(error) \

enum class LogSeverity {
	#define x(name) name,
	TL_ENUMERATE_LOGGER_SEVERITIES
	#undef x
};

struct LoggerBase {
	inline void log(this auto &&self, LogSeverity severity, char const *format, auto ...args) {
		self.impl(severity, with(temporary_allocator, tl::format(as_utf8(as_span(format)), args...)));
	}
	inline void debug  (this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::debug,   format, args...); }
	inline void info   (this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::info,    format, args...); }
	inline void warning(this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::warning, format, args...); }
	inline void error  (this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::error,   format, args...); }

	inline void debug  (this auto &&self, auto &&arg) { self.impl(LogSeverity::debug,   with(temporary_allocator, to_string(arg))); }
	inline void info   (this auto &&self, auto &&arg) { self.impl(LogSeverity::info,    with(temporary_allocator, to_string(arg))); }
	inline void warning(this auto &&self, auto &&arg) { self.impl(LogSeverity::warning, with(temporary_allocator, to_string(arg))); }
	inline void error  (this auto &&self, auto &&arg) { self.impl(LogSeverity::error,   with(temporary_allocator, to_string(arg))); }
};

struct Logger : LoggerBase {
	void (*func)(Logger *logger, LogSeverity severity, Span<utf8> message);
	void *state;

	void impl(LogSeverity severity, Span<utf8> message) { 
		func(this, severity, message);
	}
};

inline umm append(StringBuilder &builder, LogSeverity severity) {
	switch (severity) {
#define x(name) case LogSeverity::name: return append(builder, #name ## s);
		TL_ENUMERATE_LOGGER_SEVERITIES
#undef x
	}
	return 0;
}


extern TL_API Logger app_logger;
extern TL_API Logger tl_logger;
extern TL_API thread_local Logger current_logger;

namespace global_log {

inline void log(LogSeverity severity, char const *format, auto ...args) {
	current_logger.log(severity, format, args...);
}
inline void log_debug  (char const *format, auto ...args) { current_logger.debug  (format, args...); }
inline void log_info   (char const *format, auto ...args) { current_logger.info   (format, args...); }
inline void log_warning(char const *format, auto ...args) { current_logger.warning(format, args...); }
inline void log_error  (char const *format, auto ...args) { current_logger.error  (format, args...); }

}

#ifdef TL_IMPL

void _initial_logger_proc(Logger *logger, LogSeverity severity, Span<utf8> message) {
	println("({}) {}", severity, message);
}


Logger app_logger = {.func = _initial_logger_proc};
Logger tl_logger = {.func = _initial_logger_proc};
thread_local Logger current_logger = {.func = _initial_logger_proc};

#endif

}
