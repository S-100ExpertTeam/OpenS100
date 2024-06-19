#pragma once
#include <vector>

#include "VisibilityCommands.h"
#include "TransformCommands.h"
#include "LineStyleCommands.h"
#include "TextStyleCommands.h"
#include "ColourOverrideCommands.h"
#include "GeometryCommands.h"
#include "CoverageCommands.h"
#include "TimeCommands.h"
#include "AlertCommands.h"

namespace DrawingInstruction
{
  

    // StateCommands class
    class StateCommands {
    public:
        ~StateCommands();

        void parse(const std::string& key, std::string value);
        void executeCommands();

    private:
        VisibilityCommands* visibilityCommands = nullptr;
        TransformCommands* transformCommands = nullptr;
        LineStyleCommands* lineStyleCommands = nullptr;
        TextStyleCommands* textStyleCommands = nullptr;
        ColourOverrideCommands* colourOverrideCommands = nullptr;
        GeometryCommands* geometryCommands = nullptr;
        CoverageCommands* coverageCommands = nullptr;
        TimeCommands* timeCommands = nullptr;
        AlertCommands* alertCommands = nullptr;
    };
}
