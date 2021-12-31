#include "stdafx.h"
#include "AreaFillInfo.h"

AreaFillInfo::AreaFillInfo()
{

}

AreaFillInfo::~AreaFillInfo()
{
	PatternMapDelete();
}

void AreaFillInfo::PatternMapDelete()
{
	for (auto i = patternMap.begin(); i != patternMap.end(); i++)
	{
		delete i->second;
	}
	patternMap.clear();
	return;
}