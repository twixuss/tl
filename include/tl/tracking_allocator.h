#pragma once
#include "thread.h"
#include "debug.h"
#include "hash_map.h"

namespace tl {

extern TL_API Allocator tracking_allocator;

struct AllocationInfo {
	umm size;
	std::source_location location;
	CallStack call_stack;
};

//
// Before accessing `tracked_allocations`, make sure that you lock `tracked_allocations_mutex`
//

extern TL_API HashMap<void *, AllocationInfo> tracked_allocations;
extern TL_API Mutex tracked_allocations_mutex;

#ifdef TL_IMPL

HashMap<void *, AllocationInfo> tracked_allocations;
Mutex tracked_allocations_mutex;

forceinline void track_allocation(void *pointer, umm size, std::source_location location) {
	scoped_allocator(default_allocator);

	AllocationInfo a;
	a.size = size;
	a.location = location;
	a.call_stack = get_call_stack();

	scoped_lock(tracked_allocations_mutex);
	tracked_allocations.get_or_insert(pointer) = a;
}
forceinline void untrack_allocation(void *pointer) {
	scoped_lock(tracked_allocations_mutex);
	tracked_allocations.erase(pointer);
}
forceinline void retrack_allocation(void *old_pointer, void *new_pointer, umm size, std::source_location location) {
	scoped_allocator(default_allocator);

	AllocationInfo a;
	a.size = size;
	a.location = location;
	a.call_stack = get_call_stack();

	scoped_lock(tracked_allocations_mutex);
	tracked_allocations.get_or_insert(new_pointer) = a;
	tracked_allocations.erase(old_pointer);
}
Allocator tracking_allocator = {
	[](AllocatorMode mode, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *) -> void * {
		switch (mode) {
			case Allocator_allocate: {
				auto result = default_allocator.allocate(new_size, align, location);
				track_allocation(result, new_size, location);
				return result;
			}
			case Allocator_reallocate: {
				auto result = default_allocator.reallocate(data, old_size, new_size, align, location);
				retrack_allocation(data, result, new_size, location);
				return result;
			}
			case Allocator_free: {
				default_allocator.free(data, new_size, align, location);
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
