#include "../include/tl/string.h"
#include "../include/tl/console.h"

using namespace TL;

void string_test() {
	print("string_test ... ");
	StringBuilder<char, 16> builder;
	char const *long_string = "Surprise steepest recurred landlord mr wandered amounted of. Continuing devonshire but considered its. Rose past oh shew roof is song neat. Do depend better praise do friend garden an wonder to. Intention age nay otherwise but breakfast. Around garden beyond to extent by. ";
	builder.append(long_string);
	auto result = builder.get();
	assert(result.size == strlen(long_string));
	assert(memcmp(result.data, long_string, result.size) == 0);

	auto copyFn = [&] (Span<char const> span) {return true;};
	static_assert(isCopyFn<decltype(copyFn), char>);
	using TTT = CopyFnRet<decltype(copyFn), char>;

	to_string<char>(v3f{}, std::move(copyFn)); // TODO: move should be unnecessary

	puts("ok");
}
