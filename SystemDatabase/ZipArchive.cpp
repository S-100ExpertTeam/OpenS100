#include "ZipArchive.h"

#include <zlib.h>

#include <algorithm>
#include <format>
#include <limits>

namespace s100::sdb
{
    namespace
    {
        constexpr std::uint32_t localHeaderSignature = 0x04034b50;
        constexpr std::uint32_t centralHeaderSignature = 0x02014b50;
        constexpr std::uint32_t endOfDirectorySignature = 0x06054b50;
        constexpr std::size_t localHeaderSize = 30;
        constexpr std::size_t centralHeaderSize = 46;
        constexpr std::size_t endOfDirectorySize = 22;
        constexpr std::uint16_t methodStored = 0;
        constexpr std::uint16_t methodDeflate = 8;
        constexpr std::uint16_t flagEncrypted = 0x0001;

        std::uint16_t le16(std::span<const std::uint8_t> bytes, std::size_t at)
        {
            return static_cast<std::uint16_t>(bytes[at] | (bytes[at + 1] << 8));
        }

        std::uint32_t le32(std::span<const std::uint8_t> bytes, std::size_t at)
        {
            return static_cast<std::uint32_t>(bytes[at]) | (static_cast<std::uint32_t>(bytes[at + 1]) << 8)
                | (static_cast<std::uint32_t>(bytes[at + 2]) << 16) | (static_cast<std::uint32_t>(bytes[at + 3]) << 24);
        }

        void require(bool condition, const char* what)
        {
            if (!condition)
            {
                throw ZipError(what);
            }
        }

        std::vector<std::uint8_t> inflateRaw(std::span<const std::uint8_t> input, std::uint64_t expectedSize)
        {
            require(expectedSize <= std::numeric_limits<std::size_t>::max(), "zip entry too large");
            std::vector<std::uint8_t> output(static_cast<std::size_t>(expectedSize));

            z_stream stream{};
            // Negative window bits: a raw deflate stream, as ZIP stores it (no zlib header).
            require(inflateInit2(&stream, -MAX_WBITS) == Z_OK, "zlib initialisation failed");

            stream.next_in = const_cast<Bytef*>(input.data());   // zlib does not write to its input
            stream.avail_in = static_cast<uInt>(input.size());
            stream.next_out = output.data();
            stream.avail_out = static_cast<uInt>(output.size());
            const int status = inflate(&stream, Z_FINISH);
            const auto produced = stream.total_out;
            inflateEnd(&stream);

            require(status == Z_STREAM_END, "zip entry is not a valid deflate stream");
            require(produced == expectedSize, "zip entry inflates to a size other than recorded");
            return output;
        }
    }

    namespace
    {
        std::uint64_t le64(std::span<const std::uint8_t> bytes, std::size_t at)
        {
            return static_cast<std::uint64_t>(le32(bytes, at)) | (static_cast<std::uint64_t>(le32(bytes, at + 4)) << 32);
        }

        // A raw deflate stream whose inflated size is not known: grows the output until the
        // stream ends. Returns the bytes and how much input the stream took.
        std::vector<std::uint8_t> inflateUnsized(std::span<const std::uint8_t> input)
        {
            constexpr std::size_t limit = std::size_t{ 4 } << 30;   // 4 GiB: no S-100 file is near it
            std::vector<std::uint8_t> output(std::max<std::size_t>(input.size() * 4, 64 * 1024));

            z_stream stream{};
            require(inflateInit2(&stream, -MAX_WBITS) == Z_OK, "zlib initialisation failed");
            stream.next_in = const_cast<Bytef*>(input.data());   // zlib does not write to its input
            stream.avail_in = static_cast<uInt>(std::min<std::size_t>(input.size(), std::numeric_limits<uInt>::max()));
            int status = Z_OK;
            while (status == Z_OK)
            {
                if (stream.total_out == output.size())
                {
                    if (output.size() >= limit)
                    {
                        break;
                    }
                    output.resize(std::min(output.size() * 2, limit));
                }
                stream.next_out = output.data() + stream.total_out;
                stream.avail_out = static_cast<uInt>(std::min<std::size_t>(output.size() - stream.total_out, std::numeric_limits<uInt>::max()));
                status = inflate(&stream, Z_NO_FLUSH);
                if (status == Z_BUF_ERROR && stream.avail_out == 0)
                {
                    status = Z_OK;   // only out of room: grow and go on
                }
            }
            const auto produced = static_cast<std::size_t>(stream.total_out);
            inflateEnd(&stream);
            require(status == Z_STREAM_END, "zip entry is not a valid (or a complete) deflate stream");
            output.resize(produced);
            return output;
        }
    }

