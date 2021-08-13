#include <tl/file.h>
using namespace tl;
void file_test() {
	File file = open_file(tl_file_string("../test/test.txt"), {.write=true});
	assert(is_valid(file));
	write(file, value_as_bytes((u32)1337));
	close(file);
	
	file = open_file(tl_file_string("../test/test.txt"), {.read=true});
	assert(is_valid(file));
	assert(remaining_bytes(file) == 4);
	u32 value;
	read(file, value_as_bytes(value));
	assert(value == 1337);
	close(file);
}