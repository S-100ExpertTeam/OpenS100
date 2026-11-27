#include "stdafx.h"
#include "S100_Symbol.h"
#include "S100_Description.h"

#include "GraphicBasePackage_Enum.h"

#include "../LatLonUtility/cpp_util.h"

S100_Symbol::S100_Symbol()
{
	rotation = NULL;
	//description = NULL;
}

S100_Symbol::~S100_Symbol()
{
}

void S100_Symbol::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	this->reference = node.attribute("reference").value();

	SetRotation(node.attribute("rotation").as_double());

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (instructionName == nullptr)
		{
			continue;
		}
		
		if (!strcmp(instructionName, "rotation"))
		{
			rotation =cpp_util::stod(instruction.child_value());
		}
		else if (!strcmp(instructionName, "rotationCRS"))
		{
			rotationCRS = GraphicBasePackage::GetCRSTypeFromString(instruction.child_value());

			//rotationCRS = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "scaleFactor"))
		{
			//scaleFactor = pugi::as_wide(instruction.child_value());
			scaleFactor = cpp_util::stod(instruction.child_value());
		}
		else if (!strcmp(instructionName, "areaPlacement"))
		{
			areaPlacement.value().GetContents(instruction);
		}
		else if (!strcmp(instructionName, "linePlacement"))
		{
			linePlacement.value().GetContents(instruction);

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

void S100_Symbol::SetReference(const std::string& value)  { reference = value; }
void S100_Symbol::SetReference(const std::wstring& value) { reference = toUtf8(value); }

void S100_Symbol::SetRotation(double value)
{
	rotation = value;
}

void S100_Symbol::SetRotationCRS(const std::string& value)  { rotationCRS = GraphicBasePackage::GetCRSTypeFromString(value); }
void S100_Symbol::SetRotationCRS(const std::wstring& value) { rotationCRS = GraphicBasePackage::GetCRSTypeFromString(value); }

void S100_Symbol::SetScaleFactor(double value)
{
	scaleFactor = value;
}

std::string  S100_Symbol::GetReference()  { return reference; }
std::wstring S100_Symbol::GetReferenceW() { return toWide(reference); }

double S100_Symbol::GetRotation()
{
	return rotation;
}

GraphicBasePackage::CRSType S100_Symbol::GetRotationCRS()
{
	return rotationCRS;
}

double S100_Symbol::GetScaleFactor()
{
	return scaleFactor;
}

std::optional<S100_LineSymbolPlacement> S100_Symbol::GetLinePlacement()
{
	return linePlacement;
}

std::optional<S100_AreaSymbolPlacement> S100_Symbol::GetAreaPlacement()
{
	return areaPlacement;
}

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

void S100_Symbol::SetLinePlacement(double _offset, S100_LinePlacementMode placementMode, std::optional<bool> visibleParts)
{
	linePlacement = S100_LineSymbolPlacement();
	linePlacement.value().SetOffset(_offset);
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