    ZipArchive::LocalFile ZipArchive::readFirstLocalFile(std::span<const std::uint8_t> bytes)
    {
        require(bytes.size() >= localHeaderSize && le32(bytes, 0) == localHeaderSignature, "no zip local file header");
        const std::uint16_t flags = le16(bytes, 6);
        const std::uint16_t method = le16(bytes, 8);
        const std::uint32_t crc = le32(bytes, 14);
        std::uint64_t compressedSize = le32(bytes, 18);
        std::uint64_t size = le32(bytes, 22);
        const std::uint16_t nameLength = le16(bytes, 26);
        const std::uint16_t extraLength = le16(bytes, 28);
        require((flags & flagEncrypted) == 0, "encrypted zip entries are not supported");
        require(localHeaderSize + std::size_t{ nameLength } + extraLength <= bytes.size(), "zip local header out of range");

        // ZIP64: sizes of 0xFFFFFFFF are in extra field 0x0001 (uncompressed first, then compressed).
        for (std::size_t at = localHeaderSize + nameLength, end = at + extraLength; at + 4 <= end;)
        {
            const std::uint16_t id = le16(bytes, at);
            const std::uint16_t length = le16(bytes, at + 2);
            if (id == 0x0001)
            {
                std::size_t field = at + 4;
                if (size == 0xFFFFFFFF && field + 8 <= at + 4 + length)
                {
                    size = le64(bytes, field);
                    field += 8;
                }
                if (compressedSize == 0xFFFFFFFF && field + 8 <= at + 4 + length)
                {
                    compressedSize = le64(bytes, field);
                }
            }
            at += 4 + std::size_t{ length };
        }

        LocalFile file;
        file.name.assign(reinterpret_cast<const char*>(bytes.data() + localHeaderSize), nameLength);
        const auto data = bytes.subspan(localHeaderSize + nameLength + extraLength);
        // Bit 3: sizes and CRC follow the data instead, so they are unknown here.
        const bool sizesKnown = (flags & 0x0008) == 0 && size != 0xFFFFFFFF && compressedSize != 0xFFFFFFFF;
        switch (method)
        {
        case methodStored:
            require(sizesKnown && compressedSize == size && size <= data.size(), "stored zip entry without its size");
            file.content.assign(data.begin(), data.begin() + static_cast<std::ptrdiff_t>(size));
            break;
        case methodDeflate:
            file.content = inflateUnsized(sizesKnown && compressedSize <= data.size() ? data.first(static_cast<std::size_t>(compressedSize)) : data);
            require(!sizesKnown || file.content.size() == size, "zip entry inflates to a size other than recorded");
            break;
        default:
            throw ZipError(std::format("zip compression method {} is not supported", method));
        }
        require(!sizesKnown || computeCrc32(file.content) == crc, "zip entry fails its CRC-32 check");
        return file;
    }

    std::uint32_t computeCrc32(std::span<const std::uint8_t> bytes) noexcept
    {
        uLong crc = ::crc32(0L, Z_NULL, 0);
        std::size_t at = 0;
        while (at < bytes.size())
        {
            const auto chunk = static_cast<uInt>(std::min<std::size_t>(bytes.size() - at, std::numeric_limits<uInt>::max()));
            crc = ::crc32(crc, bytes.data() + at, chunk);
            at += chunk;
        }
        return static_cast<std::uint32_t>(crc);
    }

    bool ZipArchive::looksLikeZip(std::span<const std::uint8_t> bytes) noexcept
    {
        return bytes.size() >= 4
            && (le32(bytes, 0) == localHeaderSignature || le32(bytes, 0) == endOfDirectorySignature);
    }

