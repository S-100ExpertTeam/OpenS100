#pragma once
#include <string>

class S100Utilities
{
public:
	// Determining whether an XML file is an S-100 ExchangeSet Catalog.
	static bool IsS100Catalog(std::wstring path);
	static int GetLevel(std::wstring path);
	static int GetScaleByLevel(int level);
};