#include "string.h"
#include "console.h"
extern tl::s32 tl_main(tl::Span<tl::Span<tl::utf8>> args);
int wmain(int argc, wchar_t **argv) {
	using namespace tl;
	init_allocator();
	defer { deinit_allocator(); };

	current_printer = console_printer;

	List<Span<utf8>> arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.add(to_utf8(as_span((utf16 *)argv[i])));
	}

	return tl_main(arguments);
}
