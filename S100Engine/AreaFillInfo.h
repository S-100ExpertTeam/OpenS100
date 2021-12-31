#pragma once
#include "AreaPatternBitmap.h"
#include "CStringCompareNoCaseLessThan.h"

#include <map>

class AreaFillInfo
{
public:
	AreaFillInfo();
	virtual ~AreaFillInfo();

public:
	std::map<CString, AreaPatternBitmap*, CStringCompareNoCaseLessThan> patternMap;

public:
	void PatternMapDelete();
};