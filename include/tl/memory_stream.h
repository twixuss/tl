#pragma once
#include "stream.h"

namespace tl {

struct MemoryStream : Stream {
	Span<u8> memory;
	umm cursor;
	umm read(Span<u8> destination) {
		umm bytes_to_read = min(destination.size, memory.size - cursor);
		memcpy(destination.data, memory.data + cursor, bytes_to_read);
		cursor += bytes_to_read;
		return bytes_to_read;
	}
	umm write(Span<u8> source) {
		umm bytes_to_write = min(source.size, memory.size - cursor);
		memcpy(memory.data + cursor, source.data, bytes_to_write);
		cursor += bytes_to_write;
		return bytes_to_write;
	}
	umm remaining_bytes() {
		return memory.size - cursor;
	}
};

inline MemoryStream *create_memory_stream(Span<u8> memory) {
	auto result = create_stream<MemoryStream>();
	result->memory = memory;
	return result;
}

}
