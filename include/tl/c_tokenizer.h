#pragma once
#include "common.h"
#include "list.h"
#include "string.h"

namespace tl::c {

/*
// MSVC, GCC, and Clang support multi-characted character literals.
// This makes it easy to refer to token kinds in your source code.
// For example you want to check if the token is "->":

if (token.kind == '->') {
	...
}

// Note that these literals are big-endian, so '->' does not equal *(u32 *)"->" on little endian machines.
// Also they are limited to 4 bytes.
*/
using TokenKind = u32;
enum : TokenKind {
	tk_end_of_file,
	tk_identifier = 0x8000, // foo7
	tk_directive,           // #include / #define / ...
	tk_string,              // "hello world"
	tk_character,           // 'c'
	tk_integer,             // 12345

	// errors
	tk_unclosed_string,     // "hello w
	tk_unclosed_character,  // 'c
	tk_unclosed_comment,    // /* hello w

	// unary operators
	tk_plus        = '+',
	tk_negate      = '-',
	tk_bitwise_not = '~',
	tk_logical_not = '!',
	tk_address     = '&',
	tk_dereference = '*',
	tk_increment = '++',
	tk_decrement = '--',
	tk_stringize = '#',

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

struct Token {
	TokenKind kind = {};
	Span<char> string;
};

struct Tokenizer {
	// Unescaped string from string literals, character literals and include paths.
	List<char> string;

	char *source_start = 0;
	char *source_end = 0;
	char *cursor = 0;
	bool previous_is_include = false;

	static Tokenizer create(Span<char> source) {
		return {
			.source_start = source.begin(), 
			.source_end = source.end(),
			.cursor = source.begin(),
		};
	}

	Token next() {
		string.clear();
		bool is_include = false;
		defer { previous_is_include = is_include; };

	restart:
		while ((cursor < source_end) && ((*cursor == ' ') | (*cursor == '\v') | (*cursor == '\r') | (*cursor == '\n') | (*cursor == '\t') | (*cursor == '\f'))) { ++cursor; }
		
		Token token = {
			.kind = tk_end_of_file,
			.string = {cursor, cursor},
		};

		defer {
			// At the end cursor should point one char after the token.
			token.string.set_end(cursor);
		};

		if (cursor == source_end) {
			return token;
		}

		switch (*cursor) {
			// Single char
			case '(':case ')':
			case '[':case ']':
			case '{':case '}':
			case ',':case '.':
			case '?':case '\\':
			case ';':case ':':
			case '~': {
				token.kind = *cursor++;
				return token;
			}

			// Single char or same double char
			case '=': {
				char first = *cursor;
				token.kind = *cursor++;
				if (cursor < source_end && *cursor == first) {
					token.kind = token.kind << 8 | *cursor++;
				}
				return token;
			}

			// Single char or with equal
			case '!':
			case '*':
			case '%':
			case '^': {
				token.kind = *cursor++;
				if (cursor < source_end && *cursor == '=') {
					token.kind = token.kind << 8 | *cursor++;
				}
				return token;
			}

			// Single char or same double char or with equal
			case '&':
			case '|':
			case '+': {
				char first = *cursor;
				token.kind = *cursor++;
				if (cursor < source_end && ((*cursor == first) | (*cursor == '='))) {
					token.kind = token.kind << 8 | *cursor++;
				}
				return token;
			}

			//  -  --  -=  ->
			case '-': {
				token.kind = *cursor++;
				if (cursor < source_end && ((*cursor == '-') | (*cursor == '=') | (*cursor == '>'))) {
					token.kind = token.kind << 8 | *cursor++;
				}
				return token;
			}

			// #  ##  #directive
			case '#': {
				token.kind = *cursor++;
				if (cursor < source_end) {
					switch (*cursor) {
						case '#': {
							token.kind = token.kind << 8 | *cursor++;
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
							token.kind = tk_directive;
							is_include = Span(token.string.data, cursor) == "#include"s;
							break;
						}
					}
				}
				return token;
			}

			// <  <=  <<  <<=  <include.h>
			case '<': {
				char first = *cursor;
				token.kind = *cursor++;
				if (cursor < source_end) {
					if (previous_is_include) {
						while (cursor < source_end && *cursor != '>') { ++cursor; }

						++cursor;

						token.kind = tk_string;
						token.string.set_end(cursor);

						unescape_c_string(token.string.subspan(1, token.string.count - 2), [&](Span<char> s) {
							string.add(s);
						});
					} else {
						if (*cursor == '=') {
							token.kind = token.kind << 8 | *cursor++;
						} else if (*cursor == first) {
							token.kind = token.kind << 8 | *cursor++;
							if (cursor < source_end && *cursor == '=') {
								token.kind = token.kind << 8 | *cursor++;
							}
						}
					}
				}
				return token;
			}

			// >  >=  >>  >>=
			case '>': {
				char first = *cursor;
				token.kind = *cursor++;
				if (cursor < source_end) {
					if (*cursor == '=') {
						token.kind = token.kind << 8 | *cursor++;
					} else if (*cursor == first) {
						token.kind = token.kind << 8 | *cursor++;
						if (cursor < source_end && *cursor == '=') {
							token.kind = token.kind << 8 | *cursor++;
						}
					}
				}
				return token;
			}

			// Strings
			case '"': {
				parse_string(token, tk_string, tk_unclosed_string, '"');
				return token;
			}

			// Characters
			case '\'': {
				parse_string(token, tk_character, tk_unclosed_character, '\'');
				return token;
			}

			// Identifiers
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

				token.kind = tk_identifier;
				return token;
			}

			// Numbers
			TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
				while (cursor < source_end) {
					switch (*cursor) {
						TL_ENUMERATE_DECIMAL_CHARS(TL_INSERT_CASE) {
							++cursor;
							continue;
						}
					}
					break;
				}

				token.kind = tk_integer;
				return token;
			}

