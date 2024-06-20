#pragma once
#include "XML_Collection.h"
#include "SourceHeaderType.h"
#include "TranslationItemType.h"

class SourceFileType : public xmlParseable {
public:
    SourceHeaderType Header;
    XmlCollection<TranslationItemType> TranslationItems;

    void GetContents(pugi::xml_node& node) override;
    void SaveXmlNode(pugi::xml_node& node) override;
};

