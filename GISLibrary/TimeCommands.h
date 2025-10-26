#pragma once

#include "StateCommand.h"

#include "..\\FeatureCatalog\\IntervalType.h"

namespace Part9a
{
    class Date : public StateCommand {
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

    class Time : public StateCommand {
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

    class DateTime : public StateCommand {
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

    class TimeValid : public StateCommand {
    public:
		TimeValid() = default;
        TimeValid(const IntervalType closure);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        IntervalType closure = IntervalType::none;
    };

    class ClearTime : public StateCommand {
    public:
        ClearTime() = default;

    public:
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;
    };
}