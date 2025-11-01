#pragma once

/*

Incomplete:
	integer/float literal edge cases (too big, rounding)

*/

/*

Example Usage:

Tokenizer t = Tokenizer::create(source_code);
while (t.next()) {
	println(t.location);
}

*/

#include "common.h"
#include "list.h"
#include "string.h"

namespace tl::c {

/*
MSVC, GCC, and Clang support multi-character literals. See :multi_char:
If you want a string representation of TokenKind, see TokenKindDesc
*/
using TokenKind = u32;
enum : TokenKind {
	tk_end_of_file,
	tk_identifier = 0x40000000, // foo7
	tk_directive,               // #include  #define  ...
	tk_string,                  // "hello world"
	tk_char,                    // 'c'
	tk_int,                     // 12345  678ull
	tk_float,                   // 12.34f

	// For error details see Tokenizer::error and Tokenizer::cursor
	tk_error,

	// unary operators
	tk_plus        = '+',
	tk_negate      = '-',
	tk_bitwise_not = '~',
	tk_logical_not = '!',
	tk_address     = '&',
	tk_dereference = '*',
	tk_increment   = '++',
	tk_decrement   = '--',
	tk_stringize   = '#',

	// binary operators
	tk_add            = '+',
	tk_subtract       = '-',
	tk_multiply       = '*',
	tk_divide         = '/',
	tk_modulo         = '%',
	tk_bitwise_xor    = '^',
	tk_bitwise_and    = '&',
	tk_bitwise_or     = '|',
	tk_shift_right    = '>>',
	tk_shift_left     = '<<',
	tk_logical_xor    = '!=',
	tk_logical_and    = '&&',
	tk_logical_or     = '||',
	tk_equals         = '==',
	tk_not_equals     = '!=',
	tk_less           = '<',
	tk_less_equals    = '<=',
	tk_greater        = '>',
	tk_greater_equals = '>=',
	tk_concatenate    = '##',

	// assignments
	tk_assign             = '=',
	tk_assign_add         = '+=',
	tk_assign_subtract    = '-=',
	tk_assign_multiply    = '*=',
	tk_assign_divide      = '/=',
	tk_assign_modulo      = '%=',
	tk_assign_bitwise_xor = '^=',
	tk_assign_bitwise_and = '&=',
	tk_assign_bitwise_or  = '|=',
	tk_assign_shift_right = '>>=',
	tk_assign_shift_left  = '<<=',

