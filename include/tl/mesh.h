#pragma once
#include "json.h"
#include "file.h"
#include "list.h"

namespace tl {

struct CommonVertex {
	v3f position;
	v3f normal;
	v4f tangent;
	v4f color;
	v2f uv;
};

inline void append(StringBuilder &builder, CommonVertex v) {
	append_format(builder, "({}, {}, {})", v.position, v.normal, v.color);
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
	struct Node {
		Span<utf8> name;
		CommonMesh *mesh;
		v3f position;
	};

	List<CommonMesh> meshes;
	List<Node> nodes;

	Node *get_node(Span<utf8> name) {
		return find_if(nodes, [&](Node &node){return node.name == name;});
	}
};

inline void free(Scene3D &scene) {
	for (auto &mesh : scene.meshes) {
		free(mesh.indices);
		free(mesh.vertices);
	}
	free(scene.meshes);

	for (auto &node : scene.nodes) {
	}
	free(scene.nodes);
}

template <class Vertex, class Index>
void calculate_normals(Span<Vertex> vertices, Span<Index> indices) {
	for (auto &vertex : vertices) {
		vertex.normal = {};
	}

	for (u32 i = 0; i < indices.count; i += 3) {
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
