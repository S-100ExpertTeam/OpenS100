#include "stdafx.h"
#include "LineInstruction.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingCommand
{
    LineInstruction::LineInstruction(const std::vector<std::string>& lineStyle) : lineStyle(lineStyle)
    {
    }

    void LineInstruction::init()
    {
        Command::init();
        lineStyle.clear();
    }

    void LineInstruction::execute()
    {
    }

    void LineInstruction::parse(const std::string& input)
    {
        setPresent();
        // LineInstruction:lineStyle[,lineStyle,¡¦] 
        lineStyle = LatLonUtility::Split(input, ",");
    }
}
