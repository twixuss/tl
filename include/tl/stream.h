#pragma once

#include "common.h"

namespace TL {

struct Stream {
	umm (*read)(Stream *stream, Span<u8> destination) = 0;
	umm (*write)(Stream *stream, Span<u8> source) = 0;
	void (*free)(Stream *stream) = 0;
};

forceinline umm read(Stream *stream, Span<u8> destination) { return stream->read(stream, destination); }
forceinline umm write(Stream *stream, Span<u8> source) { return stream->write(stream, source); }
forceinline umm free(Stream *stream) { return stream->free(stream); }

struct MemoryStream : Stream {
	Span<u8> memory;
	u64 cursor;
};

Stream *create_memory_stream(Span<u8> memory) {
	auto result = ALLOCATE(MemoryStream, current_allocator);
	result.read = [](Stream *stream_, Span<u8> destination) {
		auto &stream = *(MemoryStream *)stream_;
		umm bytes_to_read = min(destination.size, stream.memory.size - stream.cursor);
		memcpy(destination.data, stream.memory.data + stream.cursor, bytes_to_read);
		stream.cursor += bytes_to_read;
		return bytes_to_read;
	};
	result.write = [](Stream *stream_, Span<u8> source) {
		auto &stream = *(MemoryStream *)stream_;
		umm bytes_to_write = min(source.size, stream.memory.size - stream.cursor);
		memcpy(stream.memory.data + stream.cursor, source.data, bytes_to_write);
		stream.cursor += bytes_to_write;
		return bytes_to_write;
	};
	return result;
}

}
