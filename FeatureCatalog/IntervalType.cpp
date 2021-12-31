#include "stdafx.h"
#include "IntervalType.h"

IntervalType::IntervalType()
{
	InsertValue(L"openInterval");
	InsertValue(L"geLtInterval");
	InsertValue(L"gtLeInterval");
	InsertValue(L"closedInterval");
	InsertValue(L"gtSemiInterval"); 
	InsertValue(L"geSemiInterval");
	InsertValue(L"ltSemiInterval");
	InsertValue(L"leSemiInterval");
}

IntervalType::~IntervalType()
{

}

void IntervalType::GetContents(pugi::xml_node& node)
{
	SetValueString(pugi::as_wide(node.child_value()));
}