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

#define ALL_KEYS(K) \
	K(a            , 'A') \
	K(b            , 'B') \
	K(c            , 'C') \
	K(d            , 'D') \
	K(e            , 'E') \
	K(f            , 'F') \
	K(g            , 'G') \
	K(h            , 'H') \
	K(i            , 'I') \
	K(j            , 'J') \
	K(k            , 'K') \
	K(l            , 'L') \
	K(m            , 'M') \
	K(n            , 'N') \
	K(o            , 'O') \
	K(p            , 'P') \
	K(q            , 'Q') \
	K(r            , 'R') \
	K(s            , 'S') \
	K(t            , 'T') \
	K(u            , 'U') \
	K(v            , 'V') \
	K(w            , 'W') \
	K(x            , 'X') \
	K(y            , 'Y') \
	K(z            , 'Z') \
	K(tab          , 0x09) \
	K(shift        , 0x10) \
	K(control      , 0x11) \
	K(alt          , 0x12) \
	K(space        , ' ')  \
	K(page_up      , 0x21) \
	K(page_down    , 0x22) \
	K(end          , 0x23) \
	K(home         , 0x24) \
	K(semicolon    , 0xBA) \
	K(plus         , 0xBB) \
	K(comma        , 0xBC) \
	K(period       , 0xBE) \
	K(slash        , 0xBF) \
	K(left_bracket , 0xDB) \
	K(backslash    , 0xDC) \
	K(right_bracket, 0xDD) \
	K(quote        , 0xDE) \
	K(f1           , 0x70) \
	K(f2           , 0x71) \
	K(f3           , 0x72) \
	K(f4           , 0x73) \
	K(f5           , 0x74) \
	K(f6           , 0x75) \
	K(f7           , 0x76) \
	K(f8           , 0x77) \
	K(f9           , 0x78) \
	K(f10          , 0x79) \
	K(f11          , 0x7A) \
	K(f12          , 0x7B)

#endif

enum Key {
#define K(key, value) Key_##key = value,
	ALL_KEYS(K)
#undef K
};

inline char const *to_string(Key key) {
	switch (key) {
#define K(key, value) case Key_##key: return #key;
	ALL_KEYS(K)
#undef K
	}
	return "unknown";
}

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

extern void (*on_key_down)(u8 key);
extern void (*on_key_repeat)(u8 key);
extern void (*on_key_up)(u8 key);
extern void (*on_char)(u32 ch);

#ifdef TL_IMPL
#if OS_WINDOWS
void (*on_key_down)(u8 key);
void (*on_key_repeat)(u8 key);
void (*on_key_up)(u8 key);
void (*on_char)(u32 ch);

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
				if (on_key_repeat) on_key_repeat(key);
			} else {
				key_state[key] = KeyState_repeated | KeyState_down | KeyState_held;
				if (on_key_down) on_key_down(key);
				if (on_key_repeat) on_key_repeat(key);
			}

			return true;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP: {
			u8 key = message.wParam;
			key_state[key] = KeyState_up | KeyState_held;
			if (on_key_up) on_key_up(key);
			return true;
		}
		case WM_UNICHAR: {
			if (on_char)
				on_char(message.wParam);
			return true;
		}
	}
	return false;
}

#endif
#endif

}
