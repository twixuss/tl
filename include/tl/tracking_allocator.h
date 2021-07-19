#pragma once
#include "thread.h"
#include "debug.h"
#include <unordered_map>

namespace tl {

extern TL_API Allocator tracking_allocator;

struct AllocationInfo {
	umm size;
	std::source_location location;
	CallStack call_stack;
};

TL_API std::unordered_map<void *, AllocationInfo> get_tracked_allocations();

#ifdef TL_IMPL

std::unordered_map<void *, AllocationInfo> allocations;
Mutex allocations_mutex;
std::unordered_map<void *, AllocationInfo> get_tracked_allocations() {
	scoped_lock(allocations_mutex);
	return allocations;
}
forceinline void track_allocation(void *pointer, umm size, std::source_location location) {
	scoped_allocator(default_allocator);

	scoped_lock(allocations_mutex);
	AllocationInfo a;
	a.size = size;
	a.location = location;
	a.call_stack = get_call_stack();
	allocations[pointer] = a;
}
forceinline void untrack_allocation(void *pointer) {
	scoped_lock(allocations_mutex);
	allocations.erase(pointer);
}
forceinline void retrack_allocation(void *old_pointer, void *new_pointer, umm size, std::source_location location) {
	scoped_allocator(default_allocator);

	scoped_lock(allocations_mutex);
	AllocationInfo a;
	a.size = size;
	a.location = location;
	a.call_stack = get_call_stack();
	allocations[new_pointer] = a;
	allocations.erase(old_pointer);
}
Allocator tracking_allocator = {
	[](AllocatorMode mode, AllocateFlags flags, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *) -> void * {
		switch (mode) {
			case Allocator_allocate: {
				auto result = default_allocator.allocate(flags, new_size, align, location);
				track_allocation(result, new_size, location);
				return result;
			}
			case Allocator_reallocate: {
				auto result = default_allocator.reallocate(flags, data, old_size, new_size, align, location);
				retrack_allocation(data, result, new_size, location);
				return result;
			}
			case Allocator_free: {
				default_allocator.free(data, location);
				untrack_allocation(data);
				break;
			}
		}
		return 0;
	},
	0
};

#endif

}
