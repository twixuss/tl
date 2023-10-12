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


struct PlacedChar {
	aabb<v2s> position = {};
	aabb<v2f> uv = {};
};

struct TextInfo {
	List<PlacedChar> placed_chars;
	aabb<v2s> bounds = {};
	u32 line_count = 0;
};

struct GetTextInfoParams {
	bool place_chars = false;
	bool bounds = false;
};

TL_API FontChar get_char_info(u32 ch, SizedFont *font);
TL_API bool ensure_all_chars_present(Span<utf8> text, SizedFont *font);
TL_API void free(FontCollection *collection);
TL_API TextInfo calculate_text(Span<utf8> text, SizedFont *font, GetTextInfoParams params = {} TL_LP);
inline List<PlacedChar> place_text(Span<utf8> text, SizedFont *font TL_LP) {
	return calculate_text(text, font, {.place_chars=true} TL_LA).placed_chars;
}
inline aabb<v2s> get_text_bounds(Span<utf8> text, SizedFont *font TL_LP) {
	return calculate_text(text, font, {.bounds=true} TL_LA).bounds;
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

//
// Returns true if new characters were added
//
bool ensure_all_chars_present(Span<utf8> text, SizedFont *sized_font) {
	assert(sized_font);

	auto font = (FontFT *)sized_font->font;
	auto collection = (FontCollectionFT *)font->collection;

	assert(collection->update_atlas);

	scoped(collection->allocator);


	auto current_char = text.data;

	struct CharPointer {
		utf32 code_point;
		FontChar *info;
	};

	List<CharPointer> new_chars;
	new_chars.allocator = temporary_allocator;

	while (current_char < text.end()) {
		auto got_char = get_char_and_advance_utf8(&current_char);
		if (!got_char.has_value()) {
			invalid_code_path();
		}
		auto code_point = got_char.value();
		auto found = sized_font->chars.find(code_point);
		if (!found) {
			auto &inserted = sized_font->chars.get_or_insert(code_point);

			CharPointer p;
			p.code_point = code_point;
			p.info = &inserted;
			new_chars.add(p);
		}
	}

	if (new_chars.count) {

	redo_all:
		for (auto &[new_char_code_point, new_char] : new_chars) {

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

				new_chars.clear();
				for_each(sized_font->chars, [&] (auto &kv) {
					auto &[code_point, new_char] = kv;
					CharPointer p;
					p.code_point = code_point;
					p.info = &new_char;
					new_chars.add(p);
				});

				goto redo_all;
			}

			//
			// NOTE bitmap.width is in bytes not pixels
			//
			auto &info = *new_char;
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

		return true;
	}
	return false;
}

SizedFont *get_font_at_size(Font *font, u32 size) {
	assert(font);
	assert(size);

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

		// FIXME: implement free for HashMap
		// free(font->size_to_font);

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

TextInfo calculate_text(Span<utf8> text, SizedFont *font, GetTextInfoParams params TL_LPD) {
	TextInfo info = {};
	v2s char_position = {};
	char_position.y = font->ascender - font->line_spacing;

	if (params.bounds) {
		info.bounds.min = max_value<v2s>;
		info.bounds.max = min_value<v2s>;
	}
	info.line_count = 1;

	auto current_char = text.data;
	while (current_char < text.end()) {
		auto got_char = get_char_and_advance_utf8(&current_char);
		if (!got_char) {
			invalid_code_path();
		}
		auto code_point = got_char.value();

		auto d = get_char_info(code_point, font);

		if (code_point == '\n') {
			char_position.x = 0;
			char_position.y += font->line_spacing;
			info.line_count += 1;
		}
		if (params.place_chars) {
			if (code_point != '\n') {
				PlacedChar c;

				c.position.min = {char_position.x + d.offset.x, char_position.y + (s32)font->size - d.offset.y};
				c.position.max = c.position.min + (v2s)d.size;

				c.uv.min = (v2f)d.position / (v2f)font->atlas_size;
				c.uv.max = c.uv.min + (v2f)d.size / (v2f)font->atlas_size;

				info.placed_chars.add(c TL_LA);
			}
		}
		if (params.bounds) {
			if (code_point != '\n') {
				v2s position_min = {char_position.x + d.offset.x, char_position.y + font->size - d.offset.y};
				v2s position_max = position_min + (v2s)d.size;
				info.bounds.max = max(info.bounds.max, position_max);
				info.bounds.min = min(info.bounds.min, position_min);

			}
		}
		if (code_point != '\n') {
			char_position.x += d.advance.x;
		}
	}
	return info;
}

}

#endif
