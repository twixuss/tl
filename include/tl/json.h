#pragma once
#include "system.h"
#include "list.h"
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
};

struct Object {
	struct v_Object {
		std::unordered_map<Span<char const>, Object> members;
	};
	struct v_Array {
		List<Object> elements;
	};
	struct v_Number {
		f64 value;
	};
	struct v_String {
		Span<char const> view;
	};

	Type type;
	union {
		v_Object object;
		v_Array array;
		v_Number number;
		v_String string;
	};
	Object() {
		memset(this, 0, sizeof(*this));
	}
	Object(f64 number) : type(Type_number), number({number}) {}
	Object(Span<char const> string) : type(Type_string), string({string}) {}
	Object(Type type) {
		this->type = type;
		switch (type) {
			case Type_object: new (&object) v_Object(); break;
			case Type_array:  new (&array)  v_Array (); break;
			case Type_number: new (&number) v_Number(); break;
			case Type_string: new (&string) v_String(); break;
			default:
				break;
		}
	}
	Object(Object const &that) {
		type = that.type;
		switch (type) {
			case Type_object: new (&object) v_Object(that.object); break;
			case Type_array:  new (&array)  v_Array (that.array ); break;
			case Type_number: new (&number) v_Number(that.number); break;
			case Type_string: new (&string) v_String(that.string); break;
			default:
				break;
		}
	}
	Object(Object &&that) {
		type = that.type;
		switch (type) {
			case Type_object: new (&object) v_Object(std::move(that.object)); break;
			case Type_array:  new (&array)  v_Array (std::move(that.array )); break;
			case Type_number: new (&number) v_Number(std::move(that.number)); break;
			case Type_string: new (&string) v_String(std::move(that.string)); break;
			default:
				break;
		}
		that.type = Type_null;
	}
	Object &operator=(Object const &that) { this->~Object(); return *new (this) Object(that); }
	Object &operator=(Object &&that) { this->~Object(); return *new (this) Object(std::move(that)); }
	Object &operator=(f64 value) {
		if (type != Type_null)
			assert(type == Type_number);
		number.value = value;
		return *this;
	}
	Object &operator=(Span<char const> value) {
		if (type != Type_null)
			assert(type == Type_string);
		string.view = value;
		return *this;
	}
	~Object() {
		switch (type) {
			case Type_object: object.~v_Object(); break;
			case Type_array:  array .~v_Array (); break;
			case Type_number: number.~v_Number(); break;
			case Type_string: string.~v_String(); break;
			default:
				break;
		}
		type = Type_null;
	}
	Object &operator[](Span<char const> name) {
		assert(type == Type_object);
		return object.members[name];
	}
	template <umm size>
	Object &operator[](char const (&name)[size]) {
		return (*this)[Span<char const>(name, length(name))];
	}
	Object &operator[](umm i) {
		assert(type == Type_array);
		return array.elements[i];
	}
	Object &operator[](Object const &i) {
		switch (i.type) {
			case Type_number: return (*this)[(umm)i.number.value];
			case Type_string: return (*this)[i.string.view];
		}
		INVALID_CODE_PATH();
		return *this;
	}
};

using TokenType = u32;
enum : TokenType {
	Token_number = 0x100,
};

struct Token {
	TokenType type;
	Span<char const> view;
};

TL_API List<Token> lex(Span<char const> json);
TL_API Object parse(Token *&t);
TL_API Object parse(List<Token> tokens);
TL_API Object parse(Span<char const> json);

}
}

#ifdef TL_IMPL

namespace TL {
namespace Json {

List<Token> lex(Span<char const> json) {
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
				result.push_back(token);
				++c;
				continue;
			}
			case '"': {
				++c;
				Token token;
				token.type = '"';
				token.view._begin = c;
				while (*c++ != '"') {}
				token.view._end = c - 1;
				result.push_back(token);
				continue;
			}
		}
		if (isDigit(*c) || *c == '-') {
			Token token;
			token.type = Token_number;
			token.view._begin = c;
			++c;
			while (1) { 
				if (isDigit(*c) || *c == '.') {
					++c; 
				} else if (*c == 'e') {
					c += 2;
					while (isDigit(*c)) {
						++c;
					}
					break;
				} else {
					break;
				}
			}
			token.view._end = c;
			result.push_back(token);
			continue;
		}
		if(*c == ' ')
			break;

		INVALID_CODE_PATH();
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
					Span<char const> memberName = t->view;
					++t;
					assert(t++->type == ':');
					result.object.members[memberName] = parse(t);
				} else {
					INVALID_CODE_PATH();
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
		result.string.view = t->view;
		++t;
		return result;
	} else if (t->type == Token_number) {
		Object result = {Type_number};
		char temp[256];
		memcpy(temp, t->view.data(), t->view.size());
		temp[t->view.size()] = 0;
		result.number.value = atof(temp);
		++t;
		return result;
	} else if (t->type == '[') {
		Object result = {Type_array};
		++t;
		if (t->type != ']') {
			while (1) {
				result.array.elements.push_back(parse(t));

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
		INVALID_CODE_PATH();
		return {};
	}
}
Object parse(List<Token> tokens) {
	assert(tokens.front().type == '{');
	assert(tokens.back().type == '}');
	Token *t = tokens.begin();
	return parse(t);
}
Object parse(Span<char const> json) {
	return parse(lex(json));
}

}
}

#endif

#pragma warning(pop)
