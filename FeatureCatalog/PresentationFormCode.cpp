#include "stdafx.h"
#include "PresentationFormCode.h"

PresentationFormCode::PresentationFormCode()
{
	InsertValue("documentDigital");
	InsertValue("documentHardcopy");
	InsertValue("imageDigital");
	InsertValue("mapDigital");
	InsertValue("mapHardcopy");
	InsertValue("modelDigital");
	InsertValue("modelHardcopy");
	InsertValue("profileDigital");
	InsertValue("profileHardcopy");
	InsertValue("tableDigital");
	InsertValue("tableHardcopy");
	InsertValue("videoDigital");
	InsertValue("videoHardcopy");
}

PresentationFormCode::~PresentationFormCode()
{

}

void PresentationFormCode::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}