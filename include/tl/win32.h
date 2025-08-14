#pragma once
#include "system.h"
#include "math.h"
#include "common.h"
#include "vector.h"
#include "logger.h"
#include "static_list.h"
#define NOMINMAX
#pragma warning(push, 0)
#include <Windows.h>
#include <Psapi.h>
#pragma warning(pop, 0)
#pragma comment(lib, "user32.lib")

namespace tl {

//
// Utility
//

template <class Allocator = Allocator>
inline wchar_t *to_wstr(Span<utf8> str) {
	return (wchar_t *)to_utf16<Allocator>(str, true).data;
}

inline wchar_t *tmp_to_wstr(Span<utf8> str) {
	return (wchar_t *)to_utf16<TemporaryAllocator>(str, true).data;
}


// 
// Debug
//

struct FormattedHRESULT {
	HRESULT value;
};
TL_API void append(StringBuilder &b, FormattedHRESULT e);


struct FormattedWin32Error {
	DWORD value;
};

TL_API FormattedWin32Error win32_error();
TL_API void append(StringBuilder &b, FormattedWin32Error e);

//
// Window
//

TL_API bool register_window_class(HINSTANCE instance, char const *name, UINT style, HCURSOR cursor, WNDPROC wnd_proc);

struct CreateClassAndWindowOptions {
	u32 style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	v2s position = {CW_USEDEFAULT, CW_USEDEFAULT};
	v2s size = {CW_USEDEFAULT, CW_USEDEFAULT};
};

TL_API HWND create_class_and_window(Span<utf8> class_name, WNDPROC wnd_proc, Span<utf8> title, CreateClassAndWindowOptions options = {});
TL_API void clamp_window_to_monitor(HWND window, bool move, HMONITOR monitor = (HMONITOR)INVALID_HANDLE_VALUE);
TL_API v2u get_window_size(v2u client_size, DWORD style, bool menu = false);
TL_API v2s get_window_position(v2s client_position, DWORD style, bool menu = false);
TL_API void hide_cursor();
TL_API void show_cursor();
TL_API LRESULT get_border_hit(s32 x, s32 y, s32 size_x, s32 size_y, s32 border_width, LRESULT center_hit);
inline void set_cursor_visibility(bool visible) {
	if (visible) show_cursor();
	else hide_cursor();
}
TL_API void set_cursor_position(s32 x, s32 y);
inline void set_cursor_position(v2s position) {
	set_cursor_position(position.x, position.y);
}
TL_API v2s get_cursor_position(HWND relative_to = 0);

TL_API bool set_fullscreen(HWND window, bool enable, DWORD window_style, WINDOWPLACEMENT &placement);
TL_API f32 get_cursor_speed();

TL_API f32 get_monitor_refresh_time(HMONITOR hm);
TL_API f32 get_monitor_refresh_time(HWND hw);

//
// Raw Input
//

enum RawInputDevice {
	RawInput_mouse = 0x1,
};

TL_API bool init_rawinput(RawInputDevice device_flags);
TL_API bool process_raw_input_message(MSG msg, bool mouse_buttons[5], s32 *mouse_wheel, v2s *mouse_delta);
TL_API bool process_keyboard_message(MSG message, bool keyboard_buttons[256], bool handle_repeated);


//
// Time
//

TL_API extern u64 performance_frequency;
TL_API u64 get_performance_counter();

}

#ifdef TL_IMPL

#include "win32_error.h"

namespace tl {

static List<utf8> describe_hresult(HRESULT hr) {
	LPWSTR error_text = NULL;

	FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&error_text,
		0,
		NULL);

	if (!error_text)
		return {};

	defer { LocalFree(error_text); };

	return to_utf8(as_span((utf16 *)error_text), true);
}

void append(StringBuilder &b, FormattedHRESULT e) {
	auto description = describe_hresult(e.value);
	defer { free(description); };
	append_format(b, "{} 0x{} {}", win32_error_name(e.value), FormatInt{.value = (u32)e.value, .radix = 16, .leading_zero_count = 8}, description);
}

FormattedWin32Error win32_error() {
	return {GetLastError()};
}

void append(StringBuilder &b, FormattedWin32Error e) {
	append(b, FormattedHRESULT{HRESULT_FROM_WIN32(e.value)});
}

