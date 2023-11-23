#pragma once
#include <map>


namespace S100 {
	enum class S100_CatalogueScope
	{
		featureCatalogue = 1 ,
		portrayalCatalogue,
		interoperabilityCatalogue
	};


    std::string S100_CatalogueScopeToString(S100_CatalogueScope category) {
        switch (category) {
        case S100_CatalogueScope::featureCatalogue: return "featureCatalogue";
        case S100_CatalogueScope::portrayalCatalogue: return "portrayalCatalogue";
        case S100_CatalogueScope::interoperabilityCatalogue: return "interoperabilityCatalogue";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    S100_CatalogueScope S100_CatalogueScopeFromString(const std::string& categoryName) {
        if (categoryName == "featureCatalogue") return S100_CatalogueScope::featureCatalogue;
        if (categoryName == "portrayalCatalogue") return S100_CatalogueScope::portrayalCatalogue;
        if (categoryName == "interoperabilityCatalogue") return S100_CatalogueScope::interoperabilityCatalogue;
        throw std::invalid_argument("Unknown category name");
    }
}