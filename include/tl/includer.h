#pragma once
#include "file.h"
#include "list.h"
#include "block_list.h"
#include "string.h"

//
// Solution for including files, for example preprocessing opengl shaders.
// 
// Usage:
// 
// 1) If you don't need to store per-included-file data, use `includer::SourceFileBase` for next steps.
//    Otherwise create a class which derives from `tl::includer::SourceFileBase` and stores per-included-file data,
//    for example modification time, to recompile if dependency changes.
//    You can "override" (no vtable) dependency functions by just declaring functions in your type that use the same name as
//    in base class. (Don't forget to call base functions if you do so).
//    For example:
/*
	struct MySourceFile : includer::SourceFileBase {
		FileTime last_write_time = 0;
			
		void init() {
			last_write_time = get_file_write_time(path).value_or(0);;
		}
		bool was_modified() {
			return last_write_time < get_file_write_time(path).value_or(0);
		}
	};
*/
// 2) Create `includer::Includer` for the main file: 
/*
	includer::Includer<MySourceFile> includer = {
		.append_location_info = [](StringBuilder &builder, String path, u32 line) {
			append_format(builder, "\n#line {} \"{}\"\n", line, path); // Insert location info in result for correct locations in output messages.
		}
	};
*/
// 3) Call `includer.load("path_to_your_source_files", &output_result_text)`.
//    Includer can be reused, just call `includer.load` again.

namespace tl {
namespace includer {


struct SourceFileBase {
	using PathAllocator = Allocator;

	List<utf8, PathAllocator> path;

	void init() {

	}
	void free() {
		tl::free(path);
	}
};

using AppendLocationInfo = void (*)(StringBuilder &output, Span<utf8> path, u32 line);

template <class AppendLocationInfo = AppendLocationInfo>
struct LoadOptions {
	Span<utf8> include_directive = u8"#include"s;
	AppendLocationInfo append_location_info = autocast []{};
};

template <class SourceFile = SourceFileBase>
requires std::is_same_v<SourceFile, SourceFileBase>
	  || std::is_base_of_v<SourceFileBase, SourceFile> // is_base_of<SourceFile, SourceFile> returns true, which doesn't make sense, that's why is_same check.
struct Includer {
	// contains the main file, its dependencies, and their dependencies, all of them.
	List<SourceFile> source_files;

	template <class Allocator, class AppendLocationInfo>
	void load(Span<utf8> path, List<utf8, Allocator> *text, LoadOptions<AppendLocationInfo> options = {}) {

		using PathAllocator = typename SourceFile::PathAllocator;

		assert(text);
		
		// TODO: Implement reuse. Should ask caller for available builder
		// because storing it makes Includer non-copyable.
		StringBuilder builder;
		defer { tl::free(builder); };

		for (auto &source_file : source_files) {
			source_file.free();
		}
		source_files.clear();

		auto add_source_file = [&](List<utf8, PathAllocator> path) {
			auto &source_file = source_files.add();
			source_file.path = path;
			source_file.init();
		};

		add_source_file(to_list<PathAllocator>(path));

		struct FileState {
			// reference to `SourceFile::path`
			Span<utf8> path;

			u32 line = 1;
			Buffer buffer;
			Span<utf8> remaining;

			void init() {
				buffer = read_entire_file(path);
				remaining = as_utf8(buffer);
			}
			void deinit() {
				tl::free(buffer);
			}
		};

		List<FileState> state_stack;
		defer { tl::free(state_stack); };

		FileState current = {.path = path};
		current.init();

		while (current.remaining.count) {
			if (auto found = find(current.remaining, options.include_directive)) {
				auto text_before_include = Span(current.remaining.begin(), found);
				append(builder, text_before_include);

				current.line += count(text_before_include, [](utf8 c) { return c == u8'\n'; });

				auto line_end = find(Span(found + options.include_directive.count, current.remaining.end()), u8'\n');
				if (!line_end) {
					line_end = current.remaining.end();
				}
				current.remaining.set_begin(line_end);

				auto relative_include_path = trim(Span(found + options.include_directive.count, line_end), [&](utf8 c) { return is_whitespace((ascii)c); });

				auto full_include_path = format<PathAllocator>(u8"{}/{}"s, parse_path(current.path).directory, relative_include_path);

				add_source_file(full_include_path);

				state_stack.add(current);
				current = {.path = full_include_path};
				current.init();

				options.append_location_info(builder, full_include_path, 0);
			} else {
				append(builder, current.remaining);
				if (state_stack.count) {
					current.deinit();
					current = state_stack.pop().value();

					options.append_location_info(builder, current.path, current.line);
				} else {
					break;
				}
			}
		}

		umm count = builder.count();
		text->reserve(count);
		text->count = count;
		builder.fill(as_bytes(*text));
	}
	void free() {
		for (auto &source_file : source_files) {
			source_file.free();
		}
		tl::free(source_files);
	}
};



}
}
