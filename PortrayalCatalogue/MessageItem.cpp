#include "stdafx.h"
#include "MessageItem.h"
#include "pugixml.hpp"

using namespace S100;

void MessageItem::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("id");
    if (idAttri != nullptr)
    {
        auto idValue = pugi::as_wide(idAttri.value());
        SetId(idValue);
    }
    idAttri = node.attribute("icon");
    if (idAttri != nullptr)
    {
        Icon = std::make_shared<std::string>(idAttri.value());
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
        else if (!strcmp(instructionName, "text"))
        {
            Text txt;
            txt.GetContents(instruction);
            text.push_back(txt);
        }
    }
}
