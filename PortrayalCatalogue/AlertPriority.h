#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class AlertPriority {
        Alarm = 1,
        Warning,
        Caution,
        Indication
    };

    std::string AlertPriorityToString(AlertPriority category);
    AlertPriority AlertPriorityFromString(const std::string& categoryName);
}
