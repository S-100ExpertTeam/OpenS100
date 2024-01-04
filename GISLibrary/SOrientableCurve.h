#pragma once
#include "SGeometry.h"
class SOrientableCurve :
	public SGeometry
{
public:
	SOrientableCurve();
	SOrientableCurve(const SOrientableCurve& other);
	virtual ~SOrientableCurve();

public:
	// + : true
	// - : false
	bool orientation = true;

public:
	virtual SOrientableCurve operator=(const SOrientableCurve& other);
};

