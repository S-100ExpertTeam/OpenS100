#pragma once

#include <string>
#include "pugixml.hpp"
#include "S100_SE_DigitalSignature.h"
#include "DataStatus.h"

namespace S100 {
    class S100_SE_SignatureOnData : public S100_SE_DigitalSignature
    {
    public:
        DataStatus DataStatus;

        void GetContents(pugi::xml_node& node);
    };
}
