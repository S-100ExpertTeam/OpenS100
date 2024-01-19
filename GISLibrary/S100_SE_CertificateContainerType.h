#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Base64String.h"
#include "pugixml.hpp"

namespace S100 {

    class S100_SE_CertificateContainerType
    {
    public:
        std::shared_ptr<std::string> SchemeAdministrator;
        std::vector<Base64String> Certificate;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };

}
