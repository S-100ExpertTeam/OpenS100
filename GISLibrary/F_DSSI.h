#pragma once
#include "Field.h"

class F_DSSI : Field
{
public:
	F_DSSI();
	virtual ~F_DSSI();

public:
	double m_dcox;
	double m_dcoy;
	double m_dcoz;
	int m_cmfx;
	int m_cmfy;
	int m_cmfz;
	int m_noir;
	int m_nopn;
	int m_nomn;
	int m_nocn;
	int m_noxn;
	int m_nosn;
	int m_nofr;

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};