#pragma once
#include "list.h"

namespace tl {

List<utf8> get_clipboard();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "win32.h"

namespace tl {

List<utf8> get_clipboard() {
	if (!OpenClipboard(nullptr))
		return {};
	defer { CloseClipboard(); };

	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return {};

	auto data = GlobalLock(hData);
	if (data == nullptr)
		return {};
	defer { GlobalUnlock(hData); };

	auto len = strlen((char *)data);

	List<utf8> result;
	result.resize(len);

	memcpy(result.data, data, len);

	return result;
}

}

#endif

#endif
