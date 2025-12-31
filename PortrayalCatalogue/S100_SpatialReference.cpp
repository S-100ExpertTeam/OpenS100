#include "stdafx.h"
#include "S100_SpatialReference.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

#include <pugixml.hpp>

S100_SpatialReference::S100_SpatialReference()
{

}

S100_SpatialReference::~S100_SpatialReference()
{

}

//void S100_SpatialReference::SetType(std::string &value)
//{
//	type = std::wstring(value.begin(), value.end());
//}

//void S100_SpatialReference::SetType(std::string_view value)
//{
//	type = std::wstring(value.begin(), value.end());
//}

void S100_SpatialReference::SetReference(std::string& value)
{
	reference = value;
}

void S100_SpatialReference::SetReference(std::string_view value)
{
	reference = std::string(value);
}

void S100_SpatialReference::SetForward(bool value)
{
	forward = true;
}

void S100_SpatialReference::SetBackward()
{
	forward = false;
}

//std::wstring S100_SpatialReference::GetType() 
//{
//	return type;
//}


std::string S100_SpatialReference::GetReference()
{
	return reference;
}

std::wstring S100_SpatialReference::GetReferenceAsWString() 
{
	return pugi::as_wide(reference);
}

bool S100_SpatialReference::isForward()
{
	return forward;
}

bool S100_SpatialReference::isBackward()
{
	return !forward;
}

void S100_SpatialReference::Set(std::string& value)
{
	// value : reference[,forward]
	std::vector<std::string_view> element;
	LatLonUtility::Split(value, ",", element);

	// Set reference
	if (element.size() >= 1)
	{
		SetReference(std::string(element[0]));
	}

	// Set forward/backward
	if (element.size() >= 2)
	{
		std::string_view forwardStr = element[1];
		if (forwardStr.compare("forward") == 0)
		{
			SetForward();
		}
		else if (forwardStr.compare("backward") == 0)
		{
			SetBackward();
		}
	}
}