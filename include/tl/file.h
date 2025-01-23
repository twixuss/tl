#pragma once

#include "buffer.h"
#include "string.h"
#include "list_of_lists.h"
#include "logger.h"

// Including Shlwapi.h might cause bullshit errors inside it. Until I figure this shit out can just disable dialogs.
#ifndef TL_FILE_INCLUDE_DIALOG
#define TL_FILE_INCLUDE_DIALOG 1
#endif

namespace tl {

struct OpenFileParams {
	bool read = false;
	bool write = false;
	bool silent = false;
	bool create_directories = write;
};

enum FileCursorOrigin {
	File_begin,
	File_cursor,
	File_end,
};

struct File {
	void *handle;
};

inline bool is_valid(File file) {
	return file.handle != 0;
}

TL_API File open_file(Span<utf8> path, OpenFileParams params);
template <AChar Char>
inline File open_file(Span<Char> path, OpenFileParams params) { scoped(temporary_allocator_and_checkpoint); return open_file(to_utf8(path), params); }

TL_API void close(File file);

TL_API umm read(File file, Span<u8> data);
TL_API umm write(File file, Span<u8> data);

TL_API void set_cursor(File file, s64 offset, FileCursorOrigin origin);
TL_API s64 get_cursor(File file);
TL_API void truncate_to_cursor(File file);


TL_API bool file_exists(Span<utf8> path);
template <AChar Char>
inline bool file_exists(Span<Char> path) { scoped(temporary_allocator_and_checkpoint); return file_exists(to_utf8(path)); }

TL_API bool directory_exists(Span<utf8> path);
template <AChar Char>
inline bool directory_exists(Span<Char> path) { scoped(temporary_allocator_and_checkpoint); return directory_exists(to_utf8(path)); }

template <AChar Char>
inline Optional<u64> get_file_size(Span<Char> path) {
	auto file = open_file(path, {.read = true});
	if (!is_valid(file))
		return {};
	defer { close(file); };

	set_cursor(file, 0, File_end);
	return (u64)get_cursor(file);
}

struct ReadEntireFileParams {
	umm extra_space_before = 0;
	umm extra_space_after = 0;
	bool silent : 1 = false;
};
inline Buffer read_entire_file(File file, ReadEntireFileParams params = {} TL_LP) {
	auto old_cursor = get_cursor(file);
	defer { set_cursor(file, old_cursor, File_begin); };

	set_cursor(file, 0, File_end);
	auto size = (umm)get_cursor(file);
	set_cursor(file, 0, File_begin);

	auto result = create_buffer_uninitialized(size + params.extra_space_before + params.extra_space_after TL_LA);
	read(file, {result.data + params.extra_space_before, size});

	memset(result.data, 0, params.extra_space_before);
	memset(result.data + result.count - params.extra_space_after, 0, params.extra_space_after);

	return result;
}

template <AChar Char, class Size>
inline Buffer read_entire_file(Span<Char, Size> path, ReadEntireFileParams params = {} TL_LP) {
	File file = open_file(path, {.read = true, .silent = params.silent});
	if (!is_valid(file)) return {};
	defer { close(file); };
	return read_entire_file(file, params TL_LA);
}

struct WriteEntireFileOptions {
	bool create_directories = true;
};

inline bool write_entire_file(File file, Span<u8> span) {
	set_cursor(file, 0, File_begin);
	defer { truncate_to_cursor(file); };
	return write(file, span) != 0;
}
template <AChar Char>
inline bool write_entire_file(Span<Char> path, Span<u8> span, WriteEntireFileOptions options = {}) {
	File file = open_file(path, {.write = true, .create_directories = options.create_directories});
	if (!is_valid(file)) return false;
	defer { close(file); };
	return write(file, span) != 0;
}

 // Represents the number of 100-nanosecond intervals since January 1, 1601 (UTC).
TL_API Optional<u64> get_file_write_time(File file);
template <AChar Char>
inline Optional<u64> get_file_write_time(Span<Char> path) {
	auto file = open_file(path, {});
	if (!is_valid(file)) return {};
	defer { close(file); };
	return get_file_write_time(file);
}


enum FileItemKind {
	FileItem_unknown,
	FileItem_file,
	FileItem_directory,
};

struct FileItem {
	FileItemKind kind;
	Span<utf8> name;
};
struct FileItemList : List<FileItem> {
	using Base = List<FileItem>;

