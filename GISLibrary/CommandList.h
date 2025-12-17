#pragma once

#include "Command.h"

#include <list>
#include <memory>

class S100_Instruction;

class CommandList
{
public:
	CommandList() = default;
	virtual ~CommandList() = default;

protected:
	std::list<std::unique_ptr<Part9a::Command>> commands;

public:
	void Insert(std::string id, std::string& command, std::string& params);
	void Insert(std::unique_ptr<Part9a::Command> command);
	std::list<S100_Instruction*> Parse();
};
