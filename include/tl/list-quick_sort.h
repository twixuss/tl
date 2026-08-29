#pragma once
#include "list.h"
#include "quick_sort.h"

namespace tl {
    
// Quick sort overloads that use scratch allocated memory and a loop instead of recursion.
// Use to avoid stack overflow on big data.
template <class T, class Allocator>
void quick_sort(Span<T> initial_span, List<u8, Allocator> &temp_storage, auto get_pivot, auto less) requires requires(T a, T b) { { less(a, b) } -> std::same_as<bool>; } {

	using Frame = Span<T>;

	auto add = [&](Frame frame) {
		temp_storage.add(value_as_bytes(frame));
	};
	auto pop = [&]() -> Frame {
		Frame frame;
		temp_storage.count -= sizeof(Frame);
		memcpy(&frame, temp_storage.end(), sizeof(Frame));
		return frame;
	};

	temp_storage.clear();

	add(initial_span);

	do {
		auto span = pop();
			
		switch (span.count) {
			case 0:
			case 1:
				continue;
			case 2: {
				if (!less(span[0], span[1])) {
					Swap(span[0], span[1]);
				}
				continue;
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

		add(Span<T>{mid + 1, span.end()});
		add(Span<T>{span.begin(), mid});
	} while(temp_storage.count);
}
template <class T, class Allocator>
void quick_sort(Span<T> span, List<u8, Allocator> &temp_storage, APredicate<T, T> auto less) {
	quick_sort(span, temp_storage, quick_sort_pivot::middle, less);
}
template <class T, class Allocator>
void quick_sort(Span<T> span, List<u8, Allocator> &temp_storage, auto selector) requires requires(T a) { selector(a) < selector(a); } {
	quick_sort(span, temp_storage, quick_sort_pivot::middle, [&](T a, T b) { return selector(a) < selector(b); });
}
template <class T, class Allocator>
void quick_sort(Span<T> span, List<u8, Allocator> &temp_storage) {
	quick_sort(span, temp_storage, quick_sort_pivot::middle, [](T a, T b) { return a < b; });
}

}
