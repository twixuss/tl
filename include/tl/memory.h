#pragma once
#include "system.h"

namespace tl {

TL_API void *reserve_memory(umm size);
TL_API bool commit_memory(void *data, umm size);
TL_API bool guard_memory(void *data, umm size);
TL_API bool decommit_memory(void *data, umm size);
TL_API bool free_memory(void *data, umm size);

#ifdef TL_IMPL
#if OS_WINDOWS
void *reserve_memory(umm size) {
	return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
}
bool commit_memory(void *data, umm size) {
	return VirtualAlloc(data, size, MEM_COMMIT, PAGE_READWRITE);
}
bool guard_memory(void *data, umm size) {
	return VirtualAlloc(data, size, MEM_COMMIT, PAGE_READWRITE|PAGE_GUARD);
}
bool decommit_memory(void *data, umm size) {
	return VirtualFree(data, size, MEM_DECOMMIT);
}
bool free_memory(void *data, umm size) {
	return VirtualFree(data, size, MEM_RELEASE);
}
#else
#error memory.h is not implemeted for this platform
#endif
#endif

}
