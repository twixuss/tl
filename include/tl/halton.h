#pragma once
#include "common.h"
#include "bits.h"

namespace tl {

// Halton sequence can be used to generate random points in a unit range.
// The coverage is more even than of prngs, meaning it does not suffer
// of clumping.
// NOTE that for each dimension base must be different and coprime.
//      E.g for 2 dimensions bases 2 and 3 are fine, but 2 and 4 are not.
// 
// To generate a halton number at index I, you reverse the digits of I in specified base
// and place them after the decimal point. For example in base 2:
// # #binary #reversed  res2  res10
// 0       0         0  0.0   0
// 1       1         1  0.1   0.5
// 2      10        01  0.01  0.25
// 3      11        11  0.11  0.75
// 4     100       001  0.001 0.125
// 5     101       101  0.101 0.625
// 6     110       011  0.011 0.375
// ...

constexpr f32 halton_number(u32 base, u32 index) {
    switch (base) {
        case 2: return normalize_range_f32<f32>(reverse_bits(index));
        case 4: return normalize_range_f32<f32>(reverse_bit_pairs(index));
        case 16: return normalize_range_f32<f32>(reverse_nibbles(index));
        case 256: return normalize_range_f32<f32>(reverse_bytes(index));
    }

    f32 result = 0.0f;
    f32 f = 1.0f;
    while (index > 0) {
        f /= base;
        result += f * (index % base);
        index /= base;
    }
    return result;
}

}