	List<utf8> buffer;

	Base &base() { return *this; }
};

inline void free(FileItemList &list) {
	free(list.buffer);
	free(list.base());
}

TL_API FileItemList get_items_in_directory(Span<utf8> directory);

TL_API void create_file(Span<utf8> path);
template <AChar Char>
inline void create_file(Span<Char> path) { scoped(temporary_allocator_and_checkpoint); return create_file(to_utf8(path)); }

TL_API void delete_file(Span<utf8> path);
template <AChar Char>
inline void delete_file(Span<Char> path) { scoped(temporary_allocator_and_checkpoint); return delete_file(to_utf8(path)); }

TL_API bool create_directory(Span<utf8> path);
template <AChar Char>
inline bool create_directory(Span<Char> path) { scoped(temporary_allocator_and_checkpoint); return create_directory(to_utf8(path)); }

struct DeleteDirectoryOptions {
	bool recursive = false;
};

TL_API void delete_directory(Span<utf8> path, DeleteDirectoryOptions options = {});
template <AChar Char>
inline void delete_directory(Span<Char> path, DeleteDirectoryOptions options = {}) { scoped(temporary_allocator_and_checkpoint); return delete_directory(to_utf8(path), options); }

struct FileState {
	bool changed : 1;
	bool failed  : 1;
};

using FileTime = u64;

inline FileState detect_change(File file, FileTime *last_write_time) {
	auto maybe_retrieved_time = get_file_write_time(file);
	if (!maybe_retrieved_time) {
		return {.failed = true};
	}

	auto retrieved_time = maybe_retrieved_time.value_unchecked();

	if (*last_write_time >= retrieved_time)
		return {};

	*last_write_time = retrieved_time;
	return {.changed = true};
}
template <class Path>
requires requires(Path path) {
	open_file(path, {.read = true});
}
inline FileState detect_change(Path path, FileTime *last_write_time) {
	auto file = open_file(path, {.read=true});
	defer { close(file); };
	return detect_change(file, last_write_time);
}

inline u64 length(File file) {
	auto old_cursor = get_cursor(file);
	defer { set_cursor(file, old_cursor, File_begin); };
	set_cursor(file, 0, File_end);
	return get_cursor(file);
}
inline u64 remaining_bytes(File file) {
	auto old_cursor = get_cursor(file);
	defer { set_cursor(file, old_cursor, File_begin); };
	set_cursor(file, 0, File_end);
	return get_cursor(file) - old_cursor;
}

inline void truncate(File file, u64 size) {
	set_cursor(file, (s64)size, File_begin);
	truncate_to_cursor(file);
}

#if TL_FILE_INCLUDE_DIALOG
using FileDialogFlags = u32;
enum : FileDialogFlags {
	FileDialog_file      = 0x0,
	FileDialog_directory = 0x1,
	FileDialog_multiple  = 0x2,
};

TL_API Optional<ListOfLists<utf8>> open_file_dialog(FileDialogFlags flags, Span<Span<utf8>> allowed_extensions = {});
#endif

TL_API List<utf8> get_current_directory();

TL_API void set_current_directory(Span<utf8> path);
template <AChar Char>
inline void set_current_directory(Span<Char> path) { scoped(temporary_allocator_and_checkpoint); return set_current_directory(to_utf8(path)); }

struct MappedFile {
	Span<u8> data;
	void *mapping;
};

TL_API MappedFile map_file(File file);
TL_API void unmap_file(MappedFile &file);

struct ParsedPath {
	Span<utf8> directory;
	Span<utf8> name;
	Span<utf8> extension;

