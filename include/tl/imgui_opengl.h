#pragma once
#define TL_IMGUI_TEXTURE_HANDLE umm
#define TL_IMGUI_SHADER_HANDLE ShaderCatalog::Entry *
#include "shader_catalog_opengl.h"
#include "imgui.h"
#include "font.h"
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

TL_IMGUI_SHADER_HANDLE just_color_shader;
TL_IMGUI_SHADER_HANDLE just_color_batch_shader;
TL_IMGUI_SHADER_HANDLE texture_shader;
TL_IMGUI_SHADER_HANDLE font_shader;

FontCollection *font_collection;

std::unordered_map<u32, TextCache> text_caches;

void init(Window *window, FontCollection *font_collection) {
	init_base(window);
	::TL::Imgui::font_collection = font_collection;
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

	use_shader(*just_color_batch_shader);
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

				use_shader(*font_shader);
				glBindTexture(GL_TEXTURE_2D, (GLuint)font->texture_id);

				v2f const shadow_offset = {1,1};

				set_uniform(*font_shader, "color", color * v4f{0,0,0,1});
				set_uniform(*font_shader, "position_offset", (v2f)position + shadow_offset);
				set_uniform(*font_shader, "position_scale", 2.0f / (v2f)window->client_size);
				glDrawArrays(GL_QUADS, 0, vertices.size);

				set_uniform(*font_shader, "color", color);
				set_uniform(*font_shader, "position_offset", (v2f)position);
				set_uniform(*font_shader, "position_scale", 2.0f / (v2f)window->client_size);
				glDrawArrays(GL_QUADS, 0, vertices.size);
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
