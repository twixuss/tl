#pragma once
#include "common.h"
#include "console.h"
#include "thread.h"

namespace tl {

#define TL_ENUMERATE_LOGGER_SEVERITIES \
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
	inline void info   (this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::info,    format, args...); }
	inline void warning(this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::warning, format, args...); }
	inline void error  (this auto &&self, char const *format, auto ...args) { self.log(LogSeverity::error,   format, args...); }

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
}

extern TL_API OsLock stdout_mutex;


struct DefaultLogger : LoggerBase {
	Span<utf8> module;
	LogSeverity min_severity = TL_DEBUG ? LogSeverity::info : LogSeverity::warning;

	inline static void proc(Logger *logger, LogSeverity severity, Span<utf8> message) {
		return ((DefaultLogger *)logger->state)->impl(severity, message);
	}

	void impl(LogSeverity severity, Span<utf8> message) {
		if ((int)severity < (int)min_severity)
			return;

		scoped(temporary_allocator);

		auto color = [&] {
			switch (severity) {
				case LogSeverity::info:    return ConsoleColor::dark_gray;
				case LogSeverity::warning: return ConsoleColor::yellow;
				case LogSeverity::error:   return ConsoleColor::red;
			}
			return ConsoleColor::red;
		}();

		with(stdout_mutex, with(color, println("[{}] {}", module, message)));
	}
	operator Logger() {
		return {.func = proc, .state = this};
	}
};

extern TL_API DefaultLogger app_logger;
extern TL_API DefaultLogger tl_logger;
extern TL_API Logger default_logger;
extern TL_API thread_local Logger current_logger;

namespace global_log {

inline void log(LogSeverity severity, char const *format, auto ...args) {
	current_logger.log(severity, format, args...);
}
inline void log_info   (char const *format, auto ...args) { current_logger.info   (format, args...); }
inline void log_warning(char const *format, auto ...args) { current_logger.warning(format, args...); }
inline void log_error  (char const *format, auto ...args) { current_logger.error  (format, args...); }

}

#ifdef TL_IMPL

OsLock stdout_mutex = {};

DefaultLogger app_logger = { .module = u8"app"s };
DefaultLogger tl_logger = { .module = u8"tl"s };
Logger default_logger = app_logger;
thread_local Logger current_logger = default_logger;

#endif

}
