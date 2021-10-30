#pragma once


//
// Compiler
//

#define COMPILER_MSVC 0
#define COMPILER_GCC  0

#if defined __GNUG__
	#undef COMPILER_GCC
	#define COMPILER_GCC 1
#elif defined _MSC_VER
	#undef COMPILER_MSVC
	#define COMPILER_MSVC 1
#else
	#pragma message "TL: Unresolved compiler"
#endif


//
// Operating system
//

#define OS_WINDOWS 0
#define OS_LINUX   0

#if defined _WIN32 || defined _WIN64
	#undef OS_WINDOWS
	#define OS_WINDOWS 1
#else
	#undef OS_LINUX
	#define OS_LINUX 1
#endif


//
// Architectural features
//

#define ARCH_X86 0
#define ARCH_X64 0

#if COMPILER_MSVC
	#if defined _M_IX86
		#undef ARCH_X86
		#define ARCH_X86 1
	#elif defined _M_X64
		#undef ARCH_X64
		#define ARCH_X64 1
	#endif
#elif COMPILER_GCC
	#if defined _X86_
		#undef ARCH_X86
		#define ARCH_X86 1
	#elif defined __x86_64__
		#undef ARCH_X64
		#define ARCH_X64 1
	#endif
#endif

#if COMPILER_MSVC || COMPILER_GCC
	#ifndef ARCH_AVX
		#ifdef __AVX__
			#undef ARCH_AVX
			#define ARCH_AVX 1
		#else
			#undef ARCH_AVX
			#define ARCH_AVX 0
		#endif
	#endif
	#ifndef ARCH_AVX2
		#ifdef __AVX2__
			#undef ARCH_AVX2
			#define ARCH_AVX2 1
		#else
			#undef ARCH_AVX2
			#define ARCH_AVX2 0
		#endif
	#endif
	#ifndef ARCH_AVX512F
		#ifdef __AVX512F__
			#undef ARCH_AVX512F
			#define ARCH_AVX512F 1
		#else
			#undef ARCH_AVX512F
			#define ARCH_AVX512F 0
		#endif
	#endif
	#ifndef ARCH_AVX512VL
		#ifdef __AVX512VL__
			#undef ARCH_AVX512VL
			#define ARCH_AVX512VL 1
		#else
			#undef ARCH_AVX512VL
			#define ARCH_AVX512VL 0
		#endif
	#endif
	#ifndef ARCH_AVX512DQ
		#ifdef __AVX512DQ__
			#undef ARCH_AVX512DQ
			#define ARCH_AVX512DQ 1
		#else
			#undef ARCH_AVX512DQ
			#define ARCH_AVX512DQ 0
		#endif
	#endif
#endif

#ifndef ARCH_LZCNT
#define ARCH_LZCNT 0
#endif

#ifndef ARCH_FMA
#define ARCH_FMA ARCH_AVX2
#endif


//
// Interface
//

#if COMPILER_MSVC
	#define TL_DLL_IMPORT __declspec(dllimport)
	#define TL_DLL_EXPORT __declspec(dllexport)
#elif COMPILER_GCC
#endif

#ifdef TL_API_DLL
	#ifdef TL_IMPL
		#define TL_API TL_DLL_EXPORT
	#else
		#define TL_API TL_DLL_IMPORT
	#endif
#else
	#define TL_API
#endif


//
// Things
//

#define STRINGIZE_(x) #x
#define STRINGIZE(x)  STRINGIZE_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y)  CONCAT_(x, y)

#if COMPILER_MSVC
	#define forceinline   __forceinline
	#define no_inline     __declspec(noinline)
	#define debug_break() ::__debugbreak()
#elif COMPILER_GCC
	#define forceinline   __attribute__((always_inline))
	#define debug_break() ::__builtin_trap()
#endif


namespace tl {

template <bool v, class T, class F> struct ConditionalT { using Type = T; };
template <class T, class F> struct ConditionalT<false, T, F> { using Type = F; };
template <bool v, class T, class F> using Conditional = typename ConditionalT<v, T, F>::Type;

#if COMPILER_MSVC
using s8  = signed __int8;
using s16 = signed __int16;
using s32 = signed __int32;
using s64 = signed __int64;
using u8  = unsigned __int8;
using u16 = unsigned __int16;
using u32 = unsigned __int32;
using u64 = unsigned __int64;
#else
using s8  = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long long;
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
#endif

using f32 = float;
using f64 = double;
using f80 = long double;
using b32 = s32;
using b64 = s64;

#if ARCH_X64
using umm = u64;
using smm = s64;
#else
using umm = u32;
using smm = s32;
#endif

static_assert(sizeof(s8)  == 1);
static_assert(sizeof(s16) == 2);
static_assert(sizeof(s32) == 4);
static_assert(sizeof(s64) == 8);
static_assert(sizeof(smm) == sizeof(void *));

static_assert(sizeof(u8)  == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(umm) == sizeof(void *));

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

using ascii = char;
using utf8  = char8_t;
using utf16 = char16_t;
using utf32 = char32_t;



//
// Really?
//
using slong = signed long;
using ulong = unsigned long;
using wchar = wchar_t;

using slong_s = Conditional<sizeof(slong) == 4, s32, s64>;
using ulong_s = Conditional<sizeof(ulong) == 4, u32, u64>;
using wchar_s = Conditional<sizeof(wchar) == 2, utf16, utf32>;


#if OS_WINDOWS
static_assert(sizeof(utf16) == sizeof(wchar));
#endif

}

#if TL_DEBUG
#if TL_DEBUG != 0 && TL_DEBUG != 1
#error TL_DEBUG defined to wrong value
#endif
#else
#define TL_DEBUG 1
#endif

#ifdef TL_IMPL
	#if OS_WINDOWS
		#pragma push_macro("OS_WINDOWS")
		#undef OS_WINDOWS
		#define NOMINMAX
		#include <Windows.h>
		#pragma pop_macro("OS_WINDOWS")
	#else
		#include <pthread.h>
	#endif
#endif // TL_IMPL
