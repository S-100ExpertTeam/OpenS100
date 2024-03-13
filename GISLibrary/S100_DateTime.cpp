#include "stdafx.h"
#include "S100_DateTime.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>

S100_DateTime::S100_DateTime(const std::string& dateTime) {
    std::string normalizedDateTime = NormalizeDateTime(dateTime);
    std::istringstream ss(normalizedDateTime);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse the date time string.");
    }
    ProcessUtcOffset(dateTime);
}

void S100_DateTime::Save(pugi::xml_node& node)
{
    node.text().set(ToString().c_str());
}

std::tm S100_DateTime::GetTm() const {
    return tm;
}

std::string S100_DateTime::ToString() const {
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    if (utcOffsetHours || utcOffsetMinutes) {
        ss << (utcOffsetHours >= 0 ? "+" : "-")
            << std::setfill('0') << std::setw(2) << std::abs(utcOffsetHours)
            << ":" << std::setw(2) << std::abs(utcOffsetMinutes);
    }
    return ss.str();
}


std::string S100_DateTime::NormalizeDateTime(const std::string& dateTime) {
    std::regex dateTimeFormatRegex(R"(^(\d{4})(-?)(\d{2})(-?)(\d{2})T(\d{2}):(\d{2}):(\d{2})(Z|[+-]\d{2}:\d{2})?$)");
    std::smatch matches;
    if (std::regex_match(dateTime, matches, dateTimeFormatRegex)) {
        std::string normalizedDateTime = matches[1].str() + "-" + matches[3].str() + "-" + matches[5].str() + "T" + matches[6].str() + ":" + matches[7].str() + ":" + matches[8].str();
        return normalizedDateTime; 
    }
    throw std::runtime_error("Invalid date time format.");
}

void S100_DateTime::ProcessUtcOffset(const std::string& dateTime) {
    std::regex offsetRegex(R"(([+-])(\d{2}):(\d{2})$)");
    std::smatch matches;

    if (std::regex_search(dateTime, matches, offsetRegex)) {
        utcOffsetHours = std::stoi(matches[2].str()) * (matches[1].str() == "+" ? 1 : -1);
        utcOffsetMinutes = std::stoi(matches[3].str()) * (matches[1].str() == "+" ? 1 : -1);
    }
    else if (dateTime.back() == 'Z' || dateTime.back() == 'z') {
        utcOffsetHours = 0;
        utcOffsetMinutes = 0;
    }
    else {
        utcOffsetHours = 0;
        utcOffsetMinutes = 0;
    }
}