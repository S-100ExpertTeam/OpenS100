#pragma once
#include "RecordName.h"

struct SPAS {
public:
	RecordName m_name;
	int m_ornt;
	int m_smin;
	int m_smax;
	int m_saui;

public:
	static int GetSize();

	void Set(RecordName recordName, int ORNT = 1, int SMIN = 0, int SMAX = (2 ^ 32) - 1, int SAUI = 1);
};