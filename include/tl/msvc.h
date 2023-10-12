#pragma once
#include <tl/string.h>
#include <tl/file.h>
#include <tl/process.h>

namespace tl {

inline List<utf8> locate_visual_studio() {
	auto result = (List<utf8>)start_process_and_get_output(u8"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -property installationPath"s);
	result.count -= 2; // Remove \r\n
	return result;
}

inline List<utf8> locate_visual_c(Span<utf8> visual_studio_path) {
	return format(u8"{}\\VC", visual_studio_path);
}

inline List<utf8> locate_msvc(Span<utf8> visual_c_directory) {
	auto original_allocator = current_allocator;
	scoped(temporary_allocator);

	auto found_builds = get_items_in_directory(visual_c_directory);
	if (!found_builds.count) {
		return {};
	}

	return aformat(original_allocator, u8"{}{}", visual_c_directory, found_builds.back().name);
}

inline List<utf8> locate_wkits() {
	auto original_allocator = current_allocator;
	scoped(temporary_allocator);

	Span<utf8> path = u8"C:\\Program Files (x86)\\Windows Kits\\"s;

	auto find_version = [&]() -> Span<utf8> {
		Span<utf8> supported_versions[] = {
			u8"10"s,
			u8"8.1"s,
			u8"7"s,
		};
		auto found_versions = get_items_in_directory(path);
		for (auto supported_version : supported_versions) {
			for (auto found_version : found_versions) {
				if (found_version.kind == FileItem_directory) {
					auto name = found_version.name;
					if (name == supported_version) {
						return format(u8"{}{}\\Lib\\", path, name);
					}
				}
			}
		}
		return {};
	};

	path = find_version();
	if (!path.data) {
		return {};
	}

	auto found_builds = get_items_in_directory(path);
	if (!found_builds.count) {
		return {};
	}
	// NOTE: no slash at the end because link.exe does not understand that
	return aformat(original_allocator, u8"{}{}", path, found_builds.back().name);
}

}
