#pragma once

#include "common.h"

namespace tl {

struct Stream {
	Allocator allocator;
	umm (*_read)(Stream *stream, Span<u8> destination) = 0;
	umm (*_write)(Stream *stream, Span<u8> source) = 0;
	umm (*_remaining_bytes)(Stream *stream) = 0;
	
	forceinline umm read(Span<u8> destination) { return _read(this, destination); }
	forceinline bool b_read(Span<u8> destination) { return _read(this, destination) == destination.size; }

	forceinline umm write(Span<u8> source) { return _write(this, source); }
	forceinline bool b_write(Span<u8> source) { return _write(this, source) == source.size; }

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

}
