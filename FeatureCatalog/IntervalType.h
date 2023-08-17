#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

enum class IntervalType {
	none = 0,
	openInterval,
	geLtInterval,
	gtLeInterval,
	closedInterval,
	gtSemiInterval,
	geSemiInterval,
	ltSemiInterval,
	leSemiInterval
};

IntervalType stringToIntervalType(std::string value);
std::string IntervalTypeToString(IntervalType value);


//class IntervalType :
//	public Restriction_Item
//{
//public:
//	IntervalType();
//	virtual ~IntervalType();
//
//public:
//	void GetContents(pugi::xml_node& node);
//};