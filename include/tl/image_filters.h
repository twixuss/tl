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

struct DefaultDilateOptions {
    static constexpr f32 RADIUS_FULL_IMAGE = 0;

    bool keep_alpha : 1 = false;
    bool smooth : 1 = true;
    f32 radius = RADIUS_FULL_IMAGE;

    template <class Pixel>
    bool should_be_dilated(Pixel p) {
        if constexpr (std::is_same_v<Pixel, v4u8>) {
            return p.w == 0;
        } else if constexpr (std::is_same_v<Pixel, v4f32>) {
            return p.w < 1.0f / 255.f;
        } else {
            static_error_t(Pixel, "Unsupported pixel type");
        }
    }
    template <class Pixel>
    auto full_alpha() {
        if constexpr (std::is_same_v<Pixel, v4u8>) {
            return 255;
        } else if constexpr (std::is_same_v<Pixel, v4f32>) {
            return 1.0f;
        } else {
            static_error_t(Pixel, "Unsupported pixel type");
        }
    }

    Optional<v2smm> remap_pixel_position(v2smm p, v2u size) {
        (void)size;
        if ((umm)p.x >= size.x) return {};
        if ((umm)p.y >= size.y) return {};
        return p;
    }

    f32 distance(v2smm a, v2smm b, v2u size) {
        (void)size;
        return tl::distance((v2f)a, (v2f)b);
    }
};

