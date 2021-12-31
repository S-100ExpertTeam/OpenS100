#include "stdafx.h"
#include "PresentationFormCode.h"

PresentationFormCode::PresentationFormCode()
{
	InsertValue(L"documentDigital");
	InsertValue(L"documentHardcopy");
	InsertValue(L"imageDigital");
	InsertValue(L"mapDigital");
	InsertValue(L"mapHardcopy");
	InsertValue(L"modelDigital");
	InsertValue(L"modelHardcopy");
	InsertValue(L"profileDigital");
	InsertValue(L"profileHardcopy");
	InsertValue(L"tableDigital");
	InsertValue(L"tableHardcopy");
	InsertValue(L"videoDigital");
	InsertValue(L"videoHardcopy");
}

PresentationFormCode::~PresentationFormCode()
{

}

void PresentationFormCode::GetContents(pugi::xml_node& node)
{
	SetValueString(pugi::as_wide(node.child_value()));
}