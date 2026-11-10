#pragma once

#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

// A minimal reader for ZIP archives held in memory: the containers S-100 Part 15 wraps every
// protected file in before it is encrypted, and the archives exchange sets are delivered in.
// Reads the central directory, so archives written with data descriptors (Java's ZipOutputStream,
// which leaves the sizes in the local headers zero) are read correctly. Supports the "stored" and
// "deflate" methods; ZIP64, encryption and multi-disk archives are rejected.
namespace s100::sdb
{
    class ZipError final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class ZipArchive final
    {
    public:
        struct Entry
        {
            std::string name;   // as recorded, '/' separated (UTF-8 when the archive says so)
            std::uint16_t method = 0;
            std::uint32_t crc32 = 0;
            std::uint64_t compressedSize = 0;
            std::uint64_t size = 0;
            std::uint64_t localHeaderOffset = 0;

            [[nodiscard]] bool isDirectory() const noexcept { return !name.empty() && name.back() == '/'; }
        };

        // Reads the directory of `bytes`, which must outlive the archive. Throws ZipError when
        // the bytes are not a ZIP archive this reader supports.
        explicit ZipArchive(std::span<const std::uint8_t> bytes);

        [[nodiscard]] const std::vector<Entry>& entries() const noexcept { return entries_; }

        // The uncompressed content of an entry, checked against its CRC-32. Throws ZipError.
        [[nodiscard]] std::vector<std::uint8_t> read(const Entry& entry) const;

        // Whether the bytes start like a ZIP archive (a local file header or an empty archive).
        [[nodiscard]] static bool looksLikeZip(std::span<const std::uint8_t> bytes) noexcept;

        // The first entry of an archive read from its local header alone, for archives that end
        // without a central directory (written as a stream and never finished, as in KHOA's 2020
        // S-100 Part 15 test data). Reads ZIP64 sizes from the local header's extra field.
        // Throws ZipError.
        struct LocalFile
        {
            std::string name;
            std::vector<std::uint8_t> content;
        };
        [[nodiscard]] static LocalFile readFirstLocalFile(std::span<const std::uint8_t> bytes);

    private:
        std::span<const std::uint8_t> bytes_;
        std::vector<Entry> entries_;
    };

    // CRC-32 (ISO 3309, the one ZIP uses) of the bytes.
    [[nodiscard]] std::uint32_t computeCrc32(std::span<const std::uint8_t> bytes) noexcept;
}
