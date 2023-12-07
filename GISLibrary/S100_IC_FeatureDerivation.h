#pragma once

#include "S100_IC_Feature.h"
#include "FeatureSelector.h"
#include "XML_Collection.h"

#include <memory>

namespace S100
{
    class S100_IC_FeatureDerivation : public xmlParseable
    {
    public:
        std::string Identifier;
        dataProduct PrimaryProduct;
        std::string PrimaryFeatureCode;
        std::shared_ptr<FeatureSelector> PrimarySelector;
        dataProduct SecondaryProduct;
        std::string SecondaryFeatureCode;
        std::shared_ptr<FeatureSelector> SecondarySelector;
        dataProduct OutputProduct;
        std::string OutputFeatureCode;
        std::shared_ptr<S100_IC_Feature> FeatureRef;

        void GetContents(pugi::xml_node& node) override;
    };
}
