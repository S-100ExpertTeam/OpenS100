#pragma once

#include <string>
#include <pugixml.hpp>
#include <vector>

namespace S100 
{
    class ExXmlSupport
    {
    public:
        static bool ParseStr2Bool(std::string value);
        static std::string ParseBool2Str(bool value);

        static pugi::xml_node& GetContentSelectNode(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "");
        static pugi::xml_node& GetContentNode(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "");

        static const pugi::char_t* GetContentItem(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "");
        static void GetContentItemVector(pugi::xml_node& node, std::string nodeName, std::vector<std::string>& item, std::string nodeNameSpace = "");
        static bool Compare(std::string str, std::string nodeName, std::string nodeNameSpace = "");
    };
}
