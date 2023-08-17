#include "stdafx.h"
#include "IntervalType.h"

IntervalType stringToIntervalType(std::string value)
{
	if (!value.compare("openInterval")) {
		return IntervalType::openInterval;
	}
	else if (!value.compare("geLtInterval")) {
		return IntervalType::geLtInterval;
	}
	else if (!value.compare("gtLeInterval")) {
		return IntervalType::gtLeInterval;
	}
	else if (!value.compare("closedInterval")) {
		return IntervalType::closedInterval;
	}
	else if (!value.compare("gtSemiInterval")) {
		return IntervalType::gtSemiInterval;
	}
	else if (!value.compare("geSemiInterval")) {
		return IntervalType::geSemiInterval;
	}
	else if (!value.compare("ltSemiInterval")) {
		return IntervalType::ltSemiInterval;
	}
	else if (!value.compare("leSemiInterval")) {
		return IntervalType::leSemiInterval;
	}

	return IntervalType::none;
}

std::string IntervalTypeToString(IntervalType value) {
	if (IntervalType::openInterval == value) {
		return "openInterval";
	}
	else if (IntervalType::geLtInterval == value) {
		return "geLtInterval";
	}
	else if (IntervalType::gtLeInterval == value) {
		return "gtLeInterval";
	}
	else if (IntervalType::closedInterval == value) {
		return "closedInterval";
	}
	else if (IntervalType::gtSemiInterval == value) {
		return "gtSemiInterval";
	}
	else if (IntervalType::geSemiInterval == value) {
		return "geSemiInterval";
	}
	else if (IntervalType::ltSemiInterval == value) {
		return "ltSemiInterval";
	}
	else if (IntervalType::leSemiInterval == value) {
		return "leSemiInterval";
	}

	return "none";
}


//IntervalType::IntervalType()
//{
//	InsertValue(L"openInterval");
//	InsertValue(L"geLtInterval");
//	InsertValue(L"gtLeInterval");
//	InsertValue(L"closedInterval");
//	InsertValue(L"gtSemiInterval"); 
//	InsertValue(L"geSemiInterval");
//	InsertValue(L"ltSemiInterval");
//	InsertValue(L"leSemiInterval");
//}
//
//IntervalType::~IntervalType()
//{
//
//}
//
//void IntervalType::GetContents(pugi::xml_node& node)
//{
//	SetValueString(pugi::as_wide(node.child_value()));
//}