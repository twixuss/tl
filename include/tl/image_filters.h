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

struct DilateOptions {
    bool keep_alpha : 1 = false;
    bool smooth : 1 = true;
    f32 radius = 0;
};

template <class Pixel>
inline void dilate(Pixel *source_pixels, Pixel *destination_pixels, v2u size, u32 source_stride, u32 destination_stride, DilateOptions options = {}) {
    if (options.radius == 0)
        options.radius = max(size.x, size.y) * 1.5f;

    auto should_be_dilated = [](Pixel p) {
        if constexpr (std::is_same_v<Pixel, v4u8>) {
            return p.w == 0;
        } else if constexpr (std::is_same_v<Pixel, v4f32>) {
            return p.w < 1.0f / 255.f;
        } else {
            static_error_t(Pixel, "Unsupported pixel type");
        }
    };

    auto full_alpha = [] {
        if constexpr (std::is_same_v<Pixel, v4u8>) {
            return 255;
        } else if constexpr (std::is_same_v<Pixel, v4f32>) {
            return 1.0f;
        } else {
            static_error_t(Pixel, "Unsupported pixel type");
        }
    }();

    auto dilate_impl = [&]<class Int>() {
        using v2i = v2<Int>;

        // NOTE:
        // offsets are likely too big to store in temporary memory.
        List<v2i> offsets;
        defer { free(offsets); };

        offsets.reserve(size.x*size.y);

        for (smm iy = 0; iy < size.y; ++iy) {
            for (smm ix = 0; ix < size.x; ++ix) {
                auto offset = (v2i)(V2u(ix, iy) - size / 2);
                if (length(offset) <= options.radius)
                    offsets.add(offset);
            }
        }

        quick_sort(offsets, [](auto x) { return length(x); });

        //umm next_time_starting_from = 0;
        //
        //auto update_starting_index = [&] (umm current_index, smm radius_delta) {
        //    f32 current_radius = tl::fast_sqrt((f32)current_index) / pi;
        //    f32 smaller_radius = floor(max(0.0f, current_radius - radius_delta));
        //    next_time_starting_from = floor_to_int(pi * pow2(smaller_radius));
        //};

        for (smm iy = 0; iy < size.y; ++iy) {
            //next_time_starting_from = 0;
            for (smm ix = 0; ix < size.x; ++ix) {
                Pixel p = source_pixels[iy*source_stride + ix];

                if (should_be_dilated(p)) {
                    struct FactoredPixel {
                        Pixel pixel;
                        f32 factor;
                    };
                    scoped(temporary_storage_checkpoint);
                    List<FactoredPixel, TemporaryAllocator> closest_pixels;
                    f32 closest_pixel_distance = 0;
                    if (options.smooth) {
                        for (auto offset : offsets/*.skip(next_time_starting_from)*/) {
                            smm jx = ix + offset.x;
                            smm jy = iy + offset.y;

                            if ((umm)jx >= size.x) continue;
                            if ((umm)jy >= size.y) continue;

                            auto t = source_pixels[jy*source_stride + jx];
                            if (!should_be_dilated(t)) {
                                f32 distance = length(offset);

                                f32 const max_distance = sqrt2 - 1;

                                if (closest_pixels.count == 0) {
                                    closest_pixel_distance = distance;
                                    closest_pixels.add({t, 1});
                                } else {
                                    if (distance >= closest_pixel_distance + max_distance) {
                                        //update_starting_index(index_of(offsets, &offset), 2);
                                        break;
                                    }
                                    closest_pixels.add({t, (distance - closest_pixel_distance) / max_distance});
                                }
                            }
                        }

                        v3f color_sum = {};
                        f32 factor_sum = {};
                        for (auto t : closest_pixels) {
                            color_sum += (v3f)t.pixel.xyz * t.factor;
                            factor_sum += t.factor;
                        }

                        p.xyz = autocast (color_sum / factor_sum);
                        if (!options.keep_alpha) {
                            p.w = full_alpha;
                        }
                    } else {
                        // TODO: optimize starting offset here aswell
                        for (auto offset : offsets) {
                            smm jx = ix + offset.x;
                            smm jy = iy + offset.y;

                            if ((umm)jx >= size.x) continue;
                            if ((umm)jy >= size.y) continue;

                            auto t = source_pixels[jy*source_stride + jx];
                            if (!should_be_dilated(t)) {
                                p.xyz = t.xyz;
                                if (!options.keep_alpha) {
                                    p.w = full_alpha;
                                }
                                break;
                            }
                        }
                    }
                } else {
                    if (!options.keep_alpha) {
                        p.w = full_alpha;
                    }
                }

                destination_pixels[iy*destination_stride + ix] = p;
            }
        }
    };

    // NOTE: For some reason this runs EXTREMELY slow, like 5 times slower that s32.
    //       Hotspot was at `for (auto offset : offsets)`, assembly was not that much
    //       different from s32, just two regular moves were replaced with movxz/movsx.
    //       Have no idea why.
    //       
    //if (size.x <= (1<< 7) && size.y <= (1<< 7)) {
    //    return dilate_impl.operator()<s8>();
    //}
    if (size.x <= (1<<15) && size.y <= (1<<15)) {
        return dilate_impl.operator()<s16>();
    }
    if (size.x <= (1<<31) && size.y <= (1<<31)) {
        return dilate_impl.operator()<s32>();
    }
    return dilate_impl.operator()<s64>();
}

template <class Pixel>
inline void dilate(Pixel *pixels, v2u size, umm stride, DilateOptions options = {}) {
    // NOTE:
    // Images are likely too big to store in temporary memory.
    auto allocator = TL_GET_CURRENT(allocator);
    auto copy = allocator.allocate<Pixel>(size.y*size.x);
    defer { allocator.free(copy); };

    for (u32 y = 0; y < size.y; ++y) {
        memcpy(copy + y*size.x, pixels + y*stride, sizeof(Pixel)*size.x);
    }

    return dilate(copy, pixels, size, size.x, stride, options);
}

}
