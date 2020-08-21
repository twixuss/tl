#include "../include/tl/string.h"
using namespace TL;

#include <stdio.h>
#include <assert.h>

int main() {
	printf("%20s ... ", "StringBuilder");
	StringBuilder<OsAllocator, 16> builder;
	memset(builder.first.buffer, 'A', 16);
	for (u32 i = 0; i < 8; ++i)
		builder.append('B');

	assert(memcmp(builder.first.buffer, "BBBBBBBBAAAAAAAA", 16) == 0);
	assert(&builder.first == builder.last);
	assert(&builder.first == builder.allocLast);

	builder.ensureConsecutiveSpace(16);
	memset(builder.last->buffer, 'C', 16);
	for (u32 i = 0; i < 8; ++i)
		builder.append('D');
	
	assert(builder.first.next == builder.last);
	assert(builder.first.next == builder.allocLast);
	assert(memcmp(builder.last->buffer, "DDDDDDDDCCCCCCCC", 16) == 0);

	puts("ok");
}
