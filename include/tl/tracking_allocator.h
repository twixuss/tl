#pragma once
#include "thread.h"
#include "debug.h"
#include "hash_map.h"

namespace tl {

//
// Before accessing `tracked_allocations`, make sure that you lock `tracked_allocations_mutex`
//

struct AllocationInfo {
	std::source_location location;
	umm current_size;
	umm total_size;
};

TL_API Allocator make_tracking_allocator(Allocator underlying);
TL_API List<AllocationInfo> get_tracked_allocations(Allocator tracking_allocator);

#ifdef TL_IMPL

struct AllocationCounts {
	umm current_size;
	umm total_size;
};

struct AllocationMeta {
	AllocationCounts *counts;
	umm this_size;
};

struct TrackingAllocatorState {
	Allocator state_allocator;
	Allocator underlying_allocator;
	HashMap<std::source_location, AllocationCounts> tracked_allocations;
	Mutex tracked_allocations_mutex;
	HashMap<void *, AllocationMeta> allocation_metas;
};

Allocator make_tracking_allocator(Allocator underlying) {
	auto state_allocator = current_allocator;


	auto state = state_allocator.allocate<TrackingAllocatorState>();
	state->state_allocator = state_allocator;
	state->underlying_allocator = underlying;
	return {
		.func = [](AllocatorMode mode, void *data, umm old_size, umm new_size, umm align, std::source_location location, void *_state) -> AllocationResult {
			auto state = (TrackingAllocatorState *)_state;
			switch (mode) {
				case Allocator_allocate: {
					auto result = state->underlying_allocator.allocate_impl(new_size, align, location);

					scoped(state->tracked_allocations_mutex);
					scoped(state->state_allocator);

					auto &counts = state->tracked_allocations.get_or_insert(location);
					counts.current_size += new_size;
					counts.total_size   += new_size;

					auto &meta = state->allocation_metas.get_or_insert(result.data);
					meta.this_size = new_size;
					meta.counts = &counts;

					return result;
				}
				case Allocator_reallocate: {
					auto result = state->underlying_allocator.reallocate_impl(data, old_size, new_size, align, location);

					scoped(state->tracked_allocations_mutex);
					scoped(state->state_allocator);

					auto &old_meta = state->allocation_metas.find(data)->value;
					old_meta.counts->current_size -= old_meta.this_size;
					state->allocation_metas.erase(data);

					auto &counts = state->tracked_allocations.get_or_insert(location);
					counts.current_size += new_size;
					counts.total_size   += new_size;

					auto &new_meta = state->allocation_metas.get_or_insert(result.data);
					new_meta.this_size = new_size;
					new_meta.counts = &counts;

					return result;
				}
				case Allocator_free: {
					state->underlying_allocator.free(data, new_size, align, location);

					if (!data)
						return {};

					scoped(state->tracked_allocations_mutex);
					auto &old_meta = state->allocation_metas.find(data)->value;
					old_meta.counts->current_size -= old_meta.this_size;
					state->allocation_metas.erase(data);

					break;
				}
			}
			return {};
		},
		.state = state
	};
}
List<AllocationInfo> get_tracked_allocations(Allocator tracking_allocator) {
	auto state = (TrackingAllocatorState *)tracking_allocator.state;
	return map(state->tracked_allocations, [](auto &kv){
		return AllocationInfo {
			.location = kv.key,
			.current_size = kv.value.current_size,
			.total_size = kv.value.total_size
		};
	});
}

#endif

}
