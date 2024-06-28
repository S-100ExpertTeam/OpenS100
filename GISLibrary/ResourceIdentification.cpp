#include "stdafx.h"
#include "ResourceIdentification.h"

void ResourceIdentification::GetContents(pugi::xml_node& node) {
    Path = node.child_value("S100LA:path");
    Value = node.child_value("S100LA:value");
}

void ResourceIdentification::SaveXmlNode(pugi::xml_node& node) {
    node.append_child("S100LA:path").text().set(Path.c_str());
    node.append_child("S100LA:value").text().set(Value.c_str());
}