#pragma once
#include "common.h"

namespace tl {

struct Buffer : Span<u8> {
	using Span<u8>::Span;
	Allocator allocator = {};
};

inline Buffer create_buffer_uninitialized(umm size) {
	Buffer result;
	result.allocator = current_allocator;
	result.data = result.allocator.allocate_uninitialized<u8>(size);
	result.count = size;
	return result;
}
inline Buffer create_buffer(umm size) {
	auto result = create_buffer_uninitialized(size);
	memset(result.data, 0, result.count);
	return result;
}

inline void free(Buffer buffer) {
	if (buffer.data && buffer.allocator.func) {
		buffer.allocator.free(buffer.data);
	}
}

}
