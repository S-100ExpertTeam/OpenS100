#pragma once
#include "SGeometry.h"
class SOrientableCurve :
    public SGeometry
{
public:
    SOrientableCurve();
    virtual ~SOrientableCurve();

public:
    // + : true
    // - : false
    bool orientation = true; 
};

