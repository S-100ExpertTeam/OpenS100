#include "stdafx.h"
#include "IC_PredefinedCombination.h"

namespace S100 {
    void IC_PredefinedCombination::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "identifier"))
            {
                Identifier = instruction.child_value();
            }
            else if (!strcmp(instructionName, "name"))
            {
                Name = instruction.child_value();
            }
            else if (!strcmp(instructionName, "description"))
            {
                Description = instruction.child_value();
            }
            else if (!strcmp(instructionName, "useConditions"))
            {
                UseConditions = instruction.child_value();
            }
            else if (!strcmp(instructionName, "interoperabilityLevel"))
            {
                InteroperabilityLevel = std::make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "includedProduct"))
            {
                IncludedProduct.push_back(instruction.child_value());
            }
            else if (!strcmp(instructionName, "displayPlaneRef"))
            {
                DisplayPlaneRef.parseXmlNode(instruction, "IC_DisplayPlane");
            }
            else if (!strcmp(instructionName, "derivedFeatures"))
            {
                DerivedFeatures.parseXmlNode(instruction, "IC_FeatureDerivation");
            }
            else if (!strcmp(instructionName, "suppressedFeatureLayers"))
            {
                SuppressedFeatureLayers.parseXmlNode(instruction, "IC_SuppressedFeatureLayer");
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
