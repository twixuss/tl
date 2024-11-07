#include <tl/memory_stream.h>
#include <tl/file_stream.h>

using namespace tl;

static void read_write_test(Stream *stream) {
	u8 dest[2];
	stream->read(dest);
	assert(dest[0] == '0');
	assert(dest[1] == '1');
	stream->read(dest);
	assert(dest[0] == '2');
	assert(dest[1] == '3');
	assert(stream->remaining_bytes() == 4);
	stream->write(dest);
}
static void result_test(Span<u8> result) {
	assert(result.count == 8);
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
	auto path = tl_file_string("stream_test.txt"s);

	write_entire_file(path, as_bytes("01234567"));

	File file = open_file(path, {.read=true, .write=true});

	auto stream = create_file_stream(file);
	read_write_test(stream);
	free(stream);

	close(file);

	result_test(read_entire_file(as_span(path)));
}

void stream_test() {
	memory_stream_test();
	file_stream_test();
}
