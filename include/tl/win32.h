#pragma once
#include "system.h"
#ifdef TL_WIN32_USE_MATH
#include "math.h"
#endif
#if OS_WINDOWS
#include "common.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#pragma comment(lib, "user32.lib")

namespace TL {

enum RawInputDevice {
	RawInput_mouse = 0x1,
};
TL_API bool registerRawInput(RawInputDevice deviceFlags);
TL_API bool processRawInputMessage(MSG msg, bool mouseButtons[5], s32 *mouseWheel, v2s *mouseDelta);
TL_API bool processKeyboardMessage(MSG message, bool keyboardButtons[256], bool handleRepeated);
TL_API s64 getPerformanceFrequency();
TL_API s64 getPerformanceCounter();
TL_API bool registerWindowClass(HINSTANCE instance, char const *name, UINT style, HCURSOR cursor, LRESULT (*wndProc)(HWND, UINT, WPARAM, LPARAM));
#ifdef TL_WIN32_USE_MATH
TL_API v2u getWindowSize(v2u clientSize, DWORD style, bool menu = false);
#endif

#ifdef TL_IMPL
bool registerRawInput(RawInputDevice deviceFlags) {
	StaticList<RAWINPUTDEVICE, 1> devices;
	if (deviceFlags & RawInput_mouse) {
		RAWINPUTDEVICE mouse = {};
		mouse.usUsagePage = 0x01;
		mouse.usUsage = 0x02;
		devices.push_back(mouse);
	}
	if (devices.size()) {
		if (!RegisterRawInputDevices(devices.data(), (UINT)devices.size(), sizeof(devices[0]))) {
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
			INVALID_CODE_PATH("Error: GetRawInputData");
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
s64 getPerformanceFrequency() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}
s64 getPerformanceCounter() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}
void clampWindowToMonitor(HWND Window, bool move, HMONITOR monitor = (HMONITOR)INVALID_HANDLE_VALUE) {
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
		DEBUG_BREAK;

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
bool registerWindowClass(HINSTANCE instance, char const *name, UINT style, HCURSOR cursor, LRESULT (*wndProc)(HWND, UINT, WPARAM, LPARAM)) {
	WNDCLASSEXA c = {};
	c.cbSize = sizeof(c);
	c.hCursor = cursor;
	c.hInstance = instance;
	c.lpfnWndProc = wndProc;
	c.lpszClassName = name;
	c.style = style;
	return RegisterClassExA(&c) != 0;
}
#ifdef TL_WIN32_USE_MATH
v2u getWindowSize(v2u clientSize, DWORD style, bool menu) {
	RECT r = {0, 0, (LONG)clientSize.x, (LONG)clientSize.y};
	AdjustWindowRect(&r, style, menu);
	return {
		(u32)(r.right - r.left),
		(u32)(r.bottom - r.top)
	};
}
#endif
#endif

}

#endif
