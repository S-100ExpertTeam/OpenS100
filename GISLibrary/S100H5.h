#pragma once

#include "S100SpatialObject.h"
#include "H5_RootGroup.h"
#include "H5_FeatureContainer.h"

#include <vector>

class S100H5 :
    public S100SpatialObject
{
public:
    S100H5();
    virtual ~S100H5();

public:
    // Level 1
    H5_RootGroup* rootGroup = nullptr;

    // Level 2
    std::vector<H5_FeatureContainer*> featureContainer;

public:
    bool Open(CString _filepath, GISLibrary::D2D1Resources* d2d1) override;

    void SetMBR();
};