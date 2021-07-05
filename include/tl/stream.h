#pragma once

#include "common.h"

namespace TL {

struct Stream {
	Allocator allocator;
	umm (*read)(Stream *stream, Span<u8> destination) = 0;
	umm (*write)(Stream *stream, Span<u8> source) = 0;
};

forceinline umm read(Stream *stream, Span<u8> destination) { return stream->read(stream, destination); }
forceinline umm write(Stream *stream, Span<u8> source) { return stream->write(stream, source); }
forceinline void free(Stream *stream) { return stream->allocator.free(stream); }

template <class T>
T *create_stream() {
	auto allocator = current_allocator;
	auto result = allocator.allocate<T>();
	result->allocator = allocator;
	return result;
}

}
