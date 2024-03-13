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
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "lan:MD_CharacterSetCode")) {
                CharacterEncoding = std::make_shared<MD_CharacterSetCode>(MD_CharacterSetCodeFromString(instruction.child_value()));
            }
        }
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

    void PT_Locale::Save(pugi::xml_node& node)
    {
        if (!Language.empty() || !Country->empty() || !CharacterEncoding)
        {
            if (!Language.empty())
            {
                auto child = node.append_child("lan:language");
                child.text().set(Language.c_str());
            }
            if (!Country->empty())
            {
                auto child = node.append_child("lan:country");
                child.text().set(Country->c_str());
            }
            if (!CharacterEncoding)
            {
                auto child = node.append_child("lan:characterEncoding");
                child.text().set(MD_CharacterSetCodeToString(*CharacterEncoding).c_str());
            }
        }
      
    }
}
