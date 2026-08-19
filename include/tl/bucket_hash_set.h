#pragma once
#include "bucket_hash_map.h"

namespace tl {

template <class Value, class Traits = DefaultHashTraits<Value>, class Allocator = Allocator>
using BucketHashSet = BucketHashMap<Value, Empty, Traits, Allocator>;


}
