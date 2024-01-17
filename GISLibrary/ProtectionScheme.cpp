#include "stdafx.h"
#include "ProtectionScheme.h"

namespace S100 {
    std::string S100_ProtectionSchemeToString(ProtectionScheme category) {
        switch (category) {
        case ProtectionScheme::S100p15: return "S100p15";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    ProtectionScheme S100_ProtectionSchemeFromString(const std::string& categoryName) {
        if (categoryName == "S100p15") return ProtectionScheme::S100p15;
        throw std::invalid_argument("Unknown category name");
    }
}
