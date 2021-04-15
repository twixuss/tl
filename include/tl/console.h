#pragma once

#include "common.h"
#include "string.h"

namespace TL {

struct Printer {
	void (*func)(Span<utf8> span, void *state);
	void *state;

	void operator()(Span<utf8> span) {
		func(span, state);
	}
};

extern TL_API Printer console_printer;
extern TL_API Printer current_printer;

TL_API void set_console_encoding(Encoding);
TL_API void clear_console();

// UTF-8
TL_API void print_to_console(Span<utf8> string);

inline void print_to_console(Span<ascii>  string) { print_to_console(ascii_to_utf8(string)); }
inline void print_to_console(Span<utf16> string) {
	auto temp = utf16_to_utf8(string);
	defer { free(temp); };
	print_to_console(temp);
}

template <class T>
inline void print(T const &value) {
	StringBuilder builder;
	defer { free(builder); };

	append(builder, value);

	auto string = to_string(builder);
	defer { free(string); };

	current_printer(ascii_to_utf8(string));
}

template <> inline void print(Span<char> const &span) { current_printer(span); }
template <> inline void print(Span<utf8> const &span) { current_printer(span); }

template <class ...Args>
inline void print(char const *fmt, Args const &...args) {
	StringBuilder builder;
	defer { free(builder); };

	append_format(builder, fmt, args...);

	auto string = to_string(builder);
	defer { free(string); };

	print(string);
}

TL_API void hide_console_window();
TL_API void show_console_window();

#ifdef TL_IMPL

Printer console_printer = {
	[](Span<utf8> span, void *) { print_to_console(span); },
	0
};

#if OS_WINDOWS

static HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
static HWND console_window = GetConsoleWindow();

void print_to_console(Span<utf8> span) {
	DWORD charsWritten;
	WriteConsoleA(console_output, span.data, (DWORD)span.size, &charsWritten, 0);
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

#define push_printer(printer) tl_push(::TL::PrinterPusher, printer)
#define scoped_printer(printer) tl_scoped(::TL::current_printer, printer)

} // namespace TL

