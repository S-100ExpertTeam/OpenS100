#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class ProtectionScheme {
        S100p15 = 1
    };

    std::string S100_ProtectionSchemeToString(ProtectionScheme category);
    ProtectionScheme S100_ProtectionSchemeFromString(const std::string& categoryName);
}
