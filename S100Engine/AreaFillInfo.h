#pragma once
#include "AreaPatternBitmap.h"
#include "CaseInsensitiveLess.h"
#include <string>

#include <map>

class AreaFillInfo
{
public:
	AreaFillInfo();
	virtual ~AreaFillInfo();

public:
	std::map<std::wstring, AreaPatternBitmap*, s100::CaseInsensitiveLessW> patternMap;

public:
	void PatternMapDelete();
};