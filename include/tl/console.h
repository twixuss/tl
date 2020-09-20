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
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	while (str.size()) {
		DWORD charsToWrite = (DWORD)min(str.size(), 0xFFFFFFFF);
		WriteConsoleA(handle, str.data(), charsToWrite, &charsWritten, 0);
		str._begin += charsToWrite;
	}
}
#else
#endif
#endif

}
