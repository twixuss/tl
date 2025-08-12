#pragma once
#include "c_tokenizer.h"
#include "file.h"

//
// To use this library you need to define a struct with the api matching that of PreprocessorAPI.
//

namespace tl::c_preprocessor {

using namespace tl::c;

/*
#define x(name)
TL_C_PREPROCESSOR_ENUMERATE_ERROR(x)
#undef x
*/
#define TL_C_PREPROCESSOR_ENUMERATE_ERROR(x) \
	x(ok) \
	x(tokenizer_error) \
	x(parsing_error) \
	x(unexpected_token) \
	x(file_not_found) \
	x(file_not_readable) \
	x(no_matching_directive) \


enum class Error {
	#define x(name) name,
	TL_C_PREPROCESSOR_ENUMERATE_ERROR(x)
	#undef x
};

inline void append(StringBuilder &builder, Error error) {
	switch (error) {
		#define x(name) case Error::name: return append(builder, #name##s);
		TL_C_PREPROCESSOR_ENUMERATE_ERROR(x)
		#undef x
	}
	return append_format(builder, "Error({})", (u32)error);
}

struct ErrorInfo {
	Span<char> path = {};
	Tokenizer *tokenizer = 0;
};

// To use this library you need to define a struct with the following api.
// Then pass it to preprocess_c_source/file.
// You can change the types as long as they are compatible. e.g. you could make include_directories a list.
// You don't need to inherit from this.
struct PreprocessorAPI {
	Span<Span<char>> include_directories;
	
	void error(Error error, Span<char> message, ErrorInfo info = {}) {}

	// Called after reading an included file.
	// Ownership is transferred. You will have to free the buffer.
	void add_source_buffer(Span<char> path, Buffer buffer) {}

	void before_include(Span<char> path) {}
	void after_include(Span<char> path) {}
	
