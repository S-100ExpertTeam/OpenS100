#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class S100_ProtectionScheme {
        S100p15 = 1
    };

    std::string S100_ProtectionSchemeToString(S100_ProtectionScheme category);
    S100_ProtectionScheme S100_ProtectionSchemeFromString(const std::string& categoryName);
}
