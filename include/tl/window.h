#pragma once

#include "common.h"
#include "vector.h"
#include "input.h"

namespace TL {

using WindowFlags = u32;
enum : WindowFlags {
	Window_open = 0x1,
};

using WindowStyleFlags = u32;
enum : WindowStyleFlags {
	WindowStyle_no_frame = 0x1,
};

struct Window;

using WindowOnSize = void (*)(Window &);
using WindowOnDraw = void (*)(Window &);
using WindowHitTest = u32 (*)(Window &);

enum WindowState {
	Window_normal,
	Window_minimized,
	Window_maximized,
};

#ifdef TL_WINDOW_EXTRA_STATE
struct Window : TL_WINDOW_EXTRA_STATE {
#else
struct Window {
#endif
	Allocator allocator;
	NativeWindowHandle handle = 0;
	v2u client_size = {};
	v2u min_client_size = {240, 180};
	v2s client_position = {};
	v2s window_position = {};
	v2s mouse_position = {};
	v2s mouse_delta = {};
	s32 mouse_wheel = 0;
	bool has_focus = false;
	bool active = false;
	WindowFlags flags = 0;
	WindowStyleFlags style_flags = 0;
	WindowOnSize on_size = 0;
	WindowOnDraw on_draw = 0;
	WindowHitTest hit_test = 0;
	WindowState state;
};

struct CreateWindowInfo {
	Span<utf8> title;
	v2u client_size = {};
	v2u min_client_size = {};
	WindowStyleFlags style_flags = 0;
	WindowOnSize on_size = 0;
	WindowOnDraw on_draw = 0;
	WindowHitTest hit_test = 0;
};

TL_API bool create_window(Window **_window, CreateWindowInfo info);
TL_API void free(Window *window);

TL_API void close(Window *window);

TL_API bool update(Window *window);

TL_API void set_title(Window *window, Span<utf8> title);
TL_API void show(Window *window);
TL_API void maximize(Window *window);
TL_API void minimize(Window *window);
TL_API void restore(Window *window);

extern TL_API KeyState key_state[256 + 3];

TL_DEFINE_KEYBOARD_INPUT(Span(::TL::key_state,       256))
TL_DEFINE_MOUSE_INPUT   (Span(::TL::key_state + 256,   3))

}

#ifdef TL_IMPL

#include "string.h"
#include "win32.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi")

namespace TL {

static bool window_class_created = false;
static constexpr auto class_name = L"tl_window";
static Window *currently_creating_window = 0;
KeyState key_state[256 + 3];

static void draw(Window &window) {
	window.mouse_position = get_cursor_position() - window.client_position;
	window.on_draw(window);
	update_key_state(key_state);
	window.mouse_delta = {};
	window.mouse_wheel = 0;
}

static DWORD get_window_style(WindowStyleFlags flags) {
	return WS_OVERLAPPEDWINDOW | WS_VISIBLE;
}

static LRESULT window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	Window *window_pointer;
	if (currently_creating_window) {
		window_pointer = currently_creating_window;
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)currently_creating_window);
		currently_creating_window = 0;
	} else {
		window_pointer = (Window *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
	}

	auto &window = *window_pointer;

	static bool changing_state = false;

	switch (message) {
		case WM_ACTIVATE: {
			//if (window.style_flags & WindowStyle_no_frame) {
			//	BOOL is_composition_enabled;
			//	DwmIsCompositionEnabled(&is_composition_enabled);
			//	if (is_composition_enabled) {
			//		MARGINS margins = {1,1,1,1};
			//		DwmExtendFrameIntoClientArea((HWND)window.handle, &margins);
			//	}
			//}
			window.active = wparam != WA_INACTIVE;
			return 0;
		}
		case WM_SETFOCUS: {
			window.has_focus = true;
			return 0;
		}
		case WM_KILLFOCUS: {
			window.has_focus = false;
			return 0;
		}
		case WM_MOVE: {
			window.client_position = {
				(s16)LOWORD(lparam),
				(s16)HIWORD(lparam)
			};
			window.window_position = get_window_position(window.client_position, get_window_style(window.style_flags));
			return 0;
		}
		case WM_TIMER: {
			draw(window);
			break;
		}
		case WM_ENTERSIZEMOVE: {
			SetTimer(hwnd, 0, 10, 0);
			break;
		}
		case WM_EXITSIZEMOVE: {
			KillTimer(hwnd, 0);
			break;
		}
		case WM_WINDOWPOSCHANGING: {
			return 0;
		}
		case WM_DESTROY: {
			close(&window);
			return 0;
		}
		case WM_SIZE: {
			v2u new_size = {
				LOWORD(lparam),
				HIWORD(lparam),
			};

			if (wparam == SIZE_MAXIMIZED) {
				window.state = Window_maximized;
			} else {
				window.state = Window_normal;
			}

			if (!new_size.x || !new_size.y || (wparam == SIZE_MINIMIZED))
				return 0;

			window.client_size = new_size;
			if (window.on_size)
				window.on_size(window);
			return 0;
		}
		case WM_NCCALCSIZE: {
			if (window.style_flags & WindowStyle_no_frame) {
				if (wparam) {
					auto &params = *(NCCALCSIZE_PARAMS *)lparam;

					auto proposed_window_rect = params.rgrc[0];
					auto previous_window_rect = params.rgrc[1];
					auto previous_client_rect = params.rgrc[2];

					auto &new_client_rect = params.rgrc[0];

					HMONITOR monitor = MonitorFromWindow((HWND)window.handle, MONITOR_DEFAULTTONEAREST);
					MONITORINFO info;
					info.cbSize = sizeof(info);
					GetMonitorInfoA(monitor, &info);

					auto mr = info.rcWork;

					auto cmp = proposed_window_rect;

					// for some reason, when maximized, window's client area is extened in all directions by 8 pixels
					// idk how to do this properly, but that's how i check it
					if ((cmp.left   == mr.left   - 8) &&
						(cmp.top    == mr.top    - 8) &&
						(cmp.right  == mr.right  + 8) &&
						(cmp.bottom == mr.bottom + 8))
					{
						new_client_rect = info.rcWork;
					}

				} else {
					auto &rect = *(RECT *)lparam;
				}
			} else {
				break;
			}
			return 0;
		}
		case WM_GETMINMAXINFO: {
			auto &i = *(MINMAXINFO *)lparam;

			i.ptMinTrackSize.x = window.min_client_size.x;
			i.ptMinTrackSize.y = window.min_client_size.y;
			return 0;
		}
		case WM_NCPAINT: {
			break;
		}
		case WM_NCHITTEST: {
			if (window.hit_test)
				return window.hit_test(window);
			break;
		}
	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}

bool create_window(Window **_window, CreateWindowInfo info) {
	*_window = 0;

	auto instance = GetModuleHandleA(0);

	if (!window_class_created) {
		window_class_created = true;
		WNDCLASSEXW c = {};
		c.cbSize = sizeof(c);
		c.hCursor = LoadCursor(0, IDC_ARROW);
		c.hInstance = instance;
		c.lpfnWndProc = window_proc;
		c.lpszClassName = class_name;
		c.hbrBackground = CreateSolidBrush(RGB(10,20,30));
		c.style = 0;
		if (!RegisterClassExW(&c)) {
			print("RegisterClassExW failed with error code: %\n", last_error());
			return false;
		}
	}

	DWORD window_style = get_window_style(info.style_flags);

	auto allocator = current_allocator;
	Window *window = ALLOCATE(Window, allocator);
	*_window = window;

	window->allocator = allocator;
	window->style_flags = info.style_flags;
	window->flags = Window_open;
	window->on_size = info.on_size;
	window->on_draw = info.on_draw;
	window->hit_test = info.hit_test;
	window->min_client_size = info.min_client_size;

	auto window_size = get_window_size(info.client_size, window_style);

	currently_creating_window = window;
	window->handle = (NativeWindowHandle)CreateWindowExW(
		0, class_name, with(temporary_allocator, (wchar *)utf8_to_utf16(info.title, true).data),
		window_style, CW_USEDEFAULT, CW_USEDEFAULT, window_size.x, window_size.y, 0, 0, GetModuleHandleA(0), 0
	);
	if (!window->handle) {
		*_window = 0;
		print("CreateWindowExW failed with error code: %\n", last_error());
		return false;
	}

	init_rawinput(RawInput_mouse);

	return true;
}
void close(Window *window) {
	window->flags &= ~Window_open;
	DestroyWindow((HWND)window->handle);
}
void free(Window *window) {
	close(window);
	FREE(window->allocator, window);
}

bool update(Window *window) {
	if (!(window->flags & Window_open)) {
		return false;
	}
	MSG m;
	while (PeekMessageA(&m, 0, 0, 0, PM_REMOVE)) {
		if (process_keyboard_message(m, Span(key_state, 256))) continue;
		if (process_mouse_message(m, Span(key_state + 256, 3), &window->mouse_delta)) continue;
		switch (m.message) {
			case WM_MOUSEWHEEL: window->mouse_wheel += GET_WHEEL_DELTA_WPARAM(m.wParam) / WHEEL_DELTA; continue;
		}
		TranslateMessage(&m);
		DispatchMessageA(&m);
		if (!(window->flags & Window_open)) {
			return false;
		}
	}
	if (key_down(Key_f4) && key_held(Key_alt)) {
		close(window);
		return false;
	}
	draw(*window);
	if (!window->has_focus) {
		for (KeyState &key : key_state) {
			if (key & KeyState_held) {
				key = KeyState_up;
			}
		}
	}
	return true;
}

void set_title(Window *window, Span<utf8> title) {
	SetWindowTextW((HWND)window->handle, (wchar *)with(temporary_allocator, utf8_to_utf16(title, true)).data);
}

void show(Window *window) {
	ShowWindow((HWND)window->handle, SW_SHOW);
}
void maximize(Window *window) {
	ShowWindow((HWND)window->handle, SW_SHOWMAXIMIZED);
}
void minimize(Window *window) {
	ShowWindow((HWND)window->handle, SW_MINIMIZE);
}
void restore(Window *window) {
	ShowWindow((HWND)window->handle, SW_RESTORE);
}

}

#endif
