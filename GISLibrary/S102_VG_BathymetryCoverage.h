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

	// Number of valid entries in depth[] and uncertainty[]. Zero until Read()
	// has succeeded. Consumers must bound their iteration by this value rather
	// than by recomputing it from the file's dimension attributes.
	size_t pointCountRead = 0;

public:
	bool Read(hid_t groupID) override;
	bool Read(hid_t groupID, int numLat, int numLon);
};

