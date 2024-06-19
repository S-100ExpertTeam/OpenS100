#pragma once
#include <string>
#include <memory>

class DrawingInstruction
{
public:
    virtual void execute() const = 0;
    static std::shared_ptr<DrawingInstruction> parse(const std::string& input);
};


