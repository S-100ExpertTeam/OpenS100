#pragma once

#include "dataProduct.h"
#include "../FeatureCatalog/SpatialPrimitiveType.h"
#include "XML_Collection.h"

namespace S100 {
    class IC_DrawingInstruction : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string FeatureCode;
        dataProduct Product;
        std::vector<SpatialPrimitiveType> GeometryType;
        std::vector<std::string> AttributeCombination;
        int DrawingPriority;
        int ViewingGroup;
        std::string* SubstituteSymbolization;

        void GetContents(pugi::xml_node& node) override;
    };
}
