#pragma once

#include "common.h"
#include "list.h"
#include "buffer.h"
#include "file.h"
#include "hash_map.h"
#include "math.h"

#include <unordered_map>

#ifndef TL_FONT_TEXTURE_HANDLE
#define TL_FONT_TEXTURE_HANDLE void *
#endif

namespace tl {

struct FontChar {
	v2u position;
	v2u size;
	v2s offset, advance;
};

struct FontCollection;
struct SizedFont;
struct Font;


struct Font {
	FontCollection *collection = 0;

	List<utf8> path;
	HashMap<u32, SizedFont> size_to_font;
};

TL_API SizedFont *get_font_at_size(Font *font, u32 size);


struct SizedFont {
	Font *font = {};

	ContiguousHashMap<u32, FontChar> chars;
	TL_FONT_TEXTURE_HANDLE texture = {};
	u32 size = {};

	u8 *atlas_data = 0;
	v2u atlas_size = {};

	v2u next_char_position = {};
	u32 current_row_height = 0;

	s32 line_spacing = 0;
	s32 ascender     = 0;
	s32 descender    = 0;
};


struct FontCollection {
	Allocator allocator = current_allocator;
	List<Font *> fonts;
	TL_FONT_TEXTURE_HANDLE (*update_atlas)(TL_FONT_TEXTURE_HANDLE texture, void *data, v2u size);
};

TL_API FontCollection *create_font_collection();
TL_API void free(FontCollection *collection);

TL_API Font *add_font(FontCollection *collection, Span<utf8> path);

struct PlacedText {
	struct Char {
		aabb<v2s> position = {};
		aabb<v2f> uv = {};
	};
	struct Line {
		umm begin_index = 0;
		umm end_index = 0;
		s32 width = 0;
	};

	List<Char> chars;
	List<Line> lines;
	aabb<v2s> bounds = {};
	u32 line_count = 0;

	inline Optional<umm> line_of(umm char_index) {
		for (umm i = 0; i < lines.count; ++i) {
			auto line = lines[i];
			if ((char_index - line.begin_index) < (line.end_index - line.begin_index))
				return i;
		}
		return {};
	}
};

struct PlaceTextParams {
	f32 line_alignment = 0; // 0 = left-aligned; 0.5 = centered; 1 = right-aligned
	s32 wrap_width = max_value<s32>;
};

TL_API FontChar get_char_info(u32 ch, SizedFont *font);

struct EnsureAllCharsPresentResult {
	bool new_chars_added = false;
	bool atlas_resized = false;
};

// Returns true if new characters were added
TL_API EnsureAllCharsPresentResult ensure_all_chars_present(Span<utf8> text, SizedFont *font);

TL_API void free(FontCollection *collection);
TL_API PlacedText place_text(Span<utf8> text, SizedFont *font, PlaceTextParams params = {} TL_LP);

inline void free(PlacedText &text) {
	free(text.chars);
	free(text.lines);
}

}

#ifdef TL_IMPL

#ifdef FREETYPE_H_

