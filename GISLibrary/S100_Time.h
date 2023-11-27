#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>


class S100_Time {
public:
    S100_Time(const std::string& time) {
        std::string normalizedTime = NormalizeTime(time);
        std::istringstream ss(normalizedTime);
        ss >> std::get_time(&tm, "%H%M%S");
        if (ss.fail()) {
            throw std::runtime_error("Failed to parse the time string.");
        }
        ProcessUtcOffset(time);
    }

    std::tm GetTm() const {
        return tm;
    }

    std::string ToString() const {
        std::ostringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");
        if (utcOffsetHours || utcOffsetMinutes) {
            ss << (utcOffsetHours >= 0 ? "+" : "-")
                << std::setfill('0') << std::setw(2) << std::abs(utcOffsetHours)
                << std::setw(2) << std::abs(utcOffsetMinutes);
        }
        return ss.str();
    }

private:
    std::tm tm = {}; 
    int utcOffsetHours = 0; 
    int utcOffsetMinutes = 0; 

    std::string NormalizeTime(const std::string& time) {
        std::regex timeFormatRegex(R"(^(\d{2})(\d{2})(\d{2})(Z|[+-]\d{4})?$)");
        std::smatch matches;
        if (std::regex_match(time, matches, timeFormatRegex)) {
            return matches[1].str() + matches[2].str() + matches[3].str();
        }
        throw std::runtime_error("Invalid time format.");
    }

    void ProcessUtcOffset(const std::string& time) {
        std::regex offsetRegex(R"(([+-])(\d{2})(\d{2})$)");
        std::smatch matches;
        if (std::regex_search(time, matches, offsetRegex)) {
            utcOffsetHours = std::stoi(matches[2].str()) * (matches[1].str() == "+" ? 1 : -1);
            utcOffsetMinutes = std::stoi(matches[3].str()) * (matches[1].str() == "+" ? 1 : -1);
        }
    }
};