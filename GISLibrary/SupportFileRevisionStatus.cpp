#include "stdafx.h"
#include "SupportFileRevisionStatus.h"

namespace S100 {
    std::string S100_SupportFileRevisionStatusToString(SupportFileRevisionStatus category) {
        switch (category) {
        case SupportFileRevisionStatus::_new: return "new";
        case SupportFileRevisionStatus::replacement: return "replacement";
        case SupportFileRevisionStatus::deletion: return "deletion";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    SupportFileRevisionStatus S100_SupportFileRevisionStatusFromString(const std::string& categoryName) {
        if (categoryName == "new") return SupportFileRevisionStatus::_new;
        if (categoryName == "replacement") return SupportFileRevisionStatus::replacement;
        if (categoryName == "deletion") return SupportFileRevisionStatus::deletion;
        throw std::invalid_argument("Unknown category name");
    }
}
