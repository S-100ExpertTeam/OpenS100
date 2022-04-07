#include "stdafx.h"
#include "S100_Symbol.h"
#include "S100_Description.h"

S100_Symbol::S100_Symbol()
{
	rotation = NULL;
	description = NULL;
}

S100_Symbol::~S100_Symbol()
{
	if (description) delete description;
	if (areaPlacement) delete areaPlacement;
	if (linePlacement) delete linePlacement;
}

void S100_Symbol::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto idAttri = node.attribute("id");
	if (idAttri)
	{
		id = pugi::as_wide(idAttri.value());
	}


	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (instructionName == nullptr)
		{
			continue;
		}
		if (!strcmp(instructionName, "description"))
		{
			description = new S100_Description();
			description->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "fileName"))
		{
			fileName = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "fileType"))
		{
			fileType = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "fileFormat"))
		{
			fileFormat = pugi::as_wide(instruction.child_value());
		}
		//========================================================
		if (!strcmp(instructionName, "symbolReference"))
		{
			reference = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "rotation"))
		{
			rotation =std::stod(instruction.child_value());
		}
		else if (!strcmp(instructionName, "rotationCRS") == 0)
		{
			rotationCRS = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "scaleFactor") == 0)
		{
			scaleFactor = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "areaPlacement") == 0)
		{
			if (!areaPlacement)
			{
				areaPlacement = new S100_AreaPlacement();
				areaPlacement->GetContents(instruction);
			}
		}
		else if (!strcmp(instructionName, "linePlacement") == 0)
		{
			if (!linePlacement)
			{
				linePlacement = new S100_LinePlacement();
				linePlacement->GetContents(instruction);
			}
		}
	}



}

void S100_Symbol::SetReference(std::wstring& value)
{
	reference = value;
}

void S100_Symbol::SetRotation(double value)
{
	rotation = value;
}

void S100_Symbol::SetRotationCRS(std::wstring& value)
{
	rotationCRS = value;
}

void S100_Symbol::SetScaleFactor(std::wstring& value)
{
	scaleFactor = value;
}

void S100_Symbol::SetAreaPlacement(S100_AreaPlacement* value)
{
	areaPlacement = value;
}

void S100_Symbol::SetLinePlacement(S100_LinePlacement* value)
{
	linePlacement = value;
}

std::wstring S100_Symbol::GetReference()
{
	return reference;
}

double S100_Symbol::GetRotation()
{
	return rotation;
}

std::wstring S100_Symbol::GetRotationCRS()
{
	return rotationCRS;
}

std::wstring S100_Symbol::GetScaleFactor()
{
	return scaleFactor;
}

S100_AreaPlacement* S100_Symbol::GetAreaPlacement()
{
	return areaPlacement;
}

S100_LinePlacement* S100_Symbol::GetLinePlacement()
{
	return linePlacement;
}

std::wstring S100_Symbol::GetId() 
{
	return id;
}

S100_Description* S100_Symbol::GetDescription()
{
	return description;
}

std::wstring S100_Symbol::GetFileName()
{
	return fileName;
}

std::wstring S100_Symbol::GetFileType()
{
	return fileType;
}

std::wstring S100_Symbol::GetFileFormat()
{
	return fileFormat;
}