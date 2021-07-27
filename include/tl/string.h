#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace tl {

template <class T>
List<T> null_terminate(Span<T> span) {
	List<T> result;
	result.size = span.size + 1;
	result.data = result.allocator.allocate<T>(result.size);
	result.capacity = result.size;
	memcpy(result.data, span.data, span.size * sizeof(T));
	result.data[result.size - 1] = {};
	return result;
}

inline u32 character_to_digit(char character, u32 base) {
	u32 digit;
	if (base > 10) {
		digit = (u64)character - '0';
		if (digit > 9) digit = (u64)to_lower_ascii(character) - ('a' - 10);
	} else {
		digit = (u64)character - '0';
	}
	return digit;
}

inline Optional<u64> parse_u64(Span<char> string, u32 base) {
	u64 result = 0;
	u64 previous = 0;
	for (auto character : string) {
		u64 digit = character_to_digit(character, base);
		if (digit >= base) return {};
		previous = result;
		result = result * base + digit;
		if (result < previous) return {};
	}
	return result;
}

inline Optional<u64> parse_u64(Span<char> string) {
	if (string.size > 1) {
		if (string[1] == 'x') {
			string.data += 2;
			return parse_u64(string, 16);
		} else if (string[1] == 'b') {
			string.data += 2;
			return parse_u64(string, 2);
		}
	}
	return parse_u64(string, 10);

}
#if 0
inline ParseResult<f64> parseDecimalFloat(Span<char> s) {
	ParseResult<f64> result = {};
	bool negative = false;
	if (s.front() == '-') {
		negative = true;
		s._begin++;
	}
	bool dot = false;
	u32 placeAfterDot = 0;
	for (auto c : s) {
		if (c == '.') {
			dot = true;
			continue;
		}
		if (dot) {
			s64 digit = (s64)c - '0';
			if ((u64)digit > 9)
				return {};
			result.value += (f64)digit / pow(10, placeAfterDot++);
		} else {
			result.value *= 10;
			s64 digit = (s64)c - '0';
			if ((u64)digit > 9)
				return {};
			result.value += (f64)digit;
		}
	}
	if (negative) {
		result.value = -result.value;
	}
	result.success = true;
	return result;
}
#endif

enum FormatAlignKind : u8 {
	FormatAlign_left,
	FormatAlign_right,
};

template <class Char>
struct FormatAlign {
	FormatAlignKind kind;
	Char fill;
	u32 count;
};

template <class Char>
FormatAlign<Char> align_left(u16 count, Char fill) {
	FormatAlign<Char> result;
	result.count = count;
	result.fill = fill;
	result.kind = FormatAlign_left;
	return result;
}

template <class T, class Char>
struct Format {
	T value;
	FormatAlign<Char> align;
	Format(T value, FormatAlign<Char> align) : value(value), align(align) {}
};

template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (is_signed<Int> ? 1 : 0);

template <class Int>
struct FormatInt {
	Int value;
	u32 radix = 10;
	u32 leading_zeros = 0;
	u32 skip_digits = 0;
};

enum FloatFormat {
	FloatFormat_default,
	FloatFormat_exponential,
	FloatFormat_exponential_e,
};

template <class Float>
struct FormatFloat {
	Float value;
	u32 precision = 3;
	FloatFormat format = FloatFormat_default;
	bool trailing_zeros = false;
};

enum Encoding {
	Encoding_unknown,
	Encoding_ascii,
	Encoding_utf8,
	Encoding_utf16,
	Encoding_utf32,
};

template <class Char> inline static constexpr Encoding encoding_from_type = Encoding_unknown;
template <> inline static constexpr Encoding encoding_from_type<char > = Encoding_ascii;
template <> inline static constexpr Encoding encoding_from_type<utf8 > = Encoding_utf8;
template <> inline static constexpr Encoding encoding_from_type<utf16> = Encoding_utf16;
template <> inline static constexpr Encoding encoding_from_type<utf32> = Encoding_utf32;
template <> inline static constexpr Encoding encoding_from_type<wchar> = (sizeof(wchar) == sizeof(utf16)) ? Encoding_utf16 : Encoding_utf32;

inline u32 char_byte_count(utf8 const *ch) {
	auto byte_count = count_leading_ones((u8)*ch);
	if (byte_count == 1 || byte_count > 4)
		return ~0u;
	return byte_count;
}

