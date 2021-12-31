#pragma once

class ObjectName
{
public:
	ObjectName();
	virtual ~ObjectName();

public:
	__int64 m_objName;
	int m_agen;
	int m_fidn;
	int m_fids;

public:
	__int64 GetName();
	int GetLength();
};