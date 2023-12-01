#include "common.h"
#include "vector.h"
#include "list.h"

namespace tl {

template <class T>
struct DefaultShouldBeDilatedT;

template <>
struct DefaultShouldBeDilatedT<v4u8> {
    inline static constexpr auto value = [](v4u8 pixel) { return pixel.w == 0; };
};

template <>
struct DefaultShouldBeDilatedT<v4f32> {
    inline static constexpr auto value = [](v4f32 pixel) { return pixel.w < 1.0f / 255.f; };
};

template <class T>
using DefaultShouldBeDilated = DefaultShouldBeDilatedT<T>::value;

template <class Pixel, class ShouldBeDilated, class GetLength>
struct DilateParams {
    bool keep_alpha = {};
    ShouldBeDilated should_be_dilated = {};
    GetLength get_length = {};
};

template <class Pixel>
inline void dilate(Pixel *source_pixels, Pixel *destination_pixels, v2u size, u32 stride, bool keep_alpha) {
    auto radius = max(size.x, size.y) * 1.5f;

    if (stride == 0) {
        stride = size.x;
    }

    auto should_be_dilated = [](Pixel p) {
        if constexpr (std::is_same_v<Pixel, u32>) {
            return (p >> 24) == 0;
        } else if constexpr (std::is_same_v<Pixel, v4u8>) {
            return p.w == 0;
        } else if constexpr (std::is_same_v<Pixel, v4f32>) {
            return p.w < 1.0f / 255.f;
        } else {
            static_error_t(Pixel, "Unsupported pixel type");
        }
    };

    auto full_alpha = [] {
        if constexpr (std::is_same_v<Pixel, u32>) {
            return 255;
        } else if constexpr (std::is_same_v<Pixel, v4u8>) {
            return 255;
        } else if constexpr (std::is_same_v<Pixel, v4f32>) {
            return 1.0f;
        } else {
            static_error_t(Pixel, "Unsupported pixel type");
        }
    }();

    List<v2u> offsets;
    defer { free(offsets); };

    offsets.reserve(size.x*size.y);

    for (u32 iy = 0; iy < size.y; ++iy) {
        for (u32 ix = 0; ix < size.x; ++ix) {
            auto offset = v2u{ix,iy} - size/2;
            if (length(offset) <= radius)
                offsets.add(offset);
        }
    }

    quick_sort(offsets, [](auto x) { return length(x); });

    for (u32 iy = 0; iy < size.y; ++iy) {
        for (u32 ix = 0; ix < size.x; ++ix) {

            Pixel p = source_pixels[iy*stride + ix];

            if (should_be_dilated(p)) {
                for (auto offset : offsets) {
                    u32 jx = ix + offset.x;
                    u32 jy = iy + offset.y;

                    if (jx >= (u32)size.x) continue;
                    if (jy >= (u32)size.y) continue;

                    auto t = source_pixels[jy*stride + jx];
                    if (!should_be_dilated(t)) {
                        p.xyz = t.xyz;
                        if (!keep_alpha) {
                            p.w = full_alpha;
                        }
                        break;
                    }
                }
            } else {
                if (!keep_alpha) {
                    p.w = full_alpha;
                }
            }

            destination_pixels[iy*stride + ix] = p;
        }
    }
}

template <class Pixel>
inline void dilate(Pixel *pixels, v2u size, u32 stride, bool keep_alpha) {
    return dilate(pixels, pixels, size, stride, keep_alpha);
}

}
