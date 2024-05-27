#pragma once
#include "logger.h"
#include "file.h"
#include "thread.h"
/*
#define TL_DEFAULT_LOGGER_MAIN_CONTEXT_MEMBERS \
	x(tl::File, default_logger_file) \
	x(tl::OsLock, stdout_mutex) \
*/

namespace tl {

#ifndef TL_USE_CONTEXT
extern TL_API OsLock stdout_mutex;
extern TL_API File default_logger_file;
#endif

struct TL_API DefaultLogger : LoggerBase {
	Span<utf8> module = {};
	File file = TL_GET_GLOBAL(default_logger_file);
	LogSeverity min_severity = TL_DEBUG ? LogSeverity::debug : LogSeverity::info;

	void impl(LogSeverity severity, Span<utf8> message) {
		if ((int)severity < (int)min_severity)
			return;

		scoped(TL_GET_CURRENT(temporary_allocator));

		auto color = [&] {
			switch (severity) {
				case LogSeverity::debug:   return ConsoleColor::cyan;
				case LogSeverity::info:    return ConsoleColor::dark_gray;
				case LogSeverity::warning: return ConsoleColor::yellow;
				case LogSeverity::error:   return ConsoleColor::red;
			}
			return ConsoleColor::red;
		}();

		withs(TL_GET_GLOBAL(stdout_mutex)) {
			with(color, println("[{}] {}", module, message));
			if (is_valid(file)) {
				write(file, as_bytes(tformat("[{}] ({}) {}\r\n", module, severity, message)));
			}
		};
	}
	operator Logger() {
		return {
			.func = [](Logger *logger, LogSeverity severity, Span<utf8> message) {
				return ((DefaultLogger *)logger->state)->impl(severity, message);
			}, 
			.state = this
		};
	}

	static void global_init(Span<utf8> shared_file_path);
};

#ifdef TL_IMPL

#ifndef TL_USE_CONTEXT
File default_logger_file;
OsLock stdout_mutex;
#endif

void DefaultLogger::global_init(Span<utf8> shared_file_path) {
	auto file = open_file(shared_file_path, {.write = true});
	if (is_valid(file)) {
		TL_GET_GLOBAL(default_logger_file) = file;
	}
}


#endif

}
