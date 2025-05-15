#include <tl/c_tokenizer.h>
#include <tl/file.h>
#include <WINdows.h>

using namespace tl;

void c_tokenizer_test() {

	//for_each_file(u8"F:\\projects\\simplex\\dep\\tl\\include\\tl"s, {.recursive = true}, [&] (Span<utf8> path) {
	//for_each_file(u8"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.18362.0\\um"s, {.recursive = true}, [&] (Span<utf8> path) {
	//	if (!ends_with(path, u8".h"s) && !ends_with(path, u8".c"s))
	//		return;
	//	println(path);
		//auto source = (Span<char>)read_entire_file(path);
		//auto source = (Span<char>)read_entire_file("F:\\projects\\simplex\\dep\\tl\\include\\tl\\c_tokenizer.h"s);
		auto source = (Span<char>)read_entire_file("C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.18362.0\\um\\MShtml.h"s);
		

		if (starts_with(source, "\xEF\xBB\xBF"s)) {
			source = source.skip(3);
		}
		auto tokenizer = c::Tokenizer::create(source);

		c::Token token, prev;

		token = tokenizer.next();
		while (token.kind != c::tk_end_of_file) {
			println("{} {} {}", Format{token.kind, align_left(12, ' ')}, Format{token.string, align_left(24, ' ')}, tokenizer.string);
			prev = token;
			token = tokenizer.next();
		}

		if (tokenizer.cursor != tokenizer.source_end) {
			println("Unfinished source: {}", Span(tokenizer.cursor, min<umm>(tokenizer.source_end - tokenizer.cursor, 16)));
			println("Offset: {}", tokenizer.cursor - tokenizer.source_start);
			println("Source size: {}", source.count);
			println("Prev token: {}", prev.string);
			exit(1);
		}
	//});
}