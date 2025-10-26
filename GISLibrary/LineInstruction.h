#pragma once

#include "DrawingCommand.h"

namespace Part9a
{
    class LineInstruction : public DrawingCommand
    {
    public:
        LineInstruction() = default;
        LineInstruction(const std::vector<std::string>& lineStyle);
        virtual ~LineInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> lineStyle;
    };
}
