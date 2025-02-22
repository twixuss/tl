#pragma once
#include "string.h"

namespace tl {

inline List<utf8> bin2cpp(Span<u8> data, Span<utf8> name) {
	StringBuilder builder;
	builder.allocator = current_temporary_allocator;

	append(builder, "unsigned char const ");
	append(builder, name);
	append(builder, "[] = {\n\t");
	for (auto byte : data) {
		append(builder, byte);
		append(builder, ',');
	}
	append(builder, "\n};\n");

	return (List<utf8>)to_string(builder);
}

}