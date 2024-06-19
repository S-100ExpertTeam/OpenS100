#pragma once
#include <string>
#include <memory>

#include "DrawingCommands.h"
#include "StateCommands.h"


namespace DrawingInstructions
{
	class DrawingInstruction
	{
	public:
		DrawingInstruction() = default;
		DrawingInstruction(const DrawingInstruction&) = delete;
		DrawingInstruction& operator=(const DrawingInstruction&) = delete;
		virtual ~DrawingInstruction();

		void execute();
		void parse(const std::string& key ,std::string value);

	private:
		DrawingInstructions::DrawingCommands* drawingCommand = nullptr;
		DrawingInstructions::StateCommands* stateCommands = nullptr;

	};

}


