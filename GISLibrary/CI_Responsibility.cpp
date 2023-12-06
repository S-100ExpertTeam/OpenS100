#include "stdafx.h"
#include "CI_Responsibility.h"

namespace S100
{
    void CI_Responsibility::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:role"))
            {
                role = CI_RoleCodeFromString(GetContentNode(instruction, "CI_RoleCode", "cit").child_value());
            }
            else if (!strcmp(instructionName, "cit:party"))
            {
                CI_Party pa;
                pa.GetContents(instruction);
                party.push_back(pa);
            }
        }
    }
}
