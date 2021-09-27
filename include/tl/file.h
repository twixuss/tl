#pragma once

#include "buffer.h"
#include "string.h"
#include "list_list.h"

namespace tl {

#if OS_WINDOWS
using pathchar = utf16;
inline List<pathchar> to_pathchars(Span<utf8> string, bool terminate = false) { return to_utf16(string, terminate); }
#define tl_file_string(x) u ## x

inline static constexpr pathchar path_separator = u'\\';

#endif

struct OpenFileParams {
	u8 read : 1;
	u8 write : 1;
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

TL_API File open_file(ascii const *path, OpenFileParams params);
TL_API File open_file(utf16 const *path, OpenFileParams params);

inline File open_file(Span<ascii> path, OpenFileParams params) { return open_file(temporary_null_terminate(path).data, params); }
inline File open_file(Span<utf16> path, OpenFileParams params) { return open_file(temporary_null_terminate(path).data, params); }
inline File open_file(Span<utf8> path, OpenFileParams params) { return open_file(with(temporary_allocator, to_utf16(path, true).data), params); }

TL_API void close(File file);

TL_API umm read(File file, Span<u8> data);
TL_API umm write(File file, Span<u8> data);

TL_API void set_cursor(File file, s64 offset, FileCursorOrigin origin);
TL_API s64 get_cursor(File file);
TL_API void truncate_to_cursor(File file);


TL_API bool file_exists(ascii const *path);
TL_API bool file_exists(utf16 const *path);

inline bool file_exists(Span<ascii> path) {
	return file_exists(temporary_null_terminate(path).data);
}
inline bool file_exists(Span<utf8> path) {
	return file_exists(to_utf16(path, true).data);
}
inline bool file_exists(Span<utf16> path) {
	return file_exists(temporary_null_terminate(path).data);
}

TL_API bool directory_exists(utf16 const *path);
inline bool directory_exists(Span<utf16> path) {
	return directory_exists(temporary_null_terminate(path).data);
}
inline bool directory_exists(Span<utf8> path) {
	return directory_exists(to_utf16(path, true).data);
}

struct ReadEntireFileParams {
	umm extra_space_before = 0;
	umm extra_space_after = 0;
};
inline Buffer read_entire_file(File file, ReadEntireFileParams params = {}) {
	auto old_cursor = get_cursor(file);
	defer { set_cursor(file, old_cursor, File_begin); };

	set_cursor(file, 0, File_end);
	auto size = (umm)get_cursor(file);
	set_cursor(file, 0, File_begin);

	auto result = create_buffer(size + params.extra_space_before + params.extra_space_after);
	read(file, {result.data + params.extra_space_before, size});

	return result;
}

template <class Char>
inline Buffer read_entire_file(Span<Char> path, ReadEntireFileParams params = {}) {
	File file = open_file(path, {.read = true});
	if (!is_valid(file)) return {};
	defer { close(file); };
	return read_entire_file(file, params);
}
template <class Path>
inline Buffer read_entire_file(Path path, ReadEntireFileParams params = {}) {
	return read_entire_file(as_span(path), params);
}


inline bool write_entire_file(File file, Span<u8> span) {
	set_cursor(file, 0, File_begin);
	defer { truncate_to_cursor(file); };
	return write(file, span);
}
inline bool write_entire_file(pathchar const *path, Span<u8> span) {
	File file = open_file(path, {.write = true});
	if (!is_valid(file)) return false;
	defer { close(file); };
	return write(file, span);
}
inline bool write_entire_file(Span<ascii> path, Span<u8> span) {
	File file = open_file(path, {.write = true});
	if (!is_valid(file)) return false;
	defer { close(file); };
	return write(file, span);
}
inline bool write_entire_file(Span<utf8> path, Span<u8> span) {
	File file = open_file(path, {.write = true});
	if (!is_valid(file)) return false;
	defer { close(file); };
	return write(file, span);
}
inline bool write_entire_file(Span<utf16> path, Span<u8> span) {
	File file = open_file(path, {.write = true});
	if (!is_valid(file)) return false;
	defer { close(file); };
	return write(file, span);
}


 // Represents the number of 100-nanosecond intervals since January 1, 1601 (UTC).
TL_API Optional<u64> get_file_write_time(File file);
inline Optional<u64> get_file_write_time(pathchar const *path) {
	auto file = open_file(path, {});
	if (!is_valid(file)) return {};
	defer { close(file); };
	return get_file_write_time(file);
}
inline Optional<u64> get_file_write_time(Span<pathchar> path) {
	return get_file_write_time(temporary_null_terminate(path).data);
}


TL_API ListList<pathchar> get_file_names_in_directory(Span<pathchar> directory);


enum FileItemKind {
	FileItem_unknown,
	FileItem_file,
	FileItem_directory,
};

struct FileItem {
	FileItemKind kind;
	Span<pathchar> name;
};
struct FileItemList : List<FileItem> {
	using Base = List<FileItem>;

