/*

	Quite OK Image Format
	https://github.com/phoboslab
	https://github.com/phoboslab/qoi

*/

#pragma once
#include "common.h"
#include "math.h"
#include "string.h"

namespace tl {
namespace qoi {

struct Image {
	v4u8 *pixels = 0;
	v2u size = {};
	Allocator allocator = TL_GET_CURRENT(allocator);
};

TL_API void free(Image &image);

enum class DecodeError {
	not_enough_data,
	bad_magic,
	dimensions_too_big,
	not_supported,
	bad_data,
	bad_end_padding,
	too_much_data,
};

inline umm append(StringBuilder &builder, DecodeError error) {
	switch (error) {
		case DecodeError::not_enough_data:    return append(builder, "not enough data");
		case DecodeError::bad_magic:          return append(builder, "not a .qoi file");
		case DecodeError::dimensions_too_big: return append(builder, "dimensions are too big");
		case DecodeError::not_supported:      return append(builder, "not supported");
		case DecodeError::bad_data:           return append(builder, "invalid data");
		case DecodeError::bad_end_padding:    return append(builder, "data is improperly padded at the end");
		case DecodeError::too_much_data:      return append(builder, "data remaining after decoding");
	}
	return append(builder, "(unknown tl::qoi::DecodeError)");
}
struct DecodeOptions {
	bool ignore_extra_data = false;
};

TL_API Result<Image, DecodeError> decode(Span<u8> bytes, DecodeOptions options = {});

enum class EncodeError {
};

inline umm append(StringBuilder &builder, EncodeError error) {
	switch (error) {
	}
	return append(builder, "(unknown tl::qoi::EncodeError)");
}
struct EncodeOptions {
};

TL_API Result<List<u8>, EncodeError> encode(v4u8 *pixels, v2u size, EncodeOptions options = {});

}
}

#ifdef TL_IMPL

#include "bits.h"

