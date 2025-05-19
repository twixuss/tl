#include <tl/c_preprocessor.h>
#include <tl/contiguous_hash_map.h>

using namespace tl;
using namespace tl::c_preprocessor;

using String = Span<char>;

struct SourceFile {
	String path;
	Buffer source_buffer;
};

List<SourceFile, DefaultAllocator> source_files;


struct SourceLine {
	String string;
	u32 number = 0;
};

struct SourceLocation {
	String location;
	String file;
	u32 location_line_number = 0;
	u32 location_column_number = 0;

	u32 lines_start_number = 0;
	List<String> lines;
};
	
enum class ReportKind : u8 {
	internal_error,
	error,
	warning,
	info,
	help,
};

void append(StringBuilder &builder, SourceLocation location) {
	append_format(builder, "{}:{}:{}", location.file, location.location_line_number, location.location_column_number);
}

struct GetSourceLocationOptions {
	int lines_before = 2;
	int lines_after = 0;
};

	
SourceLocation get_source_location(String location, GetSourceLocationOptions options = {}) {
	SourceLocation result;
	result.location = location;
	result.location_column_number = 0;

	auto chunk_start = location.begin();
	auto chunk_end = location.end();

	if (location == "\n"s) {
		// Show preceding line in this case
		chunk_start--;
		chunk_end--;
	}

	while (*chunk_start && *chunk_start != '\n') {
		--chunk_start;
		++result.location_column_number;
	}
	++chunk_start;
	// Now chunk_start it is at the beggining of first line of `location`

	while (*chunk_end && *chunk_end != '\n') {
		++chunk_end;
	}
	// Now chunk_end it is at the end of last line of `location`

	// chunk_* now points at line(s) with `location`. Extend to desired line count.
	
	int missing_lines_before = 0;
	for (int i = 0; i < options.lines_before; ++i) {
		--chunk_start;
		if (*chunk_start == 0) {
			missing_lines_before = options.lines_before - i;
			break;
		}
		assert(*chunk_start == '\n');
		--chunk_start;
		
		while (*chunk_start && *chunk_start != '\n') {
			--chunk_start;
		}
		++chunk_start;
	}
	
	int missing_lines_after = 0;
	for (int i = 0; i < options.lines_after; ++i) {
		if (*chunk_end == 0) {
			missing_lines_after = options.lines_after - i;
			break;
		}

		++chunk_end;
		while (*chunk_end && *chunk_end != '\n') {
			++chunk_end;
		}
	}

	for (int i = 0; i < missing_lines_before; ++i) {
		result.lines.add(Span(chunk_start, (umm)0));
	}
	split_by_one(Span(chunk_start, chunk_end), '\n', [&](String line) {
		result.lines.add(line);
	});
	for (int i = 0; i < missing_lines_after; ++i) {
		result.lines.add(Span(chunk_end, (umm)0));
	}

	assert(result.lines.count);
	assert(result.lines.front().begin() <= location.begin());
	if (location != "\n"s) {
		assert(result.lines.back().end() >= location.end());
	}

	char *file_start = 0;
	for (auto &source_file : source_files) {
		if (owns(source_file.source_buffer, (u8 *)chunk_start)) {
			file_start = (char *)source_file.source_buffer.data;
			result.file = source_file.path;
			break;
		}
	}

	if (file_start) {
		result.lines_start_number = 1;
		auto cursor = chunk_start;
		while (cursor >= file_start) {
			if (*cursor == '\n') 
				++result.lines_start_number;
			--cursor;
		}

		result.location_line_number = result.lines_start_number + options.lines_before;
	}

	return result;
}

void print_replacing_tabs_with_spaces(String string) {
	const umm n_spaces = 4;
	for (auto c : string) {
		if (c == '\t')
			for (umm i = 0; i < n_spaces; ++i)
				print(' ');
		else 
			print(c);
	}
}

void print_with_length_of(char r, umm count) {
	for (umm i = 0; i < count; ++i) {
		print(r);
	}
}

void print_with_length_of(char r, String string) {
	for (auto c : string) {
		if (c == '\t')
			for (umm i = 0; i < 4;++i) 
				tl::print(r);
		else 
			print(r);
	}
}

void print_report_indentation(int indentation) {
	for (u32 i = indentation; i--;) {
		tl::print("|   "s);
	}
}

