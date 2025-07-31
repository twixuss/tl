#ifndef _TL_STRING_H
#define _TL_STRING_H

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
void append(StringBuilder &builder, YourType t) {
	return append_format(builder, "({} {})", t.first, t.second);
}
*/
// Note that a lot of other functions like `print` or `format` use `scoped(temporary_storage_checkpoint)` for better memory utilization.
// With that make sure you don't make temporary allocations and use them after `append`.
//

#include "common.h"
#include "list.h"
#include "static_list.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4702) // unreachable code

namespace tl {

template <class Allocator = Allocator, class T, class Size>
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
TL_API umm _to_utf8(Span<utf16> src, Span<utf8> dst);

TL_API umm _utf16_size(Span<utf8> src);
TL_API umm _to_utf16(Span<utf8> src, Span<utf16> dst);

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

template <class Allocator = Allocator>
List<utf32, Allocator> to_utf32(Span<utf8> utf8, bool terminate = false, utf32 unfound = '?' TL_LP) {
	List<utf32, Allocator> result;
	auto c = utf8.data;
	while (1) {
		if (c >= utf8.end())
			break;
		auto code = decode_and_advance(&c);
		if (code.has_value()) {
			result.add(code.value());
		} else {
			result.add(unfound);
			++c;
		}
	}
	if (terminate) {
		result.add(0);
		result.count--;
	}
	return result;
}

template <class Allocator = Allocator>
List<utf8, Allocator> to_utf8(Span<utf32> str, bool terminate = false TL_LP) {
	List<utf8, Allocator> result;
	auto c = str.data;
	for (auto code : str) {
		result.add(encode_utf8(code).span());
	}
	if (terminate) {
		result.add(0);
		result.count--;
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

	Allocator allocator = TL_GET_CURRENT(allocator);
	Block first = {.capacity = TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY};
	u8 initial_buffer[TL_STRING_BUILDER_INITIAL_BUFFER_CAPACITY];
	Block *last = &first;
	Block *alloc_last = &first;

	// You can't use this static_assert in struct scope that uses said struct. Didn't know that? Really? SMH
private:
	static void static_asserts() {
		static_assert(offsetof(StringBuilder, first) + sizeof(Block) == offsetof(StringBuilder, initial_buffer));
	}
public:

	StringBuilder() = default;
	StringBuilder(Allocator allocator) : allocator(allocator) {}
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
	void for_each_block(Fn &&in_fn) const {
		Block *block = (Block *)&first;

		auto fn = wrap_foreach_fn<Block *>(in_fn);

		do {
			if (block->count) {
				auto d = fn(block);
				switch (d) {
					case ForEach_continue: break;
					case ForEach_break: return;
					default: invalid_code_path("not supported");
				}
			}
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

forceinline void append_bytes(StringBuilder &b, void const *_data, umm size TL_LP) {
	u8 *data = (u8 *)_data;
	while (size > b.last->available_space()) {
		umm left = b.last->available_space();
		memcpy(b.last->end(), data, left);
		size -= left;
		data += left;
		b.last->count += left;
		b.last = b.last->next;
		if (!b.last) {
			b.last = b.allocate_block(TL_LAC);
		}
	}
	memcpy(b.last->end(), data, size);
	b.last->count += size;
}

template <class T>
forceinline void append_bytes(StringBuilder &b, T const &value TL_LP) {
	return append_bytes(b, &value, sizeof(value) TL_LA);
}

template <class T, class Size>
forceinline void append_bytes(StringBuilder &b, Span<T, Size> span TL_LP) {
	return append_bytes(b, span.data, span.count * sizeof(T) TL_LA);
}

template <class T, class Allocator, class Size>
forceinline void append_bytes(StringBuilder &b, List<T, Allocator, Size> list TL_LP) {
	return append_bytes(b, list.data, list.count * sizeof(T) TL_LA);
}

inline void append(StringBuilder &builder, Empty) {}
inline void append(StringBuilder &b, ascii ch) { return append_bytes(b, ch); }
inline void append(StringBuilder &b, utf8  ch) { return append_bytes(b, ch); }
inline void append(StringBuilder &b, utf16 ch) { return append_bytes(b, ch); }
inline void append(StringBuilder &b, utf32 ch) { return append_bytes(b, ch); }

template <class Size> inline void append(StringBuilder &b, Span<u8   , Size> string) { return append_bytes(b, string); }
template <class Size> inline void append(StringBuilder &b, Span<ascii, Size> string) { return append_bytes(b, string); }
template <class Size> inline void append(StringBuilder &b, Span<utf8 , Size> string) { return append_bytes(b, string); }
template <class Size> inline void append(StringBuilder &b, Span<utf16, Size> string) { return append_bytes(b, string); }
template <class Size> inline void append(StringBuilder &b, Span<utf32, Size> string) { return append_bytes(b, string); }


forceinline void append(StringBuilder &b, ascii const *string) { return append(b, as_span(string)); }
forceinline void append(StringBuilder &b, utf8  const *string) { return append(b, as_span(string)); }
forceinline void append(StringBuilder &b, utf16 const *string) { return append(b, as_span(string)); }
forceinline void append(StringBuilder &b, utf32 const *string) { return append(b, as_span(string)); }

template <class T>
void append(StringBuilder &builder, Optional<T> v) {
	if (v.has_value())
		return append(builder, v.value_unchecked());
	return append(builder, "empty");
}


template <class Value, class Error>
void append(StringBuilder &builder, Result<Value, Error> r) {
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
forceinline void append(StringBuilder &b, FormattedSpan<T, Size> formatted) {
	append_bytes(b, formatted.before);
	if (formatted.value.count) {
		append(b, *formatted.value.data);
	}
	for (auto &val : formatted.value.skip(1)) {
		append_bytes(b, formatted.separator);
		append(b, val);
	}
	append_bytes(b, formatted.after);
}

template <class T, class Size>
inline void append(StringBuilder &b, Span<T, Size> span) {
	return append(b, format_span(span, default_span_format));
}

template <class T, class Allocator, class Size>
forceinline void append(StringBuilder &b, List<T, Allocator, Size> list) { return append(b, format_span(list, default_span_format)); }

template <class Allocator, class Size> forceinline void append(StringBuilder &b, List<u8   , Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline void append(StringBuilder &b, List<ascii, Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline void append(StringBuilder &b, List<utf8 , Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline void append(StringBuilder &b, List<utf16, Allocator, Size> list) { return append(b, as_span(list)); }
template <class Allocator, class Size> forceinline void append(StringBuilder &b, List<utf32, Allocator, Size> list) { return append(b, as_span(list)); }

inline void append(StringBuilder &b, StringBuilder const &that) {
	that.for_each_block([&](StringBuilder::Block *block) {
		append(b, block->span());
	});
}

template <class T>
concept AChar = OneOf<T, ascii, utf8, utf16, utf32>;

template <class T>
concept ACharSpan = OneOf<T, Span<ascii>, Span<utf8>, Span<utf16>, Span<utf32>>;

template <class T>
inline umm append_and_return_width(StringBuilder &builder, T const &it) {
	umm initial_count = builder.count();
	append(builder, it);
	return builder.count() - initial_count;
}

template <AChar Char>
inline void append_format(StringBuilder &b, Span<Char> format_string) {
	Char previous = {};
	auto start = format_string.data;
	auto c = start;
	auto end = format_string.end();

	for (;c != end; ++c) {
		auto next_prev = *c;
		defer { previous = next_prev; };
		if (previous == '{') {
			switch (*c) {
				case '{':
					append(b, Span(start, c));
					start = c + 1;
					next_prev = 0;
					break;
				case '}': invalid_code_path("not enough arguments were provided for 'append_format'"); break;
				default:  invalid_code_path("bad format string: only '{' or '}' can follow '{'"); break;
			}
		} else if (previous == '}') {
			if (*c == '}') {
				append(b, Span(start, c));
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
	append(b, Span(start, end));
}

template <class T>
concept Appendable = requires (StringBuilder &builder, T const &t) {
	{ append(builder, t) } -> std::same_as<void>;
};

template <Appendable Arg, Appendable ...Args, AChar Char>
void append_format(StringBuilder &b, Span<Char> format_string, Arg const &arg, Args const &...args) {
	Char previous = {};
	auto start = format_string.data;
	auto c = start;
	auto end = format_string.end();
	for (;c != end; ++c) {
		auto next_prev = *c;
		defer { previous = next_prev; };
		if (previous == '{') {
			switch (*c) {
				case '{':
					append(b, Span(start, c));
					start = c + 1;
					next_prev = 0;
					break;
				case '}':
					append(b, Span(start, c - 1));
					append(b, arg);
					append_format(b, Span(c + 1, end), args...);
					start = end;
					c = end;
					return;
				default:
					invalid_code_path("bad format string: only '{' or '}' can follow '{'");
					break;
			}
		} else if (previous == '}') {
			if (*c == '}') {
				append(b, Span(start, c));
				start = c + 1;
				next_prev = 0;
			} else {
				invalid_code_path("bad format string: only '}' can follow '}'");
			}
		}
	}
	invalid_code_path("Too many arguments provided for this format string");
}
template <Appendable ...Args, AChar Char>
void append_format(StringBuilder &b, Char const *format_string, Args const &...args) {
	return append_format(b, as_span(format_string), args...);
}

inline void append(StringBuilder &builder, bool value) {
	return append(builder, value ? "true"s : "false"s);
}

template <Appendable T, class Char>
void append(StringBuilder &builder, Format<T, Char> format) {
	if (format.align.count) {
		if (format.align.kind == FormatAlign_left) {
			umm appended_char_count = append_and_return_width(builder, format.value);

			if (appended_char_count < format.align.count) {
				append(builder, Repeat{format.align.fill, format.align.count - appended_char_count});
			}
		} else {
			StringBuilder temp;
			temp.allocator = TL_GET_CURRENT(temporary_allocator);

			umm appended_char_count = append_and_return_width(temp, format.value);
			if (appended_char_count < format.align.count) {
				append(builder, Repeat{format.align.fill, format.align.count - appended_char_count});
			}
			append(builder, temp);
		}
	} else {
		append(builder, format.value);
	}
}

// :appendFormatFloat: 
// gcc requires this specifically in this project. I did a simplified version of this in Compiler Explorer and
// it compiled successfully WITHOUT forward declaration, so I don't know what's up with that.
// https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGEgMykrgAyeAyYAHI%2BAEaYxCBmgQAOqAqETgwe3r4BpClpjgKh4VEssfGJtpj2hQxCBEzEBFk%2BflyBdpgOGfWNBMWRMXEJHQ1NLTnttmP9YYNlw4kAlLaoXsTI7BwEmCxJBjsA1Cb%2BbsgGCgqHACon2CYaAIIKBMReDocAYqioxwDsVieh2BN0OBBOgMeJj%2BABEIQ8oU8dnsDphjqdzkxLjc7giXm8PhZGv9ISDQeD/JDoXDKQiEQA3VB4dCHJhJJKuCDRH60JYk6l0pG7fZMI4nM4XK63fz3J6M5mHADuxEImAA%2Bli1fiwsAIN9UOLpdhDlQ%2BdDSSC2RyGOgIETiOaqAA6cGwpbwp4Cz1ClGitHizHYo0Mpksq2c%2B2Gu6HaJmgEIsl02GCx5hAiHFhMMIQOMW4HK1UahRa146vU/c1canu2me2EcFa0TgAVl4fg4WlIqE4bms1kOCjWG39iR4pAImgbKwA1iB/H8nf5583JH8AByJMwATj%2BXC4zf0nEkvBYEg0GlI7c73Y4vAUIAvE47DdIcFgMEQKFQezocXIlDQH96HiYApDMPg6B2Yh7y5SdSGiMJGgAT04McEOYYgkIAeWibQuifMdALYQQsIYWgUOfUgsGiLxgDcMRaHvbheCwTMjHESj8GIfC8HpTAmM7TBVC6LwdlQ3g02qODaDwaJiGQjwsDg0tT2Y0g%2BOIbklBhXZDGAGSjEnFYqAMYAFAANTwTBFSw61xJkQQRDEdgpAc%2BQlDUODdC4fQ9JQPtLH0WT70gFZUCSWomIAWhedAThhUxLGsRJeFQDSVSwEKcyqGoMhcG0JjaIIbQGUpyj0fJ0gEQqKtSKqGFKoZ4h8zpugEXpxk8Vo9Fa2oOtmEomp6mYapamZGoWZqVkHdZNgkRsWzbOCb0OVQ1wANii9bJEOYBkGQQ4pCdMxDggXBCBIY5RyWXgny0JYZwSA8mw4Y9SFPZsLyvVLODvB9xyM18PwgJA1gIJJRP/CBAKSX9iAiVgtjWzbtt2/bDskY7eEwfAiAyvR%2BEc0RxFcwn3JUdRKO80hlTZcSFo4VtL2WzgsNEiH01QKhVo2radr2g6jpOiAPCAuIrv8LgboB58HtIBBMCYLB4myl63o%2Br6WdvWx/ruqdSFnMw10XSRJDXDQzb%2BMxmytrhKhe/wlsom9bsBt8oGBpAYbhqHveAkB6WQdk1XpLgtzVSQNDVcyhC4P41S4KPVG2iDaCgmDojg9DkPs7PMJwvCHHsojGAIUjyLg6jaPo2hGPs1i9I4zsuJ4viBOx4TkFErYx0kl7OxkuSFIwLZOxU%2ByNK0zAdLY/SdUBkymDMyzrNsxh7LJpySekMnFApryEl8wzEqsQLB6ysKIoyJiAHpYv8pLLFRF5UvS5l%2BPgabqh45wIFcUbiroAmuVHylVagALARkYBwwWrfzanUEaXVJg5R/ggvo0DmrTD6AAl46C5hlRgdNIcc0paHkZk7a8nBVqoxYAoQOhxQ5bidJHQ4Mc45OkTqdc6eMJZS1drLFYCslbDGyobdaToLZmFNs2fwGgbaSC3M9I8J4QCfWZs7X6OtHxu09iAMGHNfbflhsBBGbBODJx2rQ%2BhjDmEaFYbHBcidsa4xIMyAmsgt4uR3rIPenkqaH1pkkemZCmbfS7KzdmolDhc2oZYuhB0bEsLYY4uxIsjFwwlmYaWes5ZCOVpQBm6tVGaw0dre82iBEGznJjSWfxtp/FkZIdoGg6lkMduoyh2sckMzMBQn6XSjIrA0mkZwkggA%3D%3D
template <class Float>
inline void append(StringBuilder &builder, FormatFloat<Float> format);

template <class Int, umm capacity>
void write_as_string(StaticList<ascii, capacity> &buffer, FormatInt<Int> f) {
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
				return;
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
}
template <class Int, umm capacity> requires is_integer<Int>
void write_as_string(StaticList<ascii, capacity> &buffer, Int v) {
	write_as_string(buffer, FormatInt{.value = v});
}

template <class Int>
void append(StringBuilder &builder, FormatInt<Int> f) {
	StaticList<ascii, _intToStringSize<Int>> buffer;
	write_as_string(buffer, f);
	append(builder, buffer.span());
}

template <class Int> requires is_integer<Int>
void append(StringBuilder &builder, Int v) {
	append(builder, FormatInt{.value = v});
}

struct FormatHexOptions {
	bool upper_case = false;
};

template <class Int>  requires is_integer<Int>
auto format_hex(Int value, FormatHexOptions options = {}) {
	return FormatInt{
		.value = (std::make_unsigned_t<Int>)value,
		.radix = 16, 
		.leading_zero_count = sizeof(Int) * 2,
		.char_set = options.upper_case ?
			"0123456789ABCDEF" :
			"0123456789abcdef"
	};
}

// TODO: I need to come up with something thats more modular than this.
//       Formatting nested elements is annoying currently because it 
//       requires all these specializations
template <class T>
struct HexSpan {
	Span<T> span;
};

template <class Int>  requires is_integer<Int>
auto format_hex(Span<Int> value) {
	return HexSpan{value};
}

template <class T>
inline void append(StringBuilder &builder, HexSpan<T> hex_span) {
	for (auto &t : hex_span.span) {
		append(builder, format_hex(t));
	}
}

forceinline void append(StringBuilder &builder, void const *p) {
	append(builder, FormatInt{.value = (umm)p, .radix = 16, .leading_zero_count = sizeof(void *) * 2});
}

template <class Float>
inline void append(StringBuilder &builder, FormatFloat<Float> format) {
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

forceinline void append(StringBuilder &builder, f64 v) { append(builder, FormatFloat{.value = v}); }
forceinline void append(StringBuilder &builder, f32 v) { append(builder, FormatFloat{.value = v}); }

inline void append(StringBuilder &builder, std::source_location location) {
	append_format(builder, "{}:{}:{}: {}", location.file_name(), location.line(), location.column(), location.function_name());
}

struct OnlyFileAndLine : std::source_location {};

inline void append(StringBuilder &builder, OnlyFileAndLine location) {
	append_format(builder, "{}:{}", location.file_name(), location.line());
}


inline StaticList<char, 4> escape_c_character(char ch) {
	StaticList<char, 4> result;
	switch (ch) {
		case '\x0': result.add("\\x0"s); break;
		case '\x1': result.add("\\x1"s); break;
		case '\x2': result.add("\\x2"s); break;
		case '\x3': result.add("\\x3"s); break;
		case '\x4': result.add("\\x4"s); break;
		case '\x5': result.add("\\x5"s); break;
		case '\x6': result.add("\\x6"s); break;
		case '\a': result.add("\\a"s); break;
		case '\b': result.add("\\b"s); break;
		case '\t': result.add("\\t"s); break;
		case '\n': result.add("\\n"s); break;
		case '\v': result.add("\\v"s); break;
		case '\f': result.add("\\f"s); break;
		case '\r': result.add("\\r"s); break;
		case '\x0e': result.add("\\x0e"s); break;
		case '\x0f': result.add("\\x0f"s); break;
		case '\x10': result.add("\\x10"s); break;
		case '\x11': result.add("\\x11"s); break;
		case '\x12': result.add("\\x12"s); break;
		case '\x13': result.add("\\x13"s); break;
		case '\x14': result.add("\\x14"s); break;
		case '\x15': result.add("\\x15"s); break;
		case '\x16': result.add("\\x16"s); break;
		case '\x17': result.add("\\x17"s); break;
		case '\x18': result.add("\\x18"s); break;
		case '\x19': result.add("\\x19"s); break;
		case '\x1a': result.add("\\x1a"s); break;
		case '\x1b': result.add("\\x1b"s); break;
		case '\x1c': result.add("\\x1c"s); break;
		case '\x1d': result.add("\\x1d"s); break;
		case '\x1e': result.add("\\x1e"s); break;
		case '\x1f': result.add("\\x1f"s); break;
		case '\\': result.add("\\\\"s); break;
		default:
			result.add(ch);
			break;
	}
	return result;
}

template <umm buffer_capacity = 4096>
inline void escape_c_string(Span<utf8> string, auto write) {
	StaticList<utf8, buffer_capacity> buffer;
	for (auto ch : string) {
		auto escaped = escape_c_character((char)ch);
		for (auto ch : escaped) {
			buffer.add(ch); 
			if (buffer.count == buffer.capacity) {
				write(buffer.span());
				buffer.clear();
			}
		}
	}
	write(buffer.span());
}

struct EscapedCString {
	Span<utf8> unescaped_string;
};

inline void append(StringBuilder &builder, EscapedCString string) {
	escape_c_string(string.unescaped_string, [&](auto s) { append(builder, s); });
}

enum class UnescapeOneCCharacterError {
	empty_string,
	unfinished_escape_sequence,
};
// Parses one character from str, advances str
// Supported:
//     basic char
//     \n \r \t ...
//     \xFF
// Unsupported: TODO:
//     universal character name \u...
inline Result<char, UnescapeOneCCharacterError> unescape_one_c_character(Span<char> *str) {
	char *c = str->data;
	char *end = str->end();
	defer { str->set_begin(c); };

	if (c == end) {
		return UnescapeOneCCharacterError::empty_string;
	}

	if (*c == '\\') {
		++c;
		if (c == end) {
			return UnescapeOneCCharacterError::unfinished_escape_sequence;
		}
		switch (*c) {
			case 'a': ++c; return '\a';
			case 'b': ++c; return '\b';
			case 'f': ++c; return '\f';
			case 'n': ++c; return '\n';
			case 'r': ++c; return '\r';
			case 't': ++c; return '\t';
			case 'v': ++c; return '\v';
			case '\\': ++c; return '\\';
			case 'x': case 'X': {
				char x = 0;
				while (c < end && is_hex_digit(*c)) {
					x = (x << 4) | hex_digit_to_int_unchecked(*c);
					++c;
				}
				return x;
			}
			default: 
				return *c;
		}
	} else {
		return *c++;
	}
}

template <umm buffer_capacity = 4096>
inline void unescape_c_string(Span<char> string, auto write) {
	StaticList<char, buffer_capacity> buffer;
	while (string.count) {
		buffer.add(unescape_one_c_character(&string).value());
		if (buffer.count == buffer.capacity) {
			write(buffer.span());
			buffer.clear();
		}
	}
	write(buffer.span());
}

struct HexCString {
	Span<utf8> string;
};

inline void append(StringBuilder &builder, HexCString string) {
	for (auto c : string.string) {
		append(builder, "\\x"s);
		append(builder, format_hex(c));
	}
}

template <class T>
inline void append(StringBuilder &builder, Repeat<T> r) {
	for (umm i = 0; i < r.count; ++i)
		append(builder, r.value);
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


template <class Allocator = Allocator, Appendable ...Args>
inline auto concatenate(Args const &...args) {
	StringBuilder builder;
	int _ = ((append(builder, args), ...), 0);
	(void)_;
	if constexpr (are_utf8<Args...>) {
		return (List<utf8>)to_string<Allocator>(builder);
	} else if constexpr (are_utf16<Args...>) {
		return (List<utf16>)to_string<Allocator>(builder);
	} else {
		return (List<u8>)to_string<Allocator>(builder);
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

struct NumberAbbreviation {
	f64 value;
	Span<utf8> between_value_and_unit = u8" "s;
	u8 unit;
	Span<Span<utf8>> const *unit_table;
};

struct NumberAbbreviationUnits {
	f64 divisor;
	Span<Span<utf8>> unit_table;
};

struct NumberAbbreviationOptions {
	Span<utf8> between_value_and_unit = u8" "s;
};

inline static constexpr Span<utf8> abbrev_k_strings[] = {u8""s, u8"K"s, u8"M"s, u8"B"s, u8"T"s, u8"Q"s};
inline static constexpr NumberAbbreviationUnits abbrev_k = {1000, array_as_span(abbrev_k_strings)};

inline static constexpr Span<utf8> abbrev_kb_strings[] = {u8"B"s, u8"KB"s, u8"MB"s, u8"GB"s, u8"TB"s, u8"PB"s, u8"EB"s};
inline static constexpr NumberAbbreviationUnits abbrev_kb = {1000, array_as_span(abbrev_kb_strings)};

inline static constexpr Span<utf8> abbrev_kib_strings[] = {u8"B"s, u8"KiB"s, u8"MiB"s, u8"GiB"s, u8"TiB"s, u8"PiB"s, u8"EiB"s};
inline static constexpr NumberAbbreviationUnits abbrev_kib = {1024, array_as_span(abbrev_kib_strings)};

inline NumberAbbreviation abbreviate(f64 value, NumberAbbreviationUnits const &units, NumberAbbreviationOptions options = {}) {
	bool ok = !isinf(value) && !isnan(value);

	u8 unit = 0;
	if (ok) {
		while (value > units.divisor) {
			value /= units.divisor;
			unit += 1;
		}
	}

	return {
		.value = value,
		.between_value_and_unit = options.between_value_and_unit,
		.unit = unit,
		.unit_table = &units.unit_table,
	};
}
inline void append(StringBuilder &builder, FormatFloat<NumberAbbreviation> abbrev) {
	append_format(builder, "{}{}{}", abbrev.with_value(abbrev.value.value), abbrev.value.between_value_and_unit, (*abbrev.value.unit_table)[abbrev.value.unit]);
}

inline void append(StringBuilder &builder, NumberAbbreviation abbrev) {
	append(builder, FormatFloat{.value = abbrev, .precision = 3, .trailing_zeros = false});
}


struct FormatBytesParams {
	bool kilo_is_1024 = true;
};

inline NumberAbbreviation format_bytes(auto byte_count, FormatBytesParams params = {}) {
	return abbreviate(byte_count, params.kilo_is_1024 ? abbrev_kib : abbrev_kb, {.between_value_and_unit = u8" "s});
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
umm _to_utf8(Span<utf16> src, Span<utf8> dst) {
	return WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)src.data, (int)src.count, (char *)dst.data, (int)dst.count, 0, 0);
}

umm _utf16_size(Span<utf8> src) {
	return (umm)MultiByteToWideChar(CP_UTF8, 0, (char *)src.data, (int)src.count, 0, 0);
}
umm _to_utf16(Span<utf8> src, Span<utf16> dst) {
	return MultiByteToWideChar(CP_UTF8, 0, (char *)src.data, (int)src.count, (wchar_t *)dst.data, (int)dst.count);
}

#elif OS_LINUX
umm _utf8_size(Span<utf16> src) {
	not_implemented();
}
umm _to_utf8(Span<utf16> src, Span<utf8> dst) {
	not_implemented();
}

umm _utf16_size(Span<utf8> src) {
	not_implemented();
}
umm _to_utf16(Span<utf8> src, Span<utf16> dst) {
	not_implemented();
}
#endif

#endif

}

#pragma warning(pop)

#endif // _TL_STRING_H
