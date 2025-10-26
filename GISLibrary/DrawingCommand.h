#pragma once
#include "Command.h"

namespace Part9a
{
	class DrawingCommand : public Command
	{
	public:
		DrawingCommand() = default;
		virtual ~DrawingCommand() = default;

	public:
		virtual void init() override;
		virtual void execute() override;
		virtual void parse(const std::string& input) override;
	};
}
