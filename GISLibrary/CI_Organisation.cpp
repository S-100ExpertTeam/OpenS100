#include "stdafx.h"
#include "CI_Organisation.h"

namespace S100
{
    void CI_Organisation::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:name"))
            {
                Name = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:CI_Contact"))
            {
                CI_Contact cc;
                cc.GetContents(instruction);
                ContactInfo.push_back(cc);
            }
        }
    }

    void CI_Organisation::Save(pugi::xml_node& node)
    {
        if (Name)
        {
            auto child = node.append_child("cit:name");
            child.text().set(Name->c_str());
        }
        if (!ContactInfo.empty())
        {
            for (int i = 0; i < ContactInfo.size(); i++)
            {
                auto child = node.append_child("cit:CI_Contact");
                ContactInfo[i].Save(child);
            }
        }
    }
}

