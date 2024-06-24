#include "stdafx.h"
#include "HighlightStyle.h"

namespace S100
{
    std::string HighlightStyleToString(HighlightStyle category);

    HighlightStyle HighlightStyleFromString(const std::string& categoryName);
}