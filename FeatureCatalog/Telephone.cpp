#include "stdafx.h"
#include "Telephone.h"

Telephone::Telephone()
{

}

Telephone::~Telephone()
{

}

void Telephone::GetContents(pugi::xml_node& node)
{
	//S100CI
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:voice"))
		{
			voice.push_back(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:facsimile"))
		{
			facsimile = instruction.child_value();
		}
		else 
		{
			std::string content = instructionName;
			content.append("is another data");
		}
	}
}

void Telephone::SetVoice(std::string value) 
{
	voice.push_back(value);
}

void Telephone::SetVoice(std::list<std::string> value)
{
	voice = value;
}

std::list<std::string>& Telephone::GetVoicePointer()
{
	return voice;
}

const std::string& Telephone::GetVoice(int index)
{
	auto it = voice.begin();
	std::advance(it,index);
	return *it;
}

void Telephone::SetFacsimile(std::string value) 
{
	facsimile = value;
}

const std::string& Telephone::GetFacsimile()
{
	return facsimile;
}