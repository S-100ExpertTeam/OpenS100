#pragma once

#include "RecordName.h"

struct THAS
{
public:
	THAS();
	THAS(const THAS& other);
	THAS(RecordName name, int taui);
	virtual ~THAS();

public:
	RecordName m_name;
	int m_taui;

public:
	virtual THAS operator=(const THAS& other);

public:
	static int GetSize();
};