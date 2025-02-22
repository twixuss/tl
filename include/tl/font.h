#pragma once

#include "common.h"
#include "list.h"
#include "buffer.h"
#include "file.h"
#include "hash_map.h"
#include "math.h"
#include "atlas.h"

#ifndef TL_FONT_TEXTURE_HANDLE
#define TL_FONT_TEXTURE_HANDLE void *
#endif

// Set TL_FONT_USE_SPECIAL_CODES to 1 to be able to use special 
// codes to modify the appearance of a single text string.
// 
// Start the code with TL_FONT_SPECIAL_CODE_PREFIX ('$' by default, so it will be used for the rest of this comment)
// 
// Escape the $ with $$
// 
// Color:
//             $fff
//             ||||
//     prefix -+|||
//         red -+||
//        green -+|
//          blue -+
//     
//     Each component is defined by a single hex digit (4 bits),
//     so the total number of representable colors is 16^3 = 4096
//
// TODO: add bold flag
// TODO: add italic flag
//
#ifndef TL_FONT_USE_SPECIAL_CODES
#define TL_FONT_USE_SPECIAL_CODES 0
#endif
#ifndef TL_FONT_SPECIAL_CODE_PREFIX
#define TL_FONT_SPECIAL_CODE_PREFIX '$'
#endif

namespace tl {

struct SizedFont {
	s32 line_spacing = 0;
	s32 ascender     = 0;
	s32 descender    = 0;
};

struct FontBase {
	ContiguousHashMap<u32, SizedFont> sized;
	Span<u8> buffer;
};
struct Font : FontBase {
	u8 impl_data[32];
};

struct FontChar {
	aabb<v2u> rect = {};
	v2s offset = {};
	v2s advance = {};
	u32 line_spacing = 0; // This is per sized font data, but it is needed where there is no access to SizedFont
};
using FontAtlas = Atlas<v3u8, FontChar>;

struct CodePointAndSize {
	utf32 code = 0;
	u32 size = 0;
	constexpr auto operator<=>(CodePointAndSize const &) const noexcept = default;
};

}

template<>
inline tl::u64 get_hash(tl::CodePointAndSize const &x) {
	return ((tl::u64)x.code << 32) | x.size;
}

namespace tl {

struct FontCollection {
	Allocator allocator = TL_GET_CURRENT(allocator);
	List<Font> fonts;
	
	FontAtlas atlas;
	// Code point + size -> index in atlas.areas
	ContiguousHashMap<CodePointAndSize, u32> chars;

	TL_FONT_TEXTURE_HANDLE texture = {};
	
	TL_FONT_TEXTURE_HANDLE (*resize_texture)(TL_FONT_TEXTURE_HANDLE texture, v2u size) = autocast noop;
	void (*update_texture)(TL_FONT_TEXTURE_HANDLE texture, v3u8 *data, v2u size, aabb<v2u> updated_region) = autocast noop;
	void (*free_texture)(TL_FONT_TEXTURE_HANDLE texture) = autocast noop;
};

TL_API FontCollection *create_font_collection();
TL_API void free(FontCollection *collection);

// buffer - view to font file contents. Must be valid until FontCollection is freed.
TL_API void add_font(FontCollection *collection, Span<u8> buffer);

struct PlaceTextParams {
	f32 line_alignment = 0; // 0 = left-aligned; 0.5 = centered; 1 = right-aligned
	s32 wrap_width = max_value<s32>;
	u32 font_size = 14;
	#if TL_FONT_USE_SPECIAL_CODES
	v3f default_color = {1,1,1};
	bool use_codes = true;
	#endif
};
struct TL_API TextPlacer {
	struct Quad {
		aabb<v2s> position = {};
		aabb<v2u> uv = {};
		#if TL_FONT_USE_SPECIAL_CODES
		union Color {
			struct {
				u16 r : 4;
				u16 g : 4;
				u16 b : 4;
			};
			u16 flat;

			Color &operator=(v3f c) {
				auto i = floor_to_int(c * 15.999999f);
				r = i.x;
				g = i.y;
				b = i.z;
				return *this;
			}
			operator v3f() const { return V3f(r,g,b) / 15.0f; }
		};
		Color color;
		#endif
	};
	struct Line {
		// indices for `quads`
		umm begin_index = 0;
		umm end_index = 0;
		s32 width = 0;
	};
	struct CursorPosition {
		u32 x = 0;
		u32 line = 0;
	};

	// Quads for rendering.
	// Whitespace and special codes don't produce quads.
	List<Quad> quads;

	List<Line> lines;

	// Where cursor should be rendered.
	// Just index by cursor in your input field.
	List<CursorPosition> cursor_positions;

