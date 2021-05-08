#pragma once
#define TL_IMGUI_TEXTURE_HANDLE u32
#define TL_IMGUI_SHADER_HANDLE u32
#include "shader_catalog_opengl.h"
#include "imgui.h"
#include "font.h"
#include "opengl.h"
#include <unordered_map>
namespace TL {
namespace Imgui {

struct FontVertex {
	v2f position;
	v2f uv;
};

struct TextCache {
	List<utf8> text;
	List<FontVertex> vertices;
	u32 buffer;
};

u32 just_color_batch_shader;
ShaderCatalog::Entry *texture_shader;
u32 font_shader;
u32 font_uniforms;

struct FontUniforms {
	v2f position_offset;
	v2f position_scale;
	v4f color;
};

FontCollection *font_collection;

std::unordered_map<u32, TextCache> text_caches;

void begin_frame() {
	begin_frame_base();
	glEnable(GL_SCISSOR_TEST);
}
void end_frame() {
	end_frame_base();
}
void begin() {
	begin_base();
}
void end() {
	end_base();
}

void init(Window *window, FontCollection *font_collection) {
	using namespace ::TL::OpenGL;
	init_base(window);
	::TL::Imgui::font_collection = font_collection;

	auto just_color_batch_source = R"(
#ifdef VERTEX_SHADER
#define V2F out
#else
#define V2F in
#endif

V2F vec4 vertex_color;

#ifdef VERTEX_SHADER

layout(location = 0) in vec2 input_position;
layout(location = 1) in vec4 input_color;

void main() {
	gl_Position = vec4(input_position, 0, 1);
	vertex_color = input_color;
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 fragment_color;

void main() {
	fragment_color = vertex_color;
}

#endif)"s;

	auto vertex_shader = OpenGL::create_shader(GL_VERTEX_SHADER, 330, true, just_color_batch_source);
	auto fragment_shader = OpenGL::create_shader(GL_FRAGMENT_SHADER, 330, true, just_color_batch_source);
	just_color_batch_shader = OpenGL::create_program(vertex_shader, fragment_shader);


	auto font_source = R"(
#ifdef VERTEX_SHADER
#define V2F out
#else
#define V2F in
#endif

uniform sampler2D main_texture;
layout(std140) uniform _ {
	vec2 position_offset;
	vec2 position_scale;
	vec4 color;
};

V2F vec2 vertex_uv;

#ifdef VERTEX_SHADER

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_uv;

void main() {
	vec2 p = (i_position + position_offset) * position_scale - 1;
	p.y = -p.y;
	gl_Position = vec4(p, 0, 1);
	vertex_uv = i_uv;
}

#endif

#ifdef FRAGMENT_SHADER

layout(location = 0, index = 0) out vec4 fragment_text_color;
layout(location = 0, index = 1) out vec4 fragment_text_mask;

void main() {
	fragment_text_color = color;
	fragment_text_mask = color.a * texture(main_texture, vertex_uv);
}

#endif
)"s;
	vertex_shader = OpenGL::create_shader(GL_VERTEX_SHADER, 330, true, font_source);
	fragment_shader = OpenGL::create_shader(GL_FRAGMENT_SHADER, 330, true, font_source);
	font_shader = OpenGL::create_program(vertex_shader, fragment_shader);

	glGenBuffers(1, &font_uniforms);
	glBindBuffer(GL_UNIFORM_BUFFER, font_uniforms);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(FontUniforms), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	u32 buffer_index = glGetUniformBlockIndex(font_shader, "_");
	glUniformBlockBinding(font_shader, buffer_index, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, font_uniforms);
}

void _set_scissor_impl(aabb<v2s> region) {
	::glScissor(region.min.x, window->client_size.y - region.max.y, region.size().x, region.size().y);
}

void _draw_and_free_elements(Span<UIElement> elements) {
	using namespace OpenGL;

	struct PanelVertex {
		v2f position;
		v4f color;
	};

	List<PanelVertex> panel_vertices;
	panel_vertices.allocator = temporary_allocator;

	for (auto &element : elements) {
		switch (element.kind) {
			case UIElement_panel: {
				auto p = element.panel;
				auto color = p.color;
				auto rect  = p.rect;

				auto min = client_to_ndc(rect.min);
				auto max = client_to_ndc(rect.max);

				panel_vertices.add({
					{{min.x, min.y}, color},
					{{min.x, max.y}, color},
					{{max.x, max.y}, color},
					{{max.x, min.y}, color},
				});

				break;
			}
		}
	}

	static GrowingBuffer<PanelVertex> panel_vertex_buffer;
	panel_vertex_buffer.usage = GL_STATIC_COPY;
	panel_vertex_buffer.init();
	panel_vertex_buffer.reset(panel_vertices);

	glUseProgram(just_color_batch_shader);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	glBindBuffer(GL_ARRAY_BUFFER, panel_vertex_buffer.buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(PanelVertex), (void *)offsetof(PanelVertex, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(PanelVertex), (void *)offsetof(PanelVertex, color));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_QUADS, 0, panel_vertices.size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (auto &element : elements) {
		defer { free(element); };

		if (volume(element.scissor_rect) <= 0) {
			continue; // Maybe do early check??
		}

		auto old_scissor = current_scissor;
		defer { set_scissor(old_scissor); };
		set_scissor(element.scissor_rect);
		switch (element.kind) {
			case UIElement_texture: {
				auto t = element.texture;
				auto texture = t.texture;
				auto color   = t.color;
				auto rect    = t.rect;

				glBindTexture(GL_TEXTURE_2D, texture);
				use_shader(*texture_shader);
				set_uniform(*texture_shader, "color", color);
				set_uniform(*texture_shader, "position_min", client_to_ndc(rect.min));
				set_uniform(*texture_shader, "position_max", client_to_ndc(rect.max));
				glDrawArrays(GL_QUADS, 0, 4);
				break;
			}
			case UIElement_text: {
				auto t         = element.text;
				auto id        = t.id;
				auto color     = t.color;
				auto rect      = t.rect;
				auto alignment = t.alignment;
				auto font_size = t.font_size;
				auto text      = t.text;

				auto font = get_font_at_size(font_collection, font_size);

				v2s position;
				switch (alignment) {
					case TextAlignment_top_left: position = rect.min; break;
					case TextAlignment_center: position = rect.min + (rect.size() - get_text_bounds(text, font, true).size()) / 2; break;
				}


				auto &cache = text_caches[id];

				bool rehash = true;

				if (cache.text == text) {
					rehash = false;
				}

				auto &vertices = cache.vertices;

				if (rehash) {
					free(cache.text);
					cache.text = copy(text);

					List<PlacedChar> chars = with(temporary_allocator, place_text(text, font, true));

					vertices.clear();
					for (auto c : chars) {
						vertices += {
							{{c.position.min.x, c.position.min.y}, {c.uv.min.x, c.uv.min.y}},
							{{c.position.max.x, c.position.min.y}, {c.uv.max.x, c.uv.min.y}},
							{{c.position.max.x, c.position.max.y}, {c.uv.max.x, c.uv.max.y}},
							{{c.position.min.x, c.position.max.y}, {c.uv.min.x, c.uv.max.y}},
						};
					}

					if (!cache.buffer) {
						glGenBuffers(1, &cache.buffer);
					}

					glBindBuffer(GL_ARRAY_BUFFER, cache.buffer);
					glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(vertices[0]), vertices.data, GL_STATIC_DRAW);
				} else {
					glBindBuffer(GL_ARRAY_BUFFER, cache.buffer);
				}

				glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(FontVertex), (void *)offsetof(FontVertex, position));
				glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(FontVertex), (void *)offsetof(FontVertex, uv));
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);

				defer {
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				};

				//font.vertex_buffer = vertices;

				glUseProgram(font_shader);
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_ZERO, GL_ONE);
				set_sampler(font_shader, "main_texture", (GLuint)font->texture_id, 0);

				v2f const shadow_offset = {1,1};

				glBindBuffer(GL_UNIFORM_BUFFER, font_uniforms);
				FontUniforms font_uniforms_data;

				font_uniforms_data.color = color * v4f{0,0,0,1};
				font_uniforms_data.position_offset = (v2f)position + shadow_offset;
				font_uniforms_data.position_scale = 2.0f / (v2f)window->client_size;
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(font_uniforms_data), &font_uniforms_data);
				glDrawArrays(GL_QUADS, 0, vertices.size);

				font_uniforms_data.color = color;
				font_uniforms_data.position_offset = (v2f)position;
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(font_uniforms_data), &font_uniforms_data);
				glDrawArrays(GL_QUADS, 0, vertices.size);

				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				break;
			}
		}
	}
}

v2s _get_text_bounds(Span<utf8> text, u32 font_size) {
	return get_text_bounds(text, get_font_at_size(font_collection, font_size), false).max;
}

}
}
