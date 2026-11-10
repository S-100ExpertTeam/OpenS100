#include "Geometry.h"

#include <algorithm>
#include <bit>
#include <limits>

namespace s100::sdb
{
    namespace
    {
        constexpr std::uint32_t wkbPolygon = 3;
        constexpr std::uint32_t wkbMultiPolygon = 6;

        class Writer
        {
        public:
            void byte(std::uint8_t value) { bytes_.push_back(value); }

            void uint32(std::uint32_t value)
            {
                for (int i = 0; i < 4; ++i)
                {
                    bytes_.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
                }
            }

            void real(double value) { uint64(std::bit_cast<std::uint64_t>(value)); }

            [[nodiscard]] std::vector<std::uint8_t> take() { return std::move(bytes_); }

        private:
            void uint64(std::uint64_t value)
            {
                for (int i = 0; i < 8; ++i)
                {
                    bytes_.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
                }
            }

            std::vector<std::uint8_t> bytes_;
        };

        class Reader
        {
        public:
            explicit Reader(std::span<const std::uint8_t> bytes) : bytes_(bytes) {}

            [[nodiscard]] bool byteOrder()
            {
                if (offset_ >= bytes_.size() || bytes_[offset_] > 1)
                {
                    return false;
                }
                littleEndian_ = bytes_[offset_++] == 1;
                return true;
            }

            [[nodiscard]] std::optional<std::uint32_t> uint32()
            {
                auto value = read(4);
                return value ? std::optional(static_cast<std::uint32_t>(*value)) : std::nullopt;
            }

            [[nodiscard]] std::optional<double> real()
            {
                auto value = read(8);
                return value ? std::optional(std::bit_cast<double>(*value)) : std::nullopt;
            }

            [[nodiscard]] std::size_t remaining() const noexcept { return bytes_.size() - offset_; }

        private:
            std::optional<std::uint64_t> read(std::size_t size)
            {
                if (remaining() < size)
                {
                    return std::nullopt;
                }
                std::uint64_t value = 0;
                for (std::size_t i = 0; i < size; ++i)
                {
                    const std::size_t shift = littleEndian_ ? i : size - 1 - i;
                    value |= std::uint64_t{ bytes_[offset_ + i] } << (8 * shift);
                }
                offset_ += size;
                return value;
            }

            std::span<const std::uint8_t> bytes_;
            std::size_t offset_ = 0;
            bool littleEndian_ = true;
        };

        bool readPolygon(Reader& reader, PolygonRings& rings)
        {
            const auto ringCount = reader.uint32();
            // Each ring needs at least its 4-byte point count: refuse counts the input cannot hold.
            if (!ringCount || *ringCount > reader.remaining() / 4)
            {
                return false;
            }
            for (std::uint32_t r = 0; r < *ringCount; ++r)
            {
                const auto pointCount = reader.uint32();
                if (!pointCount || *pointCount > reader.remaining() / 16)
                {
                    return false;
                }
                Ring ring;
                ring.reserve(*pointCount);
                for (std::uint32_t p = 0; p < *pointCount; ++p)
                {
                    const auto x = reader.real();
                    const auto y = reader.real();
                    if (!x || !y)
                    {
                        return false;
                    }
                    ring.push_back(Position{ *x, *y });
                }
                rings.push_back(std::move(ring));
            }
            return true;
        }
    }

    std::vector<std::uint8_t> encodeWkbPolygon(std::span<const Ring> rings)
    {
        Writer writer;
        writer.byte(1);   // little endian
        writer.uint32(wkbPolygon);
        writer.uint32(static_cast<std::uint32_t>(rings.size()));
        for (const Ring& ring : rings)
        {
            const bool closed = !ring.empty() && ring.front() == ring.back();
            const std::size_t count = ring.size() + (closed || ring.empty() ? 0 : 1);
            writer.uint32(static_cast<std::uint32_t>(count));
            for (const Position& position : ring)
            {
                writer.real(position.lon);
                writer.real(position.lat);
            }
            if (!closed && !ring.empty())
            {
                writer.real(ring.front().lon);
                writer.real(ring.front().lat);
            }
        }
        return writer.take();
    }

    std::optional<PolygonRings> decodeWkbPolygon(std::span<const std::uint8_t> wkb)
    {
        Reader reader(wkb);
        if (!reader.byteOrder())
        {
            return std::nullopt;
        }
        const auto type = reader.uint32();
        PolygonRings rings;
        if (type == wkbPolygon)
        {
            if (!readPolygon(reader, rings))
            {
                return std::nullopt;
            }
            return rings;
        }
        if (type == wkbMultiPolygon)
        {
            const auto count = reader.uint32();
            if (!count)
            {
                return std::nullopt;
            }
            for (std::uint32_t i = 0; i < *count; ++i)
            {
                // Every member carries its own byte order and type.
                if (!reader.byteOrder() || reader.uint32() != wkbPolygon || !readPolygon(reader, rings))
                {
                    return std::nullopt;
                }
            }
            return rings;
        }
        return std::nullopt;
    }

    std::optional<BoundingBox> boundsOf(std::span<const Ring> rings)
    {
        constexpr double infinity = std::numeric_limits<double>::infinity();
        BoundingBox box{ infinity, infinity, -infinity, -infinity };
        bool any = false;
        for (const Ring& ring : rings)
        {
            for (const Position& position : ring)
            {
                box.minX = std::min(box.minX, position.lon);
                box.minY = std::min(box.minY, position.lat);
                box.maxX = std::max(box.maxX, position.lon);
                box.maxY = std::max(box.maxY, position.lat);
                any = true;
            }
        }
        return any ? std::optional(box) : std::nullopt;
    }

    BoundingBox merge(const BoundingBox& a, const BoundingBox& b)
    {
        return BoundingBox{ std::min(a.minX, b.minX), std::min(a.minY, b.minY), std::max(a.maxX, b.maxX), std::max(a.maxY, b.maxY) };
    }
}
