#pragma once
#include <tl/hash_map.h>
#include <tl/console.h>
using namespace tl;

struct Hashablabla {
    int value = 0;
};

template <>
u64 get_hash(Hashablabla x) {
    return 0;
}

bool operator==(Hashablabla a, Hashablabla b) {
    return a.value == b.value;
}

void hash_map_test() {
    {
        ContiguousHashMap<s64, f64> map;

        assert(map.count == 0);
        map.get_or_insert(1337) = 2.34f; assert(map.get_or_insert(1337) == 2.34f); assert(map.count == 1);
        map.get_or_insert(4242) = 1.34f; assert(map.get_or_insert(4242) == 1.34f); assert(map.count == 2);
        map.get_or_insert(2020) = 3.34f; assert(map.get_or_insert(2020) == 3.34f); assert(map.count == 3);
        map.get_or_insert(9999) = 5.34f; assert(map.get_or_insert(9999) == 5.34f); assert(map.count == 4);

        assert(*map.find(1337) == 2.34f);
        assert(*map.find(4242) == 1.34f);
        assert(*map.find(2020) == 3.34f);
        assert(*map.find(9999) == 5.34f);
        assert(!map.find(1));
        assert(!map.find(4));
        assert(!map.find(2));
        assert(!map.find(9));

        sizeof(map.cells.data[0]);

        for_each(map, [&](auto key, auto value) {
            print("{}: {}\n", key, value);
        });

        for (int i = 0; i < 1000; ++i) {
             map.get_or_insert(i) = i;
        }
        int x = 12;
    }
    {
        ContiguousHashMap<Hashablabla, int> map;
        assert(map.count == 0);
        map.get_or_insert({0}) = 0; assert(map.count == 1);
        map.get_or_insert({1}) = 1; assert(map.count == 2);
        map.get_or_insert({2}) = 2; assert(map.count == 3);
        map.get_or_insert({3}) = 3; assert(map.count == 4);
        assert(*map.find({0}) == 0);
        assert(*map.find({1}) == 1);
        assert(*map.find({2}) == 2);
        assert(*map.find({3}) == 3);
        assert(!map.find({4}));
        assert(!map.find({5}));
        assert(!map.find({6}));
        assert(!map.find({7}));

    }
}
