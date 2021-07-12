#pragma once

#include "common.h"
#include "list.h"
#include "buffer.h"
#include "file.h"
#include <unordered_map>

namespace tl {

struct FontChar {
	v2u position;
	v2u size;
	v2s offset, advance;
};

struct FontCollection;

struct SizedFont {
	FontCollection *collection;

	std::unordered_map<u32, FontChar> chars;
	umm texture_id;
	u32 size;

	u8 *atlas_data = 0;
	v2u atlas_size = {};

	v2u next_char_position = {};
	u32 current_row_height = 0;
};

struct FontCollection {
	Allocator allocator = current_allocator;
	std::unordered_map<u32, SizedFont> size_to_font;
	umm (*update_atlas)(umm texture_id, void *data, v2u size);
};

struct PlacedChar {
	aabb<v2f> position;
	aabb<v2f> uv;
};

TL_API FontChar get_char_info(u32 ch, SizedFont *font);
TL_API bool ensure_all_chars_present(Span<utf8> text, SizedFont *font);
TL_API SizedFont *get_font_at_size(FontCollection *collection, u32 size);
TL_API FontCollection *create_font_collection(Span<Span<filechar>> font_paths);
TL_API void free(FontCollection *collection);
TL_API aabb<v2s> get_text_bounds(Span<utf8> text, SizedFont *font, bool min_at_zero = false);
TL_API List<PlacedChar> place_text(Span<utf8> text, SizedFont *font, bool shrink = false);

}

#ifdef TL_IMPL

#ifdef FREETYPE_H_

