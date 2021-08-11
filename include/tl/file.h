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

enum FileOpenFlags {
	File_read  = 0x1,
	File_write = 0x2,
};

enum FileCursorOrigin {
	File_begin,
	File_cursor,
	File_end,
};

struct File {
	void *handle;
};

inline bool valid(File file) {
	return file.handle != 0;
}

#if TL_DEBUG
#define tl_check_null_terminator(path) assert(string_unit_count(path.data) <= path.size)
#else
#define tl_check_null_terminator(path)
#endif

TL_API File open_file(pathchar const *path, u32 open_flags);
inline File open_file(Span<pathchar> path, u32 open_flags) {
	tl_check_null_terminator(path);
	return open_file(path.data, open_flags);
}
TL_API void close(File file);

TL_API bool read(File file, void *data, umm size);
template <class T>
inline bool read_bytes(File file, T &value) {
	return read(file, &value, sizeof(value));
}
TL_API bool write(File file, void const *data, umm size);
template <class T>
inline bool write_bytes(File file, T const &value) {
	return write(file, &value, sizeof(value));
}

TL_API void set_cursor(File file, s64 offset, FileCursorOrigin origin);
TL_API s64 get_cursor(File file);
TL_API void truncate_to_cursor(File file);


TL_API bool file_exists(Span<pathchar> path);
TL_API bool directory_exists(Span<pathchar> path);

inline Buffer read_entire_file(File file, umm extra_space_before = 0, umm extra_space_after= 0) {
	timed_function();

	auto oldCursor = get_cursor(file);
	defer { set_cursor(file, oldCursor, File_begin); };

	set_cursor(file, 0, File_end);
	auto size = (umm)get_cursor(file);
	set_cursor(file, 0, File_begin);

	auto result = create_buffer(size + extra_space_before + extra_space_after);
	read(file, result.data + extra_space_before, size);

	return result;
}
inline Buffer read_entire_file(pathchar const *path, umm extra_space_before = 0, umm extra_space_after = 0) {
	File file = open_file(path, File_read);
	if (!valid(file)) return {};
	defer { close(file); };
	return read_entire_file(file, extra_space_before, extra_space_after);
}
inline Buffer read_entire_file(Span<pathchar> path, umm extra_space_before = 0, umm extra_space_after = 0) {
	tl_check_null_terminator(path);
	return read_entire_file(path.data, extra_space_before, extra_space_after);
}

inline umm write_entire_file(File file, void const *data, umm size) {
	set_cursor(file, 0, File_begin);
	defer { truncate_to_cursor(file); };
	return write(file, data, size);
}
inline umm write_entire_file(pathchar const *path, void const *data, umm size) {
	File file = open_file(path, File_write);
	if (!valid(file)) return false;
	defer { close(file); };
	return write_entire_file(file, data, size);
}
inline umm write_entire_file(Span<pathchar> path, void const *data, umm size) {
	tl_check_null_terminator(path);
	return write_entire_file(path.data, data, size);
}
inline umm write_entire_file(File file, Span<u8> span) { return write_entire_file(file, span.data, span.size); }
inline umm write_entire_file(pathchar const *path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }
inline umm write_entire_file(Span<pathchar> path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }

 // Represents the number of 100-nanosecond intervals since January 1, 1601 (UTC).
TL_API u64 get_file_write_time(Span<pathchar> path);

TL_API ListList<pathchar> get_files_in_directory(Span<pathchar> directory);

TL_API FileItemList get_items_in_directory(Span<pathchar> directory);

TL_API void create_file(pathchar const *path);
inline void create_file(Span<pathchar> path) {
	tl_check_null_terminator(path);
	create_file(path.data);
}

TL_API void delete_file(pathchar const *path);
inline void delete_file(Span<pathchar> path) {
	tl_check_null_terminator(path);
	delete_file(path.data);
}

struct FileTracker {
	List<pathchar> path;
	u64 last_write_time;

	Allocator allocator;
	void (*on_update)(FileTracker &tracker, void *state);
	void *state;
};

