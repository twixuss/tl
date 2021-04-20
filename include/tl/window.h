#pragma once

#include "common.h"

namespace TL {

TL_DECLARE_HANDLE(Window);

TL_API void set_title(Window window, Span<utf8> title);
TL_API void show(Window window);
TL_API void maximize(Window window);
TL_API void minimize(Window window);
TL_API void restore(Window window);

}

#ifdef TL_IMPL

namespace TL {

void set_title(Window window, Span<utf8> title) {
	SetWindowTextW((HWND)window, (wchar *)with(temporary_allocator, utf8_to_utf16(title, true)).data);
}

void show(Window window) {
	ShowWindow((HWND)window, SW_SHOW);
}
void maximize(Window window) {
	ShowWindow((HWND)window, SW_SHOWMAXIMIZED);
}
void minimize(Window window) {
	ShowWindow((HWND)window, SW_MINIMIZE);
}
void restore(Window window) {
	ShowWindow((HWND)window, SW_RESTORE);
}

}

#endif
