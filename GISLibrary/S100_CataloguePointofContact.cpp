#include "stdafx.h"
#include "S100_CataloguePointofContact.h"
#include "CT_Catalogue.h"

namespace S100 {
    void S100_CataloguePointofContact::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:organization"))
            {
                Organization = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "S100XC:phone"))
            {
                CI_Telephone tel;
                tel.GetContents(instruction);
                Phone = tel;
            }
            else if (!strcmp(instructionName, "S100XC:address"))
            {
                CI_Address addr;
                addr.GetContents(instruction);
                Address = addr;
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
