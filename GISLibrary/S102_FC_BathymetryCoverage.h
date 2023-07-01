#pragma once

#include "H5_FeatureContainer.h"
#include "S102_FI_BathymetryCoverage.h"

class S102_FC_BathymetryCoverage :
	public H5_FeatureContainer
{
public:
	S102_FC_BathymetryCoverage();
	virtual ~S102_FC_BathymetryCoverage();

public:
	bool Read(hid_t groupID) override;

	S102_FI_BathymetryCoverage* GetBathymetryCoverage();
};

