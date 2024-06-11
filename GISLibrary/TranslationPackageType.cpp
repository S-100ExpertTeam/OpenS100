#include "stdafx.h"
#include "TranslationPackageType.h"
#include "pugixml.hpp"
#include <iostream>

bool TranslationPackageType::LoadFromFile(const std::string& filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        std::cerr << "Failed to load file " << filename << ": " << result.description() << std::endl;
        return false;
    }

    pugi::xml_node root = doc.child("S100LA:translationPackage");
    if (!root) {
        std::cerr << "No translationPackage element found." << std::endl;
        return false;
    }

    GetContents(root);
    return true;
}

void TranslationPackageType::GetContents(pugi::xml_node& node) {
    Language = node.child("S100LA:language").child("S100LA:languageCode").text().get();
    IssueDate = node.child_value("S100LA:issueDate");
    IssueTime = node.child_value("S100LA:issueTime");
    SourceFiles.parseXmlNode(node, "S100LA:sourceFile");
}

void TranslationPackageType::SaveXmlNode(pugi::xml_node& node) {
    node.append_child("S100LA:language").append_child("S100LA:languageCode").text().set(Language.c_str());
    node.append_child("S100LA:issueDate").text().set(IssueDate.c_str());
    node.append_child("S100LA:issueTime").text().set(IssueTime.c_str());
    pugi::xml_node partyNode = node.append_child("S100LA:responsibleParty").append_child("cit:CI_Responsibility").append_child("cit:party").append_child("cit:CI_Individual").append_child("cit:name").append_child("gco:CharacterString");
    SourceFiles.SaveXmlNode(node, "S100LA:sourceFile");
}
