#include "stdafx.h"
#include "PointInstruction.h"

namespace DrawingCommand
{
    // PointInstruction class implementation
    PointInstruction::PointInstruction(const std::string& symbol) : symbol(symbol)
    {
    }

    void PointInstruction::init()
    {
        Command::init();
        symbol.clear();
    }

    void PointInstruction::execute()
    {
    }

    void PointInstruction::parse(const std::string& input)
    {
        setPresent();
        // PointInstruction:symbol 
        symbol = input;
    }

    std::string PointInstruction::GetSymbol() const
    {
        return symbol;
    }
}
