#include "CoverageInstructions.h"

#include <charconv>

namespace s100::coverage
{
    namespace
    {
        std::vector<std::string> split(std::string_view text, char separator)
        {
            std::vector<std::string> parts;
            std::size_t start = 0;
            while (true)
            {
                const auto at = text.find(separator, start);
                parts.emplace_back(text.substr(start, at == std::string_view::npos ? std::string_view::npos : at - start));
                if (at == std::string_view::npos)
                {
                    return parts;
                }
                start = at + 1;
            }
        }

        std::optional<double> number(std::string_view text)
        {
            while (!text.empty() && text.front() == ' ') text.remove_prefix(1);
            while (!text.empty() && text.back() == ' ') text.remove_suffix(1);
            if (text.empty())
            {
                return std::nullopt;
            }
            if (text.front() == '+') text.remove_prefix(1);
            double value = 0;
            const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
            if (error != std::errc() || end != text.data() + text.size())
            {
                return std::nullopt;
            }
            return value;
        }

        std::string argument(const std::vector<std::string>& arguments, std::size_t index)
        {
            return index < arguments.size() ? arguments[index] : std::string();
        }
    }

    std::vector<CoverageInstruction> parseCoverageInstructions(std::string_view instructions)
    {
        std::vector<CoverageInstruction> result;
        CoverageInstruction base;
        std::optional<CoverageColor> color;
        std::vector<LookupEntry> lookup;

        for (const std::string& item : split(instructions, ';'))
        {
            const auto colon = item.find(':');
            const std::string name = item.substr(0, colon);
            const std::vector<std::string> a = colon == std::string::npos ? std::vector<std::string>() : split(std::string_view(item).substr(colon + 1), ',');
            if (name == "ViewingGroup")
            {
                base.viewingGroups.clear();
                for (const std::string& group : a)
                {
                    if (const auto value = number(group)) base.viewingGroups.push_back(static_cast<int>(*value));
                }
            }
            else if (name == "DrawingPriority")
            {
                base.drawingPriority = static_cast<int>(number(argument(a, 0)).value_or(0));
            }
            else if (name == "DisplayPlane")
            {
                base.displayPlane = argument(a, 0);
            }
            else if (name == "CoverageColor")
            {
                // CoverageColor:startToken[,startTransparency[,endToken,endTransparency[,penWidth]]]
                CoverageColor next{ .startToken = argument(a, 0), .startTransparency = number(argument(a, 1)).value_or(0) };
                if (!argument(a, 2).empty())
                {
                    next.endToken = argument(a, 2);
                    next.endTransparency = number(argument(a, 3)).value_or(0);
                }
                color = next;
            }
            else if (name == "LookupEntry")
            {
                // LookupEntry:label,lower,upper,closure; an empty bound is unbounded.
                const auto closure = intervalClosureFromString(argument(a, 3));
                if (closure)
                {
                    lookup.push_back(LookupEntry{ .label = argument(a, 0), .lower = number(argument(a, 1)), .upper = number(argument(a, 2)),
                        .closure = *closure, .color = color });
                }
                color.reset();
            }
            else if (name == "CoverageFill")
            {
                // CoverageFill:attributeCode[,uom[,placement]]
                if (argument(a, 0).empty())
                {
                    continue;
                }
                CoverageInstruction instruction = base;
                instruction.attributeCode = argument(a, 0);
                if (!argument(a, 1).empty()) instruction.uom = argument(a, 1);
                instruction.lookup = std::move(lookup);
                lookup.clear();
                result.push_back(std::move(instruction));
            }
        }
        return result;
    }
}
