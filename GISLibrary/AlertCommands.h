#pragma once
#include "StateCommand.h"

namespace Part9a
{
	class AlertReference : public StateCommand {
	public:
		AlertReference() = default;
		AlertReference(std::string alertReference, std::string plan, std::string monitor);

	public:
		virtual void init() override;
		virtual void execute() override;
		virtual void parse(const std::string& input) override;
	private:
		std::string alertReference;
		std::string plan;
		std::string monitor;
	};
}


