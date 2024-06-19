#include "stdafx.h"
#include "DrawingInstruction.h"
#include "DrawingCommands.h"
#include "StateCommands.h"



namespace DrawingInstructions
{
	DrawingInstruction::~DrawingInstruction()
	{
		delete drawingCommand;
		drawingCommand = nullptr;

		delete stateCommands;
		stateCommands = nullptr;
	}

	void DrawingInstruction::execute()
	{
		if(drawingCommand)
			drawingCommand->execute();
		if(stateCommands)
			stateCommands->execute();

	}

	void DrawingInstruction::parse(const std::string& key, std::string value)
	{
		drawingCommand->parse(key,value);
		stateCommands->parse(key,value);
	}

}

