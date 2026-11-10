#include "Sha256.h"

#include <bit>
#include <fstream>
#include <system_error>
#include <vector>

namespace s100::sdb
{
    namespace
    {
        constexpr std::array<std::uint32_t, 64> roundConstants = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
        };

        constexpr std::array<std::uint32_t, 8> initialState = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
        };
    }

    Sha256::Sha256() noexcept : state_(initialState)
    {
    }

    void Sha256::compress(const std::uint8_t* block) noexcept
    {
        std::array<std::uint32_t, 64> w{};
        for (std::size_t i = 0; i < 16; ++i)
        {
            w[i] = (std::uint32_t{ block[i * 4] } << 24) | (std::uint32_t{ block[i * 4 + 1] } << 16)
                | (std::uint32_t{ block[i * 4 + 2] } << 8) | std::uint32_t{ block[i * 4 + 3] };
        }
        for (std::size_t i = 16; i < 64; ++i)
        {
            const std::uint32_t s0 = std::rotr(w[i - 15], 7) ^ std::rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
            const std::uint32_t s1 = std::rotr(w[i - 2], 17) ^ std::rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        auto [a, b, c, d, e, f, g, h] = state_;
        for (std::size_t i = 0; i < 64; ++i)
        {
            const std::uint32_t s1 = std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25);
            const std::uint32_t choose = (e & f) ^ (~e & g);
            const std::uint32_t t1 = h + s1 + choose + roundConstants[i] + w[i];
            const std::uint32_t s0 = std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22);
            const std::uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
            const std::uint32_t t2 = s0 + majority;
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        state_[0] += a;
        state_[1] += b;
        state_[2] += c;
        state_[3] += d;
        state_[4] += e;
        state_[5] += f;
        state_[6] += g;
        state_[7] += h;
    }

    void Sha256::update(std::span<const std::uint8_t> bytes) noexcept
    {
        length_ += bytes.size();
        std::size_t offset = 0;
        if (buffered_ > 0)
        {
            const std::size_t take = std::min(bytes.size(), buffer_.size() - buffered_);
            std::copy_n(bytes.begin(), take, buffer_.begin() + static_cast<std::ptrdiff_t>(buffered_));
            buffered_ += take;
            offset = take;
            if (buffered_ < buffer_.size())
            {
                return;
            }
            compress(buffer_.data());
            buffered_ = 0;
        }
        for (; offset + 64 <= bytes.size(); offset += 64)
        {
            compress(bytes.data() + offset);
        }
        const std::size_t rest = bytes.size() - offset;
        std::copy_n(bytes.begin() + static_cast<std::ptrdiff_t>(offset), rest, buffer_.begin());
        buffered_ = rest;
    }

    Sha256::Digest Sha256::finish() noexcept
    {
        const std::uint64_t bitLength = length_ * 8;
        buffer_[buffered_++] = 0x80;
        if (buffered_ > 56)
        {
            std::fill(buffer_.begin() + static_cast<std::ptrdiff_t>(buffered_), buffer_.end(), std::uint8_t{ 0 });
            compress(buffer_.data());
            buffered_ = 0;
        }
        std::fill(buffer_.begin() + static_cast<std::ptrdiff_t>(buffered_), buffer_.begin() + 56, std::uint8_t{ 0 });
        for (std::size_t i = 0; i < 8; ++i)
        {
            buffer_[56 + i] = static_cast<std::uint8_t>(bitLength >> (56 - 8 * i));
        }
        compress(buffer_.data());

        Digest digest{};
        for (std::size_t i = 0; i < state_.size(); ++i)
        {
            digest[i * 4] = static_cast<std::uint8_t>(state_[i] >> 24);
            digest[i * 4 + 1] = static_cast<std::uint8_t>(state_[i] >> 16);
            digest[i * 4 + 2] = static_cast<std::uint8_t>(state_[i] >> 8);
            digest[i * 4 + 3] = static_cast<std::uint8_t>(state_[i]);
        }
        return digest;
    }

    std::string Sha256::toHex(const Digest& digest)
    {
        constexpr std::string_view digits = "0123456789abcdef";
        std::string text;
        text.reserve(digest.size() * 2);
        for (const std::uint8_t byte : digest)
        {
            text.push_back(digits[byte >> 4]);
            text.push_back(digits[byte & 0x0f]);
        }
        return text;
    }

    std::string sha256Hex(std::span<const std::uint8_t> bytes)
    {
        Sha256 hash;
        hash.update(bytes);
        return Sha256::toHex(hash.finish());
    }

    std::string sha256File(const std::filesystem::path& file)
    {
        std::ifstream in(file, std::ios::binary);
        if (!in)
        {
            throw std::filesystem::filesystem_error("cannot read file", file,
                std::make_error_code(std::errc::no_such_file_or_directory));
        }
        Sha256 hash;
        std::vector<char> chunk(1 << 16);
        while (in)
        {
            in.read(chunk.data(), static_cast<std::streamsize>(chunk.size()));
            const auto count = static_cast<std::size_t>(in.gcount());
            if (count > 0)
            {
                hash.update(std::span(reinterpret_cast<const std::uint8_t*>(chunk.data()), count));
            }
        }
        if (in.bad())
        {
            throw std::filesystem::filesystem_error("error while reading file", file,
                std::make_error_code(std::errc::io_error));
        }
        return Sha256::toHex(hash.finish());
    }
}
