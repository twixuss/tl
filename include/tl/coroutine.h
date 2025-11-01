#pragma once
#include "common.h"
namespace tl {

struct CoroutineState {
	void *return_addr = 0;
	void *rsp = 0;
	void *stack_base = 0;
	Allocator allocator = {};
};

using Coroutine = void *(*)(CoroutineState &, void *);

extern "C" void *tl_coroutine_yield(CoroutineState &, void *);

forceinline void *yield(CoroutineState &state, void *value) { return tl_coroutine_yield(state, value); }
forceinline void *run(CoroutineState &state) { return tl_coroutine_yield(state, 0); }

inline static constexpr umm coroutine_saved_registers_count = 8;

inline void init(CoroutineState &state, Coroutine coroutine, umm stack_size = 1024*1024 TL_LP) {
	assert(stack_size >= 16);
	assert(stack_size % 16 == 0);
	state.allocator = current_allocator;
	state.stack_base = state.allocator.allocate<u8>(stack_size, 16 TL_LA);
	state.rsp =
		(u8 *)state.stack_base + stack_size
		- coroutine_saved_registers_count * 8
		- 8 // return address
		- 16; // shadow space
	state.return_addr = (void *)coroutine;
	memset(state.rsp, 0xcd, (u8 *)state.stack_base + stack_size - (u8*)state.rsp);
}
inline void free(CoroutineState &state) {
	state.allocator.free(state.stack_base);
	state = {};
}

}

#ifdef TL_IMPL
#if !OS_WINDOWS
extern "C" void *tl_coroutine_yield(tl::CoroutineState &, void *) {
	not_implemented();
}
#endif
#endif
