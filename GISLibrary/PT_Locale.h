#pragma once

#include "LanguageCode.h"
#include "CountryCode.h"
#include "MD_CharacterSetCode.h"
#include "pugixml.hpp"

namespace S100 {
    class PT_Locale
    {
    public:
        LanguageCode Language;
        CountryCode* Country;
        std::shared_ptr<MD_CharacterSetCode> CharacterEncoding;

        void setLanguage(pugi::xml_node& node);
        void setCountry(pugi::xml_node& node);
        void setCharacterEncoding(pugi::xml_node& node);
        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
