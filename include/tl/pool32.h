#pragma once
#include "common.h"
#include "memory.h"
#include "string.h"
#include <typeinfo>

// Provides linear strorage addressable by 32-bit number.
// This basically allows to have 32-bit pointers in 64-bit programs.

namespace tl {

template <class Tag>
struct Pool32 : AllocatorBase<Pool32<Tag>> {
	template <class T>
	struct Ptr {
		using Pool = Pool32;
		using Element = T;

		u32 offset = 0;

		forceinline Ptr() = default;
		forceinline Ptr(T *pointer) {
			*this = pointer;
		}
		template <class U>
		requires std::is_base_of_v<T, U>
		forceinline Ptr(Ptr<U> pointer) : Ptr(pointer.offset) {}


		forceinline Ptr &operator=(T *that) {
			assert(base);

			if (!that) {
				offset = 0;
				return *this;
			}

			umm full_offset = (u8 *)that - base;
			offset = (u32)full_offset;
			assert_equal(offset, full_offset);
			assert(offset);
			return *this;
		}

		template <class U>
		requires std::is_base_of_v<T, U>
		forceinline Ptr &operator=(Pool32<Tag>::Ptr<U> that) {
			assert(base);
			offset = that.offset;
			return *this;
		}

		forceinline explicit operator bool() { return offset; }

		template <class U>
		forceinline explicit(!(std::is_same_v<T, U> || std::is_base_of_v<U, T>)) operator U *() { return (U *)raw(); }

		template <class U>
		forceinline explicit(!(std::is_same_v<T, U> || std::is_base_of_v<U, T>)) operator Ptr<U>() { return offset; }


		forceinline T *operator->() { assert(offset); return raw(); }
		forceinline T &operator*() { assert(offset); return *raw(); }

		forceinline bool operator==(T *that) { return raw() == that; };
		forceinline bool operator!=(T *that) { return raw() != that; };

		forceinline T *raw() {
			assert(base);
			return (T *)(((umm)base + offset) * (offset != 0));
		}
	private:
		forceinline Ptr(umm offset) : offset(offset) {}
	};

	inline static u8 *base;
	inline static u8 *next_free;
	inline static u8 *next_uncommitted;

	inline static Pool32 current() { return {}; }

	inline static void init() {
		base = (u8 *)reserve_memory(4 * GiB);
		if (!base) {
			with(ConsoleColor::red, print("ERROR :: FAILED TO RESERVE MEMORY FOR {}\n", typeid(Pool32).name()));
			exit(-1);
		}
		clear();
	}
	inline static void clear() {
		next_free = base + 1; // first byte is reserved for `null` pointer, so skip it.
		next_uncommitted = 0;
	}
	inline static umm size() {
		return next_free - base;
	}

	AllocationResult allocate_impl(umm size, umm alignment, std::source_location location) {
		next_free = (u8 *)ceil(next_free, alignment);

		if (next_free + size > next_uncommitted) {
			if (!commit_memory(next_free, size)) {
				with(ConsoleColor::red, print("ERROR :: FAILED TO COMMIT MEMORY FOR {}\n", typeid(Pool32).name()));
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
		// This allocator does not support fine-grained deallocation.
	}

};

template <class Tag, class T>
inline void append(StringBuilder &builder, typename Pool32<Tag>::template Ptr<T> ptr) {
	return append(builder, Pool32<Tag>::base + ptr.offset);
}

}
