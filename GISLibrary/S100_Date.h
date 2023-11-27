#pragma once
#include <string>
#include <time.h>
#include <stdexcept>
#include <iomanip>
#include <regex>
#include <sstream>


namespace S100
{
    class S100_Date {
    public:
        std::tm tm = {};

        S100_Date() = default;

        S100_Date(const std::string& date) {
            std::string normalizedDate = NormalizeDate(date);
            std::istringstream ss(normalizedDate);
            ss >> std::get_time(&tm, "%Y-%m-%d");
            if (ss.fail()) {
                throw std::runtime_error("Failed to parse the date string.");
            }
            /*  time = std::mktime(&tm);
              if (time == -1) {
                  throw std::runtime_error("Failed to convert to time_t.");
              }*/
        }

        inline std::tm GetTm() const {
            return tm;
        }

        inline std::string ToString() const {
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
            return std::string(buffer);
        }

        inline std::string NormalizeDate(const std::string& date) {
            std::regex basicFormatRegex(R"(^(\d{4})(\d{2})(\d{2})$)");
            std::smatch matches;
            if (std::regex_match(date, matches, basicFormatRegex)) {
                return matches[1].str() + "-" + matches[2].str() + "-" + matches[3].str();
            }
            return date;
        }
    };
}
