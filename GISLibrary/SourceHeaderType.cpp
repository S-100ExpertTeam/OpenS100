#include "stdafx.h"
#include "SourceHeaderType.h"

void SourceHeaderType::GetContents(pugi::xml_node& node) {
    ResourceIdentifier = node.child_value("S100LA:resourceIdentifier");
    Identifications.parseXmlNode(node, "S100LA:identification");
}

void SourceHeaderType::SaveXmlNode(pugi::xml_node& node) {
    node.append_child("S100LA:resourceIdentifier").text().set(ResourceIdentifier.c_str());
    Identifications.SaveXmlNode(node, "S100LA:identification");
}
