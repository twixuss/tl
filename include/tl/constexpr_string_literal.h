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

// Some shit for standard compliance. You can't specialize templates in class scope for no fucking reason.
template<auto str1, auto str2>
struct CSLIdEq {
    inline static constexpr bool value = false;
};
template<auto str>
struct CSLIdEq<str, str> {
    inline static constexpr bool value = true;
};

template<auto str>
struct CSLId {
    static constexpr const char* data = str.data;
    static constexpr umm count = str.count;
    static constexpr Span<char> span = {str.data, str.count};
    template <auto other_str>
    constexpr bool operator==(const CSLId<other_str> &) const {
        return CSLIdEq<str, other_str>::value;
    }
};

template<CSL str>
constexpr auto operator"" _cs() {
    return CSLId<str>{};
}

}