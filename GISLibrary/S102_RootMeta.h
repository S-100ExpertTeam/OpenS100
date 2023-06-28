#pragma once

#include "H5_RootMeta.h"
#include "S102_GriddingMethod.h"

class S102_RootMeta :
    public H5_RootMeta
{
public:
    S102_RootMeta();
    virtual ~S102_RootMeta();

public:
    S102_GriddingMethod* griddingMethod = nullptr;

public:
    bool hasGriddingMethod() const;
    S102_GriddingMethod getGriddingMethod() const;
    void setGriddingMethod(const S102_GriddingMethod value);

public:
    bool Read(hid_t rootID) override;
};