HWND create_class_and_window(Span<utf8> class_name, WNDPROC wnd_proc, Span<utf8> title, CreateClassAndWindowOptions options) {
	scoped(temporary_allocator_and_checkpoint);

	WNDCLASSEXW c {
		.cbSize = sizeof c,
		.lpfnWndProc = wnd_proc,
		.hInstance = GetModuleHandle(0),
		.hCursor = LoadCursor(0, IDC_ARROW),
		.lpszClassName = to_wstr(class_name),
	};

	if (!RegisterClassExW(&c)) {
		current_logger.error("RegisterClassExW failed: {}", win32_error());
		return 0;
	}

	HWND hwnd = CreateWindowExW(0, c.lpszClassName, to_wstr(title), options.style, options.position.x, options.position.y, options.size.x, options.size.y, 0, 0, c.hInstance, 0);

	if (!hwnd) {
		current_logger.error("CreateWindowEx failed: {}", win32_error());
		return 0;
	}

	return hwnd;
}
bool init_rawinput(RawInputDevice device_flags) {
	StaticList<RAWINPUTDEVICE, 1> devices;
	if (device_flags & RawInput_mouse) {
		RAWINPUTDEVICE mouse = {};
		mouse.usUsagePage = 0x01;
		mouse.usUsage = 0x02;
		devices.add(mouse);
	}
	if (devices.count) {
		if (!RegisterRawInputDevices(devices.data, (UINT)devices.count, sizeof(devices[0]))) {
			return false;
		}
	}
	return true;
}
bool process_raw_input_message(MSG msg, bool mouse_buttons[5], s32 *mouse_wheel, v2s *mouse_delta) {
	if (msg.message == WM_INPUT) {
		RAWINPUT raw_input;
		UINT raw_input_size = sizeof(raw_input);
		if (GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, &raw_input, &raw_input_size, sizeof(RAWINPUTHEADER)) == -1) {
			current_logger.error("GetRawInputData failed: {}", win32_error());
		}
		if (raw_input.header.dwType == RIM_TYPEMOUSE) {
			auto &mouse = raw_input.data.mouse;
			if (mouse_delta)
				*mouse_delta += {mouse.lLastX, mouse.lLastY};
			if (mouse_wheel)
				if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
					*mouse_wheel += (s16)mouse.usButtonData / WHEEL_DELTA;
			if (mouse_buttons) {
				u8 key = (u8)~0;
				bool value = false;
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN)	{ key = 0; value = true;  }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP)		{ key = 0; value = false; }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN)	{ key = 1; value = true;  }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP)		{ key = 1; value = false; }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN)	{ key = 2; value = true;  }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP)		{ key = 2; value = false; }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)	{ key = 3; value = true;  }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)		{ key = 3; value = false; }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)	{ key = 4; value = true;  }
				if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)		{ key = 4; value = false; }
				if (key != (u8)~0) {
					mouse_buttons[key] = value;
				}
			}
		}
		return true;
	}
	return false;
}
bool process_keyboard_message(MSG message, bool keyboard_buttons[256], bool handle_repeated) {
	switch (message.message) {
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN: {
			auto code = (u8)message.wParam;
			bool extended    = message.lParam & (LPARAM(1) << 24);
			bool alt         = message.lParam & (LPARAM(1) << 29);
			bool is_repeated = message.lParam & (LPARAM(1) << 30);
			bool went_up     = message.lParam & (LPARAM(1) << 31);
			if (handle_repeated || is_repeated == went_up) { // Don't handle repeated
				keyboard_buttons[code] = !went_up;
			}
			(void)extended;
			(void)alt;
			return true;
		}
	}
	return false;
}

u64 performance_frequency = [] {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}();
u64 get_performance_counter() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

