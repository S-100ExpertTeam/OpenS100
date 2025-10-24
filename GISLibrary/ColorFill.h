#pragma once

#include "Command.h"

namespace DrawingCommand
{
    class ColorFill : public Command
    {
    public:
        ColorFill() = default;
        ColorFill(const std::string& token, double transparency = 0.0);
        virtual ~ColorFill() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency = 0.0;
    };
}
