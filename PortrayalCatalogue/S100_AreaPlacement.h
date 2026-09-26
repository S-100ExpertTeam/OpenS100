#pragma once
#include <pugixml.hpp>

#include <string>

class S100_AreaPlacement
{
public:
	S100_AreaPlacement();
	virtual ~S100_AreaPlacement();

private:
	std::string placementMode;

public:
	void GetContents(pugi::xml_node& node);
	void SetPlacementMode(const std::string& value);
	void SetPlacementMode(const std::wstring& value);
	std::string  GetPlacementMode();
	std::wstring GetPlacementModeW();
};
