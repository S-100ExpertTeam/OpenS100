#pragma once
#include "RecordName.h"

struct CUCO
{
public:
	CUCO();
	CUCO(int rcnm, int rcid, int ornt);
	virtual ~CUCO();

public:
	RecordName m_name;

	// 1 : Forward
	// 2 : Reverse
	int m_ornt;

public:
	static int GetSize();

public:
	bool IsCurve();
	bool IsCompositeCurve();
};