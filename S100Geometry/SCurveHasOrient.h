#pragma once

#include "SCurve.h"

class SCurveHasOrient
	: public SCurve
{
public:
	//SCurveHasOrient(bool _parentOrient, SCurve* _curve);
	//SCurveHasOrient(int _parentOrient, SCurve* _curve);
	//SCurveHasOrient(SCurve* curve, bool parentOrient, bool masking, bool isDuplicated);
	//SCurveHasOrient(SCurve* curve, bool masking);
	SCurveHasOrient();
	SCurveHasOrient(bool masking);
	virtual ~SCurveHasOrient();

private:
	//bool parentOrient = true;
	bool m_masking = false;
	bool suppress = false;

	// If true, curve is suppressed
	//bool m_isDuplicated = false;
	//bool isSuppressed = 
	//SCurve* curve = nullptr;

public:
	//bool GetParentOrient();
	//SCurve* GetCurve();
	void SetMasking(bool value);
	bool GetMasking();

	void SetSuppress(bool value);
	bool GetSuppress();
	//void SetIsDuplicated(bool value);
	//bool GetIsDuplicated();
};