namespace tl {

struct FontFT : Font {
	Buffer file;
	FT_Face face;
	u32 size;
};

struct FontCollectionFT : FontCollection {
	FT_Library ft_library;
};

void init_ft(FontFT *ft) {
	ft->file = read_entire_file(ft->path);
	auto error = FT_New_Memory_Face(static_cast<FontCollectionFT *>(ft->collection)->ft_library, ft->file.data, ft->file.count, 0, &ft->face);
	assert(!error, "FT_New_Face failed: {}", FT_Error_String(error));
	ft->face->glyph->format = FT_GLYPH_FORMAT_BITMAP;
}

EnsureAllCharsPresentResult ensure_all_chars_present(Span<utf8> text, SizedFont *sized_font) {
	assert(sized_font);

	auto font = (FontFT *)sized_font->font;
	auto collection = (FontCollectionFT *)font->collection;

	assert(collection->update_atlas);

	scoped(collection->allocator);


	auto current_char = text.data;

	List<utf32> new_chars;
	new_chars.allocator = temporary_allocator;

	while (current_char < text.end()) {
		auto got_char = decode_and_advance(&current_char);
		if (!got_char.has_value()) {
			invalid_code_path();
		}
		auto code_point = got_char.value();
		if (!sized_font->chars.find(code_point)) {
			new_chars.add(code_point);
		}
	}

	if (new_chars.count) {

		bool atlas_was_resized = false;

	redo_all:
		while (new_chars.count) {
			auto new_char_code_point = new_chars.pop().value();

		retry_glyph:
			if (!font->face) {
				init_ft(font);
			}

			if (font->size != sized_font->size) {
				font->size = sized_font->size;
				auto error = FT_Set_Pixel_Sizes(font->face, sized_font->size, sized_font->size);
				assert(!error);

				sized_font->line_spacing = font->face->size->metrics.height / 64;
				sized_font->ascender     = font->face->size->metrics.ascender / 64;
				sized_font->descender    = font->face->size->metrics.descender / 64;
			}

			auto glyph_index = FT_Get_Char_Index(font->face, new_char_code_point);

			auto error = FT_Load_Glyph(font->face, glyph_index, 0);

			error = FT_Render_Glyph(font->face->glyph, FT_RENDER_MODE_LCD);
			if (error)
				continue;

			auto slot = font->face->glyph;
			auto &bitmap = slot->bitmap;

			v2u char_size = {bitmap.width/3, bitmap.rows};

			if (sized_font->next_char_position.x + char_size.x >= sized_font->atlas_size.x) {
				sized_font->next_char_position.x = 0;
				sized_font->next_char_position.y += sized_font->current_row_height;
				sized_font->current_row_height = 0;
			}

			if ((sized_font->next_char_position.y + char_size.y > sized_font->atlas_size.y) || !sized_font->atlas_data) {

				//
				// TODO:
				// Do not reset the atlas, just extend it
				//

				if (sized_font->atlas_data) {
					sized_font->atlas_size *= 2;
					current_allocator.free(sized_font->atlas_data);
				} else {
					sized_font->atlas_size = V2u(sized_font->size * 16);
				}
				sized_font->atlas_data = current_allocator.allocate<u8>(sized_font->atlas_size.x * sized_font->atlas_size.y * 3);
				memset(sized_font->atlas_data, 0, sized_font->atlas_size.x * sized_font->atlas_size.y * 3);

				sized_font->next_char_position = {};
				sized_font->current_row_height = 0;

				new_chars.add(new_char_code_point);
				for_each(sized_font->chars, [&] (auto &kv) {
					auto &[code_point, _] = kv;
					new_chars.add(code_point);
				});

				atlas_was_resized = true;

				goto redo_all;
			}

			//
			// NOTE bitmap.width is in bytes not pixels
			//
			auto &info = sized_font->chars.get_or_insert(new_char_code_point);
			info.position = sized_font->next_char_position;
			info.size = char_size;
			info.offset = {slot->bitmap_left, slot->bitmap_top};
			info.advance = {slot->advance.x >> 6, slot->advance.y >> 6};

			//
			// Add new rows
			//

			for (u32 y = 0; y < bitmap.rows; ++y) {
				memcpy(
					sized_font->atlas_data + ((sized_font->next_char_position.y + y)*sized_font->atlas_size.x + sized_font->next_char_position.x) * 3,
					bitmap.buffer + y*bitmap.pitch,
					bitmap.width
				);
			}
			sized_font->next_char_position.x += char_size.x;
			sized_font->current_row_height = max(sized_font->current_row_height, char_size.y);
		}

		sized_font->texture = collection->update_atlas(sized_font->texture, sized_font->atlas_data, sized_font->atlas_size);

		return {
			.new_chars_added = true,
			.atlas_resized = atlas_was_resized,
		};
	}
	return {
		.new_chars_added = false,
		.atlas_resized = false,
	};
}

SizedFont *get_font_at_size(Font *font, u32 size) {
	assert(font);
	assert(size);
	scoped(font->collection->allocator);

	auto found = font->size_to_font.find(size);
	if (found) {
		return &found->value;
	}

	SizedFont &sized_font = font->size_to_font.get_or_insert(size);

	sized_font.font = font;

	sized_font.size = size;

	return &sized_font;
}

FontCollection *create_font_collection() {
	auto allocator = current_allocator;
	auto result = allocator.allocate<FontCollectionFT>();
	result->allocator = allocator;

	auto error = FT_Init_FreeType(&result->ft_library);
	assert(!error, "FT_Init_FreeType Error {}", FT_Error_String(error));
	return result;
}

Font *add_font(FontCollection *collection, Span<utf8> path) {
	auto font = (FontFT *)collection->fonts.add(collection->allocator.allocate<FontFT>());
	font->path.set(path);
	font->collection = collection;
	return font;
}

void free(FontCollection *collection) {
	for (auto &_font : collection->fonts) {
		auto font = (FontFT *)_font;
		for_each(font->size_to_font, [&](auto &kv) {
			auto &[size, sized_font] = kv;
			collection->allocator.free(sized_font.atlas_data);
		});
		free(font->path);

		free(font->size_to_font);

		free(font->file);
		FT_Done_Face(font->face);

		collection->allocator.free(font);
	}
	free(collection->fonts);
	collection->allocator.free(collection);
}

}