	Span<utf8> path_without_extension() {
		return {directory.begin(), name.end()};
	}
	Span<utf8> name_and_extension() {
		return {name.begin(), extension.end()};
	}
};

inline ParsedPath parse_path(Span<utf8> path) {
	if (!path.count)
		return {};
	ParsedPath result = {};

	while (path.count && (path.back() == '\\' || path.back() == '/')) {
		path.count -= 1;
	}
	auto last_slash = find_last_any(path, u8"/\\"s);
	auto last_dot = find_last(path, u8'.');
	if (last_dot) {
		if (last_slash) {
			if (last_dot > last_slash) {
				result.directory = {path.data, last_slash};
				result.name      = {last_slash + 1, last_dot};
				result.extension = {last_dot + 1, path.end()};
				// Include the dot into the name because the extension is empty
				if (result.extension.count == 0) {
					result.name.count += 1;
				}
				if (result.name.count == 0) {
					result.name = result.extension;
					result.extension = {};
					result.name.data --;
					result.name.count ++;
				}
			} else {
				result.directory = {path.data, last_slash};
				result.name      = {last_slash + 1, path.end()};
			}

		} else {
			result.name      = {path.data, last_dot};
			result.extension = {last_dot + 1, path.end()};
			// Include the dot into the name because the extension is empty
			if (result.extension.count == 0) {
				result.name.count += 1;
			}
			if (result.name.count == 0) {
				result.name = result.extension;
				result.extension = {};
				result.name.data --;
				result.name.count ++;
			}
		}
	} else {
		if (last_slash) {
			result.directory = {path.data, last_slash};
			result.name      = {last_slash + 1, path.end()};
		} else {
			result.name = path;
		}
	}

	if (!result.directory.data) {
		result.directory.data = result.name.data;
	}

	return result;
}

inline Span<utf8> parent_directory(Span<utf8> path, bool remove_last_slash = false) {
	if (path.count == 0)
		return path;

	if (path.back() == '\\' || path.back() == '/') {
		path.count--;
	}

	while (path.count && !(path.back() == '\\' || path.back() == '/')) {
		path.count--;
	}

	if (path.count && remove_last_slash) {
		path.count--;
	}

	return path;
}

inline List<utf8> make_absolute_path(Span<utf8> path) {
	if (path.count >= 2) {
		if (path.data[1] == ':')
			return to_list(path);
	}
	scoped(temporary_storage_checkpoint);
	return (List<utf8>)concatenate(TL_TMP(get_current_directory()), u8'/', path);
}

inline ListOfLists<utf8> split_path(Span<utf8> path) {
	ListOfLists<utf8> result;

	split_by_any(path, Span{u8'\\', u8'/'}, [&](Span<utf8> part) {
		if (part.count) {
			result.add(part);
		}
	});

	result.enable_reading();

	return result;
}

// Returns original path on error
inline List<utf8> normalize_path(Span<utf8> path, utf8 separator = u8'/') {
	List<utf8> result;

	scoped(current_temporary_allocator);

	List<Span<utf8>> result_parts;

	auto parts = split_path(path);

	for (auto part : parts) {
		if (part == u8"."s) {
			continue;
		} else if (part == u8".."s) {
			if (!result_parts.count) {
				return to_list(path);
			}
			result_parts.pop();
		} else {
			result_parts.add(part);
		}
	}

	for (auto &part : result_parts) {
		if (&part != result_parts.begin()) {
			result.add(separator);
		}
		result.add(part);
	}

	return result;
}


template <class Size>
inline bool is_absolute_path(Span<utf8, Size> path) {
	if (path.count < 2)
		return false;
	return path.data[1] == ':';
}

inline bool create_directories(Span<utf8> path) {
	List<Span<utf8>> directories_to_create;
	directories_to_create.allocator = TL_GET_CURRENT(temporary_allocator);
	while (1) {
		if (directory_exists(path)) {
			break;
		}
		directories_to_create.add(path);
		path = parent_directory(path);
		if (path.count == 0)
			break;
	}
	for (auto dir : reversed(directories_to_create)) {
		if (!create_directory(dir))
			return false;
	}
	return true;
}

TL_API bool copy_file(Span<utf8> source, Span<utf8> destination);
template <AChar SChar, AChar DChar>
inline bool copy_file(Span<SChar> source, Span<DChar> destination) { scoped(temporary_allocator_and_checkpoint); return copy_file(to_utf8(source), to_utf8(destination)); }

TL_API List<utf8> get_executable_path(bool null_terminated = false TL_LP);

struct MoveFileParams {
	bool replace_existing = false;
	bool allow_copy = true;
};

TL_API bool move_file(Span<utf8> source, Span<utf8> destination, MoveFileParams params = {});
template <AChar SChar, AChar DChar>
inline bool move_file(Span<SChar> source, Span<DChar> destination, MoveFileParams params = {}) {
	scoped(temporary_allocator_and_checkpoint);
	return move_file(to_utf8(source), to_utf8(destination), params);
}

struct ForEachFileOptions {
	bool recursive = true;
	bool files = false;
	bool directories = false;
};

// NOTE: Almost redundant with FileItem.
// Only difference is full path instead of just name.
// But it makes it clearer.
struct ForEachFileItemValue {
	FileItemKind kind;
	Span<utf8> path;
};

template <ForEachIterator<ForEachFileItemValue> Fn>
inline bool for_each_file_item(Span<utf8> directory, ForEachFileOptions options, Fn &&process_item_) {
	auto outer_allocator = TL_GET_CURRENT(allocator);
	scoped(TL_GET_CURRENT(temporary_allocator));

	auto list = get_items_in_directory(directory);

	auto process_item = wrap_foreach_fn<ForEachFileItemValue>(process_item_);

	for (auto item : list) {
		auto item_path = format(u8"{}\\{}", directory, item.name);

		ForEachFileItemValue item_to_process = {
			.kind = item.kind,
			.path = item_path
		};

		switch (item.kind) {
			case FileItem_file: {
				if (options.files) {
					auto d = with(outer_allocator, process_item(item_to_process));

					switch (d & ForEach_erase_mask) {
						case ForEach_erase:
						case ForEach_erase_unordered:
							delete_file(item_path);
							break;
					}

					if (d & ForEach_break)
						return true;
				}
				break;
			}
			case FileItem_directory: {
				if (options.directories) {
					auto d = with(outer_allocator, process_item(item_to_process));

					switch (d & ForEach_erase_mask) {
						case ForEach_erase:
						case ForEach_erase_unordered:
							delete_directory(item_path, {.recursive = true});
							break;
					}

					if (d & ForEach_dont_recurse)
						options.recursive = false;

					if (d & ForEach_break)
						return true;
				}
				if (options.recursive) {
					bool broken = with(outer_allocator, for_each_file_item(item_path, options, process_item_));
					if (broken) {
						return true;
					}
				}
				break;
			}
		}
	}
	return false;
}

template <ForEachIterator<Span<utf8>> Fn>
inline bool for_each_file(Span<utf8> directory, ForEachFileOptions options, Fn &&process_item_) {
	options.files = true;
	options.directories = false;
	return for_each_file_item(directory, options, [&] (ForEachFileItemValue item) { return process_item_(item.path); });
}

}