namespace tl {
namespace qoi {

#pragma pack(push, 1)
struct Header {
	u32 magic;
	u32 width;
	u32 height;
	u8 channels;
	u8 colorspace;
};
#pragma pack(pop)

static constexpr u8 QOI_OP_INDEX = 0x00; /* 00xxxxxx */
static constexpr u8 QOI_OP_DIFF  = 0x40; /* 01xxxxxx */
static constexpr u8 QOI_OP_LUMA  = 0x80; /* 10xxxxxx */
static constexpr u8 QOI_OP_RUN   = 0xc0; /* 11xxxxxx */
static constexpr u8 QOI_OP_RGB   = 0xfe; /* 11111110 */
static constexpr u8 QOI_OP_RGBA  = 0xff; /* 11111111 */

static u32 hash(v4u8 p) {
	return (p.x*3 + p.y*5 + p.z*7 + p.w*11) & 63;
}

Result<Image, DecodeError> decode(Span<u8> bytes, DecodeOptions options) {
	#define read(Type, name)                        \
		if (bytes.count < sizeof(Type))             \
			return DecodeError::not_enough_data;    \
		Type name = *(Type *)bytes.data;            \
		bytes.set_begin(bytes.data + sizeof(Type)); \

	#define read_into(name)                         \
		if (bytes.count < sizeof(name))             \
			return DecodeError::not_enough_data;    \
		name = *(decltype(name) *)bytes.data;       \
		bytes.set_begin(bytes.data + sizeof(name)); \

	read(Header, header);

	if (header.magic != 0x66696f71) {
		return DecodeError::bad_magic;
	}

	header.width = reverse_bytes(header.width);
	header.height = reverse_bytes(header.height);

	if (header.width >= 65536 || header.height >= 65536) {
		return DecodeError::dimensions_too_big;
	}

	if (header.channels != 4) {
		TL_GET_CURRENT(logger).error("decoding images with {} channels is not implemented", header.channels);
		return DecodeError::not_supported;
	}

	Image result = {};
	result.size = {header.width, header.height};
	result.pixels = result.allocator.allocate_uninitialized<v4u8>(result.size.x * result.size.y);

	v4u8 p = {0,0,0,255};

	v4u8 seen[64] = {};

	v4u8 *result_cursor = result.pixels;
	v4u8 *end_of_result = result.pixels + (result.size.x * result.size.y);

	while (result_cursor != end_of_result) {
		read(u8, op);
		switch (op) {
			case QOI_OP_RGB: {
				read_into(p.xyz);
				*result_cursor++ = p;
				seen[hash(p)] = p;
				break;
			}
			case QOI_OP_RGBA: {
				read_into(p);
				*result_cursor++ = p;
				seen[hash(p)] = p;
				break;
			}
			default: {
				switch (op & 0xc0) {
					case QOI_OP_INDEX: {
						p = seen[op];
						*result_cursor++ = p;
						seen[hash(p)] = p;
						break;
					}
					case QOI_OP_DIFF: {
						p.x += ((op >> 4) & 3) - 2;
						p.y += ((op >> 2) & 3) - 2;
						p.z += (op & 3) - 2;
						*result_cursor++ = p;
						seen[hash(p)] = p;
						break;
					}
					case QOI_OP_LUMA: {
						u32 gd = (op & 63) - 32;
						read(u8, b2);
						u32 dr_dg = ((b2 >> 4) & 15) - 8;
						u32 db_dg = (b2 & 15) - 8;

						p.x += gd + dr_dg;
						p.y += gd;
						p.z += gd + db_dg;
						
						*result_cursor++ = p;
						seen[hash(p)] = p;
						break;
					}
					case QOI_OP_RUN: {
						u32 run = (op & 63) + 1;
						while (run--) {
							*result_cursor++ = p;
						}
						break;
					}
				}
				break;
			}
		}
	}

	read(u64, end_padding);
	if (end_padding != 0x01'00'00'00'00'00'00'00) {
		return DecodeError::bad_end_padding;
	}

	if (!options.ignore_extra_data) {
		if (bytes.count != 0) {
			return DecodeError::too_much_data;
		}
	}

	return result;

	#undef read
}
Result<List<u8>, EncodeError> encode(v4u8 *pixels, v2u size, EncodeOptions options) {
	StringBuilder builder;
	defer { free(builder); };

	auto write = Combine {
		[&](Header value) { append_bytes(builder, value); },
		[&](u8     value) { append_bytes(builder, value); },
		[&](v3u8   value) { append_bytes(builder, value); },
		[&](v4u8   value) { append_bytes(builder, value); },
	};

	Header header = {
		.magic = 0x66696f71,
		.width = reverse_bytes(size.x),
		.height = reverse_bytes(size.y),
		.channels = 4,
		.colorspace = 1,
	};

	write(header);
	
	v4u8 seen[64] = {};

	auto cursor = pixels;
	auto end = pixels + size.x * size.y;

	v4u8 p = {0,0,0,255};

	u32 run = 0;

	while (cursor != end) {
		v4u8 c = *cursor++;

		if (all(c == p)) {
			++run;
			if (run == 62 || cursor == end) {
				write(QOI_OP_RUN | (run - 1));
				run = 0;
			}
		} else {
			if (run > 0) {
				write(QOI_OP_RUN | (run - 1));
				run = 0;
			}

			u32 index = hash(c);

			if (all(seen[index] == c)) {
				write(QOI_OP_INDEX | index);
			}
			else {
				seen[index] = c;

				if (c.w == p.w) {
					s8 vr = c.x - p.x;
					s8 vg = c.y - p.y;
					s8 vb = c.z - p.z;

					s8 vg_r = vr - vg;
					s8 vg_b = vb - vg;

					if (
						vr > -3 && vr < 2 &&
						vg > -3 && vg < 2 &&
						vb > -3 && vb < 2
					) {
						write(QOI_OP_DIFF | (vr + 2) << 4 | (vg + 2) << 2 | (vb + 2));
					}
					else if (
						vg_r >  -9 && vg_r <  8 &&
						vg   > -33 && vg   < 32 &&
						vg_b >  -9 && vg_b <  8
					) {
						write(QOI_OP_LUMA     | (vg   + 32));
						write((vg_r + 8) << 4 | (vg_b +  8));
					}
					else {
						write(QOI_OP_RGB);
						write(c.xyz);
					}
				}
				else {
					write(QOI_OP_RGBA);
					write(c);
				}
			}
		}
		p = c;
	}
	
	append_bytes(builder, 0x01'00'00'00'00'00'00'00);

	return to_string(builder);

	#undef write
}
void free(Image &image) {
	image.allocator.free(image.pixels, image.size.x * image.size.y);
	image = {};
}

}
}

#endif