#pragma once
#include "RecordName.h"

struct RIAS
{
public:
	RecordName m_name;
	int m_ornt;
	int m_usag;
	int m_raui;

public:
	static int GetSize();
};