#ifdef TL_IMPL
#if OS_WINDOWS

#if TL_FILE_INCLUDE_DIALOG
#pragma push_macro("OS_WINDOWS")
#undef OS_WINDOWS
#pragma warning(push, 0)
#include <Shlwapi.h>
#include <shlobj.h>
#pragma warning(pop)
#pragma pop_macro("OS_WINDOWS")

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "comctl32")
#pragma comment(lib, "Ole32.lib")

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

namespace tl {

struct WinOpenFileParams {
	DWORD access;
	DWORD share;
	DWORD creation;
};
WinOpenFileParams get_open_file_params(OpenFileParams params) {
	WinOpenFileParams result;
	if (params.read && params.write) {
		result.access = GENERIC_READ | GENERIC_WRITE;
		result.share = 0;
		result.creation = OPEN_ALWAYS;
	} else if (params.read) {
		result.access = GENERIC_READ;
		result.share = FILE_SHARE_READ;
		result.creation = OPEN_EXISTING;
	} else if (params.write) {
		result.access = GENERIC_WRITE;
		result.share = 0;
		result.creation = CREATE_ALWAYS;
	} else {
		result.access = 0;
		result.share = FILE_SHARE_READ | FILE_SHARE_WRITE;
		result.creation = OPEN_EXISTING;
	}
	return result;
}
File open_file(Span<utf8> path8, OpenFileParams params) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);

	auto win_params = get_open_file_params(params);
	if (params.create_directories) {
		create_directories(parse_path(path8).directory);
	}
	auto handle = CreateFileW((wchar_t *)path16.data, win_params.access, win_params.share, 0, win_params.creation, 0, 0);
	if (handle == INVALID_HANDLE_VALUE) {
		if (!params.silent) {
			TL_GET_GLOBAL(tl_logger).error("Could not open file \"{}\"", path8);
		}
		handle = 0;
	}
	return {handle};
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
	SetFilePointerEx(file.handle, newP, 0, moveMethod);
}
s64 get_cursor(File file) {
	LARGE_INTEGER curP;
	SetFilePointerEx(file.handle, {}, &curP, FILE_CURRENT);
	return curP.QuadPart;
}
umm read(File file, Span<u8> span) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_read = 0;
	umm total_bytes_read = 0;
	while (span.count > max_bytes) {
		if (!ReadFile(file.handle, span.data, max_bytes, &bytes_read, 0)) {
			return total_bytes_read;
		}
		span.data += max_bytes;
		span.count -= max_bytes;
		total_bytes_read += bytes_read;
		if (bytes_read != max_bytes) {
			return total_bytes_read;
		}
	}
	if (span.count) {
		if (!ReadFile(file.handle, span.data, (DWORD)span.count, &bytes_read, 0)) {
			return total_bytes_read;
		}
		total_bytes_read += bytes_read;
		if (bytes_read != span.count) {
			return total_bytes_read;
		}
	}
	return total_bytes_read;
}
umm write(File file, Span<u8> span) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_written = 0;
	umm total_bytes_written = 0;
	while (span.count > max_bytes) {
		if (!WriteFile(file.handle, span.data, max_bytes, &bytes_written, 0)) {
			return total_bytes_written;
		}
		span.data += max_bytes;
		span.count -= max_bytes;
		total_bytes_written += bytes_written;
		if (bytes_written != max_bytes) {
			return total_bytes_written;
		}
	}
	if (span.count) {
		if (!WriteFile(file.handle, span.data, (DWORD)span.count, &bytes_written, 0)) {
			return total_bytes_written;
		}
		total_bytes_written += bytes_written;
		if (bytes_written != span.count) {
			return total_bytes_written;
		}
	}
	return total_bytes_written;
}
void truncate_to_cursor(File file) {
	SetEndOfFile(file.handle);
}
void close(File file) {
	CloseHandle(file.handle);
}

