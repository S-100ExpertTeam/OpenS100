#include "stdafx.h"
#include "IC_HybridFC.h"

namespace S100
{
    void IC_HybridFC::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "IC_HybridFC"))
            {
                *this = std::string(instruction.child_value());
            }
        }
    }
}
