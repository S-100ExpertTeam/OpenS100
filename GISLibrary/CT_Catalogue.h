#pragma once

#include "MD_CharacterSetCode.h"
#include "PT_Locale.h"
#include "S100_Date.h"


#include "pugixml.hpp"
#include <memory>
#include <string>
#include <vector>


namespace S100 {
    std::string GetCharacterString(pugi::xml_node& node);
    void SaveCharacterString(pugi::xml_node& node, std::string str);

    class CT_Catalogue {
    public:
        std::string Name;
        std::shared_ptr<std::string> Scope;
        std::vector<std::string> FieldOfApplication;
        std::string VersionNumber;
        S100_Date VersionDate;
        std::shared_ptr<std::string> Language;
        std::vector<std::shared_ptr<PT_Locale>> Locale;
        std::shared_ptr<MD_CharacterSetCode> CharacterSet;

        std::shared_ptr<std::string> getLanguage();
        void setLanguage(std::string language);
        std::vector<std::shared_ptr<PT_Locale>> getLocale();
        void setLocale(pugi::xml_node& node);
    };
}
