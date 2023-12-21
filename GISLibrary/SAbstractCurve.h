#pragma once
#include "SGeometry.h"
class SAbstractCurve :
    public SGeometry
{
public:
    SAbstractCurve();
    virtual ~SAbstractCurve();

public:
    virtual void Set(int index, double x, double y);
    virtual SAbstractCurve* Clone() const;
};