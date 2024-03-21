#include "stdafx.h"
#include "CompliancyCategory.h"

namespace S100 {
    std::string S100_CompliancyCategoryToString(CompliancyCategory category) {
        switch (category) {
        case CompliancyCategory::category1: return "category1";
        case CompliancyCategory::category2: return "category2";
        case CompliancyCategory::category3: return "category3";
        case CompliancyCategory::category4: return "category4";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    CompliancyCategory S100_CompliancyCategoryFromString(const std::string& categoryName) {
        if (categoryName == "category1") return CompliancyCategory::category1;
        if (categoryName == "category2") return CompliancyCategory::category2;
        if (categoryName == "category3") return CompliancyCategory::category3;
        if (categoryName == "category4") return CompliancyCategory::category4;
        throw std::invalid_argument("Unknown category name");
    }
}
