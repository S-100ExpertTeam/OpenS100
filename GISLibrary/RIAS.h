#pragma once
#include "RecordName.h"

struct RIAS
{
public:
	RecordName m_name;

	// 1 : Forward
	// 2 : Reverse
	int m_ornt;

	// 1 : Exterior
	// 2 : Interior
	int m_usag;

	// 1 : Insert
	int m_raui;

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