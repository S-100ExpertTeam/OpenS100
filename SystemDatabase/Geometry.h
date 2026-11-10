#pragma once

#include "Records.h"

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

// Coverage polygons as the System Database keeps them: longitude/latitude rings, stored as
// well-known binary (ISO 19125 / OGC Simple Features).
namespace s100::sdb
{
    struct Position
    {
        double lon = 0;
        double lat = 0;

        friend bool operator==(const Position&, const Position&) = default;
    };

    using Ring = std::vector<Position>;

    // The rings of one polygon: the exterior first, then the holes. Rings are closed (the last
    // position repeats the first) once encoded; an open ring is closed on encoding.
    using PolygonRings = std::vector<Ring>;

    // Little-endian WKB Polygon (type 3).
    [[nodiscard]] std::vector<std::uint8_t> encodeWkbPolygon(std::span<const Ring> rings);
    // The rings of a WKB Polygon, or of every polygon of a MultiPolygon (type 6) in order. Either
    // byte order. std::nullopt for anything else or for truncated input.
    [[nodiscard]] std::optional<PolygonRings> decodeWkbPolygon(std::span<const std::uint8_t> wkb);

    // The extent of the rings; std::nullopt when they hold no position.
    [[nodiscard]] std::optional<BoundingBox> boundsOf(std::span<const Ring> rings);
    // The union of two extents.
    [[nodiscard]] BoundingBox merge(const BoundingBox& a, const BoundingBox& b);
}
