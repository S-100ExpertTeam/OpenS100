#pragma once

#include "Command.h"

namespace DrawingCommand
{
    class NullInstruction : public Command
    {
    public:
        NullInstruction() = default;
        virtual ~NullInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;
    };
}