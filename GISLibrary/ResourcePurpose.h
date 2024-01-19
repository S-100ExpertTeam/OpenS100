#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class ResourcePurpose {
        supportFile = 1,
        ISOMetadata,
        languagePack,
        GMLSchema,
        other = 100
    };

    std::string S100_ResourcePurposeToString(ResourcePurpose category);
    ResourcePurpose S100_ResourcePurposeFromString(const std::string& categoryName);
}
