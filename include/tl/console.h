#pragma once

/*

print:
	Prints the arguments to `current_printer`.
	Returns number of bytes printed.

*/

#include "common.h"
#include "string.h"

namespace tl {

struct Printer {
	void (*func)(Span<utf8> span, void *state);
	void *state;

	umm operator()(Span<utf8> span) { 
		func(span, state);
		return span.count;
	}

	template <class ...T>
	inline umm write(char const *fmt, T const &...args) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = temporary_allocator;
		append_format(builder, fmt, args...);
		return (*this)((Span<utf8>)(to_string(builder, temporary_allocator)));
	}

	template <class T>
	inline umm write(T const &value) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = temporary_allocator;
		append(builder, value);
		return (*this)((Span<utf8>)(to_string(builder, temporary_allocator)));
	}

	template <class ...T>
	inline umm writeln(char const *fmt, T const &...args) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = temporary_allocator;
		append_format(builder, fmt, args...);
		append(builder, '\n');
		return (*this)((Span<utf8>)(to_string(builder, temporary_allocator)));
	}

	template <class T>
	inline umm writeln(T const &value) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = temporary_allocator;
		append(builder, value);
		append(builder, '\n');
		return (*this)((Span<utf8>)(to_string(builder, temporary_allocator)));
	}
};

extern TL_API Printer console_printer;
extern TL_API Printer standard_output_printer;
extern TL_API Printer standard_error_printer;
extern thread_local Printer current_printer;

TL_API void init_printer();
TL_API void deinit_printer();

TL_API void set_console_encoding(Encoding);
TL_API void clear_console();

TL_API void print_to_console(Span<ascii> string);
TL_API void print_to_console(Span<utf8 > string);
TL_API void print_to_console(Span<utf16> string);
TL_API void print_to_console(Span<utf32> string);

enum class ConsoleColor {
    black,
    dark_blue,
    dark_green,
    dark_cyan,
    dark_red,
    dark_magenta,
    dark_yellow,
    dark_gray,
    gray,
    blue,
    green,
    cyan,
    red,
    magenta,
    yellow,
    white,
};

TL_API void set_console_color(ConsoleColor foreground, ConsoleColor background);
inline void set_console_color(ConsoleColor foreground) {
	set_console_color(foreground, ConsoleColor::black);
}

template <class T>
inline umm print(T const &value) {
	scoped(temporary_storage_checkpoint);
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append(builder, value);
	auto string = as_utf8(to_string(builder, temporary_allocator));
	current_printer(string);
	return string.count;
}

template <class Size> inline umm print(Span<char, Size> const &span) { current_printer((Span<utf8>)span); return span.count; }
template <class Size> inline umm print(Span<utf8, Size> const &span) { current_printer((Span<utf8>)span); return span.count; }

template <class Size> inline umm print(List<char> const &list) { current_printer((Span<utf8>)(Span<char>)list); return list.count; }
template <class Size> inline umm print(List<utf8> const &list) { current_printer((Span<utf8>)list); return list.count; }

template <class ...Args>
inline umm print(utf8 const *fmt, Args const &...args) {
	scoped(temporary_storage_checkpoint);
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append_format(builder, fmt, args...);
	auto string = to_string(builder, temporary_allocator);
	print(string);
	return string.count;
}

inline umm print(utf8 const *string) {
	return print(as_span(string));
}

inline umm print(char const *fmt, auto const &...args) {
	return current_printer.write(fmt, args...);
}

inline umm print(char const *string) {
	return print(as_span(string));
}

inline umm println() {
	return print('\n');
}

inline umm println(char const *fmt, auto const &...args) {
	return current_printer.writeln(fmt, args...);
}

inline umm println(auto const &value) {
	return current_printer.writeln(value);
}

inline umm errln(char const *fmt, auto const &...args) {
	return standard_error_printer.writeln(fmt, args...);
}

inline umm errln(auto const &value) {
	return standard_error_printer.writeln(value);
}

TL_API void hide_console_window();
TL_API void show_console_window();
TL_API void toggle_console_window();


template <>
struct Scoped<ConsoleColor> {
	inline static ConsoleColor current = ConsoleColor::dark_gray;
	ConsoleColor old;
	Scoped(ConsoleColor _new) {
		old = current;
		current = _new;
		set_console_color(_new);
	}
	~Scoped() {
		current = old;
		set_console_color(old);
	}
};

#ifdef TL_IMPL

thread_local Printer current_printer = {[](Span<utf8>, void *) {}};

void deinit_printer() {

}

#if OS_WINDOWS

static HANDLE std_out;
static HANDLE std_err;
static HWND console_window;

Printer console_printer = {
	[](Span<utf8> span, void *) {
		print_to_console(span);
	},
	0
};

Printer standard_output_printer = {
	[](Span<utf8> span, void *) {
		WriteFile(std_out, span.data, (DWORD)span.count, 0, 0);
	},
	0
};

Printer standard_error_printer = {
	[](Span<utf8> span, void *) {
		WriteFile(std_err, span.data, (DWORD)span.count, 0, 0);
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
		case Encoding_unknown: break;
		case Encoding_ascii: return 1251;
		case Encoding_utf8: return 65001;
		case Encoding_utf16: return 1200;
		case Encoding_utf32: invalid_code_path("not implemented");
	}
	invalid_code_path();
	return (DWORD)-1;
}

void set_console_encoding(Encoding encoding) {
	auto cp = get_code_page(encoding);
	SetConsoleOutputCP(cp);
	SetConsoleCP(cp);
}

void set_console_color(ConsoleColor foreground, ConsoleColor background) {
	SetConsoleTextAttribute(std_out, (WORD)((DWORD)foreground | ((DWORD)background << 4)));
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

void init_printer() {
	current_printer = standard_output_printer;
#if OS_WINDOWS
	std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	std_err = GetStdHandle(STD_ERROR_HANDLE);
	console_window = GetConsoleWindow();
#endif
}

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

