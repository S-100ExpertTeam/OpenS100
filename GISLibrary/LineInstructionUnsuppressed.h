#pragma	once

#include "Command.h"

#include <vector>

namespace DrawingCommand
{
    class LineInstructionUnsuppressed : public Command
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
