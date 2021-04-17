#pragma once
#include "common.h"

#define TL_DEFINE_MOUSE_INPUT(mouse_state) \
inline bool mouse_held(u32 key) { return (mouse_state)[key] & KeyState_held; } \
inline bool mouse_down(u32 key) { return (mouse_state)[key] & KeyState_down; } \
inline bool mouse_up  (u32 key) { return (mouse_state)[key] & KeyState_up;   }

#define TL_DEFINE_KEYBOARD_INPUT(key_state) \
inline bool key_held    (u32 key) { return (key_state)[key] & KeyState_held;     } \
inline bool key_down    (u32 key) { return (key_state)[key] & KeyState_down;     } \
inline bool key_repeated(u32 key) { return (key_state)[key] & KeyState_repeated; } \
inline bool key_up      (u32 key) { return (key_state)[key] & KeyState_up;       }

namespace TL {

using KeyState = u8;

enum : KeyState {
	KeyState_none     = 0,
	KeyState_held     = 1,
	KeyState_down     = 2,
	KeyState_up       = 4,
	KeyState_repeated = 8,
};

#if OS_WINDOWS

enum Key {
	Key_tab       = 0x09,
	Key_shift     = 0x10,
	Key_control   = 0x11,
	Key_alt       = 0x12,
	Key_space     = ' ',
	Key_page_up   = 0x21,
	Key_page_down = 0x22,
	Key_end       = 0x23,
	Key_home      = 0x24,
	Key_f1        = 0x70,
	Key_f2        = 0x71,
	Key_f3        = 0x72,
	Key_f4        = 0x73,
	Key_f5        = 0x74,
	Key_f6        = 0x75,
	Key_f7        = 0x76,
	Key_f8        = 0x77,
	Key_f9        = 0x78,
	Key_f10       = 0x79,
	Key_f11       = 0x7A,
	Key_f12       = 0x7B,
};

#endif

inline void update_key_state(Span<KeyState> key_state) {
	for (KeyState &key : key_state) {
		if (key & KeyState_down) {
			key &= ~KeyState_down;
		} else if (key & KeyState_up) {
			key = KeyState_none;
		}
		if (key & KeyState_repeated) {
			key &= ~KeyState_repeated;
		}
	}
}

#ifdef TL_IMPL
#if OS_WINDOWS
bool process_mouse_message(MSG message, Span<KeyState> mouse_state, v2s *mouse_delta = 0, s32 *wheel = 0) {
	switch (message.message) {
		case WM_LBUTTONDOWN: { mouse_state[0] = KeyState_down | KeyState_held; return true; }
		case WM_RBUTTONDOWN: { mouse_state[1] = KeyState_down | KeyState_held; return true; }
		case WM_MBUTTONDOWN: { mouse_state[2] = KeyState_down | KeyState_held; return true; }
		case WM_LBUTTONUP: { mouse_state[0] = KeyState_up; return true; }
		case WM_RBUTTONUP: { mouse_state[1] = KeyState_up; return true; }
		case WM_MOUSEWHEEL:
			if (wheel) {
				*wheel += GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA;
				return true;
			}
			break;
		case WM_INPUT: {
			RAWINPUT rawInput;
			if (UINT rawInputSize = sizeof(rawInput);
				GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, &rawInput, &rawInputSize,
								sizeof(RAWINPUTHEADER)) == -1) {
				invalid_code_path("Error: GetRawInputData");
			}
			if (rawInput.header.dwType == RIM_TYPEMOUSE) {
				auto &mouse = rawInput.data.mouse;

				if (mouse_delta)
					*mouse_delta += {mouse.lLastX, mouse.lLastY};

				if (!mouse.usButtonFlags)
					return true;
				if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP) && (mouse_state[0] & KeyState_held)) mouse_state[0] = KeyState_up;
				if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP) && (mouse_state[1] & KeyState_held)) mouse_state[1] = KeyState_up;
				if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP) && (mouse_state[2] & KeyState_held)) mouse_state[2] = KeyState_up;
			}
			return true;
		}
	}
	return false;
}
bool process_keyboard_message(MSG message, Span<KeyState> key_state) {
	switch (message.message) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			u8 key = message.wParam;

			bool is_repeated = message.lParam & (LPARAM)(1 << 30);

			if (is_repeated) {
				key_state[key] |= KeyState_repeated;
			} else {
				key_state[key] = KeyState_repeated | KeyState_down | KeyState_held;
			}

			return true;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP: {
			u8 key = message.wParam;
			key_state[key] = KeyState_up | KeyState_held;
			return true;
		}
	}
	return false;
}

#endif
#endif

}
