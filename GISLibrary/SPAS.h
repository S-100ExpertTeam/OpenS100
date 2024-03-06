#pragma once
#include "RecordName.h"

struct SPAS {
public:
	SPAS();
	SPAS(const SPAS& other);
	SPAS(RecordName name, int ornt, UINT smin, UINT smax, int saui);
	virtual ~SPAS();

public:
	RecordName m_name;
	int m_ornt = 0;
	unsigned int m_smin = 0;
	unsigned int m_smax = UINT32_MAX;
	int m_saui = 0;

public:
	virtual SPAS operator=(const SPAS& other);

public:
	static int GetSize();

	void Set(RecordName recordName, int ORNT = 1, unsigned int SMIN = 0, unsigned int SMAX = UINT32_MAX, int SAUI = 1);
};