#define TL_IMPL
#include <tl/common.h>
#include <tl/file.h>
#include <tl/console.h>
#include <tl/main.h>

using namespace tl;

using TokenKind = u16;
enum : TokenKind {
	Token_identifier = 0x100,
};
struct Token {
	TokenKind kind;
	Span<char> view;
	u32 line;
	u32 column;
};

void append(StringBuilder &builder, Token token) {
	append_format(builder, "'{}:{}:{}'", token.view, token.line, token.column);
}

s32 tl_main(Span<Span<utf8>> args) {
	current_allocator = current_temporary_allocator;

	auto signature_path = u8"../data/opengl.h"s;
	auto signature_file = read_entire_file(signature_path);
	if (!signature_file.data) {
		print("Failed to open {}\n", signature_path);
		return 1;
	}

	char *c = (char *)signature_file.data;
	char *end = (char *)signature_file.end();

	List<Token> tokens;
	u32 line = 1;
	char *start_of_line = c;

	auto push_token = [&](Token t) {
		t.line = line;
		t.column = (u32)(t.view.data - start_of_line);
		tokens.add(t);
	};

	while (c != end) {
		while (*c == ' ' || *c == '\t' || *c == '\n' || *c == '\r') {
			if (*c == '\n') {
				start_of_line = c;
				++line;
			}
			++c;
			if (c == end) {
				goto begin_parse;
			}
		}
		if (is_alpha(*c)) {
			Token t = {};
			t.kind = Token_identifier;
			t.view.data = c;
			++c;
			if (c != end) {
				while (is_alpha(*c) || is_digit(*c) || *c == '_') {
					++c;
				}
			}
			t.view.count = c - t.view.data;
			push_token(t);
		} else {
			Token t = {};
			t.view.data = c;
			t.view.count = 1;
			t.kind = *c;
			switch (*c) {
				case '(':
				case ')':
				case ',':
				case ';':
				case '*': break;
				default:
					print("Failed to parse input file: character `{}` is not part of the syntax\n", *c);
					return 2;
			}
			++c;
			push_token(t);
		}
	}

begin_parse:
	struct Arg {
		Span<char> name;
		Span<char> type;
	};
	struct Func {
		Span<char> ret;
		Span<char> name;
		List<Arg> args;
	};

	List<Func> base_funcs;
	List<Func> windows_funcs;
	List<Func> extension_funcs;
	List<Func> *funcs = &base_funcs;

	Token *t = tokens.data;
	while (t != tokens.end()) {
		if (t->view == "EXTENSIONS"s) {
			funcs = &extension_funcs;
			++t;
		} else if (t->view == "WINDOWS"s) {
			funcs = &windows_funcs;
			++t;
		}

		Func f = {};

		List<Token *> pre_aruments;
		while (t->kind != '(') {
			pre_aruments.add(t);
			++t;
		}
		++t;

		if (pre_aruments.count < 2) {
			print("Bad syntax before token {}\n", *t);
			return 3;
		}

		f.name = pre_aruments.back()->view;
		f.ret = {pre_aruments[0]->view.data, pre_aruments.end()[-2]->view.end()};

		while (t->kind != ')') {
			List<Token *> arg_tokens;
			while (t->kind != ',' && t->kind != ')') {
				arg_tokens.add(t);
				++t;
			}
			if (t->kind == ',')
				++t;

			Arg arg;
			arg.name = arg_tokens.back()->view;
			arg.type = { arg_tokens[0]->view.data, arg_tokens.end()[-2]->view.end() };
			f.args.add(arg);
		}
		++t;
		if (t->kind != ';') {
			print("Expected ';' after declaration: {}\n", *t);
			return 3;
		}
		++t;

		funcs->add(f);
	}


	StringBuilder builder;

	append(builder, "#ifdef TL_GL_VALIDATE_EACH_CALL\n");
	{
		auto append_redefines = [&](List<Func> &funcs) {
			for (auto &f : funcs) {
				append_format(builder, "#define {} tl::gl::_{}\n", f.name, f.name);
			}
		};

		append_redefines(base_funcs);

		append_redefines(extension_funcs);

		append(builder, "#if OS_WINDOWS\n");
		append_redefines(windows_funcs);
		append(builder, "#endif\n");
	}
	append(builder, "#else\n");
	{
		auto append_redefines = [&](List<Func> &funcs) {
			for (auto &f : funcs) append_format(builder, "#define {} ::tl_opengl_functions()->_{}\n", f.name, f.name);
		};


		append_redefines(extension_funcs);
		append(builder, "#if OS_WINDOWS\n");
		append_redefines(windows_funcs);
		append(builder, "#endif\n");
	}
	append(builder, "#endif\n");

	write_entire_file(u8"../include/tl/generated/opengl_redefine.h"s, as_bytes(to_string(builder)));
	builder.clear();

	auto append_ds = [&](List<Func> &funcs) {
		for (auto &f : funcs) {
			append_format(builder, "D({}, {}, (", f.ret, f.name);
			for (auto &arg : f.args) {
				append_format(builder, "{} {}", arg.type, arg.name);
				if (&arg != &f.args.back()) {
					append(builder, ", ");
				}
			}
			append_format(builder, "), (");
			for (auto &arg : f.args) {
				append_format(builder, "{}", arg.name);
				if (&arg != &f.args.back()) {
					append(builder, ", ");
				}
			}
			append_format(builder, "))\\\n");

			print("{}\n", f.name);
		}
	};

	append_format(builder, "#define TL_OPENGL_BASE_FUNCS \\\n");
	append_ds(base_funcs);

	append_format(builder, "\n#define TL_OPENGL_EXTENSION_FUNCS \\\n");
	append_ds(extension_funcs);

	append_format(builder, "\n#define TL_OPENGL_WINDOWS_FUNCS \\\n");
	append_ds(windows_funcs);

	write_entire_file(u8"../include/tl/generated/opengl_all_funcs.h"s, as_bytes(to_string(builder)));

	print("Done\n");

	return 0;
}
