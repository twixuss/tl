#pragma once
#ifdef FREETYPE_H_

#include "common.h"
#include "list.h"

namespace TL {

struct FontChar {
	v2u position;
	v2u size;
	v2s offset, advance;
};

struct CharBucket {
	u32 index;
	Array<FontChar, 256> chars;
};

struct FontCollection;

struct SizedFont {
	FontCollection *collection;

	std::unordered_map<u32, CharBucket> char_buckets;
	void *texture;
	u32 size;
	
	u8 *atlas_data = 0;
	v2u atlas_size = {};
	
	v2u next_char_position = {};
	u32 current_row_height = 0;
};

struct FontCollection {
	List<FT_Face> faces;
	std::unordered_map<u32, SizedFont> size_to_font;
	List<Buffer> files;
	void *(*create_atlas)();
	void (*update_atlas)(void *texture, void *data, v2u size);
};

inline FontChar get_char_info(u32 ch, SizedFont &font) {
	auto found = font.char_buckets.find(ch >> 8);
	assert(found != font.char_buckets.end(), "get_char_info: character is not present. Call ensure_all_chars_present before");
	return found->second.chars[ch & 0xff];
}

inline void ensure_all_chars_present(Span<utf8> text, SizedFont &font) {
	auto current_char = text.data;

	List<CharBucket *> new_buckets;
	defer { free(new_buckets); };

	while (current_char < text.end()) {
		u32 code_point = get_char_and_advance_utf8(current_char);
		u32 bucket_index = code_point >> 8;
		auto found = font.char_buckets.find(bucket_index);
		if (found == font.char_buckets.end()) {
			auto &bucket = font.char_buckets.insert({bucket_index, CharBucket{}}).first->second;
			bucket.index = bucket_index;
			new_buckets.add(&bucket);
		}
	}

	if (new_buckets.size) {
		for (auto &face : font.collection->faces) {
			auto error = FT_Set_Pixel_Sizes(face, font.size, font.size);
			assert(!error);
		}

	retry_buckets:
		auto current_face = font.collection->faces.data;
		for (auto bucket_pointer : new_buckets) {
			auto &bucket = *bucket_pointer;

			for (u32 char_index = 0; char_index < 256; ++char_index) {

			retry_glyph:
				auto glyph_index = FT_Get_Char_Index(*current_face, (bucket.index << 8) | char_index);

				auto error = FT_Load_Glyph(
					*current_face,          /* handle to face object */
					glyph_index,   /* glyph index           */
					0              /* load flags, see below */
				);

				if (error || glyph_index == 0) {
					if (current_face == &font.collection->faces.back()) {
						current_face = font.collection->faces.data;
						FT_Load_Glyph(*current_face, 0, 0);
					} else {
						++current_face;
						goto retry_glyph;
					}
				}


				error = FT_Render_Glyph((*current_face)->glyph, FT_RENDER_MODE_LCD);
				if (error)
					continue;

				auto slot = (*current_face)->glyph;
				auto &bitmap = slot->bitmap;

				v2u char_size = {bitmap.width/3, bitmap.rows};

				if (font.next_char_position.x + char_size.x >= font.atlas_size.x) {
					font.next_char_position.x = 0;
					font.next_char_position.y += font.current_row_height;
					font.current_row_height = 0;
				}
					
				if ((font.next_char_position.y + char_size.y > font.atlas_size.y) || !font.atlas_data) {
					if (font.atlas_data) {
						font.atlas_size *= 2;
						FREE(current_allocator, font.atlas_data);
					} else {
						font.atlas_size = V2u(font.size * 16);
					}
					font.atlas_data = ALLOCATE(u8, current_allocator, font.atlas_size.x * font.atlas_size.y * 3);
					memset(font.atlas_data, 0, font.atlas_size.x * font.atlas_size.y * 3);

					font.next_char_position = {};
					font.current_row_height = 0;

					new_buckets.clear();
					for (auto &[bucket_index, bucket] : font.char_buckets) {
						new_buckets.add(&bucket);
					}

					goto retry_buckets;
				}

				//
				// NOTE bitmap.width is in bytes not pixels
				//
				bucket.chars[char_index].position = font.next_char_position;
				bucket.chars[char_index].size = char_size;
				bucket.chars[char_index].offset = {slot->bitmap_left, slot->bitmap_top};
				bucket.chars[char_index].advance = {slot->advance.x >> 6, slot->advance.y >> 6};

				//
				// Add new rows
				//

				for (u32 y = 0; y < bitmap.rows; ++y) {
					memcpy(
						font.atlas_data + ((font.next_char_position.y + y)*font.atlas_size.x + font.next_char_position.x) * 3,
						bitmap.buffer + y*bitmap.pitch,
						bitmap.width
					);
				}
				font.next_char_position.x += char_size.x;
				font.current_row_height = max(font.current_row_height, char_size.y);
			}
		}
		
		font.collection->update_atlas(font.texture, font.atlas_data, font.atlas_size);
	}
}

inline SizedFont &get_font_at_size(FontCollection &fonts, u32 size) {
	auto found = fonts.size_to_font.find(size);
	if (found != fonts.size_to_font.end()) {
		return found->second;
	}

	SizedFont &font = fonts.size_to_font[size];

	font.collection = &fonts;

	font.size = size;

	font.texture = font.collection->create_atlas();

	return font;
}

inline FontCollection init_font(FT_Library library, Span<Span<utf8>> font_paths) {
	FontCollection result = {};

	for (auto &path : font_paths) {
		auto file = read_entire_file(path);
		result.files.add(file);

		FT_Face face;
		auto error = FT_New_Memory_Face(library, file.data, file.size, 0, &face);
		if (error) {
			invalid_code_path("FT_New_Face failed: %", FT_Error_String(error));
		}

		face->glyph->format = FT_GLYPH_FORMAT_BITMAP;
		result.faces.add(face);
	}

	return result;
}

inline aabb<v2s> get_text_bounds(Span<utf8> text, SizedFont &font, bool min_at_zero = false) {
	ensure_all_chars_present(text, font);

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
		auto ch = get_char_and_advance_utf8(current_char);

		auto d = get_char_info(ch, font);

		if (ch == '\n') {
			pos.x = 0;
			pos.x += font.size;
			continue;
		}


		v2s position_min = {pos.x + d.offset.x, pos.y + font.size - d.offset.y};
		v2s position_max = position_min + (v2s)d.size;
		result.max = max(result.max, position_max);
		result.min = min(result.min, position_min);

		pos.x += d.advance.x;
	}

	return result;
}

struct PlacedChar {
	aabb<v2f> position;
	aabb<v2f> uv;
};

inline List<PlacedChar> place_text(Span<utf8> text, SizedFont &font) {
	List<PlacedChar> result;
	
	ensure_all_chars_present(text, font);

	f32 posX = 0;
	f32 posY = 0;
	
	auto current_char = text.data;
	while (current_char < text.end()) {
		auto ch = get_char_and_advance_utf8(current_char);

		auto d = get_char_info(ch, font);

		if (ch == '\n') {
			posX = 0;
			posY += font.size;
			continue;
		}

		PlacedChar c;

		c.position.min = {posX + d.offset.x, posY + font.size - d.offset.y};
		c.position.max = c.position.min + (v2f)d.size;

		c.uv.min = (v2f)d.position / (v2f)font.atlas_size;
		c.uv.max = c.uv.min + (v2f)d.size / (v2f)font.atlas_size;

		result.add(c);

		posX += d.advance.x;
	}

	return result;
}

}

#endif