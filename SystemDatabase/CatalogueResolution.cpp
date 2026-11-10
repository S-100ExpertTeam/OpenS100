#include "CatalogueResolution.h"

#include <algorithm>
#include <cctype>
#include <charconv>

namespace s100::sdb
{
    namespace
    {
        bool isDigit(char c)
        {
            return std::isdigit(static_cast<unsigned char>(c)) != 0;
        }
    }

    std::string productFromIdentifier(std::string_view text)
    {
        const auto threeDigitsAt = [&](std::size_t at) {
            return at + 3 <= text.size() && isDigit(text[at]) && isDigit(text[at + 1]) && isDigit(text[at + 2])
                && (at + 3 == text.size() || !isDigit(text[at + 3]));
        };
        for (std::size_t i = 0; i < text.size(); ++i)
        {
            if ((text[i] != 'S' && text[i] != 's') || (i > 0 && std::isalnum(static_cast<unsigned char>(text[i - 1]))))
            {
                continue;
            }
            std::size_t at = i + 1;
            if (at < text.size() && text[at] == '-')
            {
                ++at;
            }
            if (threeDigitsAt(at))
            {
                return "S-" + std::string(text.substr(at, 3));
            }
        }
        if (text.size() == 3 && threeDigitsAt(0))
        {
            return "S-" + std::string(text);
        }
        return {};
    }

    std::optional<std::vector<int>> parseVersion(std::string_view text)
    {
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) text.remove_prefix(1);
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.remove_suffix(1);
        if (text.empty())
        {
            return std::nullopt;
        }
        std::vector<int> parts;
        while (true)
        {
            const auto dot = text.find('.');
            const std::string_view part = text.substr(0, dot);
            int value = 0;
            const auto [end, error] = std::from_chars(part.data(), part.data() + part.size(), value);
            if (part.empty() || error != std::errc() || end != part.data() + part.size())
            {
                return std::nullopt;
            }
            parts.push_back(value);
            if (dot == std::string_view::npos)
            {
                break;
            }
            text.remove_prefix(dot + 1);
        }
        while (parts.size() < 3)
        {
            parts.push_back(0);
        }
        return parts;
    }

    int compareVersions(std::string_view a, std::string_view b)
    {
        const auto left = parseVersion(a);
        const auto right = parseVersion(b);
        if (!left || !right)
        {
            return (left ? 1 : 0) - (right ? 1 : 0);
        }
        const std::size_t count = std::max(left->size(), right->size());
        for (std::size_t i = 0; i < count; ++i)
        {
            const int l = i < left->size() ? (*left)[i] : 0;
            const int r = i < right->size() ? (*right)[i] : 0;
            if (l != r)
            {
                return l < r ? -1 : 1;
            }
        }
        return 0;
    }

    bool sameMajorVersion(std::string_view a, std::string_view b)
    {
        const auto left = parseVersion(a);
        const auto right = parseVersion(b);
        return left && right && left->front() == right->front();
    }

    CatalogueResolution resolveCatalogue(std::string_view product, std::string_view version,
        std::span<const CatalogueRecord> installed)
    {
        const std::string wanted = productFromIdentifier(product);
        std::vector<const CatalogueRecord*> candidates;
        for (const CatalogueRecord& record : installed)
        {
            if (!wanted.empty() && productFromIdentifier(record.product) == wanted)
            {
                candidates.push_back(&record);
            }
        }
        std::ranges::stable_sort(candidates, [](const CatalogueRecord* a, const CatalogueRecord* b) {
            return compareVersions(a->version, b->version) > 0;   // newest first
        });

        CatalogueResolution resolution;
        if (candidates.empty())
        {
            return resolution;
        }
        if (!parseVersion(version))
        {
            resolution.match = CatalogueMatch::Fallback;
            resolution.reason = FallbackReason::UnknownVersion;
            resolution.catalogue = *candidates.front();
            return resolution;
        }
        for (const CatalogueRecord* record : candidates)
        {
            if (compareVersions(record->version, version) == 0)
            {
                resolution.match = CatalogueMatch::Ready;
                resolution.catalogue = *record;
                return resolution;
            }
        }
        for (const CatalogueRecord* record : candidates)
        {
            if (sameMajorVersion(record->version, version))
            {
                resolution.match = CatalogueMatch::Fallback;
                resolution.reason = FallbackReason::Version;
                resolution.catalogue = *record;
                return resolution;
            }
        }
        for (const CatalogueRecord* record : candidates)
        {
            resolution.available.push_back(record->version);
        }
        return resolution;
    }
}
