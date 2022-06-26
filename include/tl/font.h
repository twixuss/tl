#pragma once

#include "common.h"
#include "list.h"
#include "buffer.h"
#include "file.h"
#include "hash_map.h"
#include "math.h"

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

struct SizedFont {
	FontCollection *collection;

	HashMap<u32, FontChar> chars;
	TL_FONT_TEXTURE_HANDLE texture;
	u32 size;

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
	HashMap<u32, SizedFont> size_to_font;
	TL_FONT_TEXTURE_HANDLE (*update_atlas)(TL_FONT_TEXTURE_HANDLE texture, void *data, v2u size);
};

struct PlacedChar {
	aabb<v2s> position;
	aabb<v2f> uv;
};

struct TextInfo {
	List<PlacedChar> placed_chars;
	aabb<v2s> bounds;
	u32 line_count;
};

struct GetTextInfoParams {
	bool place_chars = false;
	bool bounds = false;
};

TL_API FontChar get_char_info(u32 ch, SizedFont *font);
TL_API bool ensure_all_chars_present(Span<utf8> text, SizedFont *font);
TL_API SizedFont *get_font_at_size(FontCollection *collection, u32 size);
TL_API FontCollection *create_font_collection(Span<Span<utf8>> font_paths);
TL_API void free(FontCollection *collection);
TL_API TextInfo get_text_info(Span<utf8> text, SizedFont *font, GetTextInfoParams params = {} TL_LP);
inline List<PlacedChar> place_text(Span<utf8> text, SizedFont *font TL_LP) {
	return get_text_info(text, font, {.place_chars=true} TL_LA).placed_chars;
}
inline aabb<v2s> get_text_bounds(Span<utf8> text, SizedFont *font TL_LP) {
	return get_text_info(text, font, {.bounds=true} TL_LA).bounds;
}

}

#ifdef TL_IMPL

#ifdef FREETYPE_H_

namespace tl {

static FT_Library ft_library;

struct FontFace {
	List<utf8> path;
	Buffer file;
	FT_Face ft;
	u32 size;
};

struct FontCollectionFT : FontCollection {
	List<FontFace> faces;
};

void init_face(FontFace &face) {
	face.file = read_entire_file(face.path);
	defer {
		print("Loaded font '{}'\n", face.path);
		free(face.path);
	};

	{
		auto error = FT_New_Memory_Face(ft_library, face.file.data, face.file.count, 0, &face.ft);
		if (error) {
			invalid_code_path("FT_New_Face failed: {}", FT_Error_String(error));
		}
	}

	face.ft->glyph->format = FT_GLYPH_FORMAT_BITMAP;
}

//
// Returns true if new characters were added
//
bool ensure_all_chars_present(Span<utf8> text, SizedFont *font) {
	assert(font);

	auto collection = (FontCollectionFT *)font->collection;

	assert(collection->update_atlas);

	scoped_allocator(collection->allocator);


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
		auto found = font->chars.find(code_point);
		if (!found) {
			auto &inserted = font->chars.get_or_insert(code_point);

			CharPointer p;
			p.code_point = code_point;
			p.info = &inserted;
			new_chars.add(p);
		}
	}

