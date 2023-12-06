#pragma once

#include <string>
#include <pugixml.hpp>

namespace S100 {
    class dataProduct : public std::string {
    public:
        using std::string::string;
        using std::string::operator=;

        void GetContents(pugi::xml_node& node, std::string nodeName);
    };
}
