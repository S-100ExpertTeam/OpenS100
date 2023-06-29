#pragma once

#include "H5_RootGroup.h"
#include "S102_GriddingMethod.h"

class S102_RootGroup :
    public H5_RootGroup
{
public:
    S102_RootGroup();
    virtual ~S102_RootGroup();

public:
    S102_GriddingMethod* griddingMethod = nullptr;

public:
    bool hasGriddingMethod() const;
    S102_GriddingMethod getGriddingMethod() const;
    void setGriddingMethod(const S102_GriddingMethod value);

public:
    bool Read(hid_t rootID) override;
};

