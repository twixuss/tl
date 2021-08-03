#pragma once

#include "common.h"
#include "vector.h"
#include "input.h"
#include "list.h"

namespace tl {

using WindowFlags = u32;
enum : WindowFlags {
	Window_open = 0x1,
};

using WindowStyleFlags = u32;
enum : WindowStyleFlags {
	WindowStyle_no_frame = 0x1,
};

struct Window;

enum Cursor {
	Cursor_none,
	Cursor_default,
	Cursor_horizontal,
	Cursor_vertical,
	Cursor_horizontal_and_vertical,
};

using WindowOnCreate = void (*)(Window &);
using WindowOnDraw = void (*)(Window &);
using WindowOnSize = void (*)(Window &);
using WindowHitTest = u32 (*)(Window &);
using WindowGetCursor = Cursor (*)(Window &);

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
	v2u min_client_size = {};
	v2u min_window_size = {};
	v2s client_position = {};
	v2s window_position = {};
	v2s mouse_position = {};
	v2f mouse_delta = {};
	f32 mouse_wheel = 0;
	bool has_focus = false;
	bool active = false;
	WindowFlags flags = 0;
	WindowStyleFlags style_flags = 0;
	WindowOnCreate on_create = 0;
	WindowOnDraw on_draw = 0;
	WindowOnSize on_size = 0;
	WindowHitTest hit_test = 0;
	WindowGetCursor get_cursor = 0;
	WindowState state;
	f32 mouse_sensitivity;
};

struct CreateWindowInfo {
	Span<utf8> title;
	v2u client_size = {};
	v2u min_client_size = {512, 512};
	WindowStyleFlags style_flags = 0;
	WindowOnCreate on_create = 0;
	WindowOnSize on_size = 0;
	WindowOnDraw on_draw = 0;
	WindowHitTest hit_test = 0;
	WindowGetCursor get_cursor = 0;
};

TL_API Window *create_window(CreateWindowInfo info);
TL_API void free(Window *window);

TL_API void close(Window *window);

TL_API bool update(Window *window);

TL_API void set_title(Window *window, Span<utf8> title);
TL_API void show(Window *window);
TL_API void maximize(Window *window);
TL_API void minimize(Window *window);
TL_API void restore(Window *window);

TL_API void set_cursor(Window &window, Cursor cursor);

enum ClipboardKind {
	Clipboard_text,
};

TL_API bool set_clipboard(Window *window, ClipboardKind kind, Span<u8> data);
template <class T>
inline bool set_clipboard(Window *window, ClipboardKind kind, Span<T> span) {
	return set_clipboard(window, kind, as_bytes(span));
}

TL_API List<u8> get_clipboard(Window *window, ClipboardKind kind);

TL_API void lock_cursor();
TL_API void unlock_cursor();

TL_API v2u client_size_to_window_size(Window &window, v2u client_size);

TL_API v2u get_client_size(NativeWindowHandle window);

}

#ifdef TL_IMPL

#include "string.h"
#include "win32.h"
#include "console.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi")

