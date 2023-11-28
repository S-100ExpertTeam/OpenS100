#include "stdafx.h"
#include "CT_Catalogue.h"
#include "CI_Address.h"

namespace S100
{
    void CI_Address::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:deliveryPoint"))
            {
                Deliverypoint = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:city"))
            {
                City = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:administrativeArea"))
            {
                Administrativearea = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:postalCode"))
            {
                Postalcode = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:country"))
            {
                Country = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:electronicMailAddress"))
            {
                Electronicmailaddress.push_back(CharacterString(instruction));
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
