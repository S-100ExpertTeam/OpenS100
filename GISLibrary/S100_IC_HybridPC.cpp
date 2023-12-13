#include "stdafx.h"
#include "S100_IC_HybridPC.h"

namespace S100
{
    void S100_IC_HybridPC::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100_IC_HybridPC"))
            {
                *this = std::string(instruction.child_value());
            }
        }
    }
}