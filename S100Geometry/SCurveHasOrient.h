#pragma once

class SCurve;
class SCurveHasOrient
{
public:
	//SCurveHasOrient(bool _parentOrient, SCurve* _curve);
	//SCurveHasOrient(int _parentOrient, SCurve* _curve);
	//SCurveHasOrient(SCurve* curve, bool parentOrient, bool masking, bool isDuplicated);
	SCurveHasOrient(SCurve* curve, bool masking);
	~SCurveHasOrient();

private:
	//bool parentOrient = true;
	bool m_masking = false;

	// If true, curve is suppressed
	//bool m_isDuplicated = false;
	//bool isSuppressed = 
	SCurve* curve = nullptr;

public:
	//bool GetParentOrient();
	SCurve* GetCurve();
	void SetMasking(bool value);
	bool GetMasking();
	//void SetIsDuplicated(bool value);
	//bool GetIsDuplicated();
};