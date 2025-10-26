#pragma once

#include "Command.h"

namespace Part9a
{
	class StateCommand : public Command
	{
	public:
		StateCommand() = default;
		virtual ~StateCommand() = default;
	};
}