inline u8 const *advance_utf8(u8 const *string) {
	if (*string < 0x80) {
		++string;
	} else {
		u32 bytes = count_leading_ones(*string);
		if (bytes > 4) return 0;
		string += bytes;
	}
	return string;
}
template <class Ptr>
inline Ptr advance_utf8(Ptr string) { return (Ptr)advance_utf8((u8 const *&)string); }

inline u32 get_char_utf8(void const *_ptr) {
	auto ptr = (u8 const *)_ptr;
	if (*ptr < 0x80) {
		return *ptr;
	}

	switch (count_leading_ones(*ptr)) {
		case 2: return ((ptr[0] & 0x1Fu) <<  6u) | ((ptr[1] & 0x3Fu));
		case 3: return ((ptr[0] & 0x0Fu) << 12u) | ((ptr[1] & 0x3Fu) <<  6u) | ((ptr[2] & 0x3Fu));
		case 4: return ((ptr[0] & 0x07u) << 18u) | ((ptr[1] & 0x3Fu) << 12u) | ((ptr[2] & 0x3Fu) << 6u) | ((ptr[3] & 0x3Fu));
		default: return ~0u;
	}
}

inline bool get_char_and_advance_utf8(utf8 const *&ptr, u32 *code_point) {
	if (*ptr < 0x80) {
		defer { ++ptr; };
		*code_point = *ptr;
		return true;
	}

	u32 byte_count = count_leading_ones((u8)*ptr);
	if (byte_count == 1 || byte_count > 4)
		return false;

	defer { ptr += byte_count; };

	u32 result;
	switch (byte_count) {
		case 2: result = ((ptr[0] & 0x1Fu) <<  6u) | ((ptr[1] & 0x3Fu)); break;
		case 3: result = ((ptr[0] & 0x0Fu) << 12u) | ((ptr[1] & 0x3Fu) <<  6u) | ((ptr[2] & 0x3Fu)); break;
		case 4: result = ((ptr[0] & 0x07u) << 18u) | ((ptr[1] & 0x3Fu) << 12u) | ((ptr[2] & 0x3Fu) << 6u) | ((ptr[3] & 0x3Fu)); break;
	}
	*code_point = result;
	return true;
}
inline bool get_char_and_advance_utf8(utf8 *&ptr, u32 *code_point) {
	return get_char_and_advance_utf8((utf8 const *&)ptr, code_point);
}

inline StaticList<utf8, 4> encode_utf8(u32 ch) {
	StaticList<utf8, 4> result;
	if (ch <= 0x80) {
		result.add(ch);
	} else if (ch <= 0x800) {
		result.add(0xC0 | ((ch >> 6) & 0x1f));
		result.add(0x80 | (ch & 0x3f));
	} else if (ch <= 0x10000) {
		result.add(0xE0 | ((ch >> 12) & 0x0f));
		result.add(0x80 | ((ch >>  6) & 0x3f));
		result.add(0x80 | (ch & 0x3f));
	} else {
		result.add(0xF0 | ((ch >> 18) & 0x07));
		result.add(0x80 | ((ch >> 12) & 0x3f));
		result.add(0x80 | ((ch >>  6) & 0x3f));
		result.add(0x80 | (ch & 0x3f));
	}
	return result;
}

// NOTE: TODO: surrogate pairs ate not supported
inline List<ascii> utf16_to_ascii(Span<utf16> span, bool terminate = false, ascii unfound = '?') {
	List<ascii> result;
	result.reserve(span.size);
	for (auto u16 : span) {
		result.add(u16 > 0xFF ? unfound : (ascii)u16);
	}
	if (terminate) {
		result.add(0);
	}
	return result;
}

inline Span<utf8> ascii_to_utf8(Span<ascii> span) {
	return Span((utf8 *)span.data, span.size);
}

inline List<utf16> ascii_to_utf16(Span<ascii> span, bool terminate = false) {
	List<utf16> result;
	result.reserve(span.size);
	for (auto ch : span) {
		result.add((utf16)ch);
	}
	if (terminate) {
		result.add(0);
	}
	return result;
}

TL_API List<utf8> utf16_to_utf8(Span<utf16> utf16, bool terminate = false);
TL_API List<utf16> utf8_to_utf16(Span<utf8> utf8, bool terminate = false);

