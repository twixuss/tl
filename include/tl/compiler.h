#pragma once
#include "list.h"

namespace TL {

TL_API List<ascii> demangle(ascii const *function_name);

}

#ifdef TL_IMPL
#if COMPILER_MSVC

#include "string.h"

namespace TL {

struct DemangleState {
	List<char> last_argument;
};

static void parse_arguments(DemangleState &state, StringBuilder &builder, char const *&c, bool add_comma = false) {
	if (add_comma) {
		append(builder, ", "s);
	}
	switch (*c++) {
		case 'C': append(builder, "s8"s);    break;
		case 'D': append(builder, "ascii"s); break;
		case 'E': append(builder, "u8"s);    break;
		case 'F': append(builder, "s16"s);   break;
		case 'G': append(builder, "u16"s);   break;
		case 'H': append(builder, "s32"s);   break;
		case 'I': append(builder, "u32"s);   break;
		case 'J': append(builder, "slong"s); break;
		case 'K': append(builder, "ulong"s); break;
		case 'M': append(builder, "f32"s);   break;
		case 'N': append(builder, "f64"s);   break;
		case 'O': append(builder, "f80"s);   break;
		case 'U': {
			StringBuilder identifier_builder;

			bool templated = false;
			if (*c == '?') {
				templated = true;
				++c;
				assert(*c == '$');
				++c;
			}
			List<char> template_parameter;
			//
			//if (templated) {
			//	char const *name_start = c;
			//	while (*c != '@') {
			//		++c;
			//	}
			//	append(builder, Span(name_start, c));
			//	++c;
			//	append(builder, '<');
			//	parse_arguments(state, builder, c, false);
			//	append(builder, '>');
			//
			//} else {
				char const *name_start = c;
				while (*c != '@') {
					++c;
				}
				append(identifier_builder, Span(name_start, c));
				++c;
			check_next:
				switch (*c) {
					case '@': ++c; break;
					default: {
						if (templated) {
							StringBuilder template_builder;
							parse_arguments(state, template_builder, c, false);
							template_parameter = to_string(template_builder);
							goto check_next;
						} else {
							char const *namespace_start = c;
							while (*c != '@') {
								++c;
							}
							append(builder, Span(namespace_start, c));
							++c;
							append(builder, "::"s);
							goto check_next;
						}

						//bool templated = false;
						//if (*c == '?') {
						//	templated = true;
						//	++c;
						//	assert(*c == '$');
						//	++c;
						//}
						//if (templated) {
						//	StringBuilder template_builder;
						//	char const *name_start = c;
						//	while (*c != '@') {
						//		++c;
						//	}
						//	append(template_builder, Span(name_start, c));
						//	++c;
						//	append(template_builder, '<');
						//	parse_arguments(state, template_builder, c, false);
						//	append(template_builder, '>');
						//	identifiers.add(to_string(template_builder));
						//	goto check_for_another_identifier;
						//} else {
						//	char const *name_space_start = c;
						//	while (*c != '@') {
						//		++c;
						//	}
						//	identifiers.add(as_list(Span(name_space_start, c)));
						//	++c;
						//	goto check_for_another_identifier;
						//}
					}
				}

				if (templated) {
					append(identifier_builder, '<');
					append(identifier_builder, template_parameter);
					append(identifier_builder, '>');
				}
				auto identifier = to_string(identifier_builder);
				append(builder, identifier);
				state.last_argument = identifier;

			//	StringBuilder argument_builder;
			//
			//	bool append_4dots = false;
			//	for (auto &identifier : reverse(identifiers)) {
			//		if (append_4dots) {
			//			append(argument_builder, "::");
			//		}
			//		append(argument_builder, identifier);
			//		append_4dots = true;
			//	}
			//
			//	auto argument = to_string(argument_builder);
			//	state.last_argument = argument;
			//	append(builder, argument);
			//}
			break;
		}
		case 'X': ++c; break;
		case '_':
			switch (*c++) {
				case 'J': append(builder, "s64"s);   assert(*c++ == '@'); break;
				case 'K': append(builder, "u64"s);   assert(*c++ == '@'); break;
				case 'N': append(builder, "bool"s);  assert(*c++ == '@'); break;
				case 'Q': append(builder, "utf8"s);  assert(*c++ == '@'); break;
				case 'S': append(builder, "utf16"s); assert(*c++ == '@'); break;
				case 'U': append(builder, "utf32"s); assert(*c++ == '@'); break;
				case 'W': append(builder, "wchar"s); assert(*c++ == '@'); break;
			}
			break;
		case '$':
			assert(*c++ == '$');
			assert(*c++ == 'T');
			append(builder, "nullptr_t"s);
			state.last_argument = as_list("nullptr_t"s);
			break;
		default: {
			invalid_code_path();
		}
	}
	if (is_digit(*c)) {
		return;
	}
	switch (*c) {
		case '@': break;
		case 'Z': ++c; break;
		case 0: break;
		default: parse_arguments(state, builder, c, true); break;
	}
}

List<ascii> demangle(ascii const *function_name) {
	auto old_allocator = current_allocator;
	current_allocator = temporary_allocator;

	StringBuilder builder;

	if (*function_name == '?') {
		append(builder, function_name);
		append(builder, ' ');

		// ?fn35@@YA@U?$TemplatedChild@C@?$Templated@E@@@Z - auto f35(Templated<u8>::TemplatedChild<s8>)

		DemangleState state;

		char const *c = function_name + 1;
		char const *name_start = c;
		while (*c != '@') {
			++c;
		}
		append(builder, Span(name_start, c));
		append(builder, "("s);
		++c;
		if (*c++ != '@') goto end;
		if (*c++ != 'Y') goto end;
		if (*c++ != 'A') goto end;
		if (*c++ != '@') goto end;
		parse_arguments(state, builder, c);
		while (*c++ == '0') {
			append(builder, ", "s);
			append(builder, state.last_argument);
		}
		append(builder, ")"s);

	} else {
		append(builder, function_name);
	}
end:
	current_allocator = old_allocator;
	return to_string(builder, current_allocator);
}


}

#endif
#endif
