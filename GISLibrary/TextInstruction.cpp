#include "stdafx.h"
#include "TextInstruction.h"

namespace DrawingCommand
{
    // TextInstruction class implementation
    TextInstruction::TextInstruction(const std::string& text) : text(text) {}

    void TextInstruction::init()
    {
        Command::init();
        text.clear();
    }

    void TextInstruction::execute() {
    }

    void TextInstruction::parse(const std::string& input)
    {
        setPresent();
        // TextInstruction:text 
        text = input;
    }
}