bool file_exists(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	DWORD attr = GetFileAttributesW((wchar_t *)path16.data);
	return ((attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}
bool directory_exists(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	DWORD attr = GetFileAttributesW((wchar_t *)path16.data);
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
}

Optional<u64> get_file_write_time(File file) {
	FILETIME last_write_time;
	if (!GetFileTime(file.handle, 0, 0, &last_write_time))
		return {};

	return last_write_time.dwLowDateTime | ((u64)last_write_time.dwHighDateTime << 32);
}

// Appends /* to directory. Result is null terminated.
static List<wchar_t> make_wildcard(Span<utf8> directory8) {
	auto directory_wildcard = (List<wchar_t>)to_utf16(directory8, false);
	if (directory_wildcard.count) {
		if (directory_wildcard.back() != u'\\' && directory_wildcard.back() != u'/') {
			directory_wildcard.add(u'\\');
		}
	}
	directory_wildcard.add(u'*');
	directory_wildcard.add(u'\0');
	return directory_wildcard;
}

FileItemList get_items_in_directory(Span<utf8> directory8) {
	FileItemList result;

	// NOTE: no checkpoint because result.allocator may be temporary.
	scoped(current_temporary_allocator);

	auto directory_wildcard = make_wildcard(directory8);

	WIN32_FIND_DATAW find_data;
	HANDLE handle = FindFirstFileW(directory_wildcard.data, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}

		auto name = as_span((utf16 *)find_data.cFileName);

		FileItem item;
		item.name.count = name.count;
		item.name.data = (utf8 *)result.buffer.count;

		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			item.kind = FileItem_directory;
		} else {
			item.kind = FileItem_file;
		}

		result.add(item);
		result.buffer.add(TL_TMP(to_utf8(name)));

	} while (FindNextFileW(handle, &find_data));

	FindClose(handle);

	for (auto &item : result) {
		item.name.data = result.buffer.data + (umm)item.name.data;
	}

	return result;
}

