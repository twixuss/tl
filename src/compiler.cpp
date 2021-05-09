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

static char const *mangled_name;
//auto fn00(     ){mangled_name=__FUNCDNAME__;}
//auto fn01(u8   ){mangled_name=__FUNCDNAME__;}
//auto fn02(u16  ){mangled_name=__FUNCDNAME__;}
//auto fn03(u32  ){mangled_name=__FUNCDNAME__;}
//auto fn04(u64  ){mangled_name=__FUNCDNAME__;}
//auto fn05(s8   ){mangled_name=__FUNCDNAME__;}
//auto fn06(s16  ){mangled_name=__FUNCDNAME__;}
//auto fn07(s32  ){mangled_name=__FUNCDNAME__;}
//auto fn08(s64  ){mangled_name=__FUNCDNAME__;}
//auto fn09(bool ){mangled_name=__FUNCDNAME__;}
//auto fn10(ascii){mangled_name=__FUNCDNAME__;}
//auto fn11(utf8 ){mangled_name=__FUNCDNAME__;}
//auto fn12(utf16){mangled_name=__FUNCDNAME__;}
//auto fn13(utf32){mangled_name=__FUNCDNAME__;}
//auto fn14(f32  ){mangled_name=__FUNCDNAME__;}
//auto fn15(f64  ){mangled_name=__FUNCDNAME__;}
//auto fn16(void ){mangled_name=__FUNCDNAME__;}
//auto fn17(std::nullptr_t){mangled_name=__FUNCDNAME__;}
//auto fn18(slong){mangled_name=__FUNCDNAME__;}
//auto fn19(ulong){mangled_name=__FUNCDNAME__;}
//auto fn20(wchar){mangled_name=__FUNCDNAME__;}
//auto fn21(long double){mangled_name=__FUNCDNAME__;}
//auto fn22(u8, u8){mangled_name=__FUNCDNAME__;}
//auto fn23(u8, u8, u8){mangled_name=__FUNCDNAME__;}
//auto fn24(std::nullptr_t, std::nullptr_t){mangled_name=__FUNCDNAME__;}
//auto fn25(std::nullptr_t, std::nullptr_t, std::nullptr_t){mangled_name=__FUNCDNAME__;}
//auto fn26(::EmptyStruct){mangled_name=__FUNCDNAME__;}
//auto fn27(TL::EmptyStruct){mangled_name=__FUNCDNAME__;}
//auto fn28(N0::N1::EmptyStruct){mangled_name=__FUNCDNAME__;}
//auto fn29(Parent::Child){mangled_name=__FUNCDNAME__;}
//auto fn30(Parent::Child, Parent::Child){mangled_name=__FUNCDNAME__;}
//auto fn31(Templated<u8>){mangled_name=__FUNCDNAME__;}
//auto fn32(Templated2<u8, s8>){mangled_name=__FUNCDNAME__;}
//auto fn33(Templated<u8>::Child){mangled_name=__FUNCDNAME__;}
//auto fn34(Templated2<u8, s8>::Child){mangled_name=__FUNCDNAME__;}
//auto fn35(Templated<u8>::TemplatedChild<s8>){mangled_name=__FUNCDNAME__;}
//char const *fn36(){mangled_name=__FUNCDNAME__;return{};}
//char *fn37(){mangled_name=(char *)__FUNCDNAME__;return{};}

u8 X;

