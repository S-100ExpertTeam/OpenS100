#pragma once

#include "Records.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// Which installed catalogues portray a dataset. A dataset names its product and the edition of the
// product specification it follows; catalogues are issued for an edition too. Same rules as the
// web viewer (opens100-web packages/sdk/src/catalogue-registry.ts):
//
// - Ready:      catalogues of the very version the dataset follows.
// - Fallback:   none of that version, but some of the same major version, the newest of which
//               portray it (a minor or patch edition changes no model the rules rely on); or the
//               dataset does not say its version, and the newest catalogues portray it.
// - Unresolved: no catalogues of that product, or none of its major version. The dataset is
//               kept; it is portrayed once catalogues for it are installed.
namespace s100::sdb
{
    // "S-101", "S101", "101", "INT.IHO.S-101.2.0" -> "S-101"; empty for anything else.
    [[nodiscard]] std::string productFromIdentifier(std::string_view text);

    // "2.0" and "2.0.0" alike as {2, 0, 0}; std::nullopt for text that is not dotted numbers.
    [[nodiscard]] std::optional<std::vector<int>> parseVersion(std::string_view text);
    // Negative, zero or positive as a is older than, the same as or newer than b. Text that is not
    // a version sorts before every version.
    [[nodiscard]] int compareVersions(std::string_view a, std::string_view b);
    // Whether the first numbers are equal.
    [[nodiscard]] bool sameMajorVersion(std::string_view a, std::string_view b);

    enum class CatalogueMatch
    {
        Ready,
        Fallback,
        Unresolved,
    };

    enum class FallbackReason
    {
        None,
        Version,          // another version of the same major
        UnknownVersion,   // the dataset says no version
    };

    struct CatalogueResolution
    {
        CatalogueMatch match = CatalogueMatch::Unresolved;
        FallbackReason reason = FallbackReason::None;
        std::optional<CatalogueRecord> catalogue;   // set unless unresolved
        std::vector<std::string> available;         // unresolved: the product's versions, newest first
    };

    // `version` empty: the dataset does not say. `installed` may hold other products; they are
    // passed over.
    [[nodiscard]] CatalogueResolution resolveCatalogue(std::string_view product, std::string_view version,
        std::span<const CatalogueRecord> installed);
}
