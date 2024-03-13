#pragma once

#include "CI_Contact.h"
#include <string>
#include <vector>
#include <memory>

namespace S100
{
    class CI_Organisation
    {
    public:
        std::shared_ptr<std::string> Name;
        std::vector<CI_Contact> ContactInfo;
        // vector<MD_BrowseGraphic> Logo;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