#ifndef TL_STRING_BUILDER_BLOCK_SIZE
#define TL_STRING_BUILDER_BLOCK_SIZE 0x4000
#endif

struct StringBuilder {
	static constexpr umm block_size = TL_STRING_BUILDER_BLOCK_SIZE;
	struct Block {
		StaticList<utf8, block_size> buffer;
		Block *next = 0;
		umm available_space() { return block_size - buffer.size; }
	};

	Allocator allocator = {};
	Block first = {};
	Block *last = &first;
	Block *alloc_last = &first;
	Encoding encoding = Encoding_utf8;
	ascii unfound_ascii = '?';

	StringBuilder() { allocator = current_allocator; }
	StringBuilder(StringBuilder const &that) = delete;
	StringBuilder(StringBuilder&& that) = default;
	StringBuilder &operator=(StringBuilder const &that) = delete;
	StringBuilder &operator=(StringBuilder &&that) = default;
	umm available_space() {
		umm space = 0;
		Block *block = last;
		while (block) {
			space += block->available_space();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = available_space();
		while (space < amount) {
			Block *new_block = allocate_block();
			alloc_last->next = new_block;
			alloc_last = new_block;
			space += block_size;
		}
	}
	void ensure_consecutive_space(umm amount) {
		assert(amount <= block_size);
		if (last->available_space() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocate_block();
				last = alloc_last = last->next;
			}
		}
	}
	umm size() {
		umm total_size = 0;
		for (Block *block = &first; block != 0; block = block->next) {
			total_size += block->buffer.size;
		}
		return total_size;
	}
	Span<char> fill(Span<char> dst_string) {
		char *dst_char = dst_string.data;
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dst_char, block->buffer.data, block->buffer.size * sizeof(char));
			dst_char += block->buffer.size;
		}
		return Span<char>(dst_string.begin(), dst_char);
	}
	List<char> getNullTerminated() {
		List<char> result;
		result.reserve(size() + 1);
		fill(result);
		result.size = result.capacity;
		result.back() = '\0';
		return result;
	}

	template <class Fn>
	void for_each_block(Fn &&fn) {
		Block *block = &first;
		do {
			auto block_size = block->buffer.size;
			if (block_size) {
				fn(block->buffer);
			}
			block = block->next;
		} while (block);
	}
	void clear() {
		Block *block = &first;
		do {
			block->buffer.size = 0;
			block = block->next;
		} while (block);
		last = &first;
	}

	Block *allocate_block() {
		return new (allocator.allocate<Block>()) Block;
	}
};

inline void free(StringBuilder &builder) {
	for (auto block = builder.first.next; block != 0;) {
		auto next = block->next;
		builder.allocator.free(block);
		block = next;
	}
	builder.allocator = {};
}

forceinline void append_bytes(StringBuilder &b, void const *_data, umm size) {
	umm chars_to_write = size;
	u8 *data = (u8 *)_data;
	while (b.last->available_space() < chars_to_write) {
		umm space_in_block = b.last->available_space();
		memcpy(b.last->buffer.end(), data, space_in_block);
		chars_to_write -= space_in_block;
		b.last->buffer.size += space_in_block;
		data += space_in_block;
		if (!b.last->next) {
			b.last->next = b.alloc_last = b.allocate_block();
			b.last = b.last->next;
		}
	}
	memcpy(b.last->buffer.end(), data, chars_to_write);
	b.last->buffer.size += chars_to_write;
}

template <class T>
forceinline void append_bytes(StringBuilder &b, T const &value) {
	append_bytes(b, &value, sizeof(value));
}

template <class T>
forceinline void append_bytes(StringBuilder &b, Span<T> span) {
	append_bytes(b, span.data, span.size * sizeof(T));
}

template <class T>
forceinline void append_bytes(StringBuilder &b, List<T> list) {
	append_bytes(b, list.data, list.size * sizeof(T));
}

