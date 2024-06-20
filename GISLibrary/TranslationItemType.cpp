#include "stdafx.h"
#include "TranslationItemType.h"

void TranslationItemType::GetContents(pugi::xml_node& node) {
    Path = node.child_value("S100LA:path");
    Original = node.child_value("S100LA:original");
    ItemStatus.fromString(node.child_value("S100LA:status"));
    Translation = node.child_value("S100LA:translation");
}

void TranslationItemType::SaveXmlNode(pugi::xml_node& node) {
    node.append_child("S100LA:path").text().set(Path.c_str());
    node.append_child("S100LA:original").text().set(Original.c_str());
    node.append_child("S100LA:status").text().set(ItemStatus.toString().c_str());
    node.append_child("S100LA:translation").text().set(Translation.c_str());
}
