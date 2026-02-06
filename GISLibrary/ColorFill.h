#pragma once

#include "DrawingCommand.h"

namespace Part9a
{
    class ColorFill : public DrawingCommand
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
