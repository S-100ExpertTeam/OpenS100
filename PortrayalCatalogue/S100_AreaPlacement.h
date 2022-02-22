#pragma once
#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_AreaPlacement
{
public:
	S100_AreaPlacement();
	virtual ~S100_AreaPlacement();

private:
	std::wstring placementMode;

public:
	void GetContents(pugi::xml_node& node);
	void SetPlacementMode(std::wstring& value);
	std::wstring GetPlacementMode();
};