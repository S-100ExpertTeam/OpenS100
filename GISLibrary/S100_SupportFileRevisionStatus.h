#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class S100_SupportFileRevisionStatus {
        _new = 1,
        replacement,
        deletion
    };

    std::string S100_SupportFileRevisionStatusToString(S100_SupportFileRevisionStatus category);
    S100_SupportFileRevisionStatus S100_SupportFileRevisionStatusFromString(const std::string& categoryName);
}
