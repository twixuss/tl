#pragma once

/*

print:
	Prints the arguments to `current_printer`.
	Returns number of bytes printed.

*/

#include "common.h"
#include "string.h"

namespace tl {

enum PrintKind {
	Print_default,
	Print_info,
	Print_warning,
	Print_error,
};

struct Printer {
	void (*func)(PrintKind kind, Span<utf8> span, void *state);
	void *state;

	void operator()(PrintKind kind, Span<utf8> span) {
		func(kind, span, state);
	}
};

extern TL_API Printer console_printer;
extern TL_API Printer standard_output_printer;
extern TL_API thread_local Printer current_printer;

TL_API void init_printer();
TL_API void deinit_printer();

TL_API void set_console_encoding(Encoding);
TL_API void clear_console();

TL_API void print_to_console(Span<ascii> string);
TL_API void print_to_console(Span<utf8 > string);
TL_API void print_to_console(Span<utf16> string);
TL_API void print_to_console(Span<utf32> string);

template <class T>
inline umm print(PrintKind kind, T const &value) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	builder.encoding = Encoding_utf8;
	append(builder, value);
	auto string = as_utf8(to_string(builder, temporary_allocator));
	current_printer(kind, string);
	return string.count;
}

template <> inline umm print(PrintKind kind, Span<char> const &span) { current_printer(kind, (Span<utf8>)span); return span.count; }
template <> inline umm print(PrintKind kind, Span<utf8> const &span) { current_printer(kind, span); return span.count; }

template <> inline umm print(PrintKind kind, List<char> const &list) { current_printer(kind, (Span<utf8>)(Span<char>)list); return list.count; }
template <> inline umm print(PrintKind kind, List<utf8> const &list) { current_printer(kind, (Span<utf8>)list); return list.count; }

template <class ...Args>
inline umm print(PrintKind kind, char const *fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append_format(builder, fmt, args...);
	auto string = to_string(builder, temporary_allocator);
	print(kind, string);
	return string.count;
}

inline umm print(PrintKind kind, char const *string) {
	return print(kind, as_span(string));
}

template <class ...T>
inline umm print(T const &...values) {
	return print(Print_default, values...);
}

TL_API void hide_console_window();
TL_API void show_console_window();
TL_API void toggle_console_window();

#ifdef TL_IMPL

thread_local Printer current_printer = {[](PrintKind, Span<utf8>, void *) {}};

void init_printer() {
	current_printer = standard_output_printer;
}
void deinit_printer() {

}

#if OS_WINDOWS

static HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
static HWND console_window = GetConsoleWindow();

Printer console_printer = {
	[](PrintKind kind, Span<utf8> span, void *) {
		switch (kind) {
			case Print_default: SetConsoleTextAttribute(std_out, 7); break;
			case Print_info:    SetConsoleTextAttribute(std_out, 7); break;
			case Print_warning: SetConsoleTextAttribute(std_out, 14); break;
			case Print_error:   SetConsoleTextAttribute(std_out, 12); break;
		}
		print_to_console(span);
	},
	0
};

Printer standard_output_printer = {
	[](PrintKind kind, Span<utf8> span, void *) {
		switch (kind) {
			case Print_default: SetConsoleTextAttribute(std_out, 7); break;
			case Print_info:    SetConsoleTextAttribute(std_out, 7); break;
			case Print_warning: SetConsoleTextAttribute(std_out, 14); break;
			case Print_error:   SetConsoleTextAttribute(std_out, 12); break;
		}
		WriteFile(std_out, span.data, (DWORD)span.count, 0, 0);
	},
	0
};

void print_to_console(Span<ascii> span) {
	DWORD charsWritten;
	WriteConsoleA(std_out, span.data, (DWORD)span.count, &charsWritten, 0);
}
void print_to_console(Span<utf8> span) {
	DWORD charsWritten;
	WriteConsoleA(std_out, span.data, (DWORD)span.count, &charsWritten, 0);
}
void print_to_console(Span<utf16> span) {
	DWORD charsWritten;
	WriteConsoleW(std_out, span.data, (DWORD)span.count, &charsWritten, 0);
}

void clear_console() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT sr;
	COORD st;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(std_out, &csbi))
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
	ScrollConsoleScreenBufferA(std_out, &sr, NULL, st, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(std_out, csbi.dwCursorPosition);
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
		std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	ShowWindow(console_window, SW_SHOW);
}
void toggle_console_window() {
	if (console_window && IsWindowVisible(console_window)) {
		hide_console_window();
	} else {
		show_console_window();
	}
}

#endif

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

#define push_printer(printer) tl_push(::tl::PrinterPusher, printer)
#define scoped_printer(printer) tl_scoped(::tl::current_printer, printer)

} // namespace tl

