#pragma once
#include "XML_Collection.h"
#include "SourceFileType.h"
#include <vector>
#include <string>

class TranslationPackageType : public xmlParseable {
public:
    std::string Language;
    std::string IssueDate;
    std::string IssueTime;
    XmlCollection<SourceFileType> SourceFiles;

    bool LoadFromFile(const std::string& filename);
    void GetContents(pugi::xml_node& node) override;
    void SaveXmlNode(pugi::xml_node& node) override;
};