template <class Pixel, class DilateOptions = DefaultDilateOptions>
inline void dilate(Pixel *source_pixels, Pixel *destination_pixels, v2u size, u32 source_stride_in_pixels, u32 destination_stride_in_pixels, DilateOptions options = {}) {
    if (options.radius == DilateOptions::RADIUS_FULL_IMAGE)
        options.radius = max(size.x, size.y) * 1.5f;

    auto full_alpha = options.template full_alpha<Pixel>();

    auto dilate_smooth_impl = [&]<class Int>() {
        using v2i = v2<Int>;

        // NOTE:
        // offsets are likely too big to store in temporary memory.
        List<v2i> offsets;
        defer { free(offsets); };

        offsets.reserve(min((u32)pow2(ceil_to_int(options.radius)*2 + 1), (size.x*2 - 1) * (size.y*2 - 1)));
        
        f32 rsq = pow2(options.radius);
        for (s32 i = 1; i <= options.radius; ++i) {
            s32 isq = i*i;
            if (i < size.x) {
                s32 lim = min(i - 1, (s32)size.y);
                for (s32 j = -lim; j <= lim; ++j) {
                    offsets.data[offsets.count + 0] = (v2i)v2s{-i,j};
                    offsets.data[offsets.count + 1] = (v2i)v2s{+i,j};
                    offsets.count += (isq + j*j <= rsq) << 1;
                }
            }
            if (i < size.y) {
                s32 lim = min(i, (s32)size.x);
                for (s32 j = -lim; j <= lim; ++j) {
                    offsets.data[offsets.count + 0] = (v2i)v2s{j,-i};
                    offsets.data[offsets.count + 1] = (v2i)v2s{j,+i};
                    offsets.count += (isq + j*j <= rsq) << 1;
                }
            }
        }

        quick_sort(offsets, [](auto o) { return o.x*o.x + o.y*o.y; });

        umm next_time_starting_from = 0;
        
        auto update_starting_index = [&] (umm current_index, smm radius_delta) {
            f32 current_radius = tl::sqrt((f32)current_index) / pi;
            f32 smaller_radius = floor(max(0.0f, current_radius - radius_delta));
            next_time_starting_from = floor_to_int(pi * pow2(smaller_radius));
        };
    
        for (smm iy = 0; iy < size.y; ++iy) {
            // TODO: could make this smarter
            next_time_starting_from = 0;
            for (smm ix = 0; ix < size.x; ++ix) {
                Pixel p = source_pixels[iy*source_stride_in_pixels + ix];

                if (options.should_be_dilated(p)) {
                    
                    v3f color_sum = {};
                    f32 factor_sum = {};

                    f32 closest_pixel_distance = 0;
                    for (auto &offset : offsets.skip(next_time_starting_from)) {
                        v2smm j = {
                            ix + offset.x,
                            iy + offset.y,
                        };

                        if (auto remapped = options.remap_pixel_position(j, size)) {
                            j = remapped.value();
                        } else {
                            continue;
                        }

                        auto t = source_pixels[j.y*source_stride_in_pixels + j.x];
                        if (!options.should_be_dilated(t)) {
                            f32 distance = options.distance(v2smm{ix, iy}, j, size);

                            f32 const max_distance = 1;

                            if (factor_sum == 0) {
                                closest_pixel_distance = distance;
                                color_sum += (v3f)t.xyz;
                                factor_sum += 1;
                            } else {
                                if (distance >= closest_pixel_distance + max_distance) {
                                    update_starting_index(index_of(offsets, &offset), 2);
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
    };

    if (options.smooth) {
        // NOTE: For some reason this runs EXTREMELY slow, like 5 times slower that s32.
        //       Hotspot was at `for (auto offset : offsets)`, assembly was not that much
        //       different from s32, just two regular moves were replaced with movxz/movsx.
        //       Have no idea why.
        //       
        //if (size.x <= (1<< 7) && size.y <= (1<< 7)) {
        //    return dilate_smooth_impl.operator()<s8>();
        //}
        if (size.x <= (1<<15) && size.y <= (1<<15)) {
            return dilate_smooth_impl.template operator()<s16>();
        }
        if (size.x <= (1<<31) && size.y <= (1<<31)) {
            return dilate_smooth_impl.template operator()<s32>();
        }
        return dilate_smooth_impl.template operator()<s64>();
    } else {
        // Jump flood

        List<v2u> src_positions, dst_positions;
        List<f32> src_distances, dst_distances;
        defer {
            free(src_positions);
            free(src_distances);
            free(dst_positions);
            free(dst_distances);
        };

        src_positions.reserve(size.x * size.y);
        src_distances.reserve(size.x * size.y);
        dst_positions.reserve(size.x * size.y);
        dst_distances.reserve(size.x * size.y);

        src_positions.count = size.x * size.y;
        src_distances.count = size.x * size.y;
        dst_positions.count = size.x * size.y;
        dst_distances.count = size.x * size.y;

        for (u32 y = 0; y < size.y; ++y)
        for (u32 x = 0; x < size.x; ++x) {
            if (options.should_be_dilated(source_pixels[y*source_stride_in_pixels + x])) {
                src_distances.data[y*size.x + x] = 
                dst_distances.data[y*size.x + x] = infinity<f32>;
            } else {
                src_positions.data[y*size.x + x] =
                dst_positions.data[y*size.x + x] = {x,y};
                src_distances.data[y*size.x + x] =
                dst_distances.data[y*size.x + x] = 0.0f;
            }
        }

        auto size_max_dim = max(size.x, size.y);
        auto pass_count = log2(size_max_dim - 1) + 1;
        for (umm pass_index = 0; pass_index < pass_count; ++pass_index) {
            for (smm y = 0; y < size.y; ++y)
            for (smm x = 0; x < size.x; ++x) {
                constexpr v2smm ds[] = {
                    {-1,-1},
                    {-1, 0},
                    {-1,+1},
                    { 0,-1},
                    { 0, 0}, // 0,0 has to be there, otherwise does't work. Not sure why.
                    { 0,+1},
                    {+1,-1},
                    {+1, 0},
                    {+1,+1},
                };
                for (auto d : ds){
                    v2smm src_p = v2smm{x,y} + (d << (pass_count - pass_index - 1));

                    if (auto remapped = options.remap_pixel_position(src_p, size)) {
                        src_p = remapped.value();
                    } else {
                        continue;
                    }
                    
                    f32 it_distance = src_distances.data[src_p.y*size.x + src_p.x];
                    if (it_distance < infinity<f32>) {
                        auto it_position = src_positions.data[src_p.y*size.x + src_p.x];
                        float l = options.distance((v2smm)it_position, v2smm{x, y}, size);
                        if (l < dst_distances.data[y*size.x + x]) {
                            dst_distances.data[y*size.x + x] = l;
                            dst_positions.data[y*size.x + x] = it_position;
                        }
                    }
                }
            }
            Swap(dst_positions, src_positions);
            Swap(dst_distances, src_distances);
        }

        
        for (u32 y = 0; y < size.y; ++y)
        for (u32 x = 0; x < size.x; ++x) {
            v2u p = {x,y};
            if (src_distances[y*size.x + x] != 0.0f) {
                p = src_positions[y*size.x + x];
            }
            destination_pixels[y*destination_stride_in_pixels + x] = V4(
                source_pixels[p.y*source_stride_in_pixels + p.x].xyz,
                options.keep_alpha ? 
                    source_pixels[y*source_stride_in_pixels + x].w :
                    (u8)255
            );
        }
    }
}

template <class Pixel, class DilateOptions = DefaultDilateOptions>
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
