#include "stdafx.h"
#include "ResourcePurpose.h"

namespace S100 {
    std::string S100_ResourcePurposeToString(ResourcePurpose category) {
        switch (category) {
        case ResourcePurpose::supportFile: return "supportFile";
        case ResourcePurpose::ISOMetadata: return "ISOMetadata";
        case ResourcePurpose::languagePack: return "languagePack";
        case ResourcePurpose::GMLSchema: return "GMLSchema";
        case ResourcePurpose::other: return "other";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    ResourcePurpose S100_ResourcePurposeFromString(const std::string& categoryName) {
        if (categoryName == "supportFile") return ResourcePurpose::supportFile;
        if (categoryName == "ISOMetadata") return ResourcePurpose::ISOMetadata;
        if (categoryName == "languagePack") return ResourcePurpose::languagePack;
        if (categoryName == "GMLSchema") return ResourcePurpose::GMLSchema;
        if (categoryName == "other") return ResourcePurpose::other;
        throw std::invalid_argument("Unknown category name");
    }
}
