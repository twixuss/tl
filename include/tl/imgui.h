#pragma once
#include "math.h"
#include "window.h"
#include "font.h"

#ifndef TL_IMGUI_TEXTURE
#error TL_IMGUI_TEXTURE must be defined before including tl/imgui.h
#endif

#ifndef TL_IMGUI_SHADER
#error TL_IMGUI_SHADER must be defined before including tl/imgui.h
#endif

namespace TL {
namespace Imgui {

using Texture = typename TL_IMGUI_TEXTURE;
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

Window *window;
List<UILayer> layers;
UILayer *current_layer = 0;
Texture button_background_texture;

List<Region> region_stack;

void init_base(Window *window) {
	layers.allocator = current_allocator;
	tooltip.allocator = current_allocator;
	region_stack.allocator = current_allocator;
	::TL::Imgui::window = window;
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
	UIElement e;
	e.kind = UIElement_texture;
	e.texture.texture = texture;
	e.texture.color = color;
	e.texture.rect = region_to_client(rect);
	current_layer->elements.add(e);
}

struct Label {
	Span<utf8> text;
	aabb<v2s> rect;
	TextAlignment alignment;
	u32 font_size;
	v4f color = V4f(1);
};

void label(u32 id, Label l) {
	if (!l.text.size)
		return;

	assert(l.font_size);

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
void label(u32 id, Span<utf8> text, aabb<v2s> rect, TextAlignment alignment, u32 size, v4f color = V4f(1)) {
	Label l;
	l.text = text;
	l.rect = rect;
	l.alignment = alignment;
	l.font_size = size;
	l.color = color;
	label(id, l);
}

void panel(aabb<v2s> rect, v4f color) {
	if (!intersects(rect, current_region.visible_rect - current_region.visible_rect.min)) return;

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

Span<utf8> get_tooltip_stub() {
	return {};
}

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
	Span<utf8> (*get_tooltip)() = get_tooltip_stub;
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

void check_if_tooltip_was_set_properly() {
	if (should_set_tooltip) {
		// tooltip was not set
		should_set_tooltip = false;
		show_tooltip = false;
	}
}

ButtonState button(Button &b) {
	check_if_tooltip_was_set_properly();

	defer { ++b.id; };
	static u32 current_id = -1;

	if (b.text.size) {
		assert(b.font_size);
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

	if (background_texture) {
		draw_texture(background_rect, background_texture, background_color);
	} else {
		if (!foreground_texture) {
			panel(background_rect, background_color);
		}
	}

	if (state.flags & (ButtonState_hovered | ButtonState_pressed)) {
		if (button_background_texture) {
			draw_texture(background_rect, button_background_texture, V4f(V3f((state.flags & ButtonState_hovered) != 0), 0.1f));
		} else {
			panel(background_rect, V4f(V3f((state.flags & ButtonState_hovered) != 0), 0.1f));
		}
	}

	if (foreground_texture) {
		draw_texture(foreground_rect, foreground_texture, foreground_color);
	}

	if (text.size) {
		//begin_region(foreground_rect);
		label(id, text, foreground_rect, TextAlignment_top_left, font_size, foreground_color);
		//label(id, text, to_zero(foreground_rect), b.text_alignment, font_size, foreground_color);
		//end_region();
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
};

template <class T>
bool slider(Slider<T> &s) {
	auto id          = s.id++;
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

	//if (texture.handle) {
	//	draw_texture(rect,      texture, s.background_color);
	//	draw_texture(knob_rect, texture, s.foreground_color);
	//	if (state != UIElement_normal) {
	//		draw_texture(rect, texture, V4f(V3f(state == UIElement_hovered), 0.05f));
	//	}
	//} else {
		panel(rect, s.background_color);
		panel(knob_rect, s.foreground_color);
		if (state != UIElement_normal) {
			panel(rect, V4f(V3f(state == UIElement_hovered), 0.05f));
		}
	//}

	return holding;
}

Texture *default_slider_texture;

template <class T>
bool slider(u32 id, T &value, T lower_bound, T upper_bound, aabb<v2s> rect) {
	Slider<T> s;
	s.value       = &value;
	s.lower_bound = lower_bound;
	s.upper_bound = upper_bound;
	s.rect        = rect;
	s.texture     = *default_slider_texture;
	return slider(id, s);
}

using ScrollBarFlags = u8;
enum : ScrollBarFlags {
	ScrollBar_zoom_with_wheel = 0x1,
	ScrollBar_pan_with_wheel  = 0x2,
	ScrollBar_pan_with_mouse  = 0x4,
	ScrollBar_no_clamp        = 0x8,
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
};

template <class T>
bool scroll_bar(ScrollBar<T> &s) {
	auto id = s.id;
	auto &scroll_amount = *s.scroll_amount;
	auto &target_scroll_amount = *s.target_scroll_amount;
	auto &scale = *s.scale;
	auto total_size = s.total_size;
	auto rect = s.rect;

	auto max_visible_dim = max(current_region.visible_rect.size());
	s32 max_scroll = total_size - max_visible_dim;
	defer { scroll_amount = roundf(lerp((f32)scroll_amount, (f32)target_scroll_amount, 0.5f)); };

	static u32 panning_id = -1;
	static s32 panning_scroll_start;
	static s32 panning_mouse_start;

	if (in_bounds(window->mouse_position, current_region.visible_rect)) {
		if (window->mouse_wheel) {
			if ((s.flags & ScrollBar_zoom_with_wheel) && (s.flags & ScrollBar_pan_with_wheel)) {
				if (key_held(Key_shift)) {
					target_scroll_amount += window->mouse_wheel * max_visible_dim / 8;
				} else {
					auto old_scale = scale;
					scale /= powf(1.1f, window->mouse_wheel);
					scale = clamp(scale, s.min_scale, s.max_scale);
					target_scroll_amount = scroll_amount = lerp<f32>(window->mouse_position.x, target_scroll_amount, old_scale / scale);

					panning_mouse_start = window->mouse_position.x;
					panning_scroll_start = target_scroll_amount;
				}
			} else if (s.flags & ScrollBar_zoom_with_wheel) {
				target_scroll_amount += window->mouse_wheel * max_visible_dim / 8;
			} else if (s.flags & ScrollBar_pan_with_wheel) {
			}
		}
		if (key_down(Key_page_down)) {
			target_scroll_amount -= max_visible_dim;
		}
		if (key_down(Key_page_up)) {
			target_scroll_amount += max_visible_dim;
		}
		if (key_down(Key_home)) {
			target_scroll_amount = 0;
		}
		if (key_down(Key_end)) {
			target_scroll_amount = current_region.visible_rect.size().x - s.total_size; // flipped because target_scroll_amount is negative at the end
		}
		if (s.flags & ScrollBar_pan_with_mouse) {
			if (mouse_down(0)) {
				panning_id = id;
				panning_mouse_start = window->mouse_position.x;
				panning_scroll_start = target_scroll_amount;
			}
			if (panning_id == id) {
				if (mouse_held(0)) {
					target_scroll_amount = scroll_amount = panning_scroll_start + window->mouse_position.x - panning_mouse_start;
				}
			}
			if (mouse_up(0)) {
				panning_id = -1;
			}
		}
	}

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
	if (!hovering_interactive_element && in_bounds(window->mouse_position, current_region.visible_rect) && in_bounds(mouse_position, rect)) {
		hovering_interactive_element = true;
		if (mouse_down(0)) {
			current_id = id;
			set_offset = true;
		}
		if (current_id != id) {
			state = UIElement_hovered;
		}
	}
	if (current_id == id) {
		state = UIElement_pressed;
	}

	v2s knob_position;
	v2s knob_size;
	auto size = rect.size();
	bool holding = false;

	if (size.y > size.x) {
		knob_size = {size.x, (s32)((f32)pow2(max_visible_dim) / total_size)};

		knob_position = {
			rect.min.x,
			(s32)map_clamped<T>(scroll_amount, -max_scroll, 0, rect.min.y + size.y - knob_size.y, rect.min.y),
		};

		if (set_offset) {
			pick_offset = mouse_position.y - knob_position.y;
			if (pick_offset < 0 || pick_offset >= knob_size.y) {
				pick_offset = knob_size.y / 2;
			}
		}

		mouse_position.y -= pick_offset;

		s32 thickness = knob_size.y;
		if (current_id == id) {
			scroll_amount = map_clamped<T>(mouse_position.y, rect.min.y, rect.min.y + size.y - thickness, 0, -max_scroll);
			holding = true;
			if (mouse_up(0)) {
				current_id = -1;
				pick_offset = 0;
			}
		}
	} else {
		knob_size = {(s32)((f32)pow2(max_visible_dim) / total_size), size.y};

		knob_position = {
			(s32)map_clamped<T>(scroll_amount, -max_scroll, 0, rect.min.x + size.x - knob_size.x, rect.min.x),
			rect.min.y,
		};

		if (set_offset) {
			pick_offset = mouse_position.x - knob_position.x;
			if (pick_offset < 0 || pick_offset >= knob_size.x) {
				pick_offset = knob_size.x / 2;
			}
		}

		mouse_position.x -= pick_offset;

		s32 thickness = knob_size.x;
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

	if (!intersects(rect, current_region.visible_rect - current_region.visible_rect.min)) return false;

	if (default_slider_texture) {
		draw_texture(rect, *default_slider_texture, s.background_color);
		draw_texture(aabb_min_size(knob_position, knob_size), *default_slider_texture, s.foreground_color);
		if (state != UIElement_normal) {
			draw_texture(rect, *default_slider_texture, V4f(V3f(state == UIElement_hovered), 0.05f));
		}
	} else {
		panel(rect, s.background_color);
		panel(aabb_min_size(knob_position, knob_size), s.foreground_color);
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

bool frame_has_begun;
void begin_frame_base() {
	assert(!frame_has_begun);
	frame_has_begun = true;
	current_scissor = current_region.rect = current_region.visible_rect = aabb_min_max({}, (v2s)window->client_size);
	hovering_interactive_element = false;
}
void end_frame_base() {
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

		auto bounds = _get_text_bounds(tooltip, tooltip_font_size);

		s32 const margin = 4;
		auto panel_rect = clamp(extend(aabb_min_max({}, bounds), V2s(margin)) + window->mouse_position + v2s{-32, -24}, aabb_min_max(v2s{}, (v2s)window->client_size));

		v4f background_color = {.2,.3,.1,1.};
		panel(panel_rect, V4f(background_color.xyz, tooltip_opacity * 0.5f));
		label(-2, tooltip, panel_rect, TextAlignment_center, tooltip_font_size, V4f(1,1,1,tooltip_opacity));
		draw_and_free_layer(tooltip_layer);
	}
	tooltip_opacity_t = move_toward(tooltip_opacity_t, (f32)(show_tooltip && tooltip.size), 1 / 60.0f);
	tooltip_opacity = smoothstep(clamp(map(tooltip_opacity_t, 0.0f, 1.0f, -3.0f, 1.0f), 0.0f, 1.0f));

	show_tooltip = false;
}

}
}
