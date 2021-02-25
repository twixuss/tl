#pragma once
#include "common.h"

namespace TL {

struct Buffer : Span<u8> {
	using Span<u8>::Span;
	Allocator allocator;
};

inline Buffer create_buffer(umm size) {
	Buffer result;
	result.allocator = current_allocator;
	result.data = ALLOCATE(u8, result.allocator, size);
	result.size = size;
	return result;
}

inline void free(Buffer buffer) {
	if (buffer.data) {
		FREE(buffer.allocator, buffer.data);
	}
}

}