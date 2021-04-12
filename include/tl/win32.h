#pragma once
#include "system.h"
#include "math.h"
#include "common.h"
#define NOMINMAX
#pragma warning(push, 0)
#include <Windows.h>
#include <Psapi.h>
#pragma warning(pop, 0)
#pragma comment(lib, "user32.lib")

namespace TL {

enum RawInputDevice {
	RawInput_mouse = 0x1,
};

TL_API extern s64 performance_frequency;

TL_API bool init_rawinput(RawInputDevice deviceFlags);
TL_API bool processRawInputMessage(MSG msg, bool mouseButtons[5], s32 *mouseWheel, v2s *mouseDelta);
TL_API bool processKeyboardMessage(MSG message, bool keyboardButtons[256], bool handleRepeated);
TL_API s64 get_performance_counter();
TL_API bool register_window_class(HINSTANCE instance, char const *name, UINT style, HCURSOR cursor, LRESULT (*wndProc)(HWND, UINT, WPARAM, LPARAM));
TL_API void clampWindowToMonitor(HWND Window, bool move, HMONITOR monitor = (HMONITOR)INVALID_HANDLE_VALUE);
TL_API LRESULT getBorderHit(s32 x, s32 y, s32 sizeX, s32 sizeY, s32 borderWidth, LRESULT centerHit);
TL_API v2u get_window_size(v2u clientSize, DWORD style, bool menu = false);
TL_API v2s get_window_position(v2s client_position, DWORD style, bool menu = false);
TL_API void hide_cursor();
TL_API void show_cursor();
inline void set_cursor_visibility(bool visible) {
	if (visible) show_cursor();
	else hide_cursor();
}
TL_API void set_cursor_position(s32 x, s32 y);
inline void set_cursor_position(v2s position) {
	set_cursor_position(position.x, position.y);
}
TL_API v2s get_cursor_position();
TL_API u64 get_memory_usage();

TL_API bool set_fullscreen(HWND window, bool enable, DWORD window_style, WINDOWPLACEMENT &placement);

#ifdef TL_IMPL
bool init_rawinput(RawInputDevice deviceFlags) {
	StaticList<RAWINPUTDEVICE, 1> devices;
	if (deviceFlags & RawInput_mouse) {
		RAWINPUTDEVICE mouse = {};
		mouse.usUsagePage = 0x01;
		mouse.usUsage = 0x02;
		devices.add(mouse);
	}
	if (devices.size) {
		if (!RegisterRawInputDevices(devices.data, (UINT)devices.size, sizeof(devices[0]))) {
			return false;
		}
	}
	return true;
}
bool processRawInputMessage(MSG msg, bool mouseButtons[5], s32 *mouseWheel, v2s *mouseDelta) {
	if (msg.message == WM_INPUT) {
		RAWINPUT rawInput;
		if (UINT rawInputSize = sizeof(rawInput);
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, &rawInput, &rawInputSize,
							sizeof(RAWINPUTHEADER)) == -1) {
			invalid_code_path("Error: GetRawInputData");
		}
		if (rawInput.header.dwType == RIM_TYPEMOUSE) {
			auto &mouse = rawInput.data.mouse;
			if (mouseDelta)
				*mouseDelta += {mouse.lLastX, mouse.lLastY};
			if (mouseWheel)
				if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
					*mouseWheel += (s16)mouse.usButtonData / WHEEL_DELTA;
			if (mouseButtons) {
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
					mouseButtons[key] = value;
				}
			}
		}
		return true;
	}
	return false;
}
bool processKeyboardMessage(MSG message, bool keyboardButtons[256], bool handleRepeated) {
	switch (message.message) {
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN: {
			auto code = (u8)message.wParam;
			bool extended   = message.lParam & u32(1 << 24);
			bool alt        = message.lParam & u32(1 << 29);
			bool isRepeated = message.lParam & u32(1 << 30);
			bool wentUp     = message.lParam & u32(1 << 31);
			if (handleRepeated || isRepeated == wentUp) { // Don't handle repeated
				keyboardButtons[code] = !wentUp;
			}
			(void)extended;
			(void)alt;
			return true;
		}
	}
	return false;
}
s64 performance_frequency = [] {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}();
s64 get_performance_counter() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}
void clampWindowToMonitor(HWND Window, bool move, HMONITOR monitor) {
    RECT Rect = {};
    GetWindowRect(Window, &Rect);
    if (monitor == INVALID_HANDLE_VALUE) {
		monitor = MonitorFromRect(&Rect, MONITOR_DEFAULTTONEAREST);
	}
    MONITORINFO MonitorInfo = {};
    MonitorInfo.cbSize = sizeof(MonitorInfo);
    GetMonitorInfoA(monitor, &MonitorInfo);

	auto monitorX = MonitorInfo.rcWork.left;
	auto monitorY = MonitorInfo.rcWork.top;
	auto monitorWidth = MonitorInfo.rcWork.right - MonitorInfo.rcWork.left;
	auto monitorHeight = MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top;

    int X = Rect.left - MonitorInfo.rcWork.left;
    int Y = Rect.top - MonitorInfo.rcWork.top;
    int Width = Rect.right - Rect.left;
    int Height = Rect.bottom - Rect.top;

    X = max(X, 0);
    Y = max(Y, 0);

	if (move) {
		if (Width > monitorWidth) Width = monitorWidth;
		if (Height > monitorHeight) Height = monitorHeight;
	} else {
		if (Width > monitorWidth - X) Width = monitorWidth - X;
		if (Height > monitorHeight - Y) Height = monitorHeight - Y;
	}

	if (Width == 0)
		debug_break();

    if (X > monitorWidth - Width) X = monitorWidth - Width;
    if (Y > monitorHeight - Height) Y = monitorHeight - Height;

    MoveWindow(Window, X + monitorX, Y + monitorY, Width, Height, 0);
}
LRESULT getBorderHit(s32 x, s32 y, s32 sizeX, s32 sizeY, s32 borderWidth, LRESULT centerHit) {
	if (x < borderWidth) {
		if (y < borderWidth) {
			return HTTOPLEFT;
		} else if (y >= sizeY - borderWidth) {
			return HTBOTTOMLEFT;
		} else {
			return HTLEFT;
		}
	} else if (x >= sizeX - borderWidth) {
		if (y < borderWidth) {
			return HTTOPRIGHT;
		} else if (y >= sizeY - borderWidth) {
			return HTBOTTOMRIGHT;
		} else {
			return HTRIGHT;
		}
	} else {
		if (y < borderWidth) {
			return HTTOP;
		} else if (y >= sizeY - borderWidth) {
			return HTBOTTOM;
		}
	}
	return centerHit;
}
bool register_window_class(HINSTANCE instance, char const *name, UINT style, HCURSOR cursor, LRESULT (*wndProc)(HWND, UINT, WPARAM, LPARAM)) {
	WNDCLASSEXA c = {};
	c.cbSize = sizeof(c);
	c.hCursor = cursor;
	c.hInstance = instance;
	c.lpfnWndProc = wndProc;
	c.lpszClassName = name;
	c.style = style;
	return RegisterClassExA(&c) != 0;
}
v2u get_window_size(v2u clientSize, DWORD style, bool menu) {
	RECT r = {0, 0, (LONG)clientSize.x, (LONG)clientSize.y};
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
v2s get_cursor_position() {
	POINT p;
	GetCursorPos(&p);
	return {p.x, p.y};
}
u64 get_memory_usage() {
	u64 result = 0;
	PROCESS_MEMORY_COUNTERS memoryCounters;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &memoryCounters, sizeof(memoryCounters))) {
		result = memoryCounters.PagefileUsage;
	}
	return result;
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

#endif

}

#ifdef TL_WIN32_ENTRY
extern TL::s32 tl_main(TL::Span<char> args);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR command_line, int) {
	TL::init_allocator();
	TL::print("command_line: %\n", command_line);
	return tl_main({});
}
#endif
