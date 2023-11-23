#pragma once


namespace S100 {
	enum class MD_MaintenanceFrequencyCode
	{
		asNeeded=1,
		irregular
	};



	inline std::string MD_MaintenanceFrequencyCodeToString(MD_MaintenanceFrequencyCode category) {
		switch (category) {
		case MD_MaintenanceFrequencyCode::asNeeded: return "asNeeded";
		case MD_MaintenanceFrequencyCode::irregular: return "irregular";
		default: throw std::invalid_argument("Unknown category");
		}
	}

	inline  MD_MaintenanceFrequencyCode MD_MaintenanceFrequencyCodeFromString(const std::string& categoryName) {
		if (categoryName == "asNeeded") return MD_MaintenanceFrequencyCode::asNeeded;
		if (categoryName == "irregular") return MD_MaintenanceFrequencyCode::irregular;
		throw std::invalid_argument("Unknown category name");
	}
}

