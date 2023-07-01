#pragma once

#include "H5_ValuesGroup.h"

class S102_VG_BathymetryCoverage : 
	public H5_ValuesGroup
{
public:
	S102_VG_BathymetryCoverage();
	virtual ~S102_VG_BathymetryCoverage();

public:
	float* depth = nullptr;
	float* uncertainty = nullptr;

public:
	bool Read(hid_t groupID) override;
	bool Read(hid_t groupID, int numLat, int numLon);
};