inline FileTracker create_file_tracker_steal_path(List<pathchar> path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	tl_check_null_terminator(path);
	FileTracker result = {};
	result.on_update = on_update;
	result.state = state;
	result.path = path;
	return result;
}
inline FileTracker create_file_tracker_steal_path(List<pathchar> path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker_steal_path(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker_steal_path(List<pathchar> path, Fn &&on_update) {
	auto allocator = current_allocator;

	auto params = ALLOCATE_NOINIT(Fn, allocator);
	new(params) Fn(std::forward<Fn>(on_update));

	FileTracker result = create_file_tracker_steal_path(path, [](FileTracker &tracker, void *state) {
		Fn &fn = *(Fn *)state;
		fn(tracker);
	}, params);
	result.allocator = allocator;
	return result;
}
inline FileTracker create_file_tracker(Span<pathchar> path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	return create_file_tracker_steal_path((path.back() == 0) ? as_list(path) : null_terminate(path), on_update, state);
}
inline FileTracker create_file_tracker(Span<pathchar> path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker(Span<pathchar> path, Fn &&on_update) {
	return create_file_tracker_steal_path((path.back() == 0) ? as_list(path) : null_terminate(path), std::forward<Fn>(on_update));
}

inline bool update(FileTracker &tracker) {
	if (!tracker.path.data)
		return false;

	u64 last_write_time = get_file_write_time(tracker.path);
	if (last_write_time > tracker.last_write_time) {
		tracker.last_write_time = last_write_time;
		tracker.on_update(tracker, tracker.state);
		return true;
	}
	return false;
}

inline void free(FileTracker &tracker) {
	if (tracker.path.data) {
		if (tracker.allocator) {
			tracker.allocator.free(tracker.state);
		}
		free(tracker.path);
		tracker = {};
	}
}

template <class Fn>
inline void reset(FileTracker &tracker, Span<pathchar> path, Fn &&on_update) {
	if (tracker.path.data) {
		if (tracker.allocator) {
			tracker.allocator.free(tracker.state);
		}
	}
	tracker.path.set(path);
	tracker = create_file_tracker_steal_path(tracker.path, std::forward<Fn>(on_update));
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
forceinline bool read(File file, Span<u8> span) { return read(file, span.data, span.size); }
forceinline bool write(File file, Span<u8> span) { return write(file, span.data, span.size);}

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

TL_API bool create_directory(Span<pathchar> path);

TL_API Span<u8> map_file(File file);

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
struct OpenFileParams {
	DWORD access;
	DWORD share;
	DWORD creation;
};
OpenFileParams get_open_file_params(u32 open_flags) {
	OpenFileParams result;
	if ((open_flags & File_read) && (open_flags & File_write)) {
		result.access = GENERIC_READ | GENERIC_WRITE;
		result.share = 0;
		result.creation = OPEN_ALWAYS;
	} else if (open_flags & File_read) {
		result.access = GENERIC_READ;
		result.share = FILE_SHARE_READ;
		result.creation = OPEN_EXISTING;
	} else if (open_flags & File_write) {
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
File open_file(pathchar const *path, u32 open_flags) {
	auto params = get_open_file_params(open_flags);
	auto handle = CreateFileW(
		(wchar *)path,
		params.access, params.share, 0, params.creation, 0, 0
	);
	if (!params.access) {
		CloseHandle(handle);
		handle = 0;
	}
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
bool read(File file, void *data_, umm size) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_read = 0;
	u8 *data = (u8 *)data_;
	umm total_bytes_read = 0;
	umm remaining = size;
	while (remaining > max_bytes) {
		ReadFile(file.handle, data, max_bytes, &bytes_read, 0);
		data += max_bytes;
		remaining -= max_bytes;
		total_bytes_read += bytes_read;
	}
	if (remaining) {
		ReadFile(file.handle, data, (DWORD)remaining, &bytes_read, 0);
		total_bytes_read += bytes_read;
	}
	return total_bytes_read == size;
}
bool write(File file, void const *data_, umm size) {
	DWORD const max_bytes = (DWORD)~0;
	DWORD bytes_written = 0;
	u8 *data = (u8 *)data_;
	umm total_bytes_written = 0;
	umm remaining = size;
	while (remaining > max_bytes) {
		WriteFile(file.handle, data, max_bytes, &bytes_written, 0);
		data += max_bytes;
		remaining -= max_bytes;
		total_bytes_written += bytes_written;
	}
	if (remaining) {
		WriteFile(file.handle, data, (DWORD)remaining, &bytes_written, 0);
		total_bytes_written += bytes_written;
	}
	return total_bytes_written == size;
}
void truncate_to_cursor(File file) {
	SetEndOfFile(file.handle);
}
void close(File file) {
	CloseHandle(file.handle);
}

bool file_exists(Span<pathchar> path) {
	tl_check_null_terminator(path);
	return PathFileExistsW((wchar *)path.data);
}
bool directory_exists(Span<pathchar> path) {
	tl_check_null_terminator(path);
	return PathFileExistsW((wchar *)path.data);
}

static u64 get_file_write_time(HANDLE file) {
	FILETIME last_write_time;
	if (!GetFileTime(file, 0, 0, &last_write_time))
		return 0;

	return last_write_time.dwLowDateTime | ((u64)last_write_time.dwHighDateTime << 32);
}
u64 get_file_write_time(Span<pathchar> path) {
	tl_check_null_terminator(path);
	HANDLE file = CreateFileW(
		(wchar *)path.data,
		0, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0
	);
	if (file == INVALID_HANDLE_VALUE)
		return 0;

	defer { CloseHandle(file); };

	return get_file_write_time(file);
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

ListList<pathchar> get_files_in_directory(Span<pathchar> directory) {
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
	return temp;
}

bool create_directory(Span<pathchar> path) {
	tl_check_null_terminator(path);
	return (bool)CreateDirectoryW((wchar *)path.data, 0);
}

Span<u8> map_file(File file) {
	auto mapping = CreateFileMappingW(file.handle, 0, PAGE_READONLY, 0, 0, 0);
	void *data = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
	return {(u8 *)data, (umm)length(file)};
}

#else
	XXX;
#endif

}

#endif
