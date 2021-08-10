#pragma once
#include "stream.h"

namespace tl {

struct MemoryStream : Stream {
	Span<u8> memory;
	umm cursor;
	bool read(Span<u8> destination) {
		umm bytes_to_read = min(destination.size, memory.size - cursor);
		memcpy(destination.data, memory.data + cursor, bytes_to_read);
		cursor += bytes_to_read;
		return bytes_to_read == destination.size;
	}
	bool write(Span<u8> source) {
		umm bytes_to_write = min(source.size, memory.size - cursor);
		memcpy(memory.data + cursor, source.data, bytes_to_write);
		cursor += bytes_to_write;
		return bytes_to_write == source.size;
	}
	umm remaining_bytes() {
		return memory.size - cursor;
	}
};

MemoryStream *create_memory_stream(Span<u8> memory) {
	auto result = create_stream<MemoryStream>();
	result->memory = memory;
	result->_read = [](Stream *stream, Span<u8> destination) { return ((MemoryStream *)stream)->read(destination); };
	result->_write = [](Stream *stream, Span<u8> destination) { return ((MemoryStream *)stream)->write(destination); };
	result->_remaining_bytes = [](Stream *stream) { return ((MemoryStream *)stream)->remaining_bytes(); };
	return result;
}

}
