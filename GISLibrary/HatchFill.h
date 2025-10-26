#pragma once

#include "DrawingCommand.h"
#include "VectorParameter.h"

namespace Part9a
{
    class HatchFill : public DrawingCommand
    {
    public:
        HatchFill() = default;
        HatchFill(const Part9a::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2);
        virtual ~HatchFill() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        Part9a::Vector direction;
        double distance = 0.0;
        std::string lineStyle1;
        std::string lineStyle2;
    };
}
