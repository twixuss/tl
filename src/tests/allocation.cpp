//#define TL_DEBUG 0
#include <typeinfo>
#include <tl/masked_block_list.h>
#include <tl/console.h>
#include <tl/array.h>

using namespace tl;

u32 const table_size = 0x10000;
u32 *table = new u32[table_size];
u32 too_long;

template <class T>
void test_container() {

	print("%\n", typeid(T).name());

	memset(table, 0, table_size * sizeof(table[0]));
	too_long = 0;

	T container;

	u32 const test_count = 0x1000000;
	u32 const too_small = test_count / 1024;

	for (u32 i = 0; i < 0x1000000; ++i) {
		auto begin = __rdtsc();
		container.add();
		auto t = __rdtsc() - begin;
		if (t < table_size) {
			table[t] += 1;
		} else {
			too_long += 1;
		}
	}
	for (u32 i = 0; i < table_size; ++i) {
		if (table[i] >= too_small) {
			print("% x % cy\n", table[i], i);
		}
	}
	if (too_long) {
		print("more: %x\n", too_long);
	}
}

void allocation_test() {
	test_container<MaskedBlockList<u32, 256>>();
}
