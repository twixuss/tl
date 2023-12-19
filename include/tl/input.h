#pragma once
#include "common.h"

namespace tl {

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
	K(enter        , 0x0d) \
	K(shift        , 0x10) \
	K(control      , 0x11) \
	K(alt          , 0x12) \
	K(escape       , 0x1b) \
	K(space        , ' ')  \
	K(page_up      , 0x21) \
	K(page_down    , 0x22) \
	K(end          , 0x23) \
	K(home         , 0x24) \
	K(left_arrow   , 0x25) \
	K(up_arrow     , 0x26) \
	K(right_arrow  , 0x27) \
	K(down_arrow   , 0x28) \
	K(delete       , 0x2e) \
	K(semicolon    , 0xba) \
	K(plus         , 0xbb) \
	K(comma        , 0xbc) \
	K(minus        , 0xbd) \
	K(period       , 0xbe) \
	K(slash        , 0xbf) \
	K(left_bracket , 0xdb) \
	K(backslash    , 0xdc) \
	K(right_bracket, 0xdd) \
	K(quote        , 0xde) \
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
	K(f11          , 0x7a) \
	K(f12          , 0x7b) \

#endif

enum Key {
#define K(key, value) Key_##key = value,
	ALL_KEYS(K)
#undef K
};

inline umm append(StringBuilder &builder, Key key) {
	switch (key) {
#define K(key, value) case Key_##key: return append(builder, u8#key##s);
	ALL_KEYS(K)
#undef K
	}
	return append(builder, u8"none"s);
}

}