void create_file(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	CloseHandle(CreateFileW((wchar_t *)path16.data, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
}

void delete_file(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	DeleteFileW((wchar_t *)path16.data);
}

bool create_directory(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	return (bool)CreateDirectoryW((wchar_t *)path16.data, 0);
}

void delete_directory(Span<utf8> path8, DeleteDirectoryOptions options) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	if (options.recursive) {
		for_each_file_item(path8, {.recursive = true, .files = true, .directories = true}, [&](ForEachFileItemValue item) {
			switch (item.kind) {
				case FileItem_file: 
					delete_file(item.path);
					return ForEach_continue;
				case FileItem_directory:
					delete_directory(item.path, {.recursive = true});
					return ForEach_dont_recurse;
				default:
					return ForEach_continue;

			}
		});
	}
	if (!RemoveDirectoryW((wchar_t *)path16.data)) {
		TL_GET_GLOBAL(tl_logger).error("Could not delete directory {}", path8);
	}
}

#if TL_FILE_INCLUDE_DIALOG
class CDialogEventHandler : public IFileDialogEvents, public IFileDialogControlEvents {
public:
	// IUnknown methods
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) {
		static const QITAB qit[] = {
			QITABENT(CDialogEventHandler, IFileDialogEvents),
			QITABENT(CDialogEventHandler, IFileDialogControlEvents), { 0 },
#pragma warning(suppress:4838)
		};
		return QISearch(this, qit, riid, ppv);
	}

	IFACEMETHODIMP_(ULONG) AddRef() {
		return InterlockedIncrement(&_cRef);
	}

	IFACEMETHODIMP_(ULONG) Release() {
		long cRef = InterlockedDecrement(&_cRef);
		if (!cRef)
			delete this;
		return cRef;
	}

	// IFileDialogEvents methods
	IFACEMETHODIMP OnFileOk(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnFolderChange(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnFolderChanging(IFileDialog *, IShellItem *) { return S_OK; };
	IFACEMETHODIMP OnHelp(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnSelectionChange(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnShareViolation(IFileDialog *, IShellItem *, FDE_SHAREVIOLATION_RESPONSE *) { return S_OK; };
	IFACEMETHODIMP OnTypeChange(IFileDialog *) { return S_OK; }
	IFACEMETHODIMP OnOverwrite(IFileDialog *, IShellItem *, FDE_OVERWRITE_RESPONSE *) { return S_OK; };

	// IFileDialogControlEvents methods
	IFACEMETHODIMP OnItemSelected(IFileDialogCustomize *, DWORD, DWORD) { return S_OK; }
	IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize *, DWORD) { return S_OK; };
	IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize *, DWORD, BOOL) { return S_OK; };
	IFACEMETHODIMP OnControlActivating(IFileDialogCustomize *, DWORD) { return S_OK; };

	CDialogEventHandler() : _cRef(1) { };
private:
	~CDialogEventHandler() { };
	long _cRef;
};

HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void **ppv) {
	*ppv = NULL;
	CDialogEventHandler *pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
	HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hr)) {
		hr = pDialogEventHandler->QueryInterface(riid, ppv);
		pDialogEventHandler->Release();
	}
	return hr;
}

