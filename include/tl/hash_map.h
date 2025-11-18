#pragma once

#include "bucket_hash_map.h"

namespace tl {

template <class Key, class Value, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
using HashMap = BucketHashMap<Key, Value, Traits, Allocator>;

}
