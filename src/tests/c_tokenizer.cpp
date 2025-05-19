#define TL_USE_SIMD 1
#include <tl/math_random.h>
#include <tl/c_tokenizer.h>
#include <tl/file.h>
#include <tl/time.h>

using namespace tl;
using namespace tl::c;

static void speed_test();
static void correctness_test();

struct Token {
	TokenKind kind;
	Span<char> location;
	u64 int_value;
	f64 float_value;
	Span<char> string_value;
	Span<char> suffix;
};

struct Test {
	Span<char> source;
	Span<Token> tokens;
};

void append(StringBuilder &builder, Token token) {
	print("{} {}", Format{token.kind, align_left(12, ' ')}, Format{token.location, align_left(24, ' ')});
	switch (token.kind) {
		case tk_int:    print("{}{}", token.int_value, token.suffix); break;
		case tk_float:  print("{}{}", token.float_value, token.suffix); break;
		case tk_string: print("\"{}\"", EscapedCString{as_utf8(token.string_value)}); break;
	}
	println();
}

void c_tokenizer_test() {
	//return speed_test();
	return correctness_test();

	auto path = as_utf8(as_span(__FILE__));
	
	auto source = (Span<char>)read_entire_file(path);

	auto t = Tokenizer::create(source);

	auto prev_location = t.location;

	while (t.next()) {
		if (t.kind == tk_error) {
			println("ERROR: {}", Span(t.cursor, min<umm>(t.source_end - t.cursor, 16)));
			println("DESCRIPTION: {}", t.error);
			break;
		}

		println(Token{t.kind, t.location, t.int_value, t.float_value, t.string_value, t.suffix});

		prev_location = t.location;
	}

	if (t.cursor != t.source_end) {
		println(path);
		println("Unfinished source: {}", Span(t.cursor, min<umm>(t.source_end - t.cursor, 16)));
		println("Offset: {}", t.cursor - t.source_start);
		println("Source size: {}", source.count);
		println("Prev token: {}", prev_location);
	}
}

