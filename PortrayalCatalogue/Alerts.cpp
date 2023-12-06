#include "stdafx.h"
#include "Alerts.h"
#include "pugixml.hpp"

using namespace S100;

void Alerts::GetContents(pugi::xml_node& node)
{
    for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
    {
        const pugi::char_t* instructionName = instruction.name();

        if (!strcmp(instructionName, "alert"))
        {
            AlertItem* al = new AlertItem();
            al->GetContents(instruction);
            alert.push_back(al);
        }
    }
}
