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
