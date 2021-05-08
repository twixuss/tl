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
		List<utf8> name;
		FileTracker tracker;
		bool initialized;
		void (*on_update)(Entry &entry);
	};
	Allocator allocator;
	std::unordered_map<Span<utf8>, Entry> entries;
	ListList<filechar> file_names;
	Entry *fallback = 0;
	void (*update_entry)(Entry &) = 0;
	bool (*entry_valid)(Entry &) = 0;
	void (*free_entry)(Entry &) = 0;
	bool load_all_at_start = false;
	Span<char> entry_string;
};

template <class T>
inline void free(Catalog<T> &catalog) {
	for (auto &[name, entry] : catalog.entries) {
		free(entry.tracker);
		catalog.free_entry(entry);
	}
	free(catalog.file_names);
}

template <class T>
void update_entry(Catalog<T> &catalog, typename Catalog<T>::Entry &entry) {
	catalog.update_entry(entry);
	if (entry.on_update)
		entry.on_update(entry);
}

template <class T>
Catalog<T>::Entry *find(Catalog<T> &catalog, Span<utf8> name) {
	auto result = catalog.entries.find(name);
	if (result != catalog.entries.end()) {
		if (!result->second.initialized) {
			result->second.initialized = true;
			update_entry(catalog, result->second);
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
Catalog<T>::Entry &add_file(Catalog<T> &catalog, Span<filechar> directory, Span<filechar> full_name) {
	auto file_name = full_name;
	file_name.size = find(file_name, TL_FILE_STRING('.')) - file_name.data;

	auto name = path_to_utf8(file_name);

	auto &entry = catalog.entries[name];
	entry.name = name;
	entry.tracker = create_file_tracker_steal_path(format(TL_FILE_STRING("%/%\0"s), directory, full_name), [&](FileTracker &tracker) {
		if (entry.initialized) {
			update_entry(catalog, entry);
		}
	});
	if (catalog.load_all_at_start) {
		entry.initialized = true;
		update_entry(catalog, entry);
	}
	return entry;
}

template <class T>
void init_catalog(Catalog<T> &catalog, Span<filechar> directory) {
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
	timed_function();
	bool any_updated = false;
	for (auto &[name, entry] : catalog.entries) {
		any_updated |= update(entry.tracker);
	}
	return any_updated;
}

}
