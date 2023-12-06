#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class S100_ResourcePurpose {
        supportFile = 1,
        ISOMetadata,
        languagePack,
        GMLSchema,
        other = 100
    };

    std::string S100_ResourcePurposeToString(S100_ResourcePurpose category);
    S100_ResourcePurpose S100_ResourcePurposeFromString(const std::string& categoryName);
}
