#pragma once
#include "string.h"
#include "console.h"
#include "default_logger.h"

#ifdef TL_USE_CONTEXT

#include "context.h"

extern tl::s32 tl_main(tl::Span<tl::Span<tl::utf8>> args);
int wmain(int argc, wchar_t **argv) {
	using namespace tl;

	auto context = main_context();

	init_allocator();
	defer { deinit_allocator(); };

	List<Span<utf8>> arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i]), true).skip(-1));
	}
	
	init_printer();

	DefaultLogger::global_init(tformat(u8"{}.log", arguments[0]));

	DefaultLogger app_logger = {.module = u8"app"s};
	DefaultLogger tl_logger = {.module = u8"tl"s};
	
	context->app_logger() = app_logger;
	context->tl_logger() = tl_logger;

	return tl_main(arguments);
}

#else

namespace tl {
void init_logger_thread() {
	current_logger = tl::app_logger;
}
}

extern tl::s32 tl_main(tl::Span<tl::Span<tl::utf8>> args);
int wmain(int argc, wchar_t **argv) {
	using namespace tl;

	constexpr int DENORMALS_ARE_ZERO_ON = 0x8040;
	_mm_setcsr(_mm_getcsr() | (_MM_FLUSH_ZERO_ON | DENORMALS_ARE_ZERO_ON));

	init_allocator();
	defer { deinit_allocator(); };

	List<Span<utf8>> arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i]), true).skip(-1));
	}
	
	init_printer();

	tl::init_logger_thread();

	return tl_main(arguments);
}

#endif