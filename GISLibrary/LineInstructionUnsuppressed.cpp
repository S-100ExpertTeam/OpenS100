#include "stdafx.h"
#include "LineInstructionUnsuppressed.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
    LineInstructionUnsuppressed::LineInstructionUnsuppressed(const std::vector<std::string>& lineStyle) : lineStyle(lineStyle) {}

    void LineInstructionUnsuppressed::init()
    {
        Command::init();
        lineStyle.clear();
    }

    void LineInstructionUnsuppressed::execute()
    {
    }

    void LineInstructionUnsuppressed::parse(const std::string& input)
    {
        setPresent();
        // LineInstructionUnsuppressed:lineStyle[,lineStyle,¡¦] 
        lineStyle = LatLonUtility::Split(input, ",");
    }
}
