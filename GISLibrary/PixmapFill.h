#pragma once

#include "DrawingCommand.h"

namespace Part9a
{
    class PixmapFill : public DrawingCommand
    {
    public:
        PixmapFill() = default;
        PixmapFill(const std::string& reference);
        virtual ~PixmapFill() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };
}
