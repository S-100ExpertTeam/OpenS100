#include "stdafx.h"
#include "CatalogueScope.h"

namespace S100 {
    std::string S100_CatalogueScopeToString(CatalogueScope category) {
        switch (category) {
        case CatalogueScope::featureCatalogue: return "featureCatalogue";
        case CatalogueScope::portrayalCatalogue: return "portrayalCatalogue";
        case CatalogueScope::interoperabilityCatalogue: return "interoperabilityCatalogue";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    CatalogueScope S100_CatalogueScopeFromString(const std::string& categoryName) {
        if (categoryName == "featureCatalogue") return CatalogueScope::featureCatalogue;
        if (categoryName == "portrayalCatalogue") return CatalogueScope::portrayalCatalogue;
        if (categoryName == "interoperabilityCatalogue") return CatalogueScope::interoperabilityCatalogue;
        throw std::invalid_argument("Unknown category name");
    }
}
