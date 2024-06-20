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
                Number = GetCharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit::numbertype"))
            {
                Numbertype = GetCharacterString(instruction);
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

    void CI_Telephone::Save(pugi::xml_node& node, std::string nodeName)
    {
        if (Number != "")
        {
            auto Node = node.append_child(nodeName.c_str());
            auto child = Node.append_child("cit:number");
            child.text().set(Number.c_str());

            if (Numbertype != "")
            {
                child = Node.append_child("cit::numbertype");
                child.text().set(Numbertype.c_str());
            }
        }
    }
}
