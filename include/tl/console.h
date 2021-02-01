#pragma once

#include "system.h"

namespace TL {

TL_API void _print( char const *string, umm length);
TL_API void _print(wchar const *string, umm length);
TL_API void clear_console();

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
	to_string<Char>(value, [](Span<Char const> span) {
		_print(span.data(), span.size());
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
#ifdef TL_DEBUG
	OutputDebugStringA(null_terminate(Span{string, length}).data());
#endif
}
void _print(wchar const *string, umm length) {
	DWORD charsWritten;
	WriteConsoleW(consoleOutput, string, (DWORD)length, &charsWritten, 0);
#ifdef TL_DEBUG
	OutputDebugStringW(null_terminate(Span{string, length}).data());
#endif
}

void clear_console() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(consoleOutput, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBufferA(consoleOutput, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(consoleOutput, csbi.dwCursorPosition);
}

#else

#endif

#endif

}
