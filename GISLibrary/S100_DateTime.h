#pragma once

#include <string>
#include <stdexcept>

class S100_DateTime {
public:
    S100_DateTime() = default;
    S100_DateTime(const std::string& dateTime);

    std::tm GetTm() const;
    std::string ToString() const;
    void Save(pugi::xml_node& node);

private:
    std::tm tm = {};
    int utcOffsetHours = 0;
    int utcOffsetMinutes = 0;

    std::string NormalizeDateTime(const std::string& dateTime);
    void ProcessUtcOffset(const std::string& dateTime);
};