inline umm append(StringBuilder &b, ascii ch) {
	switch (b.encoding) {
		case Encoding_ascii:
		case Encoding_utf8:
			append_bytes(b, ch);
			break;
		case Encoding_utf16:
			append_bytes(b, (utf16)ch);
			break;
		case Encoding_utf32:
			append_bytes(b, (utf32)ch);
			break;
		default:
			invalid_code_path("StringBuilder.encoding was invalid");
			break;
	}
	return 1;
}
inline umm append(StringBuilder &b, utf8 ch) {
	switch (b.encoding) {
		case Encoding_ascii:
		case Encoding_utf8:
			append_bytes(b, ch);
			break;
		case Encoding_utf16:
			append_bytes(b, (utf16)ch);
			break;
		case Encoding_utf32:
			invalid_code_path("not implemented");
			break;
		default:
			invalid_code_path("StringBuilder.encoding was invalid");
			break;
	}
	return 1;
}
inline umm append(StringBuilder &b, utf16 ch) {
	switch (b.encoding) {
		case Encoding_ascii:
			if (ch >= 256) {
				append_bytes(b, b.unfound_ascii);
			} else {
				append_bytes(b, (ascii)ch);
			}
			break;
		case Encoding_utf8:
			append_bytes(b, with(temporary_allocator, utf16_to_utf8(Span(&ch, 1))));
			break;
		case Encoding_utf16:
			append_bytes(b, ch);
			break;
		case Encoding_utf32:
			invalid_code_path("not implemented");
			break;
		default:
			invalid_code_path("StringBuilder.encoding was invalid");
			break;
	}
	return 1;
}

inline umm append(StringBuilder &b, Span<ascii> string) {
	switch (b.encoding) {
		case Encoding_ascii:
		case Encoding_utf8:
			append_bytes(b, string);
			break;
		case Encoding_utf16:
			while (*string.data) {
				append_bytes(b, (utf16)*string.data);
				++string.data;
			}
			break;
		case Encoding_utf32:
			while (*string.data) {
				append_bytes(b, (utf32)*string.data);
				++string.data;
			}
			break;
		default:
			invalid_code_path("StringBuilder.encoding was invalid");
			break;
	}
	return string.size;
}

inline umm append(StringBuilder &b, Span<utf8> string) {
	switch (b.encoding) {
		case Encoding_utf8:
			append_bytes(b, string);
			break;
		case Encoding_ascii: {
			auto string_end = string.end();
			while (string.data != string_end) {
				if (*string.data < 0x80) {
					append_bytes(b, (ascii)*string.data);
					++string.data;
				} else {
					append_bytes(b, b.unfound_ascii);
					u32 byte_count = char_byte_count(string.data);
					if (byte_count == ~0u) {
						// print("string was invalid\n");
						return 0;
					}
					string.data += byte_count;
				}
			}
			break;
		}
		case Encoding_utf16: {
			auto utf16 = with(temporary_allocator, utf8_to_utf16(string));
			append_bytes(b, utf16);
			break;
		}
		case Encoding_utf32:
			invalid_code_path("case Encoding_utf32 is not implemented");
			break;
		default:
			invalid_code_path("StringBuilder.encoding was invalid");
			break;
	}
	return string.size;
}

inline umm append(StringBuilder &b, Span<utf16> string) {
	switch (b.encoding) {
		case Encoding_utf8: {
			auto utf8 = with(temporary_allocator, utf16_to_utf8(string));
			append_bytes(b, utf8);
			break;
		}
		case Encoding_ascii:
			while (*string.data) {
				if (*string.data < 0x80) {
					append_bytes(b, (ascii)*string.data);
					++string.data;
				} else {
					append_bytes(b, b.unfound_ascii);
					++string.data; // NOTE: TODO: surrogate pairs
				}
			}
			break;
		case Encoding_utf16:
			append_bytes(b, string);
			break;
		case Encoding_utf32:
			invalid_code_path("case Encoding_utf32 is not implemented");
			break;
		default:
			invalid_code_path("StringBuilder.encoding was invalid");
			break;
	}
	return string.size;
}
inline umm append(StringBuilder &b, Span<utf32> string) {
	invalid_code_path("not implemented");
	return string.size;
}
inline umm append(StringBuilder &b, Span<wchar> string) {
	return append(b, (Span<wchar_s>)string);
}

forceinline umm append(StringBuilder &b, ascii const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf8  const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf16 const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf32 const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, wchar const *string) {
	if constexpr (sizeof(wchar) == sizeof(utf16)) {
		return append(b, as_span((utf16 *)string));
	} else {
		return append(b, as_span((utf32 *)string));
	}
}

