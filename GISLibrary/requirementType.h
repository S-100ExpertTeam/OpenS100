#pragma once

namespace S100
{
	enum class requirementType
	{
		IHO = 1,
		OEM = 2,
		national = 3,
		local = 4,
		port = 5,
		company = 6,
		pilot = 7,
		master = 8,
		other = 9
	};

    const std::map<std::string, requirementType> RequirementTypeCodeMap = {
    {"IHO", requirementType::IHO},
    {"OEM", requirementType::OEM},
    {"national", requirementType::national},
    {"local", requirementType::local},
    {"port", requirementType::port},
    {"company", requirementType::company},
    {"pilot", requirementType::pilot},
    {"master", requirementType::master},
    {"other", requirementType::other}
    };

    inline requirementType  stringTorequirementType(std::string codeName) {
        auto it = RequirementTypeCodeMap.find(codeName);
        if (it != RequirementTypeCodeMap.end()) {
            return it->second;
        }
        return requirementType::other;
    }

}
