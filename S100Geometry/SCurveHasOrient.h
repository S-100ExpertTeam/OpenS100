#pragma once

class SCurve;
class SCurveHasOrient
{
public:
	SCurveHasOrient(bool _parentOrient, SCurve* _curve);
	SCurveHasOrient(int _parentOrient, SCurve* _curve);
	~SCurveHasOrient();

private:
	bool parentOrient = true;
	bool m_masking = false;
	bool m_isDuplicated = false;
	SCurve* curve = nullptr;

public:
	bool GetParentOrient();
	SCurve* GetCurve();
	void SetMasking(bool value);
	bool GetMasking();
	void SetIsDuplicated(bool value);
	bool GetIsDuplicated();
};