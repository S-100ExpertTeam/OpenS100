#include "stdafx.h"
#include "AlertCatalog.h"

namespace S100
{
     std::string AlertPriorityToString(AlertPriority category) {
        switch (category) {
        case AlertPriority::Alarm: return "Alarm";
        case AlertPriority::Warning: return "Warning";
        case AlertPriority::Caution: return "Caution";
        case AlertPriority::Indication: return "Indication";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    AlertPriority AlertPriorityFromString(const std::string& categoryName) {
        if (categoryName == "Alarm") return AlertPriority::Alarm;
        if (categoryName == "Warning") return AlertPriority::Warning;
        if (categoryName == "Caution") return AlertPriority::Caution;
        if (categoryName == "Indication") return AlertPriority::Indication;
        throw std::invalid_argument("Unknown category name");
    }
}