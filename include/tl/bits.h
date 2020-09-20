#pragma once
#include "system.h"

namespace TL {

u8 reverse(u8 b) {
   b = (u8)((b & 0xF0) >> 4 | (b & 0x0F) << 4);
   b = (u8)((b & 0xCC) >> 2 | (b & 0x33) << 2);
   b = (u8)((b & 0xAA) >> 1 | (b & 0x55) << 1);
   return b;
}
u16 reverse(u16 b) {
   b = (u16)((b & 0xFF00) >> 8 | (b & 0x00FF) << 8);
   b = (u16)((b & 0xF0F0) >> 4 | (b & 0x0F0F) << 4);
   b = (u16)((b & 0xCCCC) >> 2 | (b & 0x3333) << 2);
   b = (u16)((b & 0xAAAA) >> 1 | (b & 0x5555) << 1);
   return b;
}
u32 reverse(u32 b) {
   b = (b & 0xFFFF0000) >> 16 | (b & 0x0000FFFF) << 16;
   b = (b & 0xFF00FF00) >>  8 | (b & 0x00FF00FF) <<  8;
   b = (b & 0xF0F0F0F0) >>  4 | (b & 0x0F0F0F0F) <<  4;
   b = (b & 0xCCCCCCCC) >>  2 | (b & 0x33333333) <<  2;
   b = (b & 0xAAAAAAAA) >>  1 | (b & 0x55555555) <<  1;
   return b;
}
u64 reverse(u64 b) {
   b = (b & 0xFFFFFFFF00000000) >> 32 | (b & 0x00000000FFFFFFFF) << 32;
   b = (b & 0xFFFF0000FFFF0000) >> 16 | (b & 0x0000FFFF0000FFFF) << 16;
   b = (b & 0xFF00FF00FF00FF00) >>  8 | (b & 0x00FF00FF00FF00FF) <<  8;
   b = (b & 0xF0F0F0F0F0F0F0F0) >>  4 | (b & 0x0F0F0F0F0F0F0F0F) <<  4;
   b = (b & 0xCCCCCCCCCCCCCCCC) >>  2 | (b & 0x3333333333333333) <<  2;
   b = (b & 0xAAAAAAAAAAAAAAAA) >>  1 | (b & 0x5555555555555555) <<  1;
   return b;
}

}
