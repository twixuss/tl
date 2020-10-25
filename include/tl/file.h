#pragma once

#include "common.h"
#include "string.h"

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

TL_API File openFile(char const *path, u32 openFlags);
TL_API File openFile(wchar const *path, u32 openFlags);
TL_API void setCursor(File file, s64 offset, FileCursor origin);
TL_API s64 getCursor(File file);
TL_API s64 length(File file);
TL_API void read(File file, void *data, u64 size);
TL_API void write(File file, void const *data, u64 size);
TL_API void truncateToCursor(File file);
TL_API void close(File file);

inline s64 length(File file) {
	auto oldCursor = getCursor(file);
	DEFER { setCursor(file, oldCursor, File_begin); };
	setCursor(file, 0, File_end);
	return getCursor(file);
}
inline void read(File file, Span<void> span) { read(file, span.data(), span.size()); }
inline void write(File file, Span<void const> span) { write(file, span.data(), span.size());}

template <class Char = char, class T>
inline void writeString(File file, T const &value) {
	toString<Char>(value, [&](Char const *string, umm length) {
		write(file, string, length * sizeof(Char));
	});
}

template <class Allocator = TL_DEFAULT_ALLOCATOR>
inline Buffer<Allocator> readEntireFile(File file, umm extraPreSpace = 0, umm extraPostSpace = 0) {
	auto oldCursor = getCursor(file);
	DEFER { setCursor(file, oldCursor, File_begin); };

	setCursor(file, 0, File_end);
	auto size = (umm)getCursor(file);
	setCursor(file, 0, File_begin);

	void *data = ALLOCATE(Allocator, size + extraPreSpace + extraPostSpace, 0);
	read(file, (u8 *)data + extraPreSpace, size);

	return {data, size + extraPreSpace + extraPostSpace};
}
template <class Allocator = TL_DEFAULT_ALLOCATOR>
inline Buffer<Allocator> readEntireFile(char const *path, umm extraPreSpace = 0, umm extraPostSpace = 0) {
	File file = openFile(path, File_read);
	if (file) {
		DEFER { close(file); };
		return readEntireFile<Allocator>(file, extraPreSpace, extraPostSpace);
	} else {
		return {};
	}
}
template <class Allocator = TL_DEFAULT_ALLOCATOR>
inline Buffer<Allocator> readEntireFile(wchar const *path, umm extraPreSpace = 0, umm extraPostSpace = 0) {
	File file = openFile(path, File_read);
	if (file) {
		DEFER { close(file); };
		return readEntireFile<Allocator>(file, extraPreSpace, extraPostSpace);
	} else {
		return {};
	}
}
inline void truncate(File file, u64 size) {
	setCursor(file, (s64)size, File_begin);
	truncateToCursor(file);
}
inline void writeEntireFile(File file, void const *data, u64 size) {
	setCursor(file, 0, File_begin);
	write(file, data, size);
	truncateToCursor(file);
}
inline bool writeEntireFile(char const *path, void const *data, u64 size) {
	File file = openFile(path, File_write);
	if (!file) return false;
	writeEntireFile(file, data, size);
	close(file);
	return true;
}
inline bool writeEntireFile(wchar const *path, void const *data, u64 size) {
	File file = openFile(path, File_write);
	if (!file) return false;
	writeEntireFile(file, data, size);
	close(file);
	return true;
}
inline bool writeEntireFile(char  const *path, Span<void const> span) { return writeEntireFile(path, span.data(), span.size()); }
inline bool writeEntireFile(wchar const *path, Span<void const> span) { return writeEntireFile(path, span.data(), span.size()); }

#ifdef TL_IMPL

#if OS_WINDOWS
struct FileParams {
	DWORD access;
	DWORD share;
	DWORD creation;
};
FileParams getFileParams(u32 openFlags) {
	FileParams result;
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
	auto params = getFileParams(openFlags);
	auto handle = CreateFileA(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(handle);
		handle = 0;
	}
	if (handle == INVALID_HANDLE_VALUE)
		handle = 0;
	return (File)handle;
}
File openFile(wchar const *path, u32 openFlags) {
	auto params = getFileParams(openFlags);
	auto handle = CreateFileW(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(handle);
		handle = 0;
	}
	if (handle == INVALID_HANDLE_VALUE)
		handle = 0;
	return (File)handle;
}
void setCursor(File file, s64 offset, FileCursor origin) {
	LARGE_INTEGER newP;
	newP.QuadPart = offset;
	DWORD moveMethod;
	switch (origin) {
		case File_begin: moveMethod = FILE_BEGIN; break;
		case File_cursor: moveMethod = FILE_CURRENT; break;
		case File_end: moveMethod = FILE_END; break;
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
void truncateToCursor(File file) {
	SetEndOfFile((HANDLE)file);
}
void close(File file) {
	CloseHandle((HANDLE)file);
}
#else
	XXX;
#endif
#endif

}
