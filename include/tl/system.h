#pragma once
#define OS_WINDOWS 0
#define OS_LINUX   0

#if defined _WIN32 || defined _WIN64
	#undef OS_WINDOWS
	#define OS_WINDOWS 1
#else
	#undef OS_LINUX
	#define OS_LINUX 1
#endif

#define COMPILER_MSVC 0
#define COMPILER_GCC  0

#if defined __GNUG__
	#undef  COMPILER_GCC
	#define COMPILER_GCC 1
#elif defined _MSC_VER
	#undef  COMPILER_MSVC
	#define COMPILER_MSVC 1
#else
	#pragma message "TL: Unresolved compiler"
#endif

#if COMPILER_MSVC

#define TL_DLL_IMPORT __declspec(dllimport)
#define TL_DLL_EXPORT __declspec(dllexport)

#define forceinline        __forceinline
#define no_inline           __declspec(noinline)
#define debug_break()      ::__debugbreak()
#define WRITE_BARRIER      ::_WriteBarrier()
#define READ_BARRIER       ::_ReadBarrier()
#define READ_WRITE_BARRIER ::_ReadWriteBarrier()

#define ARCH_X86 0
#define ARCH_X64 0

#if defined _M_IX86
#undef ARCH_X86
#define ARCH_X86 1
#elif defined _M_X64
#undef ARCH_X64
#define ARCH_X64 1
#endif

#elif COMPILER_GCC

#define forceinline //__attribute__((always_inline))
#define debug_break() ::__builtin_trap()
#if defined _X86_
#undef ARCH_X86
#define ARCH_X86 1
#elif defined __x86_64__
#undef ARCH_X64
#define ARCH_X64 1
#endif

#endif

// clang-format off
#if COMPILER_MSVC || COMPILER_GCC
	#ifndef ARCH_AVX
		#ifdef __AVX__
			#define ARCH_AVX 1
		#else
			#define ARCH_AVX 0
		#endif
	#endif
	#ifndef ARCH_AVX2 
		#ifdef __AVX2__
			#define ARCH_AVX2 1
		#else
			#define ARCH_AVX2 0
		#endif
	#endif
	#ifndef ARCH_AVX512F
		#ifdef __AVX512F__
			#define ARCH_AVX512F 1
		#else
			#define ARCH_AVX512F 0
		#endif
	#endif
	#ifndef ARCH_AVX512VL
		#ifdef __AVX512VL__
			#define ARCH_AVX512VL 1
		#else
			#define ARCH_AVX512VL 0
		#endif
	#endif
	#ifndef ARCH_AVX512DQ
		#ifdef __AVX512DQ__
			#define ARCH_AVX512DQ 1
		#else
			#define ARCH_AVX512DQ 0
		#endif
	#endif
#endif
// clang-format on

#if !(ARCH_X86 | ARCH_X64)
#error "Unresolved target architecture"
#endif

#define STRINGIZE_(x) #x
#define STRINGIZE(x)  STRINGIZE_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y)  CONCAT_(x, y)

#ifdef TL_API_DLL
	#ifdef TL_IMPL
		#define TL_API TL_DLL_EXPORT
	#else
		#define TL_API TL_DLL_IMPORT
	#endif
#else
	#define TL_API
#endif

#if OS_WINDOWS
#define TL_OS_INCLUDE(x) STRINGIZE(windows/x)
#else
#define TL_OS_INCLUDE(x) STRINGIZE(?????/x)
#endif

namespace TL {

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

using slong = signed long;
using ulong = unsigned long;

using slong_s = Conditional<sizeof(slong) == 4, s32, s64>;
using ulong_s = Conditional<sizeof(ulong) == 4, u32, u64>;

using f32   = float;
using f64   = double;
using b32   = s32;
using b64   = s64;
using wchar = wchar_t;

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
static_assert(sizeof(u8)  == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

}
