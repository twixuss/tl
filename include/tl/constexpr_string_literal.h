#pragma once

// Modified version of https://godbolt.org/z/Yaxsv17dW

namespace tl {

template<umm capacity>
struct CSL {
    static constexpr umm count = capacity - 1;
    char data[capacity];
    constexpr CSL(const char (&init)[capacity]) { 
        for (umm i = 0; i < capacity; ++i) {
            data[i] = init[i];
        }
    }
};

template<auto str>
struct CSLId {
    static constexpr const char* data = str.data;
    static constexpr umm count = str.count;
    static constexpr Span<char> span = {str.data, str.count};
    template <auto other_str>
    constexpr bool operator==(const CSLId<other_str> &) const { return false; }
    template <>
    constexpr bool operator==(const CSLId<str> &) const { return true; }
};

template<CSL str>
constexpr auto operator"" _cs() {
    return CSLId<str>{};
}

}