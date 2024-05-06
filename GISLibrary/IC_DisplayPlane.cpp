#include "stdafx.h"
#include "IC_DisplayPlane.h"

namespace S100
{
    void IC_DisplayPlane::GetContents(pugi::xml_node& node)
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
            else if (!strcmp(instructionName, "order"))
            {
                Order = atoi(instruction.child_value());
            }
            else if (!strcmp(instructionName, "description"))
            {
                Description = instruction.child_value();
            }
            else if (!strcmp(instructionName, "interoperabilityLevel"))
            {
                InteroperabilityLevel = std::make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "features"))
            {
                Features.parseXmlNode(instruction, "IC_Feature");
            }
            else if (!strcmp(instructionName, "drawingInstructions"))
            {
                DrawingInstructions.parseXmlNode(instruction, "IC_DrawingInstruction");
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

    void IC_DisplayPlane::SaveXmlNode(pugi::xml_node& node)
    {

    }
}
