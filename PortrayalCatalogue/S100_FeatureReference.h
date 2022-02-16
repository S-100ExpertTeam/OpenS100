#pragma once
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class S100_FeatureReference
{
public:
	S100_FeatureReference();
	virtual ~S100_FeatureReference();

private:
	std::wstring reference;
public:
	void GetContents(pugi::xml_node node);

	void SetReference(std::wstring& value);
	std::wstring GetReference();
};