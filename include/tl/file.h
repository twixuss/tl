#pragma once

#include "buffer.h"
#include "string.h"

namespace TL {

enum FileOpenFlags {
	File_read   = 0x1,
	File_write  = 0x2,
};

enum FileCursorOrigin {
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
	void *state;
};

TL_API File open_file(char const *path, u32 openFlags);
TL_API File open_file(wchar const *path, u32 openFlags);
TL_API void close(File file);

TL_API bool read(File file, void *data, u64 size);
template <class T>
inline bool read_bytes(File file, T &value) {
	return read(file, &value, sizeof(value));
}

TL_API bool write(File file, void const *data, u64 size);
template <class T>
inline bool write_bytes(File file, T const &value) {
	return write(file, &value, sizeof(value));
}

TL_API void set_cursor(File file, s64 offset, FileCursorOrigin origin);
TL_API s64 get_cursor(File file);
TL_API s64 length(File file);

TL_API void truncate_to_cursor(File file);
TL_API bool file_exists(char const *path);
TL_API bool file_exists(wchar const *path);
TL_API u64 get_file_write_time(char const *path);
TL_API StringList<char> get_files_in_directory(char const *directory);

inline void update_file_tracker(FileTracker &tracker) {
	u64 last_write_time = get_file_write_time(tracker.path);
	if (last_write_time > tracker.last_write_time) {
		tracker.last_write_time = last_write_time;
		tracker.on_update(tracker, tracker.state);
	}
}

inline FileTracker create_file_tracker(char const *path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	FileTracker result = {};
	result.on_update = on_update;
	result.state = state;
	result.path = path;
	update_file_tracker(result);
	return result;
}
inline FileTracker create_file_tracker(char const *path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker(char const *path, Fn &&on_update) {
	auto allocator = current_allocator;

	auto params = ALLOCATE(Fn, allocator);
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
		FREE(tracker.allocator, tracker.state);
	}
	tracker = {};
}

inline s64 length(File file) {
	auto oldCursor = get_cursor(file);
	defer { set_cursor(file, oldCursor, File_begin); };
	set_cursor(file, 0, File_end);
	return get_cursor(file);
}
forceinline void read(File file, Span<u8> span) { read(file, span.data, span.size); }
forceinline void write(File file, Span<u8> span) { write(file, span.data, span.size);}

template <class Char = char, class T>
inline void writeString(File file, T const &value) {
	to_string<Char>(value, [&](Span<Char> span) {
		write(file, span.data(), span.size() * sizeof(Char));
	});
}

inline Buffer read_entire_file(File file, umm extra_space_before = 0, umm extra_space_after= 0) {
	auto oldCursor = get_cursor(file);
	defer { set_cursor(file, oldCursor, File_begin); };

	set_cursor(file, 0, File_end);
	auto size = (umm)get_cursor(file);
	set_cursor(file, 0, File_begin);

	auto result = create_buffer(size + extra_space_before + extra_space_after);
	read(file, result.data + extra_space_before, size);

	return result;
}
inline Buffer read_entire_file(char const *path, umm extra_space_before = 0, umm extra_space_after= 0) {
	File file = open_file(path, File_read);
	if (file) {
		defer { close(file); };
		return read_entire_file(file, extra_space_before, extra_space_after);
	} else {
		return {};
	}
}
inline Buffer read_entire_file(wchar const *path, umm extra_space_before = 0, umm extra_space_after= 0) {
	File file = open_file(path, File_read);
	if (file) {
		defer { close(file); };
		return read_entire_file(file, extra_space_before, extra_space_after);
	} else {
		return {};
	}
}
inline Buffer read_entire_file(Span<char> path, umm extra_space_before = 0, umm extra_space_after= 0) {
	if (path.back() == '\0') {
		return read_entire_file(path.data, extra_space_before, extra_space_after);
	} else {
		auto null_terminated_path = null_terminate(path);
		defer { free(null_terminated_path); };
		return read_entire_file(null_terminated_path.data, extra_space_before, extra_space_after);
	}
}
inline Buffer read_entire_file(Span<wchar> path, umm extra_space_before = 0, umm extra_space_after= 0) {
	if (path.back() == '\0') {
		return read_entire_file(path.data, extra_space_before, extra_space_after);
	} else {
		auto null_terminated_path = null_terminate(path);
		defer { free(null_terminated_path); };
		return read_entire_file(null_terminated_path.data, extra_space_before, extra_space_after);
	}
}
inline void truncate(File file, u64 size) {
	set_cursor(file, (s64)size, File_begin);
	truncate_to_cursor(file);
}
inline void write_entire_file(File file, void const *data, u64 size) {
	set_cursor(file, 0, File_begin);
	write(file, data, size);
	truncate_to_cursor(file);
}
inline bool write_entire_file(char const *path, void const *data, u64 size) {
	File file = open_file(path, File_write);
	if (!file) return false;
	write_entire_file(file, data, size);
	close(file);
	return true;
}
inline bool write_entire_file(wchar const *path, void const *data, u64 size) {
	File file = open_file(path, File_write);
	if (!file) return false;
	write_entire_file(file, data, size);
	close(file);
	return true;
}
inline bool write_entire_file(Span<char> path, void const *data, u64 size) {
	if (path.back() == '\0') {
		return write_entire_file(path.data, data, size);
	} else {
		auto null_terminated_path = null_terminate(path);
		defer { free(null_terminated_path); };
		return write_entire_file(null_terminated_path.data, data, size);
	}
}
forceinline bool write_entire_file(char  const *path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }
forceinline bool write_entire_file(wchar const *path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }
forceinline bool write_entire_file(Span<char> path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }

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
File open_file(char const *path, u32 openFlags) {
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
File open_file(wchar const *path, u32 openFlags) {
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
void set_cursor(File file, s64 offset, FileCursorOrigin origin) {
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
s64 get_cursor(File file) {
	LARGE_INTEGER curP;
	SetFilePointerEx((HANDLE)file, {}, &curP, FILE_CURRENT);
	return curP.QuadPart;
}
bool read(File file, void *data_, u64 size) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_read = 0;
	u8 *data = (u8 *)data_;
	u64 total_bytes_read = 0;
	u64 remaining = size;
	while (remaining > max_bytes) {
		ReadFile((HANDLE)file, data, max_bytes, &bytes_read, 0);
		data += max_bytes;
		remaining -= max_bytes;
		total_bytes_read += bytes_read;
	}
	if (remaining) {
		ReadFile((HANDLE)file, data, (DWORD)remaining, &bytes_read, 0);
		total_bytes_read += bytes_read;
	}
	return total_bytes_read == size;
}
bool write(File file, void const *data_, u64 size) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_written = 0;
	u8 *data = (u8 *)data_;
	u64 total_bytes_written = 0;
	u64 remaining = size;
	while (remaining > max_bytes) {
		WriteFile((HANDLE)file, data, max_bytes, &bytes_written, 0);
		data += max_bytes;
		remaining -= max_bytes;
		total_bytes_written += bytes_written;
	}
	if (remaining) {
		WriteFile((HANDLE)file, data, (DWORD)remaining, &bytes_written, 0);
		total_bytes_written += bytes_written;
	}
	return total_bytes_written == size;
}
void truncate_to_cursor(File file) {
	SetEndOfFile((HANDLE)file);
}
void close(File file) {
	CloseHandle((HANDLE)file);
}

bool file_exists(char const *path) {
	return PathFileExistsA(path);
}
bool file_exists(wchar const *path) {
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
StringList<char> get_files_in_directory(char const *directory) {
	auto directory_len = length(directory);
	auto allocator = current_allocator;
	char *directory_with_star;
	if (directory[directory_len - 1] == '\\' || directory[directory_len - 1] == '/') {
		directory_with_star = ALLOCATE(char, allocator, directory_len + 2);
		memcpy(directory_with_star, directory, directory_len);
		directory_with_star[directory_len + 0] = '*';
		directory_with_star[directory_len + 1] = 0;
	} else {
		directory_with_star = ALLOCATE(char, allocator, directory_len + 3);
		memcpy(directory_with_star, directory, directory_len);
		directory_with_star[directory_len + 0] = '/';
		directory_with_star[directory_len + 1] = '*';
		directory_with_star[directory_len + 2] = 0;
	}
	defer { FREE(allocator, directory_with_star); };

	WIN32_FIND_DATAA find_data;
	HANDLE handle = FindFirstFileA(directory_with_star, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	StringList<char> result;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}
		result.add(find_data.cFileName);
	} while (FindNextFileA(handle, &find_data));
	FindClose(handle);

	result.make_absolute();
	return result;
}

#else
	XXX;
#endif

}

#endif
