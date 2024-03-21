#include "stdafx.h"
#include "IC_FeatureDerivation.h"

namespace S100
{
    void IC_FeatureDerivation::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "identifier"))
            {
                Identifier = instruction.child_value();
            }
            else if (!strcmp(instructionName, "primaryProduct"))
            {
                PrimaryProduct = instruction.child_value();
            }
            else if (!strcmp(instructionName, "primaryFeatureCode"))
            {
                PrimaryFeatureCode = atoi(instruction.child_value());
            }
            else if (!strcmp(instructionName, "primarySelector"))
            {
                PrimarySelector = std::make_shared<FeatureSelector>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "secondaryProduct"))
            {
                SecondaryProduct = instruction.child_value();
            }
            else if (!strcmp(instructionName, "secondaryFeatureCode"))
            {
                SecondaryFeatureCode = instruction.child_value();
            }
            else if (!strcmp(instructionName, "secondarySelector"))
            {
                SecondarySelector = std::make_shared<FeatureSelector>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "outputProduct"))
            {
                OutputProduct = instruction.child_value();
            }
            else if (!strcmp(instructionName, "outputFeatureCode"))
            {
                OutputFeatureCode = instruction.child_value();
            }
            else if (!strcmp(instructionName, "featureRef"))
            {
                FeatureRef = std::make_shared<IC_Feature>();
                FeatureRef->GetContents(instruction);
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
