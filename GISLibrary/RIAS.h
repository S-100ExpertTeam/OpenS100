#pragma once
#include "RecordName.h"

struct RIAS
{
public:
	RIAS();
	RIAS(const RIAS& other);
	RIAS(int rcnm, int rcid, int usag, int ornt);
	virtual ~RIAS();

public:
	RecordName m_name;

	// 1 : Forward
	// 2 : Reverse
	int m_ornt;

	// 1 : Exterior
	// 2 : Interior
	int m_usag;

	// 1 : Insert
	int m_raui = 1;

public:
	virtual RIAS operator=(const RIAS& other);

public:
	static int GetSize();

public:
	bool IsForward();
	bool IsReverse();

	bool IsExterior();
	bool IsInterior();

	bool IsCurve();
	bool IsCompositeCurve();
};