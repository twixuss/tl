#pragma once
#include "opengl.h"
#include "file.h"
#include "string.h"
#include "std_hash.h"
#include "buffer.h"
#include "console.h"
#include <unordered_map>

namespace TL {

struct TextureCatalog {
#ifdef TL_TEXTURE_CATALOG_ENTRY
	struct Entry : TL_TEXTURE_CATALOG_ENTRY {
#else
	struct Entry {
#endif
		List<utf8> path;
		FileTracker tracker;
		bool initialized;
	};
	Allocator allocator;
	std::unordered_map<Span<utf8>, Entry> textures;
	ListList<utf8> texture_file_names;
	Entry *fallback_texture = 0;
	void (*update_texture)(Entry &) = 0;
	bool (*texture_valid)(Entry &) = 0;
};

inline void free(TextureCatalog &catalog) {
	for (auto &[name, texture] : catalog.textures) {
		free(texture.path);
		free(texture.tracker);
	}
	free(catalog.texture_file_names);
}

TL_API TextureCatalog::Entry *find(TextureCatalog &catalog, Span<utf8> name);

TL_API TextureCatalog::Entry &add_file(TextureCatalog &catalog, Span<utf8> directory, Span<utf8> full_name);
TL_API void init(TextureCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_texture_name);
TL_API bool update(TextureCatalog &catalog);

#ifdef TL_IMPL

TextureCatalog::Entry *find(TextureCatalog &catalog, Span<utf8> name) {
	auto result = catalog.textures.find(name);
	if (result != catalog.textures.end()) {
		if (!result->second.initialized) {
			result->second.initialized = true;
			catalog.update_texture(result->second);
		}
		if (catalog.texture_valid(result->second)) {
			return &result->second;
		} else {
			print("Texture '%' is invalid\n", name);
		}
	} else {
		print("Texture '%' not found\n", name);
	}
	return catalog.fallback_texture;
}

TextureCatalog::Entry &add_file(TextureCatalog &catalog, Span<utf8> directory, Span<utf8> full_name) {
	using namespace OpenGL;

	auto file_name = full_name;
	file_name.size = find(file_name, u8'.') - file_name.data;

	auto &texture = catalog.textures[file_name];

	texture.path = (List<utf8>)concatenate(directory, '/', full_name, '\0');
	texture.tracker = create_file_tracker(texture.path, [&](FileTracker &tracker) {
		if (texture.initialized) {
			catalog.update_texture(texture);
		}
	});
	return texture;
}

void init(TextureCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_texture_path) {
	catalog.allocator = current_allocator;
	catalog.texture_file_names = get_files_in_directory(directory);
	catalog.fallback_texture = &add_file(catalog, directory, fallback_texture_path);
	for (auto &full_name : catalog.texture_file_names) {
		if (full_name == as_span(fallback_texture_path))
			continue;
		add_file(catalog, directory, full_name);
	}
	update(catalog);
}

//
// Updates file trackers in the catalog and reloads modified textures.
// Should be used only for development.
//
bool update(TextureCatalog &catalog) {
	bool any_updated = false;
	for (auto &[name, texture] : catalog.textures) {
		any_updated |= update_file_tracker(texture.tracker);
	}
	return any_updated;
}

#endif

}
