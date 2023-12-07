#include "stdafx.h"
#include "S100_SupportFileRevisionStatus.h"

namespace S100 {
    std::string S100_SupportFileRevisionStatusToString(S100_SupportFileRevisionStatus category) {
        switch (category) {
        case S100_SupportFileRevisionStatus::_new: return "new";
        case S100_SupportFileRevisionStatus::replacement: return "replacement";
        case S100_SupportFileRevisionStatus::deletion: return "deletion";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    S100_SupportFileRevisionStatus S100_SupportFileRevisionStatusFromString(const std::string& categoryName) {
        if (categoryName == "new") return S100_SupportFileRevisionStatus::_new;
        if (categoryName == "replacement") return S100_SupportFileRevisionStatus::replacement;
        if (categoryName == "deletion") return S100_SupportFileRevisionStatus::deletion;
        throw std::invalid_argument("Unknown category name");
    }
}
