#include "stdafx.h"
#include "IC_SuppressedFeatureLayer.h"

namespace S100 {
    void IC_SuppressedFeatureLayer::GetContents(pugi::xml_node& node)
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
                Product = instruction.child_value();
            }
            else if (!strcmp(instructionName, "featureRef"))
            {
                IC_Feature fe;
                fe.GetContents(instruction);
                FeatureRef.push_back(fe);
            }
            else if (!strcmp(instructionName, "drawingInstructionRef"))
            {
                IC_DrawingInstruction fe;
                fe.GetContents(instruction);
                DrawingInstructionRef.push_back(fe);
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

    void IC_SuppressedFeatureLayer::SaveXmlNode(pugi::xml_node& node)
    {

    }
}
