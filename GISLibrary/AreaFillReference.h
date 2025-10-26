#pragma once

#include "DrawingCommand.h"

namespace Part9a
{
    class AreaFillReference : public DrawingCommand
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