	aabb<v2s> bounds = {};
	u32 line_count = 0;
	
	// Calling place a second time will clear previous data and reuse memory.
	void place(FontCollection *collection, Span<utf8> text, PlaceTextParams params = {} TL_LP);
	void place(FontCollection *collection, Span<utf32> text, PlaceTextParams params = {} TL_LP);

	inline Optional<umm> line_of(umm char_index) {
		for (umm i = 0; i < lines.count; ++i) {
			auto line = lines[i];
			if ((char_index - line.begin_index) < (line.end_index - line.begin_index))
				return i;
		}
		return {};
	}
};

inline void free(TextPlacer &placer) {
	free(placer.quads);
	free(placer.lines);
}

TL_API void ensure_all_chars_present(FontCollection *collection, Span<utf8> text);
TL_API void ensure_all_chars_present(FontCollection *collection, Span<utf32> text);

TL_API void render_glyph(FontCollection *collection, Font &font_, u32 font_size, SizedFont &sized_font, utf32 new_char_code_point);

struct Utf8Reader {
	utf8 *cursor = 0;
	utf8 *end = 0;

	utf32 next() {
		if (cursor >= end)
			return 0;
		return decode_and_advance(&cursor).value_or(0);
	}
	utf8 *checkpoint() { return cursor; }
	void reset(utf8 *checkpoint) { cursor = checkpoint; }
	bool empty() { return cursor >= end; }
};

struct Utf32Reader {
	utf32 *cursor = 0;
	utf32 *end = 0;

	utf32 next() {
		if (cursor >= end)
			return 0;
		return *cursor++;
	}
	utf32 *checkpoint() { return cursor; }
	void reset(utf32 *checkpoint) { cursor = checkpoint; }
	bool empty() { return cursor >= end; }
};

}

#ifdef TL_IMPL

#ifdef FREETYPE_H_

namespace tl {

struct FontFT : FontBase {
	FT_Face face;
	u32 face_size;
};
static_assert(sizeof(FontFT) <= sizeof(Font), "increase impl_data size");

struct FontCollectionFT : FontCollection {
	FT_Library ft_library;
};

FontCollection *create_font_collection() {
	auto allocator = TL_GET_CURRENT(allocator);
	auto result = allocator.allocate<FontCollectionFT>();
	result->allocator = allocator;

	result->atlas.user_data = result;
	result->atlas.on_modify = [](FontAtlas *atlas, void *user_data, aabb<v2u> modified_area) {
		auto result = (FontCollection *)user_data;

		result->update_texture(result->texture, atlas->data, atlas->size, modified_area);
	};
	result->atlas.on_resize = [](FontAtlas *atlas, void *user_data, v2u new_size) {
		auto result = (FontCollection *)user_data;

		result->texture = result->resize_texture(result->texture, new_size);
	};

	auto error = FT_Init_FreeType(&result->ft_library);
	assert(!error, "FT_Init_FreeType Error {}", FT_Error_String(error));
	return result;
}

void free(FontCollection *collection) {
	for (auto &_font : collection->fonts) {
		auto font = (FontFT *)&_font;

		FT_Done_Face(font->face);
	}
	collection->free_texture(collection->texture);
	collection->atlas.free();
	free(collection->fonts);
}

void render_glyph(FontCollection *collection_, Font &font_, u32 font_size, SizedFont &sized_font, utf32 new_char_code_point, List<v3u8, TemporaryAllocator> &temp_pixels) {
	auto collection = (FontCollectionFT *)collection_;
	auto &atlas = collection->atlas;
	auto &font = *(FontFT *)&font_;
	if (!font.face) {
		auto error = FT_New_Memory_Face(collection->ft_library, font.buffer.data, font.buffer.count, 0, &font.face);
		assert(!error, "FT_New_Face failed: {}", FT_Error_String(error));
		font.face->glyph->format = FT_GLYPH_FORMAT_BITMAP;
	}


	if (font.face_size != font_size) {
		font.face_size = font_size;
		auto error = FT_Set_Pixel_Sizes(font.face, font_size, font_size);
		assert(!error);

		sized_font.line_spacing = font.face->size->metrics.height >> 6;
		sized_font.ascender     = font.face->size->metrics.ascender >> 6;
		sized_font.descender    = font.face->size->metrics.descender >> 6;
	}

	auto glyph_index = FT_Get_Char_Index(font.face, new_char_code_point);

	auto error = FT_Load_Glyph(font.face, glyph_index, 0);

	error = FT_Render_Glyph(font.face->glyph, FT_RENDER_MODE_LCD);
	if (error) {
		collection->chars.get_or_insert({new_char_code_point, font_size}) = 0;
		return;
	}

	auto slot = font.face->glyph;

	auto &bitmap = slot->bitmap;

	collection->chars.get_or_insert({new_char_code_point, font_size}) = atlas.areas.count;

	FontChar *info = 0;

	assert(bitmap.width % 3 == 0);
	if (bitmap.width % 3 == 0) {
		v2u char_size = {bitmap.width/3, bitmap.rows};
		info = &atlas.add((v3u8*)bitmap.buffer, char_size, bitmap.pitch);
	} else {
		v2u char_size = {(bitmap.width+2)/3, bitmap.rows};
		temp_pixels.clear();
		temp_pixels.reserve(char_size.x * char_size.y);
		for (int y = 0; y < bitmap.rows; ++y) {
			auto dst = (u8 *)&temp_pixels.data[y * char_size.x];
			auto src = (u8 *)bitmap.buffer + y * bitmap.pitch;
			memcpy(dst, src, bitmap.width);
			memset(dst + bitmap.width, 0, char_size.x*3 - bitmap.width);
		}

		info = &atlas.add(temp_pixels.data, char_size);
	}

	info->offset = {
		slot->bitmap_left,
		(s32)font_size - slot->bitmap_top + sized_font.ascender - sized_font.line_spacing,
	};
	info->advance = {slot->advance.x >> 6, slot->advance.y >> 6};
	info->line_spacing = sized_font.line_spacing;
}

}