	// other
	tk_paren_open    = '(',
	tk_paren_close   = ')',
	tk_bracket_open  = '[',
	tk_bracket_close = ']',
	tk_curly_open    = '{',
	tk_curly_close   = '}',
	tk_semicolon     = ';',
	tk_colon         = ':',
	tk_comma         = ',',
	tk_dot           = '.',
	tk_arrow         = '->',
	tk_question      = '?',
	tk_backslash     = '\\',
};

// All information about a token. Don't store it in big lists, wasteful!
struct FatToken {
	TokenKind kind;
	Span<char> location;
	union {
		u64 int_value;
		f64 float_value;
		List<char> string_value;
	};
	Span<char> suffix;
};

/*
#define x(ch)
TL_ENUMERATE_LATIN_CHARS(x)
#undef x
*/
#define TL_ENUMERATE_LATIN_CHARS(x) \
	x('a') x('A') x('b') x('B') \
	x('c') x('C') x('d') x('D') \
	x('e') x('E') x('f') x('F') \
	x('g') x('G') x('h') x('H') \
	x('i') x('I') x('j') x('J') \
	x('k') x('K') x('l') x('L') \
	x('m') x('M') x('n') x('N') \
	x('o') x('O') x('p') x('P') \
	x('q') x('Q') x('r') x('R') \
	x('s') x('S') x('t') x('T') \
	x('u') x('U') x('v') x('V') \
	x('w') x('W') x('x') x('X') \
	x('y') x('Y') x('z') x('Z') \

/*
#define x(ch)
TL_ENUMERATE_DECIMAL_CHARS(x)
#undef x
*/
#define TL_ENUMERATE_DECIMAL_CHARS(x) \
	x('0') x('1') x('2') x('3') x('4') \
	x('5') x('6') x('7') x('8') x('9') \

#define TL_INSERT_CASE(value) case value:

#ifndef TL_C_TOKENIZER_IDENT_AT
#define TL_C_TOKENIZER_IDENT_AT 1
#endif

#ifndef TL_C_TOKENIZER_IDENT_DOLLAR
#define TL_C_TOKENIZER_IDENT_DOLLAR 1
#endif

/*
#define x(name)
TL_C_ENUMERATE_TOKENIZER_ERRORS(x)
#undef X
*/
#define TL_C_ENUMERATE_TOKENIZER_ERRORS(x)                              \
	x(no_error)                  /* default for less confusion while debugging */ \
	x(unclosed_string)           /* "hello w                         */ \
	x(unclosed_character)        /* 'c                               */ \
	x(unclosed_comment)          /* /* hello w                       */ \
	x(unfinished_binary_integer) /* 0b                               */ \
	x(invalid_octal_integer)     /* 09                               */ \
	x(unfinished_float)          /* 1.4e  5.6e+                      */ \
	x(invalid_string)            /* contains invalid escape sequence */ \
	x(invalid_character)         /* ^^^                              */ \

struct Tokenizer {
	// Kind of previously parsed token.
	TokenKind kind = {};

	// Location of previously parsed token.
	Span<char> location;

	// Parsed/unescaped values.
	// Note that they are not reset after parsing a different token!
	// Refer to them only if kind matches!
	
	// Unescaped string from tk_string, tk_char.
	// Angle bracket path after #include is tk_string and is here as well.
	// You can check if it's external by comparing location.data[0] with '"' or '<'
	// Note that memory is reused, so don't forget to copy the list if you're storing it for later.
	List<char> string_value;
	
	// Parsed integer
	u64 int_value = 0;

	// Parsed float
	f64 float_value = 0;
	
	// Suffix for integer and float literals, e.g. "ull", "f", etc.
	// Unknown suffixes are parsed and ignored.
	Span<char> suffix;

	char *source_start = 0;
	char *source_end = 0;

	// If an error occurs, cursor will be pointing at it.
	char *cursor = 0;

	enum class Error {
		#define x(name) name,
		TL_C_ENUMERATE_TOKENIZER_ERRORS(x) // See definition of this macro for list of errors
		#undef X
	};

	// Describes the error if kind == tk_error
	Error error;

	// Was the previous token "#include"?
	bool previous_is_include : 1 = false;

	// Was there a newline between end of previous token and start of current?
	bool had_newline_before : 1 = false;

	static Tokenizer create(Span<char> source);

	// Reads further, updates current token.
	// Returns false if reached end of file;
	bool next();

