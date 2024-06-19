#include "stdafx.h"
#include "DrawingInstruction.h"
#include "DrawingCommands.h"
#include <sstream>


std::shared_ptr<DrawingInstruction> DrawingInstruction::parse(const std::string& input) {
    std::istringstream stream(input);
    std::string commandType;
    stream >> commandType;



    
    return nullptr;
}