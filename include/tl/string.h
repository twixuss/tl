#pragma once

// Basic string utilities are here.

//
//     UTF8 READING
//
// If you want to read utf8 text, use decode_and_advance.
//
// You can get more throughput by using decode_and_advance_fast, but there's a few things it assumes about input text:
// 1) ALL characters are VALID.
// 2) There is 3 extra bytes available at the end of the text.


// Tested on random 640 MiB text.
// Generating only ascii ...
// Testing decode_and_advance ... 449.927 ms
// Testing decode_and_advance_fast ... 503.598 ms // Strange. ascii handling is identical in both functions...
// Generating different sizes ... Octet count was equally likely.
// Testing decode_and_advance ... 2438.868 ms
// Testing decode_and_advance_fast ... 1488.426 ms
//

// 
// If you want to print your type or write it to a string builder:
/*
umm append(StringBuilder &builder, YourType t) {
	return append_format(builder, "({} {})", t.first, t.second);
}
*/
// This function should return the number of bytes written.
// Note that a lot of other functions like `print` or `format` use `scoped(temporary_storage_checkpoint)` for better memory utilization.
// With that make sure you don't make temporary allocations and use them after `append`.
//

#include "common.h"
#include "list.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4702) // unreachable code

namespace tl {

template <class T, class Size>
List<T, Allocator, Size> null_terminate(Span<T, Size> span) {
	List<T, Allocator, Size> result;
	result.count = span.count + 1;
	result.data = result.allocator.template allocate<T>(result.count);
	result.capacity = result.count;
	memcpy(result.data, span.data, span.count * sizeof(T));
	result.data[result.count - 1] = {};
	return result;
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
	if (string.count == 0)
		return {};

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

enum IntFormat {
	IntFormat_full,
	IntFormat_kmb,
};

extern thread_local IntFormat default_int_format_format;
extern thread_local u32 default_int_format_radix;
extern thread_local u32 default_int_format_leading_zero_count;
extern thread_local u32 default_int_format_skip_digits;
extern thread_local ascii const *default_int_format_char_set;

template <class Int>
struct FormatInt {
	Int value;
	IntFormat format = default_int_format_format;
	u32 radix = default_int_format_radix;
	u32 leading_zero_count = default_int_format_leading_zero_count;
	u32 skip_digits = default_int_format_skip_digits;
	ascii const *char_set = default_int_format_char_set;
};

enum FloatFormat {
	FloatFormat_default,
	FloatFormat_exponential,
	FloatFormat_exponential_e,
	FloatFormat_kmb,
};

extern thread_local u32 default_float_format_precision;
extern thread_local FloatFormat default_float_format_format;
extern thread_local bool default_float_format_trailing_zeros;

template <class Float>
struct FormatFloat {
	Float value;
	u32 precision = default_float_format_precision;
	FloatFormat format = default_float_format_format;
	bool trailing_zeros = default_float_format_trailing_zeros;

	template <class U>
	FormatFloat<U> with_value(U new_value) const {
		return FormatFloat<U> {
			.value = new_value,
			.precision = precision,
			.format = format,
			.trailing_zeros = trailing_zeros,
		};
	}
};

enum class Encoding {
	unknown,
	ascii,
	utf8,
	utf16,
	utf32,
};

template <class Char> inline constexpr Encoding encoding_from_type = Encoding::unknown;
template <> inline constexpr Encoding encoding_from_type<ascii> = Encoding::ascii;
template <> inline constexpr Encoding encoding_from_type<utf8 > = Encoding::utf8;
template <> inline constexpr Encoding encoding_from_type<utf16> = Encoding::utf16;
template <> inline constexpr Encoding encoding_from_type<utf32> = Encoding::utf32;

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

struct DecodedUtf8Char {
	utf32 code;
	u8 size;
};
inline Optional<DecodedUtf8Char> decode(utf8 const *text) {
	if (*text < 0x80) {
		return DecodedUtf8Char{.code = *text, .size = 1};
	}

	switch (count_leading_ones((u8)*text)) {
		case 2: return DecodedUtf8Char{.code = ((text[0] & 0x1Fu) <<  6u) | ((text[1] & 0x3Fu)), .size = 2};
		case 3: return DecodedUtf8Char{.code = ((text[0] & 0x0Fu) << 12u) | ((text[1] & 0x3Fu) <<  6u) | ((text[2] & 0x3Fu)), .size = 3};
		case 4: return DecodedUtf8Char{.code = ((text[0] & 0x07u) << 18u) | ((text[1] & 0x3Fu) << 12u) | ((text[2] & 0x3Fu) << 6u) | ((text[3] & 0x3Fu)), .size = 4};
	}
	return {};
}

inline Optional<utf32> decode_and_advance(utf8 const **ptr) {
	if (auto decoded = decode(*ptr)) {
		*ptr += decoded.value().size;
		return decoded.value().code;
	}
	return {};
}

inline Optional<utf32> decode_and_advance(utf8 **ptr) {
	return decode_and_advance((utf8 const **)ptr);
}

#define RENAMED_API(old_name, new_name) [[deprecated("use " #new_name " instead")]] inline decltype(auto) old_name(auto &&...args) requires requires { new_name(args...); } { return new_name(args...); }

RENAMED_API(get_char_and_advance_utf8, decode_and_advance)

inline utf32 decode_and_advance_fast(utf8 const **ptr) {
	auto &text = *ptr;
	if (*text < 0x80) {
		return *text++;
	}
	u32 byte_count = count_leading_ones((u8)*text);
	utf32 result[3];
	result[0] = ((text[0] & 0x1Fu) <<  6u) | ((text[1] & 0x3Fu));
	result[1] = ((text[0] & 0x0Fu) << 12u) | ((text[1] & 0x3Fu) <<  6u) | ((text[2] & 0x3Fu));
	result[2] = ((text[0] & 0x07u) << 18u) | ((text[1] & 0x3Fu) << 12u) | ((text[2] & 0x3Fu) << 6u) | ((text[3] & 0x3Fu));
	text += byte_count;
	return result[byte_count-2];
}

inline utf32 decode_and_advance_fast(utf8 **ptr) {
	return decode_and_advance_fast((utf8 const **)ptr);
}

inline utf8 *get_prev_char(utf8 *ptr, utf8 *limit) {
	while (1) {
		--ptr;
		if (ptr < limit) {
			return limit;
		}

		if ((*ptr & 0xC0) != 0x80)
			return ptr;
	}
}

inline u8 write_utf8(utf8 **dst, u32 ch) {
	if (ch <= 0x80) {
		*(*dst)++ = ch;
		return 1;
	} else if (ch <= 0x800) {
		*(*dst)++ = 0xC0 | ((ch >> 6) & 0x1f);
		*(*dst)++ = 0x80 | (ch & 0x3f);
		return 2;
	} else if (ch <= 0x10000) {
		*(*dst)++ = 0xE0 | ((ch >> 12) & 0x0f);
		*(*dst)++ = 0x80 | ((ch >>  6) & 0x3f);
		*(*dst)++ = 0x80 | (ch & 0x3f);
		return 3;
	} else {
		*(*dst)++ = 0xF0 | ((ch >> 18) & 0x07);
		*(*dst)++ = 0x80 | ((ch >> 12) & 0x3f);
		*(*dst)++ = 0x80 | ((ch >>  6) & 0x3f);
		*(*dst)++ = 0x80 | (ch & 0x3f);
		return 4;
	}
}

inline u8 write_utf8(utf8 *dst, u32 ch) {
	return write_utf8(&dst, ch);
}

inline StaticList<utf8, 4> encode_utf8(u32 ch) {
	StaticList<utf8, 4> result;
	result.count = write_utf8(result.data, ch);
	return result;
}


// NOTE: TODO: surrogate pairs ate not supported
inline List<ascii> to_ascii(Span<utf16> span, bool terminate = false, ascii unfound = '?') {
	List<ascii> result;
	result.reserve(span.count + terminate);
	for (auto u16 : span) {
		result.add(u16 > 0xFF ? unfound : (ascii)u16);
	}
	if (terminate) {
		*result.end() = 0;
	}
	return result;
}

inline Span<utf8> to_utf8(Span<ascii> span) {
	return Span((utf8 *)span.data, span.count);
}

template <class Allocator = Allocator>
inline List<utf16, Allocator> to_utf16(Span<ascii> span, bool terminate = false) {
	List<utf16, Allocator> result;
	result.reserve(span.count + terminate);
	for (auto ch : span) {
		result.add((utf16)ch);
	}
	if (terminate) {
		*result.end() = 0;
	}
	return result;
}

TL_API umm _utf8_size(Span<utf16> src);
TL_API void _to_utf8(Span<utf16> src, Span<utf8> dst);

TL_API umm _utf16_size(Span<utf8> src);
TL_API void _to_utf16(Span<utf8> src, Span<utf16> dst);

template <class Allocator = Allocator>
List<utf8, Allocator> to_utf8(Span<utf16> utf16, bool terminate = false TL_LP) {
	List<utf8, Allocator> result;

	if (utf16.count > max_value<int>)
		return {};

	auto bytes_required = _utf8_size(utf16);
	if (!bytes_required)
		return {};

	result.reserve(bytes_required + terminate TL_LA);
	result.count = bytes_required;

	_to_utf8(utf16, result);

	if (terminate)
		*result.end() = 0;

	return result;
}

template <class Allocator = Allocator>
List<utf16, Allocator> to_utf16(Span<utf8> utf8, bool terminate = false TL_LP) {
	List<utf16, Allocator> result;

	if (utf8.count > max_value<int>)
		return {};

	auto chars_required = _utf16_size(utf8);
	if (!chars_required)
		return {};

	result.reserve(chars_required + terminate TL_LA);
	result.count = chars_required;

	_to_utf16(utf8, result);

	if (terminate) {
		*result.end() = 0;
	}

	return result;
}

#ifndef TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY
#define TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY 0x1000
#endif

struct StringBuilder {
	struct Block {
		umm count = 0;
		umm capacity = 0;
		Block *prev = 0;
		Block *next = 0;
		u8 *data() { return (u8 *)(this + 1); }
		u8 *begin() { return data(); }
		u8 *end() { return data() + count; }
		umm available_space() { return capacity - count; }
		Span<u8> span() { return {data(), count}; }
	};

	Allocator allocator = {};
	Block first = {};
	u8 initial_buffer[TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY];
	Block *last = &first;
	Block *alloc_last = &first;

	StringBuilder() {
		static_assert(offsetof(StringBuilder, first) + sizeof(Block) == offsetof(StringBuilder, initial_buffer));
		allocator = TL_GET_CURRENT(allocator);
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
	*/
	u8 *reserve_contiguous_space(umm amount, bool change_count = true TL_LP) {
		while (last) {
			if (last->available_space() >= amount)
				break;
			last = last->next;
		}
		if (!last) {
			auto new_capacity = alloc_last->capacity;
			while (new_capacity < amount)
				new_capacity *= 2;
			last = allocate_block(new_capacity TL_LA);
		}

		defer {
			if (change_count) {
				memset(last->end(), 0, amount);
				last->count += amount;
			}
		};

		return last->end();
	}
	template <class T>
	T *reserve_contiguous_space(bool change_count = true TL_LP) {
		return (T *)reserve_contiguous_space(sizeof(T), change_count TL_LA);
	}
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

		umm remaining_count = dst_string.count;
		while (block && remaining_count) {
			umm bytes_to_copy = min(remaining_count, block->count);
			memcpy(dst_char, block->data(), bytes_to_copy);
			dst_char += block->count;
			block = block->next;
			remaining_count -= bytes_to_copy;
		}

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
	void for_each_block(Fn &&fn) const {
		Block *block = (Block *)&first;
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

	Block *allocate_block(umm new_capacity TL_LP) {
		auto block = (Block *)allocator.allocate_uninitialized(sizeof(Block) + new_capacity, alignof(Block) TL_LA);
		block->count = 0;
		block->capacity = new_capacity;
		block->prev = alloc_last;
		block->next = 0;
		alloc_last = alloc_last->next = block;
		return block;
	}
	Block *allocate_block(TL_LPC) {
		return allocate_block(alloc_last->capacity*2 TL_LA);
	}

	Optional<u8> pop() {
		if (!last)
			return {};

		if (!last->count)
			return {};

		last->count--;

		auto result = *last->end();

		if (last->count == 0 && last->prev)
			last = last->prev;

		return result;
	}
};

inline void free(StringBuilder &builder) {
	for (auto block = builder.first.next; block != 0;) {
		auto next = block->next;
		builder.allocator.free_t(block);
		block = next;
	}
	builder.first.next = {};
	builder.last = &builder.first;
	builder.allocator = {};
}

template <class Allocator = Allocator>
inline List<u8, Allocator> to_string(StringBuilder &builder, Allocator allocator = Allocator::current() TL_LP) {
	List<u8, Allocator> result;
	result.allocator = allocator;
	result.reserve(builder.count() TL_LA);
	result.count = result.capacity;
	builder.fill(result);
	return result;
}

forceinline umm append_bytes(StringBuilder &b, void const *_data, umm size TL_LP) {
	umm remaining = size;
	u8 *data = (u8 *)_data;
	while (remaining > b.last->available_space()) {
		umm left = b.last->available_space();
		memcpy(b.last->end(), data, left);
		remaining -= left;
		data += left;
		b.last->count += left;
		b.last = b.last->next;
		if (!b.last) {
			b.last = b.allocate_block(TL_LAC);
		}
	}
	memcpy(b.last->end(), data, remaining);
	b.last->count += remaining;
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

inline umm append(StringBuilder &builder, Empty) {
	return 0;
}
inline umm append(StringBuilder &b, ascii ch) { return append_bytes(b, ch); }
inline umm append(StringBuilder &b, utf8  ch) { return append_bytes(b, ch); }
inline umm append(StringBuilder &b, utf16 ch) { return append_bytes(b, ch); }
inline umm append(StringBuilder &b, utf32 ch) { return append_bytes(b, ch); }

template <class Size> inline umm append(StringBuilder &b, Span<u8   , Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<ascii, Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<utf8 , Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<utf16, Size> string) { return append_bytes(b, string); }
template <class Size> inline umm append(StringBuilder &b, Span<utf32, Size> string) { return append_bytes(b, string); }


forceinline umm append(StringBuilder &b, ascii const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf8  const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf16 const *string) { return append(b, as_span(string)); }
forceinline umm append(StringBuilder &b, utf32 const *string) { return append(b, as_span(string)); }

template <class T>
umm append(StringBuilder &builder, Optional<T> v) {
	if (v.has_value())
		return append(builder, v.value_unchecked());
	return append(builder, "empty");
}


template <class Value, class Error>
umm append(StringBuilder &builder, Result<Value, Error> r) {
	if (r.is_value())
		return append(builder, r.value());
	return append(builder, r.error());
}

struct SpanFormat {
	Span<u8> before;
	Span<u8> separator;
	Span<u8> after;
};

inline static const SpanFormat default_span_format = {
	.before = "{"b,
	.separator = ", "b,
	.after = "}"b,
};

template <class T, class Size>
struct FormattedSpan : SpanFormat {
	Span<T, Size> value;
};

template <class T, class Size>
FormattedSpan<T, Size> format_span(Span<T, Size> span, SpanFormat format) {
	FormattedSpan<T, Size> result;
	(SpanFormat &)result = format;
	result.value = span;
	return result;
}


template <class T, class Size>
forceinline umm append(StringBuilder &b, FormattedSpan<T, Size> formatted) {
	umm count = 0;
	count += append_bytes(b, formatted.before);
	if (formatted.value.count) {
		count += append(b, *formatted.value.data);
	}
	for (auto &val : formatted.value.skip(1)) {
		count += append_bytes(b, formatted.separator);
		count += append(b, val);
	}
	count += append_bytes(b, formatted.after);
	return count;
}

template <class T, class Size>
inline umm append(StringBuilder &b, Span<T, Size> span) {
	return append(b, format_span(span, default_span_format));
}

template <class T, class Allocator, class Size>
forceinline umm append(StringBuilder &b, List<T, Allocator, Size> list) { return append(b, format_span(list, default_span_format)); }

template <class T, class Size>
forceinline umm append(StringBuilder &b, LinearSet<T, Size> set) { return append(b, format_span(set, default_span_format)); }

template <class Allocator, class Size> forceinline umm append(StringBuilder &b, List<u8   , Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline umm append(StringBuilder &b, List<ascii, Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline umm append(StringBuilder &b, List<utf8 , Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline umm append(StringBuilder &b, List<utf16, Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline umm append(StringBuilder &b, List<utf32, Allocator, Size> list) { return append(b, as_span(list)); }

inline umm append(StringBuilder &b, StringBuilder const &that) {
	umm result = 0;
	that.for_each_block([&](StringBuilder::Block *block) {
		result += append(b, block->span());
	});
	return result;
}

template <class T>
concept AChar = OneOf<T, ascii, utf8, utf16, utf32>;

template <class T>
concept ACharSpan = OneOf<T, Span<ascii>, Span<utf8>, Span<utf16>, Span<utf32>>;

template <AChar Char>
inline umm append_format(StringBuilder &b, Span<Char> format_string) {
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

template <class T>
concept Appendable = requires (StringBuilder &builder, T const &t) {
	{ append(builder, t) } -> std::same_as<umm>;
};

template <Appendable Arg, Appendable ...Args, AChar Char>
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
template <Appendable ...Args, AChar Char>
umm append_format(StringBuilder &b, Char const *format_string, Args const &...args) {
	return append_format(b, as_span(format_string), args...);
}

inline umm append(StringBuilder &builder, bool value) {
	return append(builder, value ? "true"s : "false"s);
}

template <Appendable T, class Char>
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
			temp.allocator = TL_GET_CURRENT(temporary_allocator);

			umm appended_char_count = append(temp, format.value);
			while (appended_char_count < format.align.count) {
				appended_char_count += append(builder, format.align.fill);
			}
			append(builder, to_string(temp, TL_GET_CURRENT(temporary_allocator)));
			return 0;
		}
	} else {
		return append(builder, format.value);
	}
}

// :appendFormatFloat: 
// gcc requires this specifically in this project. I did a simplified version of this in Compiler Explorer and
// it compiled successfully WITHOUT forward declaration, so I don't know what's up with that.
// https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGEgMykrgAyeAyYAHI%2BAEaYxCBmgQAOqAqETgwe3r4BpClpjgKh4VEssfGJtpj2hQxCBEzEBFk%2BflyBdpgOGfWNBMWRMXEJHQ1NLTnttmP9YYNlw4kAlLaoXsTI7BwEmCxJBjsA1Cb%2BbsgGCgqHACon2CYaAIIKBMReDocAYqioxwDsVieh2BN0OBBOgMeJj%2BABEIQ8oU8dnsDphjqdzkxLjc7giXm8PhZGv9ISDQeD/JDoXDKQiEQA3VB4dCHJhJJKuCDRH60JYk6l0pG7fZMI4nM4XK63fz3J6M5mHADuxEImAA%2Bli1fiwsAIN9UOLpdhDlQ%2BdDSSC2RyGOgIETiOaqAA6cGwpbwp4Cz1ClGitHizHYo0Mpksq2c%2B2Gu6HaJmgEIsl02GCx5hAiHFhMMIQOMW4HK1UahRa146vU/c1canu2me2EcFa0TgAVl4fg4WlIqE4bms1kOCjWG39iR4pAImgbKwA1iB/H8nf5583JH8AByJMwATj%2BXC4zf0nEkvBYEg0GlI7c73Y4vAUIAvE47DdIcFgMEQKFQezocXIlDQH96HiYApDMPg6B2Yh7y5SdSGiMJGgAT04McEOYYgkIAeWibQuifMdALYQQsIYWgUOfUgsGiLxgDcMRaHvbheCwTMjHESj8GIfC8HpTAmM7TBVC6LwdlQ3g02qODaDwaJiGQjwsDg0tT2Y0g%2BOIbklBhXZDGAGSjEnFYqAMYAFAANTwTBFSw61xJkQQRDEdgpAc%2BQlDUODdC4fQ9JQPtLH0WT70gFZUCSWomIAWhedAThhUxLGsRJeFQDSVSwEKcyqGoMhcG0JjaIIbQGUpyj0fJ0gEQqKtSKqGFKoZ4h8zpugEXpxk8Vo9Fa2oOtmEomp6mYapamZGoWZqVkHdZNgkRsWzbOCb0OVQ1wANii9bJEOYBkGQQ4pCdMxDggXBCBIY5RyWXgny0JYZwSA8mw4Y9SFPZsLyvVLODvB9xyM18PwgJA1gIJJRP/CBAKSX9iAiVgtjWzbtt2/bDskY7eEwfAiAyvR%2BEc0RxFcwn3JUdRKO80hlTZcSFo4VtL2WzgsNEiH01QKhVo2radr2g6jpOiAPCAuIrv8LgboB58HtIBBMCYLB4myl63o%2Br6WdvWx/ruqdSFnMw10XSRJDXDQzb%2BMxmytrhKhe/wlsom9bsBt8oGBpAYbhqHveAkB6WQdk1XpLgtzVSQNDVcyhC4P41S4KPVG2iDaCgmDojg9DkPs7PMJwvCHHsojGAIUjyLg6jaPo2hGPs1i9I4zsuJ4viBOx4TkFErYx0kl7OxkuSFIwLZOxU%2ByNK0zAdLY/SdUBkymDMyzrNsxh7LJpySekMnFApryEl8wzEqsQLB6ysKIoyJiAHpYv8pLLFRF5UvS5l%2BPgabqh45wIFcUbiroAmuVHylVagALARkYBwwWrfzanUEaXVJg5R/ggvo0DmrTD6AAl46C5hlRgdNIcc0paHkZk7a8nBVqoxYAoQOhxQ5bidJHQ4Mc45OkTqdc6eMJZS1drLFYCslbDGyobdaToLZmFNs2fwGgbaSC3M9I8J4QCfWZs7X6OtHxu09iAMGHNfbflhsBBGbBODJx2rQ%2BhjDmEaFYbHBcidsa4xIMyAmsgt4uR3rIPenkqaH1pkkemZCmbfS7KzdmolDhc2oZYuhB0bEsLYY4uxIsjFwwlmYaWes5ZCOVpQBm6tVGaw0dre82iBEGznJjSWfxtp/FkZIdoGg6lkMduoyh2sckMzMBQn6XSjIrA0mkZwkggA%3D%3D
template <class Float>
inline umm append(StringBuilder &builder, FormatFloat<Float> format);

template <class Int, umm capacity>
umm write_as_string(StaticList<ascii, capacity> &buffer, FormatInt<Int> f) {
	Int v = f.value;
	auto radix = convert<Int>(f.radix);
	constexpr u32 maxDigits = _intToStringSize<Int>;
	ascii buf[maxDigits];
	auto charMap = f.char_set;
	ascii *lsc = buf + maxDigits - 1;
	u32 charsWritten = 0;
	Span<ascii> suffix = {};

	switch (f.format) {
		case IntFormat_full:
			break;
		case IntFormat_kmb: {
			if (v >= 1000) {
				f64 f = (f64)v;
				scoped(temporary_allocator_and_checkpoint);
				StringBuilder builder;
				// :appendFormatFloat:
				append(builder, FormatFloat{.value = f, .precision = 3, .format = FloatFormat_kmb});
				buffer.add((Span<ascii>)builder.first.span());
				return builder.first.span().count;
			}
			break;
		}
	}

	bool negative = false;
	if constexpr (is_signed<Int>) {
		if (v < 0) {
			negative = true;
			if constexpr (std::is_same_v<Int, s8> || std::is_same_v<Int, s16> || std::is_same_v<Int, s32> || std::is_same_v<Int, s64>) {
				if (v == min_value<Int>) {
					*lsc-- = charMap[(u32)-(v % radix)];
					v /= radix;
					++charsWritten;
				}
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
	buffer.add(suffix);
	return charsWritten + suffix.count;
}
template <class Int, umm capacity> requires is_integer<Int>
umm write_as_string(StaticList<ascii, capacity> &buffer, Int v) {
	return write_as_string(buffer, FormatInt{.value = v});
}

template <class Int>
umm append(StringBuilder &builder, FormatInt<Int> f) {
	StaticList<ascii, _intToStringSize<Int>> buffer;
	write_as_string(buffer, f);
	return append(builder, buffer.span());
}

template <class Int> requires is_integer<Int>
umm append(StringBuilder &builder, Int v) {
	return append(builder, FormatInt{.value = v});
}

forceinline umm append(StringBuilder &builder, void const *p) {
	return append(builder, FormatInt{.value = (umm)p, .radix = 16, .leading_zero_count = sizeof(void *) * 2});
}

template <class Float>
inline umm append_float(StringBuilder &builder, FormatFloat<Float> format) {
	auto value = format.value;

	if (is_nan(value)) {
		return append(builder, "NaN");
	}

	scoped_replace(default_int_format_format, IntFormat_full);

	auto precision = format.precision;

	StaticList<ascii, 128> buffer;

	bool negative = is_negative(value);

	if (negative) {
		value = -value;
	}

	if (value == infinity<Float>) {
		return append_format(builder, "{}inf", negative ? "-" : "");
	}

	auto append_float = [&](Float f) {
		auto whole_part = (u64)f;

		auto round_and_trim = [&] {
			f = frac(f);
			if (f != 0) {
				if (f >= 0.5f) {
					auto it = buffer.end() - 1;
					while (1) {
						*it += 1;
						if (*it == (ascii)('9' + 1)) {
							buffer.pop_back();
							--it;
							if (*it == '.') {
								++whole_part;
								break;
							} else {
								continue;
							}
						}
						break;
					}
				}
			}
			while (buffer.back() == '0') {
				buffer.pop_back();
			}
			if (buffer.back() == '.') {
				buffer.pop_back();
			}
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
			if (precision) {
				if (frac(f) != 0) {
					buffer.add('.');
					for (u32 i = 0; i < precision && f != 0; ++i) {
						f = frac(f) * 10;
						buffer.add((ascii)((u32)f + '0'));
					}
					round_and_trim();
				}
			}
		}

		StaticList<ascii, 128> whole_part_buffer;
		if (negative)
			whole_part_buffer.add('-');
		write_as_string(whole_part_buffer, whole_part);
		buffer.insert_at(whole_part_buffer, 0);
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
		case FloatFormat_kmb: {
			Span<ascii> suffix = {};
			if (value >= 1000) { value /= 1000; suffix = "k"s; }
			if (value >= 1000) { value /= 1000; suffix = "m"s; }
			if (value >= 1000) { value /= 1000; suffix = "b"s; }
			if (value >= 1000) { value /= 1000; suffix = "t"s; }
			if (value >= 1000) { value /= 1000; suffix = "q"s; }
			if (value > 100) { precision = (u32)max((s32)precision - 3, 0); }
			else if (value > 10) { precision = (u32)max((s32)precision - 2, 0); }
			else if (value > 1) { precision = (u32)max((s32)precision - 1, 0); }
			append_float(value);
			buffer.add(suffix);
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

struct OnlyFileAndLine : std::source_location {};

inline umm append(StringBuilder &builder, OnlyFileAndLine location) {
	return append_format(builder, "{}:{}", location.file_name(), location.line());
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
inline constexpr bool are_utf8 = are_utf8_t<Args...>::value;


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
inline constexpr bool are_utf16 = are_utf16_t<Args...>::value;


template <Appendable ...Args>
inline auto concatenate(Args const &...args) {
	StringBuilder builder;
	int _ = ((append(builder, args), ...), 0);
	(void)_;
	if constexpr (are_utf8<Args...>) {
		return (List<utf8>)to_string(builder, TL_GET_CURRENT(allocator));
	} else if constexpr (are_utf16<Args...>) {
		return (List<utf16>)to_string(builder, TL_GET_CURRENT(allocator));
	} else {
		return (List<u8>)to_string(builder, TL_GET_CURRENT(allocator));
	}
}

template <Appendable ...Args>
inline auto tconcatenate(Args &&...args) {
	return TL_TMP(concatenate(args...));
}

template <class Allocator = Allocator, Appendable ...Args>
List<ascii, Allocator> aformat(Allocator allocator, Span<ascii> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = TL_GET_CURRENT(temporary_allocator);
	append_format(builder, fmt, args...);
	return (List<ascii, Allocator>)to_string<Allocator>(builder, allocator);
}

template <class Allocator = Allocator, Appendable ...Args>
List<utf8, Allocator> aformat(Allocator allocator, Span<utf8> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = TL_GET_CURRENT(temporary_allocator);
	append_format(builder, fmt, args...);
	return (List<utf8, Allocator>)to_string<Allocator>(builder, allocator);
}

template <class Allocator = Allocator, Appendable ...Args>
List<utf16, Allocator> aformat(Allocator allocator, Span<utf16> fmt, Args const &...args) {
	StringBuilder builder;
	builder.allocator = TL_GET_CURRENT(temporary_allocator);
	append_format(builder, fmt, args...);
	return (List<utf16, Allocator>)to_string<Allocator>(builder, allocator);
}

template <class Allocator = Allocator, Appendable ...Args> List<ascii, Allocator> aformat(Allocator allocator, ascii const *fmt, Args const &...args) { return aformat<Allocator>(allocator, as_span(fmt), args...); }
template <class Allocator = Allocator, Appendable ...Args> List<utf8 , Allocator> aformat(Allocator allocator, utf8  const *fmt, Args const &...args) { return aformat<Allocator>(allocator, as_span(fmt), args...); }
template <class Allocator = Allocator, Appendable ...Args> List<utf16, Allocator> aformat(Allocator allocator, utf16 const *fmt, Args const &...args) { return aformat<Allocator>(allocator, as_span(fmt), args...); }

template <class Allocator = Allocator, Appendable ...Args>
List<ascii, Allocator> format(Span<ascii> fmt, Args const &...args) {
	return aformat<Allocator>(Allocator::current(), fmt, args...);
}

template <class Allocator = Allocator, Appendable ...Args>
List<utf8, Allocator> format(Span<utf8> fmt, Args const &...args) {
	return aformat<Allocator>(Allocator::current(), fmt, args...);
}

template <class Allocator = Allocator, Appendable ...Args>
List<utf16, Allocator> format(Span<utf16> fmt, Args const &...args) {
	return aformat<Allocator>(Allocator::current(), fmt, args...);
}

template <class Allocator = Allocator, Appendable ...Args> List<ascii, Allocator> format(ascii const *fmt, Args const &...args) { return aformat<Allocator>(Allocator::current(), as_span(fmt), args...); }
template <class Allocator = Allocator, Appendable ...Args> List<utf8 , Allocator> format(utf8  const *fmt, Args const &...args) { return aformat<Allocator>(Allocator::current(), as_span(fmt), args...); }
template <class Allocator = Allocator, Appendable ...Args> List<utf16, Allocator> format(utf16 const *fmt, Args const &...args) { return aformat<Allocator>(Allocator::current(), as_span(fmt), args...); }

template <Appendable ...Args> List<ascii> tformat(ascii const *fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }
template <Appendable ...Args> List<utf8 > tformat(utf8  const *fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }
template <Appendable ...Args> List<utf16> tformat(utf16 const *fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }
template <Appendable ...Args> List<utf32> tformat(utf32 const *fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }

template <Appendable ...Args> List<ascii> tformat(Span<ascii> fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }
template <Appendable ...Args> List<utf8 > tformat(Span<utf8 > fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }
template <Appendable ...Args> List<utf16> tformat(Span<utf16> fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }
template <Appendable ...Args> List<utf32> tformat(Span<utf32> fmt, Args const &...args) { return TL_TMP(format(fmt, args...)); }

template <class Allocator = Allocator, Appendable T>
List<utf8, Allocator> to_string(T const &value) {
	StringBuilder builder;
	builder.allocator = TL_GET_CURRENT(temporary_allocator);
	append(builder, value);

	return (List<utf8, Allocator>)to_string<Allocator>(builder, Allocator::current());
}

struct FormattedBytes {
	f32 count;
	u8 unit;
	bool kilo_is_1024;
};

struct FormatBytesParams {
	bool kilo_is_1024 = true;
};

inline FormattedBytes format_bytes(auto byte_count, FormatBytesParams params = {}) {
	FormattedBytes result{};

	result.kilo_is_1024 = params.kilo_is_1024;

	f64 kilo = params.kilo_is_1024 ? 1024 : 1000;

	f64 count = (f64)byte_count;
	while (count > kilo) {
		count /= kilo;
		result.unit += 1;
	}
	result.count = (f32)count;

	return result;
}

inline umm append(StringBuilder &builder, FormattedBytes bytes) {
	static constexpr Span<ascii> unit_strings[2][7] = {
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

#ifdef TL_IMPL

thread_local IntFormat default_int_format_format = IntFormat_full;
thread_local u32 default_int_format_radix = 10;
thread_local u32 default_int_format_leading_zero_count = 0;
thread_local u32 default_int_format_skip_digits = 0;
thread_local ascii const *default_int_format_char_set = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

thread_local u32 default_float_format_precision = 3;
thread_local FloatFormat default_float_format_format = FloatFormat_default;
thread_local bool default_float_format_trailing_zeros = false;

#if OS_WINDOWS
umm _utf8_size(Span<utf16> src) {
	return (umm)WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)src.data, (int)src.count, 0, 0, 0, 0);
}
void _to_utf8(Span<utf16> src, Span<utf8> dst) {
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)src.data, (int)src.count, (char *)dst.data, (int)dst.count, 0, 0);
}

umm _utf16_size(Span<utf8> src) {
	return (umm)MultiByteToWideChar(CP_UTF8, 0, (char *)src.data, (int)src.count, 0, 0);
}
void _to_utf16(Span<utf8> src, Span<utf16> dst) {
	MultiByteToWideChar(CP_UTF8, 0, (char *)src.data, (int)src.count, (wchar_t *)dst.data, (int)dst.count);
}

#elif OS_LINUX
umm _utf8_size(Span<utf16> src) {
	not_implemented();
}
void _to_utf8(Span<utf16> src, Span<utf8> dst) {
	not_implemented();
}

umm _utf16_size(Span<utf8> src) {
	not_implemented();
}
void _to_utf16(Span<utf8> src, Span<utf16> dst) {
	not_implemented();
}
#endif

#endif

}

#pragma warning(pop)
