#pragma once
#include "math.h"
#include "window.h"
#include "font.h"
#include "time.h"
#include "texture.h"

#ifndef TL_IMGUI_TEXTURE
#error TL_IMGUI_TEXTURE must be defined before including tl/imgui.h
#endif

#ifndef TL_IMGUI_SHADER
#error TL_IMGUI_SHADER must be defined before including tl/imgui.h
#endif

namespace TL {
namespace Imgui {

struct Texture {
	TL_IMGUI_TEXTURE handle = {};
	v2s stretch_min = {};
	v2s stretch_max = {};
	v2s size = {};
};

using Shader = typename TL_IMGUI_SHADER;

struct Region {
	aabb<v2s> rect;
	aabb<v2s> visible_rect;
};

Region current_region;
List<utf8> tooltip;
bool show_tooltip;
f32 tooltip_opacity;
f32 tooltip_opacity_t;
f32 frame_time = 1.0f / 60.0f;
PreciseTimer frame_timer;
Texture tooltip_texture;
Texture texture_round_4;
Texture texture_round_4_top;
Texture texture_round_4_bottom;
Texture texture_round_8;

enum UIElementKind {
	UIElement_unknown,
	UIElement_texture,
	UIElement_text,
	UIElement_panel,
};

enum TextAlignment {
	TextAlignment_top_left,
	TextAlignment_center,
};

struct UIElement {
	UIElementKind kind;
	UIElement() {
		memset(this, 0, sizeof(*this));
		scissor_rect = current_region.visible_rect;
	}
	aabb<v2s> scissor_rect;
	union {
		struct {
			aabb<v2s> rect;
			v4f color;
			Texture texture;
		} texture;

		struct {
			u32 id;
			aabb<v2s> rect;
			TextAlignment alignment;
			List<utf8> text;
			u32 font_size;
			v4f color;
		} text;

