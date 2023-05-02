#pragma once

#include "S100SpatialObject.h"
#include "IF_FeatureType.h"
#include "IF_InformationType.h"
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
    std::vector<S100Interface::FeatureType*> features;
    std::vector<S100Interface::InformationType*> informations;

    //std::vector<
};