forceinline umm append(StringBuilder &b, List<ascii> list) { return append(b, as_span(list)); }
forceinline umm append(StringBuilder &b, List<utf8 > list) { return append(b, as_span(list)); }
forceinline umm append(StringBuilder &b, List<utf16> list) { return append(b, as_span(list)); }
forceinline umm append(StringBuilder &b, List<utf32> list) { return append(b, as_span(list)); }
forceinline umm append(StringBuilder &b, List<wchar> list) { return append(b, (Span<utf16>)as_span(list)); }

template <class T>
inline static constexpr bool is_char = is_same<T, ascii> || is_same<T, utf8> || is_same<T, utf16> || is_same<T, utf32> || is_same<T, wchar>;

template <class Char, class = EnableIf<is_char<Char>>>
inline void append_format(StringBuilder &b, Span<Char> format_string) {
	bool previous_is_percent = false;
	auto c = format_string.data;
	auto end = format_string.end();
	for (;c != end; ++c) {
		if (*c == '%') {
			previous_is_percent = true;
			continue;
		}
		if (previous_is_percent) {
			if (*c == '`') {
				append(b, '%');
				continue;
			} else {
				invalid_code_path("not enough arguments were provided for 'append_format'");
				return;
			}
		}
		append(b, *c);
		previous_is_percent = false;
	}
}

template <class Arg, class ...Args, class Char, class = EnableIf<is_char<Char>>>
void append_format(StringBuilder &b, Span<Char> format_string, Arg const &arg, Args const &...args) {
	bool previous_is_percent = false;
	auto c = format_string.data;
	auto end = format_string.end();
	for (;c != end; ++c) {
		if (previous_is_percent) {
			if (*c == '`') {
				append(b, '%');
				continue;
			} else {
				append(b, arg);
				append_format(b, Span(c, end), args...);
				return;
			}
		}
		if (*c == '%') {
			previous_is_percent = true;
			continue;
		}
		append(b, *c);
		previous_is_percent = false;
	}
	if (previous_is_percent) {
		append(b, arg);
		append_format(b, Span(c, end), args...);
	} else {
		invalid_code_path("too many arguments were provided for 'append_format'");
	}
}
template <class ...Args, class Char, class = EnableIf<is_char<Char>>>
void append_format(StringBuilder &b, Char const *format_string, Args const &...args) {
	append_format(b, as_span(format_string), args...);
}

inline umm append(StringBuilder &builder, bool value) {
	return append(builder, value ? "true"s : "false"s);
}

template <class T, class Char>
umm append(StringBuilder &builder, Format<T, Char> format) {
	if (format.align.count) {
		if (format.align.kind == FormatAlign_left) {
			auto appended_char_count = append(builder, format.value);
			for (; appended_char_count < format.align.count; ++appended_char_count) {
				append(builder, format.align.fill);
			}
			return appended_char_count;
		} else {
			invalid_code_path("not implemented");
			return 0;
		}
	} else {
		return append(builder, format.value);
	}
}
template <class Int>
umm append(StringBuilder &builder, FormatInt<Int> f) {
	Int v = f.value;
	u32 radix = f.radix;
	constexpr u32 maxDigits = _intToStringSize<Int>;
	char buf[maxDigits];
	constexpr char charMap[] = "0123456789ABCDEF";
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if constexpr (is_signed<Int>) {
		if (v < 0) {
			negative = true;
			if (v == min_value<Int>) {
				*lsc-- = charMap[-(v % cvt<Int>(radix))];
				v /= (Int)radix;
				++charsWritten;
			}
			v = -v;
		}
	}

	for (;;) {
		*lsc-- = charMap[(u32)(v % cvt<Int>(radix))];
		++charsWritten;
		v /= cvt<Int>(radix);
		if (v == Int{})
			break;
	}
	lsc += f.skip_digits;
	charsWritten -= f.skip_digits;
	if constexpr (is_signed<Int>) {
		if (negative) {
			++charsWritten;
			*lsc-- = '-';
		}
	} else {
		(void)negative;
	}
	if (f.leading_zeros) {
		for (u32 i = charsWritten; i < f.leading_zeros; ++i) {
			*lsc-- = '0';
		}
		if (f.leading_zeros > charsWritten)
			charsWritten = f.leading_zeros;
	}
	return append(builder, Span(lsc + 1, charsWritten));
}

