#pragma once
#include "system.h"
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

#endif

}

#endif