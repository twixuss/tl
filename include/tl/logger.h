#pragma once
#include "common.h"
#include "console.h"
#include "thread.h"

namespace tl {

#define TL_ENUMERATE_LOGGER_SEVERITIES \
x(info) \
x(warning) \
x(error) \

struct Logger {
	enum class Severity {
#define x(name) name,
		TL_ENUMERATE_LOGGER_SEVERITIES
#undef x
	};

	void (*func)(Logger *logger, Severity severity, Span<utf8> message);
	void *state;

	void log(Severity severity, Span<utf8> message) { 
		func(this, severity, message);
	}

	inline void log(Severity severity, char const *format, auto ...args) {
		log(severity, with(temporary_allocator, tl::format(as_utf8(as_span(format)), args...)));
	}
	inline void info   (char const *format, auto ...args) { log(Severity::info,    format, args...); }
	inline void warning(char const *format, auto ...args) { log(Severity::warning, format, args...); }
	inline void error  (char const *format, auto ...args) { log(Severity::error,   format, args...); }

	inline void info   (auto &&arg) { log(Severity::info,    with(temporary_allocator, to_string(arg))); }
	inline void warning(auto &&arg) { log(Severity::warning, with(temporary_allocator, to_string(arg))); }
	inline void error  (auto &&arg) { log(Severity::error,   with(temporary_allocator, to_string(arg))); }
};

inline umm append(StringBuilder &builder, Logger::Severity severity) {
	switch (severity) {
#define x(name) case Logger::Severity::name: return append(builder, #name ## s);
		TL_ENUMERATE_LOGGER_SEVERITIES
#undef x
	}
}

extern TL_API Logger default_logger;
extern TL_API Logger tl_logger;
extern TL_API thread_local Logger current_logger;

namespace global_log {

inline void log(Logger::Severity severity, char const *format, auto ...args) {
	current_logger.log(severity, format, args...);
}
inline void log_info   (char const *format, auto ...args) { current_logger.info   (format, args...); }
inline void log_warning(char const *format, auto ...args) { current_logger.warning(format, args...); }
inline void log_error  (char const *format, auto ...args) { current_logger.error  (format, args...); }

}

extern TL_API OsLock stdout_mutex;

inline void default_log_proc(Logger *logger, Logger::Severity severity, Span<utf8> message) {
	scoped(temporary_allocator);

	auto color = [&] {
		switch (severity) {
			case Logger::Severity::info:    return ConsoleColor::dark_gray;
			case Logger::Severity::warning: return ConsoleColor::yellow;
			case Logger::Severity::error:   return ConsoleColor::red;
		}
		return ConsoleColor::red;
	}();

	with(stdout_mutex, with(color, println("[{}] {}", (char *)logger->state, message)));
}

#ifdef TL_IMPL

OsLock stdout_mutex = {};

Logger default_logger = {
	.func = default_log_proc,
	.state = (void *)"app",
};
Logger tl_logger = {
	.func = default_log_proc,
	.state = (void *)"tl", 
};
thread_local Logger current_logger = {
	.func = default_log_proc,
	.state = (void *)"app",
};

#endif

}
