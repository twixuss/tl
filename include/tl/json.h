#pragma once
#include "system.h"
#include "list.h"

#pragma warning(push)
#pragma warning(disable: 4820) // padding after struct
#pragma warning(disable: 4582) // constructor not called implicitly
#pragma warning(disable: 4583) // destructor not called implicitly
#pragma warning(disable: 4061 4062) // incomplete switch

namespace tl {
namespace Json {

#define TL_ENUMERATE_JSON_TYPES \
	x(null) \
	x(object) \
	x(array) \
	x(number) \
	x(string) \
	x(boolean) \

enum class Type {
	#define x(name) name,
	TL_ENUMERATE_JSON_TYPES
	#undef x
};

inline umm append(StringBuilder &builder, Type type) {
	switch (type) {
		#define x(name) case Type::name: return append(builder, #name);
		TL_ENUMERATE_JSON_TYPES
		#undef x
	}
	return 0;
}

struct Object {
	struct MemberMap {
		List<Span<utf8>> names;
		List<Object> values;
	};
	using Array = List<Object>;
	using String = Span<utf8>;

	Type type;
	union {
		MemberMap _members;
		Array _array;
		f64 _number;
		String _string;
		bool _boolean;
	};
	Object() {
		memset(this, 0, sizeof(*this));
	}
	Object(f64 number) : type(Type::number), _number(number) {}
	Object(Span<utf8> string) : type(Type::string), _string({string}) {}
	Object(Type type) {
		this->type = type;
		switch (type) {
			case Type::object:  new (&_members) MemberMap(); break;
			case Type::array:   new (&_array)   Array    (); break;
			case Type::number:  new (&_number)  f64      (); break;
			case Type::string:  new (&_string)  String   (); break;
			case Type::boolean: new (&_boolean) bool     (); break;
			default:
				break;
		}
	}
	~Object() = default;
	Object &operator=(f64 value) {
		if (type != Type::null)
			assert_equal(type, Type::number);
		_number = value;
		return *this;
	}
	Object &operator=(Span<utf8> value) {
		if (type != Type::null)
			assert_equal(type, Type::string);
		_string = value;
		return *this;
	}
	Object &operator=(bool value) {
		if (type != Type::null)
			assert_equal(type, Type::boolean);
		_boolean = value;
		return *this;
	}
	Object *member(Span<utf8> name) {
		assert_equal(type, Type::object);
		auto found = find(_members.names, name);
		if (found)
			return &_members.values[index_of(_members.names, found)];
		return 0;
	}
	Object *index(umm i) {
		assert_equal(type, Type::array);
		return &_array[i];
	}
	f64 number() {
		assert_equal(type, Type::number);
		return _number;
	}
	Array &array() {
		assert_equal(type, Type::array);
		return _array;
	}
	String &string() {
		assert_equal(type, Type::string);
		return _string;
	}
	bool &boolean() {
		assert_equal(type, Type::boolean);
		return _boolean;
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
		case Json::Type::object: {
			for (auto &member : obj._members.values) {
				free(member);
			}
			free(obj._members.values);
			free(obj._members.names);
			break;
		}
		case Json::Type::array: {
			for (auto &element : obj._array) {
				free(element);
			}
			free(obj._array);
			break;
		}
		default: break;
	}
	obj.type = Json::Type::null;
}

inline void append(StringBuilder &b, Json::Object obj) {
	switch (obj.type) {
		case Json::Type::number: {
			append(b, obj.number());
			break;
		}
		case Json::Type::string: {
			append(b, '"');
			append(b, obj._string);
			append(b, '"');
			break;
		}
		case Json::Type::array: {
			append(b, '[');
			bool comma = false;
			for (auto &e : obj._array) {
				if (comma) {
					append(b, ',');
				}
				append(b, e);
				comma = true;
			}
			append(b, ']');
			break;
		}
		case Json::Type::object: {
			append(b, '{');
			bool comma = false;
			for (u32 i = 0; i < obj._members.names.count; ++i) {
				auto name = obj._members.names[i];
				auto value = obj._members.values[i];
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

namespace tl {
namespace Json {

List<Token> lex(Span<utf8> json) {
	List<Token> result;
	auto c = json.begin();
	while (c != json.end()) {
		while (*c == ' ' || *c == '\t' || *c == '\n' || *c == '\r') {
			++c;
			if (c == json.end()) {
				goto end;
			}
		}
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
				token.view.count = c - token.view.data - 1;
				result.add(token);
				continue;
			}
		}

		auto check_keyword = [&] (Span<utf8> keyword, TokenType token_type) {
			if (Span(c, min(c + keyword.count, json.end())) == keyword) {
				Token token;
				token.type = token_type;
				token.view = {c, keyword.count};
				c += keyword.count;
				result.add(token);
				return true;
			}
			return false;
		};

		if (check_keyword(u8"true"s,  Token_true)) continue;
		if (check_keyword(u8"false"s, Token_false)) continue;

		if (is_digit((ascii)*c) || *c == '-') {
			Token token;
			token.type = Token_number;
			token.view.data = c;
			++c;
			while (1) {
				if (is_digit((ascii)*c) || *c == '.') {
					++c;
				} else if (*c == 'e') {
					c += 2;
					while (is_digit((ascii)*c)) {
						++c;
					}
					break;
				} else {
					break;
				}
			}
			token.view.count = c - token.view.data;
			result.add(token);
			continue;
		}
		if(*c == ' ')
			break;

		invalid_code_path();
	}
end:
	return result;
}

Object parse(Token *&t) {
	if (t->type == '{') {
		Object result = {Type::object};
		++t;
		if (t->type != '}') {
			while (1) {
				if (t->type == '"') {
					Span<utf8> member_name = t->view;
					++t;
					assert_equal(t++->type, ':');
					result._members.names.add(member_name);
					result._members.values.add(parse(t));
				} else {
					invalid_code_path();
				}

				if (t->type == '}') {
					break;
				} else {
					assert_equal(t->type, ',');
					++t;
					// Allow trailing comma ;)
					if (t->type == '}') {
						break;
					}
				}
			}
		}
		++t;
		return result;
	} else if (t->type == '"') {
		Object result = {Type::string};
		result._string = t->view;
		++t;
		return result;
	} else if (t->type == Token_number) {
		Object result = {Type::number};
		utf8 temp[256];
		memcpy(temp, t->view.data, t->view.count);
		temp[t->view.count] = 0;
		result._number = atof((char *)temp);
		++t;
		return result;
	} else if (t->type == Token_true || t->type == Token_false) {
		Object result = {Type::boolean};
		result._boolean = t->type == Token_true;
		++t;
		return result;
	} else if (t->type == '[') {
		Object result = {Type::array};
		++t;
		if (t->type != ']') {
			while (1) {
				result._array.add(parse(t));

				if (t->type == ']') {
					break;
				} else {
					assert_equal(t->type, ',');
					t++;
				}
			}
		}
		++t;
		return result;
	} else {
		invalid_code_path();
		return {};
	}
}
Object parse(List<Token> tokens) {
	assert_equal(tokens.front().type, '{');
	assert_equal(tokens.back().type, '}');
	Token *t = tokens.begin();
	return parse(t);
}
Object parse(Span<utf8> json) {
	auto tokens = lex(json);
	defer { free(tokens); };
	return parse(tokens);
}

}
}

#endif

#pragma warning(pop)
