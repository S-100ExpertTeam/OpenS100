#include "stdafx.h"
#include "NullInstruction.h"

namespace Part9a
{
    void NullInstruction::init()
    {
        Command::init();
        // No specific initialization needed for NullInstruction
    }

    // NullInstruction class implementation
    void NullInstruction::execute() {
    }

    void NullInstruction::parse(const std::string& input)
    {
        setPresent();
        // NullInstruction 
    }
}