		struct {
			aabb<v2s> rect;
			v4f color;
		} panel;
	};
};

struct UILayer {
	List<UIElement> elements;
};

extern void _set_scissor_impl(aabb<v2s> region);
extern void _draw_and_free_elements(Span<UIElement> e);
extern v2s _get_text_bounds(Span<utf8> text, u32 font_size);
extern TL_IMGUI_TEXTURE _create_texture(void *pixels, v2u size, TextureFormat format);
extern void _ensure_all_chars_present(Span<utf8> text, u32 font_size);

Window *window;
List<UILayer> layers;
UILayer *current_layer = 0;

List<Region> region_stack;

void init_base(Window *window) {
	timed_function();

	layers.allocator = current_allocator;
	tooltip.allocator = current_allocator;
	region_stack.allocator = current_allocator;
	::TL::Imgui::window = window;

	{

		u32 round_4_pixels[8*8];
		for (s32 y = 0; y < 8; ++y)
		for (s32 x = 0; x < 8; ++x)
		{
			v2f p = V2f(x, y) - 4 + 0.5f;
			round_4_pixels[y*8+x] = ((u32)(clamp<f32>(4 + 0.5f - length(p), 0, 1) * 255) << 24) | 0xffffff;
		}

		texture_round_4.handle = _create_texture(round_4_pixels, {8, 8}, TextureFormat_u8_rgba);
		texture_round_4.stretch_min = V2s(4);
		texture_round_4.stretch_max = V2s(4);
		texture_round_4.size = V2s(8);

		texture_round_4_top.handle = _create_texture(round_4_pixels, {8, 4}, TextureFormat_u8_rgba);
		texture_round_4_top.stretch_min = {4, 4};
		texture_round_4_top.stretch_max = {4, 0};
		texture_round_4_top.size = {8, 4};

		texture_round_4_bottom.handle = _create_texture(round_4_pixels+8*4, {8, 4}, TextureFormat_u8_rgba);
		texture_round_4_bottom.stretch_min = {4, 0};
		texture_round_4_bottom.stretch_max = {4, 4};
		texture_round_4_bottom.size = {8, 4};
	}

	{
		u32 round_8_pixels[16*16];
		for (s32 y = 0; y < 16; ++y)
		for (s32 x = 0; x < 16; ++x)
		{
			v2f p = V2f(x, y) - 8 + 0.5f;
			round_8_pixels[y*16+x] = ((u32)(clamp<f32>(8 + 0.5f - length(p), 0, 1) * 255) << 24) | 0xffffff;
		}

		texture_round_8.handle = _create_texture(round_8_pixels, {16, 16}, TextureFormat_u8_rgba);
		texture_round_8.stretch_min = V2s(8);
		texture_round_8.stretch_max = V2s(8);
		texture_round_8.size = V2s(16);
	}

	tooltip_texture = texture_round_4;


	frame_timer = create_precise_timer();
}

void begin_region(aabb<v2s> new_region) {
	region_stack.add(current_region);

	new_region += current_region.rect.min;
	current_region.rect = new_region;
	current_region.visible_rect = Union(current_region.visible_rect, new_region);
}
void end_region() {
	current_region = region_stack.pop();
}


aabb<v2s> current_scissor;

void set_scissor(aabb<v2s> region) {
	current_scissor = region;
	_set_scissor_impl(region);
}

v2f client_to_ndc(v2f p) {
	return map(p, v2f{}, (v2f)window->client_size, v2f{-1,1}, v2f{1,-1});
}
v2f client_to_ndc(v2u p) { return client_to_ndc((v2f)p); }
v2f client_to_ndc(v2s p) { return client_to_ndc((v2f)p); }
aabb<v2f> client_to_ndc(aabb<v2s> p) { return aabb_min_max(client_to_ndc((v2f)p.min), client_to_ndc((v2f)p.max)); }

v2f region_to_client(v2f p) { return p + (v2f)current_region.rect.min; }
v2s region_to_client(v2s p) { return p + current_region.rect.min; }
aabb<v2s> region_to_client(aabb<v2s> p) { return {region_to_client(p.min), region_to_client(p.max)}; }

void free(UIElement &elem) {
	switch (elem.kind) {
		case UIElement_text:
			free(elem.text.text);
			break;
	}
}

void begin_base() {
	current_layer = &layers.add();
}

void end_base() {
}

bool hovering_interactive_element;

bool should_set_tooltip;
void set_tooltip(Span<utf8> new_tooltip) {
	tooltip.set(new_tooltip);
	should_set_tooltip = false;
}

void draw_and_free_layer(UILayer &layer) {
	_draw_and_free_elements(layer.elements);
	free(layer.elements);
}

void draw_texture(aabb<v2s> rect, Texture texture, v4f color = V4f(1)) {
	if (!has_volume(current_region.visible_rect)) return;

	UIElement e;
	e.kind = UIElement_texture;
	e.texture.texture = texture;
	e.texture.color = color;
	e.texture.rect = region_to_client(rect);
	if (texture.stretch_max != v2s{} || texture.stretch_min != v2s{}) {
		assert(texture.size != v2s{});
	}
	current_layer->elements.add(e);
}

struct Label {
	Span<utf8> text;
	aabb<v2s> rect;
	TextAlignment alignment;
	u32 font_size;
	v4f color = V4f(1);
};

void label(u32 id, Label l, bool check_present = true) {
	if (!l.text.size) return;
	if (!has_volume(current_region.visible_rect)) return;

	assert(l.font_size);

	if (check_present)
		_ensure_all_chars_present(l.text, l.font_size);

	UIElement e;
	e.kind = UIElement_text;
	e.text.text.allocator = temporary_allocator;
	e.text.text.set(l.text);
	e.text.rect      = region_to_client(l.rect);
	e.text.alignment = l.alignment;
	e.text.font_size = l.font_size;
	e.text.color     = l.color;
	e.text.id        = id;
	current_layer->elements.add(e);
}
void label(u32 id, Span<utf8> text, aabb<v2s> rect, TextAlignment alignment, u32 size, v4f color = V4f(1), bool check_present = true) {
	Label l;
	l.text = text;
	l.rect = rect;
	l.alignment = alignment;
	l.font_size = size;
	l.color = color;
	label(id, l, check_present);
}

void panel(aabb<v2s> rect, v4f color) {
	if (!intersects(rect, current_region.visible_rect - current_region.visible_rect.min)) return;
	if (!has_volume(current_region.visible_rect)) return;

	UIElement e;
	e.kind = UIElement_panel;
	e.panel.color = color;
	e.panel.rect = region_to_client(rect);
	current_layer->elements.add(e);
}

enum UIElementState {
	UIElement_normal,
	UIElement_hovered,
	UIElement_pressed,
};

struct Button {
	u32 id = {};
	aabb<v2s> rect = {};
	Span<utf8> text = {};
	TextAlignment text_alignment = {};
	u32 font_size = {};
	v4f foreground_color = {1,1,1,1};
	v4f background_color = {};
	Texture background_texture = {};
	Texture foreground_texture = {};
	u32 content_padding = {};
};

using ButtonStateFlags = u8;
enum : u8 {
	ButtonState_hovered = 0x1,
	ButtonState_pressed = 0x2,
	ButtonState_clicked = 0x4,
};

struct ButtonState {
	ButtonStateFlags flags;
	u8 mouse_button;
	operator bool() {
		return flags & ButtonState_clicked;
	}
};

void check_if_tooltip_was_set() {
	if (should_set_tooltip) {
		// tooltip was not set
		should_set_tooltip = false;
		show_tooltip = false;
	}
}

ButtonState button(Button &b) {
	check_if_tooltip_was_set();

	defer { ++b.id; };
	static u32 current_id = -1;

	if (b.text.size) {
		assert(b.font_size);
		_ensure_all_chars_present(b.text, b.font_size);
	}

	auto id                 = b.id;
	auto foreground_color   = b.foreground_color;
	auto background_color   = b.background_color;
	auto text               = b.text;
	auto foreground_texture = b.foreground_texture;
	auto background_texture = b.background_texture;
	auto font_size          = b.font_size;
	auto content_padding    = b.content_padding;

	if (b.rect.size() == v2s{}) {
		assert(b.text.size);
		b.rect = aabb_min_size(b.rect.min, _get_text_bounds(b.text, b.font_size));
	}

	auto foreground_rect = b.rect;
	foreground_rect += V2s(content_padding);

	auto &background_rect = b.rect;
	background_rect.max += content_padding * 2;

	if (!intersects(background_rect, current_region.visible_rect - current_region.visible_rect.min)) return {};

	auto mouse_position = window->mouse_position;

	static u8 start_button;
	ButtonState state = {};
	if (!hovering_interactive_element && in_bounds(window->mouse_position, current_region.visible_rect) && in_bounds(mouse_position - current_region.visible_rect.min, background_rect)) {
		hovering_interactive_element = true;
		show_tooltip = true;
		should_set_tooltip = true;
		if (current_id == id) {
			if (mouse_up(start_button)) {
				state.mouse_button = start_button;
				state.flags |= ButtonState_clicked;
			}
		} else {
			if (mouse_down(0)) {
				current_id = id;
				start_button = 0;
			}
			if (mouse_down(1)) {
				current_id = id;
				start_button = 1;
			}
			state.flags |= ButtonState_hovered;
		}
	} else {
		should_set_tooltip = false;
	}
	if (current_id == id) {
		state.flags |= ButtonState_pressed;
		if (mouse_up(start_button)) {
			current_id = -1;
			start_button = 0;
		}
	}

	if (TL_IMGUI_TEXTURE_GET(background_texture.handle)) {
		draw_texture(background_rect, background_texture, background_color);
	} else {
		if (!TL_IMGUI_TEXTURE_GET(foreground_texture.handle)) {
			panel(background_rect, background_color);
		}
	}

	if (state.flags & (ButtonState_hovered | ButtonState_pressed)) {
		if (TL_IMGUI_TEXTURE_GET(background_texture.handle)) {
			draw_texture(background_rect, background_texture, V4f(V3f((state.flags & ButtonState_hovered) != 0), 0.1f));
		} else {
			panel(background_rect, V4f(V3f((state.flags & ButtonState_hovered) != 0), 0.1f));
		}
	}

	if (TL_IMGUI_TEXTURE_GET(foreground_texture.handle)) {
		draw_texture(foreground_rect, foreground_texture, foreground_color);
	}

	if (text.size) {
		begin_region(foreground_rect);
		//label(id, text, foreground_rect, TextAlignment_top_left, font_size, foreground_color);
		label(id, text, to_zero(foreground_rect), b.text_alignment, font_size, foreground_color, false);
		end_region();
	}

	return state;
}

template <class T>
T get_hovered_slider_value(aabb<v2s> rect, T lower_bound, T upper_bound) {
	auto size = rect.size();
	auto mouse_position = window->mouse_position - current_region.visible_rect.min;
	if (size.y > size.x) {
		s32 thickness = size.x;
		return map_clamped((T)mouse_position.y, (T)(rect.min.y + thickness/2), (T)(rect.min.y + size.y - thickness/2), upper_bound, lower_bound);
	} else {
		s32 thickness = size.y;
		return map_clamped((T)mouse_position.x, (T)(rect.min.x + thickness/2), (T)(rect.min.x + size.x - thickness/2), lower_bound, upper_bound);
	}
}

template <class T>
struct Slider {
	u32 id;
	T *value;
	T lower_bound;
	T upper_bound;
	aabb<v2s> rect;
	Texture texture;
	v4f background_color = {.1,.1,.1,1};
	v4f foreground_color = {.2,.2,.2,1};
	s32 knob_padding = 0;
};

template <class T>
bool slider(Slider<T> &s) {
	check_if_tooltip_was_set();

	defer { ++s.id; };
	auto id          = s.id;
	auto value       = s.value;
	auto lower_bound = s.lower_bound;
	auto upper_bound = s.upper_bound;
	auto rect        = s.rect;
	auto texture     = s.texture;

	static u32 current_id = -1;

	auto mouse_position = window->mouse_position - current_region.visible_rect.min;
	UIElementState state = UIElement_normal;
	bool clicked = false;
	if (!hovering_interactive_element && in_bounds(window->mouse_position, current_region.visible_rect) && in_bounds(mouse_position, rect)) {
		hovering_interactive_element = true;
		show_tooltip = true;
		should_set_tooltip = true;
		if (mouse_down(0)) {
			current_id = id;
		}
		if (current_id != id) {
			state = UIElement_hovered;
		}
	} else {
		should_set_tooltip = false;
	}
	if (current_id == id) {
		state = UIElement_pressed;
	}
	aabb<v2s> knob_rect;
	auto size = rect.size();
	bool holding = false;

	if (size.y > size.x) {
		s32 thickness = size.x;
		knob_rect.min = {
			rect.min.x,
			(s32)map_clamped(*value, lower_bound, upper_bound, (T)(rect.min.y + size.y - thickness), (T)rect.min.y),
		};
		knob_rect.max = knob_rect.min + V2s(thickness);
	} else {
		s32 thickness = size.y;
		knob_rect.min = {
			(s32)map_clamped(*value, lower_bound, upper_bound, (T)rect.min.x, (T)(rect.min.x + size.x - thickness)),
			rect.min.y
		};
		knob_rect.max = knob_rect.min + V2s(thickness);
	}
	if (current_id == id) {
		*value = get_hovered_slider_value(rect, lower_bound, upper_bound);
		holding = true;
		if (mouse_up(0)) {
			current_id = -1;
		}
	}

	if (!intersects(rect, current_region.visible_rect - current_region.visible_rect.min)) return false;

	knob_rect = extend(knob_rect, V2s(-s.knob_padding));

	if (TL_IMGUI_TEXTURE_GET(texture.handle)) {
		draw_texture(rect,      texture, s.background_color);
		draw_texture(knob_rect, texture, s.foreground_color);
		if (state != UIElement_normal) {
			draw_texture(rect, texture, V4f(V3f(state == UIElement_hovered), 0.05f));
		}
	} else {
		panel(rect, s.background_color);
		panel(knob_rect, s.foreground_color);
		if (state != UIElement_normal) {
			panel(rect, V4f(V3f(state == UIElement_hovered), 0.05f));
		}
	}

	return holding;
}

using ScrollBarFlags = u8;
enum : ScrollBarFlags {
	ScrollBar_zoom_with_wheel   = 0x1,
	ScrollBar_no_pan_with_wheel = 0x2,
	ScrollBar_pan_with_mouse    = 0x4,
	ScrollBar_no_clamp          = 0x8,
};

template <class T>
struct ScrollBar {
	u32 id = {};
	T *scroll_amount = {};
	T *target_scroll_amount = {};
	T total_size = {};
	aabb<v2s> rect = {};
	ScrollBarFlags flags = {};
	T *scale = {};
	T min_scale = {};
	T max_scale = {};
	v4f background_color = {.1,.1,.1,1};
	v4f foreground_color = {.2,.2,.2,1};
	s32 knob_padding = 0;
	Texture texture;
};

template <class T>
aabb<v2s> get_knob_rect(ScrollBar<T> const &s) {
	auto rect = s.rect;
	auto size = rect.size();
	auto total_size = (s.flags & ScrollBar_zoom_with_wheel) ? (s.total_size / *s.scale) : s.total_size;
	auto scroll_amount = *s.scroll_amount;

	s32 visible_size = current_region.visible_rect.size().s[size.y > size.x];
	s32 max_scroll = total_size - visible_size;


	v2s knob_position;
	v2s knob_size;
	if (size.y > size.x) {
		knob_size = {size.x, (s32)((f32)pow2(visible_size) / total_size)};

		knob_position = {
			rect.min.x,
			(s32)map_clamped<T>(scroll_amount, -max_scroll, 0, rect.min.y + size.y - knob_size.y, rect.min.y),
		};
	} else {
		knob_size = {(s32)((f32)pow2(visible_size) / total_size), size.y};

		knob_position = {
			(s32)map_clamped<T>(scroll_amount, -max_scroll, 0, rect.min.x + size.x - knob_size.x, rect.min.x),
			rect.min.y,
		};
	}
	return aabb_min_size(knob_position, knob_size);
}

template <class T>
bool scroll_bar(ScrollBar<T> &s) {
	check_if_tooltip_was_set();

	auto id = s.id;
	auto &scroll_amount = *s.scroll_amount;
	auto &target_scroll_amount = *s.target_scroll_amount;
	auto &scale = *s.scale;
	auto total_size = (s.flags & ScrollBar_zoom_with_wheel) ? (s.total_size / *s.scale) : s.total_size;
	auto rect = s.rect;

	auto size = rect.size();

	s32 visible_size = current_region.visible_rect.size().s[size.y > size.x];

	s32 max_scroll = total_size - visible_size;


	if (max_scroll <= 0) {
		rect = {};
	}

	static u32 panning_id = -1;
	static T panning_scroll_start;
	static s32 panning_mouse_start;

	if (!(s.flags & ScrollBar_no_clamp) && max_scroll <= 0) {
		target_scroll_amount = 0;
		scroll_amount = 0;
		return false;
	}


	static u32 current_id = -1;
	static s32 pick_offset = 0;

	auto mouse_position = window->mouse_position - current_region.visible_rect.min;

	bool clicked = false;
	bool set_offset = false;
	UIElementState state = UIElement_normal;
	if (in_bounds(window->mouse_position, current_region.visible_rect)) {
		if (window->mouse_wheel) {
			if ((s.flags & ScrollBar_zoom_with_wheel) && !(s.flags & ScrollBar_no_pan_with_wheel)) {
				if (key_held(Key_shift)) {
					target_scroll_amount += window->mouse_wheel * visible_size / 8;
				} else {
					auto old_scale = scale;
					scale /= powf(1.1f, window->mouse_wheel);
					scale = clamp(scale, s.min_scale, s.max_scale);
					target_scroll_amount = scroll_amount = lerp<f32>(mouse_position.x, target_scroll_amount, old_scale / scale);

					panning_mouse_start = mouse_position.x;
					panning_scroll_start = target_scroll_amount;
				}
			} else if (s.flags & ScrollBar_zoom_with_wheel) {
			} else if (!(s.flags & ScrollBar_no_pan_with_wheel)) {
				target_scroll_amount += window->mouse_wheel * visible_size / 8;
			}
		}
		if (key_down(Key_page_down)) {
			target_scroll_amount -= visible_size;
		}
		if (key_down(Key_page_up)) {
			target_scroll_amount += visible_size;
		}
		if (key_down(Key_home)) {
			target_scroll_amount = 0;
		}
		if (key_down(Key_end)) {
			target_scroll_amount = current_region.visible_rect.size().x - s.total_size;
		}

		if (in_bounds(mouse_position, rect)) {
			if (!hovering_interactive_element) {
				hovering_interactive_element = true;
				if (mouse_down(0)) {
					current_id = id;
					set_offset = true;
				}
				if (current_id != id) {
					state = UIElement_hovered;
				}
			}
		} else {
			if (s.flags & ScrollBar_pan_with_mouse) {
				if (mouse_down(0)) {
					panning_id = id;
					panning_mouse_start = mouse_position.x;
					panning_scroll_start = target_scroll_amount;
				}
				if (panning_id == id) {
					if (mouse_held(0)) {
						target_scroll_amount = scroll_amount = panning_scroll_start + mouse_position.x - panning_mouse_start;
					}
				}
				if (mouse_up(0)) {
					panning_id = -1;
				}
			}
		}
	}
	if (current_id == id) {
		state = UIElement_pressed;
	}

	auto knob_rect = get_knob_rect(s);
	bool holding = false;

	if (size.y > size.x) {
		if (set_offset) {
			pick_offset = mouse_position.y - knob_rect.min.y;
			if (pick_offset < 0 || pick_offset >= knob_rect.size().y) {
				pick_offset = knob_rect.size().y / 2;
			}
		}

		mouse_position.y -= pick_offset;

		s32 thickness = knob_rect.size().y;
		if (current_id == id) {
			scroll_amount = map_clamped<T>(mouse_position.y, rect.min.y, rect.min.y + size.y - thickness, 0, -max_scroll);
			holding = true;
			if (mouse_up(0)) {
				current_id = -1;
				pick_offset = 0;
			}
		}
	} else {
		if (set_offset) {
			pick_offset = mouse_position.x - knob_rect.min.x;
			if (pick_offset < 0 || pick_offset >= knob_rect.size().x) {
				pick_offset = knob_rect.size().x / 2;
			}
		}

		mouse_position.x -= pick_offset;

		s32 thickness = knob_rect.size().x;
		if (current_id == id) {
			scroll_amount = map_clamped<T>(mouse_position.x, rect.min.x, rect.min.x + size.x - thickness, 0, -max_scroll);
			holding = true;
			if (mouse_up(0)) {
				current_id = -1;
				pick_offset = 0;
			}
		}
	}

	if (!(s.flags & ScrollBar_no_clamp)) {
		target_scroll_amount = clamp<T>(target_scroll_amount, -max_scroll, 0);
		scroll_amount = clamp<T>(scroll_amount, -max_scroll, 0);
	}

	if (holding) {
		target_scroll_amount = scroll_amount;
	}

	if constexpr (is_integer<T>) {
		f32 f = lerp<f32>(scroll_amount, target_scroll_amount, frame_time * 30);
		if (target_scroll_amount > scroll_amount) {
			scroll_amount = ceil(f);
		} else {
			scroll_amount = floor(f);
		}
	} else {
		scroll_amount = lerp<T>(scroll_amount, target_scroll_amount, frame_time * 30);
	}

	if (!intersects(rect, current_region.visible_rect - current_region.visible_rect.min)) return false;

	knob_rect = extend(get_knob_rect(s), V2s(-s.knob_padding));

	if (TL_IMGUI_TEXTURE_GET(s.texture.handle)) {
		draw_texture(rect, s.texture, s.background_color);
		draw_texture(knob_rect, s.texture, s.foreground_color);
		if (state != UIElement_normal) {
			draw_texture(rect, s.texture, V4f(V3f(state == UIElement_hovered), 0.05f));
		}
	} else {
		panel(rect, s.background_color);
		panel(knob_rect, s.foreground_color);
		if (state != UIElement_normal) {
			panel(rect, V4f(V3f(state == UIElement_hovered), 0.05f));
		}
	}

	return holding;
}

using Anchor = v2f;

inline static constexpr Anchor anchor_top_left      = {0.0f, 0.0f};
inline static constexpr Anchor anchor_top_right     = {1.0f, 0.0f};
inline static constexpr Anchor anchor_bottom_left   = {0.0f, 1.0f};
inline static constexpr Anchor anchor_bottom_right  = {1.0f, 1.0f};
inline static constexpr Anchor anchor_center        = {0.5f, 0.5f};
inline static constexpr Anchor anchor_top_center    = {0.5f, 0.0f};
inline static constexpr Anchor anchor_bottom_center = {0.5f, 1.0f};
inline static constexpr Anchor anchor_center_left   = {0.0f, 0.5f};
inline static constexpr Anchor anchor_center_right  = {1.0f, 0.5f};

enum Dock {
	Dock_top,
	Dock_bottom,
	Dock_right,
	Dock_left,
};

// Returns a rectancle relative to current_region
aabb<v2s> get_rect(v2s pos, v2s size, Anchor anchor) {
	return aabb_min_size((v2s)lerp((v2f)pos, (v2f)(current_region.rect.size() - pos - size), anchor), size);
}

aabb<v2s> get_dock(s32 size, Dock dock) {
	switch (dock) {
		case Dock_top: return aabb_min_max(v2s{}, {current_region.visible_rect.size().x, size});
		case Dock_bottom: return aabb_min_max({0, current_region.visible_rect.size().y - size}, current_region.visible_rect.size());
		case Dock_right: return aabb_min_max({current_region.visible_rect.size().x - size, 0}, current_region.visible_rect.size());
		case Dock_left: return aabb_min_max(v2s{}, {size, current_region.visible_rect.size().y});
		default: invalid_code_path("bad Dock");
	}
}

aabb<v2s> get_other_dock(s32 size, Dock dock) {
	switch (dock) {
		case Dock_top: return aabb_min_max(v2s{0, size}, current_region.visible_rect.size());
		case Dock_bottom: return aabb_min_max(v2s{}, current_region.visible_rect.size() - v2s{0, size});
		case Dock_right: return aabb_min_max(v2s{}, current_region.visible_rect.size() - v2s{size, 0});
		case Dock_left: return aabb_min_max(v2s{size, 0}, current_region.visible_rect.size());
		default: invalid_code_path("bad Dock");
	}
}

Array<aabb<v2s>, 2> split(s32 size, Dock dock) {
	Array<aabb<v2s>, 2> result;
	result.data[0] = get_dock(size, dock);
	result.data[1] = get_other_dock(size, dock);
	return result;
}

bool frame_has_begun;
void begin_frame_base() {
	assert(!frame_has_begun);
	frame_has_begun = true;
	current_scissor = current_region.rect = current_region.visible_rect = aabb_min_max({}, (v2s)window->client_size);
	hovering_interactive_element = false;
}
void end_frame_base() {
	timed_function();

	assert(frame_has_begun);
	frame_has_begun = false;
	assert(region_stack.size == 0);

	for (auto &layer : reverse(layers)) {
		draw_and_free_layer(layer);
	}
	layers.clear();

	if (tooltip_opacity > 0) {
		UILayer tooltip_layer;
		current_layer = &tooltip_layer;
		auto tooltip_font_size = 16;

		_ensure_all_chars_present(tooltip, tooltip_font_size);
		auto bounds = _get_text_bounds(tooltip, tooltip_font_size);

		s32 const margin = 4;
		auto panel_rect = clamp(extend(aabb_min_max({}, bounds), V2s(margin)) + window->mouse_position + v2s{-32, -24}, aabb_min_max(v2s{}, (v2s)window->client_size));

		v4f background_color = {.2,.3,.1,1.};
		if (TL_IMGUI_TEXTURE_GET(tooltip_texture.handle)) {
			draw_texture(panel_rect, tooltip_texture, V4f(background_color.xyz, tooltip_opacity * 0.5f));
		} else {
			panel(panel_rect, V4f(background_color.xyz, tooltip_opacity * 0.5f));
		}
		label(-2, tooltip, panel_rect, TextAlignment_center, tooltip_font_size, V4f(1,1,1,tooltip_opacity));
		draw_and_free_layer(tooltip_layer);
	}
	tooltip_opacity_t = move_toward(tooltip_opacity_t, (f32)(show_tooltip && tooltip.size), frame_time);
	tooltip_opacity = smoothstep(clamp(map(tooltip_opacity_t, 0.0f, 1.0f, -3.0f, 1.0f), 0.0f, 1.0f));

	show_tooltip = false;

	frame_time = reset(frame_timer);
}

}
}
