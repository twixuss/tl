#pragma once
#include "contiguous_hash_map.h"

namespace tl {

template <class Key, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
using ContiguousHashSet = ContiguousHashMap<Key, Empty, Traits, Allocator>;

}