#pragma once
#include "XML_Collection.h"
#include "Status.h"
#include <string>

class TranslationItemType : public xmlParseable {
public:
    std::string Path;
    std::string Original;
    LanguagePackModelStatus ItemStatus;
    std::string Translation;

    void GetContents(pugi::xml_node& node) override;
    void SaveXmlNode(pugi::xml_node& node) override;
};

