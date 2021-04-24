#pragma once
#include "common.h"
#include "std_hash.h"
#include <unordered_map>

namespace TL {

struct Shader {
	u32 program;
	std::unordered_map<Span<char>, u32> uniforms;
	u32 cull;
	u32 src_blend, dst_blend;
	u32 depth_func;
	bool depth_write;
	bool depth_test;
	bool enable_blend;
};
#define TL_SHADER_CATALOG_ENTRY ::TL::Shader

}

#include "shader_catalog.h"
#include "buffer.h"
#include "console.h"
#include "opengl.h"

#ifndef timed_block
#define timed_block(...)
#endif

namespace TL {

bool parse_shader(Shader &shader, Span<char> source) {
	shader.cull = 0;
	shader.depth_write = true;
	shader.enable_blend = false;
	shader.depth_func = GL_LEQUAL;
	shader.depth_test = true;

	bool success = true;

	auto get_blend = [&](Span<char> str) {
		if (str == "src_alpha"s) {
			return GL_SRC_ALPHA;
		} else if (str == "inv_src_alpha"s) {
			return GL_ONE_MINUS_SRC_ALPHA;
		} else if (str == "src_color"s) {
			return GL_SRC_COLOR;
		} else if (str == "inv_src_color"s) {
			return GL_ONE_MINUS_SRC_COLOR;
		} else if (str == "src_color2"s) {
			return GL_SRC1_COLOR;
		} else if (str == "inv_src_color2"s) {
			return GL_ONE_MINUS_SRC1_COLOR;
		} else if (str == "one"s) {
			return GL_ONE;
		} else if (str == "zero"s) {
			return GL_ZERO;
		}
		success = false;
		print("bad blend param\n");
		return GL_ONE;
	};
	auto get_cull = [&](Span<char> str) {
		if (str == "off"s) {
			return 0;
		} else if (str == "back"s) {
			return GL_BACK;
		} else if (str == "front"s) {
			return GL_FRONT;
		}
		success = false;
		print("bad cull param\n");
		return 0;
	};
	auto get_bool = [&](Span<char> str) {
		if (str == "off"s) {
			return false;
		} else if (str == "on"s) {
			return true;
		}
		success = false;
		print("bad bool param\n");
		return false;
	};

	auto params_start = find(source, "/*!\n"s);
	auto params_end   = find(source, "\n!*/"s);
	if ((bool)params_start != (bool)params_end) {
		success = false;
		print("Bad parameter scope\n");
	} else if (params_start && params_end) {
		StaticList<Span<char>, 16> tokens;
		auto start = params_start + 4;
		while (1) {
			while (is_whitespace(*start))
				++start;

			if (*start == '!')
				break;

			if (*start == '#') {
				while (*start != '\n') {
					++start;
				}
				continue;
			}

			auto end = start;
			while (!is_whitespace(*end))
				++end;
			tokens.add({start, end});
			start = end + 1;
		}
		auto token = tokens.begin();
		while (token != tokens.end()) {
			if (*token == "blend"s) {
				shader.enable_blend = true;
				shader.src_blend = get_blend(*++token);
				shader.dst_blend = get_blend(*++token);
			} else if (*token == "cull"s) {
				shader.cull = get_cull(*++token);
			} else if (*token == "depth_write"s) {
				shader.depth_write = get_bool(*++token);
			} else if (*token == "depth_test"s) {
				shader.depth_test = get_bool(*++token);
			} else if (*token == "depth_func"s) {
				auto func = *++token;
				if (func == "less"s) shader.depth_func = GL_LESS;
				else if (func == "less_equal"s) shader.depth_func = GL_LEQUAL;
				else if (func == "greater"s) shader.depth_func = GL_GREATER;
				else if (func == "greater_equal"s) shader.depth_func = GL_GEQUAL;
				else if (func == "equal"s) shader.depth_func = GL_EQUAL;
				else if (func == "not_equal"s) shader.depth_func = GL_NOTEQUAL;
				else {
					print("bad depth_func param\n");
					success = false;
				}
			} else {
				print("bad token: %\n", *token);
				success = false;
			}
			++token;
		}
	}
	return success;
}

Buffer load_shader_file(Span<utf8> full_path) {
	scoped_allocator(temporary_allocator);

	auto source = read_entire_file(full_path);
	assert(source.data);

	for (auto &c : source) {
		if (c == '\r') {
			c = '\n';
		}
	}

	auto directory = full_path;
	while (directory.back() != '/') --directory.size;

	while (1) {
		auto directive = "//include "s;
		auto include_start = find(source, as_bytes(directive));
		if (!include_start) {
			break;
		}
		auto file_name_start = include_start + directive.size;
		auto file_name_end = file_name_start;
		while (*file_name_end != '\n') {
			++file_name_end;
		}

		Span<utf8> file_name = {(utf8 *)file_name_start, (utf8 *)file_name_end};

		auto file_path = (List<utf8>)concatenate(directory, file_name);
		auto included_file = load_shader_file(file_path);

		auto full_directive_size = directive.size + file_name.size;
		auto new_source_size = source.size - full_directive_size + included_file.size;

		auto new_source = create_buffer(new_source_size);

		auto cursor = new_source.data;
		auto append_copy = [&](u8 const *from, umm size) {
			memcpy(cursor, from, size);
			cursor += size;
			assert(cursor <= (new_source.data + new_source_size));
		};
		append_copy(source.begin(), include_start - source.begin());
		append_copy(included_file.data, included_file.size);
		append_copy(file_name_end, source.end() - file_name_end);

		source = new_source;
	}
	return source;
}

GLuint get_uniform_location(Shader &shader, Span<char> name) {
	auto it = shader.uniforms.find(name);
	if (it == shader.uniforms.end()) {
		return -1;
	}
	return it->second;
}

void set_uniform(Shader &shader, Span<char> name, f32 value) { OpenGL::glUniform1f(get_uniform_location(shader, name), value); }
void set_uniform(Shader &shader, Span<char> name, v2f value) { OpenGL::glUniform2fv(get_uniform_location(shader, name), 1, value.s); }
void set_uniform(Shader &shader, Span<char> name, v3f value) { OpenGL::glUniform3fv(get_uniform_location(shader, name), 1, value.s); }
void set_uniform(Shader &shader, Span<char> name, v4f value) { OpenGL::glUniform4fv(get_uniform_location(shader, name), 1, value.s); }
void set_uniform(Shader &shader, Span<char> name, m4  value) { OpenGL::glUniformMatrix4fv(get_uniform_location(shader, name), 1, false, value.s); }

void use_shader(Shader &shader) {
	using namespace OpenGL;
	glUseProgram(shader.program);

	if (shader.cull) {
		glEnable(GL_CULL_FACE);
		glCullFace(shader.cull);
	} else {
		glDisable(GL_CULL_FACE);
	}

	if (shader.depth_test) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(shader.depth_func);
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	glDepthMask(shader.depth_write);

	if (shader.enable_blend) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(shader.src_blend, shader.dst_blend, GL_ZERO, GL_ONE);
	} else {
		glDisable(GL_BLEND);
	}
}

