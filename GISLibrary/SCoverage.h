#pragma once
#include "SGeometry.h"

class MBR;
class SCoverage :
	public SGeometry
{
public:
	SCoverage();
	SCoverage(const SCoverage& other);
	virtual ~SCoverage();

public:
	virtual SCoverage operator=(const SCoverage& other);

public:
	SGeometryType GetType() override;

	MBR* m_pBoundary = nullptr;
	int m_xIndex = 0;
	int m_yIndex = 0;
};