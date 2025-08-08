#pragma once
#include "StateCommand.h"

namespace DrawingInstructions
{
	class AlertReference : public StateCommand {
	public:
		AlertReference() = default;
		AlertReference(std::string alertReference, std::string plan, std::string monitor);
		virtual void init() override;
		virtual void execute() override;
		virtual void parse(const std::string& input) override;
	private:
		std::string alertReference;
		std::string plan;
		std::string monitor;
	};

	class AlertCommands
	{
		public:
		AlertCommands() = default;
		AlertCommands(const AlertCommands&) = delete;
		AlertCommands& operator=(const AlertCommands&) = delete;
		AlertCommands(AlertCommands&&) = delete;
		AlertCommands& operator=(AlertCommands&&) = delete;
		~AlertCommands();

		void setAlertReference(const std::string& alertReference, const std::string& plan, const std::string& monitor);

		void parse(const std::string& key, std::string value);
		void execute() const;
	private:
		//Alert Commands
		AlertReference* alertReference = nullptr;
	};

}