			//   /  //  /*
			case '/': {
				token.kind = (TokenKind)*cursor;
				++cursor;
				if (*cursor == '/') {
					while (cursor != source_end && *cursor != '\n') {
						++cursor;
					}
					goto restart;
				} else if (*cursor == '*') {
					while (true) {
						if (*cursor == '*') {
							++cursor;
							if (cursor >= source_end) {
								token.kind = tk_unclosed_comment;
								return token;
							}
							if (*cursor == '/') {
								++cursor;
								goto restart;
							}
						}
						++cursor;
						if (cursor >= source_end) {
							token.kind = tk_unclosed_comment;
							return token;
						}
					}
					goto restart;
				} else {
					token.kind = '/';
					return token;
				}
				break;
			}
		}
		return token;
	}

	void parse_string(Token &token, TokenKind valid_kind, TokenKind unclosed_kind, char closing_char) {
		++cursor;

		token.kind = valid_kind;
		while (1) {
			if (cursor >= source_end) {
				token.kind = unclosed_kind;
				break;
			}
			if (*cursor == '\\') {
				++cursor;
				if (cursor >= source_end) {
					token.kind = unclosed_kind;
					break;
				}

				switch (*cursor) {
					case 'a': string.add('\a'); ++cursor; break;
					case 'b': string.add('\b'); ++cursor; break;
					case 'f': string.add('\f'); ++cursor; break;
					case 'n': string.add('\n'); ++cursor; break;
					case 'r': string.add('\r'); ++cursor; break;
					case 't': string.add('\t'); ++cursor; break;
					case 'v': string.add('\v'); ++cursor; break;
					case '\\': string.add('\\'); ++cursor; break;
					case 'x': case 'X': {
						char x = 0;
						while (cursor < source_end && is_hex_digit(*cursor)) {
							x = (x << 4) | hex_digit_to_int(*cursor);
							++cursor;
						}
						string.add(x);
						break;
					}
					default: 
						string.add(*cursor);
						++cursor;
						break;
				}
			} else if (*cursor == closing_char) {
				++cursor;
				break;
			} else {
				string.add(*cursor);
				++cursor;
			}
		}
	}
};

}
