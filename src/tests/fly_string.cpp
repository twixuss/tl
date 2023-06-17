#include <tl/fly_string.h>

using namespace tl;

void fly_string_test() {
    FlyString::init();
    auto str1 = FlyString(u8"Hello"s);
    auto str2 = FlyString(u8"Hello"s);
    assert(str1.data == str2.data);
}
