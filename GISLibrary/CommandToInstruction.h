#pragma once

#include "..\\PortrayalCatalogue\\S100_PointInstruction.h"
#include "..\\PortrayalCatalogue\\S100_LineInstruction.h"

#include "ProcessS101.h"

class CommandToInstruction
{
public:
	CommandToInstruction() = default;
	virtual ~CommandToInstruction() = default;

public:
	static S100_PointInstruction* ToS100PointInstruction(Local_StateCommands& sc, DrawingCommand::PointInstruction& pi);
	static S100_LineInstruction* ToS100LineInstruction(Local_DrawingCommands& dc, Local_StateCommands& sc);
	static bool SetDrawingInstruction(Local_StateCommands& sc, S100_Instruction* out);
};

