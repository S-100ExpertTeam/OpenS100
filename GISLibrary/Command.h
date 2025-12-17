#pragma once

#include "Enum_CommandType.h"

#include <string>

namespace Part9a
{
    class Command 
    {
    public:
        Command() = default;
        virtual ~Command() = default;
        virtual Enum_CommandType GetType() const { return Enum_CommandType::None; }
        virtual void init() { present = false; };
        virtual void execute() {};
        virtual void parse(const std::string& input) {};
        virtual bool execute(std::list<Part9a::Command*>& stateCommands) { return false; };

    public:
        static GraphicBasePackage::CRSType GetCRSTypeFromString(const std::string& value);
		static IntervalType GetIntervalTypeFromString(const std::string& value);

    public:
        std::string id;
        bool present = false;
        std::string parameter;

    public:
        void setPresent(bool value = true);
        bool isPresent() const;

		void setParameter(const std::string& value);
        std::string getParameter();

		void setId(const std::string& value);
		std::string getId();
    };
}