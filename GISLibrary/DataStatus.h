#pragma once
#include <string>
#include <stdexcept>

namespace S100
{
	enum class DataStatus
	{
		Unencrypted,
		Encrypted,
		Compressed
	};
    std::string DataStatusToString(DataStatus category) {
        switch (category) {
        case DataStatus::Unencrypted: return "Unencrypted";
        case DataStatus::Encrypted: return "Encrypted";
        case DataStatus::Compressed: return "Compressed";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    DataStatus DataStatusFromString(const std::string& categoryName) {
        if (categoryName == "Unencrypted") return DataStatus::Unencrypted;
        if (categoryName == "Encrypted") return DataStatus::Encrypted;
        if (categoryName == "Compressed") return DataStatus::Compressed;
        throw std::invalid_argument("Unknown category name");
    }
}

