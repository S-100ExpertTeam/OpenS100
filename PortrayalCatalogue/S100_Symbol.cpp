#include "stdafx.h"
#include "S100_Symbol.h"
#include "S100_Description.h"

#include "..\\S100Engine\\GraphicBasePackage_Enum.h"

S100_Symbol::S100_Symbol()
{
	rotation = NULL;
	//description = NULL;
}

S100_Symbol::~S100_Symbol()
{
	//if (description) delete description;
	//if (areaPlacement) delete areaPlacement;
	//if (linePlacement) delete linePlacement;
}

void S100_Symbol::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	this->reference = pugi::as_wide(node.attribute("reference").value());

	//auto idAttri = node.attribute("id");
	//if (idAttri)
	//{
	//	id = pugi::as_wide(idAttri.value());
	//}

	SetRotation(node.attribute("rotation").as_double());

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (instructionName == nullptr)
		{
			continue;
		}
		//if (!strcmp(instructionName, "description"))
		//{
		//	description = new S100_Description();
		//	description->GetContents(instruction);
		//}
		//else if (!strcmp(instructionName, "fileName"))
		//{
		//	fileName = pugi::as_wide(instruction.child_value());
		//}
		//else if (!strcmp(instructionName, "fileType"))
		//{
		//	fileType = pugi::as_wide(instruction.child_value());
		//}
		//else if (!strcmp(instructionName, "fileFormat"))
		//{
		//	fileFormat = pugi::as_wide(instruction.child_value());
		//}
		
		if (!strcmp(instructionName, "rotation"))
		{
			rotation =std::stod(instruction.child_value());
		}
		else if (!strcmp(instructionName, "rotationCRS"))
		{
			rotationCRS = GraphicBasePackage::GetCRSTypeFromString(instruction.child_value());

			//rotationCRS = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "scaleFactor"))
		{
			//scaleFactor = pugi::as_wide(instruction.child_value());
			scaleFactor = std::stod(instruction.child_value());
		}
		else if (!strcmp(instructionName, "areaPlacement"))
		{
			//if (!areaPlacement)
			//{
			//	areaPlacement = new S100_AreaPlacement();
			//	areaPlacement->GetContents(instruction);
			//}
			areaPlacement.value().GetContents(instruction);
		}
		else if (!strcmp(instructionName, "linePlacement"))
		{
			linePlacement.value().GetContents(instruction);

			//if (!linePlacement)
			//{
			//	linePlacement = new S100_LinePlacement();
			//	linePlacement->GetContents(instruction);
			//}
		}

		auto offsetNode = node.child("offset");
		if (offsetNode)
		{
			double offsetX = offsetNode.child("x").text().as_double();
			double offsetY = offsetNode.child("y").text().as_double();
			offset.Set(offsetX, offsetY);
		}
	}
}

void S100_Symbol::SetReference(std::wstring& value)
{
	reference = value;
}

void S100_Symbol::SetReference(std::string& value)
{
	reference = std::wstring(value.begin(), value.end());
}

void S100_Symbol::SetRotation(double value)
{
	rotation = value;
}

void S100_Symbol::SetRotationCRS(std::string& value)
{
	rotationCRS = GraphicBasePackage::GetCRSTypeFromString(value);
}

void S100_Symbol::SetRotationCRS(std::wstring& value)
{
	//rotationCRS = value;
	rotationCRS = GraphicBasePackage::GetCRSTypeFromString(value);
}

void S100_Symbol::SetScaleFactor(double value)
{
	scaleFactor = value;
}

std::wstring S100_Symbol::GetReference()
{
	return reference;
}

double S100_Symbol::GetRotation()
{
	return rotation;
}

//std::wstring S100_Symbol::GetRotationCRS()
//{
//	return rotationCRS;
//}

GraphicBasePackage::CRSType S100_Symbol::GetRotationCRS()
{
	return rotationCRS;
}

double S100_Symbol::GetScaleFactor()
{
	return scaleFactor;
}

//S100_AreaPlacement* S100_Symbol::GetAreaPlacement()
//{
//	return areaPlacement;
//}

std::optional<S100_LineSymbolPlacement> S100_Symbol::GetLinePlacement()
{
	return linePlacement;
}

std::optional<S100_AreaSymbolPlacement> S100_Symbol::GetAreaPlacement()
{
	return areaPlacement;
}

//S100_LinePlacement* S100_Symbol::GetLinePlacement()
//{
//	return linePlacement;
//}

//std::wstring S100_Symbol::GetId() 
//{
//	return id;
//}
//
//S100_Description* S100_Symbol::GetDescription()
//{
//	return description;
//}
//
//std::wstring S100_Symbol::GetFileName()
//{
//	return fileName;
//}
//
//std::wstring S100_Symbol::GetFileType()
//{
//	return fileType;
//}
//
//std::wstring S100_Symbol::GetFileFormat()
//{
//	return fileFormat;
//}

double S100_Symbol::GetOffsetX()
{
	//return offsetX;
	return offset.GetX();
}

double S100_Symbol::GetOffsetY()
{
	//return offsetY;
	return offset.GetY();
}

void S100_Symbol::SetOffsetX(double value)
{
	//offsetX = value;
	offset.SetX(value);
}

void S100_Symbol::SetOffsetY(double value)
{
	//offsetY = value;
	offset.SetY(value);
}

void S100_Symbol::SetLinePlacement(double offset, S100_LinePlacementMode placementMode, std::optional<bool> visibleParts)
{
	linePlacement = S100_LineSymbolPlacement();
	linePlacement.value().SetOffset(offset);
	linePlacement.value().SetPlacementMode(placementMode);
	if (visibleParts.has_value())
	{
		linePlacement.value().SetVisibleParts(visibleParts.value());
	}
	else
	{
		linePlacement.value().SetVisibleParts(false);
	}
}

void S100_Symbol::SetAreaPlacement(S100_AreaPlacementMode mode)
{
	areaPlacement = S100_AreaSymbolPlacement();
	areaPlacement.value().SetPlacementMode(mode);
}