	// Returns all available info about the current token organized in a structure.
	FatToken fat_token();
};

struct TokenKindDesc { TokenKind kind; };

inline void append(StringBuilder &builder, TokenKindDesc desc) {
	switch (desc.kind) {
		case tk_end_of_file: return append(builder, "end_of_file"s);
		case tk_identifier:  return append(builder, "identifier"s);
		case tk_directive:   return append(builder, "directive"s);
		case tk_string:      return append(builder, "string"s);
		case tk_char:        return append(builder, "char"s);
		case tk_int:         return append(builder, "int"s);
		case tk_float:       return append(builder, "float"s);
		case tk_error:       return append(builder, "error"s);
	}

	char buf[5];
	buf[4] = 0;
	char *c = buf + 4;
	while (desc.kind) {
		*--c = desc.kind;
		desc.kind >>= 8;
	}

	return append(builder, as_span(c));
}


inline void append(StringBuilder &builder, Tokenizer::Error error) {
	switch (error) {
		#define x(name) case Tokenizer::Error::name: return append(builder, #name##s);
		TL_C_ENUMERATE_TOKENIZER_ERRORS(x) // See definition of this macro for list of errors
		#undef X
	}
	return append_format(builder, "Tokenizer::Error({})", (u32)error);
}

#ifdef TL_IMPL

Tokenizer Tokenizer::create(Span<char> source) {
	return {
		.source_start = source.begin(), 
		.source_end = source.end(),
		.cursor = source.begin(),
	};
}

#pragma warning(push, 4)
#pragma warning(disable: 4309) // constant truncation
#pragma warning(disable: 4244) // lossy conversion
#pragma warning(error: 5262) // case fallthrough
	
bool Tokenizer::next() {
	auto parse_string = [&](Error unclosed_error, Error invalid_error, char closing_char) {
		++cursor;
		
		string_value.clear();

		while (1) {
			if (cursor >= source_end) {
				kind = tk_error;
				error = unclosed_error;
				break;
			}
			if (*cursor == '\\') {
				++cursor;
				if (cursor >= source_end) {
					kind = tk_error;
					error = unclosed_error;
					break;
				}

				switch (*cursor) {
					case 'a': string_value.add('\a'); ++cursor; break;
					case 'b': string_value.add('\b'); ++cursor; break;
					case 'f': string_value.add('\f'); ++cursor; break;
					case 'n': string_value.add('\n'); ++cursor; break;
					case 'r': string_value.add('\r'); ++cursor; break;
					case 't': string_value.add('\t'); ++cursor; break;
					case 'v': string_value.add('\v'); ++cursor; break;
					case '\\': string_value.add('\\'); ++cursor; break;
					case 'x': case 'X': {
						++cursor;
						if (cursor == source_end) {
							kind = tk_error;
							error = unclosed_error;
							break;
						}
						if (!is_hex_digit(*cursor)) {
							kind = tk_error;
							error = invalid_error;
							break;
						}
						char x = 0;
						while (cursor < source_end && is_hex_digit(*cursor)) {
							x = (x << 4) | hex_digit_to_int_unchecked(*cursor++);
						}
						string_value.add(x);
						break;
					}
					default: 
						string_value.add(*cursor);
						++cursor;
						break;
				}
			} else if (*cursor == closing_char) {
				++cursor;
				break;
			} else {
				string_value.add(*cursor);
				++cursor;
			}
		}
	};
	auto parse_fractional = [&] {
		f64 multiplier = 1;
		while (cursor < source_end) {
			switch (*cursor) {
				TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
					float_value += (*cursor++ - '0') * (multiplier *= 0.1); // TODO: this is definitely wrong, rounding errors will accumulate.
					continue;
				}
			}
			break;
		}
	};
	auto parse_exponent = [&] {
		if (cursor < source_end && to_lower(*cursor) == 'e') {
			++cursor;
			if (cursor >= source_end) {
				// TODO: return an error.
				// kind = tk_unfinished_float;
				// return token;
				return true;
			}
			bool positive = true;
			switch (*cursor) {
				case '+': ++cursor; break;
				case '-': ++cursor; positive = false; break;
			}
							
			u64 exponent = 0;
			while (cursor < source_end) {
				switch (*cursor) {
					TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
						exponent = exponent * 10 + (*cursor++ - '0');
						continue;
					}
				}
				break;
			}

			float_value *= ::pow(10.0, (f64)exponent * ((positive<<1)-1));
			return true;
		}
		return false;
	};
	auto parse_suffix = [&] {
		suffix.data = cursor;
		switch (*cursor) {
			TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE)
			case '_': {
				while (cursor < source_end) {
					switch (*cursor) {
						TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE)
						TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE)
						case'_': {
							++cursor;
							continue;
						}
					}
					break;
				}
				break;
			}
		}
		suffix.set_end(cursor);
	};
	


	bool is_include = false;
	had_newline_before = false;

