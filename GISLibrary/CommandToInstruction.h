#pragma once

#include "..\\PortrayalCatalogue\\S100_PointInstruction.h"

#include "ProcessS101.h"

class CommandToInstruction
{
public:
	CommandToInstruction() = default;
	virtual ~CommandToInstruction() = default;

public:
	static S100_PointInstruction* ToS100PointInstruction(Local_DrawingCommands& dc, Local_StateCommands& sc);
};

