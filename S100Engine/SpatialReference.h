#pragma once

#include <string>

class SpatialReference
{
public:
	SpatialReference();
	virtual ~SpatialReference();

public:
	std::string m_strReference;
	bool        m_bForword;
};