#pragma once
#include "common.h"

namespace tl {

template <class T>
void merge_sort(Span<T> span, Span<T> out, auto less) {
    assert(span.count == out.count, "src and dst spans must be the same size");
    assert(span.begin() >= out.end() || out.begin() >= span.end(), "src and dst spans must not intersect");

    switch (span.count) {
        case 0:
            return;
        case 1:
            *out.data = *span.data;
            return;
    }

    auto mid = midpoint(span.begin(), span.end());
    merge_sort(Span(span.begin(), mid), less);
    merge_sort(Span(mid, span.end()), less);

    auto a = span.begin();
    auto b = mid;
    auto d = out.begin();

    while ((a < mid) & (b < span.end())) *d++ = less(*a, *d) ? *a++ : *b++;
    while (a < mid) *d++ = *a++;
    while (b < span.end())*d++ = *b++;
}

}
