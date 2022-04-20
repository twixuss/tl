#include <tl/fly_string.h>

using namespace tl;

void fly_string_test() {
    auto str1 = FlyString::create(u8"Hello"s);
    auto str2 = FlyString::create(u8"Hello"s);
    assert(str1.data == str2.data);
}
