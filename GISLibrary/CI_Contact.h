#pragma once

#include "CI_Telephone.h"
#include "CI_Address.h"
#include "CI_OnlineResource.h"
#include <string>
#include <vector>
#include <memory>

namespace S100
{
    class CI_Contact
    {
    public:
        std::vector<CI_Telephone> Phone;
        std::vector<CI_Address> Address;
        std::vector<CI_OnlineResource> OnlineResource;

        std::vector<std::string> HoursOfService;
        std::shared_ptr<std::string> ContactInstructions;
        std::shared_ptr<std::string> ContactType;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}