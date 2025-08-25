#pragma once

#include "Command.h"

#include "..\\FeatureCatalog\\IntervalType.h"

namespace DrawingInstructions
{
    class Date : public Command {
    public:
		Date() = default;
        Date(const std::string& begin, const std::string& end);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string begin = "-…";
        std::string end = "…";
    };

    class Time : public Command {
    public:
		Time() = default;
        Time(const std::string& begin, const std::string& end);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string begin = "-…";
        std::string end = "…";
    };

    class DateTime : public Command {
    public:
		DateTime() = default;
        DateTime(const std::string& begin, const std::string& end);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string begin = "-…";
        std::string end = "…";
    };

    class TimeValid : public Command {
    public:
		TimeValid() = default;
        TimeValid(const IntervalType closure);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        IntervalType closure = IntervalType::none;
    };

    class ClearTime : public Command {
    public:
        ClearTime() = default;

    public:
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;
    };
}