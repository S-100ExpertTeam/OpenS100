#include "stdafx.h"
#include "CI_Individual.h"

namespace S100
{
    void CI_Individual::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:name"))
            {
                Name = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:contactInfo"))
            {
                CI_Contact cc;
                cc.GetContents(instruction);
                ContactInfo.push_back(cc);
            }
            else if (!strcmp(instructionName, "cit:partyIdentifier"))
            {
                // Handle partyIdentifier if needed
            }
            else if (!strcmp(instructionName, "cit:positionName"))
            {
                PositionName.push_back(instruction.child_value());
            }
        }
    }

    void CI_Individual::Save(pugi::xml_node& node)
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
                auto child = node.append_child("cit:contactInfo");
                ContactInfo[i].Save(child);
            }
        }
        if (!PositionName.empty())
        {
            for (int i = 0; i < PositionName.size(); i++)
            {
                auto child = node.append_child("cit:contactInfo");
                child.text().set(PositionName[i].c_str());
            }
        }
    }
}
