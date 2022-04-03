#pragma once
#include "common.h"
#include <typeinfo>

namespace tl {

TL_API void *reserve_memory(umm size);
TL_API bool commit_memory(void *data, umm size);

// Provides strorage, addressable by 32-bit number in constant time.
template <class Tag>
struct Pool32 : AllocatorBase<Pool32<Tag>> {
	template <class T>
	struct Ptr {
		u32 offset = 0;

		forceinline Ptr() = default;
		forceinline Ptr(T *pointer) {
			assert(base);

			if (!pointer) {
				offset = 0;
				return;
			}

			umm real_offset = (u8 *)pointer - base;
			offset = (u32)real_offset;
			assert(offset == real_offset);
			assert(offset);
		}

		template <class U, class = EnableIf<std::is_convertible_v<U, T>>>
		forceinline Ptr &operator=(Pool32<Tag>::Ptr<U> that) {
			assert(base);
			offset = that.offset;
			return *this;
		}

		forceinline operator T *() {
			assert(base);
			if (offset == 0)
				return 0;
			return (T *)(base + offset);
		}
		template <class U>
		forceinline explicit operator U *() {
			assert(base);
			if (offset == 0)
				return 0;
			return (U *)(base + offset);
		}

		forceinline T *operator->() {
			assert(base);
			assert(offset);
			return (T *)(base + offset);
		}
		forceinline T &operator*() {
			return *operator->();
		}

		T *raw() {
			return (T *)(base + offset);
		}
	};

	inline static u8 *base;
	inline static u8 *next_free;
	inline static u8 *next_uncommitted;

	inline static Pool32 current() { return {}; }

	static void init() {
		base = (u8 *)reserve_memory(4 * GiB);
		if (!base) {
			print(Print_error, "ERROR :: FAILED TO RESERVE MEMORY FOR {}\n", typeid(Pool32).name());
			exit(-1);
		}
		next_free = base + 1; // first byte is reserved for `null` pointer, s+o skip it.
		next_uncommitted = 0;
	}

	AllocationResult allocate_impl(umm size, umm alignment, std::source_location location) {
		next_free = (u8 *)ceil(next_free, alignment);

		if (next_free + size > next_uncommitted) {
			if (!commit_memory(next_free, size)) {
				print(Print_error, "ERROR :: FAILED TO COMMIT MEMORY FOR {}\n", typeid(Pool32).name());
				exit(-1);
			}
			next_uncommitted = (u8 *)ceil(next_free + size + 1, 4096);
		}
		defer { next_free += size; };
		return {next_free, size};
	}

	AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment, std::source_location location = std::source_location::current()) {
		invalid_code_path("not implemented");
	}

	void deallocate_impl(void *data, umm size, umm alignment, std::source_location location) {
		print("Pool32::deallocate is not implemented yet. leaking.\n");
	}

};

#ifdef TL_IMPL
#if OS_WINDOWS
void *reserve_memory(umm size) {
	return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
}

bool commit_memory(void *data, umm size) {
	return VirtualAlloc(data, size, MEM_COMMIT, PAGE_READWRITE);
}
#else
#error pool32.h is not implemeted for this platform
#endif
#endif

}
