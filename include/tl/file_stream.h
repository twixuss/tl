#pragma once
#include "stream.h"
#include "file.h"

namespace TL {

struct FileStream : Stream {
	File file;
};

Stream *create_file_stream(File file) {
	auto result = create_stream<FileStream>();
	result->file = file;
	result->read = [](Stream *stream_, Span<u8> destination) {
		auto stream = (FileStream *)stream_;
		return read(stream->file, destination);
	};
	result->write = [](Stream *stream_, Span<u8> source) {
		auto stream = (FileStream *)stream_;
		return write(stream->file, source);
	};
	return result;
}

}
