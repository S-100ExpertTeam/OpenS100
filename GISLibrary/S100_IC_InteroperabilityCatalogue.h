#pragma once
#include "CT_Catalogue.h"
#include "requirementType.h"
#include "S100_IC_DisplayPlane.h"
#include "S100_IC_PredefinedCombination.h"
#include "S100_IC_HybridFeatureCreationRule.h"
#include "dataProduct.h"
#include "XML_Collection.h"
#include "S100_IC_HybridFC.h"
#include "S100_IC_HybridPC.h"
#include "pugixml.hpp"
#include "S100_IC_SimpleRule.h"
#include "S100_IC_ThematicRule.h"
#include "S100_IC_CompleteRule.h"
#include "S100_Date.h"

namespace S100
{
    class S100_IC_InteroperabilityCatalogue : public CT_Catalogue
    {
    public:
        std::string Description;
        std::shared_ptr<std::string> Comment = nullptr;
        std::shared_ptr<int> InteroperabilityLevel = nullptr;
        requirementType RequirementType;
        std::string RequirementDescription;
        std::vector<dataProduct> ProductCovered;

        XmlCollection<S100_IC_DisplayPlane> DisplayPlanes;
        XmlCollection<S100_IC_PredefinedCombination> PredefinedProductCombinations;
        std::vector<S100_IC_HybridFeatureCreationRule> HybridizationRules;
        std::vector<S100_IC_HybridFC> HybridFC;
        std::vector<S100_IC_HybridPC> HybridPC;

        void Open(std::string filePath);
        void GetContents(pugi::xml_node& node);
    };
}
