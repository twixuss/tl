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

TL_DECLARE_HANDLE(File);

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
	auto params = win32_getFileParams(openFlags);
	auto handle = CreateFileA(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
	return (File)handle;
}
File openFile(wchar const *path, u32 openFlags) {
	auto params = win32_getFileParams(openFlags);
	auto handle = CreateFileW(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
	return (File)handle;
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
	SetFilePointerEx((HANDLE)file, newP, 0, moveMethod);
}
s64 getCursor(File file) {
	LARGE_INTEGER curP;
	SetFilePointerEx((HANDLE)file, {}, &curP, FILE_CURRENT);
	return curP.QuadPart;
}
void read(File file, void *data_, u64 size) {
	DWORD const maxBytes = (DWORD)~0;
	DWORD bytesRead;
	u8 *data = (u8 *)data_;
	while (size > maxBytes) {
		ReadFile((HANDLE)file, data, maxBytes, &bytesRead, 0);
		data += maxBytes;
		size -= maxBytes;
	}
	if (size) {
		ReadFile((HANDLE)file, data, (DWORD)size, &bytesRead, 0);
	}
}
void write(File file, void const *data_, u64 size) {
	DWORD const maxBytes = (DWORD)~0;
	DWORD bytesWritten;
	u8 *data = (u8 *)data_;
	while (size > maxBytes) {
		WriteFile((HANDLE)file, data, maxBytes, &bytesWritten, 0);
		data += maxBytes;
		size -= maxBytes;
	}
	if (size) {
		WriteFile((HANDLE)file, data, (DWORD)size, &bytesWritten, 0);
	}
}
void close(File file) {
	CloseHandle((HANDLE)file);
}
#else
	XXX;
#endif
#endif
	
inline s64 length(File file) {
	auto oldCursor = getCursor(file);
	DEFER { setCursor(file, oldCursor, File_begin); };
	setCursor(file, 0, File_end);
	return getCursor(file);
}

inline EntireFile readEntireFile(File file, Allocator allocator = osAllocator) {
	auto oldCursor = getCursor(file);
	DEFER { setCursor(file, oldCursor, File_begin); };

	setCursor(file, 0, File_end);
	auto size = (umm)getCursor(file);
	setCursor(file, 0, File_begin);

	auto data = allocator.allocate(size);
	read(file, data, size);

	EntireFile result;
	result.allocator = allocator;
	result.data = {(char *)data, size};
	return result;
}
inline EntireFile readEntireFile(char const *path, Allocator allocator = osAllocator) {
	File file = openFile(path, File_read);
	if (file) {
		DEFER { close(file); };
		return readEntireFile(file, allocator);
	} else {
		return {};
	}
}
inline EntireFile readEntireFile(wchar const *path, Allocator allocator = osAllocator) {
	File file = openFile(path, File_read);
	if (file) {
		DEFER { close(file); };
		return readEntireFile(file, allocator);
	} else {
		return {};
	}
}
inline void freeEntireFile(EntireFile file) {
	file.allocator.deallocate(file.data.begin());
}

}
