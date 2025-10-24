#pragma once

#include "Command.h"

namespace DrawingCommand
{
    class TextInstruction : public Command
    {
    public:
        TextInstruction() = default;
        TextInstruction(const std::string& text);
        virtual ~TextInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string text;
    };
}