	// Ownership of value_tokens is transferred
	void parsed_macro(Span<char> name, List<FatToken> value_tokens) {}
	Optional<Span<FatToken>> get_macro(Span<char> name) {}
};

template <class Preprocessor>
Error preprocess_c_source(auto path, Span<char> source, Preprocessor &&p) {
	auto t = Tokenizer::create(source);
	t.next();

	ErrorInfo ei = {.tokenizer = &t};
	u32 if_depth = 0;

	auto error = Combine{
		[&](Error error, Span<char> message, ErrorInfo info) {
			p.error(error, message, info);
			return error;
		},
		[&](Error error, Span<char> message) {
			p.error(error, message, ei);
			return error;
		},
	};

	struct If {
		bool enabled = false;
		bool cant_be_enabled_anymore = false;
	};

	List<If> if_stack;
	defer { free(if_stack); };

	auto is_enabled = [&] {
		return all(if_stack, [](If i) {return i.enabled;});
	};

	auto parse_directive_expression = [&]() -> Optional<List<FatToken>> {
		List<FatToken> expr_tokens;

		bool prev_is_backslash = false;

		while (1) {
			if (t.had_newline_before) {
				if (prev_is_backslash) {
					expr_tokens.pop();
				} else {
					break;
				}
			} else {
				if (prev_is_backslash) {
					return {};
				}
			}
			expr_tokens.add(t.fat_token());
			prev_is_backslash = t.kind == '\\';
			t.next();
		}

		return expr_tokens;
	};

	auto eval = [&](this auto &&self, Span<FatToken> tokens) -> bool {
		if (tokens.count == 0) {
			// TODO: return an error
			return false;
		}

		// So, the steps for evaluating function-macro are:
		//	1. Expand all args
		//  2. Expand the body

		while (1) {
			if (tokens.count < 3)
				break;

			if (tokens.front().kind == '(' && tokens.back().kind == ')') {
				tokens = tokens.skip(1).skip(-1);
			} else {
				break;
			}
		}

		if (tokens.count == 1) {
			switch (tokens[0].kind) {
				case tk_int: return !!tokens[0].int_value;
			}
		}

		println("TODO: not implemented: eval: {}", Span(tokens[0].location.begin(), tokens.back().location.end()));
		return true;
	};

	while (t.kind != tk_end_of_file) {
		if (t.kind == tk_error)
			return error(Error::tokenizer_error, "Error while tokenizing source"s);
		switch (t.kind) {
			case tk_directive: {
				if (t.location == "#include"s) {
					t.next();
					if (t.kind != tk_string) {
						return error(Error::unexpected_token, "Expected a string after #include"s);
					}

					if (is_enabled()) {
						List<char> include_path = {};
						defer { free(include_path); };

						bool found = false;
						if (t.location[0] == '"') {
							include_path = format("{}/{}"s, parse_path(as_utf8(path)).directory, t.string_value);
							found = file_exists(include_path);
						}
						if (!found) {
							for (auto include_directory : p.include_directories) {
								free(include_path);
								include_path = format("{}/{}", include_directory, t.string_value);
								if (file_exists(include_path)) {
									found = true;
									break;
								}
							}
						}
						if (!found) {
							return error(Error::file_not_found, tformat("Could not find {}"s, t.string_value), ei withx { it.path = include_path; });
						}

						p.before_include(include_path);

						auto [include_source_buffer, ok] = read_entire_file(include_path);
						// NOTE: don't free include_source_buffer. ownership is transferred to p.add_source_buffer
						if (!ok)
							return error(Error::file_not_readable, "Could not read {}"s, ei withx { it.path = include_path; });

						p.add_source_buffer(include_path, include_source_buffer);

						auto include_source = (Span<char>)include_source_buffer;

						auto e = preprocess_c_source(include_path, include_source, p);
						if (e != Error::ok)
							return e;
					
						p.after_include(include_path);
					}
				} else if (t.location == "#define"s) {
					t.next();
					if (t.kind != tk_identifier) {
						return error(Error::unexpected_token, "Expected an identifier after #define"s);
					}
					auto name = t.location;
					t.next();
					
					auto maybe_expr_tokens = parse_directive_expression();
					if (!maybe_expr_tokens) {
						return error(Error::parsing_error, "Error while parsing expression after directive"s);
					}
					auto expr_tokens = maybe_expr_tokens.value();
					// NOTE: don't free expr_tokens. ownership is transferred to p.parsed_macro

					if (is_enabled()) {
						p.parsed_macro(name, expr_tokens);
					}
				} else if (t.location == "#ifdef"s || t.location == "#ifndef"s) {
					auto if_location = t.location;

					t.next();
					if (t.kind != tk_identifier) {
						return error(Error::unexpected_token, "Expected an identifier after #ifdef"s);
					}
					auto name = t.location;
					t.next();
					if (!t.had_newline_before) {
						return error(Error::unexpected_token, "Expected an newline after name of macro in #ifdef"s);
					}

					bool defined = (bool)p.get_macro(name);
					
					if (if_location == "#ifdef"s) {
						if_stack.add({defined});
					} else if (if_location == "#ifndef"s) {
						if_stack.add({!defined});
					} else {
						invalid_code_path();
					}
				} else if (t.location == "#if"s) {
					t.next();
					
					auto maybe_expr_tokens = parse_directive_expression();
					if (!maybe_expr_tokens) {
						return error(Error::parsing_error, "Error while parsing expression after directive"s);
					}
					auto expr_tokens = maybe_expr_tokens.value();
					defer { free(expr_tokens); };

					bool eval_result = eval(expr_tokens);

					if_stack.add({eval_result});
					if_stack.back().cant_be_enabled_anymore |= if_stack.back().enabled;
				} else if (t.location == "#elif"s) {
					t.next();
					
					auto maybe_expr_tokens = parse_directive_expression();
					if (!maybe_expr_tokens) {
						return error(Error::parsing_error, "Error while parsing expression after directive"s);
					}
					auto expr_tokens = maybe_expr_tokens.value();
					defer { free(expr_tokens); };

					if (!if_stack.count) {
						return error(Error::no_matching_directive, "No matching directive for #elif"s);
					}

					if (if_stack.back().enabled) {
						// prev branch is true, this can't be
						if_stack.back().enabled = false;
					} else if (!if_stack.back().cant_be_enabled_anymore) {
						bool eval_result = eval(expr_tokens);

						if_stack.back().enabled = eval_result;
					}
					if_stack.back().cant_be_enabled_anymore |= if_stack.back().enabled;
				} else if (t.location == "#elifdef"s || t.location == "#elifndef"s) {
					auto if_location = t.location;

					t.next();
					if (t.kind != tk_identifier) {
						return error(Error::unexpected_token, "Expected an identifier after #ifdef"s);
					}
					auto name = t.location;
					t.next();
					if (!t.had_newline_before) {
						return error(Error::unexpected_token, "Expected an newline after name of macro in #ifdef"s);
					}

					
					if (if_stack.back().enabled) {
						// prev branch is true, this can't be
						if_stack.back().enabled = false;
					} else if (!if_stack.back().cant_be_enabled_anymore) {
						bool defined = (bool)p.get_macro(name);
						if (if_location == "#elifdef"s) {
							if_stack.back().enabled = defined;
						} else if (if_location == "#elifndef"s) {
							if_stack.back().enabled = !defined;
						} else {
							invalid_code_path();
						}
					}
					if_stack.back().cant_be_enabled_anymore |= if_stack.back().enabled;
				} else if (t.location == "#else"s) {
					if (!if_stack.count) {
						return error(Error::no_matching_directive, "No matching directive for #else"s);
					}

					if (if_stack.back().enabled) {
						if_stack.back().enabled = false;
					} else if (!if_stack.back().cant_be_enabled_anymore) {
						if_stack.back().enabled = true;
					}

					t.next();
				} else if (t.location == "#endif"s) {
					if (!if_stack.count) {
						return error(Error::no_matching_directive, "No matching #if directive for #endif"s);
					}

					if_stack.pop();

					t.next();
				} else {
					t.next();
				}
				break;
			}
			default:
				t.next();
				break;
		}
	}

	if (if_stack.count) {
		return error(Error::no_matching_directive, "Some #if directives are not closed"s);
	}
	return Error::ok;
}

template <class Preprocessor>
Error preprocess_c_file(auto path, Preprocessor &&p) {
	auto [source_buffer, ok] = read_entire_file(path);
	if (!ok)
		return Error::file_not_readable;
	defer { free(source_buffer); };

	auto source = (Span<char>)source_buffer;

	return preprocess_c_source(path, source, p);
}

}
