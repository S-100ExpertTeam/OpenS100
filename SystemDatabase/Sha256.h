#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string>

// SHA-256 (FIPS 180-4), used to fingerprint the files the System Database manages so that an
// import can tell a re-issued file from the one already installed.
namespace s100::sdb
{
    class Sha256 final
    {
    public:
        using Digest = std::array<std::uint8_t, 32>;

        Sha256() noexcept;

        void update(std::span<const std::uint8_t> bytes) noexcept;
        // Ends the computation. The object must not be updated afterwards.
        [[nodiscard]] Digest finish() noexcept;

        [[nodiscard]] static std::string toHex(const Digest& digest);

    private:
        void compress(const std::uint8_t* block) noexcept;

        std::array<std::uint32_t, 8> state_;
        std::array<std::uint8_t, 64> buffer_{};
        std::size_t buffered_ = 0;
        std::uint64_t length_ = 0;   // bytes hashed so far
    };

    // Lower-case hex digest of the bytes.
    [[nodiscard]] std::string sha256Hex(std::span<const std::uint8_t> bytes);
    // Lower-case hex digest of the file's contents. Throws std::filesystem::filesystem_error when
    // the file cannot be read.
    [[nodiscard]] std::string sha256File(const std::filesystem::path& file);
}
