#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>

// Valid times of coverage records (opens100-web `core/src/coverage.ts`).
namespace s100::coverage
{
    using TimePoint = std::chrono::sys_time<std::chrono::milliseconds>;

    struct CoverageTimeGroup
    {
        std::string name;                 // HDF5 group name, "Group_001"
        std::optional<TimePoint> time;    // std::nullopt for static coverages such as S-102
    };

    // Reads the date-time forms used in Part 10c attributes, "20260914T100000Z" and
    // "2026-09-14T10:00:00" (seconds and fractions optional, a zone "Z" or "+hh:mm" optional).
    // S-100 times without a zone are UTC. Years below 100 stay as they are ("00010101T000000Z" is
    // year 1, S-102's placeholder).
    [[nodiscard]] std::optional<TimePoint> parseS100DateTime(std::string_view text);

    // The time group in force at `time`: the last one starting at or before it, clamped to the
    // first and last groups. Static coverages (and no time) always give 0.
    [[nodiscard]] std::size_t timeIndexAt(std::span<const CoverageTimeGroup> groups, std::optional<TimePoint> time) noexcept;

    // The time group to display at `time`, following S-111 Ed. 2.0.0 clause 9.4: between the
    // first and last records, the closest preceding record; before the first record, nothing;
    // after the last, the last record only while less than one record interval has passed (five
    // minutes for a single record). Static coverages always give 0.
    [[nodiscard]] std::optional<std::size_t> displayTimeIndex(std::span<const CoverageTimeGroup> groups, TimePoint time) noexcept;
}
