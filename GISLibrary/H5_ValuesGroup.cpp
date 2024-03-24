#include "stdafx.h"
#include "H5_ValuesGroup.h"

H5_ValuesGroup::H5_ValuesGroup()
{

}

H5_ValuesGroup::H5_ValuesGroup(const H5_ValuesGroup& other)
{

}

H5_ValuesGroup::~H5_ValuesGroup()
{

}

H5_ValuesGroup H5_ValuesGroup::operator=(const H5_ValuesGroup& other)
{
	return *this;
}

bool H5_ValuesGroup::Read(hid_t groupID)
{
	return true;
}