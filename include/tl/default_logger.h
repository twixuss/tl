#pragma once
#include "logger.h"
#include "file.h"

namespace tl {

extern TL_API OsLock stdout_mutex;

struct TL_API DefaultLogger : LoggerBase {
	static File shared_file;

	Span<utf8> module = {};
	File file = shared_file;
	LogSeverity min_severity = TL_DEBUG ? LogSeverity::info : LogSeverity::warning;

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

		withs(stdout_mutex) {
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

File DefaultLogger::shared_file;

void DefaultLogger::global_init(Span<utf8> shared_file_path) {
	auto file = open_file(shared_file_path, {.write = true});
	if (is_valid(file)) {
		shared_file = file;
	}
}

OsLock stdout_mutex = {};

#endif

}
