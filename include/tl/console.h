#pragma once

#include "common.h"
#include "string.h"

namespace TL {

enum Encoding {
	Encoding_ascii,
	Encoding_utf8,
	Encoding_utf16,
};

template <class Char> inline static constexpr Encoding encoding_from_type = Encoding_ascii;
template <> inline static constexpr Encoding encoding_from_type<wchar> = Encoding_utf16;

struct Printer {
	void (*func)(Span<u8> span, Encoding encoding, void *state);
	void *state;

	void operator()(Span<u8> span, Encoding encoding) {
		func(span, encoding, state);
	}
};

extern TL_API Printer console_printer;
extern TL_API Printer current_printer;

TL_API void set_console_encoding(Encoding);
TL_API void clear_console();

TL_API void print_to_console(Span<u8> string, Encoding encoding);

inline void print_to_console(Span<char > string) { print_to_console(as_bytes(string), Encoding_ascii); }
inline void print_to_console(Span<wchar> string) { print_to_console(as_bytes(string), Encoding_utf16); }

template <class Char = char, class T>
inline void print(T const &value) {
	StringBuilder builder;
	defer { free(builder); };

	append(builder, value);

	auto string = to_string(builder);
	defer { free(string); };

	current_printer(as_bytes(string), encoding_from_type<Char>);
}

template <class Char = char, class ...Args>
inline void print(Char const *fmt, Args const &...args) {
	StringBuilder builder;
	defer { free(builder); };

	append_format(builder, fmt, args...);

	auto string = to_string(builder);
	defer { free(string); };

	print<Char>(string);
}

TL_API void hide_console_window();
TL_API void show_console_window();

#ifdef TL_IMPL

Printer console_printer = {
	[](Span<u8> span, Encoding encoding, void *) { print_to_console(span, encoding); },
	0
};

#if OS_WINDOWS

static HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
static HWND console_window = GetConsoleWindow();

void print_to_console(Span<u8> span, Encoding encoding) {
	DWORD charsWritten;
	switch (encoding) {
		case Encoding_ascii: WriteConsoleA(console_output, span.data, (DWORD)span.size, &charsWritten, 0); break;
		case Encoding_utf8:  WriteConsoleA(console_output, span.data, (DWORD)span.size, &charsWritten, 0); break;
		case Encoding_utf16: WriteConsoleW(console_output, span.data, (DWORD)span.size, &charsWritten, 0); break;
	}
}

void clear_console() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT sr;
	COORD st;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(console_output, &csbi))
	{
		return;
	}

	// Scroll the rectangle of the entire buffer.
	sr.Left = 0;
	sr.Top = 0;
	sr.Right = csbi.dwSize.X;
	sr.Bottom = csbi.dwSize.Y;

	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	st.X = 0;
	st.Y = (SHORT)(0 - csbi.dwSize.Y);

	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.AsciiChar = ' ';
	fill.Attributes = csbi.wAttributes;

	// Do the scroll
	ScrollConsoleScreenBufferA(console_output, &sr, NULL, st, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(console_output, csbi.dwCursorPosition);
}

DWORD get_code_page(Encoding encoding) {
	switch (encoding) {
		case Encoding_ascii: return 1251;
		case Encoding_utf8: return 65001;
		case Encoding_utf16: return 1200;
	}
	invalid_code_path();
	return -1;
}

void set_console_encoding(Encoding encoding) {
	auto cp = get_code_page(encoding);
	SetConsoleOutputCP(cp);
	SetConsoleCP(cp);
}

void hide_console_window() {
	ShowWindow(console_window, SW_HIDE);
}
void show_console_window() {
	if (!console_window) {
		AllocConsole();
		console_window = GetConsoleWindow();
		console_output = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	ShowWindow(console_window, SW_SHOW);
}

#endif

Printer current_printer = console_printer;

#endif

struct PrinterPusher {
	Printer old_printer;
	PrinterPusher(Printer new_printer) {
		old_printer = current_printer;
		current_printer = new_printer;
	}
	~PrinterPusher() {
		current_printer = old_printer;
	}
	operator bool() { return true; }
};

#define push_printer(printer) if(auto CONCAT(_,__LINE__)=::TL::PrinterPusher(printer))

} // namespace TL

