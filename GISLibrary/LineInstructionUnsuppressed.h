#pragma	once

#include "DrawingCommand.h"

#include <vector>

namespace Part9a
{
    class LineInstructionUnsuppressed : public DrawingCommand
    {
    public:
        LineInstructionUnsuppressed() = default;
        LineInstructionUnsuppressed(const std::vector<std::string>& lineStyle);
        virtual ~LineInstructionUnsuppressed() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> lineStyle;
    };
}