restart:

	#if 0 // TL_USE_SIMD TODO: had_newline_before
	// Enabling this makes whole thing slower, however this part specifically becomes faster? idk.
	// '\t' =  9
	// '\n' = 10
	// '\v' = 11
	// '\f' = 12
	// '\r' = 13
	// ' '  = 32
	while (cursor <= source_end - 16) {
		__m128i c = _mm_loadu_epi8(cursor);
		__m128i ok =          _mm_cmpgt_epi8(_mm_set1_epi8(5^0x80), _mm_sub_epi8(c, _mm_set1_epi8(9^0x80))); // ^0x80 cuz we want unsigned comparison
		ok = _mm_or_si128(ok, _mm_cmpeq_epi8(_mm_set1_epi8(32), c));
		u16 mask = _mm_movemask_epi8(ok);
		if (mask == (u16)-1) {
			cursor += 16;
			continue;
		}
		cursor += find_lowest_zero_bit((u16)mask);
		goto whitespace_skipped;
	}
	#endif

	while (cursor < source_end) {
		switch (*cursor) {
			case '\n':
				had_newline_before = true;
				[[fallthrough]];
			case ' ': case '\v': case '\r': case '\t': case '\f':
				++cursor;
				continue;
		}
		break;
	}
whitespace_skipped:

	kind = tk_end_of_file;
	location = {cursor, cursor};

	if (cursor == source_end) {
		goto end;
	}

	switch (*cursor) {
		// Single char
		case '(':case ')':
		case '[':case ']':
		case '{':case '}':
		case ',':case '~':
		case '?':case '\\':
		case ';':case ':': {
			kind = *cursor++;
			goto end;
		}

		// Single char or same double char
		case '=': {
			char first = *cursor;
			kind = *cursor++;
			if (cursor < source_end && *cursor == first) {
				kind = kind << 8 | *cursor++;
			}
			goto end;
		}

		// Single char or with equal
		case '!':
		case '*':
		case '%':
		case '^': {
			kind = *cursor++;
			if (cursor < source_end && *cursor == '=') {
				kind = kind << 8 | *cursor++;
			}
			goto end;
		}

		// Single char or same double char or with equal
		case '&':
		case '|':
		case '+': {
			char first = *cursor;
			kind = *cursor++;
			if (cursor < source_end && ((*cursor == first) | (*cursor == '='))) {
				kind = kind << 8 | *cursor++;
			}
			goto end;
		}

		//  -  --  -=  ->
		case '-': {
			kind = *cursor++;
			if (cursor < source_end && ((*cursor == '-') | (*cursor == '=') | (*cursor == '>'))) {
				kind = kind << 8 | *cursor++;
			}
			goto end;
		}

		// #  ##  #directive
		case '#': {
			kind = *cursor++;
			if (cursor < source_end) {
				switch (*cursor) {
					case '#': {
						kind = kind << 8 | *cursor++;
						break;
					}
					TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE) {
						while (cursor < source_end) {
							switch (*cursor) {
								TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE)
								case'0':case'1':case'2':case'3':case'4':
								case'5':case'6':case'7':case'8':case'9':
								case'_': {
									++cursor;
									continue;
								}
							}
							break;
						}
						kind = tk_directive;
						is_include = Span(location.data, cursor) == "#include"s;
						break;
					}
				}
			}
			goto end;
		}

		// <  <=  <<  <<=  <include.h>
		case '<': {
			char first = *cursor;
			kind = *cursor++;
			if (cursor < source_end) {
				if (previous_is_include) {
					while (cursor < source_end && *cursor != '>') { ++cursor; }

					++cursor;

					kind = tk_string;
					location.set_end(cursor);

					string_value.clear();
					unescape_c_string(location.subspan(1, location.count - 2), [&](Span<char> s) {
						string_value.add(s);
					});
				} else {
					if (*cursor == '=') {
						kind = kind << 8 | *cursor++;
					} else if (*cursor == first) {
						kind = kind << 8 | *cursor++;
						if (cursor < source_end && *cursor == '=') {
							kind = kind << 8 | *cursor++;
						}
					}
				}
			}
			goto end;
		}

		// >  >=  >>  >>=
		case '>': {
			char first = *cursor;
			kind = *cursor++;
			if (cursor < source_end) {
				if (*cursor == '=') {
					kind = kind << 8 | *cursor++;
				} else if (*cursor == first) {
					kind = kind << 8 | *cursor++;
					if (cursor < source_end && *cursor == '=') {
						kind = kind << 8 | *cursor++;
					}
				}
			}
			goto end;
		}

		// Strings
		case '"': {
			kind = tk_string;
			parse_string(Error::unclosed_string, Error::invalid_string, '"');
			goto end;
		}

		// Characters
		case '\'': {
			kind = tk_char;
			parse_string(Error::unclosed_character, Error::invalid_character, '\'');
			goto end;
		}

		// Identifiers
		#if TL_C_TOKENIZER_IDENT_AT
		case '@':
		#endif
		#if TL_C_TOKENIZER_IDENT_DOLLAR
		case '$':
		#endif // Some identifiers in windows headers contain those
		TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE)
		case '_': {
			++cursor;
			#if TL_USE_SIMD
			while (1) {
				#if ARCH_AVX512
				if (cursor <= source_end - 64) {
					__m512i c = _mm512_loadu_epi8(cursor);
					__mmask64 ok =       _mm512_cmpgt_epi8_mask(_mm512_set1_epi8(10 ^ 0x80), _mm512_sub_epi8(c, _mm512_set1_epi8('0' ^ 0x80))); // ^0x80 cuz we want unsigned comparison
					ok = _kor_mask64(ok, _mm512_cmpgt_epi8_mask(_mm512_set1_epi8(26 ^ 0x80), _mm512_sub_epi8(c, _mm512_set1_epi8('a' ^ 0x80))));
					ok = _kor_mask64(ok, _mm512_cmpgt_epi8_mask(_mm512_set1_epi8(26 ^ 0x80), _mm512_sub_epi8(c, _mm512_set1_epi8('A' ^ 0x80))));
					ok = _kor_mask64(ok, _mm512_cmpeq_epi8(c, _mm512_set1_epi8('_')));
					#if TL_C_TOKENIZER_IDENT_AT
					ok = _kor_mask64(ok, _mm512_cmpeq_epi8(c, _mm512_set1_epi8('@')));
					#endif
					#if TL_C_TOKENIZER_IDENT_DOLLAR
					ok = _kor_mask64(ok, _mm512_cmpeq_epi8(c, _mm512_set1_epi8('$')));
					#endif
					if (ok == (u64)-1) {
						cursor += 64;
						continue;
					}
					cursor += find_lowest_zero_bit((u64)ok);
					break;
				}
				#elif ARCH_AVX2
				if (cursor <= source_end - 32) {
					__m256i c = _mm256_loadu_epi8(cursor);
					__m256i ok =             _mm256_cmpgt_epi8(_mm256_set1_epi8(10 ^ 0x80), _mm256_sub_epi8(c, _mm256_set1_epi8('0' ^ 0x80))); // ^0x80 cuz we want unsigned comparison
					ok = _mm256_or_si256(ok, _mm256_cmpgt_epi8(_mm256_set1_epi8(26 ^ 0x80), _mm256_sub_epi8(c, _mm256_set1_epi8('a' ^ 0x80))));
					ok = _mm256_or_si256(ok, _mm256_cmpgt_epi8(_mm256_set1_epi8(26 ^ 0x80), _mm256_sub_epi8(c, _mm256_set1_epi8('A' ^ 0x80))));
					ok = _mm256_or_si256(ok, _mm256_cmpeq_epi8(c, _mm256_set1_epi8('_')));
					#if TL_C_TOKENIZER_IDENT_AT
					ok = _mm256_or_si256(ok, _mm256_cmpeq_epi8(c, _mm256_set1_epi8('@')));
					#endif
					#if TL_C_TOKENIZER_IDENT_DOLLAR
					ok = _mm256_or_si256(ok, _mm256_cmpeq_epi8(c, _mm256_set1_epi8('$')));
					#endif
					u32 mask = _mm256_movemask_epi8(ok);
					if (mask == (u32)-1) {
						cursor += 32;
						continue;
					}
					cursor += find_lowest_zero_bit((u32)mask);
					break;
				}
				#else
				if (cursor <= source_end - 16) {
					__m128i c = _mm_loadu_epi8(cursor);

					// ok = c - '0' < 10
					__m128i ok =          _mm_cmpgt_epi8(_mm_set1_epi8(10 ^ 0x80), _mm_sub_epi8(c, _mm_set1_epi8('0' ^ 0x80))); // ^0x80 cuz we want unsigned comparison
					ok = _mm_or_si128(ok, _mm_cmpgt_epi8(_mm_set1_epi8(26 ^ 0x80), _mm_sub_epi8(c, _mm_set1_epi8('a' ^ 0x80))));
					ok = _mm_or_si128(ok, _mm_cmpgt_epi8(_mm_set1_epi8(26 ^ 0x80), _mm_sub_epi8(c, _mm_set1_epi8('A' ^ 0x80))));
					ok = _mm_or_si128(ok, _mm_cmpeq_epi8(c, _mm_set1_epi8('_')));
					#if TL_C_TOKENIZER_IDENT_AT
					ok = _mm_or_si128(ok, _mm_cmpeq_epi8(c, _mm_set1_epi8('@')));
					#endif
					#if TL_C_TOKENIZER_IDENT_DOLLAR
					ok = _mm_or_si128(ok, _mm_cmpeq_epi8(c, _mm_set1_epi8('$')));
					#endif
					u16 mask = _mm_movemask_epi8(ok);
					if (mask == (u16)-1) {
						cursor += 16;
						continue;
					}
					cursor += find_lowest_zero_bit((u16)mask);
					break;
				}
				#endif
				while (cursor < source_end) {
					switch (*cursor) {
						//case '@': case '$': // Some identifiers in windows headers contain those
						TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE)
						TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE)
						case'_': {
							++cursor;
							continue;
						}
					}
					break;
				}
				break;
			}
			#else // ^^^ SIMD ^^^ --- vvv scalar vvv
			while (cursor < source_end) {
				switch (*cursor) {
					//case '@': case '$': // Some identifiers in windows headers contain those
					TL_ENUMERATE_LATIN_CHARS(TL_INSERT_CASE)
					TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE)
					case'_': {
						++cursor;
						continue;
					}
				}
				break;
			}
			#endif // ^^^ scalar ^^^

			kind = tk_identifier;
			goto end;
		}

		case '.': {
			kind = *cursor++;
			if (cursor < source_end) {
				switch (*cursor) {
					TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
						kind = tk_float;
						float_value = 0;
						parse_fractional();
						parse_exponent();
						parse_suffix();
						break;
					}
				}
			}
			goto end;
		}

		// Numbers
		TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
			kind = tk_int;
			int_value = 0;
			float_value = 0;

			if (*cursor == '0') {
				++cursor;
				if (cursor >= source_end) {
					goto end;
				}
				switch (*cursor) {
					// 
					// Binary
					//
					case 'b': case 'B': {
						++cursor;
						if (cursor >= source_end) {
							kind = tk_error;
							error = Error::unfinished_binary_integer;
							goto end;
						}
						while (1) {
							switch (*cursor) {
								case '0': case '1':
									int_value = (int_value << 1) | (*cursor - '0');
									++cursor;
									continue;
							}
							break;
						}
						break;
					}
								
					// 
					// Hex
					//
					case 'x': case 'X': {
						++cursor;
						if (cursor >= source_end) {
							kind = tk_error;
							error = Error::unfinished_binary_integer;
							goto end;
						}
						while (cursor < source_end) {
							switch (*cursor) {
								case '0':case '1':case '2':case '3':case '4':
								case '5':case '6':case '7':case '8':case '9':
								case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
								case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
									int_value = (int_value << 4) | hex_digit_to_int_unchecked(*cursor++);
									continue;
							}
							break;
						}
						break;
					}

					//
					// Octal or float
					//
					TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
						while (cursor < source_end) {
							switch (*cursor) {
								case '0':case '1':case '2':case '3':
								case '4':case '5':case '6':case '7':
									int_value = (int_value << 3) | (*cursor++ - '0');
									continue;
								//
								// Invalid digit for octal, must be float
								//
								case '8': case '9':
									kind = tk_float;
									for (char *c = location.data; c < cursor; ++c) {
										float_value = float_value * 10 + (*c - '0');
									}
									while (cursor < source_end) {
										switch (*cursor) {
											TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
												float_value = float_value * 10 + (*cursor++ - '0');
												continue;
											}
										}
										break;
									}

									if (cursor >= source_end) {
										kind = tk_error;
										error = Error::invalid_octal_integer;
										goto end;
									}

									if (*cursor != '.') {
										kind = tk_error;
										error = Error::invalid_octal_integer;
										goto end;
									}

									++cursor;
									parse_fractional();
									parse_exponent();
									break;
							}
							break;
						}
						break;
					}

					// Float;
					case '.': {
						kind = tk_float;
						++cursor;
						parse_fractional();
						parse_exponent();
						break;
					}
				}
			} else {
				while (cursor < source_end) {
					switch (*cursor) {
						TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
							int_value = int_value * 10 + (*cursor++ - '0');
							continue;
						}
					}
					break;
				}

				if (cursor < source_end) {
					switch (*cursor) {
						case '.': {
							kind = tk_float;
							float_value = (f64)int_value;
						
							++cursor;
							parse_fractional();
							parse_exponent();
							break;
						}
						case 'e':case 'E': {
							kind = tk_float;
							float_value = (f64)int_value;
							parse_exponent();
							break;
						}
					}
				}
			}

			parse_suffix();
			break;
		}

		//   /  //  /*
		case '/': {
			kind = (TokenKind)*cursor;
			++cursor;
			if (*cursor == '/') {
				while (cursor != source_end && *cursor != '\n') {
					++cursor;
				}
				goto restart;
			} else if (*cursor == '*') {
				++cursor;
				while (true) {
					if (*cursor == '*') {
						++cursor;
						if (cursor >= source_end) {
							kind = tk_error;
							error = Error::unclosed_comment;
							goto end;
						}
						if (*cursor == '/') {
							++cursor;
							goto restart;
						}
					} else {
						++cursor;
						if (cursor >= source_end) {
							kind = tk_error;
							error = Error::unclosed_comment;
							goto end;
						}
					}
				}
				goto restart;
			} else {
				kind = '/';
			}
			break;
		}
	}
end:
	location.set_end(cursor);
	previous_is_include = is_include;
	return kind != tk_end_of_file;
}

FatToken Tokenizer::fat_token() {
	FatToken token = {};
	token.kind = kind;
	token.location = location;
	switch (kind) {
		case tk_int: token.int_value = int_value; break;
		case tk_float: token.float_value = float_value; break;
		case tk_string: token.string_value = copy(string_value); break;
	}
	token.suffix = suffix;
	return token;
}

#pragma warning(pop)
#endif

}

/*

	:multi_char:
MSVC, GCC, and Clang support multi-character literals.
You can use them for comparisons insread of tk_* enum values:

if (t.kind == '->') {
	...
}

Note that these literals are big-endian, so '->' does not equal *(u32 *)"->" on little endian machines.
Also they are limited to 4 bytes.



*/