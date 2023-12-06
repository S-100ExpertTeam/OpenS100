#include "stdafx.h"
#include "PT_Locale.h"

namespace S100 {
    void PT_Locale::setLanguage(pugi::xml_node& node)
    {
        Language.GetContents(node);
    }

    void PT_Locale::setCountry(pugi::xml_node& node)
    {
        Country = new CountryCode();
        Country->GetContents(node);
    }

    void PT_Locale::setCharacterEncoding(pugi::xml_node& node)
    {
        CharacterEncoding = parseMD_CharacterSetCode(node);
    }

    void PT_Locale::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "lan:language"))
            {
                setLanguage(instruction);
            }
            else if (!strcmp(instructionName, "lan:country"))
            {
                setCountry(instruction);
            }
            else if (!strcmp(instructionName, "lan:characterEncoding"))
            {
                setCharacterEncoding(instruction);
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
