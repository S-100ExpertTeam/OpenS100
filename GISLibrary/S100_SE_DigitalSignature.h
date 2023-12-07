#pragma once
#include <string>
#include "pugixml.hpp"

namespace S100 {
    class S100_SE_DigitalSignature
    {
    public:
        std::string Id;
        std::string CertificateRef;
        std::string Value;

        void GetContents(pugi::xml_node& node);
    };
}