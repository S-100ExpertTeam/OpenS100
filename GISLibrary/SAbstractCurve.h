#pragma once
#include "SGeometry.h"
class SAbstractCurve :
    public SGeometry
{
public:
    SAbstractCurve();
    SAbstractCurve(const SAbstractCurve& other);
    virtual ~SAbstractCurve();

public:
    virtual SAbstractCurve operator=(const SAbstractCurve& other);
    virtual void Set(int index, double x, double y);
};