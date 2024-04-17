#pragma once

#include "common.h"

namespace tl {

struct Stream {
	Allocator allocator;
	umm (*_read)(Stream *stream, Span<u8> destination) = 0;
	umm (*_write)(Stream *stream, Span<u8> source) = 0;
	umm (*_remaining_bytes)(Stream *stream) = 0;

	forceinline umm read(Span<u8> destination) { return _read(this, destination); }
	forceinline umm write(Span<u8> source) { return _write(this, source); }
	forceinline umm remaining_bytes() { return _remaining_bytes(this); }

	forceinline void free() { return allocator.free(this); }
};


template <class T>
T *create_stream() {
	auto allocator = TL_GET_CURRENT(allocator);
	auto result = allocator.allocate<T>();
	result->allocator = allocator;
	result->_read = [](Stream *stream, Span<u8> destination) {
		if constexpr (is_statically_overridden(read, T, Stream)) {
			return ((T *)stream)->read(destination);
		} else {
			invalid_code_path("unavailable");
			return (umm)0;
		}
	};
	result->_write = [](Stream *stream, Span<u8> destination) {
		if constexpr (is_statically_overridden(write, T, Stream)) {
			return ((T *)stream)->write(destination);
		} else {
			invalid_code_path("unavailable");
			return (umm)0;
		}
	};
	result->_remaining_bytes = [](Stream *stream) {
		if constexpr (is_statically_overridden(write, T, Stream)) {
			return ((T *)stream)->remaining_bytes();
		} else {
			invalid_code_path("unavailable");
			return (umm)0;
		}
	};
	return result;
}


inline void free(Stream *stream) {
	stream->free();
	*stream = {};
}
}
