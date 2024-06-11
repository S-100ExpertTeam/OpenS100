#pragma once
#include "XML_Collection.h"
#include <string>

class ResourceIdentification : public xmlParseable {
public:
    std::string Path;
    std::string Value;

    void GetContents(pugi::xml_node& node) override;
    void SaveXmlNode(pugi::xml_node& node) override;
};