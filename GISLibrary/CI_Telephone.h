#pragma once

#include <string>
#include "pugixml.hpp"

namespace S100 {
    class CI_Telephone
    {
    public:
        std::string Number;
        std::string Numbertype;

        void GetContents(pugi::xml_node& node);
    };
}
