#pragma once

#include "S100SpatialObject.h"
#include "H5_RootMeta.h"

class S100H5 :
    public S100SpatialObject
{
public:
    S100H5();
    virtual ~S100H5();

public:
    H5_RootMeta* rootMeta = nullptr;

public:
    bool Open(CString _filepath) override;

    void SetMBR();
};