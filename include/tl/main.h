#pragma once
#include "string.h"
#include "console.h"
#include "default_logger.h"

namespace tl {
void init_logger_thread() {
	current_logger = tl::app_logger;
}
}

extern tl::s32 tl_main(tl::Span<tl::Span<tl::utf8>> args);
int wmain(int argc, wchar_t **argv) {
	using namespace tl;
	init_allocator();
	defer { deinit_allocator(); };

	List<Span<utf8>> arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i]), true).skip(-1));
	}
	
	init_printer();

	DefaultLogger::global_init(tformat(u8"{}.log", arguments[0]));

	DefaultLogger app_logger = {.module = u8"app"s};
	tl::app_logger = app_logger;

	DefaultLogger tl_logger = {.module = u8"tl"s};
	tl::tl_logger = tl_logger;

	tl::init_logger_thread();

	return tl_main(arguments);
}
