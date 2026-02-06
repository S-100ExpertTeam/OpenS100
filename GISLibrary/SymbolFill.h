#pragma once

#include "DrawingCommand.h"
#include "VectorParameter.h"

namespace Part9a
{
    class SymbolFill : public DrawingCommand
    {
    public:
        SymbolFill() = default;
        SymbolFill(const std::string& symbol, const Part9a::Vector& v1, const Part9a::Vector& v2, bool clipSymbols = true);
        virtual ~SymbolFill() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;
        Part9a::Vector v1;
        Part9a::Vector v2;
        bool clipSymbols = true;
    };
}