void clamp_window_to_monitor(HWND window, bool move, HMONITOR monitor) {
    RECT Rect = {};
    GetWindowRect(window, &Rect);
    if (monitor == INVALID_HANDLE_VALUE) {
		monitor = MonitorFromRect(&Rect, MONITOR_DEFAULTTONEAREST);
	}
    MONITORINFO monitor_info = {};
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfoA(monitor, &monitor_info);

	auto monitor_x = monitor_info.rcWork.left;
	auto monitor_y = monitor_info.rcWork.top;
	auto monitor_width = monitor_info.rcWork.right - monitor_info.rcWork.left;
	auto monitor_height = monitor_info.rcWork.bottom - monitor_info.rcWork.top;

    int x = Rect.left - monitor_info.rcWork.left;
    int y = Rect.top - monitor_info.rcWork.top;
    int width = Rect.right - Rect.left;
    int height = Rect.bottom - Rect.top;

    x = max(x, 0);
    y = max(y, 0);

	if (move) {
		if (width  > monitor_width ) width  = monitor_width;
		if (height > monitor_height) height = monitor_height;
	} else {
		if (x + width  > monitor_width ) width  = monitor_width  - x;
		if (y + height > monitor_height) height = monitor_height - y;
	}

    if (x > monitor_width  - width ) x = monitor_width  - width;
    if (y > monitor_height - height) y = monitor_height - height;

    MoveWindow(window, x + monitor_x, y + monitor_y, width, height, 0);
}
LRESULT get_border_hit(s32 x, s32 y, s32 size_x, s32 size_y, s32 border_width, LRESULT center_hit) {
	if (x < border_width) {
		if (y < border_width) {
			return HTTOPLEFT;
		} else if (y >= size_y - border_width) {
			return HTBOTTOMLEFT;
		} else {
			return HTLEFT;
		}
	} else if (x >= size_x - border_width) {
		if (y < border_width) {
			return HTTOPRIGHT;
		} else if (y >= size_y - border_width) {
			return HTBOTTOMRIGHT;
		} else {
			return HTRIGHT;
		}
	} else {
		if (y < border_width) {
			return HTTOP;
		} else if (y >= size_y - border_width) {
			return HTBOTTOM;
		}
	}
	return center_hit;
}
bool register_window_class(HINSTANCE instance, char const *name, UINT style, HCURSOR cursor, WNDPROC wndProc) {
	WNDCLASSEXA c = {};
	c.cbSize = sizeof(c);
	c.hCursor = cursor;
	c.hInstance = instance;
	c.lpfnWndProc = wndProc;
	c.lpszClassName = name;
	c.style = style;
	return RegisterClassExA(&c) != 0;
}
v2u get_window_size(v2u client_size, DWORD style, bool menu) {
	RECT r = {0, 0, (LONG)client_size.x, (LONG)client_size.y};
	AdjustWindowRect(&r, style, menu);
	return {
		(u32)(r.right - r.left),
		(u32)(r.bottom - r.top)
	};
}
v2s get_window_position(v2s client_position, DWORD style, bool menu) {
	RECT r = {client_position.x, client_position.y, 0, 0};
	AdjustWindowRect(&r, style, menu);
	return {
		(s32)(r.left),
		(s32)(r.top)
	};
}
void hide_cursor() {
	while (ShowCursor(false) >= 0) {}
}
void show_cursor() {
	while (ShowCursor(true) < 0) {}
}
void set_cursor_position(s32 x, s32 y) {
	SetCursorPos(x, y);
}
v2s get_cursor_position(HWND relative_to) {
	POINT p;
	GetCursorPos(&p);
	if (relative_to) {
		ScreenToClient(relative_to, &p);
	}
	return {p.x, p.y};
}

bool set_fullscreen(HWND window, bool fullscreen, DWORD window_style, WINDOWPLACEMENT &placement) {
	if (fullscreen) {
		HMONITOR hmon = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (!GetMonitorInfoA(hmon, &mi) || !GetWindowPlacement(window, &placement))
			return false;

		SetWindowLongA(window, GWL_STYLE, window_style & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		return true;
	} else {
        SetWindowLongA(window, GWL_STYLE, window_style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(window, &placement);
		SetWindowPos(window, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		return false;
	}

}

f32 get_cursor_speed() {
	int speed;
	SystemParametersInfoW(SPI_GETMOUSESPEED, 0, &speed, 0);
	f32 speed_table[20] = {
		0.03125f,
		0.0625f,
		0.125f,
		0.25f,
		0.375f,
		0.5f,
		0.666666f,
		0.75f,
		0.875f,
		1,
		1.25f,
		1.5f,
		1.75f,
		2,
		2.25f,
		2.5f,
		2.75f,
		3,
		3.25f,
		3.5f,
	};

	return speed_table[speed - 1];
}

f32 get_monitor_refresh_time(HMONITOR hm) {
	MONITORINFOEXW mi;
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfoW(hm, &mi)) {
		DEVMODEW mode;
		mode.dmSize = sizeof(mode);
		if (EnumDisplaySettingsW(mi.szDevice, ENUM_CURRENT_SETTINGS, &mode)) {
			return 1.0f / mode.dmDisplayFrequency;
		}
	}
	return 1.0f / 60;
}
f32 get_monitor_refresh_time(HWND hw) {
	return get_monitor_refresh_time(MonitorFromWindow(hw, MONITOR_DEFAULTTONEAREST));
}

}

#endif
