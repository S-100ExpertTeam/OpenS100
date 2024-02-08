#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <pugixml.hpp>

namespace S100 {
    class GM_Object {
    public:
        std::string Description;
        std::string DescriptionReference;
        std::string Identifier;
        std::string Name;

        std::string Type;

        std::string srsName;
        std::vector<std::string> Geom;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);

    private:
        std::string GetGeom(pugi::xml_node& node);
        std::string GetGeoms(pugi::xml_node& node);
    };
}
