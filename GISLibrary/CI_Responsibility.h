#pragma once

#include <memory>
#include <vector>
#include <string>
#include "CI_RoleCode.h"
#include "CI_Party.h"
#include "pugiSupportClass.h"

namespace S100 {
    class CI_Responsibility
    {
    public:
        CI_RoleCode role;
        // EX_Extent extent
        std::vector<CI_Party> party;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
