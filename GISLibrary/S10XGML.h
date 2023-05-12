#pragma once

#include "S100SpatialObject.h"
#include "GF_FeatureType.h"
#include "GF_InformationType.h"
#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCurve.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include <vector>

class S10XGML :
    public S100SpatialObject
{
public:
    S10XGML();
    virtual ~S10XGML();

public:
    bool Open(CString _filepath) override;

public:
    std::vector<GF::FeatureType*> features;
    std::vector<GF::InformationType*> informations;

    //std::vector<
};