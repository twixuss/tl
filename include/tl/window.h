#pragma once

#include "common.h"
#include "vector.h"
#include "input.h"

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

using WindowOnCreate = void (*)(Window &);
using WindowOnDraw = void (*)(Window &);
using WindowOnSize = void (*)(Window &);
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
	v2u min_client_size = {};
	v2u min_window_size = {};
	v2s client_position = {};
	v2s window_position = {};
	v2s mouse_position = {};
	v2s mouse_delta = {};
	s32 mouse_wheel = 0;
	bool has_focus = false;
	bool active = false;
	WindowFlags flags = 0;
	WindowStyleFlags style_flags = 0;
	WindowOnCreate on_create = 0;
	WindowOnDraw on_draw = 0;
	WindowOnSize on_size = 0;
	WindowHitTest hit_test = 0;
	WindowState state;
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

static LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
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
	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}

Window *create_window(CreateWindowInfo info) {
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
	window->min_client_size = info.min_client_size;
	window->min_window_size = get_window_size(info.min_client_size, window_style);
	window->client_size = max(info.client_size, info.min_client_size);

	auto window_size = get_window_size(info.client_size, window_style);

	currently_creating_window = window;
	CreateWindowExW(
		0, class_name, with(temporary_allocator, (wchar *)utf8_to_utf16(info.title, true).data),
		window_style, CW_USEDEFAULT, CW_USEDEFAULT, window_size.x, window_size.y, 0, 0, GetModuleHandleA(0), 0
	);
	if (!window->handle) {
		print("CreateWindowExW failed with error code: %\n", last_error());
		return 0;
	}

	init_rawinput(RawInput_mouse);

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
	while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
		switch (message.message) {
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN: {
				SetCapture(message.hwnd);
				break;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP: {
				ReleaseCapture();
				break;
			}
		}
		static bool alt_is_held;
		switch (message.message) {
			case WM_LBUTTONDOWN: { if (on_mouse_down) on_mouse_down(0); continue; }
			case WM_RBUTTONDOWN: { if (on_mouse_down) on_mouse_down(1); continue; }
			case WM_MBUTTONDOWN: { if (on_mouse_down) on_mouse_down(2); continue; }
			case WM_LBUTTONUP: { if (on_mouse_up) on_mouse_up(0); continue; }
			case WM_RBUTTONUP: { if (on_mouse_up) on_mouse_up(1); continue; }
			case WM_MBUTTONUP: { if (on_mouse_up) on_mouse_up(2); continue; }
			case WM_INPUT: {
				RAWINPUT rawInput;
				if (UINT rawInputSize = sizeof(rawInput);
					GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, &rawInput, &rawInputSize,
									sizeof(RAWINPUTHEADER)) == -1) {
					invalid_code_path("Error: GetRawInputData");
				}
				if (rawInput.header.dwType == RIM_TYPEMOUSE) {
					auto &mouse = rawInput.data.mouse;

					window->mouse_delta += {mouse.lLastX, mouse.lLastY};
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

				continue;
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
			case WM_UNICHAR: {
				if (on_char) on_char((u32)message.wParam);
				continue;
			}
			case WM_MOUSEWHEEL: window->mouse_wheel += GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA; continue;
		}
		TranslateMessage(&message);
		DispatchMessageA(&message);
		if (!(window->flags & Window_open)) {
			return false;
		}
	}
	draw(*window);
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
