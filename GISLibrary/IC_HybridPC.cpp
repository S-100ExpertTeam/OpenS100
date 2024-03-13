#include "stdafx.h"
#include "IC_HybridPC.h"

namespace S100
{
    void IC_HybridPC::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "IC_HybridPC"))
            {
                *this = std::string(instruction.child_value());
            }
        }
    }
}
