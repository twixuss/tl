#pragma once
#include "hash.h"
#include "array.h"

template <class T, tl::umm count>
inline constexpr tl::u64 get_hash(tl::Array<T, count> const &v) {
    using namespace tl;

    u64 hash = 0x54f8'71a8'd965'1ced;
    for (umm i = 0; i < count; ++i) {
        hash ^= get_hash(v.data[i]);
        hash *= 0x9e37'79b9'7f4a'7c15;
    }
    return hash;
}
