#pragma once
#include"RecordName.h"

struct PTAS
{
public:
	PTAS();
	PTAS(int rcid, int topi);
	virtual ~PTAS();

public:
	RecordName m_name;
	/*
	{1} - Beginning point
	{2} - End point
	{3} - Beginning & End point
	*/
	int m_topi;

public:
	static int GetSize();
};