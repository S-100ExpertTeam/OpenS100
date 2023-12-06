#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class HighlightStyle {
        AlarmHighlight = 1,
        CautionHighlight,
    };

    std::string HighlightStyleToString(HighlightStyle category) {
        switch (category) {
        case HighlightStyle::AlarmHighlight: return "AlarmHighlight";
        case HighlightStyle::CautionHighlight: return "CautionHighlight";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    HighlightStyle HighlightStyleFromString(const std::string& categoryName) {
        if (categoryName == "AlarmHighlight") return HighlightStyle::AlarmHighlight;
        if (categoryName == "CautionHighlight") return HighlightStyle::CautionHighlight;
        throw std::invalid_argument("Unknown category name");
    }
}
