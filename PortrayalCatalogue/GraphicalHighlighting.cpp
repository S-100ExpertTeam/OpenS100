#include "stdafx.h"
#include "GraphicalHighlighting.h"
#include "pugixml.hpp"

using namespace S100;

void GraphicalHighlighting::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("id");
    if (idAttri != nullptr)
    {
        auto idValue = pugi::as_wide(idAttri.value());
        SetId(idValue);
    }
    idAttri = node.attribute("optional");
    if (idAttri != nullptr)
    {
        optional = std::make_shared<bool>((std::string)idAttri.value() == "true" ? true : false);
    }
    idAttri = node.attribute("msg");
    if (idAttri != nullptr)
    {
        msg = std::make_shared<std::string>(idAttri.value());
    }

    for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
    {
        const pugi::char_t* instructionName = instruction.name();

        if (!strcmp(instructionName, "description"))
        {
            S100_Description* des = new S100_Description();
            des->GetContents(instruction);
            AddDescription(des);
        }
        else if (!strcmp(instructionName, "viewingGroup"))
        {
            ViewingGroupReference vg;
            vg.GetContents(instruction);
            viewingGroup.push_back(vg);
        }
    }
}
