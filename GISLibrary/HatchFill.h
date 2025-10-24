#pragma once

#include "Command.h"
#include "VectorParameter.h"

namespace DrawingCommand
{
    class HatchFill : public Command
    {
    public:
        HatchFill() = default;
        HatchFill(const DrawingCommand::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2);
        virtual ~HatchFill() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        DrawingCommand::Vector direction;
        double distance = 0.0;
        std::string lineStyle1;
        std::string lineStyle2;
    };
}
