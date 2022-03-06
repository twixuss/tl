#pragma once
#include "string.h"
#include "console.h"
#include "context.h"

extern tl::s32 tl_main(tl::Context &context, tl::Span<tl::Span<tl::utf8>> args);
int wmain(int argc, wchar_t **argv) {
	using namespace tl;
	init_allocator();
	defer { deinit_allocator(); };

	current_printer = standard_output_printer;

	List<Span<utf8>> arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i])));
	}

	Context context;
	context.current_allocator = default_allocator;
	return tl_main(context, arguments);
}
