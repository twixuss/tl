#pragma once
#include "string.h"
#include "console.h"
#include <xmmintrin.h>

extern tl::s32 tl_main(tl::Span<tl::Span<tl::utf8>> args);

#ifdef TL_USE_CONTEXT
#include "context.h"
#endif

#ifdef TL_MAIN_WINMAIN
#pragma warning(push, 0)
#define NOMINMAX
#include <Windows.h>
#pragma warning(pop)
#include "file.h"
int __stdcall wWinMain(HINSTANCE, HINSTANCE, wchar_t *wcmdline, int) {
#elif OS_WINDOWS
int wmain(int argc, wchar_t **argv) {
#else
int main(int argc, char **argv) {
#endif
	using namespace tl;
	
	constexpr int DENORMALS_ARE_ZERO_ON = 0x8040;
	_mm_setcsr(_mm_getcsr() | (_MM_FLUSH_ZERO_ON | DENORMALS_ARE_ZERO_ON));

	#ifdef TL_USE_CONTEXT
		auto context = main_context();
	#endif

	init_allocator();
	defer { deinit_allocator(); };
	
	List<Span<utf8>, DefaultAllocator> arguments;
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
	#elif OS_WINDOWS
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i]), true));
	}
	#else
	for (int i = 0; i < argc; ++i) {
		arguments.add(as_utf8(as_span(argv[i])));
	}
	#endif

	init_printer();

	return tl_main(arguments);
}
