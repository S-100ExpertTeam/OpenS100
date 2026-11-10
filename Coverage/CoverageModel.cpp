#include "CoverageModel.h"

#include <cctype>
#include <charconv>

namespace s100::coverage
{
    std::optional<double> CoverageDataset::numberAttribute(std::string_view key) const
    {
        const auto found = metadata.find(key);
        if (found == metadata.end())
        {
            return std::nullopt;
        }
        if (const auto* number = std::get_if<double>(&found->second))
        {
            return *number;
        }
        const auto& text = std::get<std::string>(found->second);
        double value = 0;
        const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
        if (error != std::errc() || end == text.data())
        {
            return std::nullopt;
        }
        return value;
    }

    std::optional<std::string> CoverageDataset::textAttribute(std::string_view key) const
    {
        const auto found = metadata.find(key);
        if (found == metadata.end())
        {
            return std::nullopt;
        }
        if (const auto* text = std::get_if<std::string>(&found->second))
        {
            return *text;
        }
        std::string text = std::to_string(std::get<double>(found->second));
        return text;
    }

    std::optional<std::string> productIdFromSpecification(std::string_view specification)
    {
        // /S-?(\d{3})/i: the first S, an optional hyphen, three digits.
        const auto digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; };
        for (std::size_t i = 0; i < specification.size(); ++i)
        {
            if (specification[i] != 'S' && specification[i] != 's')
            {
                continue;
            }
            std::size_t at = i + 1;
            if (at < specification.size() && specification[at] == '-')
            {
                ++at;
            }
            if (at + 3 <= specification.size() && digit(specification[at]) && digit(specification[at + 1]) && digit(specification[at + 2]))
            {
                return "S-" + std::string(specification.substr(at, 3));
            }
        }
        return std::nullopt;
    }
}
