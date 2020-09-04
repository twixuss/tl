#pragma once
#include "system.h"
#include "string.h"

namespace TL {

void _print(Span<char const>);

template <class ...Args>
void print(char const *fmt, Args const &...args) {
	StringBuilder<> builder;
	builder.appendFormat(fmt, args...);
	_print(builder.get());
}

#ifdef TL_IMPL
#if OS_WINDOWS
void _print(Span<char const> str) {
	DWORD charsWritten;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.data(), str.size(), &charsWritten, 0);
}
#else
#endif
#endif

}