void init_opengl_shader_catalog(ShaderCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_name) {
	using namespace OpenGL;

	catalog.update_entry = [](ShaderCatalog::Entry &entry) {
		timed_block("ShaderCatalog::update_entry"s);

		print("Compiling %\n", entry.tracker.path);

		auto source = load_shader_file(as_span(entry.tracker.path));

		auto vertex_shader = create_shader(GL_VERTEX_SHADER, 330, true, as_chars(source));
		if (vertex_shader) {
			auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, 330, true, as_chars(source));
			if (fragment_shader) {
				if (entry.program) {
					glDeleteProgram(entry.program);
				}
				entry.program = create_program(vertex_shader, fragment_shader);
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
				if (entry.program) {
					if (parse_shader(entry, as_chars(source))) {
						entry.uniforms.clear();

						GLint count;
						glGetProgramiv(entry.program, GL_ACTIVE_UNIFORMS, &count);

						const GLsizei bufSize = 64;
						GLchar name[bufSize];
						GLsizei length;
						GLint size;
						GLenum type;
						for (u32 i = 0; i < (u32)count; i++) {
							glGetActiveUniform(entry.program, i, bufSize, &length, &size, &type, name);
							Span<char> allocated_name = {ALLOCATE(char, default_allocator, length), (umm)length};
							memcpy(allocated_name.data, name, length);
							auto found = entry.uniforms.find(allocated_name);
							if (found != entry.uniforms.end()) {
								FREE(default_allocator, found->first.data);
							}
							entry.uniforms[allocated_name] = glGetUniformLocation(entry.program, name);
						}
						return;
					}
				}
			}
		}
		entry.program = 0;
	};
	catalog.free_entry = [](ShaderCatalog::Entry &shader) {
		for (auto &[name, index] : shader.uniforms) {
			FREE(default_allocator, name.data);
		}
	};
	catalog.entry_valid = [](ShaderCatalog::Entry &entry) {
		return entry.program != 0;
	};
	init(catalog, directory, fallback_name);
}

}
