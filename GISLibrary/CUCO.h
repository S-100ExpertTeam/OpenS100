#pragma once
#include "RecordName.h"

struct CUCO
{
public:
	RecordName m_name;
	int m_ornt;

public:
	static int GetSize();
};