#pragma once
#include <string>


namespace S100 {
	enum class S100_ProtectionScheme
	{
		S100p15 = 1
	};

    inline std::string S100_ProtectionSchemeToString(S100_ProtectionScheme category) {
        switch (category) {
        case S100_ProtectionScheme::S100p15: return "S100p15";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    inline S100_ProtectionScheme S100_ProtectionSchemeFromString(const std::string& categoryName) {
        if (categoryName == "S100p15") return S100_ProtectionScheme::S100p15;
        throw std::invalid_argument("Unknown category name");
    }


}