void correctness_test() {
	// TODO: To add:
	//     #include with backslashes

	Span<Test> tests = {
		/* Hello World! */ {
			"#include <stdio.h>\r\nint main() {\r\n\tprintf(\"Hello, World!\\n\");\r\n\treturn 0;\r\n}\r\n"s,
			to_list(Span<Token>{
				{.kind = tk_directive,  .location = "#include"s                                                },
				{.kind = tk_string,     .location = "<stdio.h>"s, .string_value = "stdio.h"s                   },
				{.kind = tk_identifier, .location = "int"s                                                     },
				{.kind = tk_identifier, .location = "main"s                                                    },
				{.kind = '(',           .location = "("s                                                       },
				{.kind = ')',           .location = ")"s                                                       },
				{.kind = '{',           .location = "{"s                                                       },
				{.kind = tk_identifier, .location = "printf"s                                                  },
				{.kind = '(',           .location = "("s                                                       },
				{.kind = tk_string,     .location = "\"Hello, World!\\n\""s, .string_value = "Hello, World!\n"s},
				{.kind = ')',           .location = ")"s                                                       },
				{.kind = ';',           .location = ";"s                                                       },
				{.kind = tk_identifier, .location = "return"s                                                  },
				{.kind = tk_int,        .location = "0"s, .int_value = 0                                       },
				{.kind = ';',           .location = ";"s                                                       },
				{.kind = '}',           .location = "}"s                                                       },
			}),
		}
	};

	List<Token> parsed_tokens;

	for (auto &test : tests) {
		auto t = Tokenizer::create(test.source);

		parsed_tokens.clear();
		while (t.next()) {
			if (t.kind == tk_error) {
				println("ERROR: {}", Span(t.cursor, min<umm>(t.source_end - t.cursor, 16)));
				println("DESCRIPTION: {}", t.error);
				break;
			}

			parsed_tokens.add({
				.kind = t.kind,
				.location = t.location,
				.int_value = t.int_value,
				.float_value = t.float_value,
				.string_value = to_list(t.string_value),
			});
		}
		
		int i = 0;
		while (1) {
			if (i == test.tokens.count && i == parsed_tokens.count) {
				break;
			} else if (i == test.tokens.count || i == parsed_tokens.count) {
				if (i < test.tokens.count) {
					println("Not enough tokens parsed! Expected {} but got {}", test.tokens.count, parsed_tokens.count);
					println("Missing tokens:");
					for (int j = i; j < test.tokens.count; ++j) {
						println(test.tokens[j]);
					}
				} else {
					println("Too many tokens parsed! Expected {} but got {}", test.tokens.count, parsed_tokens.count);
					println("Extra tokens:");
					for (int j = i; j < parsed_tokens.count; ++j) {
						println(parsed_tokens[j]);
					}
				}
				break;
			}
			
			#define CHECK_MATCHING(x)                                                                              \
				if (test.tokens[i].x != parsed_tokens[i].x) {                                                      \
					println("#{}: wrong " #x ". expected {} but got {}", i, test.tokens[i].x, parsed_tokens[i].x); \
					goto next_test;                                                                                \
				}

			if (test.tokens[i].kind != parsed_tokens[i].kind) {
				println("#{}: wrong kind. expected {} but got {}", i, TokenKindDesc{test.tokens[i].kind}, TokenKindDesc{parsed_tokens[i].kind});
				goto next_test;
			}
			CHECK_MATCHING(location);

			switch (test.tokens[i].kind) {
				case tk_int:    { CHECK_MATCHING(int_value);   CHECK_MATCHING(suffix); break; }
				case tk_float:  { CHECK_MATCHING(float_value); CHECK_MATCHING(suffix); break; }
				case tk_string: { CHECK_MATCHING(string_value); break; }
			}

			++i;
		}
	next_test:;
	}
}

void speed_test() {
	struct SourceFile {
		Span<utf8> path;
		Span<char> source;
	};

	List<SourceFile> source_files;

	u64 total_bytes = 0;

	auto add_file = [&] (Span<utf8> path) { 
		if (!ends_with(path, u8".h"s) && !ends_with(path, u8".c"s) && !ends_with(path, u8".cpp"s))
			return;
		println(path);
		auto source = (Span<char>)read_entire_file(path);
		if (starts_with(source, "\xEF\xBB\xBF"s)) {
			source = source.skip(3);
		}
		total_bytes += source.count;
		source_files.add({to_list(path), source});
	};

	println("Reading files...");
	add_file(as_utf8(as_span(__FILE__)));
	for_each_file(u8"F:\\projects\\simplex\\dep\\tl\\include\\tl"s, {.recursive = true}, add_file);
	for_each_file(u8"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.18362.0"s, {.recursive = true}, add_file);

	for (int rep = 0; rep < 10; ++rep) {
		println("Tokenizing...");
		auto timer = create_precise_timer();
		int i = 0;
		for (auto file : source_files) {
			print("\r{} / {}", i++, source_files.count);
			//println(file.path);
			//auto source = (Span<char>)read_entire_file(path);
			//auto source = (Span<char>)read_entire_file("F:\\projects\\simplex\\dep\\tl\\include\\tl\\c_t.h"s);
			//auto source = (Span<char>)read_entire_file("F:\\projects\\simplex\\dep\\tl\\src\\tests\\c_t.cpp"s);
			//auto source = (Span<char>)read_entire_file("C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.18362.0\\um\\MShtml.h"s);
			auto source = file.source;

			auto t = Tokenizer::create(source);

			while (t.next()) {}
		}
		auto time = elapsed_time(timer);
		println("Total: {}\nTime: {}s\nSpeed: {}/s", format_bytes(total_bytes), time, format_bytes(total_bytes/time));
	}
}

int x[] = {
	12345,
	0b101010,
	0x12345,
	12345ull,
	0b101010ull,
	0x12345ull,
};
float f[] = {
	66e2f,
	66e+2f,
	66e-2f,
	.12f,
	.12e2f,
	.12e+2f,
	.12e-2f,
	0.12f,
	0.12e2f,
	0.12e+2f,
	0.12e-2f,
	0129.12f,
	0129.12e2f,
	0129.12e+2f,
	0129.12e-2f,
};
