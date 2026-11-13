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

    // A symbol of a lookup entry, drawn at each grid point whose value falls in the entry's range
    // (S-111 surface current arrows). It is turned by `rotation`, or by the point's value of
    // `rotationAttribute` times `rotationFactor`; and scaled by `defaultScaleFactor`, or by the
    // point's value of `scaleAttribute` times `scaleFactor`.
    struct CoverageSymbol
    {
        std::string reference;     // the symbol's name in the portrayal catalogue
        std::string rotationCRS;   // "GeographicCRS": degrees clockwise from north
        double rotation = 0;
        std::optional<std::string> rotationAttribute;
        double rotationFactor = 1;
        double defaultScaleFactor = 1;
        std::optional<std::string> scaleAttribute;
        double scaleFactor = 1;
    };

    // The factor the symbol is drawn at; `value` is the point's value of the scale attribute.
    [[nodiscard]] double symbolScale(const CoverageSymbol& symbol, std::optional<double> value) noexcept;
    // The angle the symbol is turned by, in degrees; `value` is the point's value of the rotation
    // attribute.
    [[nodiscard]] double symbolRotation(const CoverageSymbol& symbol, std::optional<double> value) noexcept;

    struct LookupEntry
    {
        std::string label;
        std::optional<double> lower;
        std::optional<double> upper;
        IntervalClosure closure = IntervalClosure::ClosedInterval;
        std::optional<CoverageColor> color;
        std::optional<CoverageSymbol> symbol;
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
