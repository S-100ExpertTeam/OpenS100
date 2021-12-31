#pragma once
#include "SGeometry.h"

class MBR;
class SCoverage :
	public SGeometry
{
public:
	SCoverage();
	virtual ~SCoverage();

public:
	MBR* m_pBoundary = nullptr;
	int m_xIndex = 0;
	int m_yIndex = 0;
};