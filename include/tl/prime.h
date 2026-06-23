#pragma once
#include "common.h"

namespace tl {
    
forceinline constexpr bool is_prime(u64 n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0) return false;

    for (u64 i = 7; i * i <= n; i += 30) {
        if (n % (i +  0) == 0 ||
            n % (i +  4) == 0 ||
            n % (i +  6) == 0 || 
            n % (i + 10) == 0 ||
            n % (i + 12) == 0 ||
            n % (i + 16) == 0 || 
            n % (i + 22) == 0 ||
            n % (i + 24) == 0)
        {
            return false;
        }
    }
    return true;
}

forceinline constexpr u64 next_prime(u64 x) {
	while (!is_prime(x)) ++x;
    return x;
}

}