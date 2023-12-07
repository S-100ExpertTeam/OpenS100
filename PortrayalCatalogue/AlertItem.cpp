#include "stdafx.h"
#include "AlertItem.h"
#include "pugixml.hpp"

using namespace S100;

void AlertItem::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("id");
    if (idAttri != nullptr)
    {
        auto idValue = pugi::as_wide(idAttri.value());
        SetId(idValue);
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
        else if (!strcmp(instructionName, "routeMonitor"))
        {
            AlertInfo ai;
            ai.GetContents(instruction);
            routeMonitor = std::make_shared<AlertInfo>(ai);
        }
        else if (!strcmp(instructionName, "routePlan"))
        {
            AlertInfo vg;
            vg.GetContents(instruction);
            routePlan = std::make_shared<AlertInfo>(vg);
        }
    }
}
