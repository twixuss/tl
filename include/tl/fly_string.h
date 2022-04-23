#pragma once
#include "string.h"
#include "hash_map.h"

namespace tl {

struct FlyString {
    inline FlyString() = default;

    template <class Size>
    inline FlyString(Span<utf8, Size> span) {
        auto found = storage.find(span);
        if (found) {
            found->value += 1;
            data = found->key.data;
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
        return strcmp((char *)data, that) == 0;
    }

    inline umm count() const {
        return as_span(data).count;
    }

    template <class Size>
    inline operator Span<utf8, Size>() const {
        return (Span<utf8, Size>)as_span(data);
    }

    inline bool is_empty() const {
        return !data || *data == 0;
    }

    utf8 const *data = 0;

    static HashMap<Span<utf8>, umm> storage;
};

#ifdef TL_IMPL
HashMap<Span<utf8>, umm> FlyString::storage = {default_allocator};
#endif

inline umm append(StringBuilder &builder, FlyString str) {
    return append(builder, as_span(str.data));
}

}

template <>
inline tl::u64 get_hash(tl::FlyString str) {
    auto c = str.data;
	tl::u64 result = 0xdeadc0debabeface;
	while (*c) {
		result = tl::rotate_left(result, 7) ^ *c++;
	}
	return result;
}