#endif

namespace tl {

FontChar get_char_info(u32 ch, SizedFont *font) {
	auto found = font->chars.find(ch);

	assert(found, "get_char_info: character '{}' is not present. Call ensure_all_chars_present before", ch);
	return found->value;
}

PlacedText place_text(Span<utf8> text, SizedFont *font, PlaceTextParams params TL_LPD) {
	PlacedText result = {};
	v2s char_position = {};
	char_position.y = font->ascender - font->line_spacing;

	result.bounds.min = max_value<v2s>;
	result.bounds.max = min_value<v2s>;
	result.line_count = 1;

	aabb<s32> line_x_bounds = {max_value<s32>, min_value<s32>};
	umm line_begin_index = 0;
	
	s32 max_line_width = 0;

	auto current_char = text.data;

	utf8 *word_start = text.begin();
	umm word_start_index = 0;
	bool can_wrap = false;

	while (current_char < text.end()) {
		utf32 code_point = {};
		FontChar d = {};

		auto read_char = [&] {
			auto got_char = decode_and_advance(&current_char);
			if (!got_char) {
				invalid_code_path();
			}
			code_point = got_char.value();
			d = get_char_info(code_point, font);
		};

		read_char();

		if (is_whitespace(code_point)) {
			word_start = current_char;
			word_start_index = result.chars.count;
			can_wrap = true;
		}

		if (code_point == '\n') {
			result.lines.add({.begin_index = line_begin_index, .end_index = result.chars.count, .width = line_x_bounds.size()});
			max_line_width = max(max_line_width, line_x_bounds.size());
			line_x_bounds = {max_value<s32>, min_value<s32>};
			char_position.x = 0;
			char_position.y += font->line_spacing;
			result.line_count += 1;
			line_begin_index = result.chars.count;
		} else {
			PlacedText::Char c;

			auto place_char = [&] {
				c.position.min = {char_position.x + d.offset.x, char_position.y + (s32)font->size - d.offset.y};
				c.position.max = c.position.min + (v2s)d.size;
			};

			place_char();

			if (can_wrap && c.position.max.x > params.wrap_width) {
				can_wrap = false;

				current_char = word_start;
				read_char();

				result.chars.resize(word_start_index);

				char_position.x = 0;
				char_position.y += font->line_spacing;
				result.line_count += 1;

				place_char();
			}

			c.uv.min = (v2f)d.position / (v2f)font->atlas_size;
			c.uv.max = c.uv.min + (v2f)d.size / (v2f)font->atlas_size;

			result.bounds.max = max(result.bounds.max, c.position.max);
			result.bounds.min = min(result.bounds.min, c.position.min);

			result.chars.add(c TL_LA);

			line_x_bounds.min = min(line_x_bounds.min, c.position.min.x);
			line_x_bounds.max = max(line_x_bounds.max, c.position.max.x);

			char_position.x += d.advance.x;
		}
	}
	
	result.lines.add({.begin_index = line_begin_index, .end_index = result.chars.count, .width = line_x_bounds.size()});
	max_line_width = max(max_line_width, line_x_bounds.size());

	for (auto &line : result.lines) {
		for (umm i = line.begin_index; i < line.end_index; ++i) {
			s32 offset = round_to_int((max_line_width - line.width) * params.line_alignment);
			result.chars[i].position.min.x += offset;
			result.chars[i].position.max.x += offset;
		}
	}

	return result;
}

}

#endif
