#pragma once
#include "catalog.h"

namespace TL {

#ifndef TL_SHADER_CATALOG_ENTRY
#error TL_SHADER_CATALOG_ENTRY must be defined before including "shader_catalog.h"
#endif

using ShaderCatalog = Catalog<TL_SHADER_CATALOG_ENTRY>;

TL_API void init(ShaderCatalog &catalog, Span<filechar> directory);

#ifdef TL_IMPL

void init(ShaderCatalog &catalog, Span<filechar> directory) {
	catalog.entry_string = "Shader"s;
	init_catalog(catalog, directory);
}

#endif

}
