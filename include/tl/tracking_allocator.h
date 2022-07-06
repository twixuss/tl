#pragma once
#include "thread.h"
#include "debug.h"
#include "hash_map.h"
#include "fly_string.h"

namespace tl {

extern TL_API Allocator tracking_allocator;
extern TL_API Allocator tracking_allocator_fallback;

//
// Before accessing `tracked_allocations`, make sure that you lock `tracked_allocations_mutex`
//

struct AllocationInfo {
	umm current_size;
	umm total_size;
};

extern TL_API HashMap<FlyString, AllocationInfo> tracked_allocations;
extern TL_API Mutex tracked_allocations_mutex;

TL_API void init_tracking_allocator();

#ifdef TL_IMPL

HashMap<FlyString, AllocationInfo> tracked_allocations;
Mutex tracked_allocations_mutex;
Allocator tracking_allocator_fallback;

struct AllocationMeta {
	AllocationInfo *info;
	umm this_size;
};

static HashMap<void *, AllocationMeta> allocation_metas;

void init_tracking_allocator() {
	construct(tracked_allocations);
	construct(allocation_metas);
}

static auto format_location(std::source_location location) {
	return (List<utf8>)format("{}:{}", location.file_name(), location.line());
}

Allocator tracking_allocator = {
	.func = [](AllocatorMode mode, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *_state) -> AllocationResult {
		switch (mode) {
			case Allocator_allocate: {
				scoped_lock(tracked_allocations_mutex);

				auto result = tracking_allocator_fallback.allocate_impl(new_size, align, location);

				scoped_allocator(os_allocator);

				auto string = format_location(location);
				defer { free(string); };

				auto &info = tracked_allocations.get_or_insert(string);
				info.current_size += new_size;
				info.total_size   += new_size;

				auto &meta = allocation_metas.get_or_insert(result.data);
				meta.this_size = new_size;
				meta.info = &info;

				return result;
			}
			case Allocator_reallocate: {
				scoped_lock(tracked_allocations_mutex);

				auto result = tracking_allocator_fallback.reallocate_impl(data, old_size, new_size, align, location);

				scoped_allocator(os_allocator);

				auto string = format_location(location);
				defer { free(string); };

				auto &old_meta = allocation_metas.find(data)->value;
				old_meta.info->current_size -= old_meta.this_size;
				allocation_metas.erase(data);

				auto &info = tracked_allocations.get_or_insert(string);
				info.current_size += new_size;
				info.total_size   += new_size;

				auto &new_meta = allocation_metas.get_or_insert(result.data);
				new_meta.this_size = new_size;
				new_meta.info = &info;

				return result;
			}
			case Allocator_free: {
				scoped_lock(tracked_allocations_mutex);

				tracking_allocator_fallback.free(data, new_size, align, location);

				auto &old_meta = allocation_metas.find(data)->value;
				old_meta.info->current_size -= old_meta.this_size;
				allocation_metas.erase(data);

				break;
			}
		}
		return {};
	},
	.state = 0
};

#endif

}
