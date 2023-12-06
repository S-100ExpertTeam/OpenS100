#pragma once

#include <string>
#include <map>

namespace S100 {
    enum class requirementType {
        IHO = 1,
        OEM = 2,
        national = 3,
        local = 4,
        port = 5,
        company = 6,
        pilot = 7,
        master = 8,
        other = 9
    };

    const std::map<std::string, requirementType> RequiremetnTypeCodeMap = {
    {"IHO", requirementType::IHO},
    {"OEM", requirementType::OEM},
    {"national", requirementType::national},
    {"local", requirementType::local},
    {"port", requirementType::port},
    {"company", requirementType::company},
    {"pilot", requirementType::pilot},
    {"master", requirementType::master},
    {"other", requirementType::other}
    };

    requirementType stringTorequirementType(std::string codeName);
}
