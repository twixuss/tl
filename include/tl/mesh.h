#pragma once
#include "json.h"
#include "file.h"

#pragma warning(push)
#pragma warning(disable: 4061) // incomplete switch

namespace TL {

namespace Glb {

enum ComponentType {
	ComponentType_s8  = 5120,
	ComponentType_u8  = 5121,
	ComponentType_s16 = 5122,
	ComponentType_u16 = 5123,
	ComponentType_u32 = 5125,
	ComponentType_f32 = 5126,
};

#pragma pack(push, 1)
struct Header {
	u32 magic;
	u32 version;
	u32 length;
};
struct Chunk {
	u32 length;
	u32 type;
};

forceinline u8 *chunk_data(Chunk *chunk) {
	return (u8 *)(chunk + 1);
}

#pragma pack(pop)

}

struct CommonVertex {
	v3f position;
	v3f normal;
	v4f color;
};

struct Mesh {
	List<CommonVertex> vertices;
	List<u32> indices;
};

TL_API Mesh parse_glb_from_memory(Span<u8> memory);

inline Optional<Mesh> parse_glb_from_file(Span<filechar> path) {
	auto memory = with(temporary_allocator, read_entire_file(path));
	if (memory.data) {
		return parse_glb_from_memory(memory);
	}
	return {};
}


}

#ifdef TL_IMPL

namespace TL {

Mesh parse_glb_from_memory(Span<u8> memory) {
	using namespace Glb;

	Header *header = (Header *)memory.data;
	assert(header->magic == 'FTlg');

	Chunk *json_chunk = (Chunk *)(header + 1);
	auto json_chunk_data = chunk_data(json_chunk);
	auto json_chunk_span = Span((utf8 *)json_chunk_data, (umm)json_chunk->length);

	auto json = Json::parse(json_chunk_span);
	defer { free(json); };

	Chunk *binary_chunk = (Chunk *)((char *)json_chunk_data + json_chunk->length);
	auto binary_chunk_data = chunk_data(binary_chunk);

	auto accessors    = json.member(u8"accessors"s);
	auto buffer_views = json.member(u8"bufferViews"s);
	// auto buffers      = json.member(u8"buffers"];

	auto meshes     = json.member(u8"meshes"s);
	auto first_mesh = meshes->index(0);

	auto primitives      = first_mesh->member(u8"primitives"s);
	auto first_primitive = primitives->index(0);

	auto index_accessor_index    = first_primitive->member(u8"indices"s);
	auto index_accessor          = accessors->index(index_accessor_index->number);
	auto index_buffer_view_index = index_accessor->member(u8"bufferView"s);
	auto index_buffer_view       = buffer_views->index(index_buffer_view_index->number);

	assert(index_accessor->member(u8"type"s)->string == u8"SCALAR"s);
	ComponentType indexType = (ComponentType)index_accessor->member(u8"componentType"s)->number;

	auto attributes = first_primitive->member(u8"attributes"s);

	auto position_attribute_index   = attributes->member(u8"POSITION"s);
	auto position_accessor          = accessors->index(position_attribute_index->number);
	auto position_buffer_view_index = position_accessor->member(u8"bufferView"s);
	auto position_buffer_view       = buffer_views->index(position_buffer_view_index->number);
	v3f *positions                  = (v3f *)(binary_chunk_data + (umm)position_buffer_view->member(u8"byteOffset"s)->number);


	v3f *normals = 0;
	auto normal_attribute_index = attributes->member(u8"NORMAL"s);
	if (normal_attribute_index) {
		auto normal_accessor          = accessors->index(normal_attribute_index->number);
		auto normal_buffer_view_index = normal_accessor->member(u8"bufferView"s);
		auto normal_buffer_view       = buffer_views->index(normal_buffer_view_index->number);
		normals                       = (v3f *)(binary_chunk_data + (umm)normal_buffer_view->member(u8"byteOffset"s)->number);
	}


	v4f *colors = 0;
	auto color_attribute_index = attributes->member(u8"COLOR_0"s);
	if (color_attribute_index) {
		auto color_accessor          = accessors->index(color_attribute_index->number);
		auto color_component_type    = (s64)color_accessor->member(u8"componentType"s)->number;
		auto color_count             = (s64)color_accessor->member(u8"count"s)->number;
		auto color_buffer_view_index = color_accessor->member(u8"bufferView"s);
		auto color_type              = color_accessor->member(u8"type"s)->string;
		auto color_normalized        = color_accessor->member(u8"normalized"s)->boolean;
		auto color_buffer_view       = buffer_views->index(color_buffer_view_index->number);

		auto colors_data = binary_chunk_data + (umm)color_buffer_view->member(u8"byteOffset"s)->number;

		scoped_allocator(temporary_allocator);
		if (color_type == u8"VEC4"s) {
			switch ((s64)color_component_type) {
				case Glb::ComponentType_f32: {
					colors = (v4f *)colors_data;
					break;
				}
				case Glb::ComponentType_u16: {
					auto source_colors = (u16 *)colors_data;

					List<v4f> colors_list;
					colors_list.reserve(color_count);

					for (u32 i = 0; i < color_count; ++i) {
						v4f c;
						c.x = source_colors[0];
						c.y = source_colors[1];
						c.z = source_colors[2];
						c.w = source_colors[3];

						if (color_normalized) {
							c /= max_value<u16>;
						}

						source_colors += 4;
						colors_list.add(c);
					}

					colors = colors_list.data;
					break;
				}
			}
		}
		if (!colors) {
			print("Color has unsupported component_type=% and type=%\n", (s64)color_component_type, color_type);
		}
	}


	u32 vertex_count = (u32)position_accessor->member(u8"count"s)->number;
	u32 index_count  = (u32)   index_accessor->member(u8"count"s)->number;

	Mesh mesh;

	mesh.vertices.reserve(vertex_count);
	for (u32 i = 0; i < vertex_count; ++i) {
		CommonVertex v;
		v.position = positions[i];
		if (normals) v.normal = normals[i];
		if (colors ) v.color  = colors [i];
		mesh.vertices.add(v);
	}

	mesh.indices.reserve(index_count);

	void *indices_data = binary_chunk_data + (u32)index_buffer_view->member(u8"byteOffset"s)->number;
	switch (indexType) {
		case ComponentType_u32: {
			u32 *indices = (u32 *)(indices_data);
			for (u32 i = 0; i < index_count; ++i) {
				mesh.indices.add(indices[i]);
			}
			break;
		}
		case ComponentType_u16: {
			u16 *indices = (u16 *)(indices_data);
			for (u32 i = 0; i < index_count; ++i) {
				mesh.indices.add(indices[i]);
			}
			break;
		}
		default: {
			invalid_code_path();
			return {};
		}
	}

	return mesh;
}

}

#endif

#pragma warning(pop)
