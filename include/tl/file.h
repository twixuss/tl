#pragma once

#include "buffer.h"
#include "string.h"
#include "list_list.h"

namespace TL {

enum FileItemKind {
	FileItem_unknown,
	FileItem_file,
	FileItem_directory,
};

struct FileItem {
	List<utf8> name;
	FileItemKind kind;
};

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
	utf8 const *path;
	u64 last_write_time; // Represents the number of 100-nanosecond intervals since January 1, 1601 (UTC).

	Allocator allocator;
	void (*on_update)(FileTracker &tracker, void *state);
	void *state;
};

TL_API File open_file(ascii const *path, u32 open_flags);
TL_API File open_file(utf8 const *path, u32 open_flags);
TL_API File open_file(utf16 const *path, u32 open_flags);
TL_API File open_file(utf32 const *path, u32 open_flags);

inline File open_file(wchar const *path, u32 open_flags) {
	if constexpr(sizeof(wchar) == sizeof(utf16)) {
		return open_file((utf16 *)path, open_flags);
	} else {
		return open_file((utf32 *)path, open_flags);
	}
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


TL_API bool file_exists(ascii const *path);
TL_API bool file_exists(utf8  const *path);
TL_API bool file_exists(utf16 const *path);
TL_API bool file_exists(utf32 const *path);

inline bool file_exists(wchar const *path) {
	if constexpr(sizeof(wchar) == sizeof(utf16)) {
		return file_exists((utf16 *)path);
	} else {
		return file_exists((utf32 *)path);
	}
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
inline Buffer read_entire_file(ascii const *path, umm extra_space_before = 0, umm extra_space_after = 0) { File file = open_file(path, File_read); if (!file) return {}; defer { close(file); }; return read_entire_file(file, extra_space_before, extra_space_after); }
inline Buffer read_entire_file(utf8  const *path, umm extra_space_before = 0, umm extra_space_after = 0) { File file = open_file(path, File_read); if (!file) return {}; defer { close(file); }; return read_entire_file(file, extra_space_before, extra_space_after); }
inline Buffer read_entire_file(utf16 const *path, umm extra_space_before = 0, umm extra_space_after = 0) { File file = open_file(path, File_read); if (!file) return {}; defer { close(file); }; return read_entire_file(file, extra_space_before, extra_space_after); }
inline Buffer read_entire_file(utf32 const *path, umm extra_space_before = 0, umm extra_space_after = 0) { File file = open_file(path, File_read); if (!file) return {}; defer { close(file); }; return read_entire_file(file, extra_space_before, extra_space_after); }

inline Buffer read_entire_file(wchar const *path, umm extra_space_before = 0, umm extra_space_after = 0) {
	if constexpr(sizeof(wchar) == sizeof(utf16)) {
		return read_entire_file((wchar *)path, extra_space_before, extra_space_after);
	} else {
		return read_entire_file((utf32 *)path, extra_space_before, extra_space_after);
	}
}

inline Buffer read_entire_file(Span<ascii> path, umm extra_space_before = 0, umm extra_space_after = 0) { if (path.back() == '\0') { return read_entire_file(path.data, extra_space_before, extra_space_after); } else { auto null_terminated_path = null_terminate(path); defer { free(null_terminated_path); }; return read_entire_file(null_terminated_path.data, extra_space_before, extra_space_after); } }
inline Buffer read_entire_file(Span<utf8 > path, umm extra_space_before = 0, umm extra_space_after = 0) { if (path.back() == '\0') { return read_entire_file(path.data, extra_space_before, extra_space_after); } else { auto null_terminated_path = null_terminate(path); defer { free(null_terminated_path); }; return read_entire_file(null_terminated_path.data, extra_space_before, extra_space_after); } }
inline Buffer read_entire_file(Span<utf16> path, umm extra_space_before = 0, umm extra_space_after = 0) { if (path.back() == '\0') { return read_entire_file(path.data, extra_space_before, extra_space_after); } else { auto null_terminated_path = null_terminate(path); defer { free(null_terminated_path); }; return read_entire_file(null_terminated_path.data, extra_space_before, extra_space_after); } }
inline Buffer read_entire_file(Span<utf32> path, umm extra_space_before = 0, umm extra_space_after = 0) { if (path.back() == '\0') { return read_entire_file(path.data, extra_space_before, extra_space_after); } else { auto null_terminated_path = null_terminate(path); defer { free(null_terminated_path); }; return read_entire_file(null_terminated_path.data, extra_space_before, extra_space_after); } }

TL_API u64 get_file_write_time(ascii const *path);
TL_API u64 get_file_write_time(utf8 const *path);
TL_API u64 get_file_write_time(utf16 const *path);

TL_API ListList<utf8> get_files_in_directory(Span<ascii> directory);
TL_API ListList<utf8> get_files_in_directory(Span<utf8> directory);
TL_API ListList<utf8> get_files_in_directory(Span<utf16> directory);

TL_API List<FileItem> get_items_in_directory(Span<ascii> directory);
TL_API List<FileItem> get_items_in_directory(Span<utf8> directory);
TL_API List<FileItem> get_items_in_directory(Span<utf16> directory);

TL_API void delete_file(char const *path);

inline void delete_file(Span<char> path) {
	if (path.back() == '\0') {
		delete_file(path.data);
	} else {
		auto null_terminated_path = null_terminate(path);
		defer { free(null_terminated_path); };

		delete_file(null_terminated_path.data);
	}
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

inline FileTracker create_file_tracker(utf8 const *path, void (*on_update)(FileTracker &tracker, void *state), void *state) {
	FileTracker result = {};
	result.on_update = on_update;
	result.state = state;
	result.path = path;
	update_file_tracker(result);
	return result;
}
inline FileTracker create_file_tracker(utf8 const *path, void (*on_update)(FileTracker &tracker)) {
	return create_file_tracker(path, (void(*)(FileTracker &, void *))on_update, 0);
}
template <class Fn>
inline FileTracker create_file_tracker(utf8 const *path, Fn &&on_update) {
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

using FileDialogFlags = u32;
enum : FileDialogFlags {
	FileDialog_file      = 0x0,
	FileDialog_directory = 0x1,
	FileDialog_multiple  = 0x2,
};

TL_API ListList<utf8> open_file_dialog(FileDialogFlags flags);

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
OpenFileParams getOpenFileParams(u32 open_flags) {
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
File open_file(ascii const *path, u32 open_flags) {
	auto params = getOpenFileParams(open_flags);
	auto handle = CreateFileA(path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(handle);
		handle = 0;
	}
	if (handle == INVALID_HANDLE_VALUE)
		handle = 0;
	return (File)handle;
}
File open_file(utf16 const *path, u32 open_flags) {
	auto params = getOpenFileParams(open_flags);
	auto handle = CreateFileW((wchar *)path, params.access, params.share, 0, params.creation, 0, 0);
	if (!params.access) {
		CloseHandle(handle);
		handle = 0;
	}
	if (handle == INVALID_HANDLE_VALUE)
		handle = 0;
	return (File)handle;
}
File open_file(utf8 const *path, u32 open_flags) {
	auto utf16 = utf8_to_utf16(as_span(path), true);
	defer { free(utf16); };
	return open_file(utf16.data, open_flags);
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

bool file_exists(ascii const *path) { return PathFileExistsA(path); }
bool file_exists(utf16 const *path) { return PathFileExistsW((wchar *)path); }

static u64 get_file_write_time(HANDLE file) {
	FILETIME last_write_time;
	if (!GetFileTime(file, 0, 0, &last_write_time))
		return 0;

	return last_write_time.dwLowDateTime | ((u64)last_write_time.dwHighDateTime << 32);
}
u64 get_file_write_time(ascii const *path) {
	HANDLE file = CreateFileA(path, 0, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (file == INVALID_HANDLE_VALUE)
		return 0;

	defer { CloseHandle(file); };

	return get_file_write_time(file);
}
u64 get_file_write_time(utf16 const *path) {
	HANDLE file = CreateFileW((wchar *)path, 0, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (file == INVALID_HANDLE_VALUE)
		return 0;

	defer { CloseHandle(file); };

	return get_file_write_time(file);
}
u64 get_file_write_time(utf8 const *path) {
	auto utf16 = utf8_to_utf16(as_span(path), true);
	return get_file_write_time(utf16.data);
}

utf16 *append_star(Span<utf16> directory) {
	auto allocator = current_allocator;
	utf16 *directory_with_star;

	if (directory.back() == u'\0')
		directory.size--;

	if (directory.back() == u'\\' || directory.back() == u'/') {
		directory_with_star = ALLOCATE(utf16, allocator, directory.size + 2);
		memcpy(directory_with_star, directory.data, directory.size * sizeof(directory.data[0]));
		directory_with_star[directory.size + 0] = '*';
		directory_with_star[directory.size + 1] = 0;
	} else {
		directory_with_star = ALLOCATE(utf16, allocator, directory.size + 3);
		memcpy(directory_with_star, directory.data, directory.size * sizeof(directory.data[0]));
		directory_with_star[directory.size + 0] = '/';
		directory_with_star[directory.size + 1] = '*';
		directory_with_star[directory.size + 2] = 0;
	}
	return directory_with_star;
}

ListList<utf8> get_files_in_directory(Span<utf16> directory) {
	auto allocator = current_allocator;
	auto directory_with_star = append_star(directory);
	defer { FREE(allocator, directory_with_star); };

	WIN32_FIND_DATAW find_data;
	HANDLE handle = FindFirstFileW((wchar *)directory_with_star, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	ListList<utf8> result;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}
		auto utf8 = utf16_to_utf8(as_span((utf16 *)find_data.cFileName));
		defer { free(utf8); };

		result.add(utf8);
	} while (FindNextFileW(handle, &find_data));
	FindClose(handle);

	result.make_absolute();
	return result;
}
ListList<utf8> get_files_in_directory(Span<ascii> directory) {
	auto utf16 = ascii_to_utf16(directory, true);
	defer { free(utf16); };

	return get_files_in_directory(utf16);
}
ListList<utf8> get_files_in_directory(Span<utf8> directory) {
	auto utf16 = utf8_to_utf16(directory, true);
	defer { free(utf16); };

	return get_files_in_directory(utf16);
}




List<FileItem> get_items_in_directory(Span<utf16> directory) {
	auto allocator = current_allocator;
	auto directory_with_star = append_star(directory);
	defer { FREE(allocator, directory_with_star); };

	WIN32_FIND_DATAW find_data;
	HANDLE handle = FindFirstFileW((wchar *)directory_with_star, &find_data);
	if (handle == INVALID_HANDLE_VALUE) {
		return {};
	}

	u32 file_index = 0;
	List<FileItem> result;
	do {
		if (file_index++ < 2) {
			continue; // Skip . and ..
		}
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
			continue;
		}

		FileItem item;

		item.name = utf16_to_utf8(as_span((utf16 *)find_data.cFileName));

		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			item.kind = FileItem_directory;
		} else {
			item.kind = FileItem_file;
		}
		result.add(item);
	} while (FindNextFileW(handle, &find_data));
	FindClose(handle);

	return result;
}
List<FileItem> get_items_in_directory(Span<ascii> directory) {
	auto utf16 = ascii_to_utf16(directory);
	defer { free(utf16); };

	return get_items_in_directory(utf16);
}
List<FileItem> get_items_in_directory(Span<utf8> directory) {
	auto utf16 = utf8_to_utf16(directory);
	defer { free(utf16); };

	return get_items_in_directory(utf16);
}

void delete_file(char const *path) {
	DeleteFileA(path);
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

ListList<utf8> open_file_dialog(FileDialogFlags flags) {
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
		const COMDLG_FILTERSPEC file_types[] = {
			{L"Audio file", L"*.mp3;*.wav;*.ogg"},
		};
		if (FAILED(dialog->SetFileTypes(ARRAYSIZE(file_types), file_types))) return {};
	}


	if (FAILED(dialog->Show(NULL))) return {};


	ListList<utf8> result;

	auto add_item = [&](IShellItem *item) {
		PWSTR path = NULL;
		if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) return;
		defer { CoTaskMemFree(path); };

		auto utf8 = utf16_to_utf8(as_span((utf16 *)path));
		defer { free(utf8); };

		result.add(utf8);
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

#else
	XXX;
#endif

}

#endif
