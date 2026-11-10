#pragma once

#include <filesystem>
#include <string>
#include <string_view>

// Conversions between std::filesystem::path and the UTF-8 strings the System Database stores.
// On Windows a path is UTF-16 and path::string() would go through the ANSI code page, which loses
// every character outside it; these go through u8string instead.
namespace s100::sdb
{
    [[nodiscard]] inline std::string toUtf8(const std::filesystem::path& path)
    {
        const std::u8string text = path.u8string();
        return std::string(text.begin(), text.end());
    }

    [[nodiscard]] inline std::filesystem::path fromUtf8(std::string_view text)
    {
        return std::filesystem::path(std::u8string(text.begin(), text.end()));
    }

    // The path with '/' separators, as stored in the database.
    [[nodiscard]] inline std::string toGenericUtf8(const std::filesystem::path& path)
    {
        const std::u8string text = path.generic_u8string();
        return std::string(text.begin(), text.end());
    }
}
