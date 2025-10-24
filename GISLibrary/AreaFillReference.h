#pragma once

#include "Command.h"

namespace DrawingCommand
{
    class AreaFillReference : public Command
    {
    public:
        AreaFillReference() = default;
        AreaFillReference(const std::string& reference);
        virtual ~AreaFillReference() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };
}
