#pragma once

#include <hdf5.h>

class H5_ValuesGroup
{
public:
	H5_ValuesGroup();
	virtual ~H5_ValuesGroup();

public:
	virtual bool Read(hid_t groupID);
};