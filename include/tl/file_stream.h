#pragma once
#include "stream.h"
#include "file.h"

namespace tl {

struct FileStream : Stream {
	File file;
	umm read(Span<u8> destination) { return tl::read(file, destination); }
	umm write(Span<u8> source) { return tl::write(file, source); }
	umm remaining_bytes() { return tl::remaining_bytes(file); }
};

inline Stream *create_file_stream(File file) {
	auto result = create_stream<FileStream>();
	result->file = file;
	return result;
}

}
