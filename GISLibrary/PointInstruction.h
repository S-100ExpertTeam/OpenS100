#pragma once

#include "DrawingCommand.h"

namespace Part9a
{

    class PointInstruction : public DrawingCommand
    {
    public:
        PointInstruction() = default;
        PointInstruction(const std::string& symbol);
        virtual ~PointInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;

    public:
        std::string GetSymbol() const;
    };

}