namespace tl {

static bool window_class_created = false;
static constexpr auto class_name = L"tl_window";
static Window *currently_creating_window = 0;

static void draw(Window &window) {
	window.mouse_position = get_cursor_position() - window.client_position;
	if (window.on_draw)
		window.on_draw(window);
	window.mouse_delta = {};
	window.mouse_wheel = 0;
}

static DWORD get_window_style(WindowStyleFlags flags) {
	return WS_OVERLAPPEDWINDOW | WS_VISIBLE;
}

static void update_mouse_speed(Window &window) {
	// speed index, ranges from 1 to 20
	int speed;
	SystemParametersInfo(SPI_GETMOUSESPEED, 0, &speed, 0);

	// all elements marked `unknown` are picked by eye because i can't test them
	// and i couldn't find any information about this mapping

	f32 map_speed[20] = {
		0.031250f,
		0.062500f,
		0.125000f, // unknown
		0.250000f,
		0.353553f, // 2^-1.5 unknown
		0.500000f,
		0.666666f, // unknown
		0.750000f,
		0.875000f, // unknown
		1.000000f,
		1.250000f, // unknown
		1.500000f,
		1.750000f, // unknown
		2.000000f,
		2.250000f, // unknown
		2.500000f,
		2.750000f, // unknown
		3.000000f,
		3.250000f, // unknown
		3.500000f,
	};

	window.mouse_sensitivity = map_speed[speed - 1];
}

static LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	Window *window_pointer;
	if (currently_creating_window) {
		window_pointer = currently_creating_window;
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)currently_creating_window);
		currently_creating_window = 0;
	} else {
		window_pointer = (Window *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	}

	auto &window = *window_pointer;

	static bool changing_state = false;

	switch (message) {
		case WM_SETCURSOR: {
			//if (window.get_cursor && all_true((v2u)window.mouse_position < window.client_size)) {
			//	set_cursor(window.get_cursor(window));
			//	return 0;
			//}
			break;
		}
		case WM_SETTINGCHANGE: {
			if (wparam == SPI_SETMOUSESPEED) {
				update_mouse_speed(window);
			}
			break;
		}
		case WM_CREATE: {
			window.handle = (NativeWindowHandle)hwnd;
			if (window.on_create) {
				window.on_create(window);
			}
			return 0;
		}
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
					GetMonitorInfoW(monitor, &info);

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

			i.ptMinTrackSize.x = window.min_window_size.x;
			i.ptMinTrackSize.y = window.min_window_size.y;
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
		case WM_CHAR: {
			if (on_char) on_char((u32)wparam);
			break;
		}
	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}

Window *create_window(CreateWindowInfo info) {
	auto instance = GetModuleHandleW(0);

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
			return 0;
		}
	}

	DWORD window_style = get_window_style(info.style_flags);

	auto allocator = current_allocator;
	Window *window = allocator.allocate<Window>();

	window->allocator = allocator;
	window->style_flags = info.style_flags;
	window->flags = Window_open;
	window->on_create = info.on_create;
	window->on_draw = info.on_draw;
	window->on_size = info.on_size;
	window->hit_test = info.hit_test;
	window->get_cursor = info.get_cursor;
	window->min_client_size = info.min_client_size;
	window->min_window_size = get_window_size(info.min_client_size, window_style);
	window->client_size = max(info.client_size, info.min_client_size);

	auto window_size = get_window_size(info.client_size, window_style);

	currently_creating_window = window;
	CreateWindowExW(
		0, class_name, with(temporary_allocator, (wchar *)to_utf16(info.title, true).data),
		window_style, CW_USEDEFAULT, CW_USEDEFAULT, window_size.x, window_size.y, 0, 0, GetModuleHandleW(0), 0
	);
	if (!window->handle) {
		print("CreateWindowExW failed with error code: %\n", last_error());
		return 0;
	}

	init_rawinput(RawInput_mouse);
	update_mouse_speed(*window);

	return window;
}
void close(Window *window) {
	window->flags &= ~Window_open;
	DestroyWindow((HWND)window->handle);
}
void free(Window *window) {
	close(window);
	window->allocator.free(window);
}

bool update(Window *window) {
	if (!(window->flags & Window_open)) {
		return false;
	}
	MSG message;
	while (PeekMessageW(&message, 0, 0, 0, PM_REMOVE)) {
		bool mouse_went_down = false;
		bool mouse_went_up   = false;
		defer {
			if (mouse_went_down) {
				SetCapture(message.hwnd);
			}
			if (mouse_went_up) {
				ReleaseCapture();
			}
		};
		static bool alt_is_held;
		switch (message.message) {
			case WM_LBUTTONDOWN: { mouse_went_down = true; if (on_mouse_down) on_mouse_down(0); continue; }
			case WM_RBUTTONDOWN: { mouse_went_down = true; if (on_mouse_down) on_mouse_down(1); continue; }
			case WM_MBUTTONDOWN: { mouse_went_down = true; if (on_mouse_down) on_mouse_down(2); continue; }
			case WM_INPUT: {
				RAWINPUT rawInput;
				if (UINT rawInputSize = sizeof(rawInput);
					GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, &rawInput, &rawInputSize,
									sizeof(RAWINPUTHEADER)) == -1) {
					invalid_code_path("Error: GetRawInputData");
				}
				if (rawInput.header.dwType == RIM_TYPEMOUSE) {
					auto &mouse = rawInput.data.mouse;

					window->mouse_delta += V2f(mouse.lLastX, mouse.lLastY) * window->mouse_sensitivity;

					if (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP) { mouse_went_up = true; if (on_mouse_up) on_mouse_up(0); continue; }
					if (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP) { mouse_went_up = true; if (on_mouse_up) on_mouse_up(1); continue; }
					if (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP) { mouse_went_up = true; if (on_mouse_up) on_mouse_up(2); continue; }
				}
				continue;
			}
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN: {
				u8 key = (u8)message.wParam;

				bool is_repeated = message.lParam & (LPARAM)(1 << 30);

				if (is_repeated) {
					if (on_key_repeat) on_key_repeat(key);
				} else {
					if (on_key_down) on_key_down(key);
					if (on_key_repeat) on_key_repeat(key);

					if (key == Key_alt) {
						alt_is_held = true;
					} else if (key == Key_f4) {
						if (alt_is_held) {
							close(window);
							return false;
						}
					}
				}

				break;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP: {
				u8 key = (u8)message.wParam;
				if (on_key_up) on_key_up(key);
				if (key == Key_alt) {
					alt_is_held = false;
				}
				continue;
			}
			case WM_MOUSEWHEEL: window->mouse_wheel += (f32)GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA; continue;
		}
		TranslateMessage(&message);
		DispatchMessageW(&message);
		if (!(window->flags & Window_open)) {
			return false;
		}
	}
	draw(*window);
	return true;
}