#else

namespace tl {

static constexpr char const *font_no_backend_error = "tl/font.h was compiled without a backend. Include one before including tl/font.h";

FontCollection *create_font_collection() {
	invalid_code_path(font_no_backend_error);
}

void free(FontCollection *collection) {
	invalid_code_path(font_no_backend_error);
}

void render_glyph(FontCollection *collection_, Font &font_, u32 font_size, SizedFont &sized_font, utf32 new_char_code_point) {
	invalid_code_path(font_no_backend_error);
}

}

#endif

namespace tl {

void add_font(FontCollection *collection, Span<u8> buffer) {
	auto &font = collection->fonts.add();
	font.buffer = buffer;
}

static void _ensure_all_chars_present(FontCollection *collection, auto reader, u32 font_size) {
	assert(collection->update_texture);

	scoped(collection->allocator);

	auto &atlas = collection->atlas;

	LinearSet<utf32, TemporaryAllocator> new_chars;
	
	if (!collection->chars.find({'?', font_size})) {
		new_chars.add('?');
	}

	while (auto code_point = reader.next()) {
		if (!collection->chars.find({code_point, font_size})) {
			new_chars.add(code_point);
		}
	}
	
	List<v3u8, TemporaryAllocator> temp_pixels;

	if (new_chars.count) {

		bool atlas_was_resized = false;

		aabb<v2u> updated_region = {
			V2u(-1),
			V2u(0),
		};

		while (new_chars.count) {
			auto new_char_code_point = new_chars.pop().value();

			for (auto &font : collection->fonts) {
				auto &sized_font = font.sized.get_or_insert(font_size);

				render_glyph(collection, font, font_size, sized_font, new_char_code_point, temp_pixels);
			}
		}
	}
}
void ensure_all_chars_present(FontCollection *collection_, Span<utf8> text, u32 font_size) {
	return _ensure_all_chars_present(collection_, Utf8Reader{.cursor = text.data, .end = text.end()}, font_size);
}
void ensure_all_chars_present(FontCollection *collection_, Span<utf32> text, u32 font_size) {
	return _ensure_all_chars_present(collection_, Utf32Reader{.cursor = text.data, .end = text.end()}, font_size);
}

void _place_text(TextPlacer *placer, FontCollection *collection, auto reader, PlaceTextParams params TL_LPD) {
	v2s char_position = {};

	placer->quads.clear();
	placer->lines.clear();
	placer->cursor_positions.clear();
	placer->bounds.min = max_value<v2s>;
	placer->bounds.max = min_value<v2s>;
	placer->line_count = 1;

	aabb<s32> line_x_bounds = {max_value<s32>, min_value<s32>};
	umm line_begin_index = 0;
	
	s32 max_line_width = 0;

	auto word_start = reader.checkpoint();
	umm word_start_index = 0;
	bool can_wrap = false;
	
	TextPlacer::Quad c = {};
	#if TL_FONT_USE_SPECIAL_CODES
	c.color = params.default_color;
	#endif

	auto &atlas = collection->atlas;
	auto &char_infos = atlas.areas;
	
	_ensure_all_chars_present(collection, reader, params.font_size);

	auto get_char_info = [&](utf32 ch) {
		auto found = collection->chars.find({ch, params.font_size});
		return char_infos[found ? found->value : 0];
	};

	u32 line_spacing = 0;
	
	placer->cursor_positions.add({.x = 0, .line = 0});

	while (!reader.empty()) {
		utf32 code_point = {};
		FontChar d = {};

		#define read_char()                    \
			do {                               \
				code_point = reader.next();    \
				if (!code_point)               \
					break;                     \
				d = get_char_info(code_point); \
			} while (0)

		read_char();

		#if TL_FONT_USE_SPECIAL_CODES
		if (code_point == TL_FONT_SPECIAL_CODE_PREFIX && params.use_codes) {
			read_char();
			placer->cursor_positions.add({.x = (u32)char_position.x, .line = (u16)placer->lines.count});
			if (code_point == TL_FONT_SPECIAL_CODE_PREFIX) {
				// Escaped $
			} else {
				auto hex_digit_to_int = [](utf32 c) {
					u8 result = 0;
					result += (c <= '9') * (c - '0');
					result += ((c > '9') & (c <= 'F')) * (c - 'A' + 10);
					result += ((c > 'F') & (c <= 'f')) * (c - 'a' + 10);
					return result;
				};

				c.color.r = hex_digit_to_int(code_point); 
				read_char();
				c.color.g = hex_digit_to_int(code_point); 
				read_char();
				c.color.b = hex_digit_to_int(code_point); 
				read_char();
				placer->cursor_positions.add({.x = (u32)char_position.x, .line = (u16)placer->lines.count});
				placer->cursor_positions.add({.x = (u32)char_position.x, .line = (u16)placer->lines.count});
				placer->cursor_positions.add({.x = (u32)char_position.x, .line = (u16)placer->lines.count});
			}
		}
		#endif

		if (is_whitespace(code_point)) {
			word_start = reader.checkpoint();
			word_start_index = placer->quads.count;
			can_wrap = true;
		}

		if (code_point == '\n') {
			placer->lines.add({.begin_index = line_begin_index, .end_index = placer->quads.count, .width = line_x_bounds.size()});
			max_line_width = max(max_line_width, line_x_bounds.size());
			line_x_bounds = {max_value<s32>, min_value<s32>};
			char_position.x = 0;
			char_position.y += line_spacing;
			placer->line_count += 1;
			line_begin_index = placer->quads.count;

			d = {};
		}

		auto place_char = [&] {
			c.position.min = {char_position.x + d.offset.x, char_position.y + d.offset.y};
			c.position.max = c.position.min + (v2s)d.rect.size();
		};

		place_char();

		if (can_wrap && c.position.max.x > params.wrap_width) {
			can_wrap = false;

			reader.reset(word_start);
			read_char();

			placer->quads.resize(word_start_index);

			char_position.x = 0;
			char_position.y += line_spacing;
			placer->line_count += 1;

			place_char();
		}

		c.uv.min = d.rect.min;
		c.uv.max = d.rect.max;

		placer->bounds.max = max(placer->bounds.max, c.position.max);
		placer->bounds.min = min(placer->bounds.min, c.position.min);

		placer->quads.add(c TL_LA);

		line_x_bounds.min = min(line_x_bounds.min, c.position.min.x);
		line_x_bounds.max = max(line_x_bounds.max, c.position.max.x);

		char_position.x += d.advance.x;

		line_spacing = max(line_spacing, d.line_spacing);

		placer->cursor_positions.add({.x = (u32)char_position.x, .line = (u16)placer->lines.count});

		#undef read_char
	}
	
	placer->lines.add({.begin_index = line_begin_index, .end_index = placer->quads.count, .width = line_x_bounds.size()});
	max_line_width = max(max_line_width, line_x_bounds.size());

	if (params.line_alignment) {
		for (auto &line : placer->lines) {
			for (umm i = line.begin_index; i < line.end_index; ++i) {
				s32 offset = round_to_int((max_line_width - line.width) * params.line_alignment);
				placer->quads[i].position.min.x += offset;
				placer->quads[i].position.max.x += offset;
			}
		}
	}
}
void TextPlacer::place(FontCollection *collection, Span<utf8> text, PlaceTextParams params TL_LPD) {
	_place_text(this, collection, Utf8Reader{.cursor = text.data, .end = text.end()}, params TL_LA);
}
void TextPlacer::place(FontCollection *collection, Span<utf32> text, PlaceTextParams params TL_LPD) {
	_place_text(this, collection, Utf32Reader{.cursor = text.data, .end = text.end()}, params TL_LA);
}
}

#endif
