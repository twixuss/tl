#pragma once
#include "string.h"
#include "hash_map.h"
#include "thread.h"

#ifndef TL_PROTECTED_FLY_STRING_ALLOCATOR
#define TL_PROTECTED_FLY_STRING_ALLOCATOR DefaultAllocator
#endif

#ifndef TL_PROTECTED_FLY_STRING_LOCK
#define TL_PROTECTED_FLY_STRING_LOCK OsLock
#endif

namespace tl {

struct ProtectedFlyString {
    inline ProtectedFlyString() = default;

    template <class Size>
    inline ProtectedFlyString(Span<utf8, Size> span) {
        scoped(lock);

        auto found = storage.find(span);
        if (found) {
            found->value += 1;
            data = found->key.data;
        } else {
            auto buffer = storage.buffer.allocator.allocate<utf8>(span.count + 1);
            memcpy(buffer, span.data, span.count);
            buffer[span.count] = 0;
            storage.get_or_insert({buffer, span.count}) = 1;
            data = buffer;
        }
    }

    inline bool operator==(ProtectedFlyString that) const {
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

    static ContiguousHashMap<Span<utf8>, umm, DefaultHashTraits<Span<utf8>>, TL_PROTECTED_FLY_STRING_ALLOCATOR> storage;
    static TL_PROTECTED_FLY_STRING_LOCK lock;

    static void init() {
        construct(storage);
    }
};

#ifdef TL_IMPL
ContiguousHashMap<Span<utf8>, umm, DefaultHashTraits<Span<utf8>>, TL_PROTECTED_FLY_STRING_ALLOCATOR> ProtectedFlyString::storage;
TL_PROTECTED_FLY_STRING_LOCK ProtectedFlyString::lock;
#endif

inline umm append(StringBuilder &builder, ProtectedFlyString str) {
    return append(builder, as_span(str.data));
}

}

template <>
inline tl::u64 get_hash(tl::ProtectedFlyString const &str) {
	return get_hash(str.span());
}
