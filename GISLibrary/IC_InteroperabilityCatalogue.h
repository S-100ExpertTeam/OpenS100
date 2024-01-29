#pragma once
#include "CT_Catalogue.h"
#include "requirementType.h"
#include "IC_DisplayPlane.h"
#include "IC_PredefinedCombination.h"
#include "IC_HybridFeatureCreationRule.h"
#include "dataProduct.h"
#include "XML_Collection.h"
#include "IC_HybridFC.h"
#include "IC_HybridPC.h"
#include "pugixml.hpp"
#include "IC_SimpleRule.h"
#include "IC_ThematicRule.h"
#include "IC_CompleteRule.h"
#include "S100_Date.h"

namespace S100
{
    class IC_InteroperabilityCatalogue : public CT_Catalogue
    {
    public:
        std::string Description;
        std::shared_ptr<std::string> Comment = nullptr;
        std::shared_ptr<int> InteroperabilityLevel = nullptr;
        requirementType RequirementType;
        std::string RequirementDescription;
        std::vector<dataProduct> ProductCovered;

        XmlCollection<IC_DisplayPlane> DisplayPlanes;
        XmlCollection<IC_PredefinedCombination> PredefinedProductCombinations;
        std::vector<IC_HybridFeatureCreationRule> HybridizationRules;
        std::vector<IC_HybridFC> HybridFC;
        std::vector<IC_HybridPC> HybridPC;

        void Open(std::string filePath);
        void GetContents(pugi::xml_node& node);
    };
}
