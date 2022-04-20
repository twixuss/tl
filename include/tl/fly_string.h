#pragma once
#include "string.h"
#include "hash_map.h"

namespace tl {

struct FlyString {
    inline static FlyString create(Span<utf8> span) {
        auto found = storage.find(span);
        if (found) {
            found->value += 1;
            return {found->key.data};
        } else {
            auto buffer = storage.allocator.allocate<utf8>(span.count + 1);
            memcpy(buffer, span.data, span.count);
            buffer[span.count] = 0;
            storage.get_or_insert({buffer, span.count}) = 1;
            return {buffer};
        }
    }

    utf8 const *data;

    static HashMap<Span<utf8>, umm> storage;
};

#ifdef TL_IMPL
HashMap<Span<utf8>, umm> FlyString::storage = {default_allocator};
#endif

}
