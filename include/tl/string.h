#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace TL {

template <class Char>
List<Char> null_terminate(Span<Char> span) {
	List<Char> result;
	result.size = span.size + 1;
	result.data = ALLOCATE(Char, current_allocator, result.size);
	result.capacity = result.size;
	memcpy(result.data, span.data, span.size * sizeof(Char));
	result.data[result.size - 1] = 0;
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

template <class Int>
inline constexpr umm _intToStringSize = sizeof(Int) * 8 + (is_signed<Int> ? 1 : 0);

template <class Int>
struct FormatInt {
	Int value;
	u32 radix;
	u32 pad_to_length;
	explicit FormatInt(Int value, u32 radix = 10, u32 pad_to_length = 0) : value(value), radix(radix), pad_to_length(pad_to_length) {}
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
	explicit FormatFloat(Float value, u32 precision, FloatFormat format) : value(value), precision(precision), format(format) {}
	explicit FormatFloat(Float value, u32 precision) : value(value), precision(precision) {}
	explicit FormatFloat(Float value, FloatFormat format) : value(value), format(format) {}
	explicit FormatFloat(Float value) : value(value) {}
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

inline u32 get_char_and_advance_utf8(utf8 const *&ptr) {
	if (*ptr < 0x80) {
		defer { ++ptr; };
		return *ptr;
	}

	u32 byte_count = count_leading_ones((u8)*ptr);
	if (byte_count == 1 || byte_count > 4)
		return ~0u;

	defer { ptr += byte_count; };

	switch (byte_count) {
		case 2: return ((ptr[0] & 0x1Fu) <<  6u) | ((ptr[1] & 0x3Fu));
		case 3: return ((ptr[0] & 0x0Fu) << 12u) | ((ptr[1] & 0x3Fu) <<  6u) | ((ptr[2] & 0x3Fu));
		case 4: return ((ptr[0] & 0x07u) << 18u) | ((ptr[1] & 0x3Fu) << 12u) | ((ptr[2] & 0x3Fu) << 6u) | ((ptr[3] & 0x3Fu));
	}
}
inline u32 get_char_and_advance_utf8(utf8 *&ptr) {
	return get_char_and_advance_utf8((utf8 const *&)ptr);
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
		StaticList<char, block_size> buffer;
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
				fn(block->buffer.data, block_size);
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
		return new (ALLOCATE(Block, allocator)) Block;
	}
};

inline void free(StringBuilder &builder) {
	for (auto block = builder.first.next; block != 0;) {
		auto next = block->next;
		FREE(builder.allocator, block);
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

inline void append(StringBuilder &b, ascii ch) {
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
}

inline void append(StringBuilder &b, Span<ascii> string) {
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
}

inline void append(StringBuilder &b, Span<utf8> string) {
	switch (b.encoding) {
		case Encoding_utf8:
			append_bytes(b, string);
			break;
		case Encoding_ascii:
			while (*string.data) {
				if (*string.data < 0x80) {
					append_bytes(b, (ascii)*string.data);
					++string.data;
				} else {
					append_bytes(b, b.unfound_ascii);
					u32 byte_count = char_byte_count(string.data);
					if (byte_count == ~0u) {
						invalid_code_path("string was invalid");
						return;
					}
					string.data += byte_count;
				}
			}
			break;
		case Encoding_utf16: {
			auto utf16 = utf8_to_utf16(string);
			defer { free(utf16); };
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
}

inline void append(StringBuilder &b, Span<utf16> string) {
	switch (b.encoding) {
		case Encoding_utf8: {
			auto utf8 = utf16_to_utf8(string);
			defer { free(utf8); };
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
}
inline void append(StringBuilder &b, Span<utf32> string) {
	invalid_code_path("not implemented");
}

forceinline void append(StringBuilder &b, ascii const *string) { append(b, as_span(string)); }
forceinline void append(StringBuilder &b, utf8  const *string) { append(b, as_span(string)); }
forceinline void append(StringBuilder &b, utf16 const *string) { append(b, as_span(string)); }
forceinline void append(StringBuilder &b, utf32 const *string) { append(b, as_span(string)); }
forceinline void append(StringBuilder &b, wchar const *string) {
	if constexpr (sizeof(wchar) == sizeof(utf16)) {
		append(b, as_span((utf16 *)string));
	} else {
		append(b, as_span((utf32 *)string));
	}
}

forceinline void append(StringBuilder &b, List<ascii> list) { append(b, as_span(list)); }
forceinline void append(StringBuilder &b, List<utf8 > list) { append(b, as_span(list)); }
forceinline void append(StringBuilder &b, List<utf16> list) { append(b, as_span(list)); }
forceinline void append(StringBuilder &b, List<utf32> list) { append(b, as_span(list)); }
forceinline void append(StringBuilder &b, List<wchar> list) { append(b, (Span<utf16>)as_span(list)); }

template <class T>
inline static constexpr bool is_char = is_same<T, ascii> || is_same<T, utf8> || is_same<T, utf16> || is_same<T, utf32> || is_same<T, wchar>;

template <class Char, class = EnableIf<is_char<Char>>>
inline void append_format(StringBuilder &b, Char const *format_string) {
	if (!*format_string) {
		return;
	}

	Char const *c = format_string;
	while (*c) {
		if (*c == '%') {
			++c;
			if (*c != '`') {
				invalid_code_path("bad format");
			}
			append(b, Span(format_string, c));
			++c;
			format_string = c;
		} else {
			++c;
		}
	}
	append(b, as_span(format_string));
}
template <class Arg, class ...Args, class Char, class = EnableIf<is_char<Char>>>
void append_format(StringBuilder &b, Char const *format_string, Arg const &arg, Args const &...args) {
	if (!*format_string) {
		return;
	}
	Char const *c = format_string;
	Char const *percent = 0;
	Char const *after_percent = 0;
	auto format_string_end = format_string + string_unit_count(format_string);

	bool argAsFormat = false;

	while (*c) {
		if (*c == '%') {
			percent = c;
			++c;
			if (*c == '`') {
				append(b, Span(format_string, c));
				++c;
				format_string = c;
				percent = 0;
				continue;
			}
			if (starts_with(Span(c, format_string_end), "fmt%"s)) {
				argAsFormat = true;
				c += 4;
			}
			after_percent = c;
			break;
		}
		++c;
	}
	//assert(percent && after_percent, "invalid format");
	if (percent && after_percent) {
		append(b, Span(format_string, percent));

		if (argAsFormat) {
			if constexpr (is_same<RemoveReference<std::decay_t<Arg>>, Char const *> || is_same<RemoveReference<std::decay_t<Arg>>, Char *>)
				append_format(b, arg, args...);
			else
				invalid_code_path("'%fmt%' arg is not a string");
		} else {
			append(b, arg);
		}

		append_format(b, after_percent, args...);
	} else {
		invalid_code_path("missing argument in append_format");
	}
}

inline void append(StringBuilder &builder, bool value) {
	append(builder, value ? "true"s : "false"s);
}

template <class Int>
void append(StringBuilder &builder, FormatInt<Int> f) {
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
				*lsc-- = charMap[-(v % (Int)radix)];
				v /= (Int)radix;
				++charsWritten;
			}
			v = -v;
		}
	}

	for (;;) {
		*lsc-- = charMap[v % (Int)radix];
		v /= (Int)radix;
		++charsWritten;
		if (v == 0)
			break;
	}
	if constexpr (is_signed<Int>) {
		if (negative) {
			++charsWritten;
			*lsc-- = '-';
		}
	} else {
		(void)negative;
	}
	if (f.pad_to_length) {
		for (u32 i = charsWritten; i < f.pad_to_length; ++i) {
			*lsc-- = '0';
		}
		if (f.pad_to_length > charsWritten)
			charsWritten = f.pad_to_length;
	}
	append(builder, Span(lsc + 1, charsWritten));
}

template <class Int, class = EnableIf<is_integer<Int>>>
void append(StringBuilder &builder, Int v) {
	append(builder, FormatInt(v));
}

forceinline void append(StringBuilder &builder, void const *p) {
	append(builder, FormatInt((umm)p, 16, true));
}

inline void append(StringBuilder &builder, FormatFloat<f64> f) {
	auto value = f.value;
	auto precision = f.precision;
	if (is_negative(value)) {
		append(builder, "-");
		value = -value;
	}

	auto append_float = [&](f64 f) {
		append(builder, (u64)f);
		append(builder, ".");
		for (u32 i = 0; i < precision; ++i) {
			f = f - (f64)(s64)f;
			f = (f < 0 ? f + 1 : f) * 10;
			ascii ch = (ascii)((u32)f + '0');
			append(builder, Span(&ch, 1));
		}
	};

	switch (f.format) {
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
}
inline void append(StringBuilder &builder, FormatFloat<f32> f) {
	append(builder, FormatFloat((f64)f.value, f.precision, f.format));
}

forceinline void append(StringBuilder &builder, f64 v) { append(builder, FormatFloat(v)); }
forceinline void append(StringBuilder &builder, f32 v) { append(builder, FormatFloat(v)); }

// Always allocates memory for the string
inline List<char> to_string(StringBuilder &builder) {
	List<char> result;
	result.reserve(builder.size());
	builder.fill(result);
	result.size = result.capacity;
	return result;
}

template <class ...Args>
inline List<char> concatenate(Args &&...args) {
	StringBuilder builder;
	defer { free(builder); };
	int ___[] = { ((append(builder, args), ...), 0) };
	return to_string(builder);
}

template <class ...Args>
List<ascii> format(ascii const *fmt, Args const &...args) {
	StringBuilder builder;
	defer { free(builder); };
	builder.encoding = Encoding_ascii;
	append_format(builder, fmt, args...);
	return to_string(builder);
}

template <class ...Args>
List<utf8> format(utf8 const *fmt, Args const &...args) {
	StringBuilder builder;
	defer { free(builder); };
	append_format(builder, fmt, args...);
	return (List<utf8>)to_string(builder);
}

template <class ...Args>
List<utf16> format(utf16 const *fmt, Args const &...args) {
	StringBuilder builder;
	defer { free(builder); };

	builder.encoding = Encoding_utf16;

	append_format(builder, fmt, args...);
	return (List<utf16>)to_string(builder);
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

	if (terminate)
		result.back() = 0;

	return result;
}

#endif

}

#pragma warning(pop)
