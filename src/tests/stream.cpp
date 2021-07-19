#include <tl/memory_stream.h>
#include <tl/file_stream.h>

using namespace tl;

static void read_write_test(Stream *stream) {
	u8 dest[2];
	read(stream, dest);
	assert(dest[0] == '0');
	assert(dest[1] == '1');
	read(stream, dest);
	assert(dest[0] == '2');
	assert(dest[1] == '3');
	write(stream, dest);
}
static void result_test(Span<u8> result) {
	assert(result.size == 8);
	assert(result[0] == '0');
	assert(result[1] == '1');
	assert(result[2] == '2');
	assert(result[3] == '3');
	assert(result[4] == '2');
	assert(result[5] == '3');
	assert(result[6] == '6');
	assert(result[7] == '7');
}

static void memory_stream_test() {
	u8 array[] = {'0','1','2','3','4','5','6','7'};

	auto stream = create_memory_stream(array);
	read_write_test(stream);
	free(stream);

	result_test(array);
}

static void file_stream_test() {
	auto path = tl_file_string("../temp/stream_test.txt");

	write_entire_file(path, as_bytes("01234567"));

	File file = open_file(path, File_read | File_write);

	auto stream = create_file_stream(file);
	read_write_test(stream);
	free(stream);

	close(file);

	result_test(read_entire_file(path));
}

void stream_test() {
	memory_stream_test();
	file_stream_test();
}
