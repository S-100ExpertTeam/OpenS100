#include "stdafx.h"
#include "S100_CompliancyCategory.h"

namespace S100 {
    std::string S100_CompliancyCategoryToString(S100_CompliancyCategory category) {
        switch (category) {
        case S100_CompliancyCategory::category1: return "category1";
        case S100_CompliancyCategory::category2: return "category2";
        case S100_CompliancyCategory::category3: return "category3";
        case S100_CompliancyCategory::category4: return "category4";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    S100_CompliancyCategory S100_CompliancyCategoryFromString(const std::string& categoryName) {
        if (categoryName == "category1") return S100_CompliancyCategory::category1;
        if (categoryName == "category2") return S100_CompliancyCategory::category2;
        if (categoryName == "category3") return S100_CompliancyCategory::category3;
        if (categoryName == "category4") return S100_CompliancyCategory::category4;
        throw std::invalid_argument("Unknown category name");
    }
}
