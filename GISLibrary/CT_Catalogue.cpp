#include "stdafx.h"
#include "CT_Catalogue.h"

namespace S100 {
    std::string CharacterString(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "gco:CharacterString")) {
                return instruction.child_value();
            }
        }
        return "";
    }


    void SaveCharacterString(pugi::xml_node& node , std::string str) {
        if (!str.empty())
        {
            auto child = node.append_child("gco:CharacterString");
            child.text().set(str.c_str());
        }
    }



    std::shared_ptr<std::string> CT_Catalogue::getLanguage() {
        return Language;
    }

    void CT_Catalogue::setLanguage(std::string language) {
        Language = std::make_shared<std::string>(language);
    }

    std::vector<std::shared_ptr<PT_Locale>> CT_Catalogue::getLocale() {
        return Locale;
    }

    void CT_Catalogue::setLocale(pugi::xml_node& node) {
        auto loc = std::make_shared<PT_Locale>();
        loc->GetContents(node);
        Locale.push_back(loc);
    }
}
