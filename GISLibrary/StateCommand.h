#pragma once

#include <string>

namespace DrawingInstructions
{
    class StateCommand 
    {
    public:
        StateCommand() = default;
        virtual ~StateCommand() = default;
		virtual void init() = 0;
        virtual void execute() = 0;
        virtual void parse(const std::string& input) = 0;

    public:
        static GraphicBasePackage::CRSType GetCRSTypeFromString(const std::string& value);
		static IntervalType GetIntervalTypeFromString(const std::string& value);
    };
}