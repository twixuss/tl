#pragma once
#include "common.h"

namespace tl {

struct Buffer : Span<u8> {
	using Span<u8>::Span;
	Allocator allocator = {};
};

inline Buffer create_buffer(AllocateFlags flags, umm size) {
	Buffer result;
	result.allocator = current_allocator;
	result.data = result.allocator.allocate<u8>(flags, size);
	result.size = size;
	return result;
}
inline Buffer create_buffer(umm size) {
	return create_buffer(Allocate_default, size);
}

inline void free(Buffer buffer) {
	if (buffer.data && buffer.allocator.func) {
		buffer.allocator.free(buffer.data);
	}
}

}
