#pragma once

#include "common.h"

namespace TL {

enum FileOpenFlags {
	File_read   = 0x1,
	File_write  = 0x2,
};

enum FileCursor {
	File_begin,
	File_cursor,
	File_end,
};

struct File {
	void *handle;
};

struct EntireFile {
	Allocator allocator;
	StringSpan data;
};

File openFile(char const *path, u32 openFlags);
File openFile(wchar const *path, u32 openFlags);
void setCursor(File file, s64 offset, FileCursor origin);
s64 getCursor(File file);
s64 length(File file);
void read(File file, void *data, u64 size);
void write(File file, void const *data, u64 size);
void close(File);

EntireFile readEntireFile(char const *path, Allocator allocator = osAllocator);
EntireFile readEntireFile(wchar const *path, Allocator allocator = osAllocator);
void freeEntireFile(EntireFile file);

#ifdef TL_IMPL

#if OS_WINDOWS
struct win32_FileParams {
	DWORD access;
	DWORD share;
	DWORD creation;
};
win32_FileParams win32_getFileParams(u32 openFlags) {
	win32_FileParams result;
	if ((openFlags & File_read) && (openFlags & File_write)) {
		result.access = GENERIC_READ | GENERIC_WRITE;
		result.share = 0;
		result.creation = OPEN_ALWAYS;
	} else if (openFlags & File_read) {
		result.access = GENERIC_READ;
		result.share = FILE_SHARE_READ;
		result.creation = OPEN_EXISTING;
	} else if (openFlags & File_write) {
		result.access = GENERIC_WRITE;
		result.share = 0;
		result.creation = CREATE_ALWAYS;
	} else {
		result.access = 0;
		result.share = 0;
		result.creation = 0;
	}
	return result;
}
File openFile(char const *path, u32 openFlags) {
	File result;
	auto params = win32_getFileParams(openFlags);
	result.handle = CreateFileA(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(result.handle);
		result.handle = 0;
	}
	return result;
}
File openFile(wchar const *path, u32 openFlags) {
	File result;
	auto params = win32_getFileParams(openFlags);
	result.handle = CreateFileW(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(result.handle);
		result.handle = 0;
	}
	return result;
}
void setCursor(File file, s64 offset, FileCursor origin) {
	LARGE_INTEGER newP;
	newP.QuadPart = offset;
	DWORD moveMethod;
	switch (origin) {
		case TL::File_begin: moveMethod = FILE_BEGIN; break;
		case TL::File_cursor: moveMethod = FILE_CURRENT; break;
		case TL::File_end: moveMethod = FILE_END; break;
		default: return;
	}
	SetFilePointerEx(file.handle, newP, 0, moveMethod);
}
s64 getCursor(File file) {
	LARGE_INTEGER curP;
	SetFilePointerEx(file.handle, {}, &curP, FILE_CURRENT);
	return curP.QuadPart;
}
void read(File file, void *data_, u64 size) {
	DWORD const maxBytes = ~0;
	DWORD bytesRead;
	u8 *data = (u8 *)data_;
	while (size > maxBytes) {
		ReadFile(file.handle, data, maxBytes, &bytesRead, 0);
		data += maxBytes;
		size -= maxBytes;
	}
	if (size) {
		ReadFile(file.handle, data, size, &bytesRead, 0);
	}
}
void write(File file, void const *data_, u64 size) {
	DWORD const maxBytes = ~0;
	DWORD bytesWritten;
	u8 *data = (u8 *)data_;
	while (size > maxBytes) {
		WriteFile(file.handle, data, maxBytes, &bytesWritten, 0);
		data += maxBytes;
		size -= maxBytes;
	}
	if (size) {
		WriteFile(file.handle, data, size, &bytesWritten, 0);
	}
}
void close(File file) {
	CloseHandle(file.handle);
}
#else
	XXX;
#endif
	
s64 length(File file) {
	auto oldCursor = getCursor(file);
	DEFER { setCursor(file, oldCursor, File_begin); };
	setCursor(file, 0, File_end);
	return getCursor(file);
}

EntireFile readEntireFile(File file, Allocator allocator) {
	auto size = (umm)length(file);
	auto data = allocator.allocate(size);
	read(file, data, size);
	EntireFile result;
	result.allocator = allocator;
	result.data = {(char *)data, size};
	return result;
}
EntireFile readEntireFile(char const *path, Allocator allocator) {
	File file = openFile(path, File_read);
	DEFER { close(file); };
	return readEntireFile(file, allocator);
}
EntireFile readEntireFile(wchar const *path, Allocator allocator) {
	File file = openFile(path, File_read);
	DEFER { close(file); };
	return readEntireFile(file, allocator);
}
void freeEntireFile(EntireFile file) {
	file.allocator.deallocate(file.data.begin());
}

#endif

}