u8 fn00(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr32 __cdecl      fn01(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr32 __cdecl      fn02(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr32 __cdecl      fn03(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr32 __cdecl      fn04(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr64 __cdecl      fn05(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr64 __cdecl      fn06(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr64 __cdecl      fn07(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr64 __cdecl      fn08(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr32 __stdcall    fn09(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr32 __stdcall    fn10(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr32 __stdcall    fn11(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr32 __stdcall    fn12(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr64 __stdcall    fn13(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr64 __stdcall    fn14(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr64 __stdcall    fn15(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr64 __stdcall    fn16(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr32 __fastcall   fn17(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr32 __fastcall   fn18(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr32 __fastcall   fn19(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr32 __fastcall   fn20(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr64 __fastcall   fn21(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr64 __fastcall   fn22(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr64 __fastcall   fn23(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr64 __fastcall   fn24(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr32 __vectorcall fn25(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr32 __vectorcall fn26(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr32 __vectorcall fn27(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr32 __vectorcall fn28(){mangled_name=__FUNCDNAME__;return{};}
u8                * __ptr64 __vectorcall fn29(){mangled_name=__FUNCDNAME__;return{};}
u8 const          * __ptr64 __vectorcall fn30(){mangled_name=__FUNCDNAME__;return{};}
u8       volatile * __ptr64 __vectorcall fn31(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr64 __vectorcall fn32(){mangled_name=__FUNCDNAME__;return{};}
u8 const volatile * __ptr64 __vectorcall fn33(){mangled_name=__FUNCDNAME__;return{};}

u8                & __cdecl      fn34(){mangled_name=__FUNCDNAME__;return X;}
u8 const          & __cdecl      fn35(){mangled_name=__FUNCDNAME__;return X;}
u8       volatile & __cdecl      fn36(){mangled_name=__FUNCDNAME__;return X;}
u8 const volatile & __cdecl      fn37(){mangled_name=__FUNCDNAME__;return X;}
u8                & __stdcall    fn38(){mangled_name=__FUNCDNAME__;return X;}
u8 const          & __stdcall    fn39(){mangled_name=__FUNCDNAME__;return X;}
u8       volatile & __stdcall    fn40(){mangled_name=__FUNCDNAME__;return X;}
u8 const volatile & __stdcall    fn41(){mangled_name=__FUNCDNAME__;return X;}
u8                & __fastcall   fn42(){mangled_name=__FUNCDNAME__;return X;}
u8 const          & __fastcall   fn43(){mangled_name=__FUNCDNAME__;return X;}
u8       volatile & __fastcall   fn44(){mangled_name=__FUNCDNAME__;return X;}
u8 const volatile & __fastcall   fn45(){mangled_name=__FUNCDNAME__;return X;}
u8                & __vectorcall fn46(){mangled_name=__FUNCDNAME__;return X;}
u8 const          & __vectorcall fn47(){mangled_name=__FUNCDNAME__;return X;}
u8       volatile & __vectorcall fn48(){mangled_name=__FUNCDNAME__;return X;}
u8 const volatile & __vectorcall fn49(){mangled_name=__FUNCDNAME__;return X;}
u8 const volatile & __vectorcall fn50(){mangled_name=__FUNCDNAME__;return X;}

void compiler_test() {
	scoped_allocator(temporary_allocator);

	bool failed = false;

#define TEST(func, expected)\
	{\
		func;\
		auto demangled = demangle(mangled_name);\
		print("Result of demangling % is %. Expected %\n", Format(mangled_name, align_left(20, ' ')), Format(demangled, align_left(20, ' ')), expected);\
		if (demangled != expected) {\
			failed = true;\
		}\
	}
	print('\n');
	//TEST(fn00( ), "fn00()"s);
	//TEST(fn01(0), "fn01(u8)"s);
	//TEST(fn02(0), "fn02(u16)"s);
	//TEST(fn03(0), "fn03(u32)"s);
	//TEST(fn04(0), "fn04(u64)"s);
	//TEST(fn05(0), "fn05(s8)"s);
	//TEST(fn06(0), "fn06(s16)"s);
	//TEST(fn07(0), "fn07(s32)"s);
	//TEST(fn08(0), "fn08(s64)"s);
	//TEST(fn09(0), "fn09(bool)"s);
	//TEST(fn10(0), "fn10(ascii)"s);
	//TEST(fn11(0), "fn11(utf8)"s);
	//TEST(fn12(0), "fn12(utf16)"s);
	//TEST(fn13(0), "fn13(utf32)"s);
	//TEST(fn14(0), "fn14(f32)"s);
	//TEST(fn15(0), "fn15(f64)"s);
	//TEST(fn16( ), "fn16()"s);
	//TEST(fn17(0), "fn17(nullptr_t)"s);
	//TEST(fn18(0), "fn18(slong)"s);
	//TEST(fn19(0), "fn19(ulong)"s);
	//TEST(fn20(0), "fn20(wchar)"s);
	//TEST(fn21(0), "fn21(f80)"s);
	//TEST(fn22(0,0), "fn22(u8, u8)"s);
	//TEST(fn23(0,0,0), "fn23(u8, u8, u8)"s);
	//TEST(fn24(0,0), "fn24(nullptr_t, nullptr_t)"s);
	//TEST(fn25(0,0,0), "fn25(nullptr_t, nullptr_t, nullptr_t)"s);
	//TEST(fn26({}), "fn26(EmptyStruct)"s);
	//TEST(fn27({}), "fn27(TL::EmptyStruct)"s);
	//TEST(fn28({}), "fn28(N0::N1::EmptyStruct)"s);
	//TEST(fn29({}), "fn29(Parent::Child)"s);
	//TEST(fn30({},{}), "fn30(Parent::Child, Parent::Child)"s);
	//TEST(fn31({}), "fn31(Templated<u8>)"s);
	//TEST(fn32({}), "fn32(Templated2<u8, s8>)"s);
	//TEST(fn33({}), "fn33(Templated<u8>::Child)"s);
	//TEST(fn34({}), "fn34(Templated2<u8, s8>::Child)"s);
	//TEST(fn35({}), "fn35(Templated<u8>::TemplatedChild<s8>)"s);
	//TEST(fn36(), "fn36()"s);
	//TEST(fn37(), "fn37()"s);
	TEST(fn00(), "fn00"s);
	TEST(fn01(), "fn01"s);
	TEST(fn02(), "fn02"s);
	TEST(fn03(), "fn03"s);
	TEST(fn04(), "fn04"s);
	TEST(fn05(), "fn05"s);
	TEST(fn06(), "fn06"s);
	TEST(fn07(), "fn07"s);
	TEST(fn08(), "fn08"s);
	TEST(fn09(), "fn09"s);
	TEST(fn10(), "fn10"s);
	TEST(fn11(), "fn11"s);
	TEST(fn12(), "fn12"s);
	TEST(fn13(), "fn13"s);
	TEST(fn14(), "fn14"s);
	TEST(fn15(), "fn15"s);
	TEST(fn16(), "fn16"s);
	TEST(fn17(), "fn17"s);
	TEST(fn18(), "fn18"s);
	TEST(fn19(), "fn19"s);
	TEST(fn20(), "fn20"s);
	TEST(fn21(), "fn21"s);
	TEST(fn22(), "fn22"s);
	TEST(fn23(), "fn23"s);
	TEST(fn24(), "fn24"s);
	TEST(fn25(), "fn25"s);
	TEST(fn26(), "fn26"s);
	TEST(fn27(), "fn27"s);
	TEST(fn28(), "fn28"s);
	TEST(fn29(), "fn29"s);
	TEST(fn30(), "fn30"s);
	TEST(fn31(), "fn31"s);
	TEST(fn32(), "fn32"s);
	TEST(fn33(), "fn33"s);
	TEST(fn34(), "fn34"s);
	TEST(fn35(), "fn35"s);
	TEST(fn36(), "fn36"s);
	TEST(fn37(), "fn37"s);
	TEST(fn38(), "fn38"s);
	TEST(fn39(), "fn39"s);
	TEST(fn40(), "fn40"s);
	TEST(fn41(), "fn41"s);
	TEST(fn42(), "fn42"s);
	TEST(fn43(), "fn43"s);
	TEST(fn44(), "fn44"s);
	TEST(fn45(), "fn45"s);
	TEST(fn46(), "fn46"s);
	TEST(fn47(), "fn47"s);
	TEST(fn48(), "fn48"s);
	TEST(fn49(), "fn49"s);
	TEST(fn50(), "fn50"s);

	if (failed) {
		print("Test failed\n");
		assert(false);
	}
}
