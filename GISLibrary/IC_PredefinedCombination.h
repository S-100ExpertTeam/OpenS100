#pragma once

#include "IC_DisplayPlane.h"
#include "IC_SuppressedFeatureLayer.h"
#include "dataProduct.h"
#include "IC_FeatureDerivation.h"
#include "XML_Collection.h"

#include <string>
#include <memory>

namespace S100 {
    class IC_PredefinedCombination : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string Name;
        std::string Description;
        std::string UseConditions;
        std::shared_ptr<int> InteroperabilityLevel;
        std::vector<dataProduct> IncludedProduct;
        XmlCollection<IC_DisplayPlane> DisplayPlaneRef;
        XmlCollection<IC_FeatureDerivation> DerivedFeatures;
        XmlCollection<IC_SuppressedFeatureLayer> SuppressedFeatureLayers;

        void GetContents(pugi::xml_node& node) override;
    };
}