	if (new_chars.count) {

	redo_all:
		for (auto &[new_char_code_point, new_char] : new_chars) {
			auto current_face = collection->faces.data;

		retry_glyph:
			if (!current_face->ft) {
				init_face(*current_face);
			}

			if (current_face->size != font->size) {
				current_face->size = font->size;
				auto error = FT_Set_Pixel_Sizes(current_face->ft, font->size, font->size);
				assert(!error);

				font->line_spacing = current_face->ft->size->metrics.height / 64;
				font->ascender     = current_face->ft->size->metrics.ascender / 64;
				font->descender    = current_face->ft->size->metrics.descender / 64;
			}

			auto glyph_index = FT_Get_Char_Index(current_face->ft, new_char_code_point);

			auto error = FT_Load_Glyph(current_face->ft, glyph_index, 0);

			if (error || glyph_index == 0) {
				//if (error) print("{}\n", FT_Error_String(error));
				++current_face;
				if (current_face == collection->faces.end()) {
					//
					// Load default glyph from first face
					//
					current_face = collection->faces.data;
					FT_Load_Glyph(current_face->ft, 0, 0);
				} else {
					//
					// Retry with new face
					//
					goto retry_glyph;
				}
			}

			error = FT_Render_Glyph(current_face->ft->glyph, FT_RENDER_MODE_LCD);
			if (error)
				continue;

			auto slot = current_face->ft->glyph;
			auto &bitmap = slot->bitmap;

			v2u char_size = {bitmap.width/3, bitmap.rows};

			if (font->next_char_position.x + char_size.x >= font->atlas_size.x) {
				font->next_char_position.x = 0;
				font->next_char_position.y += font->current_row_height;
				font->current_row_height = 0;
			}

			if ((font->next_char_position.y + char_size.y > font->atlas_size.y) || !font->atlas_data) {

				//
				// TODO:
				// Do not reset the atlas, just extend it
				//

				if (font->atlas_data) {
					font->atlas_size *= 2;
					current_allocator.free(font->atlas_data);
				} else {
					font->atlas_size = V2u(font->size * 16);
				}
				font->atlas_data = current_allocator.allocate<u8>(font->atlas_size.x * font->atlas_size.y * 3);
				memset(font->atlas_data, 0, font->atlas_size.x * font->atlas_size.y * 3);

				font->next_char_position = {};
				font->current_row_height = 0;

				new_chars.clear();
				for_each(font->chars, [&] (u32 &code_point, FontChar &new_char) {
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
			info.position = font->next_char_position;
			info.size = char_size;
			info.offset = {slot->bitmap_left, slot->bitmap_top};
			info.advance = {slot->advance.x >> 6, slot->advance.y >> 6};

			//
			// Add new rows
			//

			for (u32 y = 0; y < bitmap.rows; ++y) {
				memcpy(
					font->atlas_data + ((font->next_char_position.y + y)*font->atlas_size.x + font->next_char_position.x) * 3,
					bitmap.buffer + y*bitmap.pitch,
					bitmap.width
				);
			}
			font->next_char_position.x += char_size.x;
			font->current_row_height = max(font->current_row_height, char_size.y);
		}

		font->texture = collection->update_atlas(font->texture, font->atlas_data, font->atlas_size);

		return true;
	}
	return false;
}

SizedFont *get_font_at_size(FontCollection *collection, u32 size) {
	assert(collection);
	assert(size);

	auto found = collection->size_to_font.find(size);
	if (found) {
		return &found->value;
	}

	SizedFont &font = collection->size_to_font.get_or_insert(size);

	font.collection = collection;

	font.size = size;

	return &font;
}

FontCollection *create_font_collection(Span<Span<utf8>> font_paths) {
	if (!ft_library) {
		auto error = FT_Init_FreeType(&ft_library);
		assert(!error, "FT_Init_FreeType Error {}", FT_Error_String(error));
	}

	auto allocator = current_allocator;
	auto result = allocator.allocate<FontCollectionFT>();

	for (auto &path : font_paths) {
		auto &face = result->faces.add();
		face.path.set(path);
	}

	return result;
}

void free(FontCollection *_collection) {
	auto collection = (FontCollectionFT *)_collection;

	for (auto &face : collection->faces) {
		free(face.file);
		FT_Done_Face(face.ft);
	}
	free(collection->faces);
	for_each(collection->size_to_font, [&](u32 size, SizedFont &font) {
		collection->allocator.free(font.atlas_data);
	});
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

TextInfo get_text_info(Span<utf8> text, SizedFont *font, GetTextInfoParams params TL_LPD) {
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
