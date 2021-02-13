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

struct FileTracker {
	char const *path;
	u64 last_write_time; // Represents the number of 100-nanosecond intervals since January 1, 1601 (UTC).

	Allocator allocator;
	void (*on_update)(FileTracker &tracker, void *state);
	void *on_update_state;
};

TL_API File openFile(char const *path, u32 openFlags);
TL_API File openFile(wchar const *path, u32 openFlags);
TL_API void setCursor(File file, s64 offset, FileCursor origin);
TL_API s64 getCursor(File file);
TL_API s64 length(File file);
TL_API void read(File file, void *data, u64 size);
TL_API void write(File file, void const *data, u64 size);
TL_API void truncateToCursor(File file);
TL_API void close(File file);
TL_API bool fileExists(char const *path);
TL_API bool fileExists(wchar const *path);
TL_API u64 get_file_write_time(char const *path);
TL_API List<String<char>> get_files_in_directory(char const *directory);

inline void update_file_tracker(FileTracker &tracker) {
	u64 last_write_time = get_file_write_time(tracker.path);
	if (last_write_time > tracker.last_write_time) {
		tracker.last_write_time = last_write_time;
		tracker.on_update(tracker, tracker.on_update_state);
	}
}

inline FileTracker create_file_tracker(char const *path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	FileTracker result = {};
	result.on_update = on_update;
	result.on_update_state = state;
	result.path = path;
	update_file_tracker(result);
	return result;
}
inline FileTracker create_file_tracker(char const *path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker(char const *path, Fn &&on_update) {
	auto allocator = get_allocator();

	auto params = allocate<Fn>(allocator);
	new(params) Fn(std::forward<Fn>(on_update));

	FileTracker result = create_file_tracker(path, [](FileTracker &tracker, void *state) {
		Fn &fn = *(Fn *)state;
		fn(tracker);
	}, params);
	result.allocator = allocator;
	return result;
}

inline void free_file_tracker(FileTracker &tracker) {
	if (tracker.allocator) {
		free(tracker.allocator, tracker.on_update_state);
	}
	tracker = {};
}

inline s64 length(File file) {
	auto oldCursor = getCursor(file);
	defer { setCursor(file, oldCursor, File_begin); };
	setCursor(file, 0, File_end);
	return getCursor(file);
}
inline void read(File file, Span<u8> span) { read(file, span.data(), span.size()); }
inline void write(File file, Span<u8 const> span) { write(file, span.data(), span.size());}

template <class Char = char, class T>
inline void writeString(File file, T const &value) {
	to_string<Char>(value, [&](Span<Char const> span) {
		write(file, span.data(), span.size() * sizeof(Char));
	});
}

inline Buffer read_entire_file(File file, umm extra_space_before = 0, umm extra_space_after= 0) {
	auto oldCursor = getCursor(file);
	defer { setCursor(file, oldCursor, File_begin); };

	setCursor(file, 0, File_end);
	auto size = (umm)getCursor(file);
	setCursor(file, 0, File_begin);

	auto result = create_buffer(size + extra_space_before + extra_space_after);
	read(file, result._begin + extra_space_before, size);

	return result;
}
inline Buffer read_entire_file(char const *path, umm extra_space_before = 0, umm extra_space_after= 0) {
	File file = openFile(path, File_read);
	if (file) {
		defer { close(file); };
		return read_entire_file(file, extra_space_before, extra_space_after);
	} else {
		return {};
	}
}
inline Buffer read_entire_file(wchar const *path, umm extra_space_before = 0, umm extra_space_after= 0) {
	File file = openFile(path, File_read);
	if (file) {
		defer { close(file); };
		return read_entire_file(file, extra_space_before, extra_space_after);
	} else {
		return {};
	}
}
inline Buffer read_entire_file(Span<char const> path, umm extra_space_before = 0, umm extra_space_after= 0) {
	if (path.back() == '\0')
		return read_entire_file(path.data(), extra_space_before, extra_space_after);
	else
		return read_entire_file(null_terminate(path).data(), extra_space_before, extra_space_after);
}
inline Buffer read_entire_file(Span<wchar const> path, umm extra_space_before = 0, umm extra_space_after= 0) {
	if (path.back() == '\0')
		return read_entire_file(path.data(), extra_space_before, extra_space_after);
	else
		return read_entire_file(null_terminate(path).data(), extra_space_before, extra_space_after);
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
inline bool writeEntireFile(char  const *path, Span<u8 const> span) { return writeEntireFile(path, span.data(), span.size()); }
inline bool writeEntireFile(wchar const *path, Span<u8 const> span) { return writeEntireFile(path, span.data(), span.size()); }

}

#ifdef TL_IMPL

#pragma push_macro("OS_WINDOWS")
#pragma warning(push, 0)
#include <Shlwapi.h>
#pragma warning(pop)
#pragma pop_macro("OS_WINDOWS")

#pragma comment(lib, "shlwapi")

namespace TL {

#if OS_WINDOWS
struct OpenFileParams {
	DWORD access;
	DWORD share;
	DWORD creation;
};
OpenFileParams getOpenFileParams(u32 openFlags) {
	OpenFileParams result;
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
	auto params = getOpenFileParams(openFlags);
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
	auto params = getOpenFileParams(openFlags);
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

bool fileExists(char const *path) {
	return PathFileExistsA(path);
}
bool fileExists(wchar const *path) {
	return PathFileExistsW(path);
}
u64 get_file_write_time(char const *path) {
	HANDLE file = CreateFileA(path, 0, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (file == INVALID_HANDLE_VALUE)
		return 0;
	
	defer { CloseHandle(file); };

	FILETIME last_write_time;
	if (!GetFileTime(file, 0, 0, &last_write_time))
		return 0;

	return last_write_time.dwLowDateTime | ((u64)last_write_time.dwHighDateTime << 32);
}
List<String<char>> get_files_in_directory(char const *directory) {
	auto directory_len = length(directory);
	auto allocator = get_allocator();
	char *directory_with_star;
	if (directory[directory_len - 1] == '\\' || directory[directory_len - 1] == '/') {
		directory_with_star = allocate<char>(allocator, directory_len + 2);
		memcpy(directory_with_star, directory, directory_len);
		directory_with_star[directory_len + 0] = '*';
		directory_with_star[directory_len + 1] = 0;
	} else {
		directory_with_star = allocate<char>(allocator, directory_len + 3);
		memcpy(directory_with_star, directory, directory_len);
		directory_with_star[directory_len + 0] = '/';
		directory_with_star[directory_len + 1] = '*';
		directory_with_star[directory_len + 2] = 0;
	}
	defer { free(allocator, directory_with_star); };

	WIN32_FIND_DATAA find_data;
	HANDLE handle = FindFirstFileA(directory_with_star, &find_data);
    if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	List<String<char>> result;
    do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}
        result += find_data.cFileName;
    } while (FindNextFileA(handle, &find_data));
    FindClose(handle);
	return result;
}

#else
	XXX;
#endif

}

#endif
