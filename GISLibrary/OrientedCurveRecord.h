#pragma once

class R_CurveRecord;
class OrientedCurveRecord
{
public:
	OrientedCurveRecord();
	virtual ~OrientedCurveRecord();

public:
	R_CurveRecord *m_pCurveRecord = nullptr;

	// 1 : Forward
	// 2 : Reverse
	int m_orient = 1;
};