void print_source_chunk(SourceLocation source_location, int indentation, ConsoleColor highlight_color) {
	auto number_width = [](umm x) {
		if (x < 10) return 1;
		if (x < 100) return 2;
		if (x < 1000) return 3;
		if (x < 10000) return 4;
		if (x < 100000) return 5;
		if (x < 1000000) return 6;
		return 7;
	};

	auto max_line_number = source_location.lines_start_number + source_location.lines.count - 1;
	auto line_number_width = number_width(max_line_number);
	auto line_number_alignment = align_right(line_number_width, ' ');

	auto output_line = [&](u32 line_number, String line, String highlight) {
		print_report_indentation(indentation);
		umm chars_pre_source = print(" {} | ", Format(line_number, line_number_alignment));

		highlight.set_begin(clamp(highlight.begin(), line.begin(), line.end()));
		highlight.set_end(clamp(highlight.end(), line.begin(), line.end()));
		if (highlight.count) {
			auto prefix = String(line.begin(), highlight.begin());
			auto postfix = String(highlight.end(), line.end());

			print_replacing_tabs_with_spaces(prefix);
			with(highlight_color, print_replacing_tabs_with_spaces(highlight));
			print_replacing_tabs_with_spaces(postfix);
			println();
			if (!is_stdout_console()) {
				withs(highlight_color) {
					print_with_length_of(' ', chars_pre_source);
					print_with_length_of(' ', prefix);
					print_with_length_of('~', highlight);
					print_with_length_of(' ', postfix);
					println();
				};
			}
		} else {
			print_replacing_tabs_with_spaces(line);
			println();
			if (source_location.location == "\n"s) {
				withs(highlight_color) {
					print_with_length_of(' ', chars_pre_source);
					print_with_length_of(' ', line);
					println('~');
				};
			}
		}

	};

	for (int i = 0; i < source_location.lines.count; ++i) {
		output_line(source_location.lines_start_number + i, source_location.lines[i], source_location.location);
	}
}
ConsoleColor get_color(ReportKind kind) {
	switch (kind) {
		case ReportKind::internal_error: return ConsoleColor::red;
		case ReportKind::error:          return ConsoleColor::red;
		case ReportKind::warning:        return ConsoleColor::yellow;
		case ReportKind::info:           return ConsoleColor::cyan;
		case ReportKind::help:           return ConsoleColor::green;
	}
	return {};
}

umm print_report_kind(ReportKind kind) {
	switch (kind) {
		case ReportKind::internal_error: return with(get_color(kind), ::print("INTERNAL ERROR"));
		case ReportKind::error:          return with(get_color(kind), ::print("Error"));
		case ReportKind::warning:        return with(get_color(kind), ::print("Warning"));
		case ReportKind::info:           return with(get_color(kind), ::print("Info"));
		case ReportKind::help:           return with(get_color(kind), ::print("Help"));
	}
	return 0;
}

void report(ReportKind kind, String location, String message) {
	scoped(temporary_allocator_and_checkpoint);

	if (location.data) {
		auto source_location = get_source_location(location);

		tl::print("{}: ", source_location);

		println();
		print_report_kind(kind);
		println(": {}",  message);

		print_source_chunk(source_location, 0, get_color(kind));
	} else {
		print_report_kind(kind);
		println(": {}", message);
	}
}


struct Preprocessor {
	List<Span<char>> include_directories;

	umm tabs = 0;
	ContiguousHashMap<Span<char>, Span<FatToken>> macros;

	void error(Error error, Span<char> message, ErrorInfo info = {}) {
		report(ReportKind::error, info.tokenizer->location, tformat("{} ({})", message, error));

		if (info.path) {
			println("Path: {}", info.path);
		}

		if (error == Error::tokenizer_error) {
			auto &t = *info.tokenizer;
			println("Tokenizer error: {}", t.error);
			println("CURSOR: {}", Span(t.cursor, min<umm>(t.source_end - t.cursor, 16)));
			println("DESCRIPTION: {}", t.error);
		}
	}
	void add_source_buffer(Span<char> path, Buffer buffer) {
		source_files.add({path, buffer});
	}
	void before_include(Span<char> path) {
		println("{}{}", Repeat{' ', tabs}, path);
		++tabs;
	}
	void after_include(Span<char> path) {
		--tabs;
	}
	void parsed_macro(Span<char> name, List<FatToken> value_tokens) {
		macros.get_or_insert(name) = value_tokens;

		if (value_tokens.count) {
			println("#define {} {}", name, Span(value_tokens.front().location.begin(), value_tokens.back().location.end()));
		} else {
			println("#define {}", name);
		}
	}
	Optional<Span<FatToken>> get_macro(Span<char> name) {
		return to_optional(macros.find(name).value);
	}









	
};

void c_preprocessor_test() {
	#if 1
	Preprocessor p = {};
	p.include_directories.add("F:\\projects\\simplex\\dep\\tl\\include"s);
	p.include_directories.add("C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.18362.0\\um"s);
	p.include_directories.add("C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.18362.0\\shared"s);
	p.include_directories.add("D:\\Programs\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.40.33807\\include"s);
	auto err = preprocess_c_file(as_span(__FILE__), p);
	println(err);
	#else
	Preprocessor p = {};
	auto err = preprocess_c_source("doesntmatter"s, R"(

)"s, p);
	println(err);
	#endif
}
