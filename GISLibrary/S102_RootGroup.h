#pragma once

#include "H5_RootGroup.h"
#include "S102_GriddingMethod.h"

class S102_RootGroup :
    public H5_RootGroup
{
public:
    S102_RootGroup();
    S102_RootGroup(const S102_RootGroup& other);
    virtual ~S102_RootGroup();

public:
    S102_GriddingMethod* griddingMethod = nullptr;

public:
    virtual S102_RootGroup operator=(const S102_RootGroup& other);

public:
    bool hasGriddingMethod() const;
    S102_GriddingMethod getGriddingMethod() const;
    void setGriddingMethod(const S102_GriddingMethod value);

public:
    bool Read(hid_t rootID) override;
};

