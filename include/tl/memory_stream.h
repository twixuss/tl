#pragma once
#include "stream.h"

namespace tl {

struct MemoryStream : Stream {
	Span<u8> memory;
	umm cursor;
};

MemoryStream *create_memory_stream(Span<u8> memory) {
	auto result = create_stream<MemoryStream>();
	result->memory = memory;
	result->_read = [](Stream *stream_, Span<u8> destination) {
		auto stream = (MemoryStream *)stream_;
		umm bytes_to_read = min(destination.size, stream->memory.size - stream->cursor);
		memcpy(destination.data, stream->memory.data + stream->cursor, bytes_to_read);
		stream->cursor += bytes_to_read;
		return bytes_to_read;
	};
	result->_write = [](Stream *stream_, Span<u8> source) {
		auto stream = (MemoryStream *)stream_;
		umm bytes_to_write = min(source.size, stream->memory.size - stream->cursor);
		memcpy(stream->memory.data + stream->cursor, source.data, bytes_to_write);
		stream->cursor += bytes_to_write;
		return bytes_to_write;
	};
	result->_remaining_bytes = [](Stream *stream_) {
		auto stream = (MemoryStream *)stream_;
		return stream->memory.size - stream->cursor;
	};
	return result;
}

}
