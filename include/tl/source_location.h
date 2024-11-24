#pragma once
#include "common.h"
#include "list.h"

namespace tl {

struct SourceLocation {
	u32 line_number = 0;
	u32 column_number = 0;
	List<Span<utf8>> lines;
};

SourceLocation get_source_location(Span<utf8> location, Span<utf8> source) {
	SourceLocation result;

	auto cursor = location.data;
	result.column_number = 0;
	while (cursor >= source.data && *cursor != '\n') {
		++result.column_number;
		--cursor;
	}

	if (location == u8"\n"s) {
		result.lines.add({location});
	} else {
		Span<utf8> combined_lines;
		combined_lines.data = cursor + 1;
		combined_lines.set_end(location.end());

		while (combined_lines.end() < source.end() && *combined_lines.end() != '\n')
			++combined_lines.count;

		split(combined_lines, u8'\n', [&](Span<utf8> line){ result.lines.add({ line }); });
	}

	assert(result.lines.front().begin() <= location.begin());
	assert(result.lines.back().end() >= location.end());

	result.line_number = 1;
	while (cursor >= source.data) {
		result.line_number += *cursor-- == '\n';
	}

	return result;
}

}