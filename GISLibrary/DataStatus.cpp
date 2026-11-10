#include "stdafx.h"
#include "DataStatus.h"

#include <algorithm>
#include <cctype>

namespace S100 {

    // S-100 Part 15 (S100_SE_DataStatus) spells the values in lower case.
    std::string DataStatusToString(DataStatus category) {
        switch (category) {
        case DataStatus::Unencrypted: return "unencrypted";
        case DataStatus::Encrypted: return "encrypted";
        case DataStatus::Compressed: return "compressed";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    // Case-insensitive, so producers that capitalise the value are still accepted.
    DataStatus DataStatusFromString(const std::string& categoryName) {
        std::string name = categoryName;
        std::ranges::transform(name, name.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (name == "unencrypted") return DataStatus::Unencrypted;
        if (name == "encrypted") return DataStatus::Encrypted;
        if (name == "compressed") return DataStatus::Compressed;
        throw std::invalid_argument("Unknown category name");
    }

}
