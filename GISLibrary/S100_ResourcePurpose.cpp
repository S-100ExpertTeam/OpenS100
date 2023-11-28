#include "stdafx.h"
#include "S100_ResourcePurpose.h"

namespace S100 {
    std::string S100_ResourcePurposeToString(S100_ResourcePurpose category) {
        switch (category) {
        case S100_ResourcePurpose::supportFile: return "supportFile";
        case S100_ResourcePurpose::ISOMetadata: return "ISOMetadata";
        case S100_ResourcePurpose::languagePack: return "languagePack";
        case S100_ResourcePurpose::GMLSchema: return "GMLSchema";
        case S100_ResourcePurpose::other: return "other";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    S100_ResourcePurpose S100_ResourcePurposeFromString(const std::string& categoryName) {
        if (categoryName == "supportFile") return S100_ResourcePurpose::supportFile;
        if (categoryName == "ISOMetadata") return S100_ResourcePurpose::ISOMetadata;
        if (categoryName == "languagePack") return S100_ResourcePurpose::languagePack;
        if (categoryName == "GMLSchema") return S100_ResourcePurpose::GMLSchema;
        if (categoryName == "other") return S100_ResourcePurpose::other;
        throw std::invalid_argument("Unknown category name");
    }
}
