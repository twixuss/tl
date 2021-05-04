#include "../include/tl/compiler.h"
#include "../include/tl/console.h"
using namespace TL;

struct EmptyStruct {
};

namespace N0 {
namespace N1 {
struct EmptyStruct {
};
}
}

struct Parent {
	struct Child {
	};
};

template <class T>
struct Templated {
	struct Child {
	};
	template <class T>
	struct TemplatedChild {
	};
};

template <class T, class U>
struct Templated2 {
	struct Child {
	};
};

auto fn00(     ){return __FUNCDNAME__;}
auto fn01(u8   ){return __FUNCDNAME__;}
auto fn02(u16  ){return __FUNCDNAME__;}
auto fn03(u32  ){return __FUNCDNAME__;}
auto fn04(u64  ){return __FUNCDNAME__;}
auto fn05(s8   ){return __FUNCDNAME__;}
auto fn06(s16  ){return __FUNCDNAME__;}
auto fn07(s32  ){return __FUNCDNAME__;}
auto fn08(s64  ){return __FUNCDNAME__;}
auto fn09(bool ){return __FUNCDNAME__;}
auto fn10(ascii){return __FUNCDNAME__;}
auto fn11(utf8 ){return __FUNCDNAME__;}
auto fn12(utf16){return __FUNCDNAME__;}
auto fn13(utf32){return __FUNCDNAME__;}
auto fn14(f32  ){return __FUNCDNAME__;}
auto fn15(f64  ){return __FUNCDNAME__;}
auto fn16(void ){return __FUNCDNAME__;}
auto fn17(std::nullptr_t){return __FUNCDNAME__;}
auto fn18(slong){return __FUNCDNAME__;}
auto fn19(ulong){return __FUNCDNAME__;}
auto fn20(wchar){return __FUNCDNAME__;}
auto fn21(long double){return __FUNCDNAME__;}
auto fn22(u8, u8){return __FUNCDNAME__;}
auto fn23(u8, u8, u8){return __FUNCDNAME__;}
auto fn24(std::nullptr_t, std::nullptr_t){return __FUNCDNAME__;}
auto fn25(std::nullptr_t, std::nullptr_t, std::nullptr_t){return __FUNCDNAME__;}
auto fn26(::EmptyStruct){return __FUNCDNAME__;}
auto fn27(TL::EmptyStruct){return __FUNCDNAME__;}
auto fn28(N0::N1::EmptyStruct){return __FUNCDNAME__;}
auto fn29(Parent::Child){return __FUNCDNAME__;}
auto fn30(Parent::Child, Parent::Child){return __FUNCDNAME__;}
auto fn31(Templated<u8>){return __FUNCDNAME__;}
auto fn32(Templated2<u8, s8>){return __FUNCDNAME__;}
auto fn33(Templated<u8>::Child){return __FUNCDNAME__;}
auto fn34(Templated2<u8, s8>::Child){return __FUNCDNAME__;}
auto fn35(Templated<u8>::TemplatedChild<s8>){return __FUNCDNAME__;}

void compiler_test() {
	scoped_allocator(temporary_allocator);

	bool failed = false;

#define TEST(func, expected)\
	{\
		auto demangled = demangle(func);\
		if (demangled != expected) {\
			if (!failed) print('\n'); \
			print("Result of demangle is '%'. Expected '%'\n", demangled, expected);\
			failed = true;\
		}\
	}

	TEST(fn00( ), "fn00()"s);
	TEST(fn01(0), "fn01(u8)"s);
	TEST(fn02(0), "fn02(u16)"s);
	TEST(fn03(0), "fn03(u32)"s);
	TEST(fn04(0), "fn04(u64)"s);
	TEST(fn05(0), "fn05(s8)"s);
	TEST(fn06(0), "fn06(s16)"s);
	TEST(fn07(0), "fn07(s32)"s);
	TEST(fn08(0), "fn08(s64)"s);
	TEST(fn09(0), "fn09(bool)"s);
	TEST(fn10(0), "fn10(ascii)"s);
	TEST(fn11(0), "fn11(utf8)"s);
	TEST(fn12(0), "fn12(utf16)"s);
	TEST(fn13(0), "fn13(utf32)"s);
	TEST(fn14(0), "fn14(f32)"s);
	TEST(fn15(0), "fn15(f64)"s);
	TEST(fn16( ), "fn16()"s);
	TEST(fn17(0), "fn17(nullptr_t)"s);
	TEST(fn18(0), "fn18(slong)"s);
	TEST(fn19(0), "fn19(ulong)"s);
	TEST(fn20(0), "fn20(wchar)"s);
	TEST(fn21(0), "fn21(f80)"s);
	TEST(fn22(0,0), "fn22(u8, u8)"s);
	TEST(fn23(0,0,0), "fn23(u8, u8, u8)"s);
	TEST(fn24(0,0), "fn24(nullptr_t, nullptr_t)"s);
	TEST(fn25(0,0,0), "fn25(nullptr_t, nullptr_t, nullptr_t)"s);
	TEST(fn26({}), "fn26(EmptyStruct)"s);
	TEST(fn27({}), "fn27(TL::EmptyStruct)"s);
	TEST(fn28({}), "fn28(N0::N1::EmptyStruct)"s);
	TEST(fn29({}), "fn29(Parent::Child)"s);
	TEST(fn30({},{}), "fn30(Parent::Child, Parent::Child)"s);
	TEST(fn31({}), "fn31(Templated<u8>)"s);
	TEST(fn32({}), "fn32(Templated2<u8, s8>)"s);
	TEST(fn33({}), "fn33(Templated<u8>::Child)"s);
	TEST(fn34({}), "fn34(Templated2<u8, s8>::Child)"s);
	TEST(fn35({}), "fn35(Templated<u8>::TemplatedChild<s8>)"s);

	if (failed) {
		assert(false);
	}
}
