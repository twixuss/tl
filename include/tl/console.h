#pragma once

#include "common.h"
#include "string.h"

namespace TL {

enum Encoding {
    Encoding_ascii,
    Encoding_utf16,
};

template <class Char> inline static constexpr Encoding encoding_from_type = Encoding_ascii;
template <> inline static constexpr Encoding encoding_from_type<wchar> = Encoding_utf16;

using Printer = void (*)(void const *data, umm length, Encoding encoding);

extern TL_API Printer console_printer;
extern TL_API Printer current_printer;

TL_API void clear_console();

inline void print_to_console(char  const *string, umm length) { console_printer(string, length, Encoding_ascii); }
inline void print_to_console(wchar const *string, umm length) { console_printer(string, length, Encoding_utf16); }

template <class Char = char, class T>
inline void print(T const &value) {
	to_string<Char>(value, [](Span<Char const> span) {
		current_printer(span.data(), span.size(), encoding_from_type<Char>);
	});
}

template <class Char = char, class ...Args>
inline void print(Char const *fmt, Args const &...args) {
	StringBuilder<Char> builder;
	builder.append_format(fmt, args...);
	print(builder.get());
}

#ifdef TL_IMPL

#include TL_OS_INCLUDE(console.h)

Printer current_printer = console_printer;

#endif

} // namespace TL

