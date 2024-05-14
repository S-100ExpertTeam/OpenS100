#pragma once
#include "RecordName.h"

struct CUCO
{
public:
	CUCO();
	CUCO(const CUCO& other);
	CUCO(int rcnm, int rcid, int ornt);
	virtual ~CUCO();

public:
	RecordName m_name;

	// 1 : Forward
	// 2 : Reverse
	int m_ornt;

public:
	virtual CUCO operator=(const CUCO& other);

public:
	static int GetSize();

public:
	bool IsCurve();
	bool IsCompositeCurve();
};