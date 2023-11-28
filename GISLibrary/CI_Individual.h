#pragma once

#include "CI_Contact.h"
#include <string>
#include <memory>
#include <vector>

namespace S100
{
    class CI_Individual
    {
    public:
        std::shared_ptr<std::string> Name;
        std::vector<CI_Contact> ContactInfo;
        // PartyIdentifier
        std::vector<std::string> PositionName;

        void GetContents(pugi::xml_node& node);
    };
}