Optional<ListOfLists<utf8>> open_file_dialog(FileDialogFlags flags, Span<Span<utf8>> allowed_extensions) {
	auto allocator = TL_GET_CURRENT(allocator);
	scoped(temporary_allocator_and_checkpoint);

	IFileOpenDialog *dialog = NULL;
	if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog)))) return {};
	defer { dialog->Release(); };


	IFileDialogEvents *events = NULL;
	if (FAILED(CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&events)))) return {};
	defer { events->Release(); };


	DWORD cookie;
	if (FAILED(dialog->Advise(events, &cookie))) return {};
	defer { dialog->Unadvise(cookie); };


	DWORD options;
	if (FAILED(dialog->GetOptions(&options))) return {};

	u32 kind = flags & 0x1;
	switch (kind) {
		case FileDialog_directory: options |= FOS_PICKFOLDERS; break;
	}
	if (flags & FileDialog_multiple) {
		options |= FOS_ALLOWMULTISELECT;
	}
	if (FAILED(dialog->SetOptions(options | FOS_FORCEFILESYSTEM))) return {};


	if (kind != FileDialog_directory) {
		COMDLG_FILTERSPEC file_type;
		if (allowed_extensions.count) {
			StringBuilder builder;
			for (auto &ext : allowed_extensions) {
				if (&ext == &allowed_extensions.back()) {
					append_format(builder, "*.{}", ext);
				} else {
					append_format(builder, "*.{};", ext);
				}
			}
			file_type = {L"Files", (wchar_t *)to_utf16((Span<utf8>)to_string(builder), true).data};
		} else {
			file_type = {L"Files", L"*.*"};
		}
		if (FAILED(dialog->SetFileTypes(1, &file_type))) return {};
	}


	if (FAILED(dialog->Show(NULL))) return {};


	ListOfLists<utf8> result;
	result.allocator = allocator;

	auto add_item = [&](IShellItem *item) {
		PWSTR path = NULL;
		if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) return;
		defer { CoTaskMemFree(path); };

		result.add(to_utf8(as_span((utf16 *)path)));
	};

	IShellItem *item;
	if (SUCCEEDED(dialog->GetResult(&item))) {
		defer { item->Release(); };

		add_item(item);
	} else {
		IShellItemArray *items;
		if (FAILED(dialog->GetResults(&items))) return {};
		defer { items->Release(); };


		IEnumShellItems *enumerator;
		if (SUCCEEDED(items->EnumItems(&enumerator))) {
			while (1) {
				ULONG items_fetched;
				if (FAILED(enumerator->Next(1, &item, &items_fetched))) break;
				if (items_fetched != 1) break;
				defer { item->Release(); };


				add_item(item);
			}
		}
	}

	result.enable_reading();
	return result;
}
#endif

List<utf8> get_current_directory() {
	List<utf16> temp;
	temp.allocator = TL_GET_CURRENT(temporary_allocator);
	temp.resize(GetCurrentDirectoryW(0, 0));
	GetCurrentDirectoryW((DWORD)temp.count, (wchar_t *)temp.data);
	temp.count--;
	return to_utf8(temp);
}

void set_current_directory(Span<utf8> path8) {
	scoped(temporary_allocator_and_checkpoint);
	auto path16 = to_utf16(path8, true);
	SetCurrentDirectoryW((wchar_t *)path16.data);
}

MappedFile map_file(File file) {

	MappedFile result = {};

	auto mapping = CreateFileMappingW(file.handle, 0, PAGE_READONLY, 0, 0, 0);

	void *data = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);

	result.data = {(u8 *)data, (umm)length(file)};
	result.mapping = mapping;

	return result;
}

void unmap_file(MappedFile &file) {
	// TODO: proper error handling
	assert(UnmapViewOfFile(file.data.data));
	assert(CloseHandle(file.mapping));

	file = {};
}

bool copy_file(Span<utf8> source8, Span<utf8> destination8) {
	scoped(temporary_allocator_and_checkpoint);
	auto source16 = to_utf16(source8, true);
	auto destination16 = to_utf16(destination8, true);
	return CopyFileW((wchar_t *)source16.data, (wchar_t *)destination16.data, false);
}

List<utf8> get_executable_path(bool null_terminated TL_LPD) {
	List<utf16> temp;
	temp.allocator = TL_GET_CURRENT(temporary_allocator);
	temp.reserve(512);
	temp.count = GetModuleFileNameW(0, (wchar_t *)temp.data, (DWORD)temp.capacity);
	return to_utf8(temp, null_terminated TL_LA);
}

bool move_file(Span<utf8> source8, Span<utf8> destination8, MoveFileParams params) {
	scoped(temporary_allocator_and_checkpoint);
	auto source16 = to_utf16(source8, true);
	auto destination16 = to_utf16(destination8, true);
	DWORD flags = 0;
	if (params.allow_copy)       flags |= MOVEFILE_COPY_ALLOWED;
	if (params.replace_existing) flags |= MOVEFILE_REPLACE_EXISTING;
	return MoveFileExW((wchar_t *)source16.data, (wchar_t *)destination16.data, flags);
}


}

