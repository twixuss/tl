#pragma once
#include "file.h"
#include "string.h"
#include "std_hash.h"
#include "console.h"
#include <unordered_map>

namespace TL {

template <class EntryAddition>
struct Catalog {
	struct Entry : EntryAddition {
		List<utf8> path;
		List<utf8> name;
		FileTracker tracker;
		bool initialized;
	};
	Allocator allocator;
	std::unordered_map<Span<utf8>, Entry> entries;
	ListList<utf8> file_names;
	Entry *fallback = 0;
	void (*update_entry)(Entry &) = 0;
	bool (*entry_valid)(Entry &) = 0;
	void (*free_entry)(Entry &) = 0;
	Span<char> entry_string;
};

template <class T>
inline void free(Catalog<T> &catalog) {
	for (auto &[name, entry] : catalog.entries) {
		free(entry.path);
		free(entry.tracker);
		catalog.free_entry(entry);
	}
	free(catalog.file_names);
}

template <class T>
Catalog<T>::Entry *find(Catalog<T> &catalog, Span<utf8> name) {
	auto result = catalog.entries.find(name);
	if (result != catalog.entries.end()) {
		if (!result->second.initialized) {
			result->second.initialized = true;
			catalog.update_entry(result->second);
		}
		if (catalog.entry_valid(result->second)) {
			return &result->second;
		} else {
			print("% '%' is invalid\n", catalog.entry_string, name);
		}
	} else {
		print("% '%' not found\n", catalog.entry_string, name);
	}
	return catalog.fallback;
}

template <class T>
Catalog<T>::Entry &add_file(Catalog<T> &catalog, Span<utf8> directory, Span<utf8> full_name) {
	using namespace OpenGL;

	auto file_name = full_name;
	file_name.size = find(file_name, u8'.') - file_name.data;

	auto &entry = catalog.entries[file_name];

	entry.name = file_name;
	entry.path = (List<utf8>)concatenate(directory, '/', full_name, '\0');
	entry.tracker = create_file_tracker(entry.path, [&](FileTracker &tracker) {
		if (entry.initialized) {
			catalog.update_entry(entry);
		}
	});
	return entry;
}

template <class T>
void init_catalog(Catalog<T> &catalog, Span<utf8> directory) {
	catalog.allocator = current_allocator;
	catalog.file_names = get_files_in_directory(directory);
	for (auto &full_name : catalog.file_names) {
		auto &entry = add_file(catalog, directory, full_name);
	}
	update(catalog);
}

//
// Updates file trackers in the catalog and reloads modified files.
// Should be used only for development.
//
template <class T>
bool update(Catalog<T> &catalog) {
	bool any_updated = false;
	for (auto &[name, entry] : catalog.entries) {
		any_updated |= update_file_tracker(entry.tracker);
	}
	return any_updated;
}

}
