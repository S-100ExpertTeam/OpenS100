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
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:voice"))
		{
			voice.push_back(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100CI:facsimile"))
		{
			facsimile = pugi::as_wide(instruction.child_value());
		}
		else 
		{
			std::wstring content = pugi::as_wide(instructionName);
			content.append(L"is another data");
		}
	}
}

void Telephone::SetVoice(std::wstring value) 
{
	voice.push_back(value);
}

void Telephone::SetVoice(std::list<std::wstring> value)
{
	voice = value;
}

std::list<std::wstring>& Telephone::GetVoicePointer()
{
	return voice;
}

const std::wstring& Telephone::GetVoice(int index)
{
	auto it = voice.begin();
	std::advance(it,index);
	return *it;
}

void Telephone::SetFacsimile(std::wstring value) 
{
	facsimile = value;
}

const std::wstring& Telephone::GetFacsimile()
{
	return facsimile;
}