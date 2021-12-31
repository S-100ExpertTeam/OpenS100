#pragma once
#include "RecordName.h"

struct SPAS {
public:
	RecordName m_name;
	int m_ornt;
	int m_smin = 1;
	int m_smax = 15;
	int m_saui;

public:
	static int GetSize();
};