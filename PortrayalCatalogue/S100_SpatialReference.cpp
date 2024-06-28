#include "stdafx.h"
#include "S100_SpatialReference.h"

S100_SpatialReference::S100_SpatialReference()
{

}

S100_SpatialReference::~S100_SpatialReference()
{

}

void S100_SpatialReference::SetType(std::string &value)
{
	type = std::wstring(value.begin(), value.end());
}

void S100_SpatialReference::SetReference(std::string &value)
{
	reference = std::wstring(value.begin(), value.end());
}

void S100_SpatialReference::SetForward()
{
	forward = true;
}

void S100_SpatialReference::SetBackward()
{
	forward = false;
}

std::wstring S100_SpatialReference::GetType() 
{
	return type;
}

std::wstring S100_SpatialReference::GetReference() 
{
	return reference;
}

bool S100_SpatialReference::isForward()
{
	return forward;
}

bool S100_SpatialReference::isBackward()
{
	return !forward;
}