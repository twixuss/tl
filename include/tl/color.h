#include "common.h"
#include "vector.h"

namespace tl {

struct color4u16 {
    u16 r, g, b, a;
};

color4u16 to_color4u16(v4f v) {
    return {
        (u16)min<f32>(v.x * 65535, 65535),
        (u16)min<f32>(v.y * 65535, 65535),
        (u16)min<f32>(v.z * 65535, 65535),
        (u16)min<f32>(v.w * 65535, 65535),
    };
}

color4u16 operator+(color4u16 a, color4u16 b) {
    return {
        (u16)min<u32>((u32)a.r + (u32)b.r, 65535),
        (u16)min<u32>((u32)a.g + (u32)b.g, 65535),
        (u16)min<u32>((u32)a.b + (u32)b.b, 65535),
        (u16)min<u32>((u32)a.a + (u32)b.a, 65535),
    };
}

color4u16 operator-(color4u16 a, color4u16 b) {
    return {
        (u16)max((s32)a.r - (s32)b.r, 0),
        (u16)max((s32)a.g - (s32)b.g, 0),
        (u16)max((s32)a.b - (s32)b.b, 0),
        (u16)max((s32)a.a - (s32)b.a, 0),
    };
}

color4u16 operator*(color4u16 a, color4u16 b) {
#if 1
    auto A = _mm_setr_epi32(a.r, a.g, a.b, a.a);
    auto B = _mm_setr_epi32(b.r, b.g, b.b, b.a);
    auto y = _mm_mullo_epi32(A, B);
    auto z = _mm_add_epi32(y, _mm_set1_epi32(65535));
    auto x = _mm_srli_epi32(z, 16);
    return {
        (u16)_mm_extract_epi32(x, 0),
        (u16)_mm_extract_epi32(x, 1),
        (u16)_mm_extract_epi32(x, 2),
        (u16)_mm_extract_epi32(x, 3),
    };
#elif 1
    return {
        (u16)((((u32)a.r * (u32)b.r) + 65535) >> (u32)16),
        (u16)((((u32)a.g * (u32)b.g) + 65535) >> (u32)16),
        (u16)((((u32)a.b * (u32)b.b) + 65535) >> (u32)16),
        (u16)((((u32)a.a * (u32)b.a) + 65535) >> (u32)16),
    };
#else
    return {
        (u16)(((u32)a.r * (u32)b.r) / (u32)65535),
        (u16)(((u32)a.g * (u32)b.g) / (u32)65535),
        (u16)(((u32)a.b * (u32)b.b) / (u32)65535),
        (u16)(((u32)a.a * (u32)b.a) / (u32)65535),
    };
#endif
}

color4u16 operator*(color4u16 a, v4f b) { return a * to_color4u16(b);}
color4u16 operator*(color4u16 a, f32 b) { return a * to_color4u16({b,b,b,b});}

color4u16 &operator+=(color4u16 &a, color4u16 b) { return a = a + b; }
color4u16 &operator-=(color4u16 &a, color4u16 b) { return a = a - b; }
color4u16 &operator*=(color4u16 &a, color4u16 b) { return a = a * b; }

}
