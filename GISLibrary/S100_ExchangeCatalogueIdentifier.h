#pragma once

#include <string>
#include "S100_DateTime.h"
#include "pugiSupportClass.h"

namespace S100 {
    class S100_ExchangeCatalogueIdentifier
    {
    public:
        std::string Identifier;
        S100_DateTime DateTime;

        void GetContents(pugi::xml_node& node);
    };
}
