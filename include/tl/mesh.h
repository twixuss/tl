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

struct GenericVertex {
	v3f position;
	v3f normal;
};

struct GenericMesh {
	List<GenericVertex> vertices;
	List<u32> indices;
};

TL_API GenericMesh parse_glb_from_memory(Span<u8> memory);

inline GenericMesh parse_glb_from_file(char const *path) {
	using namespace Glb;

	auto data = read_entire_file(path);
	defer { free(data); };

	return parse_glb_from_memory(data);
}


}

#ifdef TL_IMPL

namespace TL {

GenericMesh parse_glb_from_memory(Span<u8> memory) {
	using namespace Glb;

	Header *header = (Header *)memory.data;
	assert(header->magic == 'FTlg');

	Chunk *json_chunk = (Chunk *)(header + 1);
	auto json_chunk_data = chunk_data(json_chunk);

	auto json = Json::parse({(char *)json_chunk_data, (umm)json_chunk->length});

	Chunk *binary_chunk = (Chunk *)((char *)json_chunk_data + json_chunk->length);
	auto binary_chunk_data = chunk_data(binary_chunk);

	auto &accessors    = json["accessors"s];
	auto &buffer_views = json["bufferViews"s];
	// auto &buffers      = json["buffers"];

	auto &primitives  = json["meshes"s][0]["primitives"s][0];
	auto &attributes  = primitives["attributes"s];
	auto &position_accessor = accessors[attributes["POSITION"s]];
	auto &  normal_accessor = accessors[attributes["NORMAL"s]];
	auto &   index_accessor = accessors[primitives["indices"s]];

	auto &position_buffer_view = buffer_views[position_accessor["bufferView"s]];
	auto &  normal_buffer_view = buffer_views[  normal_accessor["bufferView"s]];

	assert(equals(index_accessor["type"s].string.view, "SCALAR"s));
	ComponentType indexType = (ComponentType)index_accessor["componentType"s].number.value;

	u32 vertex_count = (u32)position_accessor["count"s].number.value;
	u32 index_count  = (u32)   index_accessor["count"s].number.value;
	v3f *positions = (v3f *)(binary_chunk_data + (u32)position_buffer_view["byteOffset"s].number.value);
	v3f *normals   = (v3f *)(binary_chunk_data + (u32)  normal_buffer_view["byteOffset"s].number.value);

	GenericMesh mesh;
	
	mesh.vertices.reserve(vertex_count);
	for (u32 i = 0; i < vertex_count; ++i) {
		mesh.vertices.add({positions[i], normals[i]});
	}

	mesh.indices.reserve(index_count);

	void *indices_data = binary_chunk_data + (u32)buffer_views[index_accessor["bufferView"s]]["byteOffset"s].number.value;
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
