#pragma once

#include "IC_Feature.h"
#include "FeatureSelector.h"
#include "XML_Collection.h"

#include <memory>

namespace S100
{
    class IC_FeatureDerivation : public xmlParseable
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
        std::shared_ptr<IC_Feature> FeatureRef;

        void GetContents(pugi::xml_node& node) override;
    };
}
