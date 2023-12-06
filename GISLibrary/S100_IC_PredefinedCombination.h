#pragma once

#include "S100_IC_DisplayPlane.h"
#include "S100_IC_SuppressedFeatureLayer.h"
#include "dataProduct.h"
#include "S100_IC_FeatureDerivation.h"
#include "XML_Collection.h"

#include <string>
#include <memory>

namespace S100 {
    class S100_IC_PredefinedCombination : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string Name;
        std::string Description;
        std::string UseConditions;
        std::shared_ptr<int> InteroperabilityLevel;
        std::vector<dataProduct> IncludedProduct;
        XmlCollection<S100_IC_DisplayPlane> DisplayPlaneRef;
        XmlCollection<S100_IC_FeatureDerivation> DerivedFeatures;
        XmlCollection<S100_IC_SuppressedFeatureLayer> SuppressedFeatureLayers;

        void GetContents(pugi::xml_node& node) override;
    };
}
