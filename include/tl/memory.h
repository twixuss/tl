#pragma once
#include "system.h"

namespace tl {

TL_API umm get_page_size();
TL_API void *reserve_memory(umm size);
TL_API bool commit_memory(void *data, umm size);
TL_API bool guard_memory(void *data, umm size);
TL_API bool decommit_memory(void *data, umm size);
TL_API bool free_memory(void *data, umm size);

}

#ifdef TL_IMPL
#if OS_WINDOWS

namespace tl {

umm get_page_size() {
	SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    return sysInfo.dwPageSize;
}

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
}
#elif OS_LINUX
#include <sys/mman.h>
namespace tl {
void *reserve_memory(umm size) {
	return mmap(0, size, PROT_NONE, MAP_ANONYMOUS, -1, 0);
}
}
#endif
#endif
