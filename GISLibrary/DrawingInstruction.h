#pragma once
#include <string>
#include <memory>


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


