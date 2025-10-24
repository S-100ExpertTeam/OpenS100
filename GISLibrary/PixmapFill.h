#pragma once

#include "Command.h"

namespace DrawingCommand
{
    class PixmapFill : public Command
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
