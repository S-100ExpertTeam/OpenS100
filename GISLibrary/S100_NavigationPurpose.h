#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class S100_NavigationPurpose {
        port = 1,
        transit,
        overview
    };

    std::string S100_NavigationPurposeToString(S100_NavigationPurpose category);
    S100_NavigationPurpose S100_NavigationPurposeFromString(const std::string& categoryName);
}
