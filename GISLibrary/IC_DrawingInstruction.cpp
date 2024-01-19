#include "stdafx.h"
#include "IC_DrawingInstruction.h"

namespace S100 {
    void IC_DrawingInstruction::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "identifier"))
            {
                Identifier = instruction.child_value();
            }
            else if (!strcmp(instructionName, "featureCode"))
            {
                FeatureCode = instruction.child_value();
            }
            else if (!strcmp(instructionName, "product"))
            {
                Product.GetContents(instruction, "product");
            }
            else if (!strcmp(instructionName, "geometryType"))
            {
                GeometryType.push_back(StringToSpatialPrimitiveType(std::string(instruction.child_value())));
            }
            else if (!strcmp(instructionName, "attributeCombination"))
            {
                AttributeCombination.push_back(instruction.child_value());
            }
            else if (!strcmp(instructionName, "drawingPriority"))
            {
                DrawingPriority = atoi(instruction.child_value());
            }
            else if (!strcmp(instructionName, "viewingGroup"))
            {
                ViewingGroup = atoi(instruction.child_value());
            }
            else if (!strcmp(instructionName, "substituteSymbolization"))
            {
                SubstituteSymbolization = new std::string(instruction.child_value());
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
