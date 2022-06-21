#pragma once

#include <map>

class FeatureCatalogue;
class PortrayalCatalogue;

class Product
{
public:
	Product();
	virtual ~Product();

public:
	// Key : catalogue version
	std::unordered_map<std::wstring, FeatureCatalogue*> fc;
	std::unordered_map<std::wstring, PortrayalCatalogue*> pc;

public:
	bool OpenFC(std::wstring path);
	bool OpenPC(std::wstring path);
};