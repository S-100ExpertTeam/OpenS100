#include "stdafx.h"
#include "CountryCode.h"

namespace S100 {
    void CountryCode::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "lan:CountryCode"))
            {
                *this = std::string(instruction.child_value());
            }
        }
    }
}
