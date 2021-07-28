#pragma once
#include "json.h"
#include "file.h"

#pragma warning(push)
#pragma warning(disable: 4061) // incomplete switch

namespace tl {

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
	v2f uv;
};

inline void append(StringBuilder &builder, CommonVertex v) {
	append_format(builder, "{%, %, %}", v.position, v.normal, v.color);
}

struct CommonMesh {
	List<CommonVertex> vertices;
	List<u32> indices;
};

inline void free(CommonMesh &mesh) {
	free(mesh.vertices);
	free(mesh.indices);
}

struct Scene3D {
	List<utf8> name;

	struct Node {
		Span<utf8> name;
		CommonMesh *mesh;
	};

	List<CommonMesh> meshes;
	List<Node> nodes;

	Node *get_node(Span<utf8> name) {
		return find_if(nodes, [&](Node &node){return node.name == name;});
	}
};

inline void free(Scene3D &scene) {
	free(scene.name);
	for (auto &mesh : scene.meshes) {
		free(mesh.indices);
		free(mesh.vertices);
	}
	free(scene.meshes);

	for (auto &node : scene.nodes) {
	}
	free(scene.nodes);
}

TL_API Scene3D parse_glb_from_memory(Span<u8> memory);

inline Optional<Scene3D> parse_glb_from_file(Span<utf8> path) {
	auto memory = with(temporary_allocator, read_entire_file(to_pathchars(path, true)));
	if (memory.data) {
		auto result = parse_glb_from_memory(memory);
		result.name = as_list(path);
		return result;
	}
	return {};
}

template <class Vertex, class Index>
void calculate_normals(Span<Vertex> vertices, Span<Index> indices) {
	for (auto &vertex : vertices) {
		vertex.normal = {};
	}

	for (u32 i = 0; i < indices.size; i += 3) {
		auto &a = vertices[indices[i + 0]];
		auto &b = vertices[indices[i + 1]];
		auto &c = vertices[indices[i + 2]];
		v3f normal = normalize(cross(b.position - a.position, c.position - a.position));
		a.normal += normal;
		b.normal += normal;
		c.normal += normal;
	}

	for (auto &vertex : vertices) {
		vertex.normal = normalize(vertex.normal);
	}
}

}

#ifdef TL_IMPL

