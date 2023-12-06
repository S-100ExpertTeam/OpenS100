#pragma once

#include "CI_DateTypeCode.h"
#include "S100_Date.h"
#include <pugixml.hpp>

namespace S100
{
    class CI_Date
    {
    public:
        S100_Date date;
        CI_DateTypeCode dateType;

        void GetContents(pugi::xml_node& node);
    };
}