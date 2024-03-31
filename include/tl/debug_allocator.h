#pragma once
#include "common.h"

namespace tl {

struct DebugAllocator : AllocatorBase<DebugAllocator> {
	static constexpr u8 pre_byte = 0xaa;
	static constexpr u8 post_byte = 0xbb;
	static constexpr umm check_space = 64;

	static DebugAllocator current() { return {}; }

	bool is_valid() {
		return true;
	}

	AllocationResult allocate_impl(umm size, umm alignment TL_LP);
	AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP);
	void deallocate_impl(void *data, umm size, umm alignment TL_LP);
};

}

#ifdef TL_IMPL

#include "hash_map.h"
#include "thread.h"

namespace tl {

static HashMap<void *, umm, DefaultHashTraits<void *>, DefaultAllocator> allocation_sizes;
static OsLock allocation_lock;

AllocationResult DebugAllocator::allocate_impl(umm size, umm alignment TL_LPD) {
	auto memory = tl_allocate(size + check_space*2, alignment);
	for (umm i = 0; i < check_space; ++i) {
		((u8 *)memory)[i] = pre_byte;
		((u8 *)memory)[check_space + size + i] = post_byte;
	}
	with(allocation_lock, allocation_sizes.get_or_insert((u8 *)memory + check_space) = size);
	return {
		.data = (u8 *)memory + check_space,
		.count = size,
		.is_zeroed = false,
	};
}
AllocationResult DebugAllocator::reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LPD) {
	(void)old_size;
	auto result = allocate_impl(new_size, alignment);
	memcpy(result.data, data, with(allocation_lock, allocation_sizes.find(data)->value));
	deallocate_impl(data, old_size, alignment);
	return result;
}
void DebugAllocator::deallocate_impl(void *data, umm size, umm alignment TL_LPD) {
	if (!data)
		return;
	(void)size;
	(void)alignment;
	size = with(allocation_lock, allocation_sizes.find(data)->value);
	auto base = (u8 *)data - check_space;
	for (umm i = 0; i < check_space; ++i) {
		assert(base[i] == pre_byte);
		assert(base[check_space + size + i] == post_byte);
	}
	tl_free(base);
}

}

#endif
