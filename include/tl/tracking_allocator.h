#pragma once
#include "thread.h"
#include "debug.h"
#include "hash_map.h"

namespace tl {

//
// Before accessing `tracked_allocations`, make sure that you lock `tracked_allocations_lock`
//

struct AllocationInfo {
	std::source_location location;
	umm current_size;
	umm total_size;
};

inline void append(StringBuilder &builder, AllocationInfo info) {
	append_format(builder, "{} current: {} total: {}", OnlyFileAndLine{info.location}, format_bytes(info.current_size), format_bytes(info.total_size));
}

struct AllocationCounts {
	umm current_size;
	umm total_size;
};

struct AllocationMeta {
	AllocationCounts *counts;
	umm this_size;
};

struct TrackingAllocator : AllocatorBase<TrackingAllocator> {
	struct SourceLocationHashTraits : DefaultHashTraits<std::source_location> {
		constexpr u64 get_hash(std::source_location value) const {
			return 
				(u64)value.column() * (u64)1153663161 + 
				(u64)value.line() * (u64)722716341 + 
				(u64)::get_hash(as_span(value.file_name())) * (u64)834696729;
		}
		constexpr bool are_equal(std::source_location a, std::source_location b) const {
			return 
				a.column() == b.column() &&
				a.line() == b.line() &&
				as_span(a.file_name()) == as_span(b.file_name());
		}
	};
	struct Shared {
		HashMap<std::source_location, AllocationCounts, SourceLocationHashTraits, DefaultAllocator> tracked_allocations;
		HashMap<void *, AllocationMeta, DefaultHashTraits<void *>, DefaultAllocator> allocation_metas;
	};

	inline static LockProtected<Shared, RecursiveSpinLock> shared;

	AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
		auto result = DefaultAllocator{}.allocate_impl(size, alignment, location);

		shared.use([&] (auto &shared) {
			auto &counts = shared.tracked_allocations.get_or_insert(location);
			counts.current_size += size;
			counts.total_size   += size;

			auto &meta = shared.allocation_metas.get_or_insert(result.data);
			meta.this_size = size;
			meta.counts = &counts;
		});

		return result;
	}
	AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP) {
		auto result = DefaultAllocator{}.reallocate_impl(data, old_size, new_size, alignment, location);

		shared.use([&] (auto &shared) {
			if (auto found = shared.allocation_metas.find(data)) {
				auto &old_meta = *found.value;
				old_meta.counts->current_size -= old_meta.this_size;
				shared.allocation_metas.erase(data);

				auto &counts = shared.tracked_allocations.get_or_insert(location);
				counts.current_size += new_size;
				counts.total_size   += new_size;

				auto &new_meta = shared.allocation_metas.get_or_insert(result.data);
				new_meta.this_size = new_size; 
				new_meta.counts = &counts;
			}
		});
		return result;
	}
	void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
		DefaultAllocator{}.free(data, size, alignment, location);

		if (!data)
			return;

		shared.use([&] (auto &shared) {
			auto found = shared.allocation_metas.find(data);
			assert(found, "Attempt to free unallocated / double free at {}", data);
			auto &old_meta = *found.value;
			old_meta.counts->current_size -= old_meta.this_size;
			shared.allocation_metas.erase(data);
		});
	}

	operator Allocator() {
		return make_allocator_from<TrackingAllocator>(this);
	}

	static TrackingAllocator current() { return {}; }

	static List<AllocationInfo> get_tracked_allocations(TL_LPC) {
		return shared.use([&] (auto &shared) {
			return shared.tracked_allocations.map([](auto kv){
				return AllocationInfo {
					.location = *kv.key,
					.current_size = kv.value->current_size,
					.total_size = kv.value->total_size
				};
			} TL_LA);
		});
	}
};


template <>
struct Scoped<TrackingAllocator> : Scoped<Allocator> {
	void enter(TrackingAllocator) {
		Scoped<Allocator>::enter(TrackingAllocator{});
	}
	void exit() {
		Scoped<Allocator>::exit();
	}
};
}
