#pragma once

#include "common.h"

namespace tl {

struct Stream {
	Allocator allocator;
	bool (*_read)(Stream *stream, Span<u8> destination) = 0;
	bool (*_write)(Stream *stream, Span<u8> source) = 0;
	umm (*_remaining_bytes)(Stream *stream) = 0;
	
	forceinline bool read(Span<u8> destination) { return _read(this, destination); }
	forceinline bool write(Span<u8> source) { return _write(this, source); }
	forceinline umm remaining_bytes() { return _remaining_bytes(this); }

	forceinline void free() { return allocator.free(this); }
};


template <class T>
T *create_stream() {
	auto allocator = current_allocator;
	auto result = allocator.allocate<T>();
	result->allocator = allocator;
	return result;
}


void free(Stream *stream) {
	stream->free();
	*stream = {};
}
}