	List<pathchar> buffer;

	Base &base() { return *this; }
};

inline void free(FileItemList &list) {
	free(list.buffer);
	free(list.base());
}

TL_API FileItemList get_items_in_directory(Span<pathchar> directory);

TL_API void create_file(pathchar const *path);
inline void create_file(Span<pathchar> path) {
	return create_file(temporary_null_terminate(path).data);
}

TL_API void delete_file(pathchar const *path);
inline void delete_file(Span<pathchar> path) {
	return delete_file(temporary_null_terminate(path).data);
}

TL_API bool create_directory(ascii const *path);
TL_API bool create_directory(utf16 const *path);
inline bool create_directory(Span<ascii> path) { return create_directory(temporary_null_terminate(path).data); }
inline bool create_directory(Span<utf8>  path) { return create_directory(to_utf16(path, true).data); }
inline bool create_directory(Span<utf16> path) { return create_directory(temporary_null_terminate(path).data); }

struct FileTracker {
	File file;
	u64 last_write_time;

	Allocator allocator;
	void (*on_update)(FileTracker &tracker, void *state);
	void *state;
};

inline FileTracker create_file_tracker(File file, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	FileTracker result = {};
	result.on_update = on_update;
	result.state = state;
	result.file = file;
	return result;
}
inline FileTracker create_file_tracker(File file, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(file, (void(*)(FileTracker &, void *))on_update, 0);
}
inline FileTracker create_file_tracker(Span<pathchar> path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	return create_file_tracker(open_file(path, {}), (void(*)(FileTracker &, void *))on_update, 0);
}
inline FileTracker create_file_tracker(Span<pathchar> path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker(Span<pathchar> path, Fn &&on_update) {
	auto allocator = current_allocator;

	auto params = ALLOCATE_NOINIT(Fn, allocator);
	new(params) Fn(std::forward<Fn>(on_update));

	FileTracker result = create_file_tracker(path, [](FileTracker &tracker, void *state) {
		Fn &fn = *(Fn *)state;
		fn(tracker);
	}, params);
	result.allocator = allocator;
	return result;
}

enum class FileTrackerUpdateState {
	none,
	needs_update,
	failure,
};

inline FileTrackerUpdateState update(FileTracker &tracker) {
	auto retrieved_time = get_file_write_time(tracker.file);
	if (!retrieved_time) {
		return FileTrackerUpdateState::failure;
	}

	u64 last_write_time = retrieved_time.get();
	if (last_write_time > tracker.last_write_time) {
		tracker.last_write_time = last_write_time;
		tracker.on_update(tracker, tracker.state);
		return FileTrackerUpdateState::needs_update;
	}
	return FileTrackerUpdateState::none;
}

inline void free(FileTracker &tracker) {
	if (is_valid(tracker.file)) {
		if (tracker.allocator) {
			tracker.allocator.free(tracker.state);
		}
		close(tracker.file);
		tracker = {};
	}
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

using FileDialogFlags = u32;
enum : FileDialogFlags {
	FileDialog_file      = 0x0,
	FileDialog_directory = 0x1,
	FileDialog_multiple  = 0x2,
};

TL_API ListList<pathchar> open_file_dialog(FileDialogFlags flags, Span<Span<utf8>> allowed_extensions = {});

TL_API List<pathchar> get_current_directory();
TL_API void set_current_directory(pathchar const *path);
inline void set_current_directory(Span<pathchar> path) {
	return set_current_directory(temporary_null_terminate(path).data);
}

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
};

inline ParsedPath parse_path(Span<utf8> path) {
	ParsedPath result = {};

	auto last_slash = find_last_any(path, u8"/\\"s);
	auto last_dot = find_last(path, u8'.');
	if (last_dot) {
		if (last_slash) {
			if (last_dot > last_slash) {
				result.directory = {path.data, last_slash};
				result.name      = {last_slash + 1, last_dot};
				result.extension = {last_dot + 1, path.end()};
				// Include the dot into the name because the extension is empty
				if (result.extension.size == 0) {
					result.name.size += 1;
				}
				if (result.name.size == 0) {
					result.name = result.extension;
					result.extension = {};
					result.name.data --;
					result.name.size ++;
				}
			} else {
				result.directory = {path.data, last_slash};
				result.name      = {last_slash + 1, path.end()};
			}

		} else {
			result.name      = {path.data, last_dot};
			result.extension = {last_dot + 1, path.end()};
			// Include the dot into the name because the extension is empty
			if (result.extension.size == 0) {
				result.name.size += 1;
			}
			if (result.name.size == 0) {
				result.name = result.extension;
				result.extension = {};
				result.name.data --;
				result.name.size ++;
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

	return result;
}

inline Span<utf8> parent_directory(Span<utf8> path, bool remove_last_slash = false) {
	if (path.size == 0)
		return path;

	if (path.back() == '\\' || path.back() == '/') {
		path.size--;
	}

	while (path.size && !(path.back() == '\\' || path.back() == '/')) {
		path.size--;
	}

	if (path.size && remove_last_slash) {
		path.size--;
	}

	return path;
}

inline List<utf8> make_absolute_path(Span<utf8> relative_path) {
	return concatenate(with(temporary_allocator, to_utf8(get_current_directory())), path_separator, relative_path);
}

inline bool is_absolute_path(Span<utf8> path) {
	if (path.size < 2)
		return false;
	return path.data[1] == ':';
}

inline bool create_directories(Span<utf8> path) {
	List<Span<utf8>> directories_to_create;
	directories_to_create.allocator = temporary_allocator;
	while (1) {
		if (directory_exists(path)) {
			break;
		}
		directories_to_create.add(path);
		path = parent_directory(path);
		if (path.size == 0)
			break;
	}
	for (auto dir : reverse(directories_to_create)) {
		if (!create_directory(dir))
			return false;
	}
	return true;
}

TL_API bool copy_file(pathchar const *source, pathchar const *destination);
inline bool copy_file(Span<utf8> source, Span<utf8> destination) {
	return copy_file(with(temporary_allocator, to_pathchars(source, true).data), with(temporary_allocator, to_pathchars(destination, true).data));
}

}

#ifdef TL_IMPL

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

namespace tl {

#if OS_WINDOWS
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
File open_file(ascii const *path, OpenFileParams params) {
	auto win_params = get_open_file_params(params);
	auto handle = CreateFileA(path, win_params.access, win_params.share, 0, win_params.creation, 0, 0);
	if (handle == INVALID_HANDLE_VALUE)
		handle = 0;
	return {handle};
}
File open_file(pathchar const *path, OpenFileParams params) {
	auto win_params = get_open_file_params(params);
	auto handle = CreateFileW((wchar *)path, win_params.access, win_params.share, 0, win_params.creation, 0, 0);
	if (handle == INVALID_HANDLE_VALUE)
		handle = 0;
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
	while (span.size > max_bytes) {
		if (!ReadFile(file.handle, span.data, max_bytes, &bytes_read, 0)) {
			return total_bytes_read;
		}
		span.data += max_bytes;
		span.size -= max_bytes;
		total_bytes_read += bytes_read;
		if (bytes_read != max_bytes) {
			return total_bytes_read;
		}
	}
	if (span.size) {
		if (!ReadFile(file.handle, span.data, (DWORD)span.size, &bytes_read, 0)) {
			return total_bytes_read;
		}
		total_bytes_read += bytes_read;
		if (bytes_read != span.size) {
			return total_bytes_read;
		}
	}
	return total_bytes_read;
}
umm write(File file, Span<u8> span) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_written = 0;
	umm total_bytes_written = 0;
	while (span.size > max_bytes) {
		if (!WriteFile(file.handle, span.data, max_bytes, &bytes_written, 0)) {
			return total_bytes_written;
		}
		span.data += max_bytes;
		span.size -= max_bytes;
		total_bytes_written += bytes_written;
		if (bytes_written != max_bytes) {
			return total_bytes_written;
		}
	}
	if (span.size) {
		if (!WriteFile(file.handle, span.data, (DWORD)span.size, &bytes_written, 0)) {
			return total_bytes_written;
		}
		total_bytes_written += bytes_written;
		if (bytes_written != span.size) {
			return total_bytes_written;
		}
	}
	return true;
}
void truncate_to_cursor(File file) {
	SetEndOfFile(file.handle);
}
void close(File file) {
	CloseHandle(file.handle);
}

bool file_exists(ascii const *path) {
	DWORD attr = GetFileAttributesA(path);
	return ((attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}
bool file_exists(utf16 const *path) {
	DWORD attr = GetFileAttributesW((wchar *)path);
	return ((attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}
bool directory_exists(pathchar const *path) {
	DWORD attr = GetFileAttributesW((wchar *)path);
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
}

Optional<u64> get_file_write_time(File file) {
	FILETIME last_write_time;
	if (!GetFileTime(file.handle, 0, 0, &last_write_time))
		return {};

	return last_write_time.dwLowDateTime | ((u64)last_write_time.dwHighDateTime << 32);
}

static wchar *append_star(Span<utf16> directory) {
	auto allocator = temporary_allocator;
	wchar *directory_with_star;

	if (directory.back() == u'\0')
		directory.size--;

	if (directory.back() == u'\\' || directory.back() == u'/') {
		directory_with_star = allocator.allocate<wchar>(directory.size + 2);
		memcpy(directory_with_star, directory.data, directory.size * sizeof(directory.data[0]));
		directory_with_star[directory.size + 0] = '*';
		directory_with_star[directory.size + 1] = 0;
	} else {
		directory_with_star = allocator.allocate<wchar>(directory.size + 3);
		memcpy(directory_with_star, directory.data, directory.size * sizeof(directory.data[0]));
		directory_with_star[directory.size + 0] = '/';
		directory_with_star[directory.size + 1] = '*';
		directory_with_star[directory.size + 2] = 0;
	}
	return directory_with_star;
}

ListList<pathchar> get_file_names_in_directory(Span<pathchar> directory) {
	auto directory_with_star = append_star(directory);

	WIN32_FIND_DATAW find_data;
	HANDLE handle = FindFirstFileW(directory_with_star, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	ListList<pathchar> result;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}
		result.add(as_span((pathchar *)find_data.cFileName));
	} while (FindNextFileW(handle, &find_data));
	FindClose(handle);

	result.make_absolute();
	return result;
}

FileItemList get_items_in_directory(Span<pathchar> directory) {
	auto directory_with_star = append_star(directory);

	WIN32_FIND_DATAW find_data;
	HANDLE handle = FindFirstFileW(directory_with_star, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	FileItemList result;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
			continue;
		}


		auto name = as_span((utf16 *)find_data.cFileName);

		if (name.size > 200)
			debug_break();

		FileItem item;
		item.name.size = name.size;
		item.name.data = (pathchar *)result.buffer.size;

		if (item.name.size > 200)
			debug_break();

		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			item.kind = FileItem_directory;
		} else {
			item.kind = FileItem_file;
		}

		result.add(item);
		result.buffer.add(name);

	} while (FindNextFileW(handle, &find_data));
	FindClose(handle);

	for (auto &item : result) {
		item.name.data = result.buffer.data + (umm)item.name.data;
	}

	for (auto &item : result) {
		if (item.name.size > 200)
			debug_break();
	}
	return result;
}

void create_file(pathchar const *path) {
	CloseHandle(CreateFileW((wchar *)path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
}

void delete_file(pathchar const *path) {
	DeleteFileW((wchar *)path);
}

bool create_directory(ascii const *path) {
	return (bool)CreateDirectoryA(path, 0);
}
bool create_directory(utf16 const *path) {
	return (bool)CreateDirectoryW((wchar *)path, 0);
}

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
	IFACEMETHODIMP OnTypeChange(IFileDialog *pfd) { return S_OK; }
	IFACEMETHODIMP OnOverwrite(IFileDialog *, IShellItem *, FDE_OVERWRITE_RESPONSE *) { return S_OK; };

	// IFileDialogControlEvents methods
	IFACEMETHODIMP OnItemSelected(IFileDialogCustomize *pfdc, DWORD dwIDCtl, DWORD dwIDItem) { return S_OK; }
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

ListList<pathchar> open_file_dialog(FileDialogFlags flags, Span<Span<utf8>> allowed_extensions) {
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
		if (allowed_extensions.size) {
			scoped_allocator(temporary_allocator);
			StringBuilder builder;
			builder.encoding = Encoding_utf16;
			for (auto &ext : allowed_extensions) {
				if (&ext == &allowed_extensions.back()) {
					append_format(builder, u"*.%", ext);
				} else {
					append_format(builder, u"*.%;", ext);
				}
			}
			append(builder, u'\0');
			file_type = {L"Files", (wchar *)to_string(builder).data};
		} else {
			file_type = {L"Files", L"*.*"};
		}
		if (FAILED(dialog->SetFileTypes(1, &file_type))) return {};
	}


	if (FAILED(dialog->Show(NULL))) return {};


	ListList<pathchar> result;

	auto add_item = [&](IShellItem *item) {
		PWSTR path = NULL;
		if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) return;
		defer { CoTaskMemFree(path); };

		auto span = as_span((utf16 *)path);
		++span.size;
		result.add(span);
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

	result.make_absolute();
	return result;
}

List<pathchar> get_current_directory() {
	List<pathchar> temp;
	temp.resize(GetCurrentDirectoryW(0, 0));
	GetCurrentDirectoryW((DWORD)temp.size, (wchar *)temp.data);
	temp.size--;
	return temp;
}

void set_current_directory(pathchar const *path) {
	SetCurrentDirectoryW((wchar *)path);
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

bool copy_file(pathchar const *source, pathchar const *destination) {
	return CopyFileW((wchar *)source, (wchar *)destination, false);
}

#else
	XXX;
#endif

}

#endif
