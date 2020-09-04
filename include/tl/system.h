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

#define FORCEINLINE			__forceinline
#define NOINLINE			__declspec(noinline)
#define DEBUG_BREAK			::__debugbreak()
#define WRITE_BARRIER		::_WriteBarrier()
#define READ_BARRIER		::_ReadBarrier()
#define READ_WRITE_BARRIER	::_ReadWriteBarrier()

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

#define FORCEINLINE	//__attribute__((always_inline))
#define DEBUG_BREAK ::__builtin_trap()
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
	#ifdef TL_IMPL
		#define TL_API
	#else
		#define TL_API extern
	#endif
#endif
