#pragma once
#include "XML_Collection.h"
#include "ResourceIdentification.h"
#include <vector>
#include <string>

class SourceHeaderType : public xmlParseable {
public:
    std::string ResourceIdentifier;
    XmlCollection<ResourceIdentification> Identifications;

    void GetContents(pugi::xml_node& node) override;
    void SaveXmlNode(pugi::xml_node& node) override;
};

