#include "contiguous_hash_set.h"

namespace tl {

template <class Key, class Traits = DefaultHashTraits<Key>, class Allocator = Allocator>
using HashSet = ContiguousHashSet<Key, Traits, Allocator>;

}