#include "stdafx.h"
#include "S100_Date.h"
#include <iomanip>
#include <regex>
#include <sstream>

namespace S100 {
    S100_Date::S100_Date(const std::string& date) {
        std::string normalizedDate = NormalizeDate(date);
        std::istringstream ss(normalizedDate);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        if (ss.fail()) {
            throw std::runtime_error("Failed to parse the date string.");
        }
    }

    std::tm S100_Date::GetTm() const {
        return tm;
    }

    std::string S100_Date::ToString() const {
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
        return std::string(buffer);
    }

    std::string S100_Date::NormalizeDate(const std::string& date) {
        std::regex basicFormatRegex(R"(^(\d{4})(\d{2})(\d{2})$)");
        std::smatch matches;
        if (std::regex_match(date, matches, basicFormatRegex)) {
            return matches[1].str() + "-" + matches[2].str() + "-" + matches[3].str();
        }
        return date;
    }
}
