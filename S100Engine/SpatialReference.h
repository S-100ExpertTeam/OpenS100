#pragma once

class SpatialReference
{
public:
	SpatialReference();
	virtual ~SpatialReference();

public:
	CString m_strReference;
	bool    m_bForword;
};