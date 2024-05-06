#pragma once

#include <string>
#include <vector>

namespace S100
{
    class CI_Address
    {
    public:
        std::string Deliverypoint;
        std::string City;
        std::string Administrativearea;
        std::string Postalcode;
        std::string Country;
        std::vector<std::string> Electronicmailaddress;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node, std::string nodeName);
    };
}