#pragma once

#include <string>
#include <stdexcept>

class S100_Time {
public:
    S100_Time(const std::string& time);

    std::tm GetTm() const;
    std::string ToString() const;

private:
    std::tm tm = {};
    int utcOffsetHours = 0;
    int utcOffsetMinutes = 0;

    std::string NormalizeTime(const std::string& time);
    void ProcessUtcOffset(const std::string& time);
};