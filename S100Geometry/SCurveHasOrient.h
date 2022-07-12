#pragma once

#include "SCurve.h"

class SCurveHasOrient : public SCurve
{
public:
	SCurveHasOrient();
	SCurveHasOrient(bool masking);
	virtual ~SCurveHasOrient();

private:
	bool m_masking = false;
	bool suppress = false;

public:
	void SetMasking(bool value);
	bool GetMasking();

	void SetSuppress(bool value);
	bool GetSuppress();
};