void set_title(Window *window, Span<utf8> title) {
	SetWindowTextW((HWND)window->handle, (wchar *)with(temporary_allocator, to_utf16(title, true)).data);
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

static HCURSOR get_cursor(Cursor cursor) {
	switch (cursor) {
		case Cursor_default:                   { static auto result = LoadCursor(0, IDC_ARROW);  return result; }
		case Cursor_horizontal:                { static auto result = LoadCursor(0, IDC_SIZEWE); return result; }
		case Cursor_vertical:                  { static auto result = LoadCursor(0, IDC_SIZENS); return result; }
		case Cursor_horizontal_and_vertical:   { static auto result = LoadCursor(0, IDC_SIZEALL); return result; }
	}
	return 0;
}

void set_cursor(Window &window, Cursor cursor) {
	if (cursor == Cursor_none) {
		while (ShowCursor(false) >= 0) {
		}
	} else {
		while (ShowCursor(true) < 0) {
		}
		SetClassLongPtrW((HWND)window.handle, GCLP_HCURSOR, (LONG_PTR)get_cursor(cursor));
	}
}

bool set_clipboard(Window *window, ClipboardKind kind, Span<u8> data) {
	assert(kind == Clipboard_text, "Not implemented");

    if (!OpenClipboard((HWND)window->handle)) 
        return false; 
	defer { CloseClipboard(); };

	EmptyClipboard();

    auto global_memory = GlobalAlloc(GMEM_MOVEABLE, data.size + 1); 
    if (!global_memory) { 
        return false; 
    } 
 
    auto global_memory_pointer = GlobalLock(global_memory); 
	if (!global_memory_pointer) 
		return false;
	defer { GlobalUnlock(global_memory); };

    memcpy(global_memory_pointer, data.data, data.size);
	((char *)global_memory_pointer)[data.size] = 0;
 
    SetClipboardData(CF_TEXT, global_memory); 
	return true;
}

List<u8> get_clipboard(Window *window, ClipboardKind kind) {
	assert(kind == Clipboard_text, "Not implemented");

    if (!IsClipboardFormatAvailable(CF_TEXT)) 
		return {}; 
    if (!OpenClipboard((HWND)window->handle)) 
        return {}; 
	defer { CloseClipboard(); };
 
    auto global_memory = GetClipboardData(CF_TEXT); 
    if (!global_memory)
		return {};

    auto global_memory_pointer = GlobalLock(global_memory); 
    if (!global_memory_pointer) 
		return {};
	defer { GlobalUnlock(global_memory); };

	umm size = string_byte_count((char *)global_memory_pointer);

	List<u8> result;
	result.resize(size);

	memcpy(result.data, global_memory_pointer, size);

	return result;
}

void lock_cursor() {
	POINT cursor_position;
	GetCursorPos(&cursor_position);
	RECT rect = {
		.left   = cursor_position.x,
		.top    = cursor_position.y,
		.right  = cursor_position.x + 1,
		.bottom = cursor_position.y + 1,
	};
	ClipCursor(&rect);
}
void unlock_cursor() {
	ClipCursor(0);
}

v2u client_size_to_window_size(Window &window, v2u client_size) {
	return get_window_size(client_size, get_window_style(window.style_flags));
}
v2u get_client_size(NativeWindowHandle window) {
	RECT rect;
	GetClientRect((HWND)window, &rect);
	return {
		(u32)(rect.right - rect.left),
		(u32)(rect.bottom - rect.top),
	};
}

}

#endif