    ZipArchive::ZipArchive(std::span<const std::uint8_t> bytes) : bytes_(bytes)
    {
        require(bytes.size() >= endOfDirectorySize, "not a zip archive (too short)");

        // The end-of-central-directory record is the last 22 bytes plus a comment of up to 64 KiB.
        std::size_t end = std::numeric_limits<std::size_t>::max();
        const std::size_t lowest = bytes.size() > endOfDirectorySize + 0xFFFF ? bytes.size() - endOfDirectorySize - 0xFFFF : 0;
        for (std::size_t at = bytes.size() - endOfDirectorySize + 1; at-- > lowest;)
        {
            if (le32(bytes, at) == endOfDirectorySignature)
            {
                end = at;
                break;
            }
        }
        require(end != std::numeric_limits<std::size_t>::max(), "not a zip archive (no end of central directory)");

        require(le16(bytes, end + 4) == 0 && le16(bytes, end + 6) == 0, "multi-disk zip archives are not supported");
        const std::uint16_t count = le16(bytes, end + 10);
        const std::uint32_t directorySize = le32(bytes, end + 12);
        const std::uint32_t directoryOffset = le32(bytes, end + 16);
        require(count != 0xFFFF && directoryOffset != 0xFFFFFFFF, "ZIP64 archives are not supported");
        require(static_cast<std::uint64_t>(directoryOffset) + directorySize <= end, "zip central directory out of range");

        entries_.reserve(count);
        std::size_t at = directoryOffset;
        for (std::uint16_t i = 0; i < count; ++i)
        {
            require(at + centralHeaderSize <= end && le32(bytes, at) == centralHeaderSignature, "zip central directory is damaged");
            const std::uint16_t flags = le16(bytes, at + 8);
            const std::uint16_t nameLength = le16(bytes, at + 28);
            const std::uint16_t extraLength = le16(bytes, at + 30);
            const std::uint16_t commentLength = le16(bytes, at + 32);
            require(at + centralHeaderSize + nameLength <= end, "zip central directory is damaged");

            Entry entry;
            entry.method = le16(bytes, at + 10);
            entry.crc32 = le32(bytes, at + 16);
            entry.compressedSize = le32(bytes, at + 20);
            entry.size = le32(bytes, at + 24);
            entry.localHeaderOffset = le32(bytes, at + 42);
            entry.name.assign(reinterpret_cast<const char*>(bytes.data() + at + centralHeaderSize), nameLength);
            require((flags & flagEncrypted) == 0, "encrypted zip entries are not supported");
            require(entry.compressedSize != 0xFFFFFFFF && entry.size != 0xFFFFFFFF && entry.localHeaderOffset != 0xFFFFFFFF,
                "ZIP64 archives are not supported");

            entries_.push_back(std::move(entry));
            at += centralHeaderSize + nameLength + extraLength + commentLength;
        }
    }

    std::vector<std::uint8_t> ZipArchive::read(const Entry& entry) const
    {
        const std::uint64_t header = entry.localHeaderOffset;
        require(header + localHeaderSize <= bytes_.size(), "zip entry out of range");
        const auto at = static_cast<std::size_t>(header);
        require(le32(bytes_, at) == localHeaderSignature, "zip local header is damaged");

        // The local header's own name and extra lengths place the data; they may differ from the
        // central directory's.
        const std::uint64_t dataStart = header + localHeaderSize + le16(bytes_, at + 26) + le16(bytes_, at + 28);
        require(dataStart + entry.compressedSize <= bytes_.size(), "zip entry data out of range");
        const auto data = bytes_.subspan(static_cast<std::size_t>(dataStart), static_cast<std::size_t>(entry.compressedSize));

        std::vector<std::uint8_t> content;
        switch (entry.method)
        {
        case methodStored:
            require(entry.compressedSize == entry.size, "stored zip entry has inconsistent sizes");
            content.assign(data.begin(), data.end());
            break;
        case methodDeflate:
            content = inflateRaw(data, entry.size);
            break;
        default:
            throw ZipError(std::format("zip compression method {} is not supported", entry.method));
        }

        require(computeCrc32(content) == entry.crc32, "zip entry fails its CRC-32 check");
        return content;
    }
}
