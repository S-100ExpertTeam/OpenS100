#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class MD_MaintenanceFrequencyCode {
        asNeeded = 1,
        irregular
    };

    std::string MD_MaintenanceFrequencyCodeToString(MD_MaintenanceFrequencyCode category);
    MD_MaintenanceFrequencyCode MD_MaintenanceFrequencyCodeFromString(const std::string& categoryName);
}
