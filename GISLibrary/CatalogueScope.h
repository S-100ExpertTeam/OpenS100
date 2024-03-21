#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class CatalogueScope {
        featureCatalogue = 1,
        portrayalCatalogue,
        interoperabilityCatalogue
    };

    std::string S100_CatalogueScopeToString(CatalogueScope category);
    CatalogueScope S100_CatalogueScopeFromString(const std::string& categoryName);
}
