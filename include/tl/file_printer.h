#pragma once
#include "console.h"
#include "file.h"

namespace tl {

struct FilePrinter : Printer {
	File file;
};

inline FilePrinter create_file_printer(Span<utf8> path) {
	FilePrinter result;
	result.file = open_file(path, {.write = true});
	result.func = [](Span<utf8> data, void *state) {
		File file = {state};
		write(file, as_bytes(data));
	};
	result.state = result.file.handle;
	return result;
}

}