template <class Int, class = EnableIf<is_integer<Int>>>
umm append(StringBuilder &builder, Int v) {
	return append(builder, FormatInt{.value = v});
}

forceinline umm append(StringBuilder &builder, void const *p) {
	return append(builder, FormatInt{.value = (umm)p, .radix = 16, .leading_zeros = true});
}

inline umm append(StringBuilder &builder, FormatFloat<f64> format) {
	umm chars_appended = 0;

	auto append = [&](StringBuilder &builder, auto const &value) {
		chars_appended += ::tl::append(builder, value);
	};

	auto value = format.value;
	auto precision = format.precision;
	if (is_negative(value)) {
		append(builder, "-");
		value = -value;
	}

	auto append_float = [&](f64 f) {
		append(builder, (u64)f);
		u64 fract_part = 1;
		for (u32 i = 0; i < precision; ++i) {
			f = frac(f) * 10;
			fract_part *= 10;
			fract_part += (u32)f;
		}

		if (!format.trailing_zeros) {
			while ((fract_part % 10) == 0) {
				fract_part /= 10;
			}
		}

		if (fract_part != 1) {
			append(builder, '.');
			append(builder, FormatInt{.value = fract_part, .skip_digits = 1});
		}
	};

	switch (format.format) {
		case FloatFormat_default: {
			append_float(value);
			break;
		}
		case FloatFormat_exponential: {
			f64 mantissa = value;
			s32 exponent = 0;
			while (mantissa >= 10) {
				mantissa /= 10;
				++exponent;
			}
			while (mantissa < 1) {
				mantissa *= 10;
				--exponent;
			}
			append_float(mantissa);

			append(builder, "*10^"s);
			append(builder, exponent);

			break;
		}
		case FloatFormat_exponential_e: {
			f64 mantissa = value;
			s32 exponent = 0;
			while (mantissa >= 10) {
				mantissa /= 10;
				++exponent;
			}
			while (mantissa < 1) {
				mantissa *= 10;
				--exponent;
			}
			append_float(mantissa);

			append(builder, "e");

			if (exponent >= 0) append(builder, "+");

			append(builder, exponent);

			break;
		}
	}
	return chars_appended;
}
inline umm append(StringBuilder &builder, FormatFloat<f32> f) {
	return append(builder, FormatFloat{.value = (f64)f.value, .precision = f.precision, .format = f.format});
}

forceinline umm append(StringBuilder &builder, f64 v) { return append(builder, FormatFloat{.value = v}); }
forceinline umm append(StringBuilder &builder, f32 v) { return append(builder, FormatFloat{.value = v}); }

inline void append(StringBuilder &builder, std::source_location location) {
	append(builder, location.file_name());
	append(builder, '(');
	append(builder, location.line());
	append(builder, ":");
	append(builder, location.column());
	append(builder, "):");
	append(builder, location.function_name());
}

// Always allocates memory for the string
inline List<char> to_string(StringBuilder &builder, Allocator allocator) {
	List<char> result;
	result.allocator = allocator;
	result.reserve(builder.size());
	builder.fill(result);
	result.size = result.capacity;
	return result;
}
inline List<char> to_string(StringBuilder &builder) {
	return to_string(builder, builder.allocator);
}

template <class Char, class ...Args>
inline List<Char> concatenate(Span<Char> arg0, Args const &...args) {
	StringBuilder builder;
	builder.encoding = encoding_from_type<Char>;
	append(builder, arg0);
	int ___[] = { ((append(builder, args), ...), 0) };
	return (List<Char>)to_string(builder, current_allocator);
}

template <class Char, class ...Args>
inline List<Char> concatenate(List<Char> arg0, Args const &...args) {
	return concatenate((Span<Char>)arg0, args...);
}

template <class ...Args>
inline auto tconcatenate(Args &&...args) {
	return with(temporary_allocator, concatenate(args...));
}

template <class ...Args>
List<ascii> format(Span<ascii> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	builder.encoding = Encoding_ascii;
	append_format(builder, fmt, args...);
	return to_string(builder, current_allocator);
}

template <class ...Args>
List<utf8> format(Span<utf8> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	builder.encoding = Encoding_utf8;
	append_format(builder, fmt, args...);
	return (List<utf8>)to_string(builder, current_allocator);
}

