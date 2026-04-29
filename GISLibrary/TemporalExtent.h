#pragma once

#include "S100_DateTime.h"

#include <memory>

namespace S100 
{
    class TemporalExtent
    {
    public:
        std::shared_ptr<S100_DateTime> TimeInstantBegin;
        std::shared_ptr<S100_DateTime> TimeInstantEnd;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
