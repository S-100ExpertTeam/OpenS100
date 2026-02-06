#pragma once

#include "DrawingCommand.h"

namespace Part9a
{
    class CoverageFill : public DrawingCommand
    {
    public:
        CoverageFill() = default;
        CoverageFill(const std::string& attributeCode, const std::string& uom = "", const std::string& placement = "");
        virtual ~CoverageFill() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string attributeCode;
        std::string uom;
        std::string placement;
    };
}