#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class NavigationPurpose {
        port = 1,
        transit,
        overview
    };

    std::string S100_NavigationPurposeToString(NavigationPurpose category);
    NavigationPurpose S100_NavigationPurposeFromString(const std::string& categoryName);
}
