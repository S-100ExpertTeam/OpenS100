#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class SupportFileRevisionStatus {
        _new = 1,
        replacement,
        deletion
    };

    std::string S100_SupportFileRevisionStatusToString(SupportFileRevisionStatus category);
    SupportFileRevisionStatus S100_SupportFileRevisionStatusFromString(const std::string& categoryName);
}
