#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class MD_ClassificationCode {
        confidential = 1,
        forOfficialUseOnly,
        limitedDistribution,
        _protected,
        restricted,
        SBU,
        secret,
        topSecret,
        unclassified
    };

    std::string MD_ClassificationCodeToString(MD_ClassificationCode category);
    MD_ClassificationCode MD_ClassificationCodeFromString(const std::string& categoryName);
}
