#pragma once
#include "json.h"
#include "file.h"

namespace TL {

namespace Glb {

enum ComponentType {
	ComponentType_s8 = 5120,
	ComponentType_u8 = 5121,
	ComponentType_s16 = 5122,
	ComponentType_u16 = 5123,
	ComponentType_u32 = 5125,
	ComponentType_f32 = 5126,
};

}

struct Vertex {
	v3f position;
	v3f normal;
};

struct Mesh {
	List<Vertex> vertices;
	List<u32> indices;
};

Mesh parse_glb(char const *path) {
	using namespace Glb;

#pragma pack(push, 1)
	struct Header {
		u32 magic;
		u32 version;
		u32 length;
	};
	struct Chunk {
		u32 length;
		u32 type;
		u8 data[];
	};
#pragma pack(pop)

	auto data = read_entire_file(path);
	defer { free(data); };

	Header *header = (Header *)data.begin();
	assert(header->magic == 'FTlg');

	Chunk *jsonChunk = (Chunk *)(header + 1);

	auto json = Json::parse({(char *)jsonChunk->data, (umm)jsonChunk->length});

	Chunk *binaryChunk = (Chunk *)((char *)jsonChunk->data + jsonChunk->length);

	auto &accessors   = json["accessors"];
	auto &bufferViews = json["bufferViews"];
	auto &buffers     = json["buffers"];

	auto &primitives  = json["meshes"][0]["primitives"][0];
	auto &attributes  = primitives["attributes"];
	auto &positionAccessor = accessors[attributes["POSITION"]];
	auto &normalAccessor   = accessors[attributes["NORMAL"]];
	auto &indexAccessor    = accessors[primitives["indices"]];

	assert(equals(indexAccessor["type"].string.view, "SCALAR"));
	ComponentType indexType = (ComponentType)indexAccessor["componentType"].number.value;

	u32 vertexCount = positionAccessor["count"].number.value;
	u32 indexCount  =    indexAccessor["count"].number.value;
	v3f *positions = (v3f *)(binaryChunk->data + (u32)bufferViews[positionAccessor["bufferView"]]["byteOffset"].number.value);
	v3f *normals   = (v3f *)(binaryChunk->data + (u32)bufferViews[  normalAccessor["bufferView"]]["byteOffset"].number.value);

	Mesh result;
	
	result.vertices.reserve(vertexCount);
	for (u32 i = 0; i < vertexCount; ++i) {
		result.vertices.push_back({positions[i], normals[i]});
	}

	result.indices.reserve(indexCount);

	void *indices_data = binaryChunk->data + (u32)bufferViews[indexAccessor["bufferView"]]["byteOffset"].number.value;
	switch (indexType) {
		case ComponentType_u32: {
			u32 *indices = (u32 *)(indices_data); 
			for (u32 i = 0; i < indexCount; ++i) {
				result.indices.push_back(indices[i]);
			}
			break;
		}
		case ComponentType_u16: {
			u16 *indices = (u16 *)(indices_data);
			for (u32 i = 0; i < indexCount; ++i) {
				result.indices.push_back(indices[i]);
			}
			break;
		}
	}

	return result;
}
}