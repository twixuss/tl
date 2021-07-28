#pragma once
#include "system.h"

namespace tl {

TL_API umm get_ram_size();

}

#ifdef TL_IMPL
#if OS_WINDOWS

namespace tl {

umm get_ram_size() {
	ULONGLONG kilobytes = 0;
	GetPhysicallyInstalledSystemMemory(&kilobytes);
	return kilobytes * 1024;
}

}

#endif
#endif
