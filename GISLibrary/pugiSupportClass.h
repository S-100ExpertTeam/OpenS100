#pragma once

#include <string>
#include <pugixml.hpp>
#include <vector>

namespace S100 {
    bool ParseStr2Bool(std::string value);
    pugi::xml_node& GetContentNode(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "");
    const pugi::char_t* GetContentItem(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "");
    void GetContentItemVector(pugi::xml_node& node, std::string nodeName, std::vector<std::string>& item, std::string nodeNameSpace = "");
}
