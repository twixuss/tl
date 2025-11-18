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

	template <Appendable ...T>
	inline umm write(T const &...args) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = TL_GET_CURRENT(temporary_allocator);
		if constexpr (sizeof...(T) > 1) {
			append_format(builder, args...);
		} else {
			append(builder, args...);
		}
		return (*this)((Span<utf8>)(to_string(builder, TL_GET_CURRENT(temporary_allocator))));
	}

	template <class Size> inline umm write(Span<char, Size> const &span) { return (*this)((Span<utf8>)span); }
	template <class Size> inline umm write(Span<utf8, Size> const &span) { return (*this)((Span<utf8>)span); }

	template <class Size> inline umm write(List<char> const &list) { return (*this)((Span<utf8>)(Span<char>)list); }
	template <class Size> inline umm write(List<utf8> const &list) { return (*this)((Span<utf8>)list); }

	template <Appendable ...T>
	inline umm writeln(T const &...args) {
		scoped(temporary_storage_checkpoint);
		StringBuilder builder;
		builder.allocator = TL_GET_CURRENT(temporary_allocator);
		if constexpr (sizeof...(T) > 1) {
			append_format(builder, args...);
		} else {
			append(builder, args...);
		}
		append(builder, '\n');
		return (*this)((Span<utf8>)(to_string(builder, TL_GET_CURRENT(temporary_allocator))));
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
    gray, // Yes gray and dark_gray are swapped..
    dark_gray,
    blue,
    green,
    cyan,
    red,
    magenta,
    yellow,
    white,
};

// foreground, background
TL_API std::pair<ConsoleColor, ConsoleColor> get_console_color();
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
	std::pair<ConsoleColor, ConsoleColor> old;
	void enter(ConsoleColor _new) {
		old = get_console_color();
		set_console_color(_new);
	}
	void exit() {
		set_console_color(old.first, old.second);
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

}

#if OS_WINDOWS

namespace tl {

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
std::pair<ConsoleColor, ConsoleColor> get_console_color() {
	CONSOLE_SCREEN_BUFFER_INFO csbi = {};
	GetConsoleScreenBufferInfo(std_out, &csbi);

	return {
		(ConsoleColor)((csbi.wAttributes >> 0) & 0xf), 
		(ConsoleColor)((csbi.wAttributes >> 4) & 0xf), 
	};
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
		write(1, span.data, span.count);
	},
	0
};

Printer standard_error_printer = {
	[](Span<utf8> span, void *) {
		write(2, span.data, span.count);
	},
	0
};

void print_to_console(Span<ascii> span) {
	write(1, span.data, span.count);
}
void print_to_console(Span<utf8> span) {
	write(1, span.data, span.count);
}
void print_to_console(Span<utf16> span) {
	write(1, span.data, span.count);
}

void clear_console() {
	auto str = "\33[H\33[J"s;
	write(2, str.data, str.count);
}

void set_console_encoding(Encoding encoding) {
	assert(encoding == Encoding::ascii || encoding == Encoding::utf8, "not supported");
}

static std::pair<ConsoleColor, ConsoleColor> current_console_color;

void set_console_color(ConsoleColor foreground, ConsoleColor background) {
	// \33[{attr};{fg};{bg}m
	// attr:
	//     0 - reset
	//     1 - bright
	//     2 - dim
	//     3 - underline
	//     5 - blink
	//     7 - reverse
	//     8 - hidden
	// fg:
	//     30 - black
	//     31 - red
	//     32 - green
	//     33 - yellow
	//     34 - blue
	//     35 - magenta
	//     36 - cyan
	//     37 - white
	// bg:
	//     40 - black
	//     41 - red
	//     42 - green
	//     43 - yellow
	//     44 - blue
	//     45 - magenta
	//     46 - cyan
	//     47 - white

	using enum ConsoleColor;

	StaticList<char, 64> buf;
	buf.add('\33');
	buf.add('[');
	// attr
	switch (foreground) {
		case black:
		case dark_blue:
		case dark_green:
		case dark_cyan:
		case dark_red:
		case dark_magenta:
		case dark_yellow:
		case dark_gray:
			buf.add("2;"s);
			break;
		default:
			buf.add("0;"s);
			break;
	}
	// fg
	switch (foreground) {
		case black:   buf.add("30;"s); break;
		case red:     case dark_red:     buf.add("31;"s); break;
		case green:   case dark_green:   buf.add("32;"s); break;
		case yellow:  case dark_yellow:  buf.add("33;"s); break;
		case blue:    case dark_blue:    buf.add("34;"s); break;
		case magenta: case dark_magenta: buf.add("35;"s); break;
		case cyan:    case dark_cyan:    buf.add("36;"s); break;
		case white: case dark_gray: case gray: buf.add("37;"s); break;
	}
	// bg
	switch (foreground) {
		case black:   buf.add("40m"s); break;
		case red:     case dark_red:     buf.add("41m"s); break;
		case green:   case dark_green:   buf.add("42m"s); break;
		case yellow:  case dark_yellow:  buf.add("43m"s); break;
		case blue:    case dark_blue:    buf.add("44m"s); break;
		case magenta: case dark_magenta: buf.add("45m"s); break;
		case cyan:    case dark_cyan:    buf.add("46m"s); break;
		case white: case dark_gray: case gray: buf.add("47m"s); break;
	}

	write(2, buf.data, buf.count);

	current_console_color =  {foreground, background};
}
std::pair<ConsoleColor, ConsoleColor> get_console_color() {
	return current_console_color;
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


