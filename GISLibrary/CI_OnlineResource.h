#pragma once

#include "CI_OnLineFunctionCode.h"
#include <string>
#include <memory>
#include <vector>
#include "pugixml.hpp"

namespace S100
{
    class CI_OnlineResource
    {
    public:
        std::string Linkage;
        std::shared_ptr<std::string> Protocol;
        std::shared_ptr<std::string> ApplicationProfile;
        std::shared_ptr<std::string> Name;
        std::shared_ptr<std::string> Description;
        std::shared_ptr<CI_OnLineFunctionCode> Function;
        std::shared_ptr<std::string> ProtocolRequest;

        void GetContents(pugi::xml_node& node);
    };
}