#else

#include <sys/stat.h>
#include <dirent.h>

namespace tl {

File open_file(Span<utf8> path, OpenFileParams params) {
	scoped(temporary_allocator_and_checkpoint);

	assert(!params.create_directories, "not implemented");

	StaticList<char, 8> mode;

	if (params.read) mode.add('r');
	if (params.write) mode.add('w');
	mode.add('b');
	mode.add(0);

	FILE *file = fopen(null_terminate(path).data, mode.data);

	if (!file && !params.silent) {
		TL_GET_GLOBAL(tl_logger).error("Could not open file \"{}\"", path);
	}

	return {file};
}

int convert_origin(FileCursorOrigin origin) {
	switch (origin) {
		case File_begin: return SEEK_SET;
		case File_cursor: return SEEK_CUR;
		case File_end: return SEEK_END;
	}
	return -1;
}

void set_cursor(File file, s64 offset, FileCursorOrigin origin) {
	fseek((FILE *)file.handle, offset, convert_origin(origin));
}
s64 get_cursor(File file) {
	return ftell((FILE *)file.handle);
}
umm read(File file, Span<u8> span) {
	return fread(span.data, 1, span.count, (FILE *)file.handle);
}
umm write(File file, Span<u8> span) {
	return fwrite(span.data, 1, span.count, (FILE *)file.handle);
}
void truncate_to_cursor(File file) {
	ftruncate(fileno((FILE *)file.handle), get_cursor(file));
}
void close(File file) {
	fclose((FILE *)file.handle);
}


/*
implement this c++ function using c standatd library
```
FileItemList get_items_in_directory(Span<utf8> directory8);
```
*/

bool file_exists(Span<utf8> path) {
	scoped(temporary_allocator_and_checkpoint);
    FILE *file = fopen(null_terminate(path).data, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}
bool directory_exists(Span<utf8> path) {
	scoped(temporary_allocator_and_checkpoint);
    struct stat statbuf;

    if (stat(null_terminate(path).data, &statbuf) != 0) {
        return false;
    }

    return S_ISDIR(statbuf.st_mode);
}
Optional<u64> get_file_write_time(File file) {
	scoped(temporary_allocator_and_checkpoint);
	struct stat statbuf;
    if (fstat(fileno((FILE*)file.handle), &statbuf) != 0) {
		return {};
    }
	return (u64)statbuf.st_mtime;
}
FileItemList get_items_in_directory(Span<utf8> directory) {
	FileItemList result = {};
	
	// NOTE: no checkpoint because result.allocator may be temporary.
	scoped(temporary_allocator);

    DIR *dir = opendir(null_terminate(directory).data);
    if (dir == NULL) {
        current_logger.error("opendir({}) failed", directory);
		return result;
    }

    struct dirent *entry;
	int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        // Skip the current (.) and parent (..) directories
		defer { ++i; };
		if (i <= 1)
			continue;

		auto name = (Span<utf8>)as_span(entry->d_name);

		auto name_in_buffer_start = result.buffer.count;
		result.buffer.add(name);

		result.add(FileItem{
			.kind = entry->d_type == DT_REG ? FileItem_file : FileItem_directory,
			.name = {(utf8 *)name_in_buffer_start, name.count},
		});
    }

	for (auto &item : result) {
		item.name.data = result.buffer.data + (u64)item.name.data;
	}

    closedir(dir);
    return result;
}
void create_file(Span<utf8> path8);
void delete_file(Span<utf8> path8);
bool create_directory(Span<utf8> path8);
List<utf8> get_current_directory();
void set_current_directory(Span<utf8> path8);
MappedFile map_file(File file);
void unmap_file(MappedFile &file);
bool copy_file(Span<utf8> source8, Span<utf8> destination8);
List<utf8> get_executable_path(bool null_terminated TL_LPD);
bool move_file(Span<utf8> source8, Span<utf8> destination8, MoveFileParams params);

}

#endif

#endif
