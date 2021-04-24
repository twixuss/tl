#pragma once
#include "catalog.h"

namespace TL {

#ifndef TL_TEXTURE_CATALOG_ENTRY
#error TL_TEXTURE_CATALOG_ENTRY must be defined before including "texture_catalog.h"
#endif

using TextureCatalog = Catalog<TL_TEXTURE_CATALOG_ENTRY>;

void init(TextureCatalog &catalog, Span<utf8> directory, Span<utf8> fallback_path) {
	catalog.entry_string = "Texture"s;
	init_catalog(catalog, directory, fallback_path);
}

}
