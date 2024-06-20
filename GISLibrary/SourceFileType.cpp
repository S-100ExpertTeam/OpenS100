#include "stdafx.h"
#include "SourceFileType.h"

void SourceFileType::GetContents(pugi::xml_node& node) {
    pugi::xml_node headerNode = node.child("S100LA:header");
    Header.GetContents(headerNode);
    TranslationItems.parseXmlNode(node, "S100LA:translationItem");
}

void SourceFileType::SaveXmlNode(pugi::xml_node& node) {
    pugi::xml_node headerNode = node.append_child("S100LA:header");
    Header.SaveXmlNode(headerNode);
    TranslationItems.SaveXmlNode(node, "S100LA:translationItem");
}
