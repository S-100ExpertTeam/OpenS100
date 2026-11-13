#include "CoverageLookup.h"

#include <array>
#include <utility>

namespace s100::coverage
{
    std::optional<IntervalClosure> intervalClosureFromString(std::string_view text) noexcept
    {
        static constexpr std::array<std::pair<std::string_view, IntervalClosure>, 8> names{ {
            { "geSemiInterval", IntervalClosure::GeSemiInterval },
            { "gtSemiInterval", IntervalClosure::GtSemiInterval },
            { "leSemiInterval", IntervalClosure::LeSemiInterval },
            { "ltSemiInterval", IntervalClosure::LtSemiInterval },
            { "closedInterval", IntervalClosure::ClosedInterval },
            { "openInterval", IntervalClosure::OpenInterval },
            { "geLtInterval", IntervalClosure::GeLtInterval },
            { "gtLeInterval", IntervalClosure::GtLeInterval },
        } };
        for (const auto& [name, closure] : names)
        {
            if (name == text) return closure;
        }
        return std::nullopt;
    }

    bool inInterval(double value, std::optional<double> lower, std::optional<double> upper, IntervalClosure closure) noexcept
    {
        const auto above = [&](bool inclusive) { return !lower || (inclusive ? value >= *lower : value > *lower); };
        const auto below = [&](bool inclusive) { return !upper || (inclusive ? value <= *upper : value < *upper); };
        switch (closure)
        {
        case IntervalClosure::GeSemiInterval: return lower && value >= *lower;
        case IntervalClosure::GtSemiInterval: return lower && value > *lower;
        case IntervalClosure::LeSemiInterval: return upper && value <= *upper;
        case IntervalClosure::LtSemiInterval: return upper && value < *upper;
        case IntervalClosure::ClosedInterval: return above(true) && below(true);
        case IntervalClosure::OpenInterval: return above(false) && below(false);
        case IntervalClosure::GeLtInterval: return above(true) && below(false);
        case IntervalClosure::GtLeInterval: return above(false) && below(true);
        }
        return false;
    }

    std::optional<LookupMatch> lookupCoverageEntry(std::span<const LookupEntry> lookup, double value) noexcept
    {
        for (std::size_t index = 0; index < lookup.size(); ++index)
        {
            const LookupEntry& entry = lookup[index];
            if (!inInterval(value, entry.lower, entry.upper, entry.closure))
            {
                continue;
            }
            const double fraction = entry.lower && entry.upper && *entry.upper > *entry.lower
                ? (value - *entry.lower) / (*entry.upper - *entry.lower) : 0;
            return LookupMatch{ index, fraction };
        }
        return std::nullopt;
    }

    double symbolScale(const CoverageSymbol& symbol, std::optional<double> value) noexcept
    {
        return symbol.scaleAttribute && value ? *value * symbol.scaleFactor : symbol.defaultScaleFactor;
    }

    double symbolRotation(const CoverageSymbol& symbol, std::optional<double> value) noexcept
    {
        return symbol.rotationAttribute && value ? *value * symbol.rotationFactor : symbol.rotation;
    }
}
