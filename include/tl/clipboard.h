#pragma once
#include "list.h"

namespace tl {

// Result is empty if an error occurred
List<utf8> get_clipboard_text();

bool set_clipboard_text(Span<utf8> text);

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "win32.h"

namespace tl {

List<utf8> get_clipboard_text() {
	if (!OpenClipboard(nullptr))
		return {};
	defer { CloseClipboard(); };

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	if (hData == nullptr)
		return {};

	auto data = GlobalLock(hData);
	if (data == nullptr)
		return {};
	defer { GlobalUnlock(hData); };

	auto len = wcslen((wchar_t *)data);

	List<utf16> result16;
	defer { free(result16); };

	result16.resize(len);

	memcpy(result16.data, data, len * sizeof(utf16));

	return to_utf8(result16);
}

bool set_clipboard_text(Span<utf8> text) {
	if (!OpenClipboard(nullptr))
		return false;
	defer { CloseClipboard(); };

	auto text16 = to_utf16(text);
	defer { free(text16); };
	
	HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, (text16.count + 1) * sizeof(utf16));
	
	auto data = GlobalLock(hData);
	if (data == nullptr)
		return false;

	memcpy(data, text16.data, text16.count * sizeof(utf16));
	((utf16 *)data)[text16.count] = 0;

	GlobalUnlock(hData);
	
	EmptyClipboard();

	SetClipboardData(CF_UNICODETEXT, hData);

	return true;
}

}

#endif

#endif
