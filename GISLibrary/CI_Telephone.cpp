#include "stdafx.h"
#include "CI_Telephone.h"
#include "CT_Catalogue.h"

namespace S100 {
    void CI_Telephone::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:number"))
            {
                Number = CharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit::numbertype"))
            {
                Numbertype = CharacterString(instruction);
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
