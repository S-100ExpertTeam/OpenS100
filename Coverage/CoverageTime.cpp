#include "CoverageTime.h"

#include <regex>

namespace s100::coverage
{
    namespace
    {
        int toInt(const std::ssub_match& part)
        {
            return part.matched ? std::stoi(part.str()) : 0;
        }
    }

    std::optional<TimePoint> parseS100DateTime(std::string_view text)
    {
        static const std::regex compact(R"(^(\d{4})(\d{2})(\d{2})T(\d{2})(\d{2})(\d{2})(Z|[+-]\d{2}:?\d{2})?$)");
        static const std::regex extended(R"(^(\d{4})-(\d{2})-(\d{2})[T ](\d{2}):(\d{2})(?::(\d{2})(?:\.\d+)?)?(Z|[+-]\d{2}:?\d{2})?$)");

        std::string value(text);
        const auto first = value.find_first_not_of(" \t\r\n");
        const auto last = value.find_last_not_of(" \t\r\n");
        value = first == std::string::npos ? std::string() : value.substr(first, last - first + 1);

        std::smatch match;
        if (!std::regex_match(value, match, compact) && !std::regex_match(value, match, extended))
        {
            return std::nullopt;
        }
        using namespace std::chrono;
        const year_month_day date{ year{ toInt(match[1]) }, month{ static_cast<unsigned>(toInt(match[2])) },
            day{ static_cast<unsigned>(toInt(match[3])) } };
        if (!date.ok())
        {
            return std::nullopt;
        }
        TimePoint time = sys_days{ date } + hours{ toInt(match[4]) } + minutes{ toInt(match[5]) } + seconds{ toInt(match[6]) };
        const std::string zone = match[7].matched ? match[7].str() : std::string();
        if (!zone.empty() && zone != "Z")
        {
            const int sign = zone.front() == '-' ? -1 : 1;
            std::string digits = zone.substr(1);
            std::erase(digits, ':');
            time -= sign * (hours{ std::stoi(digits.substr(0, 2)) } + minutes{ std::stoi(digits.substr(2)) });
        }
        return time;
    }

    std::size_t timeIndexAt(std::span<const CoverageTimeGroup> groups, std::optional<TimePoint> time) noexcept
    {
        if (!time || groups.empty())
        {
            return 0;
        }
        std::size_t index = 0;
        for (std::size_t i = 0; i < groups.size(); ++i)
        {
            if (groups[i].time && *groups[i].time <= *time)
            {
                index = i;
            }
        }
        return index;
    }

    std::optional<std::size_t> displayTimeIndex(std::span<const CoverageTimeGroup> groups, TimePoint time) noexcept
    {
        if (groups.empty())
        {
            return std::nullopt;
        }
        const auto& first = groups.front().time;
        const auto& last = groups.back().time;
        if (!first || !last)
        {
            return 0;
        }
        if (time < *first)
        {
            return std::nullopt;
        }
        const auto interval = groups.size() > 1 ? *last - groups[groups.size() - 2].time.value_or(*last)
                                                : std::chrono::milliseconds(std::chrono::minutes(5));
        if (time > *last)
        {
            return time - *last < interval ? std::optional(groups.size() - 1) : std::nullopt;
        }
        return timeIndexAt(groups, time);
    }
}