namespace tl {

static FT_Library ft_library;

struct FontFace {
	List<filechar> path;
	Buffer file;
	FT_Face ft;
	u32 size;
};

struct FontCollectionFT : FontCollection {
	List<FontFace> faces;
};

FontChar get_char_info(u32 ch, SizedFont *font) {
	auto found = font->chars.find(ch);
	assert(found != font->chars.end(), "get_char_info: character '%' is not present. Call ensure_all_chars_present before", ch);
	return found->second;
}

void init_face(FontFace &face) {
	face.file = read_entire_file(face.path);
	defer {
		print("Loaded font '%'\n", face.path);
		free(face.path);
	};

	{
		timed_block("FT_New_Memory_Face"s);
		auto error = FT_New_Memory_Face(ft_library, face.file.data, face.file.size, 0, &face.ft);
		if (error) {
			invalid_code_path("FT_New_Face failed: %", FT_Error_String(error));
		}
	}

	face.ft->glyph->format = FT_GLYPH_FORMAT_BITMAP;
}
void set_size(FontFace &face, u32 size) {
	if (face.size == size) return;
	face.size = size;
	auto error = FT_Set_Pixel_Sizes(face.ft, size, size);
	assert(!error);
}

//
// Returns true if new characters were added
//
bool ensure_all_chars_present(Span<utf8> text, SizedFont *font) {
	timed_function();

	auto collection = (FontCollectionFT *)font->collection;

	scoped_allocator(collection->allocator);


	auto current_char = text.data;

	struct CharPointer {
		u32 code_point;
		FontChar *info;
	};

	List<CharPointer> new_chars;
	new_chars.allocator = temporary_allocator;

	while (current_char < text.end()) {
		u32 code_point;
		if (!get_char_and_advance_utf8(current_char, &code_point)) {
			invalid_code_path();
		}
		auto found = font->chars.find(code_point);
		if (found == font->chars.end()) {
			auto inserted = font->chars.insert({code_point, FontChar{}}).first;
			CharPointer p;
			p.code_point = inserted->first;
			p.info = &inserted->second;
			new_chars.add(p);
		}
	}

	if (new_chars.size) {

	redo_all:
		for (auto &[new_char_code_point, new_char] : new_chars) {
			auto current_face = collection->faces.data;

		retry_glyph:
			if (!current_face->ft) {
				init_face(*current_face);
			}
			set_size(*current_face, font->size);
			auto glyph_index = FT_Get_Char_Index(current_face->ft, new_char_code_point);

			auto error = FT_Load_Glyph(current_face->ft, glyph_index, 0);

			if (error || glyph_index == 0) {
				//if (error) print("%\n", FT_Error_String(error));
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
					FREE(current_allocator, font->atlas_data);
				} else {
					font->atlas_size = V2u(font->size * 16);
				}
				font->atlas_data = ALLOCATE(u8, current_allocator, font->atlas_size.x * font->atlas_size.y * 3);
				memset(font->atlas_data, 0, font->atlas_size.x * font->atlas_size.y * 3);

				font->next_char_position = {};
				font->current_row_height = 0;

				new_chars.clear();
				for (auto &[code_point, new_char] : font->chars) {
					CharPointer p;
					p.code_point = code_point;
					p.info = &new_char;
					new_chars.add(p);
				}

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

		font->texture_id = collection->update_atlas(font->texture_id, font->atlas_data, font->atlas_size);

		return true;
	}
	return false;
}

SizedFont *get_font_at_size(FontCollection *collection, u32 size) {
	timed_function();

	auto found = collection->size_to_font.find(size);
	if (found != collection->size_to_font.end()) {
		return &found->second;
	}

	SizedFont &font = collection->size_to_font[size];

	font.collection = collection;

	font.size = size;

	return &font;
}

FontCollection *create_font_collection(Span<Span<filechar>> font_paths) {
	timed_function();

	if (!ft_library) {
		auto error = FT_Init_FreeType(&ft_library);
		assert(!error, "FT_Init_FreeType Error %", FT_Error_String(error));
	}

	auto allocator = current_allocator;
	auto result = ALLOCATE(FontCollectionFT, allocator);

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
	for (auto &[size, font] : collection->size_to_font) {
		FREE(collection->allocator, font.atlas_data);
	}
	FREE(collection->allocator, collection);
}

aabb<v2s> get_text_bounds(Span<utf8> text, SizedFont *font, bool min_at_zero) {
	if (!text.size)
		return {};

	timed_function();

	auto collection = (FontCollectionFT *)font->collection;

	scoped_allocator(collection->allocator);

	v2s pos = {};

	aabb<v2s> result;

	result.max = min_value<v2s>;

	if (min_at_zero) {
		result.min = {};
	} else {
		result.min = max_value<v2s>;
	}

	auto current_char = text.data;
	while (current_char < text.end()) {
		u32 ch;
		if (!get_char_and_advance_utf8(current_char, &ch)) {
			invalid_code_path();
		}

		auto d = get_char_info(ch, font);

		if (ch == '\n') {
			pos.x = 0;
			pos.x += font->size;
			continue;
		}


		v2s position_min = {pos.x + d.offset.x, pos.y + font->size - d.offset.y};
		v2s position_max = position_min + (v2s)d.size;
		result.max = max(result.max, position_max);
		result.min = min(result.min, position_min);

		pos.x += d.advance.x;
	}

	return result;
}

List<PlacedChar> place_text(Span<utf8> text, SizedFont *font, bool shrink) {
	timed_function();

	List<PlacedChar> result;

	f32 posX = 0;
	f32 posY = 0;

	auto current_char = text.data;
	while (current_char < text.end()) {
		u32 ch;
		if (!get_char_and_advance_utf8(current_char, &ch)) {
			invalid_code_path();
		}

		auto d = get_char_info(ch, font);

		if (ch == '\n') {
			posX = 0;
			posY += font->size;
			continue;
		}

		PlacedChar c;

		c.position.min = {posX + d.offset.x, posY + font->size - d.offset.y};
		c.position.max = c.position.min + (v2f)d.size;

		c.uv.min = (v2f)d.position / (v2f)font->atlas_size;
		c.uv.max = c.uv.min + (v2f)d.size / (v2f)font->atlas_size;

		result.add(c);

		posX += d.advance.x;
	}

	return result;
}

}

#else
#error No font rasterization library detected
#endif

#endif
