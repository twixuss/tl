#pragma once

#include "buffer.h"
#include "string.h"
#include "list_list.h"

namespace TL {

#if OS_WINDOWS
using filechar = utf16;
inline List<utf8> path_to_utf8(Span<filechar> span) {
	return utf16_to_utf8(span);
}
#define TL_FILE_STRING(x) u ## x
#endif

enum FileItemKind {
	FileItem_unknown,
	FileItem_file,
	FileItem_directory,
};

struct FileItem {
	FileItemKind kind;
	Span<filechar> name;
};
struct FileItemList : List<FileItem> {
	using Base = List<FileItem>;

	List<filechar> buffer;

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

TL_DECLARE_HANDLE(File);

TL_API File open_file(filechar const *path, u32 open_flags);
inline File open_file(Span<filechar> path, u32 open_flags) {
	assert(path.back() == 0);
	return open_file(path.data, open_flags);
}
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


TL_API bool file_exists(Span<filechar> path);

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
inline Buffer read_entire_file(Span<filechar> path, umm extra_space_before = 0, umm extra_space_after = 0) {
	File file = open_file(path, File_read);
	if (!file) return {};
	defer { close(file); };
	return read_entire_file(file, extra_space_before, extra_space_after);
}

inline bool write_entire_file(File file, void const *data, u64 size) {
	set_cursor(file, 0, File_begin);
	if (!write(file, data, size))
		return false;
	truncate_to_cursor(file);
	return true;
}
inline bool write_entire_file(filechar const *path, void const *data, u64 size) {
	File file = open_file(path, File_write);
	if (!file) return false;
	defer { close(file); };
	return write_entire_file(file, data, size);
}
inline bool write_entire_file(Span<filechar> path, void const *data, u64 size) {
	assert(path.back() == 0);
	return write_entire_file(path.data, data, size);
}
inline bool write_entire_file(File file, Span<u8> span) { return write_entire_file(file, span.data, span.size); }
inline bool write_entire_file(filechar const *path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }
inline bool write_entire_file(Span<filechar> path, Span<u8> span) { return write_entire_file(path, span.data, span.size); }

TL_API u64 get_file_write_time(Span<filechar> path);

TL_API ListList<filechar> get_files_in_directory(Span<filechar> directory);

TL_API FileItemList get_items_in_directory(Span<filechar> directory);

TL_API void delete_file(filechar const *path);
inline void delete_file(Span<filechar> path) {
	assert(path.back() == 0);
	delete_file(path.data);
}

struct FileTracker {
	List<filechar> path;
	u64 last_write_time; // Represents the number of 100-nanosecond intervals since January 1, 1601 (UTC).

	Allocator allocator;
	void (*on_update)(FileTracker &tracker, void *state);
	void *state;
};

inline FileTracker create_file_tracker_steal_path(List<filechar> path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	assert(path.back() == 0);
	FileTracker result = {};
	result.on_update = on_update;
	result.state = state;
	result.path = path;
	return result;
}
inline FileTracker create_file_tracker_steal_path(List<filechar> path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker_steal_path(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker_steal_path(List<filechar> path, Fn &&on_update) {
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
inline FileTracker create_file_tracker(Span<filechar> path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	return create_file_tracker_steal_path((path.back() == 0) ? as_list(path) : null_terminate(path), on_update, state);
}
inline FileTracker create_file_tracker(Span<filechar> path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker(Span<filechar> path, Fn &&on_update) {
	return create_file_tracker_steal_path((path.back() == 0) ? as_list(path) : null_terminate(path), std::forward<Fn>(on_update));
}

inline bool update_file_tracker(FileTracker &tracker) {
	u64 last_write_time = get_file_write_time(tracker.path);
	if (last_write_time > tracker.last_write_time) {
		tracker.last_write_time = last_write_time;
		tracker.on_update(tracker, tracker.state);
		return true;
	}
	return false;
}

inline void free(FileTracker &tracker) {
	if (tracker.allocator) {
		FREE(tracker.allocator, tracker.state);
	}
	free(tracker.path);
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

TL_API ListList<filechar> open_file_dialog(FileDialogFlags flags, Span<Span<utf8>> allowed_extensions = {});

TL_API List<filechar> get_current_directory();

}

#ifdef TL_IMPL

#pragma push_macro("OS_WINDOWS")
#pragma warning(push, 0)
#include <Shlwapi.h>
#include <shlobj.h>
#pragma warning(pop)
#pragma pop_macro("OS_WINDOWS")

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "comctl32")

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace TL {

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
File open_file(filechar const *path, u32 open_flags) {
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

bool file_exists(Span<filechar> path) {
	assert(path.back() == 0);
	return PathFileExistsW((wchar *)path.data);
}

static u64 get_file_write_time(HANDLE file) {
	FILETIME last_write_time;
	if (!GetFileTime(file, 0, 0, &last_write_time))
		return 0;

	return last_write_time.dwLowDateTime | ((u64)last_write_time.dwHighDateTime << 32);
}
u64 get_file_write_time(Span<filechar> path) {
	assert(path.back() == 0);
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
		directory_with_star = ALLOCATE(wchar, allocator, directory.size + 2);
		memcpy(directory_with_star, directory.data, directory.size * sizeof(directory.data[0]));
		directory_with_star[directory.size + 0] = '*';
		directory_with_star[directory.size + 1] = 0;
	} else {
		directory_with_star = ALLOCATE(wchar, allocator, directory.size + 3);
		memcpy(directory_with_star, directory.data, directory.size * sizeof(directory.data[0]));
		directory_with_star[directory.size + 0] = '/';
		directory_with_star[directory.size + 1] = '*';
		directory_with_star[directory.size + 2] = 0;
	}
	return directory_with_star;
}

ListList<filechar> get_files_in_directory(Span<filechar> directory) {
	auto directory_with_star = append_star(directory);

	WIN32_FIND_DATAW find_data;
	HANDLE handle = FindFirstFileW(directory_with_star, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	ListList<filechar> result;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}
		result.add(as_span((filechar *)find_data.cFileName));
	} while (FindNextFileW(handle, &find_data));
	FindClose(handle);

	result.make_absolute();
	return result;
}

FileItemList get_items_in_directory(Span<filechar> directory) {
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
		item.name.data = (filechar *)result.buffer.size;

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

void delete_file(filechar const *path) {
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

ListList<filechar> open_file_dialog(FileDialogFlags flags, Span<Span<utf8>> allowed_extensions) {
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


	ListList<filechar> result;

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

List<filechar> get_current_directory() {
	List<filechar> temp;
	temp.resize(GetCurrentDirectoryW(0, 0));
	GetCurrentDirectoryW(temp.size, (wchar *)temp.data);
	return temp;
}

#else
	XXX;
#endif

}

#endif
