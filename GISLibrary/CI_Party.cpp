#include "stdafx.h"
#include "CI_Party.h"

namespace S100
{
    void CI_Party::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:CI_Organisation"))
            {
                CI_Organisation org;
                org.GetContents(instruction);
                Organisation = org;
            }
            else if (!strcmp(instructionName, "certificateRef"))
            {
                CI_Individual ind;
                ind.GetContents(instruction);
                Individual = ind;
            }
        }
    }

    void CI_Party::Save(pugi::xml_node& node)
    {
        {
            auto child = node.append_child("cit:CI_Organisation");
            Organisation.Save(child);
        }
        {
            auto child = node.append_child("cit:certificateRef");
            Individual.Save(child);
        }

    }
}
