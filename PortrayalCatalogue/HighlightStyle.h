#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class HighlightStyle {
        AlarmHighlight = 1,
        CautionHighlight,
    };

    std::string HighlightStyleToString(HighlightStyle category);

    HighlightStyle HighlightStyleFromString(const std::string& categoryName);
}
