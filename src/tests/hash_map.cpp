#pragma once
#include <tl/hash_map.h>
#include <tl/console.h>
using namespace tl;

void hash_map_test() {
    ContiguousHashMap<int, float> map;

    map.get_or_insert(1337) = 2.34f;
    map.get_or_insert(4242) = 1.34f;
    map.get_or_insert(2020) = 3.34f;
    map.get_or_insert(9999) = 5.34f;

    assert(map.find(1337));
    assert(map.find(4242));
    assert(map.find(2020));
    assert(map.find(9999));
    assert(!map.find(1));
    assert(!map.find(4));
    assert(!map.find(2));
    assert(!map.find(9));

    for_each(map, [&](int key, float value) {
        print("{}: {}\n", key, value);
    });
}
