#include "stdafx.h"
#include "S100_Time.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>

S100_Time::S100_Time(const std::string& time) {
    std::string normalizedTime = NormalizeTime(time);
    std::istringstream ss(normalizedTime);
    ss >> std::get_time(&tm, "%H%M%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse the time string.");
    }
    ProcessUtcOffset(time);
}

std::tm S100_Time::GetTm() const {
    return tm;
}

std::string S100_Time::ToString() const {
    std::ostringstream ss;
    ss << std::put_time(&tm, "%H:%M:%S");
    if (utcOffsetHours || utcOffsetMinutes) {
        ss << (utcOffsetHours >= 0 ? "+" : "-")
            << std::setfill('0') << std::setw(2) << std::abs(utcOffsetHours)
            << std::setw(2) << std::abs(utcOffsetMinutes);
    }
    return ss.str();
}

//std::string S100_Time::NormalizeTime(const std::string& time) {
//    std::regex timeFormatRegex(R"(^(\d{2})(\d{2})(\d{2})(Z|[+-]\d{4})?$)");
//    std::smatch matches;
//    if (std::regex_match(time, matches, timeFormatRegex)) {
//        return matches[1].str() + matches[2].str() + matches[3].str();
//    }
//    throw std::runtime_error("Invalid time format.");
//}
//
//void S100_Time::ProcessUtcOffset(const std::string& time) {
//    std::regex offsetRegex(R"(([+-])(\d{2})(\d{2})$)");
//    std::smatch matches;
//    if (std::regex_search(time, matches, offsetRegex)) {
//        utcOffsetHours = std::stoi(matches[2].str()) * (matches[1].str() == "+" ? 1 : -1);
//        utcOffsetMinutes = std::stoi(matches[3].str()) * (matches[1].str() == "+" ? 1 : -1);
//    }
//}

std::string S100_Time::NormalizeTime(const std::string& time) {
    std::regex timeFormatRegex(R"(^(\d{2}):(\d{2}):(\d{2})(Z|[+-]\d{4})?$)");
    std::smatch matches;
    if (std::regex_match(time, matches, timeFormatRegex)) {
        return matches[1].str() + matches[2].str() + matches[3].str();
    }
    throw std::runtime_error("Invalid time format.");
}

void S100_Time::ProcessUtcOffset(const std::string& time) {
    std::regex offsetRegex(R"((Z|[+-])(\d{2})(\d{2})?$)");
    std::smatch matches;
    if (std::regex_search(time, matches, offsetRegex)) {
        if (matches[1].str() == "Z") {
            utcOffsetHours = 0;
            utcOffsetMinutes = 0;
        }
        else {
            utcOffsetHours = std::stoi(matches[2].str()) * (matches[1].str() == "+" ? 1 : -1);
            utcOffsetMinutes = std::stoi(matches[3].str()) * (matches[1].str() == "+" ? 1 : -1);
        }
    }
}
