#pragma once

#include <hdf5.h>

class H5_ValuesGroup
{
public:
	H5_ValuesGroup();
	H5_ValuesGroup(const H5_ValuesGroup& other);
	virtual ~H5_ValuesGroup();

public:
	virtual H5_ValuesGroup operator=(const H5_ValuesGroup& other);

public:
	virtual bool Read(hid_t groupID);
};