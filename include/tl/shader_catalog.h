#pragma once
#include "catalog.h"

namespace TL {

#ifndef TL_SHADER_CATALOG_ENTRY
#error TL_SHADER_CATALOG_ENTRY must be defined before including "shader_catalog.h"
#endif

using ShaderCatalog = Catalog<TL_SHADER_CATALOG_ENTRY>;

void init(ShaderCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_path) {
	catalog.entry_string = "Shader"s;
	init_catalog(catalog, directory, fallback_path);
}

}
