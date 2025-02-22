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

	template <Appendable FmtOrObj, Appendable ...T>
	inline umm write(FmtOrObj fmt_or_obj, T const &...args) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = TL_GET_CURRENT(temporary_allocator);
		if constexpr (sizeof...(T) > 0) {
			append_format(builder, fmt_or_obj, args...);
		} else {
			append(builder, fmt_or_obj);
		}
		return (*this)((Span<utf8>)(to_string(builder, TL_GET_CURRENT(temporary_allocator))));
	}

	template <class Size> inline umm write(Span<char, Size> const &span) { return (*this)((Span<utf8>)span); }
	template <class Size> inline umm write(Span<utf8, Size> const &span) { return (*this)((Span<utf8>)span); }

	template <class Size> inline umm write(List<char> const &list) { return (*this)((Span<utf8>)(Span<char>)list); }
	template <class Size> inline umm write(List<utf8> const &list) { return (*this)((Span<utf8>)list); }

	template <class Fmt, Appendable ...T>
	inline umm writeln(Fmt fmt, T const &...args) {
		auto a = write(fmt, args...);
		auto b = write('\n');
		return a + b;
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

inline umm print(Appendable auto const &...args) { return current_printer.write(args...); }
inline umm println() { return print('\n'); }
inline umm println(Appendable auto const &...args) { return current_printer.writeln(args...); }

inline umm errln(Appendable auto const &...args) { return standard_error_printer.writeln(args...); }

TL_API void hide_console_window();
TL_API void show_console_window();
TL_API void toggle_console_window();

TL_API bool is_stdout_console();

template <>
struct Scoped<ConsoleColor> {
	inline static ConsoleColor current = ConsoleColor::dark_gray;
	ConsoleColor old;
	void enter(ConsoleColor _new) {
		old = current;
		current = _new;
		set_console_color(_new);
	}
	void exit() {
		current = old;
		set_console_color(old);
	}
};

template <>
struct Scoped<Printer> {
	Printer old_printer;
	void enter(Printer new_printer) {
		old_printer = current_printer;
		current_printer = new_printer;
	}
	void exit() {
		current_printer = old_printer;
	}
};

}

#ifdef TL_IMPL
#if OS_WINDOWS

namespace tl {

thread_local Printer current_printer = {[](Span<utf8>, void *) {}};

void deinit_printer() {

}

Printer console_printer = {
	[](Span<utf8> span, void *) {
		print_to_console(span);
	},
	0
};


static HANDLE std_out;
static HANDLE std_err;
static HWND console_window;

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
		case Encoding::unknown: break;
		case Encoding::ascii: return 1251;
		case Encoding::utf8: return 65001;
		case Encoding::utf16: return 1200;
		case Encoding::utf32: invalid_code_path("not implemented");
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
}
#elif OS_LINUX
#include <unistd.h>
namespace tl {
Printer standard_output_printer = {
	[](Span<utf8> span, void *) {
	not_implemented();
		write(1, span.data, span.count);
	},
	0
};

Printer standard_error_printer = {
	[](Span<utf8> span, void *) {
	not_implemented();
		write(2, span.data, span.count);
	},
	0
};

void print_to_console(Span<ascii> span) {
	not_implemented();
	write(1, span.data, span.count);
}
void print_to_console(Span<utf8> span) {
	not_implemented();
	write(1, span.data, span.count);
}
void print_to_console(Span<utf16> span) {
	not_implemented();
	write(1, span.data, span.count);
}

void clear_console() {
	not_implemented();
}

void set_console_encoding(Encoding encoding) {
	not_implemented();
}

void set_console_color(ConsoleColor foreground, ConsoleColor background) {
	not_implemented();
}

void hide_console_window() {
	not_implemented();
}
void show_console_window() {
	not_implemented();
}
void toggle_console_window() {
	not_implemented();
}
} // namespace tl

#endif
namespace tl {
void init_printer() {
	current_printer = standard_output_printer;
#if OS_WINDOWS
	std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	std_err = GetStdHandle(STD_ERROR_HANDLE);
	console_window = GetConsoleWindow();
#endif
}

bool is_stdout_console() {
#if OS_WINDOWS
	DWORD temp;
	return GetConsoleMode(std_out, &temp) != 0;
#elif OS_LINUX
	not_implemented();
#else
	static_assert(false, "not implemented");
#endif
}

} // namespace tl

#endif


