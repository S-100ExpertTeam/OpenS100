#pragma once

#include "Command.h"
#include "VectorParameter.h"

namespace DrawingCommand
{
    class SymbolFill : public Command
    {
    public:
        SymbolFill() = default;
        SymbolFill(const std::string& symbol, const DrawingCommand::Vector& v1, const DrawingCommand::Vector& v2, bool clipSymbols = true);
        virtual ~SymbolFill() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;
        DrawingCommand::Vector v1;
        DrawingCommand::Vector v2;
        bool clipSymbols = true;
    };
}