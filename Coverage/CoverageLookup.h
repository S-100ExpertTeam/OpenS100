#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>

// S-100 Part 9a coverage portrayal: the lookup entries a CoverageFill instruction colours a grid
// by (opens100-web `core/src/display-list.ts`).
namespace s100::coverage
{
    // S-100 Part 1 interval types.
    enum class IntervalClosure
    {
        GeSemiInterval,
        GtSemiInterval,
        LeSemiInterval,
        LtSemiInterval,
        ClosedInterval,
        OpenInterval,
        GeLtInterval,
        GtLeInterval,
    };

    // "geLtInterval" -> GeLtInterval; std::nullopt for anything else.
    [[nodiscard]] std::optional<IntervalClosure> intervalClosureFromString(std::string_view text) noexcept;

    // A colour of a lookup entry: a colour token of the palette with its transparency (0 opaque
    // .. 1 clear), and for a gradient the colour at the upper end.
    struct CoverageColor
    {
        std::string startToken;
        double startTransparency = 0;
        std::optional<std::string> endToken;
        double endTransparency = 0;
    };

    struct LookupEntry
    {
        std::string label;
        std::optional<double> lower;
        std::optional<double> upper;
        IntervalClosure closure = IntervalClosure::ClosedInterval;
        std::optional<CoverageColor> color;
    };

    struct LookupMatch
    {
        std::size_t index = 0;
        double fraction = 0;   // where the value lies across the entry's range, 0..1, for gradients

        friend bool operator==(const LookupMatch&, const LookupMatch&) = default;
    };

    [[nodiscard]] bool inInterval(double value, std::optional<double> lower, std::optional<double> upper, IntervalClosure closure) noexcept;

    // The first entry whose range holds `value`; std::nullopt when none matches.
    [[nodiscard]] std::optional<LookupMatch> lookupCoverageEntry(std::span<const LookupEntry> lookup, double value) noexcept;
}
