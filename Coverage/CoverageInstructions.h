#pragma once

#include "CoverageLookup.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

// The coverage drawing instructions of S-100 Part 9a, as a portrayal catalogue's rules emit
// them for a coverage feature: "ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRadar;
// CoverageColor:DEPIT,0;LookupEntry:Intertidal,,0,ltSemiInterval;...;CoverageFill:depth"
// (opens100-web `packages/portrayal/src/parse-lua-instructions.ts`).
namespace s100::coverage
{
    struct CoverageInstruction
    {
        std::vector<int> viewingGroups;
        int drawingPriority = 0;
        std::string displayPlane = "UnderRadar";
        std::string attributeCode;   // the value member coloured: "depth"
        std::optional<std::string> uom;
        std::vector<LookupEntry> lookup;
    };

    // The CoverageFill instructions in `instructions`. A CoverageColor is taken by the next
    // LookupEntry; entries gather until CoverageFill. Instructions of other kinds are ignored; a
    // LookupEntry with an unknown interval type is left out.
    [[nodiscard]] std::vector<CoverageInstruction> parseCoverageInstructions(std::string_view instructions);
}
