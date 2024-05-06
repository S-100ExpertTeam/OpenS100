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
                 auto temp = ExXmlSupport().GetContentNode(instruction, "CI_RoleCode", "cit").child_value();
                 if (strcmp(temp, "") != 0)
                     role = CI_RoleCodeFromString(temp);
            }
            else if (!strcmp(instructionName, "cit:party"))
            {
                CI_Party pa;
                pa.GetContents(instruction);
                party.push_back(pa);
            }
        }
    }

    void CI_Responsibility::Save(pugi::xml_node& node)
    {
        auto Node = node.append_child("cit:CI_Responsibility");
        {
            auto child = Node.append_child("cit:role");
            auto temp = child.append_child("cit:CI_RoleCode");
            temp.text().set(CI_RoleCodeToString(role).c_str());
        }
        if(!party.empty())
        {
            for(int i = 0 ; i< party.size(); i++)
            {
                auto child = Node.append_child("cit:party");
                party[i].Save(child);
            }
        }
    }
}
