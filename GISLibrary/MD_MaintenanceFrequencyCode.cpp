#include "stdafx.h"
#include "MD_MaintenanceFrequencyCode.h"

namespace S100 {
    std::string MD_MaintenanceFrequencyCodeToString(MD_MaintenanceFrequencyCode category) {
        switch (category) {
        case MD_MaintenanceFrequencyCode::asNeeded: return "asNeeded";
        case MD_MaintenanceFrequencyCode::irregular: return "irregular";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    MD_MaintenanceFrequencyCode MD_MaintenanceFrequencyCodeFromString(const std::string& categoryName) {
        if (categoryName == "asNeeded") return MD_MaintenanceFrequencyCode::asNeeded;
        if (categoryName == "irregular") return MD_MaintenanceFrequencyCode::irregular;
        throw std::invalid_argument("Unknown category name");
    }
}
