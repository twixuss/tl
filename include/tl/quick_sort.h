#pragma once
#include "common.h"

namespace tl {

namespace quick_sort_pivot {
	auto middle = []<class T>(Span<T> span) -> T * { return midpoint(span.begin(), span.end()); };
	auto first  = []<class T>(Span<T> span) -> T * { return span.begin(); };
	auto last   = []<class T>(Span<T> span) -> T * { return span.end() - 1; };
	auto median_of_three = []<class T>(Span<T> span) -> T * {
        auto a = span.begin();
        auto b = midpoint(span.begin(), span.end());
        auto c = span.end();
        int mask = ((a < b) << 2) | ((a < c) << 1) | (b < c);
        switch (mask) {
            default:
            case 0b011: // b a c
            case 0b100: // c a b
                return a;
            case 0b111: // a b c
            case 0b000: // c b a
                return b;
            case 0b110: // a c b
            case 0b001: // b c a
                return c;
        }
    };
	auto random = []<class T>(Span<T> span) -> T * {
        umm a = (umm)span.begin();
        umm b = (umm)span.end();
        a = (a * 0xf5fef3918801d069) ^ (b * 0x7f18e56337fdbcfd);
        //a *= 0xd0d313414ef5a0c5;
        a >>= 32;
        return &span.data[a % span.count];
    };
};

template <class T>
void quick_sort(Span<T> span, auto get_pivot, APredicate<T, T> auto less)
    requires requires(T t, Span<T> span) {
        { get_pivot(span) } -> std::same_as<T *>;
    }
{
    while (1) {
        switch (span.count) {
            case 0:
            case 1:
                return;
            case 2: {
                if (!less(span[0], span[1])) {
                    Swap(span[0], span[1]);
                }
                return;
            }
        }

        auto p = get_pivot(span);
        auto pivot = *p;
        Swap(*p, span.end()[-1]);

        auto mid = span.begin();
        for (auto i = span.begin(); i < span.end() - 1; i++) {
            if (less(*i, pivot)) {
                Swap(*i, *mid);
                mid++;
            }
        }
        Swap(*mid, span.end()[-1]);

        quick_sort(Span<T>{span.begin(), mid}, get_pivot, less);
        span = {mid + 1, span.end()};
    }
}
template <class T, class Allocator>
void quick_sort(Span<T> span, APredicate<T, T> auto less) {
	quick_sort(span, quick_sort_pivot::middle, less);
}
template <class T>
void quick_sort(Span<T> span, auto selector) requires requires(T a) { selector(a) < selector(a); } {
	quick_sort(span, quick_sort_pivot::middle, [&](T a, T b) { return selector(a) < selector(b); });
}
template <class T>
void quick_sort(Span<T> span) {
	quick_sort(span, quick_sort_pivot::middle, [](T a, T b) { return a < b; });
}
template <class T, umm size>
void quick_sort(T (&array)[size], auto ...args) {
	quick_sort(array_as_span(array), args...);
}

}

#define TL_QUICK_SORT_H_INCLUDED

#ifdef TL_LIST_H_INCLUDED
#include "list-quick_sort.h"
#endif
