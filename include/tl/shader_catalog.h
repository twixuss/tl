#pragma once
#include "opengl.h"
#include "file.h"
#include "string.h"
#include "std_hash.h"
#include "buffer.h"
#include "console.h"
#include <unordered_map>

namespace TL {

struct Shader {
	GLuint program;
	List<utf8> path;
	FileTracker tracker;
	std::unordered_map<Span<char>, GLuint> uniforms;
	bool valid;

	u32 cull;
	u32 src_blend, dst_blend;
	u32 depth_func;
	bool depth_write;
	bool depth_test;
	bool enable_blend;
};
struct ShaderCatalog {
	Allocator allocator;
	std::unordered_map<Span<utf8>, Shader> shaders;
	ListList<utf8> shader_file_names;
	Shader *fallback_shader;
};

void free(ShaderCatalog &catalog) {
	for (auto &[name, shader] : catalog.shaders) {
		free(shader.path);
		free(shader.tracker);
		for (auto &[name, index] : shader.uniforms) {
			FREE(catalog.allocator, name.data);
		}
	}
	free(catalog.shader_file_names);
}

TL_API Shader *find(ShaderCatalog &catalog, Span<utf8> name);

TL_API bool parse_shader(Shader &shader, Span<char> source);
TL_API Buffer load_shader_file(Span<utf8> full_path);
TL_API Shader &add_file(ShaderCatalog &catalog, Span<utf8> directory, Span<utf8> full_name);
TL_API void init(ShaderCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_shader_name);
TL_API bool update(ShaderCatalog &catalog);
TL_API GLuint get_uniform_location(Shader &shader, Span<char> name);
TL_API void set_uniform(Shader &shader, Span<char> name, f32 value);
TL_API void set_uniform(Shader &shader, Span<char> name, v2f value);
TL_API void set_uniform(Shader &shader, Span<char> name, v3f value);
TL_API void set_uniform(Shader &shader, Span<char> name, v4f value);
TL_API void set_uniform(Shader &shader, Span<char> name, m4  value);
TL_API void use_shader(Shader &shader);

inline void set_uniform(Shader &shader, char const *name, f32 value) { set_uniform(shader, as_span(name), value); }
inline void set_uniform(Shader &shader, char const *name, v2f value) { set_uniform(shader, as_span(name), value); }
inline void set_uniform(Shader &shader, char const *name, v3f value) { set_uniform(shader, as_span(name), value); }
inline void set_uniform(Shader &shader, char const *name, v4f value) { set_uniform(shader, as_span(name), value); }
inline void set_uniform(Shader &shader, char const *name, m4  value) { set_uniform(shader, as_span(name), value); }

#ifdef TL_IMPL

Shader *find(ShaderCatalog &catalog, Span<utf8> name) {
	auto result = catalog.shaders.find(name);
	if (result != catalog.shaders.end()) {
		if (result->second.valid) {
			return &result->second;
		} else {
			print("Shader '%' is invalid\n", name);
		}
	} else {
		print("Shader '%' not found\n", name);
	}
	return catalog.fallback_shader;
}

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

Shader &add_file(ShaderCatalog &catalog, Span<utf8> directory, Span<utf8> full_name) {
	using namespace OpenGL;

	auto file_name = full_name;
	file_name.size = find(file_name, u8'.') - file_name.data;

	auto &shader = catalog.shaders[file_name];

	shader.path = (List<utf8>)concatenate(directory, '/', full_name, '\0');
	shader.tracker = create_file_tracker(shader.path, [&](FileTracker &tracker) {
		print("Compiling %\n", tracker.path);

		auto source = load_shader_file(as_span(tracker.path));

		auto vertex_shader = create_shader(GL_VERTEX_SHADER, 330, true, as_chars(source));
		if (vertex_shader) {
			auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, 330, true, as_chars(source));
			if (fragment_shader) {
				if (shader.program) {
					glDeleteProgram(shader.program);
				}
				shader.program = create_program(vertex_shader, fragment_shader);
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
				if (shader.program) {
					if (parse_shader(shader, as_chars(source))) {
						shader.uniforms.clear();

						GLint count;
						glGetProgramiv(shader.program, GL_ACTIVE_UNIFORMS, &count);

						const GLsizei bufSize = 64;
						GLchar name[bufSize];
						GLsizei length;
						GLint size;
						GLenum type;
						for (u32 i = 0; i < (u32)count; i++) {
							glGetActiveUniform(shader.program, i, bufSize, &length, &size, &type, name);
							Span<char> stored_name = {ALLOCATE(char, catalog.allocator, length), (umm)length};
							memcpy(stored_name.data, name, length);
							auto found = shader.uniforms.find(stored_name);
							if (found != shader.uniforms.end()) {
								FREE(catalog.allocator, found->first.data);
							}
							shader.uniforms[stored_name] = glGetUniformLocation(shader.program, name);
						}

						shader.valid = true;
						return;
					}
				}
			}
		}
		shader.valid = false;
	});
	update_file_tracker(shader.tracker);
	return shader;
}

void init(ShaderCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_shader_name) {
	catalog.allocator = current_allocator;
	catalog.shader_file_names = get_files_in_directory(directory);
	auto fallback_shader_path = (List<utf8>)with(temporary_allocator, concatenate(fallback_shader_name, ".glsl"));
	catalog.fallback_shader = &add_file(catalog, directory, fallback_shader_path);
	assert(catalog.fallback_shader->program);
	for (auto &full_name : catalog.shader_file_names) {
		if (full_name == as_span(fallback_shader_path))
			continue;
		add_file(catalog, directory, full_name);
	}
	update(catalog);
}

//
// Updates file trackers in the catalog and reloads modified shaders.
// Should be used only for development.
//
bool update(ShaderCatalog &catalog) {
	bool any_updated = false;
	for (auto &[name, shader] : catalog.shaders) {
		any_updated |= update_file_tracker(shader.tracker);
	}
	return any_updated;
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

#endif

}
