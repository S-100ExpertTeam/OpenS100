#pragma once

#include "Command.h"

namespace DrawingCommand
{
	class StateCommand : public Command
	{
	public:
		StateCommand() = default;
		virtual ~StateCommand() = default;

	public:
		bool IsStateCommand() const;
	};
}