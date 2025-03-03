#pragma once
#include "string.h"
#include "hash_map.h"

#ifndef TL_FLY_STRING_ALLOCATOR
#define TL_FLY_STRING_ALLOCATOR DefaultAllocator
#endif

namespace tl {

struct FlyString {
    inline FlyString() = default;

    template <class Size>
    inline FlyString(Span<utf8, Size> span) {
        auto found = storage.find(span);
        if (found) {
            *found.value += 1;
            data = found.key->data;
        } else {
            auto buffer = storage.allocator.allocate<utf8>(span.count + 1);
            memcpy(buffer, span.data, span.count);
            buffer[span.count] = 0;
            storage.get_or_insert({buffer, span.count}) = 1;
            data = buffer;
        }
    }

    inline bool operator==(FlyString that) const {
        return data == that.data;
    }
    inline bool operator==(char const *that) const {
        return as_span((char *)data) == as_span(that);
    }

    inline umm count() const {
        if (!data)
            return 0;
        return as_span(data).count;
    }

    template <class Size>
    inline operator Span<utf8, Size>() const {
        return (Span<utf8, Size>)as_span(data);
    }

    template <class Size=umm>
    inline Span<utf8, Size> span() const {
        return (Span<utf8, Size>)as_span(data);
    }

    inline bool is_empty() const {
        return !data || *data == 0;
    }

    utf8 const *data = 0;

    static HashMap<Span<utf8>, umm, DefaultHashTraits<Span<utf8>>, TL_FLY_STRING_ALLOCATOR> storage;
    static void init() {
        construct(storage);
    }
};

#ifdef TL_IMPL
HashMap<Span<utf8>, umm, DefaultHashTraits<Span<utf8>>, TL_FLY_STRING_ALLOCATOR> FlyString::storage;
#endif

inline umm append(StringBuilder &builder, FlyString str) {
    return append(builder, as_span(str.data));
}

}

template <>
inline tl::u64 get_hash(tl::FlyString const &str) {
	return get_hash(str.span());
}
