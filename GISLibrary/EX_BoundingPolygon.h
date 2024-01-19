#pragma once

#include "GM_Object_New.h"
#include <pugixml.hpp>

namespace S100 {
    class EX_BoundingPolygon {
    public:
        std::shared_ptr<bool> ExtentTypeCode;
        GM_Object Polygon;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
