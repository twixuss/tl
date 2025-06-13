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
    static constexpr f32 RADIUS_FULL_IMAGE = 0;

    bool keep_alpha : 1 = false;
    bool smooth : 1 = true;
    f32 radius = RADIUS_FULL_IMAGE;
};

template <class Pixel>
inline void dilate(Pixel *source_pixels, Pixel *destination_pixels, v2u size, u32 source_stride_in_pixels, u32 destination_stride_in_pixels, DilateOptions options = {}) {
    if (options.radius == DilateOptions::RADIUS_FULL_IMAGE)
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

        offsets.reserve(min((u32)pow2(options.radius*2 + 1), size.x*size.y));
        
        for (s32 i = 1; i <= options.radius; ++i) {
            auto add_offset = [&](v2s o) {
                if (length_squared(o) <= pow2(options.radius)) {
                    offsets.add((v2i)o);
                }
            };

            if (i < size.x) {
                s32 lim = min(i - 1, (s32)size.y);
                for (s32 j = -lim; j <= lim; ++j) {
                    add_offset({-i, j});
                    add_offset({+i, j});
                }
            }
            if (i < size.y) {
                s32 lim = min(i, (s32)size.x);
                for (s32 j = -lim; j <= lim; ++j) {
                    add_offset({j, -i});
                    add_offset({j, +i});
                }
            }
        }

        quick_sort(offsets, [](auto o) { return o.x*o.x + o.y*o.y; });

        //umm next_time_starting_from = 0;
        //
        //auto update_starting_index = [&] (umm current_index, smm radius_delta) {
        //    f32 current_radius = tl::fast_sqrt((f32)current_index) / pi;
        //    f32 smaller_radius = floor(max(0.0f, current_radius - radius_delta));
        //    next_time_starting_from = floor_to_int(pi * pow2(smaller_radius));
        //};
        if (options.smooth) {
        
            for (smm iy = 0; iy < size.y; ++iy) {
                //next_time_starting_from = 0;
                for (smm ix = 0; ix < size.x; ++ix) {
                    Pixel p = source_pixels[iy*source_stride_in_pixels + ix];

                    if (should_be_dilated(p)) {
                        
                        v3f color_sum = {};
                        f32 factor_sum = {};

                        f32 closest_pixel_distance = 0;
                        for (auto offset : offsets/*.skip(next_time_starting_from)*/) {
                            smm jx = ix + offset.x;
                            smm jy = iy + offset.y;

                            if ((umm)jx >= size.x) continue;
                            if ((umm)jy >= size.y) continue;

                            auto t = source_pixels[jy*source_stride_in_pixels + jx];
                            if (!should_be_dilated(t)) {
                                f32 distance = length(offset);

                                f32 const max_distance = 1;

                                if (factor_sum == 0) {
                                    closest_pixel_distance = distance;
                                    color_sum += (v3f)t.xyz;
                                    factor_sum += 1;
                                } else {
                                    if (distance >= closest_pixel_distance + max_distance) {
                                        //update_starting_index(index_of(offsets, &offset), 2);
                                        break;
                                    }
                                    f32 f = (distance - closest_pixel_distance) / max_distance;
                                    color_sum += (v3f)t.xyz * f;
                                    factor_sum += f;
                                }
                            }
                        }

                        p.xyz = autocast (color_sum / factor_sum);
                        if (!options.keep_alpha) {
                            p.w = full_alpha;
                        }
                    } else {
                        if (!options.keep_alpha) {
                            p.w = full_alpha;
                        }
                    }

                    destination_pixels[iy*destination_stride_in_pixels + ix] = p;
                }
            }

        } else {
            for (smm iy = 0; iy < size.y; ++iy) {
                //next_time_starting_from = 0;
                for (smm ix = 0; ix < size.x; ++ix) {
                    Pixel p = source_pixels[iy*source_stride_in_pixels + ix];

                    if (should_be_dilated(p)) {

                        f32 closest_pixel_distance = 0;

                        // TODO: optimize starting offset here aswell
                        for (auto offset : offsets) {
                            smm jx = ix + offset.x;
                            smm jy = iy + offset.y;

                            if ((umm)jx >= size.x) continue;
                            if ((umm)jy >= size.y) continue;

                            auto t = source_pixels[jy*source_stride_in_pixels + jx];
                            if (!should_be_dilated(t)) {
                                p.xyz = t.xyz;
                                if (!options.keep_alpha) {
                                    p.w = full_alpha;
                                }
                                break;
                            }
                        }
                    } else {
                        if (!options.keep_alpha) {
                            p.w = full_alpha;
                        }
                    }

                    destination_pixels[iy*destination_stride_in_pixels + ix] = p;
                }
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
        return dilate_impl.template operator()<s16>();
    }
    if (size.x <= (1<<31) && size.y <= (1<<31)) {
        return dilate_impl.template operator()<s32>();
    }
    return dilate_impl.template operator()<s64>();
}

template <class Pixel>
inline void dilate(Pixel *pixels, v2u size, umm stride_in_pixels, DilateOptions options = {}) {
    // NOTE:
    // Images are likely too big to store in temporary memory.
    auto allocator = TL_GET_CURRENT(allocator);
    auto copy = allocator.template allocate<Pixel>(size.y*size.x);
    defer { allocator.free(copy); };

    for (u32 y = 0; y < size.y; ++y) {
        memcpy(copy + y*size.x, pixels + y*stride_in_pixels, sizeof(Pixel)*size.x);
    }

    return dilate(copy, pixels, size, size.x, stride_in_pixels, options);
}

}
