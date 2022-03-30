#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4702) // unreachable code

namespace tl {

template <class T>
List<T> null_terminate(Span<T> span) {
	List<T> result;
	result.count = span.count + 1;
	result.data = result.allocator.allocate<T>(result.count);
	result.capacity = result.count;
	memcpy(result.data, span.data, span.count * sizeof(T));
	result.data[result.count - 1] = {};
	return result;
}

template <class T>
forceinline List<T> temporary_null_terminate(Span<T> span) {
	return with(temporary_allocator, null_terminate(span));
}

inline u32 character_to_digit(utf32 character, u32 base) {
	u32 digit;
	if (base > 10) {
		digit = (u64)character - '0';
		if (digit > 9) digit = (u64)to_lower(character) - ('a' - 10);
	} else {
		digit = (u64)character - '0';
	}
	return digit;
}

inline Optional<u64> parse_u64(Span<utf8> string, u32 base) {
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

inline Optional<u64> parse_u64(Span<utf8> string) {
	if (string.count > 1) {
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

template <class Char>
FormatAlign<Char> align_right(u16 count, Char fill) {
	FormatAlign<Char> result;
	result.count = count;
	result.fill = fill;
	result.kind = FormatAlign_right;
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
	u32 leading_zero_count = 0;
	u32 skip_digits = 0;
	char const *char_set = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
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
template <> inline constexpr Encoding encoding_from_type<char > = Encoding_ascii;
template <> inline constexpr Encoding encoding_from_type<utf8 > = Encoding_utf8;
template <> inline constexpr Encoding encoding_from_type<utf16> = Encoding_utf16;
template <> inline constexpr Encoding encoding_from_type<utf32> = Encoding_utf32;
template <> inline constexpr Encoding encoding_from_type<wchar> = (sizeof(wchar) == sizeof(utf16)) ? Encoding_utf16 : Encoding_utf32;

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

inline Optional<utf32> get_char_utf8(utf8 const *ptr) {
	if (*ptr < 0x80) {
		return *ptr;
	}

	switch (count_leading_ones((u8)*ptr)) {
		case 2: return ((ptr[0] & 0x1Fu) <<  6u) | ((ptr[1] & 0x3Fu));
		case 3: return ((ptr[0] & 0x0Fu) << 12u) | ((ptr[1] & 0x3Fu) <<  6u) | ((ptr[2] & 0x3Fu));
		case 4: return ((ptr[0] & 0x07u) << 18u) | ((ptr[1] & 0x3Fu) << 12u) | ((ptr[2] & 0x3Fu) << 6u) | ((ptr[3] & 0x3Fu));
	}
	return {};
}

inline Optional<utf32> get_char_and_advance_utf8(utf8 const **ptr) {
	auto &text = *ptr;
	if (*text < 0x80) {
		defer { ++text; };
		return *text;
	}

	u32 byte_count = count_leading_ones((u8)*text);
	if (byte_count == 1 || byte_count > 4)
		return {};

	defer { text += byte_count; };

	switch (byte_count) {
		case 2: return ((text[0] & 0x1Fu) <<  6u) | ((text[1] & 0x3Fu));
		case 3: return ((text[0] & 0x0Fu) << 12u) | ((text[1] & 0x3Fu) <<  6u) | ((text[2] & 0x3Fu));
		case 4: return ((text[0] & 0x07u) << 18u) | ((text[1] & 0x3Fu) << 12u) | ((text[2] & 0x3Fu) << 6u) | ((text[3] & 0x3Fu));
	}
	return {};
}
inline Optional<utf32> get_char_and_advance_utf8(utf8 **ptr) {
	return get_char_and_advance_utf8((utf8 const **)ptr);
}

inline StaticList<utf8, 4> encode_utf8(u32 ch) {
	StaticList<utf8, 4> result;
	if (ch <= 0x80) {
		result.add((utf8)ch);
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
inline List<ascii> to_ascii(Span<utf16> span, bool terminate = false, ascii unfound = '?') {
	List<ascii> result;
	result.reserve(span.count);
	for (auto u16 : span) {
		result.add(u16 > 0xFF ? unfound : (ascii)u16);
	}
	if (terminate) {
		result.add(0);
	}
	return result;
}

inline Span<utf8> to_utf8(Span<ascii> span) {
	return Span((utf8 *)span.data, span.count);
}

inline List<utf16> to_utf16(Span<ascii> span, bool terminate = false) {
	List<utf16> result;
	result.reserve(span.count);
	for (auto ch : span) {
		result.add((utf16)ch);
	}
	if (terminate) {
		result.add(0);
	}
	return result;
}

TL_API List<utf8> to_utf8(Span<utf16> utf16, bool terminate = false TL_LP);
TL_API List<utf16> to_utf16(Span<utf8> utf8, bool terminate = false TL_LP);

#ifndef TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY
#define TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY 0x4000
#endif

struct StringBuilder {
	struct Block {
		u8 *data = 0;
		umm count = 0;
		umm capacity = 0;
		Block *next = 0;
		u8 *begin() { return data; }
		u8 *end() { return data + count; }
		umm available_space() { return capacity - count; }
	};

	Allocator allocator = {};
	u8 initial_buffer[TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY];
	Block first = {};
	Block *last = &first;
	Block *alloc_last = &first;

	StringBuilder() {
		allocator = current_allocator;
		first.data = initial_buffer;
		first.capacity = TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY;
	}
	StringBuilder(StringBuilder const &that) = delete;
	StringBuilder(StringBuilder&& that) = default;
	StringBuilder &operator=(StringBuilder const &that) = delete;
	StringBuilder &operator=(StringBuilder &&that) = default;
	/*
	umm available_space() {
		umm space = 0;
		Block *block = last;
		while (block) {
			space += block->available_space();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount TL_LP) {
		umm space = available_space();
		while (space < amount) {
			Block *new_block = allocate_block(TL_LAC);
			alloc_last->next = new_block;
			alloc_last = new_block;
			space += block_size;
		}
	}
	void ensure_consecutive_space(umm amount TL_LP) {
		assert(amount <= block_size);
		if (last->available_space() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocate_block(TL_LAC);
				last = alloc_last = last->next;
			}
		}
	}
	*/
	umm count() {
		umm total_count = 0;
		Block *block = &first;
		do {
			total_count += block->count;
			block = block->next;
		} while (block);
		return total_count;
	}
	Span<u8> fill(Span<u8> dst_string) {
		u8 *dst_char = dst_string.data;
		Block *block = &first;
		do {
			memcpy(dst_char, block->data, block->count);
			dst_char += block->count;
			block = block->next;
		} while (block);
		return Span<u8>(dst_string.begin(), dst_char);
	}
	List<u8> get_null_terminated() {
		List<u8> result;
		result.reserve(count() + 1);
		fill(result);
		result.count = result.capacity;
		result.back() = '\0';
		return result;
	}

	template <class Fn>
	void for_each_block(Fn &&fn) {
		Block *block = &first;
		do {
			if (block->count)
				fn(block);
			block = block->next;
		} while (block);
	}
	void clear() {
		Block *block = &first;
		do {
			block->count = 0;
			block = block->next;
		} while (block);
		last = &first;
	}

	Block *allocate_block(TL_LPC) {
		auto capacity = alloc_last->capacity*2;
		auto block = (Block *)allocator.allocate_uninitialized(sizeof(Block) + capacity, alignof(Block) TL_LA);
		block->data = (u8 *)(block + 1);
		block->count = 0;
		block->capacity = capacity;
		block->next = 0;
		alloc_last = alloc_last->next = block;
		return block;
	}
};

inline void free(StringBuilder &builder) {
	for (auto block = builder.first.next; block != 0;) {
		auto next = block->next;
		builder.allocator.free_t(block);
		block = next;
	}
	builder.allocator = {};
}

template <class Allocator>
inline List<u8, Allocator> to_string(StringBuilder &builder, Allocator allocator = Allocator::current() TL_LP) {
	List<u8, Allocator> result;
	result.allocator = allocator;
	result.reserve(builder.count() TL_LA);
	builder.fill(result);
	result.count = result.capacity;
	return result;
}

inline List<u8> to_string(StringBuilder &builder, Allocator allocator TL_LP) {
	return to_string<Allocator>(builder, allocator);
}

inline List<u8> to_string(StringBuilder &builder TL_LP) {
	return to_string(builder, current_allocator TL_LA);
}

forceinline umm append_bytes(StringBuilder &b, void const *_data, umm size TL_LP) {
	umm chars_to_write = size;
	u8 *data = (u8 *)_data;
	while (b.last->available_space() < chars_to_write) {
		umm space_in_block = b.last->available_space();
		memcpy(b.last->end(), data, space_in_block);
		chars_to_write -= space_in_block;
		b.last->count += space_in_block;
		data += space_in_block;
		if (!b.last->next) {
			b.allocate_block(TL_LAC);
			b.last = b.last->next;
		}
	}
	memcpy(b.last->end(), data, chars_to_write);
	b.last->count += chars_to_write;
	return size;
}

template <class T>
forceinline umm append_bytes(StringBuilder &b, T const &value TL_LP) {
	return append_bytes(b, &value, sizeof(value) TL_LA);
}

template <class T, class Size>
forceinline umm append_bytes(StringBuilder &b, Span<T, Size> span TL_LP) {
	return append_bytes(b, span.data, span.count * sizeof(T) TL_LA);
}

template <class T, class Allocator, class Size>
forceinline umm append_bytes(StringBuilder &b, List<T, Allocator, Size> list TL_LP) {
	return append_bytes(b, list.data, list.count * sizeof(T) TL_LA);
}

inline umm append(StringBuilder &b, ascii ch) { return append_bytes(b, ch); }
inline umm append(StringBuilder &b, utf8  ch) { return append_bytes(b, ch); }
inline umm append(StringBuilder &b, utf16 ch) { return append_bytes(b, ch); }
inline umm append(StringBuilder &b, utf32 ch) { return append_bytes(b, ch); }

template <class Size> inline umm append(StringBuilder &b, Span<u8   , Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<ascii, Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<wchar, Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<utf8 , Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<utf16, Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<utf32, Size> string) { return append_bytes(b, string); }

forceinline umm append(StringBuilder &b, ascii const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf8  const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf16 const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf32 const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, wchar const *string) { return append(b, as_span(string)); }

template <class T>
struct FormatList {
	Span<u8> before = "{"b;
	Span<u8> separator = ", "b;
	Span<u8> after = "}"b;
	List<T> list;
};

template <class T>
forceinline umm append(StringBuilder &b, FormatList<T> format) {
	umm count = 0;
	count += append_bytes(b, format.before);
	if (format.list.count)
		count += append(b, *format.list.data);
	for (auto &val : format.list.skip(1)) {
		count += append_bytes(b, format.separator);
		count += append(b, val);
	}
	count += append_bytes(b, format.after);
	return count;
}

template <class T>
forceinline umm append(StringBuilder &b, List<T> list) { return append(b, FormatList{.list = list}); }

template <> forceinline umm append(StringBuilder &b, List<u8   > list) { return append(b, as_span(list)); }
template <> forceinline umm append(StringBuilder &b, List<ascii> list) { return append(b, as_span(list)); }
template <> forceinline umm append(StringBuilder &b, List<wchar> list) { return append(b, as_span(list)); }
template <> forceinline umm append(StringBuilder &b, List<utf8 > list) { return append(b, as_span(list)); }
template <> forceinline umm append(StringBuilder &b, List<utf16> list) { return append(b, as_span(list)); }
template <> forceinline umm append(StringBuilder &b, List<utf32> list) { return append(b, as_span(list)); }

template <class T>
inline static constexpr bool is_char = is_same<T, ascii> || is_same<T, utf8> || is_same<T, utf16> || is_same<T, utf32> || is_same<T, wchar>;

template <class Char>
inline EnableIf<is_char<Char>, umm> append_format(StringBuilder &b, Span<Char> format_string) {
	Char previous = {};
	auto start = format_string.data;
	auto c = start;
	auto end = format_string.end();

	umm appended_char_count = 0;

	for (;c != end; ++c) {
		auto next_prev = *c;
		defer { previous = next_prev; };
		if (previous == '{') {
			switch (*c) {
				case '{':
					appended_char_count += append(b, Span(start, c));
					start = c + 1;
					next_prev = 0;
					break;
				case '}': invalid_code_path("not enough arguments were provided for 'append_format'"); break;
				default:  invalid_code_path("bad format string: only '{' or '}' can follow '{'"); break;
			}
		} else if (previous == '}') {
			if (*c == '}') {
				appended_char_count += append(b, Span(start, c));
				start = c + 1;
				next_prev = 0;
			} else {
				invalid_code_path("bad format string: only '}' can follow '}'");
			}
		}
	}
	if (previous == '{' || previous == '}') {
		invalid_code_path("bad format string: missing brace at the end");
	}
	appended_char_count += append(b, Span(start, end));
	return appended_char_count;
}

template <class Arg, class ...Args, class Char, class = EnableIf<is_char<Char>>>
umm append_format(StringBuilder &b, Span<Char> format_string, Arg const &arg, Args const &...args) {
	Char previous = {};
	auto start = format_string.data;
	auto c = start;
	auto end = format_string.end();
	umm appended_char_count = 0;
	for (;c != end; ++c) {
		auto next_prev = *c;
		defer { previous = next_prev; };
		if (previous == '{') {
			switch (*c) {
				case '{':
					appended_char_count += append(b, Span(start, c));
					start = c + 1;
					next_prev = 0;
					break;
				case '}':
					appended_char_count += append(b, Span(start, c - 1));
					static_assert(is_same<decltype(append(b, arg)), tl::umm>, "`append` must return `umm`");
					appended_char_count += append(b, arg);
					appended_char_count += append_format(b, Span(c + 1, end), args...);
					start = end;
					c = end;
					return appended_char_count;
				default:
					invalid_code_path("bad format string: only '{' or '}' can follow '{'");
					break;
			}
		} else if (previous == '}') {
			if (*c == '}') {
				appended_char_count += append(b, Span(start, c));
				start = c + 1;
				next_prev = 0;
			} else {
				invalid_code_path("bad format string: only '}' can follow '}'");
			}
		}
	}
	invalid_code_path("Too many arguments provided for this format string");
	return {};
}
template <class ...Args, class Char, class = EnableIf<is_char<Char>>>
umm append_format(StringBuilder &b, Char const *format_string, Args const &...args) {
	return append_format(b, as_span(format_string), args...);
}

inline umm append(StringBuilder &builder, bool value) {
	return append(builder, value ? "true"s : "false"s);
}

template <class T, class Char>
umm append(StringBuilder &builder, Format<T, Char> format) {
	if (format.align.count) {
		if (format.align.kind == FormatAlign_left) {
			auto appended_char_count = append(builder, format.value);
			while (appended_char_count < format.align.count) {
				appended_char_count += append(builder, format.align.fill);
			}
			return appended_char_count;
		} else {
			StringBuilder temp;
			temp.allocator = temporary_allocator;

			umm appended_char_count = append(temp, format.value);
			while (appended_char_count < format.align.count) {
				appended_char_count += append(builder, format.align.fill);
			}
			append(builder, to_string(temp, temporary_allocator));
			return 0;
		}
	} else {
		return append(builder, format.value);
	}
}
template <class Int, umm capacity>
umm write_as_string(StaticList<ascii, capacity> &buffer, FormatInt<Int> f) {
	Int v = f.value;
	auto radix = convert<Int>(f.radix);
	constexpr u32 maxDigits = _intToStringSize<Int>;
	char buf[maxDigits];
	auto charMap = f.char_set;
	char *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;

	bool negative = false;
	if constexpr (is_signed<Int>) {
		if (v < 0) {
			negative = true;
			if (v == min_value<Int>) {
				*lsc-- = charMap[-(v % radix)];
				v /= radix;
				++charsWritten;
			}
			v = -v;
		}
	}

	for (;;) {
		*lsc-- = charMap[(u32)(v % radix)];
		++charsWritten;
		v /= radix;
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
	if (f.leading_zero_count) {
		for (u32 i = charsWritten; i < f.leading_zero_count; ++i) {
			*lsc-- = '0';
		}
		if (f.leading_zero_count > charsWritten)
			charsWritten = f.leading_zero_count;
	}
	buffer.add(Span(lsc + 1, charsWritten));
	return charsWritten;
}
template <class Int, umm capacity, class = EnableIf<is_integer<Int>>>
umm write_as_string(StaticList<ascii, capacity> &buffer, Int v) {
	return write_as_string(buffer, FormatInt{.value = v});
}

template <class Int>
umm append(StringBuilder &builder, FormatInt<Int> f) {
	StaticList<ascii, _intToStringSize<Int>> buffer;
	write_as_string(buffer, f);
	return append(builder, buffer.span());
}

template <class Int, class = EnableIf<is_integer<Int>>>
umm append(StringBuilder &builder, Int v) {
	return append(builder, FormatInt{.value = v});
}

forceinline umm append(StringBuilder &builder, void const *p) {
	return append(builder, FormatInt{.value = (umm)p, .radix = 16, .leading_zero_count = sizeof(void *) * 2});
}

template <class Float>
inline umm append_float(StringBuilder &builder, FormatFloat<Float> format) {
	scoped_allocator(temporary_allocator);

	auto value = format.value;

	if (is_nan(value)) {
		return append(builder, "NaN");
	}

	auto precision = format.precision;

	StaticList<ascii, 128> buffer;

	if (is_negative(value)) {
		buffer.add('-');
		value = -value;
	}

	if (value == infinity<Float>) {
		return append(builder, "inf");
	}

	auto append_float = [&](Float f) {
		write_as_string(buffer, (u64)f);

		auto round_and_trim = [&] {
			f = frac(f);
			if (f != 0) {
				if (f >= 0.5f) {
					auto it = buffer.end() - 1;
					while (1) {
						*it += 1;
						if (*it == (char)('9' + 1)) {
							buffer.pop_back();
							--it;
							if (*it == '.')
								break;
							else
								continue;
						}
						break;
					}
				}
			}
			while (buffer.back() == '0')
				buffer.pop_back();
			if (buffer.back() == '.')
				buffer.pop_back();
		};

		if (format.trailing_zeros) {
			if (precision) {
				buffer.add('.');
				for (u32 i = 0; i < precision; ++i) {
					f = frac(f) * 10;
					buffer.add((ascii)((u32)f + '0'));
				}
				round_and_trim();
			}
		} else {
			if (frac(f) != 0) {
				buffer.add('.');
				for (u32 i = 0; i < precision && f != 0; ++i) {
					f = frac(f) * 10;
					buffer.add((ascii)((u32)f + '0'));
				}
				round_and_trim();
			}
		}
	};

	switch (format.format) {
		case FloatFormat_default: {
			append_float(value);
			break;
		}
		case FloatFormat_exponential: {
			Float mantissa = value;
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

			buffer.add("*10^"s);
			write_as_string(buffer, exponent);

			break;
		}
		case FloatFormat_exponential_e: {
			Float mantissa = value;
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

			buffer.add('e');

			if (exponent >= 0) buffer.add('+');

			write_as_string(buffer, exponent);

			break;
		}
	}
	return append(builder, buffer.span());
}
inline umm append(StringBuilder &builder, FormatFloat<f32> f) { return append_float(builder, f); }
inline umm append(StringBuilder &builder, FormatFloat<f64> f) { return append_float(builder, f); }

forceinline umm append(StringBuilder &builder, f64 v) { return append(builder, FormatFloat{.value = v}); }
forceinline umm append(StringBuilder &builder, f32 v) { return append(builder, FormatFloat{.value = v}); }

inline umm append(StringBuilder &builder, std::source_location location) {
	return append_format(builder, "{}:{}:{}: {}", location.file_name(), location.line(), location.column(), location.function_name());
}


template <class T> struct is_utf8_t { inline static constexpr bool value = false; };
template <> struct is_utf8_t<utf8>       { inline static constexpr bool value = true; };
template <class Size> struct is_utf8_t<Span<utf8, Size>> { inline static constexpr bool value = true; };
template <class Allocator, class Size> struct is_utf8_t<List<utf8, Allocator, Size>> { inline static constexpr bool value = true; };

template <class First, class ...Rest>
struct are_utf8_t {
	inline static constexpr bool value = is_utf8_t<First>::value && are_utf8_t<Rest...>::value;
};
template <class First>
struct are_utf8_t<First> {
	inline static constexpr bool value = is_utf8_t<First>::value;
};

template <class ...Args>
inline static constexpr bool are_utf8 = are_utf8_t<Args...>::value;


template <class T> struct is_utf16_t { inline static constexpr bool value = false; };
template <> struct is_utf16_t<utf16>       { inline static constexpr bool value = true; };
template <class Size> struct is_utf16_t<Span<utf16, Size>> { inline static constexpr bool value = true; };
template <class Allocator, class Size> struct is_utf16_t<List<utf16, Allocator, Size>> { inline static constexpr bool value = true; };

template <class First, class ...Rest>
struct are_utf16_t {
	inline static constexpr bool value = is_utf16_t<First>::value && are_utf16_t<Rest...>::value;
};
template <class First>
struct are_utf16_t<First> {
	inline static constexpr bool value = is_utf16_t<First>::value;
};

template <class ...Args>
inline static constexpr bool are_utf16 = are_utf16_t<Args...>::value;


template <class ...Args>
inline auto concatenate(Args const &...args) {
	StringBuilder builder;
	int _ = ((append(builder, args), ...), 0);
	(void)_;
	if constexpr (are_utf8<Args...>) {
		return (List<utf8>)to_string(builder, current_allocator);
	} else if constexpr (are_utf16<Args...>) {
		return (List<utf16>)to_string(builder, current_allocator);
	} else {
		return (List<u8>)to_string(builder, current_allocator);
	}
}

template <class ...Args>
inline auto tconcatenate(Args &&...args) {
	return with(temporary_allocator, concatenate(args...));
}

template <class Allocator = Allocator, class ...Args>
List<ascii, Allocator> aformat(Allocator allocator, Span<ascii> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append_format(builder, fmt, args...);
	return (List<ascii, Allocator>)to_string<Allocator>(builder, allocator);
}

template <class Allocator = Allocator, class ...Args>
List<utf8, Allocator> aformat(Allocator allocator, Span<utf8> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append_format(builder, fmt, args...);
	return (List<utf8, Allocator>)to_string<Allocator>(builder, allocator);
}

template <class Allocator = Allocator, class ...Args>
List<utf16, Allocator> aformat(Allocator allocator, Span<utf16> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append_format(builder, fmt, args...);
	return (List<utf16, Allocator>)to_string<Allocator>(builder, allocator);
}

template <class Allocator = Allocator, class ...Args> List<ascii, Allocator> aformat(Allocator allocator, ascii const *fmt, Args const &...args) { return aformat<Allocator>(allocator, as_span(fmt), args...); }
template <class Allocator = Allocator, class ...Args> List<utf8 , Allocator> aformat(Allocator allocator, utf8  const *fmt, Args const &...args) { return aformat<Allocator>(allocator, as_span(fmt), args...); }
template <class Allocator = Allocator, class ...Args> List<utf16, Allocator> aformat(Allocator allocator, utf16 const *fmt, Args const &...args) { return aformat<Allocator>(allocator, as_span(fmt), args...); }

template <class Allocator = Allocator, class ...Args>
List<ascii, Allocator> format(Span<ascii> fmt, Args const &...args) {
	return aformat<Allocator>(Allocator::current(), fmt, args...);
}

template <class Allocator = Allocator, class ...Args>
List<utf8, Allocator> format(Span<utf8> fmt, Args const &...args) {
	return aformat<Allocator>(Allocator::current(), fmt, args...);
}

template <class Allocator = Allocator, class ...Args>
List<utf16, Allocator> format(Span<utf16> fmt, Args const &...args) {
	return aformat<Allocator>(Allocator::current(), fmt, args...);
}

template <class Allocator = Allocator, class ...Args> List<ascii, Allocator> format(ascii const *fmt, Args const &...args) { return aformat<Allocator>(Allocator::current(), as_span(fmt), args...); }
template <class Allocator = Allocator, class ...Args> List<utf8 , Allocator> format(utf8  const *fmt, Args const &...args) { return aformat<Allocator>(Allocator::current(), as_span(fmt), args...); }
template <class Allocator = Allocator, class ...Args> List<utf16, Allocator> format(utf16 const *fmt, Args const &...args) { return aformat<Allocator>(Allocator::current(), as_span(fmt), args...); }

template <class ...Args> List<ascii> tformat(ascii const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf8 > tformat(utf8  const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf16> tformat(utf16 const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf32> tformat(utf32 const *fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }

template <class ...Args> List<ascii> tformat(Span<ascii> fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf8 > tformat(Span<utf8 > fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf16> tformat(Span<utf16> fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }
template <class ...Args> List<utf32> tformat(Span<utf32> fmt, Args const &...args) { return with(temporary_allocator, format(fmt, args...)); }

template <class T>
List<utf8> to_string(T const &value) {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append(builder, value);
	return (List<utf8>)to_string(builder, current_allocator);
}

struct FormattedBytes {
	f32 count;
	u8 unit;
	bool kilo_is_1024;
};

struct FormatBytesParams {
	bool kilo_is_1024 = true;
};

inline FormattedBytes format_bytes(u64 _count, FormatBytesParams params = {}) {
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

inline umm append(StringBuilder &builder, FormattedBytes bytes) {
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
	return append_format(builder, "{} {}", FormatFloat{.value = bytes.count, .precision = 3, .trailing_zeros = false}, unit_strings[bytes.kilo_is_1024][bytes.unit]);
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

List<utf8> to_utf8(Span<utf16> utf16, bool terminate TL_LPD) {
	List<utf8> result;

	if (utf16.count > max_value<int>)
		return {};

	auto bytes_required = (umm)WideCharToMultiByte(CP_UTF8, 0, (wchar *)utf16.data, (int)utf16.count, 0, 0, 0, 0);
	if (!bytes_required)
		return {};

	result.reserve(bytes_required + terminate TL_LA);
	result.count = bytes_required + terminate;

	if (WideCharToMultiByte(CP_UTF8, 0, (wchar *)utf16.data, (int)utf16.count, (char *)result.data, (int)bytes_required, 0, 0) != (int)bytes_required) {
		return {};
	}

	if (terminate)
		result.back() = 0;

	return result;
}

List<utf16> to_utf16(Span<utf8> utf8, bool terminate TL_LPD) {
	List<utf16> result;

	if (utf8.count > max_value<int>)
		return {};

	auto chars_required = (umm)MultiByteToWideChar(CP_UTF8, 0, (char *)utf8.data, (int)utf8.count, 0, 0);
	if (!chars_required)
		return {};

	result.reserve(chars_required + terminate TL_LA);
	result.count = chars_required + terminate;

	if (MultiByteToWideChar(CP_UTF8, 0, (char *)utf8.data, (int)utf8.count, (wchar *)result.data, (int)chars_required) != (int)chars_required) {
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
