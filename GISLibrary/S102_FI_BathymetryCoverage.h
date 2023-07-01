#pragma once

#include "H5_FeatureInstanceGroup.h"
#include "S102_VG_BathymetryCoverage.h"

class S102_FI_BathymetryCoverage :
    public H5_FeatureInstanceGroup
{
public:
    S102_FI_BathymetryCoverage();
    virtual ~S102_FI_BathymetryCoverage();

public:
    virtual bool Read(hid_t groupID, DataOrganizationIndex dataCodingFormat) override;

    S102_VG_BathymetryCoverage* GetBathymetryCoverage();
};

