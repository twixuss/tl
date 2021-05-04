#pragma once
#include "system.h"
#include "list.h"
#include "std_hash.h"
#include <unordered_map>

#pragma warning(push)
#pragma warning(disable: 4820) // padding after struct
#pragma warning(disable: 4582) // constructor not called implicitly
#pragma warning(disable: 4583) // destructor not called implicitly
#pragma warning(disable: 4061 4062) // incomplete switch

namespace TL {
namespace Json {

enum Type {
	Type_null,
	Type_object,
	Type_array,
	Type_number,
	Type_string,
	Type_boolean,
};

struct Object {
	struct MemberMap {
		List<Span<utf8>> names;
		List<Object> values;
	};
	using Array = List<Object>;
	using String = Span<utf8>;

	Type type;
	union {
		MemberMap members;
		Array array;
		f64 number;
		String string;
		bool boolean;
	};
	Object() {
		memset(this, 0, sizeof(*this));
	}
	Object(f64 number) : type(Type_number), number(number) {}
	Object(Span<utf8> string) : type(Type_string), string({string}) {}
	Object(Type type) {
		this->type = type;
		switch (type) {
			case Type_object:  new (&members) MemberMap(); break;
			case Type_array:   new (&array)   Array    (); break;
			case Type_number:  new (&number)  f64      (); break;
			case Type_string:  new (&string)  String   (); break;
			case Type_boolean: new (&boolean) bool     (); break;
			default:
				break;
		}
	}
	~Object() = default;
	Object &operator=(f64 value) {
		if (type != Type_null)
			assert(type == Type_number);
		number = value;
		return *this;
	}
	Object &operator=(Span<utf8> value) {
		if (type != Type_null)
			assert(type == Type_string);
		string = value;
		return *this;
	}
	Object &operator=(bool value) {
		if (type != Type_null)
			assert(type == Type_boolean);
		boolean = value;
		return *this;
	}
	Object *member(Span<utf8> name) {
		assert(type == Type_object);
		auto found = find(members.names, name);
		if (found)
			return &members.values[index_of(members.names, found)];
		return 0;
	}
	Object *index(umm i) {
		assert(type == Type_array);
		return &array[i];
	}
};

using TokenType = u32;
enum : TokenType {
	Token_number = 0x100,
	Token_true,
	Token_false,
};

struct Token {
	TokenType type;
	Span<utf8> view;
};

TL_API List<Token> lex(Span<utf8> json);
TL_API Object parse(Token *&t);
TL_API Object parse(List<Token> tokens);
TL_API Object parse(Span<utf8> json);

}

inline void free(Json::Object &obj) {
	switch (obj.type) {
		case Json::Type_object: {
			for (auto &member : obj.members.values) {
				free(member);
			}
			free(obj.members.values);
			free(obj.members.names);
			break;
		}
		case Json::Type_array: {
			for (auto &element : obj.array) {
				free(element);
			}
			free(obj.array);
			break;
		}
		default: break;
	}
	obj.type = Json::Type_null;
}

inline void append(StringBuilder &b, Json::Object obj) {
	switch (obj.type) {
		case Json::Type_number: {
			append(b, obj.number);
			break;
		}
		case Json::Type_string: {
			append(b, '"');
			append(b, obj.string);
			append(b, '"');
			break;
		}
		case Json::Type_array: {
			append(b, '[');
			bool comma = false;
			for (auto &e : obj.array) {
				if (comma) {
					append(b, ',');
				}
				append(b, e);
				comma = true;
			}
			append(b, ']');
			break;
		}
		case Json::Type_object: {
			append(b, '{');
			bool comma = false;
			for (u32 i = 0; i < obj.members.names.size; ++i) {
				auto name = obj.members.names[i];
				auto value = obj.members.values[i];
				if (comma) {
					append(b, ',');
				}
				append(b, name);
				append(b, ':');
				append(b, value);
				comma = true;
			}
			append(b, '}');
			break;
		}
	}
}

}

#ifdef TL_IMPL

namespace TL {
namespace Json {

List<Token> lex(Span<utf8> json) {
	List<Token> result;
	auto c = json.begin();
	while (c != json.end()) {
		switch (*c) {
			case '{':
			case '}':
			case '[':
			case ']':
			case ':':
			case ',': {
				Token token;
				token.type = (TokenType)*c;
				token.view = {c, 1};
				result.add(token);
				++c;
				continue;
			}
			case '"': {
				++c;
				Token token;
				token.type = '"';
				token.view.data = c;
				while (*c++ != '"') {}
				token.view.size = c - token.view.data - 1;
				result.add(token);
				continue;
			}
		}

		auto check_keyword = [&] (Span<utf8> keyword, TokenType token_type) {
			if (Span(c, min(c + keyword.size, json.end())) == keyword) {
				Token token;
				token.type = token_type;
				token.view = {c, keyword.size};
				c += keyword.size;
				result.add(token);
				return true;
			}
			return false;
		};

		if (check_keyword(u8"true"s,  Token_true)) continue;
		if (check_keyword(u8"false"s, Token_false)) continue;

		if (is_digit(*c) || *c == '-') {
			Token token;
			token.type = Token_number;
			token.view.data = c;
			++c;
			while (1) {
				if (is_digit(*c) || *c == '.') {
					++c;
				} else if (*c == 'e') {
					c += 2;
					while (is_digit(*c)) {
						++c;
					}
					break;
				} else {
					break;
				}
			}
			token.view.size = c - token.view.data;
			result.add(token);
			continue;
		}
		if(*c == ' ')
			break;

		invalid_code_path();
	}
	return result;
}

Object parse(Token *&t) {
	if (t->type == '{') {
		Object result = {Type_object};
		++t;
		if (t->type != '}') {
			while (1) {
				if (t->type == '"') {
					Span<utf8> member_name = t->view;
					++t;
					assert(t++->type == ':');
					result.members.names.add(member_name);
					result.members.values.add(parse(t));
				} else {
					invalid_code_path();
				}

				if (t->type == '}') {
					++t;
					break;
				} else {
					assert(t++->type == ',');
				}
			}
		}
		return result;
	} else if (t->type == '"') {
		Object result = {Type_string};
		result.string = t->view;
		++t;
		return result;
	} else if (t->type == Token_number) {
		Object result = {Type_number};
		utf8 temp[256];
		memcpy(temp, t->view.data, t->view.size);
		temp[t->view.size] = 0;
		result.number = atof((char *)temp);
		++t;
		return result;
	} else if (t->type == Token_true || t->type == Token_false) {
		Object result = {Type_boolean};
		result.boolean = t->type == Token_true;
		++t;
		return result;
	} else if (t->type == '[') {
		Object result = {Type_array};
		++t;
		if (t->type != ']') {
			while (1) {
				result.array.add(parse(t));

				if (t->type == ']') {
					++t;
					break;
				} else {
					assert(t++->type == ',');
				}
			}
		}
		return result;
	} else {
		invalid_code_path();
		return {};
	}
}
Object parse(List<Token> tokens) {
	assert(tokens.front().type == '{');
	assert(tokens.back().type == '}');
	Token *t = tokens.begin();
	return parse(t);
}
Object parse(Span<utf8> json) {
	return parse(lex(json));
}

}
}

#endif

#pragma warning(pop)
