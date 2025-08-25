#pragma once

#include "Enum_StateCommand.h"

#include <string>

namespace DrawingInstructions
{
    class Command 
    {
    public:
        Command() = default;
        virtual ~Command() = default;
        virtual Enum_StateCommand GetType() const { return Enum_StateCommand::None; }
		virtual void init() = 0;
        virtual void execute() = 0;
        virtual void parse(const std::string& input) = 0;

    public:
        static GraphicBasePackage::CRSType GetCRSTypeFromString(const std::string& value);
		static IntervalType GetIntervalTypeFromString(const std::string& value);

    public:
        bool present = false;

    public:
        void setPresent(bool value = true);
        bool isPresent() const;
    };
}