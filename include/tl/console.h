#pragma once

#include "system.h"

namespace TL {

TL_API void _print( char const *string, umm length);
TL_API void _print(wchar const *string, umm length);

template <class Char = char, class T>
inline void print(T const &value);

template <class Char = char, class ...Args>
inline void print(Char const *fmt, Args const &...args);

}

#include "common.h"
#include "string.h"

namespace TL {

template <class Char, class T>
inline void print(T const &value) {
	toString<Char>(value, [](Char const *string, umm length) {
		_print(string, length);
	});
}

template <class Char, class ...Args>
inline void print(Char const *fmt, Args const &...args) {
	StringBuilder<Char> builder;
	builder.appendFormat(fmt, args...);
	print(builder.get());
}

#ifdef TL_IMPL

#if OS_WINDOWS

static HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

void _print(char const *string, umm length) {
	DWORD charsWritten;
	WriteConsoleA(consoleOutput, string, (DWORD)length, &charsWritten, 0);
#ifdef TL_CONSOLE_DEBUG
	OutputDebugStringA(null_terminate(Span{string, length}).data());
#endif
}
void _print(wchar const *string, umm length) {
	DWORD charsWritten;
	WriteConsoleW(consoleOutput, string, (DWORD)length, &charsWritten, 0);
#ifdef TL_CONSOLE_DEBUG
	OutputDebugStringW(null_terminate(Span{string, length}).data());
#endif
}

#else

#endif

#endif

}
