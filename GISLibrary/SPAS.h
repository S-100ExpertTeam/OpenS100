#pragma once
#include "RecordName.h"

struct SPAS {
public:
	RecordName m_name;
	int m_ornt;
	unsigned int m_smin;
	unsigned int m_smax;
	int m_saui;

public:
	static int GetSize();

	void Set(RecordName recordName, int ORNT = 1, unsigned int SMIN = 0, unsigned int SMAX = pow(2, 32) - 1, int SAUI = 1);
};