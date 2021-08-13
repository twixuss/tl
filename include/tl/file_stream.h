#pragma once
#include "stream.h"
#include "file.h"

namespace tl {

struct FileStream : Stream {
	File file;
	bool read(Span<u8> destination) { return tl::read(file, destination); }
	bool write(Span<u8> source) { return tl::write(file, source); }
	umm remaining_bytes() { return tl::remaining_bytes(file); }
};

Stream *create_file_stream(File file) {
	auto result = create_stream<FileStream>();
	result->file = file;
	result->_read = [](Stream *stream, Span<u8> destination) { return ((FileStream *)stream)->read(destination); };
	result->_write = [](Stream *stream, Span<u8> destination) { return ((FileStream *)stream)->write(destination); };
	result->_remaining_bytes = [](Stream *stream) { return ((FileStream *)stream)->remaining_bytes(); };
	return result;
}

}