namespace tl {

Scene3D parse_glb_from_memory(Span<u8> memory) {
	Scene3D scene;

	using namespace Glb;

	Header *header = (Header *)memory.data;
	assert(header->magic == 'FTlg');

	constexpr u32 chunk_type_json = 'NOSJ';
	constexpr u32 chunk_type_binary = '\0NIB';

	Chunk *json_chunk = (Chunk *)(header + 1);
	assert(json_chunk->type == chunk_type_json);

	auto json_chunk_data = chunk_data(json_chunk);
	auto json_chunk_span = Span((utf8 *)json_chunk_data, (umm)json_chunk->length);

	auto json = Json::parse(json_chunk_span);
	defer { free(json); };

	Chunk *binary_chunk = (Chunk *)((char *)json_chunk_data + json_chunk->length);
	assert(binary_chunk->type == chunk_type_binary);

	auto binary_chunk_data = chunk_data(binary_chunk);


	auto accessors    = json.member(u8"accessors"s);
	auto buffer_views = json.member(u8"bufferViews"s);
	// auto buffers      = json.member(u8"buffers"];

	auto meshes = json.member(u8"meshes"s);

	for (auto &mesh : meshes->array()) {
		auto primitives      = mesh.member(u8"primitives"s);
		auto first_primitive = primitives->index(0);

		auto index_accessor_index    = first_primitive->member(u8"indices"s);
		auto index_accessor          = accessors->index(index_accessor_index->number());
		auto index_buffer_view_index = index_accessor->member(u8"bufferView"s);
		auto index_buffer_view       = buffer_views->index(index_buffer_view_index->number());
		u32 index_count              = (u32)index_accessor->member(u8"count"s)->number();

		assert(index_accessor->member(u8"type"s)->string() == u8"SCALAR"s);
		ComponentType indexType = (ComponentType)index_accessor->member(u8"componentType"s)->number();

		auto attributes = first_primitive->member(u8"attributes"s);

		auto position_attribute_index   = attributes->member(u8"POSITION"s);
		auto position_accessor          = accessors->index(position_attribute_index->number());
		auto position_buffer_view_index = position_accessor->member(u8"bufferView"s);
		auto position_buffer_view       = buffer_views->index(position_buffer_view_index->number());
		v3f *positions                  = (v3f *)(binary_chunk_data + (umm)position_buffer_view->member(u8"byteOffset"s)->number());
		u32 position_count              = (u32)position_accessor->member(u8"count"s)->number();


		v3f *normals = 0;
		auto normal_attribute_index = attributes->member(u8"NORMAL"s);
		if (normal_attribute_index) {
			auto normal_accessor          = accessors->index(normal_attribute_index->number());
			auto normal_buffer_view_index = normal_accessor->member(u8"bufferView"s);
			auto normal_buffer_view       = buffer_views->index(normal_buffer_view_index->number());
			normals                       = (v3f *)(binary_chunk_data + (umm)normal_buffer_view->member(u8"byteOffset"s)->number());
			u32 normal_count              = (u32)normal_accessor->member(u8"count"s)->number();
			if (normal_count != position_count) {
				print(Print_error, "Failed parsing glb file. Normal count does not match position count.\n");
				return {};
			}
		}

		v2f *uvs = 0;
		auto uv_attribute_index = attributes->member(u8"TEXCOORD_0"s);
		if (uv_attribute_index) {
			auto uv_accessor          = accessors->index(uv_attribute_index->number());
			auto uv_component_type    = (s64)uv_accessor->member(u8"componentType"s)->number();
			auto uv_type              = uv_accessor->member(u8"type"s)->string();
			auto uv_buffer_view_index = uv_accessor->member(u8"bufferView"s);
			auto uv_buffer_view       = buffer_views->index(uv_buffer_view_index->number());
			u32 uv_count              = (u32)uv_accessor->member(u8"count"s)->number();
			if (uv_count != position_count) {
				print(Print_error, "Failed parsing glb file. Normal count does not match position count.\n");
				return {};
			}
			switch (uv_component_type) {
				case Glb::ComponentType_f32: {
					if (uv_type == u8"VEC2"s) {
						uvs = (v2f *)(binary_chunk_data + (umm)uv_buffer_view->member(u8"byteOffset"s)->number());
					} else {
						print(Print_warning, "Only uvs with type vec2 are supported.\n");
					}
					break;
				}
				default: {
					print(Print_warning, "Only uvs with type float32 are supported.\n");
					break;
				}
			}
		}


		v4f *colors = 0;
		auto color_attribute_index = attributes->member(u8"COLOR_0"s);
		if (color_attribute_index) {
			auto color_accessor          = accessors->index(color_attribute_index->number());
			auto color_component_type    = (s64)color_accessor->member(u8"componentType"s)->number();
			auto color_buffer_view_index = color_accessor->member(u8"bufferView"s);
			auto color_type              = color_accessor->member(u8"type"s)->string();
			auto color_normalized        = color_accessor->member(u8"normalized"s)->boolean();
			auto color_buffer_view       = buffer_views->index(color_buffer_view_index->number());
			auto colors_data             = binary_chunk_data + (umm)color_buffer_view->member(u8"byteOffset"s)->number();
			u32 color_count              = (u32)color_accessor->member(u8"count"s)->number();
			if (color_count != position_count) {
				print(Print_error, "Failed parsing glb file. Color count does not match position count.\n");
				return {};
			}

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
						colors_list.allocator = temporary_allocator;
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
				print(Print_error, "Color has unsupported component_type=% and type=%\n", (s64)color_component_type, color_type);
			}
		}


		u32 vertex_count = position_count;

		v4f default_color = {1,1,1,1};
		v3f default_normal = {};
		v2f default_uv = {};

		CommonMesh mesh;

		mesh.vertices.reserve(vertex_count);
		for (u32 i = 0; i < vertex_count; ++i) {
			CommonVertex v = {};
			v.position = positions[i];
			v.normal = normals ? normals[i] : default_normal;
			v.color  = colors  ? colors [i] : default_color;
			v.uv     = uvs     ? uvs    [i] : default_uv;
			mesh.vertices.add(v);
		}

		mesh.indices.reserve(index_count);

		void *indices_data = binary_chunk_data + (u32)index_buffer_view->member(u8"byteOffset"s)->number();
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

		scene.meshes.add(mesh);
	}


	auto nodes = json.member(u8"nodes"s);
	for (auto &node : nodes->array()) {
		scene.nodes.add({
			.name = node.member(u8"name"s)->string(),
			.mesh = &scene.meshes[(u32)node.member(u8"mesh"s)->number()],
		});
	}


	return scene;
}

}

#endif

#pragma warning(pop)
