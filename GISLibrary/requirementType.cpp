#include "stdafx.h"
#include "requirementType.h"

namespace S100 {

    requirementType stringTorequirementType(std::string codeName) {
        auto it = RequiremetnTypeCodeMap.find(codeName);
        if (it != RequiremetnTypeCodeMap.end()) {
            return it->second;
        }
        return requirementType::other;
    }
}
