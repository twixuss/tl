#pragma once
#include "file.h"
#include "list.h"
#include "block_list.h"

namespace tl {
namespace includer {

struct Dependency {
	List<utf8> path;

	void init() {

	}
	void free() {
		tl::free(path);
	}
};

template <class PerDependencyData = Dependency>
requires std::is_base_of_v<Dependency, PerDependencyData>
struct PreprocessResult {
	StringBuilder builder;
	List<utf8> text;

	List<PerDependencyData> dependencies;

	void (*append_debug_info)(StringBuilder &output, Span<utf8> path, u32 line) = autocast []{};

	void load(Span<utf8> path, Span<utf8> include_directive = u8"#include"s) {
		builder.clear();
		text.clear();
		for (auto &dependency : dependencies) {
			dependency.free();
		}
		dependencies.clear();

		{
			auto &dep = dependencies.add();
			dep.path = to_list(path);
			dep.init();
		}

		struct FileState {
			Span<utf8> path;

			Span<utf8> remaining;
			u32 line = 1;
			Buffer buffer;

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
			if (auto found = find(current.remaining, include_directive)) {
				auto text_before_include = Span(current.remaining.begin(), found);
				append(builder, text_before_include);

				current.line += count(text_before_include, [](utf8 c) { return c == u8'\n'; });

				auto line_end = find(Span(found + include_directive.count, current.remaining.end()), u8'\n');
				if (!line_end) {
					line_end = current.remaining.end();
				}
				current.remaining.set_begin(line_end);

				auto relative_include_path = strip(Span(found + include_directive.count, line_end), [&](utf8 c) { return is_whitespace((ascii)c); });

				auto full_include_path = format(u8"{}/{}", parse_path(current.path).directory, relative_include_path);

				{
					auto &dep = dependencies.add();
					dep.path = full_include_path;
					dep.init();
				}

				state_stack.add(current);
				current = {.path = full_include_path};
				current.init();

				append_debug_info(builder, full_include_path, 0);
			} else {
				append(builder, current.remaining);
				if (state_stack.count) {
					current.deinit();
					current = state_stack.pop().value();

					append_debug_info(builder, current.path, current.line);
				} else {
					break;
				}
			}
		}

		text.reserve(builder.count());
		builder.fill(as_bytes(text));
		text.count = builder.count();
	}
	void free() {
		tl::free(text);
		tl::free(builder);
		for (auto &dependency : dependencies) {
			dependency.free();
		}
		tl::free(dependencies);
	}
};



}
}
