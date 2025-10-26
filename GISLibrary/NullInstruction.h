#pragma once

#include "DrawingCommand.h"

namespace Part9a
{
    class NullInstruction : public DrawingCommand
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