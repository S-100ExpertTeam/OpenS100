#pragma once

#include <string>
#include "pugixml.hpp"
#include "S100_SE_DigitalSignature.h"

namespace S100 {
    class S100_SE_SignatureOnSignature : public S100_SE_DigitalSignature
    {
    public:
        std::string Signatureref;

        void GetContents(pugi::xml_node& node);
    };
}
