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
void setCursor(File file, s64 offset, FileCursor origin);
s64 getCursor(File file);
s64 length(File file);
void closeFile(File);

EntireFile readEntireFile(char const *path, Allocator allocator = osAllocator);
void freeEntireFile(EntireFile file);

#if OS_WINDOWS
File openFile(char const *path, u32 openFlags) {
	File result;

	DWORD access = 0;
	DWORD share = 0;
	DWORD creation = 0;

	if ((openFlags & File_read) && (openFlags & File_write)) { 
		access = GENERIC_READ | GENERIC_WRITE;
		share = 0;
		creation = OPEN_ALWAYS;
	} else if (openFlags & File_read) { 
		access = GENERIC_READ;
		share = FILE_SHARE_READ;
		creation = OPEN_EXISTING;
	} else if (openFlags & File_write) { 
		access = GENERIC_WRITE;
		share = 0;
		creation = CREATE_ALWAYS;
	}

	result.handle = CreateFileA(path, openFlags, share, 0, creation, 0, 0);


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
#else
	XXX;
#endif
	
s64 length(File file) {
	auto oldCursor = getCursor(file);
	DEFER { setCursor(file, oldCursor, File_begin); };
	setCursor(file, 0, File_end);
	return getCursor(file);
}

EntireFile readEntireFile(char const *path, Allocator allocator) {
	File file = openFile(path, File_read);
	auto size = (umm)length(file);
	auto data = allocator.allocate(size);
	EntireFile result;
	result.allocator = allocator;
	result.data = {(char *)data, size};
	return result;
}
void freeEntireFile(EntireFile file) {
	file.allocator.deallocate(file.data.begin());
}

}
