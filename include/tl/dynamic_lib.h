#pragma once
#include "common.h"
#include "logger.h"

namespace tl {

struct Dll {
	void *handle = 0;
	bool should_unload = false; // TODO: windows implementation uses GetModuleHandleW, which might be unnecessary if LoadLibraryW does the same thing. If so, this member can be removed.

	explicit operator bool() const { return handle; }
};

#if OS_WINDOWS
constexpr Span<utf8> dll_extension = u8".dll"s;
#elif OS_LINUX
constexpr Span<utf8> dll_extension = u8".so"s;
#endif

TL_API Dll load_dll(Span<utf8> path);
TL_API void *get_symbol(Dll dll, Span<utf8> name);
TL_API void free(Dll &dll);

}

#ifdef TL_IMPL

#if OS_WINDOWS

namespace tl {

Dll load_dll(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	Dll result = {};
	result.handle = GetModuleHandleW((wchar_t *)path16.data);
	if (!result.handle) {
		result.handle = LoadLibraryW((wchar_t *)path16.data);
		result.should_unload = true;
	}
	if (!result.handle) {
		current_logger.error("Library {} was not found", path8);
	}
	return result;
}
void *get_symbol(Dll dll, Span<utf8> name) {
	scoped(temporary_allocator_and_checkpoint);
	return GetProcAddress((HMODULE)dll.handle, (char *)null_terminate(name).data);
}
void free(Dll &dll) {
	if (dll.should_unload) {
		FreeLibrary((HMODULE)dll.handle);
	}
	dll = {};
}

}

#elif OS_LINUX

#include <dlfcn.h>

namespace tl {

Dll load_dll(Span<utf8> path) {
	scoped(temporary_allocator_and_checkpoint);
	Dll result = {};
	result.handle = dlopen((char *)null_terminate(path).data, RTLD_NOW);
	if (!result.handle) {
		current_logger.error("Library {} was not found", path);
	}
	return result;
}
void *get_symbol(Dll dll, Span<utf8> name) {
	scoped(temporary_allocator_and_checkpoint);
	return dlsym(dll.handle, (char *)null_terminate(name).data);
}
void free(Dll &dll) {
	dlclose(dll.handle);
	dll = {};
}

}

#endif

#endif