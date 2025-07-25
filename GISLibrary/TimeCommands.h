#pragma once
#include "StateCommand.h"
namespace DrawingInstructions
{
    class Date : public StateCommand {
    public:
		Date() = default;
        Date(const std::string& begin, const std::string& end);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string begin;
        std::string end;
    };

    class Time : public StateCommand {
    public:
		Time() = default;
        Time(const std::string& begin, const std::string& end);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string begin;
        std::string end;
    };

    class DateTime : public StateCommand {
    public:
		DateTime() = default;
        DateTime(const std::string& begin, const std::string& end);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string begin;
        std::string end;
    };

    class TimeValid : public StateCommand {
    public:
		TimeValid() = default;
        TimeValid(const std::string& closure);
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string closure;
    };

    class ClearTime : public StateCommand {
    public:
        ClearTime();
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;
    };

	class TimeCommands
	{
        public:
		TimeCommands() = default;
		TimeCommands(const TimeCommands&) = delete;
		TimeCommands& operator=(const TimeCommands&) = delete;
		TimeCommands(TimeCommands&&) = delete;
		TimeCommands& operator=(TimeCommands&&) = delete;
		~TimeCommands();

		void setDate(const std::string& begin, const std::string& end);
		void setTime(const std::string& begin, const std::string& end);
		void setDateTime(const std::string& begin, const std::string& end);
		void setTimeValid(const std::string& closure);
		void clearTime();

        void parse(const std::string& key, std::string value);
		void execute() const;

    private:
        //Time Commands
        DrawingInstructions::Date* date = nullptr;
		Time* time = nullptr;
		DateTime* dateTime = nullptr;
		TimeValid* timeValid = nullptr;
		ClearTime* clearTimeCmd = nullptr;
	};

  
}

