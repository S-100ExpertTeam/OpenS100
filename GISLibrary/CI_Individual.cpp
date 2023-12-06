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
}
