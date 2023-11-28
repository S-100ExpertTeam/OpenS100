#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class S100_CatalogueScope {
        featureCatalogue = 1,
        portrayalCatalogue,
        interoperabilityCatalogue
    };

    std::string S100_CatalogueScopeToString(S100_CatalogueScope category);
    S100_CatalogueScope S100_CatalogueScopeFromString(const std::string& categoryName);
}