template <class ...Args>
List<utf16> format(Span<utf16> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	builder.encoding = Encoding_utf16;
	append_format(builder, fmt, args...);
	return (List<utf16>)to_string(builder, current_allocator);
}

template <class ...Args> List<ascii> format(ascii const *fmt, Args const &...args) { return format(as_span(fmt), args...); }
template <class ...Args> List<utf8 > format(utf8  const *fmt, Args const &...args) { return format(as_span(fmt), args...); }
template <class ...Args> List<utf16> format(utf16 const *fmt, Args const &...args) { return format(as_span(fmt), args...); }

template <class ...Args> List<ascii> tformat(ascii const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf8 > tformat(utf8  const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf16> tformat(utf16 const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf32> tformat(utf32 const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }

template <class ...Args> List<ascii> tformat(Span<ascii> fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf8 > tformat(Span<utf8 > fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf16> tformat(Span<utf16> fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf32> tformat(Span<utf32> fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }

template <class T>
List<char> to_string(T const &value, Encoding encoding = Encoding_utf8) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	builder.encoding = encoding;
	append(builder, value);
	return to_string(builder, current_allocator);
}

struct FormattedBytes {
	f32 count;
	u8 unit;
	bool kilo_is_1024;
};

struct FormatBytesParams {
	bool kilo_is_1024 = true;
};

inline FormattedBytes format_bytes(umm _count, FormatBytesParams params = {}) {
	FormattedBytes result{};

	result.kilo_is_1024 = params.kilo_is_1024;

	f64 kilo = params.kilo_is_1024 ? 1024 : 1000;

	f64 count = (f64)_count;
	while (count > kilo) {
		count /= kilo;
		result.unit += 1;
	}
	result.count = (f32)count;

	return result;
}

inline void append(StringBuilder &builder, FormattedBytes bytes) {
	append(builder, FormatFloat{.value = bytes.count, .precision = 3, .trailing_zeros = false});
	append(builder, ' ');
	static constexpr Span<char> unit_strings[2][7] = {
		{
			"B"s,
			"KB"s,
			"MB"s,
			"GB"s,
			"TB"s,
			"PB"s,
			"EB"s,
		}, {
			"B"s,
			"KiB"s,
			"MiB"s,
			"GiB"s,
			"TiB"s,
			"PiB"s,
			"EiB"s,
		}
	};
	append(builder, unit_strings[bytes.kilo_is_1024][bytes.unit]);
}

#if 0

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char, class ...Args>
String<Char, Allocator> formatAndTerminate(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.append_format(fmt, args...);
	builder.append((Char)0);
	return builder.get();
}
#endif
//template <class Char, umm capacity, class T>
//void append_string(StaticList<Char, capacity> &list, T const &value) {
//	to_string<Char>(value, [&](Span<Char> span) { list += span; });
//}

#ifdef TL_IMPL

List<utf8> utf16_to_utf8(Span<utf16> utf16, bool terminate) {
	List<utf8> result;

	if (utf16.size > max_value<int>)
		return {};

	auto bytes_required = WideCharToMultiByte(CP_UTF8, 0, (wchar *)utf16.data, (int)utf16.size, 0, 0, 0, 0);
	if (!bytes_required)
		return {};

	result.reserve(bytes_required + terminate);
	result.size = bytes_required + terminate;

	if (WideCharToMultiByte(CP_UTF8, 0, (wchar *)utf16.data, (int)utf16.size, (char *)result.data, bytes_required, 0, 0) != bytes_required) {
		return {};
	}

	if (terminate)
		result.back() = 0;

	return result;
}

List<utf16> utf8_to_utf16(Span<utf8> utf8, bool terminate) {
	List<utf16> result;

	if (utf8.size > max_value<int>)
		return {};

	auto chars_required = MultiByteToWideChar(CP_UTF8, 0, (char *)utf8.data, (int)utf8.size, 0, 0);
	if (!chars_required)
		return {};

	result.reserve(chars_required + terminate);
	result.size = chars_required + terminate;

	if (MultiByteToWideChar(CP_UTF8, 0, (char *)utf8.data, (int)utf8.size, (wchar *)result.data, chars_required) != chars_required) {
		return {};
	}

	if (terminate) {
		result.back() = {};
	}

	return result;
}

#endif

}

#pragma warning(pop)
