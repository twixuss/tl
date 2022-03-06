#include <tl/file.h>
#include <tl/console.h>
using namespace tl;
void file_test() {
	auto test = make_absolute_path(u8"local.txt"s);

	struct PathTest {
		Span<utf8> path;
		ParsedPath expected;
	};

	PathTest path_tests[] = {
		{u8"C:/dir/file.txt"s,      u8"C:/dir"s,  u8"file"s,     u8"txt"s, },
		{u8"C:/dir/file."s,         u8"C:/dir"s,  u8"file."s,    u8""s, },
		{u8"C:/dir/.file"s,         u8"C:/dir"s,  u8".file"s,    u8""s, },
		{u8"C:/dir/file"s,          u8"C:/dir"s,  u8"file"s,     u8""s, },
		{u8"C:/dir/file.txt.lol"s,  u8"C:/dir"s,  u8"file.txt"s, u8"lol"s, },
		{u8"C:/di.r/file.txt"s,     u8"C:/di.r"s, u8"file"s,     u8"txt"s, },
		{u8"C:/di.r/file."s,        u8"C:/di.r"s, u8"file."s,    u8""s, },
		{u8"C:/di.r/.file"s,        u8"C:/di.r"s, u8".file"s,    u8""s, },
		{u8"C:/di.r/file"s,         u8"C:/di.r"s, u8"file"s,     u8""s, },
		{u8"C:/di.r/file.txt.lol"s, u8"C:/di.r"s, u8"file.txt"s, u8"lol"s, },
		{u8"file.txt"s,     u8""s, u8"file"s,     u8"txt"s, },
		{u8"file."s,        u8""s, u8"file."s,    u8""s, },
		{u8".file"s,        u8""s, u8".file"s,    u8""s, },
		{u8"file"s,         u8""s, u8"file"s,     u8""s, },
		{u8"file.txt.lol"s, u8""s, u8"file.txt"s, u8"lol"s, },
	};
	for (auto &test : path_tests) {
		auto parsed = parse_path(test.path);
		if (parsed.directory != test.expected.directory ||
			parsed.name      != test.expected.name ||
			parsed.extension != test.expected.extension)
		{
			print("Failed: path: '{}', directory: '{}', name: '{}', extension: '{}'\n", test.path, parsed.directory, parsed.name, parsed.extension);
		}
	}

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
