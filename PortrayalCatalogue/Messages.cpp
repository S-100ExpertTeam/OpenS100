#include "stdafx.h"
#include "Messages.h"
#include "pugixml.hpp"

using namespace S100;

void Messages::GetContents(pugi::xml_node& node)
{
    for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
    {
        const pugi::char_t* instructionName = instruction.name();

        if (!strcmp(instructionName, "message"))
        {
            MessageItem* mi = new MessageItem();
            mi->GetContents(instruction);
            message.push_back(mi);
        }
    }
}
