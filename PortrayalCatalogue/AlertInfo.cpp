#include "stdafx.h"
#include "AlertInfo.h"
#include "pugixml.hpp"

using namespace S100;

void AlertInfo::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("msg");
    if (idAttri != nullptr)
    {
        msg = idAttri.value();
    }
    idAttri = node.attribute("highlight");
    if (idAttri != nullptr)
    {
        highlight = std::make_shared<std::string>(idAttri.value());
    }

    for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
    {
        const pugi::char_t* instructionName = instruction.name();

        if (!strcmp(instructionName, "priority"))
        {
            priority = std::make_shared<AlertPriority>(AlertPriorityFromString(instruction.child_value()));
        }
        else if (!strcmp(instructionName, "priorities"))
        {
            AlertPriorities ap;
            ap.GetContents(instruction);
            priorities = std::make_shared<AlertPriorities>(ap);
        }
    }
}
