#pragma once
#include "string.h"
#include "console.h"
#include "default_logger.h"

extern tl::s32 tl_main(tl::Span<tl::Span<tl::utf8>> args);

#ifdef TL_USE_CONTEXT
#include "context.h"
#else
namespace tl {
void init_logger_thread() {
	current_logger = tl::app_logger;
}
}
#endif

#ifdef TL_MAIN_WINMAIN
int __stdcall wWinMain(HINSTANCE, HINSTANCE, wchar_t *wcmdline, int) {
#else
int wmain(int argc, wchar_t **argv) {
#endif
	using namespace tl;
	
	constexpr int DENORMALS_ARE_ZERO_ON = 0x8040;
	_mm_setcsr(_mm_getcsr() | (_MM_FLUSH_ZERO_ON | DENORMALS_ARE_ZERO_ON));

	#ifdef TL_USE_CONTEXT
		auto context = main_context();
	#endif

	init_allocator();
	defer { deinit_allocator(); };
	
	List<Span<utf8>> arguments;
	#ifdef TL_MAIN_WINMAIN
	{
		arguments.add(get_executable_path());
		auto cmdline = to_utf8(as_span((utf16 *)wcmdline));
		// TODO: parse quotes
		split_by_one(cmdline, u8' ', [&](Span<utf8> str) {
			if (str.count) {
				arguments.add(str);
			}
		});
	}
	#else
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i]), true));
	}
	#endif

	init_printer();

	DefaultLogger::global_init(tformat(u8"{}.log", arguments[0]));

	DefaultLogger app_logger = {.module = u8"app"s};
	DefaultLogger tl_logger = {.module = u8"tl"s};
	
	#ifdef TL_USE_CONTEXT
		context->app_logger() = app_logger;
		context->tl_logger() = tl_logger;
	#else
		tl::app_logger = app_logger;
		tl::tl_logger = tl_logger;
		tl::init_logger_thread();
	#endif

	return tl_main(arguments);
}
