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

    std::string DataStatusToString(DataStatus category);
    DataStatus DataStatusFromString(const std::string& categoryName);
}

