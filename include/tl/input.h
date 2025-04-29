#pragma once
#include "common.h"
#include "string.h"

namespace tl {

#if OS_WINDOWS

#define ALL_KEYS(K) \
	K(backspace,     '\b') \
	K(tab,           '\t') \
	K(enter,         '\r') \
	K(clear,         0x0c) \
	K(shift,         0x10) \
	K(control,       0x11) \
	K(alt,           0x12) \
	K(escape,        0x1b) \
	K(pause,         0x13) \
	K(capslock,      0x14) \
	K(space,         ' ')  \
	K(page_up,       0x21) \
	K(page_down,     0x22) \
	K(end,           0x23) \
	K(home,          0x24) \
	K(left_arrow,    0x25) \
	K(up_arrow,      0x26) \
	K(right_arrow,   0x27) \
	K(down_arrow,    0x28) \
	K(delete,        0x2e) \
	K(insert,        0x2D) \
	K(0,             '0') \
	K(1,             '1') \
	K(2,             '2') \
	K(3,             '3') \
	K(4,             '4') \
	K(5,             '5') \
	K(6,             '6') \
	K(7,             '7') \
	K(8,             '8') \
	K(9,             '9') \
	K(a,             'A') \
	K(b,             'B') \
	K(c,             'C') \
	K(d,             'D') \
	K(e,             'E') \
	K(f,             'F') \
	K(g,             'G') \
	K(h,             'H') \
	K(i,             'I') \
	K(j,             'J') \
	K(k,             'K') \
	K(l,             'L') \
	K(m,             'M') \
	K(n,             'N') \
	K(o,             'O') \
	K(p,             'P') \
	K(q,             'Q') \
	K(r,             'R') \
	K(s,             'S') \
	K(t,             'T') \
	K(u,             'U') \
	K(v,             'V') \
	K(w,             'W') \
	K(x,             'X') \
	K(y,             'Y') \
	K(z,             'Z') \
	K(left_win,      0x5B) \
	K(right_win,     0x5C) \
	K(num_lock,      0x90) \
	K(scroll_lock,   0x91) \
	K(launch_app_1,  0xb6) \
	K(launch_app_2,  0xb7) \
	K(semicolon,     0xba) \
	K(plus,          0xbb) \
	K(comma,         0xbc) \
	K(minus,         0xbd) \
	K(period,        0xbe) \
	K(slash,         0xbf) \
	K(accent,        0xc0) \
	K(left_bracket,  0xdb) \
	K(backslash,     0xdc) \
	K(right_bracket, 0xdd) \
	K(quote,         0xde) \
	K(num0,          0x60) \
	K(num1,          0x61) \
	K(num2,          0x62) \
	K(num3,          0x63) \
	K(num4,          0x64) \
	K(num5,          0x65) \
	K(num6,          0x66) \
	K(num7,          0x67) \
	K(num8,          0x68) \
	K(num9,          0x69) \
	K(multiply,      0x6a) \
	K(add,           0x6b) \
	K(separator,     0x6c) \
	K(subtract,      0x6d) \
	K(decimal,       0x6e) \
	K(divide,        0x6f) \
	K(f1,            0x70) \
	K(f2,            0x71) \
	K(f3,            0x72) \
	K(f4,            0x73) \
	K(f5,            0x74) \
	K(f6,            0x75) \
	K(f7,            0x76) \
	K(f8,            0x77) \
	K(f9,            0x78) \
	K(f10,           0x79) \
	K(f11,           0x7a) \
	K(f12,           0x7b) \
	K(f13,           0x7c) \
	K(f14,           0x7d) \
	K(f15,           0x7e) \
	K(f16,           0x7f) \
	K(f17,           0x80) \
	K(f18,           0x81) \
	K(f19,           0x82) \
	K(f20,           0x83) \
	K(f21,           0x84) \
	K(f22,           0x85) \
	K(f23,           0x86) \
	K(f24,           0x87) \

#endif

enum Key : u16 {
#define K(key, value) Key_##key = value,
	ALL_KEYS(K)
#undef K
};

inline void append(StringBuilder &builder, Key key) {
	switch (key) {
		#define K(key, value) case Key_##key: return append(builder, u8###key##s);
		ALL_KEYS(K)
		#undef K
	}
	return append_format(builder, "Key {}", (u32)key);
}

}
