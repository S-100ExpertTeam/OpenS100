#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class CompliancyCategory {
        category1 = 1,
        category2,
        category3,
        category4
    };

    std::string S100_CompliancyCategoryToString(CompliancyCategory category);
    CompliancyCategory S100_CompliancyCategoryFromString(const std::string& categoryName);
}
