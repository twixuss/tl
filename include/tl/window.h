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

#ifdef TL_WINDOW_EXTRA_STATE
struct Window : TL_WINDOW_EXTRA_STATE {
#else
struct Window {
#endif
	Allocator allocator;
	void *handle = 0;
	v2u client_size = {};
	v2u min_client_size = {240, 180};
	v2s client_position = {};
	v2s mouse_position = {};
	v2s mouse_delta = {};
	bool has_focus = false;
	WindowFlags flags = 0;
	WindowStyleFlags style_flags = 0;
	WindowOnSize on_size = 0;
	WindowOnDraw on_draw = 0;
};

struct CreateWindowInfo {
	Span<utf8> title;
	v2u client_size = {};
	WindowStyleFlags style_flags = 0;
	WindowOnSize on_size = 0;
	WindowOnDraw on_draw = 0;
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

extern TL_API KeyState key_state[256 + 3];

TL_DEFINE_KEYBOARD_INPUT(Span(::TL::key_state,       256))
TL_DEFINE_MOUSE_INPUT   (Span(::TL::key_state + 256,   3))

}

#ifdef TL_IMPL

#include "string.h"

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
	switch (message) {
		case WM_SETFOCUS: {
			window.has_focus = true;
			return 0;
		}
		case WM_KILLFOCUS: {
			window.has_focus = false;
			return 0;
		}
		case WM_MOVE: {
			window.client_position =  {
				(s16)LOWORD(lparam),
				(s16)HIWORD(lparam)
			};
			return 0;
		}
		case WM_TIMER: {
			draw(window);
			return 0;
		}
		case WM_ENTERSIZEMOVE: {
			SetTimer(hwnd, 0, 1, 0);
			return 0;
		}
		case WM_EXITSIZEMOVE: {
			KillTimer(hwnd, 0);
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
			if (new_size.x && new_size.y) {
				window.client_size = new_size;
				if (window.on_size)
					window.on_size(window);
			}
			return 0;
		}
		case WM_GETMINMAXINFO: {
			auto &i = *(MINMAXINFO *)lparam;

			i.ptMinTrackSize.x = window.min_client_size.x;
			i.ptMinTrackSize.y = window.min_client_size.y;
			return 0;
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
		c.hCursor = LoadCursorA(0, IDC_ARROW);
		c.hInstance = instance;
		c.lpfnWndProc = window_proc;
		c.lpszClassName = class_name;
		c.hbrBackground = CreateSolidBrush(RGB(10,20,30));
		c.style = 0;
		RegisterClassExW(&c);
	}

	DWORD window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	auto allocator = current_allocator;
	Window *window = ALLOCATE(Window, allocator);
	window->allocator = allocator;
	window->style_flags = info.style_flags;
	window->flags = Window_open;
	window->on_size = info.on_size;
	window->on_draw = info.on_draw;

	auto window_size = get_window_size(info.client_size, window_style);

	currently_creating_window = window;
	window->handle = CreateWindowExW(
		0, class_name, with(temporary_allocator, (wchar *)utf8_to_utf16(info.title, true).data),
		window_style, CW_USEDEFAULT, CW_USEDEFAULT, window_size.x, window_size.y, 0, 0, GetModuleHandleA(0), 0
	);

	init_rawinput(RawInput_mouse);

	return window;
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
	MSG m;
	while (PeekMessageA(&m, 0, 0, 0, PM_REMOVE)) {
		if (process_keyboard_message(m, Span(key_state, 256))) continue;
		if (process_mouse_message(m, Span(key_state + 256, 3), &window->mouse_delta)) continue;
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
